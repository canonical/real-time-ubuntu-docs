Kernel Boot Parameters
======================

There are some kernel boot parameters can be used when tuning a real-time Linux system with PREEMPT_RT.

isolcpus
--------

The `isolcpus` parameter isolates CPUs from the general scheduler so that they only execute specific tasks 
and have a limited number of kernel threads available for execution.

When setting ``isolcpus=domain``, it isolates CPUs from the general SMP balancing and scheduling algorithms.

nohz
----

The `nohz` parameter is used for enabling/disabling dynamic ticks at boot time.

kthread_cpus
------------

The `kthread_cpus` parameter refers to the list of CPUs isolated for kernel usage.

For instance, specifying ``kthread_cpus=0-3`` means that the kernel is allowed to allocate between 0 and 3 kernel threads per CPU core.

timer_migration
---------------

The `timer_migration` parameter refers to an optimization technique to migrate timers from one CPU to another.
In a real-time system with multiple sockets, it is beneficial to disable this parameter to ensure that the time will stay assigned to a core. 

By setting ``timer_migration = 0`` in a multi socket machine, timer migration can be disabled.

sched_rt_runtime
----------------
The `sched_rt_runtime` parameter refers to the duration in microseconds during which a real-time process can dominate a CPU.

For instance, specifying ``sched_rt_runtime=-1`` allows a process or real-time task to dominate the CPU indefinitely.

Further reading
---------------
For more details, please refer to the `linux kernel documentation`_.

.. LINKS

.. _linux kernel documentation: https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html

