How to download Real-time Ubuntu kernel source code
===================================================

The source code for Ubuntu's real-time kernel is available for download.
The process depends on the release of Ubuntu you are using.

.. tab-set::

    .. tab-item:: LTS Releases (22.04, 24.04, etc.)

        **1. Enable Ubuntu Pro**

        LTS Releases of Ubuntu require Ubuntu Pro to download their respective real-time kernel source code.

        See the `Ubuntu Pro documentation <https://documentation.ubuntu.com/pro/>`_ for more information.

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

        **4. Download the Real-Time Kernel Source Code**

        First, apt needs to be refreshed to index newly-packaged repositories:

        .. code-block:: shell

            sudo apt update
            
        apt can now see the source code packages. Run the following commands to download them:

        .. code-block:: shell

            sudo apt install dpkg-dev
            sudo apt-get source --only-source linux-realtime

        .. tip:: 

            Ensure you have enough disk space: The downloaded and extracted packages take almost 2GB of storage.

        At this point, the unpacked source files are in your current directory!



    .. tab-item:: Interim Releases (24.10, etc.)

        **1. Enable downloading source packages with apt**

        apt is used to download the real-time kernel source code. We need to enable :spellexception:`apt's` ability to download source packages (``deb-src``) from its archives.

        We can enable this by editing ``/etc/apt/sources.list.d/ubuntu.sources``. Use this command to do the work for you:

        .. code-block:: shell
            
            sudo sed -i 's/^Types: deb$/Types: deb deb-src/' /etc/apt/sources.list.d/ubuntu.sources

        **2. Download the Real-Time Kernel Source Code**

        First, apt needs to be refreshed to index newly-packaged repositories:

        .. code-block:: shell

            sudo apt update
            
        apt can now see the source code packages. Run the following commands to download them:

        .. code-block:: shell

            sudo apt install dpkg-dev
            sudo apt-get source --only-source linux-realtime

        .. tip:: 

            Ensure you have enough disk space: The downloaded and extracted packages take almost 2GB of storage.

        At this point, the unpacked source files are in your current directory!

