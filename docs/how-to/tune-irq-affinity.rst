How to Tune IRQ Affinity
=========================

IRQ (Interrupt Request) affinity refers to assigning interrupt service routines 
(ISRs) to specific processor cores in a computer system. Having IRQ affinity
rightly set up becomes crucial for improved performance and responsiveness in a
real-time system. By tuning IRQ affinity, you dedicate specific interrupts to
particular CPU cores, preventing interruptions from being handled by multiple
cores simultaneously. The tuning enhances efficiency, reduces latency, and
ensures more predictable and consistent processing times for time-sensitive
tasks in real-time systems. Proper IRQ affinity configuration helps to decrease
contention, manage resource usage, and ultimately improve the overall 
responsiveness and reliability of the system, especially in scenarios where time
precision is critical.

The most common `kernel parameters`_ to handle IRQ interrupts are:
- `kthread_cpus`_ which define the CPUs that kernel threads are allowed to run on
- `isolcpus`_ used to specify CPUs to be isolated from the general `SMP`_ balancing and 
scheduler algorithms. 
The `IRQ affinity`_ on  other hand is a way to tune the system interruptions 
without modifying the kernel boot parameters. This is the focus of this document.
With IRQ affinity, you can set a default set of CPUs which are permitted to
handle incoming IRQs.

First check the interruptions sources in the system, all the IRQs are listed in 
the `/proc/interrupts` file. 

.. code-block:: console

    $ cat /proc/interrupts

Its useful to monitor the IRQs while tuning the affinity. You can use the 
`watch` command to monitor the IRQs in real-time:

.. code-block:: bash

    watch -n 1 cat /proc/interrupts


First you can list the isolated CPUs:

.. code-block:: bash

    cat /sys/devices/system/cpu/isolated

An empty output means that no core is isolated.

Then you can list all the available CPUs:

.. code-block:: console

    $ cat /sys/devices/system/cpu/present

    0-19

In this case, the system has 20 CPUs available.

The best way to tune a system is to isolate one or more CPUs to be used to run 
the real-time application and the others to handle the IRQs and kthreads.

First check which IRQ is being handled by the CPU, or sets of CPUs that you want
to isolate. This can do either manually checking:

.. code-block:: console

    $ cat /proc/irq/<IRQ-NUMBER>/smp_affinity

    ffffff

OBS: The `smp_affinity` file is a bitmask, where each bit represents a CPU, where
`1` means that the CPU is allowed to handle the IRQ and `0` means that the CPU
is not allowed to handle the IRQ. An output of `ffffff` means that all CPUs are
allowed to handle the IRQ.

Or you can use the :download:`check_irqs.sh` script to list the all the IRQs 
associated  with a given CPU:

.. code-block:: console

    $ ./check_irqs.sh 13

    IRQ 0 is associated with Core 13. Affinity Mask: fffff
    IRQ 1 is associated with Core 13. Affinity Mask: fffff
    IRQ 10 is associated with Core 13. Affinity Mask: fffff
    IRQ 11 is associated with Core 13. Affinity Mask: fffff
    IRQ 12 is associated with Core 13. Affinity Mask: fffff
    IRQ 120 is associated with Core 13. Affinity Mask: fffff
    IRQ 121 is associated with Core 13. Affinity Mask: fffff
    IRQ 13 is associated with Core 13. Affinity Mask: fffff
    IRQ 14 is associated with Core 13. Affinity Mask: fffff
    IRQ 141 is associated with Core 13. Affinity Mask: 02000
    IRQ 15 is associated with Core 13. Affinity Mask: fffff
    IRQ 150 is associated with Core 13. Affinity Mask: fffff
    IRQ 16 is associated with Core 13. Affinity Mask: fffff
    IRQ 164 is associated with Core 13. Affinity Mask: 02000
    IRQ 167 is associated with Core 13. Affinity Mask: fffff
    IRQ 17 is associated with Core 13. Affinity Mask: fffff
    IRQ 2 is associated with Core 13. Affinity Mask: fffff
    IRQ 3 is associated with Core 13. Affinity Mask: fffff
    IRQ 4 is associated with Core 13. Affinity Mask: fffff
    IRQ 5 is associated with Core 13. Affinity Mask: fffff
    IRQ 6 is associated with Core 13. Affinity Mask: fffff
    IRQ 7 is associated with Core 13. Affinity Mask: fffff
    IRQ 8 is associated with Core 13. Affinity Mask: fffff
    IRQ 9 is associated with Core 13. Affinity Mask: fffff

Then you can rewrite the `smp_affinity` file to set the IRQ to be handled by the
CPUs you want. Since kernel 3.0 it's possible to use the 
`/proc/irq/<IRQ-NUMBER>/smp_affinity_list`, based on the previous output, if you
want to set the IRQ 16 to be handled by the CPUs 0-12 and 14-19 (excluding the 
CPU 13), you can run:

.. code-block:: bash

    echo 0-12,14-19 > /proc/irq/0/smp_affinity_list

.. code-block:: console

    $ cat /proc/irq/0/smp_affinity_list

    fdfff

Then do this process for all the IRQs that are being handled by the CPUs that
you want to isolate.

.. warning::

    It's not allowed to turn off all CPUs for a given IRQ, meaning that you 
    should ensure every IRQ is handled by at least one CPU. In other words, the
    `smp_affinity` mask should never be 0.

Now you can run your real-time application in the isolated CPUs and check if
the IRQs are being handled by the CPUs that you want.

.. code-block:: bash

    taskset -c <CPU-NUM[s]> <COMMAND-TO-REAL-TIME-APP>

Or attaching to an already running process:

.. code-block:: bash

    taskset -pc <CPU_NUM[s]> <PID>


Then, you can check if th application is correctly running on the designated
CPU cores:

.. code-block:: bash

    ps -eo psr,tid,pid,comm,%cpu,priority,nice -T | grep <PID>


It's also important to disable the `irqbalance` service, which is responsible for
distributing IRQs across all available cores. To do so, you can run:

.. code-block:: bash

    systemctl disable irqbalance 
    systemctl stop irqbalance
    systemctl status irqbalance

Also, it's useful to keep the `systemd` services separated from the real-time
application. You can do this by setting the `CPUAffinity` parameter in the 
`/etc/systemd/system.conf` file to the cores you want to isolate. For example:

.. code-block:: console

    $ cat /etc/systemd/system.conf | grep CPUAffinity

    CPUAffinity=0,1

.. LINKS
.. _kernel parameters: https://docs.kernel.org/admin-guide/kernel-parameters.html
.. _kthread_cpus: https://www.kernel.org/doc/Documentation/sysctl/kernel.txt
.. _isolcpus: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cpu-partitioning/isolcpus
.. _irq_affinity: https://docs.kernel.org/core-api/irq/irq-affinity.html

.. This link has to be updated with the link for the reference of "boot parameters"
.. _boot parameters: https://www.kernel.org/doc/Documentation/admin-guide/kernel-parameters.rst 