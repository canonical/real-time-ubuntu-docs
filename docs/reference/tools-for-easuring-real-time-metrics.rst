Tools for Measuring Real-time Metrics
=====================================

Various tools are available for measuring real-time metrics.
Here are some tools along with example commands.

.. note::
    Some of the tools aren't available on the OS, but can be installed via Debian packages.
    The package names are provided in the corresponding sections.

    To install a Debian package:
    
    .. code-block:: shell

        sudo apt update
        sudo apt install <package-name>


cyclictest
----------

The `cyclictest`_ is most commonly used for benchmarking real-time systems, 
and providing precise measurements such as actual latency and maximum latency.
It is included in the ``rt-tests`` Debian package.

.. code-block:: shell

    sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0

This command runs a test case that is relevant for most Symmetric Multiprocessing (SMP) real-time systems.

* ``--mlockall``: Locks current and future memory allocations to prevent being paged out.
* ``--smp``: Sets options for standard testing on SMP systems.
* ``--priority=80``: Sets the priority of the first thread to 80. 
  Each further thread gets a lower priority.
* ``--interval=200``: Sets the base interval of the thread(s) to 200 microseconds.
* ``--distance=0``: Sets the distance of thread intervals to 0 microseconds.

dstat
-----

The `dstat`_ is a versatile tool for generating system resource statistics.
The tool can be installed with the ``dstat`` Debian package.

.. code-block:: shell

    dstat --time --top-int

This command displays system resource statistics with timestamps and shows most frequent interrupts.

oslat
-------

The `oslat`_ program is designed to identify thread latency at the
OS level caused by unexpected system scheduling or interruptions such as system ticks.
It is included in the ``rt-tests`` Debian package.

.. code-block:: shell

    sudo oslat --duration 30s --cpu-list 0-3 --cpu-main-thread 0 --rtprio 95 --workload-mem 10M --workload memmove

This command runs the ``oslat`` test for 30 seconds on the listed CPUs (if none are specified, it runs on all cores),
with the given priority (specified by ``--rtprio``), using a 10M memory load, 
and then prints the results, which include latency measurements for each core.

ps
---

The `ps`_ command reports a snapshot of the current processes. 
It helps monitoring system resource usage.

.. code-block:: shell

   ps -A --format psr,tid,pid,comm,%cpu,priority,nice -T | sort --general-numeric-sort | grep irq

This command lists information about all processes, formatted to display processor (psr), 
thread ID (tid), process ID (pid), command name (comm), CPU utilization of the process in percentage format (%cpu), 
kernel scheduling priority, and `nice`_ value. 

The ``-T`` flag additionally shows the thread ID, possibly with SPID (unique number representing a dispatchable entity) column.

It then filters the output to display lines related to `interrupt requests`_ (IRQs).

perf
----

The `perf`_ is a performance analysis tool for Linux.
It collects and reports statistics about system-wide performance metrics
such as CPU migrations, CPU cycles, instructions, and branches.
It is included in the ``linux-tools-common`` Debian package.

.. code-block:: shell

   # Allow access to performance monitoring and observability operations
   $ sudo sysctl -w kernel.perf_event_paranoid=-1

   $ perf stat dd if=/dev/zero of=/dev/null count=1000000

This command reports performance counter statistics for the execution of ``dd if=/dev/zero of=/dev/null count=1000000``, 
which reads 1,000,000 null characters from `/dev/zero`_ and writes them to `/dev/null`_.

This specific command can be utilized for measuring latency during disk operations.

stress-ng
---------

The `stress-ng`_ is a tool to load and stress a computer system.
It was designed to exercise various physical subsystems of a computer 
as well as the various operating system kernel interfaces.
This tool may be installed with the ``stress-ng`` Debian package.

.. code-block:: shell

    sudo stress-ng --cpu 4 --vm 2 --hdd 1 --fork 8 --timeout 2m --metrics

This command runs 4 CPU, 2 virtual memory, 1 disk and 8 fork stressors for 2 minutes 
and prints measurements including the real time spent executing the stressor, 
numbers of bogus operations and CPU usage.

watch
-----

The `watch`_ command is useful for monitoring changes over time by executing a program periodically.
It monitors real-time observation of system activities such as interrupts or process status.

.. code-block:: shell

    watch --interval 1 cat /proc/interrupts

With the ``--interval`` flag set to 1, the command updates the output every 1 second, 
providing a continuous stream of information from the ``/proc/interrupts`` file.

.. LINKS

.. _cyclictest: https://man.archlinux.org/man/cyclictest.8.en
.. _dstat: https://manpages.ubuntu.com/manpages/jammy/man1/pcp-dstat.1.html
.. _oslat: https://manpages.ubuntu.com/manpages/jammy/man8/oslat.8.html
.. _ps: https://www.man7.org/linux/man-pages/man1/ps.1.html
.. _perf: https://www.man7.org/linux/man-pages/man1/perf.1.html
.. _stress-ng: https://manpages.ubuntu.com/manpages/mantic/en/man1/stress-ng.1.html
.. _watch: https://www.man7.org/linux/man-pages/man1/watch.1.html

.. _nice: https://www.man7.org/linux/man-pages/man1/nice.1.html
.. _interrupt requests: https://en.wikipedia.org/wiki/Interrupt_request
.. _/dev/zero: https://en.wikipedia.org/wiki//dev/zero
.. _/dev/null: https://en.wikipedia.org/wiki/Null_device


