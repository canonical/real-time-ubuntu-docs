.. _kernel-boot-parameters:

Kernel boot parameters
======================

This document lists several kernel boot parameters that are useful for tuning Real-time Ubuntu.

.. _reference-irqaffinity:

irqaffinity
-----------

The ``irqaffinity`` kernel parameter sets the default IRQ affinity mask, which defines the CPUs responsible for handling interrupt requests.
It requires a `cpu-list`_ formatted value.

For example, ``irqaffinity=0-3`` means that interrupts will be handled by CPUs 0 through 3.

The current default IRQ affinity mask can be checked in the ``/proc/irq/default_smp_affinity`` file.
The information in this file is presented in a hexadecimal representation of the binary CPU mask.

Note that ``irqaffinity`` is a `symmetric multiprocessing`_ (SMP) parameter, and therefore it does not apply to systems with only one CPU.

.. _reference-isolcpus:

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

.. _reference-kthread_cpus:

kthread_cpus
------------

The ``kthread_cpus`` parameter refers to the list of CPUs specifically allocated for kernel usage.

For instance, specifying ``kthread_cpus=0-3`` means that the kernel is allowed to run kernel threads on CPUs 0 through 3.

.. _reference-nohz:

nohz
----

The ``nohz`` parameter is used for enabling/disabling `dynamic ticks`_ at boot time.
Dynamic tick is a config option that enables the kernel to run without a regular time interrupt.
Enabling this parameter means kernel performs less work when idle and can potentially save power.

When ``nohz=on`` is set, it enables dynamic ticks, enabling the kernel to function without regular timer interrupts.

Read more on `kernel's command-line parameters`_ documentation.


.. LINKS

.. _kernel's command-line parameters: https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html
.. _cpu-list: https://www.kernel.org/doc/html/latest/admin-guide/kernel-parameters.html#cpu-lists
.. _symmetric multiprocessing: https://en.wikipedia.org/wiki/Symmetric_multiprocessing
.. _dynamic ticks: https://elinux.org/Kernel_Timer_Systems#Dynamic_ticks
