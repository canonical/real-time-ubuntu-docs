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

Once the Snap metadata boilerplate is in place, it's time to define the [snapcraft parts][snapcraft_parts].
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

The simplest app definition includes the app name (which is the name of the object in the apps list) with the `command` property.
Since the apps are inside the `bin` directory, it's necessary to define the path related to it.

```yaml
apps:
  cfs:
    command: bin/cfs

  edf:
    command: bin/edf

  fifo:
    command: bin/fifo

  lock:
    command: bin/lock

  thread-affinity:
    command: bin/thread-affinity

```

So, after defining the apps, it's possible to build again the snap file, by running `snapcraft -v` again. 
The second time that the snap is build is faster than the first one because there are already a 

Then we can install the snap by running the `snap install` command:

```console
$ sudo snap install rt-app_0.1_amd64.snap
error: cannot find signatures with metadata for snap "rt-app_0.1_amd64.snap"
```

So, the command fails, this is because we're installing from an local build artifact.
According with the [snap install modes document][snap_install_modes], when developing and testing a snap, it's recommended to install in `devmode`.
Then installing it in `devmode`:

```console
$ sudo snap install ./rt-app_0.1_amd64.snap --devmode
rt-app 0.1 installed
```
Then we now have it in the list of installed snaps:

```console
$ snap list rt-app
Name       Version         Rev    Tracking       Publisher    Notes
rt-app     0.1             x1     -              -            devmode
```

Finally, it's possible to run the the defined real-time applications.
Inside a snap, the defined applications become [application commands][application_cmds].
By typing the name of the installed snap and pressing the tab key twice it's possible to list the avaible commands: 

```console
$ rt-app.
rt-app.cfs              rt-app.edf              rt-app.fifo             rt-app.lock             rt-app.thread-affinity
```

Running the `cfs` program:

```console
$ rt-app.cfs
Calls made on thread1: 100000
Calls made on thread2: 1
```

### Strictly confining the snap

On the previous part, the snap package was put in shape in a way that it simply works when installed on `devmode`.
But one of the greatest features of snap packages it's the possibility of running software in a secure way.
The snap packaging provide some [security policies][security_policies] which create a sandboxing environment.
Enabling the possibility of run software in a secure and confined way.
In order to take advantage of those security policies it's necessary to [confine the snap][snap_confinement], so it can be installed in a strictly confined way.

The first step of the confinement it's to understand what resources the applications require to access on the system.
For that, [snap interfaces][snap_interfaces] exists to provide narrow access to system resources.

For identifyng those resources we can go in two ways: 

- By doing an extensive source code analysis, identifying the used files, directories and [syscalls]. 
  Also maybe some linux internal resouces like the [procfs], [sysfs], [configfs] and so on.

- By running debug tools and idenfifying the accesses that the application do at runtime.

Although, the first way it's more recommended, resulting in a more complete and accurate result.
The second way it's bettter when starting to confine the app.

It's important to consider that this may depends on the complexity of the application being confined.
If the application has multiple configurations, each one, triggering a different path in the source code.
Then, a complete source code analysis is recommeded.
Otherwise, if the application only does one thing, not being configurable.
Then using the debugging resources it's enough.

There are many resources and tooling when it commes to [debugging snaps][debug_snaps]. 
The recommended tool for debbugging and starting to confine the app is the [snappy-debug].

The tool is also a snap to be installed:

```console
$ sudo snap install snappy-debug
snappy-debug 0.36-snapd2.59.4 from Canonical✓ installed
```

Now it's necessary to use two terminals, one for running the `snappy-debug`, and the other for running the rt-app snap applications.
When running `snappy-debug` we're going to face an attached terminal with this message:

```console
$ snappy-debug
INFO: Following '/var/log/syslog'. If have dropped messages, use:
INFO: $ sudo journalctl --output=short --follow --all | sudo snappy-debug
```

To make sure that no message logs were dropped it's a good move to run the snap in the suggested form, by pipping it from the `journalctl`.
Also, because since we're accesing the system logs, priviledge access is necessary.
We're going to run the `sudo journalctl --output=short --follow --all | sudo snappy-debug`

Then running the while running the `snappy-debug` we run the first app:

#### Analyzing `cfs` application

- Application output:
```console
$ rt-app.cfs
Calls made on thread1: 100000
Calls made on thread2: 1
```
- Debug output:

```console
$ sudo journalctl --output=short --follow --all | sudo snappy-debug


```

Nothing is seeing on the output of `snappy-debug`.
This means that the `cfs` application doesn't needs any aditional permission. 

#### Analyzing `edf` application

- Application output:
```console
$ rt-app.edf
thread1:   period =  2 s
          runtime = 10 ms
         deadline = 11 ms
thread2:   period =  2 s
          runtime = 10 ms
         deadline = 11 ms

Calls made on thread1: 100000
Calls made on thread2: 1
```

- Debug output:
```
= Seccomp =
Time: Mar 27 20:29:40
Log: auid=1000 uid=1000 gid=1000 ses=119 subj=snap.rt-app.edf pid=37555 comm="edf" exe="/snap/rt-app/x1/bin/edf" sig=0 arch=c000003e 314(sched_setattr) compat=0 ip=0x78846c90325d code=0x7ffc0000
Syscall: sched_setattr
Suggestion:
* add 'process-control' to 'plugs'
```

There is a [seccomp] message regarding the application doing a [sched_setattr] system call. 
Which in fact the application does by looking at the source code inside the `thread_start` function.
The debugger tool also suggests which interface should we use. 
To understand better about interfaces, plugs and slots checkout this [interfaces management document][iface_mgmt].

So, it's necessary to add the `process-control` interface to the field `plugs` under the `edf` app:

```yaml
  edf:
    command: bin/edf
    plugs:
      - process-control
```
Now: 
  - compile again the snap;
  - install the snap;
  - run again the `edf` application.

You may see some logs on the `snappy-debug` terminal regarding `lxd` during the `snapcraft` build, those can be safely ignored.
Listing the avaible plug connections for the `rt-app` snap it's possible to see the new plug for `process-control` interface it's there: 

```console
$ snap connections rt-app
Interface        Plug                    Slot  Notes
process-control  rt-app:process-control  -     -
```

If you run again the app `edf`, the same seccomp report will show-up.
In order to give the permission, it's necessary to connect the snap interface.

```shell
sudo snap connect rt-app:process-control
```

Now listing again it's possible to see that it reports that the interface was manually connected:

```console
$ snap connections rt-app
Interface        Plug                    Slot              Notes
process-control  rt-app:process-control  :process-control  manual
```

#### Analyzing `fifo` and `lock` applications

Repeat the same process done for `cfs` and `edf` applications on this one. 
Like occured for `cfs`, when running `fifo` application no reports were generated by `snappy-conf`.
The same happens for the `lock` application, no reports were generated.


#### Analyzing `thread-affinity` application

When repeating the proccess for `thread-affinity` something different happens on the logs:

- Application output:
```console
$ rt-app.thread-affinity
thread1 priority: 0
thread2 priority: 0
Calls made on thread1: 100000
Calls made on thread2: 1
```

- Debug output:
```
= Seccomp =
Time: Mar 27 21:04:22
Log: auid=1000 uid=1000 gid=1000 ses=119 subj=snap.rt-app.thread-affinity pid=40684 comm="thread-affinity" exe="/snap/rt-app/x2/bin/thread-affinity" sig=0 arch=c000003e 203(sched_setaffinity) compat=0 ip=0x733af5a4d6e1 code=0x7ffc0000
Syscall: sched_setaffinity
Suggestion:
* ignore the denial if the program otherwise works correctly (unconditional sched_setaffinity is often just noise)
```

We see a seccomp report, but no suggested interface to use.
Instead a message saying to just ignore the denial report.
This happens because there are some denials comming the security policies defined in the snap sandboxing may not affect the functionality of an application.
This is the case for this app.


### Concluding the strict confinement

Now that the needed interfaces are defined, it's necessary to change the `confinement` property on out manifest file.  
Change the confinement to `strict`:

```yaml
confinement: strict
```

Then:
  - Build the snap like done before with `snapcraft`.

  - Install it. This time, since we aren't using `devmode` anymore, it's possible to install it in a confined way.
    But because the snap isn't being installed from a snap store, there are no assertions files.
    This means that it's necessary to acknowleadge the **dangerous** nature of it.
    So we run our install command with the `--dangerous` flag.

    ```console
    $ sudo snap install ./rt-app_0.1_amd64.snap --dangerous
    rt-app 0.1 installed
    ```

  - Connect the `process-control` interface as shown before.
    It may be already connected if during the iterations the snap wasn't unninstaled.

  - Run all the applications.
    You may see some logs on the `snappy-debug` terminal.
    It's necessary to pain attention on the field `profile=` for [AppArmor][apparmor] denials and `exe="` for Seccomp denials. 
    If it says: `profile="/usr/lib/snapd/snap-confine"`, it's related to the `snap-confine` component and not about the `rt-app` snap.
    To know more about it, consider take a look on this reference about [the snap system architecture][snap_sys_arch].

Then you've finished the development of your snap, you can now change the `grade` field to `stable`, so it can be published on the store.

### Conclussion

The final `snapcraft.yaml` manifest file should look like this:

```{literalinclude} snapcraft.yaml
:language: yaml
```

The next steps that you may consider to do:
- [Release the snap to Snap Store][publish_snap]
- [Create a dedicated Snap Store for your company][brandstore]

% links
[snapcraft.io]: https://snapcraft.io/docs
[snapd]: https://snapcraft.io/snapd
[install_snapd]: https://snapcraft.io/docs/installing-snapd
[snapcraft_docs]: https://snapcraft.io/docs/snapcraft
[snapcraft_build_cfg]: https://snapcraft.io/docs/build-configuration
[snapcraft_yaml]: https://snapcraft.io/docs/snapcraft-yaml-schema
[snap_plugins]: https://snapcraft.io/docs/snapcraft-plugins
[snap_base]: https://snapcraft.io/docs/base-snaps
[make_plugin]: https://snapcraft.io/docs/make-plugin
[content_interface]: https://snapcraft.io/docs/content-interface
[build_provider]: https://snapcraft.io/docs/build-providers
[lxd_container]: https://documentation.ubuntu.com/server/how-to/containers/lxd-containers/
[using_craftctl_tool]: https://snapcraft.io/docs/using-craftctl
[cross_compile_autotools]: https://snapcraft.io/docs/cross-compile-an-autotools-project
[snap_format]: https://snapcraft.io/docs/the-snap-format
[snapcraft_parts]: https://canonical-snapcraft.readthedocs-hosted.com/en/stable/explanation/parts.html
[snap_install_modes]: https://snapcraft.io/docs/install-modes
[application_cmds]: https://snapcraft.io/docs/commands-and-aliases#p-19557-application-commands
[security_policies]: https://snapcraft.io/docs/security-policies
[snap_confinement]: https://snapcraft.io/docs/snap-confinement
[snap_interfaces]: https://snapcraft.io/docs/supported-interfaces
[syscalls]: https://manpages.ubuntu.com/manpages/noble/en/man2/syscalls.2.html
[procfs]: https://manpages.ubuntu.com/manpages/noble/man5/procfs.5.html
[sysfs]: https://manpages.ubuntu.com/manpages/noble/man5/sysfs.5.html
[configfs]: https://docs.kernel.org/filesystems/configfs.html
[debug_snaps]: https://snapcraft.io/docs/debug-snaps
[snappy-debug]: https://snapcraft.io/snappy-debug
[seccomp]: https://manpages.ubuntu.com/manpages/noble/man2/seccomp.2.html
[sched_setattr]: https://manpages.ubuntu.com/manpages/noble/man2/sched_setattr.2.html
[iface_mgmt]: https://snapcraft.io/docs/interface-management
[apparmor]: https://apparmor.net/
[snap_sys_arch]: https://snapcraft.io/docs/system-architecture
[publish_snap]: https://snapcraft.io/docs/releasing-to-the-snap-store
[brandstore]: https://ubuntu.com/core/docs/dedicated-snap-stores