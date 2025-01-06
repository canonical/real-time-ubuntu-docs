How to Download Real-Time Ubuntu Kernel Source Code
===================================================

The source code for Ubuntu's Real-Time kernel is available for download.
The process depends on the release of Ubuntu you are using.

=====
Setup
=====

.. TODO consider using synced tabs instead: https://sphinx-design.readthedocs.io/en/latest/tabs.html#synchronised-tabs
   This would allow for more distinct sections :shrug:

.. tab-set::

    .. tab-item:: LTS Release (22.04, 24.04, etc.)

        **1. Enable Ubuntu Pro**

        LTS Releases of Ubuntu require Ubuntu Pro to download their respective real-time kernel source code.

        Follow the following tutorial (TODO link) to ensure you have Ubuntu pro enabled.

        **2. Enable access to the Real-time Kernel**

        Enable access with the following command:

        .. code-block:: shell

            sudo pro enable realtime-kernel --access-only
        

        If you additionally want to install the real-time kernel to this system, remove the ``--access-only`` flag:

        .. code-block:: shell

            sudo pro enable realtime-kernel

        You can verify the realtime-kernel is activated with ``sudo pro status``.

        .. NOTE I don't think sudo pro status will tell you if you used the access-only flag or not.

        **3. Enable downloading source packages with apt**

        apt is used to download the real-time kernel source code. We need to enable :spellexception:`apt's` ability to download source packages (``deb-src``) from its archives.

        We can enable this by editing ``/etc/apt/sources.list.d/ubuntu-realtime-kernel.sources``. Use this command to do the work for you:

        .. code-block:: shell
            
            sudo sed -i 's/^Types: deb$/Types: deb deb-src/' /etc/apt/sources.list.d/ubuntu-realtime-kernel.sources


    .. tab-item:: Interim Releases (24.10 Oracular Oriole)

        Content 2

