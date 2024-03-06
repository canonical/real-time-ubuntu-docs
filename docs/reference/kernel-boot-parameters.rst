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

Further reading
---------------
For more details, please refer to the `linux kernel documentation`_.

.. LINKS

.. _linux kernel documentation: https://www.kernel.org/doc/html/latest/

