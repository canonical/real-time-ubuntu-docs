# Snapping your real-time app

Now that we have our real-time app working, it's time to package it.
This step describes how to package your real-time app as a [snap package][snapcraft.io].

## Installing requirements

Some requiriments are needed when building snap packages:

* Confirm that you have [snapd installed][install_snapd] by running `snap version`:
  ```console
  $ snap version
    snap    2.67.1+24.04
    snapd   2.67.1+24.04
    series  16
    ubuntu  24.04
    kernel  6.11.0-19-generic
  ```
  If you're using Ubuntu, it must come pre-installed already.

* Install the [snapcraft snap][snapcraft_docs].
  ```shell
    sudo snap install --classic snapcraft
  ```

## Starting to package

The first step to do when starting to create a snap is to get the default template in place, this is done by running `snapcraft init`:

```console
$ snapcraft init
Go to https://docs.snapcraft.io/the-snapcraft-format/8337 for more information about the snapcraft.yaml format.
Successfully initialised project.
```


By doing so, a directory called `snap` is created with the `snapcraft.yaml` file inside of it.

Let's have a look on this `snapcraft.yaml` file:

```yaml
name: demo # you probably want to 'snapcraft register <name>'
base: core24 # the base snap is the execution environment for this snap
version: '0.1' # just for humans, typically '1.2+git' or '1.3.2'
summary: Single-line elevator pitch for your amazing snap # 79 char long summary
description: |
  This is my-snap's description. You have a paragraph or two to tell the
  most important story about your snap. Keep it under 100 words though,
  we live in tweetspace and your description wants to look good in the snap
  store.

grade: devel # must be 'stable' to release into candidate/stable channels
confinement: devmode # use 'strict' once you have the right plugs and slots

parts:
  my-part:
    # See 'snapcraft plugins'
    plugin: nil
```



% links
[snapcraft.io]: https://snapcraft.io/docs
[install_snapd]: https://snapcraft.io/docs/installing-snapd
[snapcraft_docs]: https://snapcraft.io/docs/snapcraft
