Kernel Boot Parameters
======================

This document lists several kernel boot parameters that are useful for tuning Real-time Ubuntu.

isolcpus
--------

The ``isolcpus`` parameter isolates CPUs from the general scheduler so that they only execute specific tasks
and have a limited number of kernel threads available for execution.

This parameter accepts two types of values: flag-list and `cpu-list`_.

When specifying a flag-list value, such as ``isolcpus=domain``,
CPUs are isolated from the general `symmetric multiprocessing`_ (SMP) balancing and scheduling algorithms.

Alternatively, ``isolcpus`` accepts cpu-list value, allowing for a list of CPUs to be isolated from the rest.
For example, ``isolcpus=1,2,10-20`` isolates CPUs 1, 2, and 10 through 20.

For further details, please refer to `kernel's command-line parameters`_ documentation.

nohz
----

The ``nohz`` parameter is used for enabling/disabling `dynamic ticks`_ at boot time.
Dynamic tick is a config option that enables the kernel to run without a regular time interrupt.
Enabling this parameter means kernel performs less work when idle and can potentially save power.

When ``nohz=on`` is set, it enables dynamic ticks, enabling the kernel to function without regular timer interrupts.

Read more on `kernel's command-line parameters`_ documentation.

kthread_cpus
------------

The ``kthread_cpus`` parameter refers to the list of CPUs specifically allocated for kernel usage.

For instance, specifying ``kthread_cpus=0-3`` means that the kernel is allowed to run kernel threads on CPUs 0 through 3.

timer_migration
---------------

The ``timer_migration`` parameter refers to an optimization technique to migrate timers from one CPU to another.
In a real-time system with multiple sockets, it is beneficial to disable this parameter to ensure that the timer will stay assigned to a core. 

By setting ``timer_migration=0`` in a multi socket machine, timer migration can be disabled.

sched_rt_runtime
----------------

The ``sched_rt_runtime`` parameter refers to the duration in microseconds during which a real-time process can dominate a CPU.

Specifying ``sched_rt_runtime=-1`` allows a process or real-time task to dominate the CPU indefinitely.

.. LINKS

.. _kernel's command-line parameters: https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html
.. _cpu-list: https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html#cpu-lists
.. _symmetric multiprocessing: https://en.wikipedia.org/wiki/Symmetric_multiprocessing
.. _dynamic ticks: https://elinux.org/Kernel_Timer_Systems#Dynamic_ticks
