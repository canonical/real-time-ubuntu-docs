Kernel Boot Parameters
======================

This document lists several kernel boot parameters that are useful for tuning Real-time Ubuntu.

isolcpus
--------

The `isolcpus` parameter isolates CPUs from the general scheduler so that they only execute specific tasks
and have a limited number of kernel threads available for execution.
For further details, please refer the documentation on  `kernel's command-line parameters`_.

`isolcpus` accepts two types of values: flag-list and `cpu-list`_.
When setting a flag-list value (``isolcpus=domain``)
CPUs are isolated from the general SMP balancing and scheduling algorithms.

Alternatively, when setting a cpu-list value (``isolcpus=1,2,10-20``),
CPUs specified in the list (CPUs 1, 2, and 10 through 20) are isolated from the rest.

nohz
----

The `nohz` parameter is used for enabling/disabling dynamic ticks at boot time. 
For further details, please refer the documentation on  `kernel's command-line parameters`_.

When ``isolcpus=nohz`` is set, it disables the tick when a single task runs. 
When ``nohz=on`` is set, it disables dynamic ticks during boot time.

kthread_cpus
------------

The `kthread_cpus` parameter refers to the list of CPUs specifically allocated for kernel usage.

For instance, specifying ``kthread_cpus=0-3`` means that the kernel is allowed to allocate between 0 and 3 kernel threads per CPU core.

timer_migration
---------------

The `timer_migration` parameter refers to an optimization technique to migrate timers from one CPU to another.
In a real-time system with multiple sockets, it is beneficial to disable this parameter to ensure that the time will stay assigned to a core. 

By setting ``timer_migration=0`` in a multi socket machine, timer migration can be disabled.

sched_rt_runtime
----------------

The `sched_rt_runtime` parameter refers to the duration in microseconds during which a real-time process can dominate a CPU.

Specifying ``sched_rt_runtime=-1`` allows a process or real-time task to dominate the CPU indefinitely.

.. LINKS

.. _kernel's command-line parameters: https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html
.. _cpu-list: https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html#cpu-lists
