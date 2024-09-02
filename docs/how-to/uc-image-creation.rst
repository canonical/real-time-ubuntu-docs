How to create a real-time Ubuntu Core image
===========================================

.. note::

    This guide assumes access to features available exclusively to `dedicated Snap Store`_ users.

Canonical builds and publishes Ubuntu Core images for a range of `supported platforms`_.
However, these images include the generic Linux kernel. 
In order to run Ubuntu Core with the real-time kernel, we need to build it ourselves. 

This guide shows how to build an Ubuntu Core image with the real-time kernel.
We will first :ref:`build a vanilla image <ubuntu-core-image>` - which is excellent for testing and tuning - then proceed to :ref:`make a custom image <custom-ubuntu-core-image>` with production-ready kernel configurations.


.. _ubuntu-core-image:

Create an image with the real-time kernel
-----------------------------------------

To do this we need to describe the image content and then use a tool to build it.

.. _model-assertion:

Create the model assertion
~~~~~~~~~~~~~~~~~~~~~~~~~~

The model assertion is a digitally signed document that describes the content of the Ubuntu Core image.
Read the `model assertion`_ documentation before continuing.

Below is an example model assertion in YAML, describing a `core22` Ubuntu Core
image:

.. literalinclude:: uc-image-creation/model.json
   :language: json
   :emphasize-lines: 4, 8-11, 19-24

Inside an empty directory, create a file named ``model.json`` with the above content.

Change the following:

- ``model`` to a name that accurately represents your device(s).
- ``authority-id``, ``brand-id`` to your developer ID, since this is custom model. Use ``snapcraft whoami`` command to get your developer ID.
- ``timestamp`` to an RFC3339 formatted time set after the registration of your signing key. If you already have a registered key, use ``date -Iseconds --utc`` command to generate the current time. If not, do this in the next steps after registering your key.
- ``store`` to your dedicated Snap Store ID.

The ``snaps`` array is a list of snaps that get included in the image.
In that list, the ``realtime-kernel`` snap contains the realtime Linux kernel.
Here you can add any other snaps, including for example your real-time applications.

Next, we need to sign the model assertion.
Refer to the guide on `signing model assertion`_ for details on how to sign the model assertion. 
Below are the needed steps:

1. Create and register a key

Use ``snapcraft list-keys`` to check your existing keys.
If you don't already have a key, create one locally and register it with your account.
Choose a meaningful name for the key, as it can be used to sign multiple models.
In this guide we use ``rtu-model`` to sign all our real-time Ubuntu models.

.. code-block:: shell

    snapcraft create-key rtu-model
    snapcraft register-key rtu-model

Remember to update the model assertion's ``timestamp``, if you created a new key and plan to use it next.

2. Sign the model assertion

.. code-block:: shell

    snap sign -k rtu-model model.json > model.signed.yaml

The ``snap sign`` command takes JSON as input and produces YAML as output.

.. tip::

    You need to repeat the signing every time you change the input model, because the signature is calculated based on the model.


Build the image
~~~~~~~~~~~~~~~

First, get familiar with the tooling by referring to the guide on `building Ubuntu Core images`_.

We use ``ubuntu-image`` and need to set the paths to the following as input:

- Exported store credentials
- Signed model assertion YAML file

Export the store credentials to a file:

.. code-block:: shell

    snapcraft export-login credentials.txt

Then build the image:

.. code-block:: console

    $ UBUNTU_STORE_AUTH_DATA_FILENAME=credentials.txt ubuntu-image snap model.signed.yaml --verbose --validation=enforce
    [0] prepare_image
    Fetching snapd (21759)
    Fetching realtime-kernel (149)
    Fetching core22 (1586)
    Fetching pc (146)
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

.. hint::

    To fetch the ``realtime-kernel`` snap for this image build, it should be included explicitly in your dedicated Snap Store.

.. code-block:: console

    $ file pc.img 
    pc.img: DOS/MBR boot sector; partition 1 : ID=0xee, start-CHS (0x0,0,0), end-CHS (0x0,0,0), startsector 1, 6195199 sectors, extended partition table (last)

✅ The image file is now ready. Refer to Ubuntu Core guide on `flashing the image to a storage medium`_.

----

After installing this image on your device, you can continue by tuning your system for real-time processing. 
The :doc:`../how-to/modify-kernel-boot-parameters` guide describes the method for dynamically configuring the kernel command line parameters.
The configuration is an iterative process that is best done together with the expected workload.

Once satisfied with the configurations, continue below to learn how those configurations can be set statically during the image build.

.. _custom-ubuntu-core-image:

Create a custom real-time Ubuntu Core image
-------------------------------------------

This section shows how to statically set the desired Kernel command-line parameters for the Ubuntu Core system.
To do this, we need to create a custom gadget snap, create a model assertion, and then build the OS image.

.. admonition:: Project directory

    Start in an empty directory.
    We refer to this in different parts of the document as our *project directory*.

Create the gadget snap
~~~~~~~~~~~~~~~~~~~~~~

The `gadget snap`_ documentation is a recommended read before starting.

This is best done by forking an existing reference gadget, then changing it for our purpose.
For example, there is the `pc gadget`_ which is suitable for most AMD64 platforms, and the `pi gadget`_ which is meant for Raspberry Pis.

This section uses the core22 pc gadget snap as an example for creating a custom gadget snap.

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
At minimum, make sure to change the name and version to something distinct, for example, to ``realtime-pc`` and ``example`` respectively.

Now, build the gadget snap:

.. code-block:: console

    $ snapcraft --verbose
    ...
    Created snap package realtime-pc_example_amd64.snap


.. tip::
    You need to rebuild the snap every time you change the `snapcraft.yaml` file.


Create the model assertion
~~~~~~~~~~~~~~~~~~~~~~~~~~

Create the model assertion inside the project directory.
Follow the same steps in :ref:`model-assertion` section but replace the ``pc`` snap entry with the following:

.. code-block:: json

    {
        "name": "realtime-pc",
        "type": "gadget"
    },

Unlike the original ``pc`` snap definition, this entry has no listed ``channel`` and ``id``, because it isn't published in a Store.
The locally built gadget snap will be passed directly to the image builder.
For production use, the gadget snap should be uploaded to a Store and then listed in the model assertion along with its channel and id.
Uploading to the store makes it possible to use a signed snap that receives updates.

Sign the model assertion which has our custom ``realtime-pc`` gadget, using the same key which was created in the previous section of this guide:

.. code-block:: shell

    snap sign -k rtu-model model.json > model.signed.yaml

Before we continue, let's have an overview of the files inside our project directory:

.. code-block:: console

    $ tree -L 1
    .
    ├── model.json
    ├── model.signed.yaml
    └── pc-gadget

    2 directories, 2 files

The project directory should contain the model assertion, the signed model assertion, and the pc-gadget directory.

Build the Ubuntu Core image
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Similar to before, we use ``ubuntu-image`` to build the image.
This time we also need to provide the path to the custom gadget snap file.
We therefore need:

- Exported store credentials
- Signed model assertion YAML file
- **Locally built gadget snap**

Build with the following command:

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

This adds all the snaps specified in the model assertion and builds an image file called ``pc.img``.
There is a warning for ``realtime-pc`` gadget snap because this is being side-loaded, rather than fetched from the store.


✅ The image file with the custom configurations is ready! Refer to Ubuntu Core guide on `flashing the image to a storage medium`_.

After installing and running a device with this image, the kernel parameters can be verified by looking into ``/proc/cmdline``:

.. code-block:: console

    $ cat /proc/cmdline
    snapd_recovery_mode=run console=ttyS0,115200n8 console=tty1 panic=-1 nohz=on nohz_full=2-N irqaffinity=0-1

----

This guide provides a very basic setup to configure Ubuntu Core for real-time processing and create a bootable OS image for it. 
For production, the operating system configuration involves many more steps, such as network configuration and full disk encryption.
The device will also need a serial assertion to authenticate itself and receive for example updates to the real-time kernel snap from a dedicated Snap Store.

The `Ubuntu Core documentation`_ is the best place to continue to learn about the various aspects.

.. LINKS
.. _supported platforms: https://ubuntu.com/core/docs/supported-platforms
.. _dedicated Snap Store: https://ubuntu.com/core/docs/dedicated-snap-stores
.. _pc gadget: https://snapcraft.io/pc
.. _pi gadget: https://snapcraft.io/pi
.. _model assertion: https://ubuntu.com/core/docs/reference/assertions/model
.. _signing model assertion: https://ubuntu.com/core/docs/sign-model-assertion
.. _gadget snap: https://ubuntu.com/core/docs/gadget-snaps
.. _building Ubuntu Core images: https://ubuntu.com/core/docs/build-write-image
.. _Ubuntu Core documentation: https://ubuntu.com/core/docs
.. _flashing the image to a storage medium: https://ubuntu.com/core/docs/install-on-a-device