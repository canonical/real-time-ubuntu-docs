How to modify kernel boot parameters
=======================================

The real-time performance-tuning features compiled into the kernel can be
controlled by various means. The approach used here is a common one:
passing command line parameters via a bootloader.

The Linux kernel supports a wide range of parameters, but only a few of them are relevant to real-time tuning.
The `kernel boot parameters <../../reference/kernel-boot-parameters>`_ page is good a reference.

Depending on the underlying boot mechanism, there are multiple ways to pass command line parameters to the kernel.

For Raspberry Pi, jump to `modify kernel boot parameters on Raspberry Pi <#modify-kernel-boot-parameters-on-raspberry-pi>`_.

Ubuntu Core uses GRUB but provides a different interface for 
`modifying kernel options <https://ubuntu.com/core/docs/modify-kernel-options>`_.

.. Replace the Ubuntu Core link to the internal how-to (https://documentation.ubuntu.com/real-time/en/ubuntu-core-devel/how-to/uc-boot-parameters/), once released.

Continue reading for systems that use the GNU GRUB bootloader.

Modify kernel boot parameters via GRUB
--------------------------------------

You can temporarily or permanently configure parameters which GRUB will pass to
Real-time Ubuntu.

When first testing a real-time tuning feature, `configure it temporarily
<#configure-grub-temporarily>`_. Temporary bootloader parameters are passed to
the kernel on a single startup. To re-use them you'll need to re-enter them ---
they don't persist across reboots.

When you're satisfied that a parameter achieves what you want, you can make it
permanent by `updating GRUB's configuration <#configure-grub-permanently>`_
then rebooting.

Configure GRUB temporarily
~~~~~~~~~~~~~~~~~~~~~~~~~~

Reboot, and interrupt GRUB's startup sequence at its bootloader menu.

Highlight the entry --- usually **Ubuntu** --- that would boot Real-time
Ubuntu.

.. image:: grub-menu.png
   :alt: GRUB menu
   :width: 100%
   :align: center

Press **e** to edit its boot parameters.

Append parameters to the line that starts with ``linux``:

.. image:: grub-edit.png
   :alt: GRUB edit
   :width: 100%
   :align: center

* Leave a space before each new parameter

* Don't add space round ``=`` and other punctuations for each key-value

* Don't add line breaks

Press :kbd:`Ctrl+X` to exit the editor and continue booting with the new
parameters.

Configure GRUB permanently
~~~~~~~~~~~~~~~~~~~~~~~~~~

Once your device has booted, launch use a text editor to edit ``/etc/default/grub``.

Append parameters to the line that starts with ``GRUB_CMDLINE_LINUX``:

* Enter parameters *inside* the double-quotes

* Leave a space after each new parameter

* Don't add space round ``=`` and other punctuations for each key-value

* Don't add line breaks

For example, after adding ``nohz=on`` and ``nohz_full=1-3``:

.. code-block:: ini

   GRUB_CMDLINE_LINUX="nohz=on nohz_full=1-3"

Save and close the editor.

Update GRUB with its new configuration:

.. code-block:: shell

   sudo update-grub

Reboot.

Modify kernel boot parameters on Raspberry Pi
---------------------------------------------

On a Raspberry Pi running Ubuntu Server / Desktop, the kernel command line arguments are found under
`/boot/firmware/cmdline.txt <https://www.raspberrypi.com/documentation/computers/configuration.html#kernel-command-line-cmdline-txt>`_.

For example, on Ubuntu 24.04 with the real-time kernel:

.. code-block:: shell-session

   $ cat /boot/firmware/cmdline.txt 
   console=serial0,115200 multipath=off dwc_otg.lpm_enable=0 console=tty1 root=LABEL=writable rootfstype=ext4 rootwait fixrtc

Use a text editor to edit this file ``/boot/firmware/cmdline.txt``.

Append parameters to the line:

* Leave a space after each new parameter

* Don't add space round ``=`` and other punctuations for each key-value

* Don't add line breaks

For example, after adding ``nohz=on`` and ``nohz_full=1-3`` to the above example:

.. code-block:: shell-session

   $ cat /boot/firmware/cmdline.txt
   console=serial0,115200 multipath=off dwc_otg.lpm_enable=0 console=tty1 root=LABEL=writable rootfstype=ext4 rootwait fixrtc nohz=on nohz_full=1-3

Save and close the editor.

Reboot.

Verify the parameters passed to the kernel at boot time:

.. code-block:: shell-session

   $ cat /proc/cmdline
   reboot=w coherent_pool=1M 8250.nr_uarts=1 pci=pcie_bus_safe snd_bcm2835.enable_compat_alsa=0 snd_bcm2835.enable_hdmi=1  smsc95xx.macaddr=D8:3A:DD:E4:0B:D3 vc_mem.mem_base=0x3fc00000 vc_mem.mem_size=0x40000000  console=ttyAMA10,115200 multipath=off dwc_otg.lpm_enable=0 console=tty1 root=LABEL=writable rootfstype=ext4 rootwait fixrtc nohz=on nohz_full=1-3
