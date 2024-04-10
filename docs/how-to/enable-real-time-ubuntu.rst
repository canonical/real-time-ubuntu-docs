How to enable Real-time Ubuntu
==============================

Prerequisites
-------------

Currently, `Real-time Ubuntu`_ is supported only on Ubuntu 22.04 LTS ("Jammy
Jellyfish"). `Contact the real-time team`_ for further information.

Attach an Ubuntu Pro token
--------------------------

Real-time Ubuntu is available through an `Ubuntu Pro`_ security and compliance
subscription. If you don't already have access to "Pro", it's free for personal
use.

.. important:: 

   Once you enable Real-time Ubuntu, it won't be possible to enable certain
   other Pro services. The external documentation for Pro Client describes which
   services are `compatible with Real-time Ubuntu`_.

   Real-time Ubuntu is not compatible with Livepatch. If Livepatch is enabled
   when you install Real-time Ubuntu, Pro will offer to disable it before
   continuing.

If no token is visible on your Ubuntu Pro dashboard, click the Free Personal
Token section and it should appear. Attach your token, so the target computer
can access Pro services:

.. code-block:: shell

   sudo pro attach <token>

Install and enable automatically
--------------------------------

Real-time Ubuntu is installed using the APT package manager. If you wish to
access the repository but not install the package immediately, skip to `Install
and enable manually`_.

Otherwise, install Real-time Ubuntu and automatically select the right version
for your OS and processor:

.. code-block:: shell

   sudo pro enable realtime-kernel

You'll need to acknowledge a warning, then you should see confirmation that the
Real-time Ubuntu package is installed:

.. code-block:: text

   One moment, checking your subscription first
   Real-time Ubuntu is a beta version of the 22.04 Ubuntu kernel with the
   PREEMPT_RT patchset integrated for x86_64 and ARM64.

   This will change your kernel. You will need to manually configure grub to
   revert back to your original kernel after enabling real-time.

   Do you want to continue? [ default = Yes ]: (Y/n) yes
   Updating package lists
   Installing Real-time Ubuntu packages
   Real-time Ubuntu enabled
   A reboot is required to complete install.

After rebooting, you'll be running Real-time Ubuntu.

Install and enable manually
---------------------------

To access the Real-time Ubuntu repository but not install the package
immediately, first use the ``--access-only`` flag:

.. code-block:: shell-session

   $ sudo pro enable realtime-kernel --access-only

   One moment, checking your subscription first
   Updating package lists
   Skipping installing packages: ubuntu-realtime
   Real-time Ubuntu access enabled

.. important::

   The ``--access-only`` flag was introduced in Pro Client version 27.11.

Now that Real-time Ubuntu is accessible, you can install and enable it whenever
you wish:

.. code-block:: shell

   sudo apt install ubuntu-realtime

After rebooting, you'll be running Real-time Ubuntu.


.. LINKS
.. _Real-time Ubuntu: https://ubuntu.com/real-time
.. _contact the real-time team: https://ubuntu.com/kernel/real-time/contact-us
.. _Ubuntu Pro: https://ubuntu.com/pro
.. _compatible with Real-time Ubuntu: https://canonical-ubuntu-pro-client.readthedocs-hosted.com/en/latest/references/compatibility_matrix/
