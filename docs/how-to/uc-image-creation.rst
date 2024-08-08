How to create a real-time Ubuntu Core image
===========================================

.. tip::

    This guide assumes you have access to features available exclusively to `dedicated Snap Store`_ users.

The :doc:`../how-to/uc-boot-parameters` describes the method for dynamically configuring the system for real-time processing.
The configuration is an iterative process that is best done together with the expected workload. 
Once satisfied with the configurations, it is time to prepare the operating system for going into production.

This guide shows how to statically set the desired Kernel command-line options for the Ubuntu Core system.

To do this, we need to create a custom gadget snap, create a model assertion, and then build the OS image.

.. admonition:: Project directory

    Start in an empty directory.
    We refer to this in different parts of the document as our *project directory*.

Create the gadget snap
----------------------

The `gadget snap`_ documentation is a recommended read before starting.

This is best done by forking an existing reference gadget, then changing it for our purpose.
For example, there is the `pc gadget`_ which is suitable for most AMD64 platforms, and the `pi gadget`_ which is meant for Raspberry Pis.

Let's create a custom gadget snap based on the core22 pc gadget.

Inside the project directory, clone the specific branch and enter the repository:

.. code-block:: shell

    git clone https://github.com/snapcore/pc-gadget.git --branch=22 --depth=1
    cd pc-gadget


Add the desired kernel command line in an array to ``kernel-cmdline.append`` in ``gadget/gadget-amd64.yaml``.
For example:

.. code-block:: yaml

    kernel-cmdline:
        append:
            - nohz=on
            - nohz_full=2-N
            - irqaffinity=0-1


Refer to :doc:`../reference/kernel-boot-parameters` for the list of supported parameters.

Modify ``snapcraft.yaml`` to fit your application.
At least, make sure to change the name and version to something distinct, for example, to ``realtime-pc`` and ``example`` respectively.


Now, build the gadget snap:

.. code-block:: console

    $ snapcraft --verbose
    ...
    Created snap package realtime-pc_example_amd64.snap


.. tip::
    You need to rebuild the snap every time you change the `snapcraft.yaml` file.


Create the model assertion
--------------------------

The model assertion is a digitally signed document that describes the content of the Ubuntu Core image.
Read the `model assertion`_ documentation before continuing.

Below is an example model assertion in YAML, describing a `core22` Ubuntu Core
image:

.. literalinclude :: uc-image-creation/model.json
   :language: json

Create the model assertion inside the project directory.

Set the following:


- ``authority-id``, ``brand-id`` to your developer ID
- ``timestamp`` to a RFC3339 formatted string, within you signing key's validity
- ``store`` to your dedicated Snap Store ID

Change the value of ``model`` to something representative of your model.

The ``snaps`` array is a list of snaps that get included in the image.
The gadget snap has no listed ``channel`` and ``id``, because it isn't in a Store.
We have built it locally in the earlier steps and will later on pass it directly to the image builder.
In practice, the gadget snap should be uploaded to a Store and then listed in the model assertion along with its channel and id.
Uploading to the store makes it possible to use a signed snap that receives updates.

The ``realtime-kernel`` snap contains the realtime Linux kernel.
This snap should be included explicitly in your dedicated Snap Store.

Add any other snaps, including for example your realtime application.

Next, we need to sign the model assertion.
Refer to the guide on `signing model assertion`_ for details on how to sign the model assertion. 

Here are the needed steps:

1) Create and register a key


.. code-block:: shell

    snapcraft create-key realtime-ubuntu
    snapcraft register-key realtime-ubuntu


You can use ``snapcraft list-keys`` to check your existing keys.

2) Sign the model assertion

.. code-block:: shell

    snap sign -k realtime-ubuntu model.json > model.signed.yaml

The ``snap sign`` command takes JSON as input and produces YAML as output!

.. tip::

    You need to repeat the signing every time you change the input model, because the signature is calculated based on the model.

Before we continue, let's have an overview of the files inside our project directory:

.. code-block:: console

    $ tree -L 1
    .
    ├── model.json
    ├── model.signed.yaml
    └── pc-gadget

    2 directories, 2 files

The project directory should contain the model assertion, the signed model assertion, and pc-gadget directory.

Build the Ubuntu Core image
---------------------------

First, get familiar with the tooling by referring to the guide on `building Ubuntu Core images`_.

We use ``ubuntu-image`` and need to set the paths to the following as input:

- Exported store credentials
- Signed model assertion YAML file
- Locally built gadget snap

Export the store credentials to a file:

.. code-block:: shell

    snapcraft export-login credentials.txt

Then build the image:

.. code-block:: console

    $ UBUNTU_STORE_AUTH_DATA_FILENAME=credentials.txt \
        ubuntu-image snap model.signed.yaml  --verbose --validation=enforce \
        --snap pc-gadget/realtime-pc_example_amd64.snap
    
    [0] prepare_image
    Fetching snapd (21759)
    Fetching realtime-kernel (134)
    Fetching core22 (1380)
    WARNING: "realtime-pc" installed from local snaps disconnected from a store cannot be refreshed subsequently!
    Copying "pc-gadget/realtime-pc_example_amd64.snap" (realtime-pc)
    [1] load_gadget_yaml
    [2] set_artifact_names
    [3] populate_rootfs_contents
    [4] generate_disk_info
    [5] calculate_rootfs_size
    [6] populate_bootfs_contents
    [7] populate_prepare_partitions
    [8] make_disk
    [9] generate_snap_manifest
    Build successful

This downloads all the snaps specified in the model assertion and builds an image file called ``pc.img``.
There is a warning for ``realtime-pc`` gadget snap because this is being side-loaded, rather than fetched from the store.

.. code-block:: console

    $ file pc.img 
    pc.img: DOS/MBR boot sector; partition 1 : ID=0xee, start-CHS (0x0,0,0), end-CHS (0x0,0,0), startsector 1, 6162431 sectors, extended partition table (last)

✅ The image file is now ready to be flashed on a medium to create a bootable drive with the Ubuntu Core installer!

Once booted, the kernel parameters can be verified by looking into ``/proc/cmdline``:

.. code-block:: console

    $ cat /proc/cmdline
    snapd_recovery_mode=run console=ttyS0,115200n8 console=tty1 panic=-1 nohz=on nohz_full=2-N irqaffinity=0-1

----

This guide provided a very basic setup to configure Ubuntu Core for real-time processing and create a bootable OS image for it. 
For production, the operating system configuration involves many more steps, such as network configuration, user management, and full disk encryption.
The `Ubuntu Core documentation`_ is the best place to continue to learn about the various aspects.

.. LINKS
.. _dedicated Snap Store: https://ubuntu.com/core/services/guide/iot-app-store-intro
.. _pc gadget: https://snapcraft.io/pc
.. _pi gadget: https://snapcraft.io/pi
.. _model assertion: https://ubuntu.com/core/docs/reference/assertions/model
.. _signing model assertion: https://ubuntu.com/core/docs/sign-model-assertion
.. _gadget snap: https://ubuntu.com/core/docs/gadget-snaps
.. _building Ubuntu Core images: https://ubuntu.com/core/docs/build-write-image
.. _Ubuntu Core documentation: https://ubuntu.com/core/docs
