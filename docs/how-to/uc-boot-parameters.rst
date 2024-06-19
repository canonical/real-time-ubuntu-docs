How to Configure Boot parameters on Ubuntu Core 
================================================

Different from classic versions of Ubuntu (Desktop and Server), `Ubuntu Core`_ is a minimal, secure and strictly
confined operating system designed to run on embedded systems, such as IoT devices.
Ubuntu Core is a snap-based operating system, meaning it uses snap packages to manage applications and services.
Its read-only nature makes it a very secure and reliable operating system but also makes it a bit more difficult to
configure certain aspects of the system, such as boot parameters.


Configure boot parameters
-------------------------

As UC doesn't have the file :code:`/etc/default/grub` mapped for the kernel parameters, neither the `update-grub`_
utility, it's necessary to use the `snap set`_. There are two ways to set the kernel parameters:

- `system system.kernel.cmdline-append`_
- `system system.kernel.dangerous-cmdline-append`_

The first one is used to dynamically append permitted kernel boot parameters that are verified against an `allow list`_
in the `gadget snap`_, while the second one is used to append kernel boot parameters considered risky according to that
specific gadget snap. In the case that you're interested in using the :code:`pc-gadget` or the :code:`pi-gadget`, you
should use the `system system.kernel.dangerous-cmdline-append` option, since the allow list isn't present on those 
gadget snaps, but if you want to `build a gadget snap`_ yourself, you can implement it.

The kernel parameters in the snap configurations exactly match the `kernel parameters`_ that would be passed to the 
kernel on the grub command line, the :code:`GRUB_CMDLINE_LINUX_DEFAULT` on :code:`/etc/default/grub` file. 
Parameters are passed as a single string in the :code:`key=value` format, with each parameter separated by a space.

In the case of real-time kernel parameters, the most interesting parameters are `nohz`_, `nohz_full`_ and `irqaffinity`_.
The :code:`nohz` is used to enable/disable dynamic ticks (possible values are `on` or `off`), the :code:`nohz_full` 
receives a `cpu list`_ where you say in which CPUs the dynamic ticks will be disabled, and the `irqaffinity` is used to 
set the affinity of the IRQs to the CPUs, in other words: which CPUs will handle the IRQs.

.. code-block:: bash
    
    snap set system system.kernel.dangerous-cmdline-append="nohz=on nohz_full=2-N irqaffinity=0-1"


After setting the kernel parameters, it's necessary to reboot the system to apply the changes.
Then you can check if the parameters were applied:

.. code-block:: console

    $ cat /proc/cmdline
    snapd_recovery_mode=run console=ttyS0,115200n8 console=tty1 panic=-1 nohz=on nohz_full=2-N irqaffinity=0-1


You may wish to undo the modifications made to the kernel parameters. To do so, you can use the `snap unset`_ command:

.. code-block:: bash

    snap unset system system.kernel.dangerous-cmdline-append


.. LINKS
.. _Ubuntu Core: https://ubuntu.com/core
.. _update-grub: https://manpages.ubuntu.com/manpages/xenial/man8/update-grub.8.html
.. _snap set: https://ubuntu.com/core/docs/modify-kernel-options
.. _system system.kernel.cmdline-append: https://snapcraft.io/docs/system-options#heading--kernel-cmdline-append
.. _system system.kernel.dangerous-cmdline-append: https://snapcraft.io/docs/system-options#heading--kernel-dangerous-cmdline-append
.. _gadget snap: https://snapcraft.io/docs/gadget-snap
.. _allow list: https://snapcraft.io/docs/the-gadget-snap#heading--dynamic
.. _nohz: https://docs.kernel.org/timers/no_hz.html
.. _nohz_full: https://docs.kernel.org/timers/no_hz.html#omit-scheduling-clock-ticks-for-cpus-with-only-one-runnable-task
.. _kernel parameters: https://docs.kernel.org/admin-guide/kernel-parameters.html
.. _cpu list: https://docs.kernel.org/admin-guide/kernel-parameters.html#cpu-lists
.. _irqaffinity: https://docs.kernel.org/core-api/irq/irq-affinity.html
.. _snap unset: https://snapcraft.io/docs/set-system-options
.. _build a gadget snap: https://ubuntu.com/core/docs/gadget-building