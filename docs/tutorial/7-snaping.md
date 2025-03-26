# Snapping your real-time app

Now that we have our real-time app working, it's time to package it.
This step describes how to package your real-time app as a [snap package][snapcraft.io].

## Installing requirements

Some requirements are needed when building snap packages:

* Confirm that you have [snapd][snapd] installed by running `snap version`:
  If you're using Ubuntu, it must come pre-installed already. 
  Otherwise, refer to the [snapd installation docs][install_snapd].

* Install the [snapcraft snap][snapcraft_docs].
  ```shell
    sudo snap install --classic snapcraft
  ```

## Packaging

The first step to do when starting to create a snap is to get the default template in place, this is done by running `snapcraft init`:

```console
$ snapcraft init
Go to https://docs.snapcraft.io/the-snapcraft-format/8337 for more information about the snapcraft.yaml format.
Successfully initialised project.
```
By doing so, a directory called `snap` is created with the `snapcraft.yaml` file inside of it.

The command output gives you a link that points to the [build configuration][snapcraft_build_cfg] doc.
You may also want to take a look on the [snapcraft.yaml schema][snapcraft_yaml], it's a good reference to understand what is possible on the `snapcraft.yaml` file.

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
To understand better what each one of those fields mean, take a look into the [snapcraft.yaml schema][snapcraft_yaml] reference.

Our work directory should look like this:

```console
$ tree
.
├── cfs.c
├── edf.c
├── fifo.c
├── lock.c
├── snap
│   └── snapcraft.yaml
└── thread-affinity.c

2 directories, 6 files
```

It would be great to have a Makefile so we can build our real-time apps more easily.
This will also help us on packaging our apps.

So let's create one:

```{literalinclude} Makefile
:language: make
```

We're going to make some modifications to the yaml manifest file:

- modify the name from `demo` to `rt-app`.
- Write a nice summary for your snap as well a description of what it does.

### Defining the snap parts

Once the Snap metadata boilerplate is in place, it's time to define the parts.
In Snaps, `parts` are the building blocks of the package, similar to a recipe's list of ingredients and the steps to prepare them.
Since we have five `.c` source files, one might assume we need five parts.
While that approach is possible, it's much easier to use the `Makefile` we just created.
Since building sources with a Makefile is a common operation, Snapcraft provides [built-in plugins][snap_plugins] to simplify the process.

You can list the available plugins for your chosen [Snap base][snap_base] (defined under `base:` in `snapcraft.yaml`) by running `snapcraft plugins`:

```console
$ snapcraft plugins
Displaying plugins available to the current base 'core24' project
ant
autotools
cmake
dump
go
go-use
make
maven
meson
nil
npm
poetry
python
qmake
rust
scons
uv
colcon
conda
flutter
matter-sdk
```

Since we build our `C` code using a `Makefile`, we can use the [make plugin][make_plugin].

```{tip}
The make plugin relies on the Makefile had the `DESTDIR` variable as well on the possibility of that on to be overwritten.
Since it uses it to correctly place the destination targets inside the snap squashfs file.
```

We're also going to rename our part to something more meaningful, like `src`.
We need also to define the `source` from where the snapcraft tool will fetch the code.
This could be from a remote git repository as well from the local files in the directory.
Since we want all our `.c` files to be built into our realtime apps, our source will be defined as `.`.

```yaml
parts:
  src:
    plugin: make
    source: .
```

With this part defined, it's already possible to build our snap:

```console
$ snapcraft -v
```

### Defining snap apps

A snap not necessarily needs to have apps defined, it can only provide files like libraries or binaries.
Those snaps generally are called content snaps, because they make use of the [content interface][content_interface].
That's why you can successfully build your snap without having defined any apps.


### Strictly confining the snap




% links
[snapcraft.io]: https://snapcraft.io/docs
[snapd]: https://snapcraft.io/snapd
[install_snapd]: https://snapcraft.io/docs/installing-snapd
[snapcraft_docs]: https://snapcraft.io/docs/snapcraft
[snapcraft_build_cfg]: https://snapcraft.io/docs/build-configuration
[snapcraft_yaml]: https://snapcraft.io/docs/snapcraft-yaml-schema
[brandstore]: https://ubuntu.com/core/docs/dedicated-snap-stores
[snap_plugins]: https://snapcraft.io/docs/snapcraft-plugins
[snap_base]: https://snapcraft.io/docs/base-snaps
[make_plugin]: https://snapcraft.io/docs/make-plugin
[content_interface]: https://snapcraft.io/docs/content-interface