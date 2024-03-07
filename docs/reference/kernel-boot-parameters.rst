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

Add kernel boot parameters
--------------------------
Follow these steps to configure these boot parameters:

1. Edit ``/etc/default/grub`` file to append boot parameters to the ``GRUB_CMDLINE_LINUX``.

2. Run the following commands to update the grub configuration file and reboot the system:

.. code-block:: shell

    sudo update-grub
    sudo reboot

3. After rebooting, verify whether the boot parameter has been successfully added by running:

.. code-block:: shell

    cat /proc/cmdline

Further reading
---------------
For more details, please refer to the `linux kernel documentation`_.

.. LINKS

.. _linux kernel documentation: https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html

