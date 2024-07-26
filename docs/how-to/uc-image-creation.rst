How to create a real-time Ubuntu Core image
===========================================

The :doc:`../how-to/uc-boot-parameters` describes the method for dynamically configuring the system for real-time processing.
The configuration is an iterative process that is best done together with the expected workload. 
Once satisfied with the configurations, it is time to prepare the operating system for going into production.

This guide shows how to statically set the desired Kernel command-line options for the Ubuntu Core system.

To do this, we need to create a custom Gadget snap.


Create the gadget snap
----------------------

This is best done by staging an existing reference gadget, then changing it for our purpose.
For example, there is the `pc gadget`_ which is suitable for most AMD64 platforms, and the `pi gadget`_ which is meant for Raspberry Pis.

We will create our custom gadget by staging the core22 pc gadget.
This makes the process much simpler as we only need to create a `snapcraft.yaml` file which stages and customized the gadget.

Create the `snapcraft.yaml` file with the following content:

.. literalinclude :: uc-image-creation/snapcraft.yaml
   :language: yaml

By default, the ``meta.pc/gadget.yaml`` file includes the volumes layout for the Ubuntu Core image.
We can add various other configurations to it such as the list of default snap configurations, interface connections, and most interesting fos us, the static kernel command line options.
In the above example, we have are adding a few kernel command line parameters.

Now, build the gadget snap:


.. code-block:: bash

    snapcraft --verbose


This results in creating a snap named `rt-gadget_test_amd64.snap`.

.. note::
    You need to rebuild the snap every time you change the `snapcraft.yaml` file.


Create the model assertion
--------------------------

The `model assertion`_ is
a digitally signed document that describes the content of the Ubuntu Core image.

Below is an example model assertion in YAML, describing a `core22` Ubuntu Core
image:

.. literalinclude :: uc-image-creation/model.json
   :language: json


The gadget snap no channel and id, because it isn't in the store.
We're going to build it locally and pass it to the image builder.
In practice, the snap should be uploaded to the store and then listed in the model assertion along with its ID and channel.
This makes it possible to use a signed snap that receives updates.

Set the right ``authority-id`` and ``brand-id``.
Set the ``timestamp`` to a RFC3339 formatted string.

Next, we need to sign the model assertion. Refer to the guide on `signing model assertion`_ for details on how to sign the model assertion. 

Here are the needed steps:

1) Create and register a key

```bash
snap login
snap keys
# Continue if you have no existing keys.
# You'll be asked to set a passphrase which is needed before signing
snap create-key rt-gadget-guide
snapcraft register-key rt-gadget-guide
```

We now have a registered key named ``rt-gadget-guide`` which we'll use later.

2) Sign the model assertion

The `snap sign` command takes JSON as input and produces YAML as output!

.. code: bash

    snap sign -k otbr-uc-tutorial model.json > model.json.signed


This will produce a signed model named `model.signed.yaml`.

.. note:

    You need to repeat the signing every time you change the input model, because the signature is calculated based on the model.

Build the Ubuntu Core image
---------------------------

We use ``ubuntu-image`` and set the path to:

- The signed model assertion YAML file.
- The locally built gadget snap.

.. code: bash

    ubuntu-image snap model.signed.yaml --verbose --validation=enforce \
        --snap otbr-gadget_test_amd64.snap

This downloads all the snaps specified in the model assertion and builds
an image file called ``pc.img``.

✅ The image file is now ready to be flashed on a medium to create a bootable drive
with the Ubuntu Core installer!

.. LINKS
.. _pc gadget: https://snapcraft.io/pc
.. _pi gadget: https://snapcraft.io/pi
.. _model assertion: https://ubuntu.com/core/docs/reference/assertions/model
.. _signing model assertion: https://ubuntu.com/core/docs/sign-model-assertion