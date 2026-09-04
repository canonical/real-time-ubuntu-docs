.. meta::
   :description: Reduce latency on Real-time Ubuntu by tuning timer migration, power management, PSI, and measurement-tool CPU affinity.

How to reduce latency on Real-time Ubuntu
=========================================

After :doc:`isolating CPUs <cpu-boot-configs>` for a real-time workload, other
kernel and measurement-tool activity can still introduce latency. This guide
describes how to evaluate and tune:

* timer migration
* CPU idle and frequency scaling
* Pressure Stall Information (PSI)
* the placement of measurement-tool control threads

The examples use CPUs 0 and 1 for housekeeping and CPUs 2 through 7 for the
real-time workload. Adjust every CPU list for your system.

.. warning::

   These settings trade power efficiency, thermal headroom, observability, or
   general-purpose performance for lower latency. Change one setting at a time
   and measure it under a representative workload before deploying it.

Install the measurement tools
-----------------------------

Install ``cyclictest`` and ``trace-cmd``:

.. code-block:: shell

   sudo apt update
   sudo apt install rt-tests trace-cmd linux-tools-$(uname -r)

The ``rtla`` tool is available by default on Ubuntu 24.04 LTS and later.
On custom kernels, install the matching ``linux-tools`` package for the
running kernel; otherwise ``rtla`` and ``cpupower`` can report
"not found for kernel ...".

Establish a baseline
--------------------

Use ``rtla timerlat`` to measure timer IRQ and thread latency before changing
the system:

.. code-block:: shell

   sudo rtla timerlat top -q -u \
     -c 2-7 -H 0,1 \
     -d 1m \
     -p 200 -P f:99 \
     -T 30 -s 20 \
     --dma-latency 0 \
     --dump-tasks \
     -t=rtla.2-7.trace

This command:

* runs timerlat user-space measurement threads on CPUs 2 through 7
* keeps the ``rtla`` control threads on housekeeping CPUs 0 and 1
* uses a 200-microsecond period and ``SCHED_FIFO`` priority 99
* stops and saves the trace when thread latency exceeds 30 microseconds
* saves a stack trace when thread latency exceeds 20 microseconds
* requests a zero-microsecond DMA latency to constrain CPU idle states

Repeat the same command after each tuning change. Compare distributions and
maximum values across multiple runs rather than relying on one outlier. You
also can narrow the '-T'/'-s' parameter to shorten latency trigger catching.

Disable timer migration
-----------------------

When ``timer_migration`` is enabled, the kernel can move timers away from idle
CPUs to keep more CPUs in full-dynticks (``nohz_full``) mode for longer
periods. On an isolated real-time CPU, servicing the migrated timer can require
an inter-processor interrupt (IPI). If the housekeeping and real-time CPUs are
in different CPU clusters, the IPI can add significant latency.

Check whether the sysctl is available and enabled:

.. code-block:: shell

   sysctl kernel.timer_migration

Disable timer migration temporarily:

.. code-block:: shell

   sudo sysctl -w kernel.timer_migration=0

Despite CPU isolation being configured with a CPU list, ``timer_migration`` is
a system-wide setting; it can't be disabled for individual real-time CPUs.
Restore the default behavior with:

.. code-block:: shell

   sudo sysctl -w kernel.timer_migration=1

Trace timer migration latency
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Record scheduler, timer, IRQ, and IPI events while ``cyclictest`` runs. The
``--breaktrace`` threshold stops tracing when latency exceeds 35 microseconds,
and ``--tracemark`` adds a marker at that point:

.. code-block:: shell

   sudo trace-cmd record -q \
     -o /tmp/timer-migration.dat \
     -e sched:sched_switch \
     -e sched:sched_wakeup \
     -e irq:irq_handler_entry \
     -e irq:softirq_entry \
     -e timer:hrtimer_expire_entry \
     -e ipi:ipi_raise \
     -e ipi:ipi_entry \
     -e ipi:ipi_exit \
     cyclictest \
       --mainaffinity 0 \
       --affinity=2-7 \
       --threads=6 \
       --mlockall \
       --priority=99 \
       --interval=200 \
       --breaktrace=35 \
       --tracemark \
       --quiet

Press :kbd:`Ctrl+C` after capturing a latency spike, then render the trace:

.. code-block:: shell

   sudo trace-cmd report -i /tmp/timer-migration.dat \
     > /tmp/timer-migration.report

Inspect the events around the trace marker and look for a timer expiry followed
by an IPI between a housekeeping CPU and a real-time CPU:

.. code-block:: shell

   grep -nE 'tracing_mark_write|hrtimer_expire_entry|ipi_(raise|entry|exit)' \
     /tmp/timer-migration.report

Compare traces with ``kernel.timer_migration`` set to ``1`` and ``0``. The
cost is topology-dependent; cross-cluster IPIs can be substantially more
expensive than IPIs within a CPU cluster.

Limit power-management latency
------------------------------

Waking a CPU from a deep idle state can take microseconds or longer. Frequency
scaling can also increase the time required to complete a workload.

To disable the CPU idle and CPU frequency subsystems system-wide, add these
:doc:`kernel command-line parameters <modify-kernel-boot-parameters>`:

.. code-block:: text

   cpuidle.off=1 cpufreq.off=1

Reboot and verify that they appear in ``/proc/cmdline``.

Disabling both subsystems is the most aggressive option and increases power
consumption. As a less disruptive alternative, request the performance
governor:

.. code-block:: shell

   sudo apt install linux-tools-$(uname -r)
   sudo cpupower frequency-set --governor performance

Use ``cpupower frequency-info`` to verify which scaling driver and governors
the hardware supports. The driver, firmware, or platform can limit whether the
requested governor fixes the clock at its maximum frequency.
If you booted with ``cpufreq.off=1``, CPU frequency scaling is disabled and
``cpupower frequency-set`` will fail by design.

For a measurement-only change, ``rtla timerlat --dma-latency 0`` requests a
latency constraint while the tool runs. ``cyclictest`` makes the same request
by default. These options help distinguish idle-state exit latency without
permanently disabling CPU idle support.

Disable PSI
-----------

PSI tracks CPU, memory, and I/O resource pressure. Its scheduler accounting is
useful for monitoring overloaded systems, but it adds work to scheduler paths.
If tracing shows PSI accounting in a latency-critical wakeup path, add this
:doc:`kernel command-line parameter <modify-kernel-boot-parameters>`:

.. code-block:: text

   psi=0

Reboot, then verify the setting:

.. code-block:: shell

   cat /proc/pressure/cpu

If PSI is disabled, files under ``/proc/pressure`` can be unavailable.
Disabling PSI removes pressure metrics used by monitoring and
resource-management software.

On one test system, function tracing attributed about 10 microseconds of a
``cyclictest`` wakeup to PSI-related scheduler work. Disabling PSI reduced the
observed maximum from 43 microseconds to 32 microseconds. These values describe
that system and workload; measure the effect on the target hardware.

Trace the architecture timer path
---------------------------------

Use the function graph tracer when a timerlat trace points to architecture
timer or hrtimer processing but doesn't identify the expensive function.
The following example traces CPU 2; its hexadecimal CPU mask is ``4``.

.. warning::

   Ftrace configuration is global. Don't run this procedure while another
   process is using ftrace.

Run these commands as root:

.. code-block:: bash

   T=/sys/kernel/tracing
   [ -d "$T" ] || T=/sys/kernel/debug/tracing

   RUN=cpu2_fgraph_cyclic_$(date +%Y%m%d_%H%M%S)
   FGRAPH=/tmp/${RUN}.trace
   CYCLIC_OUT=/tmp/${RUN}.cyclic.out

   echo 0 > "$T/tracing_on"
   echo 0 > "$T/events/enable"
   echo nop > "$T/current_tracer"
   : > "$T/trace"
   echo 16384 > "$T/buffer_size_kb"
   echo 4 > "$T/tracing_cpumask"
   echo function_graph > "$T/current_tracer"
   : > "$T/set_graph_function"

   for f in \
     arch_timer_handler_virt \
     arch_timer_handler_phys \
     hrtimer_interrupt \
     __hrtimer_run_queues \
     clockevents_program_event \
     scheduler_tick \
     handle_percpu_devid_irq \
     handle_percpu_irq \
     irq_exit_rcu \
     irq_exit
   do
     sudo grep -qw "$f" "$T/available_filter_functions" &&
       echo "$f" >> "$T/set_graph_function"
   done

   for opt in \
     funcgraph-duration \
     funcgraph-proc \
     funcgraph-cpu \
     funcgraph-overhead
   do
     [ -e "$T/options/$opt" ] && echo 1 > "$T/options/$opt"
   done

   echo 20 > "$T/max_graph_depth"

   for e in \
     events/irq/irq_handler_entry/enable \
     events/irq/irq_handler_exit/enable \
     events/timer/hrtimer_expire_entry/enable \
     events/timer/hrtimer_expire_exit/enable \
     events/timer/hrtimer_start/enable \
     events/sched/sched_waking/enable \
     events/sched/sched_wakeup/enable \
     events/sched/sched_switch/enable
   do
     [ -e "$T/$e" ] && echo 1 > "$T/$e"
   done

   echo 1 > "$T/tracing_on"

   cyclictest \
     --mainaffinity 0 \
     --affinity=2 \
     --threads=1 \
     --loops=5000 \
     --mlockall \
     --priority=99 \
     --interval=200 \
     --histogram=400 \
     --quiet \
     > "$CYCLIC_OUT" 2>&1

   echo 0 > "$T/tracing_on"
   cat "$T/trace" > "$FGRAPH"

The available architecture timer functions vary by architecture and kernel
version. The loop adds only functions exposed in
``available_filter_functions``.

List the longest relevant calls:

.. code-block:: bash

   F="$FGRAPH"

   awk '
     /[+#! ]*[0-9]+\.[0-9]+ us[[:space:]]+\|/ {
       line=$0
       if (match(line, /[0-9]+\.[0-9]+ us/)) {
         value=substr(line, RSTART, RLENGTH)
         sub(/ us/, "", value)
         duration=value+0
         if (line ~ /arch_timer|hrtimer_interrupt|__hrtimer_run_queues|clockevents_program_event|scheduler_tick|handle_percpu|irq_exit/) {
           printf "%10.3f %s\n", duration, line
         }
       }
     }
   ' "$F" | sort -nr | head -80

Inspect the timer expiry sequence:

.. code-block:: shell

   grep -nE 'arch_timer|hrtimer_interrupt|__hrtimer_run_queues|clockevents_program_event|irq_exit' "$F" | tail -60

To inspect timer-expiry events emitted by tracepoints, query the trace-cmd
report instead:

.. code-block:: shell

   grep -n 'hrtimer_expire_entry' /tmp/timer-migration.report | tail -60

Tracing changes the workload and adds overhead. Use it to identify candidate
paths, then disable tracing and repeat the latency measurement to quantify each
tuning change.

Keep measurement control threads on housekeeping CPUs
-----------------------------------------------------

``cyclictest`` creates a main control thread in addition to its measurement
threads. If the main thread runs on an isolated real-time CPU, its background
work can interfere with measurements and add scheduler latency.

Pin the main thread to a housekeeping CPU independently of the measurement
threads:

.. code-block:: shell

   sudo cyclictest \
     --mainaffinity 0 \
     --affinity=2-7 \
     --threads=6 \
     --mlockall \
     --priority=99 \
     --interval=200 \
     --duration=1m

For ``rtla``, use ``--house-keeping`` (``-H``) for the same purpose:

.. code-block:: shell

   sudo rtla timerlat top -c 2-7 -H 0,1 -d 1m

Confirm thread placement during the run with:

.. code-block:: shell

   ps -eLo pid,tid,psr,cls,rtprio,comm |
     grep -E 'cyclictest|timerlat|rtla'

.. LINKS
.. _timer migration: https://docs.kernel.org/admin-guide/sysctl/kernel.html#timer-migration
.. _PSI: https://docs.kernel.org/accounting/psi.html
