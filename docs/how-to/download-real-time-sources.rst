How to download Real-time Ubuntu kernel source code
===================================================

The source code for Ubuntu's real-time kernel is available for download.
The process depends on the release of Ubuntu you are using.

1. Enable access to the real-time Ubuntu kernel.

.. tab-set::
    :sync-group: release

    .. tab-item:: LTS releases (22.04, 24.04, etc.)
        :sync: lts

        .. note::
            LTS Releases of Ubuntu require Ubuntu Pro to download their respective real-time kernel source code.
            See the `Ubuntu Pro documentation <https://documentation.ubuntu.com/pro/>`_ for more information.

        Enable access to the real-time kernel feature with the following command:

        .. code-block:: shell

            sudo pro enable realtime-kernel --access-only

        If you additionally want to install the real-time kernel to this system, remove the ``--access-only`` flag:

        .. code-block:: shell

            sudo pro enable realtime-kernel

        You can verify the realtime-kernel is activated with ``sudo pro status``.

        .. NOTE I don't think sudo pro status will tell you if you used the access-only flag or not.

    .. tab-item:: Interim releases (24.10, etc.)
        :sync: interim
        
        The real-time kernel source is freely available in the 
        `universe repository <https://packages.ubuntu.com/oracular/linux-realtime>`_ for interim releases. 
        No actions are required for this step!
    

2. Enable downloading source packages with apt.

apt is used to download the real-time kernel source. We need to enable the source repositories (``deb-src``) to allow ``apt`` to download real-time kernel source packages from its archives.

.. tab-set::
    :sync-group: release

    .. tab-item:: 24.04 LTS and newer
        :sync: lts

        Use the following command to add ``deb-src`` to the :file:`ubuntu-realtime-kernel.sources` file:

        .. code-block:: shell
            
            sudo sed -i '2s/^Types: deb$/Types: deb deb-src/' /etc/apt/sources.list.d/ubuntu-realtime-kernel.sources

    .. tab-item:: 22.04 LTS and older
        :sync: lts

        In older LTS versions of Ubuntu, open the ``/etc/apt/sources.list`` file in a text editor (requires elevated permissions).

        Then, :spellexception:`uncomment` (delete the leading ``#``) the line which includes ``deb-src http://archive.ubuntu.com/ubuntu/ jammy main``. 
        For other releases, ``jammy`` will match the code-name for your release.

        For example, the file looks like this for 22.04 LTS (Jammy Jellyfish):

        .. code-block:: debsources

            # See http://help.ubuntu.com/community/UpgradeNotes for how to upgrade to
            # newer versions of the distribution.
            deb http://archive.ubuntu.com/ubuntu/ jammy main restricted universe multiverse
            # deb-src http://archive.ubuntu.com/ubuntu/ jammy main restricted universe multiverse
            # ^^^^^ Uncomment the line above! ^^^^^

            deb http://archive.ubuntu.com/ubuntu/ jammy-updates main restricted universe multiverse
            # deb-src http://archive.ubuntu.com/ubuntu/ jammy-updates main restricted universe multiverse
            # ^^^^^ Leave this one (and all others underneath) alone. ^^^^^

            # ...


        .. caution::
            Be sure to :spellexception:`uncomment` only the line which includes ``main`` after the release code-name.


    .. tab-item:: Interim Releases (24.10, etc.)
        :sync: interim

        Use the following command to add ``deb-src`` to the :file:`ubuntu.sources` file:

        .. code-block:: shell
            
            sudo sed -i '1s/^Types: deb$/Types: deb deb-src/' /etc/apt/sources.list.d/ubuntu.sources


3. Get the real-time kernel source code.

First, refresh apt to index newly-added repositories:

.. code-block:: shell 

    sudo apt update

Next, install ``dpkg-dev``, which is required to unpack Debian source packages:

.. code-block:: shell
    
    sudo apt install dpkg-dev

Lastly, download the real-time kernel source code:

.. code-block:: shell

    sudo apt-get source --only-source linux-realtime


.. tip:: 

    Ensure you have enough disk space; the downloaded and extracted packages take almost 2GB of storage.

At this point, the unpacked source files are in your current directory.

