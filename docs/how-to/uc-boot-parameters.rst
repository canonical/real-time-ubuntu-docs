How to configure boot parameters on Ubuntu Core 
================================================

.. note::

    This guide assumes access to features available exclusively to `dedicated Snap Store`_ users.

Ubuntu Core doesn't have the :code:`/etc/default/grub` file mapped for the kernel parameters, neither the `update-grub`_ utility.
On Ubuntu Core, the kernel parameters can be modified statically or dynamically.
Static configurations are added during the image build.
On the other hand, dynamic configurations are set `using snap options`_ on a running system.
This guide focuses on the dynamic configuration method, suitable for iteratively tuning the operating system.

.. hint::
    
    Refer to :doc:`../how-to/uc-image-creation` if you want to create a real-time Ubuntu Core image.

Open a terminal and access your real-time Ubuntu Core instance.
The instance should be on a physical device rather than a virtual machine.
While a virtual machine can be used to follow the steps in this guide, it will not provide the benefits of a real-time kernel.

There are two snap options for setting the kernel parameters:

- `system.kernel.cmdline-append`_
- `system.kernel.dangerous-cmdline-append`_

The first one is used to append permitted kernel boot parameters that are verified against an `allow list`_ in the `gadget snap`_. 
The second one is used to append any other kernel boot parameters, considered as not analyzed by the gadget snap.

When using the reference gadget snaps (`pc-gadget`_ and `pi-gadget`_), you need to use the `system.kernel.dangerous-cmdline-append` option, as the allow list is not present in them.

When creating a custom `gadget snap`_ for a real-time Ubuntu Core system, it is advisable to add an allow list with the relevant parameters.

The kernel parameters in the snap configurations exactly match the `kernel parameters`_ that would be passed to the kernel on the grub command line, the :code:`GRUB_CMDLINE_LINUX_DEFAULT` on :code:`/etc/default/grub` file. 
Parameters are passed as a single string in the :code:`key=value` format, with each parameter separated by a space.

In the case of :doc:`real-time kernel parameters <../reference/kernel-boot-parameters>`, the most interesting parameters are `nohz`_, `nohz_full`_ and `irqaffinity`_.
The :code:`nohz` is used to enable/disable dynamic ticks (possible values are `on` or `off`). 
The :code:`nohz_full` receives a `cpu list`_ specifying which CPUs will have dynamic ticks disabled. 
The `irqaffinity` is used to set the affinity of the interrupt requests (IRQs) to the CPUs, in other words: which CPUs will handle the IRQs.

For example:

.. code-block:: bash
    
    snap set system system.kernel.dangerous-cmdline-append="nohz=on nohz_full=2-N irqaffinity=0-1"


After setting the kernel parameters, it's necessary to reboot the system to apply the changes.
Then you can check if the parameters were applied:

.. code-block:: console

    $ cat /proc/cmdline
    snapd_recovery_mode=run console=ttyS0,115200n8 console=tty1 panic=-1 nohz=on nohz_full=2-N irqaffinity=0-1


To undo the modifications made to the kernel parameters, use the `snap unset`_ command:

.. code-block:: bash

    snap unset system system.kernel.dangerous-cmdline-append

---

Once you are satisfied with the configurations, consider configuring them statically so that they are built into the image. 
Read more on the `image creation guide <../uc-image-creation#custom-ubuntu-core-image>`_.

.. Is there a better way to reference an anchor on another doc? 

.. LINKS
.. _Ubuntu Core: https://ubuntu.com/core
.. _update-grub: https://manpages.ubuntu.com/manpages/xenial/man8/update-grub.8.html
.. _snap set: https://ubuntu.com/core/docs/modify-kernel-options
.. _system.kernel.cmdline-append: https://snapcraft.io/docs/system-options#heading--kernel-cmdline-append
.. _system.kernel.dangerous-cmdline-append: https://snapcraft.io/docs/system-options#heading--kernel-dangerous-cmdline-append
.. _gadget snap: https://snapcraft.io/docs/the-gadget-snap
.. _allow list: https://snapcraft.io/docs/the-gadget-snap#heading--dynamic
.. _nohz: https://docs.kernel.org/timers/no_hz.html
.. _nohz_full: https://docs.kernel.org/timers/no_hz.html#omit-scheduling-clock-ticks-for-cpus-with-only-one-runnable-task
.. _kernel parameters: https://docs.kernel.org/admin-guide/kernel-parameters.html
.. _cpu list: https://docs.kernel.org/admin-guide/kernel-parameters.html#cpu-lists
.. _irqaffinity: https://docs.kernel.org/core-api/irq/irq-affinity.html
.. _snap unset: https://snapcraft.io/docs/set-system-options
.. _building a gadget snap: https://ubuntu.com/core/docs/gadget-building
.. _pc-gadget: https://github.com/snapcore/pc-gadget
.. _pi-gadget: https://github.com/snapcore/pi-gadget
.. _using snap options: https://snapcraft.io/docs/system-options
.. _dedicated Snap Store: https://ubuntu.com/core/docs/dedicated-snap-stores
