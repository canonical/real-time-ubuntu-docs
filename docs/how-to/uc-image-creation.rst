How to create a real-time Ubuntu Core image
===========================================

The :doc:`../how-to/uc-boot-parameters` describes the method for dynamically configuring the system for real-time processing.
The configuration is an iterative process that is best done together with the expected workload. 
Once satisfied with the configurations, it is time to prepare the operating system for going into production.

This guide shows how to statically set the desired Kernel command-line options for the Ubuntu Core system.

To do this, we need to create a custom gadget snap.
The `gadget snap`_ documentation is a recommended read before starting.


Create the gadget snap
----------------------

This is best done by forking an existing reference gadget, then changing it for our purpose.
For example, there is the `pc gadget`_ which is suitable for most AMD64 platforms, and the `pi gadget`_ which is meant for Raspberry Pis.

Let's create a custom gadget snap based on the core22 pc gadget.

Clone the specific branch and enter the directory:

.. code-block:: shell

    git clone https://github.com/snapcore/pc-gadget.git --branch=22 --depth=1
    cd pc-gadget


Add the desired kernel command line in an array to `kernel-cmdline.append` in `gadget/gadget-amd64.yaml`.
For example:

.. code-block:: yaml

    kernel-cmdline:
        append:
            - nohz=on
            - nohz_full=2-N
            - irqaffinity=0-1


Modify `snapcraft.yaml` to fit your application.
At least, make sure to change the name and version to something distinct, for example: `realtime-pc` and `example`


Now, build the gadget snap:

.. code-block:: console

    $ snapcraft --verbose
    ...
    Created snap package realtime-pc_example_amd64.snap


.. note::
    You need to rebuild the snap every time you change the `snapcraft.yaml` file.


Create the model assertion
--------------------------

The model assertion is a digitally signed document that describes the content of the Ubuntu Core image.
Read the `model assertion`_ documentation before continuing.

Below is an example model assertion in YAML, describing a `core22` Ubuntu Core
image:

.. literalinclude :: uc-image-creation/model.json
   :language: json

Create the model assertion in the same directory as the pc-gadget.

The gadget snap no channel and id, because it isn't in the store.
We're going to build it locally and pass it to the image builder.
In practice, the snap should be uploaded to the store and then listed in the model assertion along with its ID and channel.
This makes it possible to use a signed snap that receives updates.

Set the right ``authority-id`` and ``brand-id``.
Set the ``timestamp`` to a RFC3339 formatted string.

Next, we need to sign the model assertion. Refer to the guide on `signing model assertion`_ for details on how to sign the model assertion. 

Here are the needed steps:

1) Create and register a key


.. code-block:: shell

    snapcraft create-key realtime-ubuntu
    snapcraft register-key realtime-ubuntu


You can use `snapcraft list-keys` to check your existing keys.

2) Sign the model assertion

.. code-block:: shell

    snap sign -k realtime-ubuntu model.json > model.signed.yaml

The `snap sign` command takes JSON as input and produces YAML as output!

.. note:

    You need to repeat the signing every time you change the input model, because the signature is calculated based on the model.

Before we continue, let's have an overview of our files :

.. code-block:: console

    $ tree -L 1
    .
    ├── model.json
    ├── model.signed.yaml
    └── pc-gadget

    2 directories, 2 files


Build the Ubuntu Core image
---------------------------

First, get familiar with the tooling by refer to the guide on `building Ubuntu Core images`_.

We use ``ubuntu-image`` and set the path to the:

- signed model assertion YAML file
- locally built gadget snap

.. code-block:: shell

    ubuntu-image snap model.signed.yaml --verbose --validation=enforce \
        --snap pc-gadget/realtime-pc_example_amd64.snap

This downloads all the snaps specified in the model assertion and builds an image file called ``pc.img``.

✅ The image file is now ready to be flashed on a medium to create a bootable drive with the Ubuntu Core installer!


----

This guide provided a very basic setup to configure Ubuntu Core for real-time processing and create a bootable OS image for it. 
For production settings, the operating system configuration involved many more steps, such as network configuration, user management, and full disk encryption.
The `Ubuntu Core documentation`_ is the best place to continue to learn about the various aspects.

.. LINKS
.. _pc gadget: https://snapcraft.io/pc
.. _pi gadget: https://snapcraft.io/pi
.. _model assertion: https://ubuntu.com/core/docs/reference/assertions/model
.. _signing model assertion: https://ubuntu.com/core/docs/sign-model-assertion
.. _gadget snap: https://snapcraft.io/docs/the-gadget-snap
.. _building Ubuntu Core images: https://ubuntu.com/core/docs/build-write-image
.. _Ubuntu Core documentation: https://ubuntu.com/core/docs