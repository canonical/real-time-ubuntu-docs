.. meta::
   :description: Step-by-step guides for key operations in Real-time Ubuntu. Learn to install the kernel, configure CPUs, measure latency, and create Ubuntu Core images.

How-to guides
=============

These guides walk you through key operations and processes of Real-time Ubuntu.

Get started with Real-time Ubuntu
---------------------------------

Install the real-time kernel, and revert the change if needed.
You can also run Real-time Ubuntu inside a KVM virtual machine for evaluation.

.. toctree::
   :maxdepth: 1

   Enable Real-time Ubuntu <enable-real-time-ubuntu>
   Switch from real-time to generic kernel <switch-from-realtime-to-generic-kernel>
   Create a Real-time Ubuntu VM using KVM <create-rt-ubuntu-vm-using-kvm>

Measure system performance
--------------------------

Before tuning, establish a baseline latency measurement so you can quantify the effect of any changes you make to the real-time kernel.

.. toctree::
   :maxdepth: 1

   Measure maximum latency <measure-maximum-latency>

Tune for real-time processing
-----------------------------

These guides help you reduce and bound latency by isolating workloads, routing interrupts away from critical CPUs, and adjusting kernel boot parameters.
More details of tunable parameters are available in :doc:`/reference/kernel-boot-parameters` and :doc:`/reference/kernel-config-options`.

.. toctree::
   :maxdepth: 1

   Modify kernel boot parameters <modify-kernel-boot-parameters>
   Configure CPUs for real-time processing <cpu-boot-configs>
   Tune IRQ affinity <tune-irq-affinity>
   Isolate CPUs from general execution with cpusets <isolate-workload-cpusets>

Configure Real-time Ubuntu Core
-------------------------------

.. toctree::
   :maxdepth: 1

   Create a Real-time Ubuntu Core image <uc-image-creation>
   Install real-time kernel on Ubuntu Core <uc-install-real-time-kernel>

For tuning an existing Ubuntu Core system, refer to :ref:`ubuntu-core-kernel-cmdline`.

Access source code
------------------

.. toctree::
   :maxdepth: 1

   Download Real-time Ubuntu kernel source code <download-real-time-sources>

.. LINKS
.. _Ubuntu Core: https://ubuntu.com/core
