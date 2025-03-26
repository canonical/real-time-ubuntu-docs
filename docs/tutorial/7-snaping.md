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

With this part defined, it's already possible to build the snap:

```console
$ snapcraft -v
Starting snapcraft, version 8.7.3
Logging execution to '/home/ubuntu/.local/state/snapcraft/log/snapcraft-20250326-122052.993463.log'
lxd (5.21/stable) 5.21.3-75def3c from Canonical✓ installed
Launching managed ubuntu 24.04 instance...
Creating new instance from remote
Creating new base instance from remote
Creating new instance from base instance
Starting instance
Starting snapcraft, version 8.7.3
Logging execution to '/tmp/snapcraft.log'
Initialising lifecycle
Installing build-packages
Installing build-snaps
Pulling src
Building src
:: + make -j12
:: gcc -Wall -Wextra -Wno-unused-result -O2 -o cfs cfs.c
:: gcc -Wall -Wextra -Wno-unused-result -O2 -o edf edf.c
:: gcc -Wall -Wextra -Wno-unused-result -O2 -o fifo fifo.c
:: gcc -Wall -Wextra -Wno-unused-result -O2 -o lock lock.c
:: gcc -Wall -Wextra -Wno-unused-result -O2 -o thread-affinity thread-affinity.c
:: + make -j12 install DESTDIR=/root/parts/src/install
:: mkdir -p /root/parts/src/install/bin
:: install -m 755 cfs edf fifo lock thread-affinity /root/parts/src/install/bin
Staging src
Priming src
Packing...
Reading snap metadata...
Running linters...
Running linter: classic
Running linter: library
Creating snap package...
Packed rt-app_0.1_amd64.snap
```

The first time you build a snap it may take longer because snapcraft is also donwloading and installing a [build provider][build_provider] (as can be observed in the logs)
It's also provisioning a [LXD container][lxd_container].

The build generates an artifact named `rt-app_0.1_amd64.snap`.
Each part of this name has a meaning:
- `rt-app`: It's the name of the snap, as defined in the `name` field of `snapcraft.yaml`.
- `0.1`: The version of the snap, as defined in the `version` field of `snapcraft.yaml`.
  It can also be set dynamically based on a git tag or other information, as explained on [this document][using_craftctl_tool].
- `amd64`: It's the architecture that the host builded from.
  Since we're doing a native build, snapcraft detects the host architecture. 
  It's also possible to do a [cross compile when using autotools][cross_compile_autotools].

It's possible to look what is inside this `.snap` file.
Since the snap is basically a SquashFS file as explained in the [snap format document][snap_format], it's possible to unsquash it:

```console
$ unsquashfs rt-app_0.1_amd64.snap
Parallel unsquashfs: Using 12 processors
6 inodes (6 blocks) to write

[==================================================================|] 12/12 100%

created 6 files
created 4 directories
created 0 symlinks
created 0 devices
created 0 fifos
created 0 sockets
created 0 hardlinks
```

This creates a `squashfs-root` directory with the following contents:

```console
$ tree squashfs-root/
squashfs-root/
├── bin
│   ├── cfs
│   ├── edf
│   ├── fifo
│   ├── lock
│   └── thread-affinity
└── meta
    ├── gui
    └── snap.yaml

4 directories, 6 files
```

Now that it's confirmed that the real-time app binaries are there, it's time to define the apps.

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
[build_provider]: https://snapcraft.io/docs/build-providers
[lxd_container]: https://documentation.ubuntu.com/server/how-to/containers/lxd-containers/
[using_craftctl_tool]: https://snapcraft.io/docs/using-craftctl
[cross_compile_autotools]: https://snapcraft.io/docs/cross-compile-an-autotools-project
[snap_format]: https://snapcraft.io/docs/the-snap-format
