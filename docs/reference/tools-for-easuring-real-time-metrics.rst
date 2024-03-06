Tools for Measuring Real-time Metrics
=====================================

Various tools are available for measuring real-time metrics.
Here are some tools along with example commands:

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
such as CPU migrations, CPU cycles, instructions, and branches .

.. code-block:: shell

   perf stat dd if=/dev/zero of=/dev/null count=1000000

This command reports performance counter stats for the execution of ``dd  if=/dev/zero of=/dev/null count=1000000`` 
which copies 1 million blocks from `/dev/zero`_ to `/dev/null`_.

stress-ng
---------

The `stress-ng`_ is a tool to load and stress a computer system.
It was designed to exercise various physical subsystems of a computer 
as well as the various operating system kernel interfaces. 

.. code-block:: shell
    
   stress-ng --cpu 4 --vm 2 --hdd 1 --fork 8 --timeout 2m --metrics

This command runs 4 CPU, 2 virtual memory, 1 disk and 8 fork stressors for 2 minutes 
and prints measurements including the real time spent executing the stressor, 
numbers of bogus operations and CPU usage.

cyclictest
----------

The `cyclictest`_ is most commonly used for benchmarking real-time systems, 
and providing precise measurements such as actual latency and maximum latency.

.. code-block:: shell

    cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0

This command runs a test case that is relevant for most Symmetric Multiprocessing (SMP) real-time systems.

* ``--mlockall``: Locks current and future memory allocations to prevent being paged out.
* ``--smp``: Sets options for standard testing on SMP systems.
* ``--priority=80``: Set the priority of the first thread to 80. 
  Each further thread gets a lower priority.
* ``--interval=200``: Sets the base interval of the thread(s) to 200 microseconds.
* ``--distance=0``: Sets the distance of thread intervals to 0 microseconds.

dstat
-----

The `dstat`_ is a versatile tool for generating system resource statistics.

.. code-block:: shell

    dstat --time --top-int

This command displays system resource statistics with timestamps and shows most frequent interrupts.

watch
-----

The `watch`_ command is useful for monitoring changes over time by executing a program periodically.
It monitors real-time observation of system activities such as interrupts or process status.

.. code-block:: shell

    watch --interval 1 cat /proc/interrupts

With the ``--interval`` flag set to 1, the command updates the output every 1 second, 
providing a continuous stream of information from the ``/proc/interrupts`` file.

.. LINKS

.. _ps: https://www.man7.org/linux/man-pages/man1/ps.1.html
.. _perf: https://www.man7.org/linux/man-pages/man1/perf.1.html
.. _stress-ng: https://manpages.ubuntu.com/manpages/mantic/en/man1/stress-ng.1.html
.. _cyclictest: https://man.archlinux.org/man/cyclictest.8.en
.. _dstat: https://linuxcommandlibrary.com/man/dstat
.. _watch: https://www.man7.org/linux/man-pages/man1/watch.1.html

.. _nice: https://www.man7.org/linux/man-pages/man1/nice.1.html
.. _interrupt requests: https://en.wikipedia.org/wiki/Interrupt_request
.. _/dev/zero: https://en.wikipedia.org/wiki//dev/zero
.. _/dev/null: https://en.wikipedia.org/wiki/Null_device


