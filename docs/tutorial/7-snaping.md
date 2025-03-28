# Snapping your real-time app

Now that we have our real-time app working, it's time to package it.
This section describes how to package your real-time app as a [snap package][snapcraft.io].

## Installing requirements

Some dependencies are required when building snap packages:

- Confirm that you have [snapd][snapd] installed by running snap version:
  If you're using Ubuntu, it should come pre-installed. 
  Otherwise, refer to the [snapd installation docs][install_snapd].

- Install the [snapcraft snap][snapcraft_docs]:
  ```shell
    sudo snap install --classic snapcraft
  ```

## Packaging

The first step in creating a snap is to generate the default template by running `snapcraft init`:

```console
$ snapcraft init
Go to https://docs.snapcraft.io/the-snapcraft-format/8337 for more information about the snapcraft.yaml format.
Successfully initialised project.
```
This command creates a snap directory containing the snapcraft.yaml file.

The output provides a link to the [build configuration][snapcraft_build_cfg] documentation.

Let's examine the `snapcraft.yaml` file:

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
You may also want to review the [snapcraft.yaml schema][snapcraft_yaml].
Which serves as a reference for understanding what is possible in the `snapcraft.yaml` file.

Your working directory should now look like this:

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

To streamline the build process for our real-time apps, we should create a Makefile.
This will also help with packaging.

Let's create one:

```{literalinclude} Makefile
:language: make
```

We'll also make some modifications to the YAML manifest file:

- Change the name from **demo** to **rt-app**.
- Provide a meaningful summary and description of what the snap does.

### Defining the snap parts

Once the Snap metadata boilerplate is in place, it's time to define the [snapcraft parts][snapcraft_parts].
In Snaps, `parts` serve as the building blocks of the package, similar to a recipe's list of ingredients and preparation steps.

Since we have five `.c` source files, we might assume that we need five separate parts.
While that approach is possible, it's much easier to leverage the Makefile we just created.
Because compiling sources with a Makefile is common, Snapcraft provides [built-in plugins][snap_plugins] to simplify the process.

To list the available plugins for your chosen [Snap base][snap_base] (as defined in the `base:` field of `snapcraft.yaml`), run `snapcraft plugins`:

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
Since we build our `C` code using a Makefile, we can use the [make plugin][make_plugin].

```{tip}
The make plugin relies on the `DESTDIR` variable being available and configurable in the Makefile.
It uses this variable to correctly place the built files inside the snap's squashfs file.
```

We're also going to rename our part to something more meaningful, such as `src`.
Additionally, we need to specify the source location for snapcraft to fetch the code.
This can be either a remote Git repository or local files within the project directory.
Since we want all our `.c` files to be compiled into our real-time applications, we'll define our source as `.`:

```yaml
parts:
  src:
    plugin: make
    source: .
```
With this part defined, we can now build the snap:

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

The first time you build a snap, it may take longer because Snapcraft also downloads and installs a [build provider][build_provider] (as seen in the logs).
It also provisions a [LXD container][lxd_container].

The build generates an artifact named `rt-app_0.1_amd64.snap`.
Each part of this name has a meaning:
- `rt-app`: The snap's name, as defined in the `name` field of `snapcraft.yaml`.
- `0.1`: The snap's version, as defined in the `version` field of `snapcraft.yaml`.
  This can be set dynamically based on a git tag or other information, as explained in [this document][using_craftctl_tool].
- `amd64`: The architecture of the build system.
  Since we're doing a native build, Snapcraft detects the host architecture.
  Cross-compilation is also possible, as described in [cross compiling with autotools][cross_compile_autotools].

It's possible to inspect the contents of this `.snap` file.
Since a Snap is a [squashfs] file, as explained in the [Snap format document][snap_format], we can unsquash it:

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

This creates the `squashfs-root` directory with the following contents:

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

Now that we've confirmed that the real-time app binaries are present, it's time to define the apps.

### Defining snap apps

A Snap doesn't necessarily need to have apps defined—it can provide only files such as libraries or binaries.
These are generally referred to as content Snaps, as they use the [content interface][content_interface].
That's why it's possible to build a Snap successfully without defining any apps.

The simplest app definition includes the app name (which is the name of the object in the apps list) and the `command` property.
Since the apps are inside the `bin` directory, we need to define the relative path to them:

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

After defining the apps, we can rebuild the Snap by running `snapcraft -v` again.
The second build is faster than the first one because the necessary dependencies are already available.
As well the necessary resources already created by the build provider (which is a lxd container in this case).

We can then install the Snap using:

```console
$ sudo snap install rt-app_0.1_amd64.snap
error: cannot find signatures with metadata for snap "rt-app_0.1_amd64.snap"
```

The command fails because we're installing a locally built artifact.
According to the [Snap install modes document][snap_install_modes], when developing and testing a Snap, it's recommended to install it in `devmode`:

```console
$ sudo snap install ./rt-app_0.1_amd64.snap --devmode
rt-app 0.1 installed
```
Now, the Snap appears in the list of installed Snaps:

```console
$ snap list rt-app
Name       Version         Rev    Tracking       Publisher    Notes
rt-app     0.1             x1     -              -            devmode
```

Finally, we can run the defined real-time applications.
Within a Snap, the defined applications become [application commands][application_cmds].
By typing the name of the installed Snap and pressing the Tab key twice, we can list the available commands:

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

In the previous section, we created a Snap that works when installed in `devmode`.
However, one of the key benefits of Snap packages is their ability to run software securely.
Snap packaging provides various [security policies][security_policies] that create a sandboxed environment, enabling software to run in a confined and secure manner.
To take advantage of these security policies, we must [confine the Snap][snap_confinement] so that it can be installed in a strictly confined manner.

The first step in confinement is understanding which system resources the applications need access to.
[Snap interfaces][snap_interfaces] provide a mechanism to grant narrow access to specific system resources.

To identify these required resources, we can take two approaches:

- Perform an extensive source code analysis to identify required files, directories, and [syscalls].
  This may also include analyzing the access to Linux-specific resources such as [procfs], [sysfs], and [configfs].

- Use debugging tools to identify runtime accesses made by the application.

While the first approach is recommended for completeness and accuracy, the second approach is useful when initially confining the application.

The complexity of the application determines the best approach.
If the application has multiple configurations, each triggering different execution paths, a full source code analysis is preferable.
However, if the application has a single, fixed execution path, debugging tools alone may be sufficient.

A variety of resources and tools exist for [debugging Snaps][debug_snaps].
The recommended tool for debugging and initial confinement is [snappy-debug].

To install it:

```console
$ sudo snap install snappy-debug
snappy-debug 0.36-snapd2.59.4 from Canonical✓ installed
```

Now, we need two terminals: one to run `snappy-debug` and another to execute the `rt-app` Snap applications.
Running `snappy-debug` presents the following message:

```console
$ snappy-debug
INFO: Following '/var/log/syslog'. If have dropped messages, use:
INFO: $ sudo journalctl --output=short --follow --all | sudo snappy-debug
```

To ensure no log messages are lost, it is recommended to use the suggested `journalctl` command.
As well running with `sudo` since accessing system logs requires privileged access:

```shell
sudo journalctl --output=short --follow --all | sudo snappy-debug
``` 

#### Analyzing `cfs` application

- Application output:
```console
$ rt-app.cfs
Calls made on thread1: 100000
Calls made on thread2: 1
```
- Debug output:

```console


```

No output from snappy-debug indicates that the `cfs` application does not require additional permissions

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

The [seccomp] message indicates that the application makes a [sched_setattr] system call.
As confirmed in the source code, this occurs inside the `thread_start` function.
The debug tool suggests adding the [process-control interface][proc_ctr_iface].
To understand interfaces, plugs, and slots, refer to the [interface management document][iface_mgmt].

To grant the necessary permission, add `process-control` to the `plugs` field under the `edf` app definition:

```yaml
  edf:
    command: bin/edf
    plugs:
      - process-control
```

Rebuild and reinstall the Snap, then connect the interface:

```{note}
You may see some logs on the `snappy-debug` terminal regarding `lxd` during the `snapcraft` build, those can be safely ignored.
```

```shell
sudo snap connect rt-app:process-control
```

Listing Snap connections confirms the manual connection:

```console
$ snap connections rt-app
Interface        Plug                    Slot              Notes
process-control  rt-app:process-control  :process-control  manual
```

#### Analyzing `fifo` and `lock` applications

Repeat the same process done for `cfs` and `edf` applications on this one. 
Like occurred for `cfs`, when running `fifo` application no reports were generated by `snappy-conf`.
The same happens for the `lock` application, no reports were generated.


#### Analyzing `thread-affinity` application

Repeating the process for `thread-affinity` something different happens on the logs:

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
This happens because there are some denials coming the security policies defined in the snap sandboxing may not affect the functionality of an application.
This is the case for this app.


### Concluding the strict confinement

Now that the required interfaces are defined, change the `confinement` property to `strict`:

```yaml
confinement: strict
```

Then:
  - Build the snap like done before with `snapcraft`.

  - Install it. This time, since we aren't using `devmode` anymore, it's possible to install it in a confined way.
    But because the snap isn't being installed from a snap store, there are no assertions files.
    This means that it's necessary to acknowledge the **dangerous** nature of it.
    So we run our install command with the `--dangerous` flag.

    ```console
    $ sudo snap install ./rt-app_0.1_amd64.snap --dangerous
    rt-app 0.1 installed
    ```

  - Connect the `process-control` interface as shown before.
    It may be already connected if during the iterations the snap wasn't uninstalled.

  - Run all the applications.
    You may see some logs on the `snappy-debug` terminal.
    It's necessary to pain attention on the field `profile=` for [AppArmor][apparmor] denials and `exe="` for Seccomp denials. 
    If it says: `profile="/usr/lib/snapd/snap-confine"`, it's related to the `snap-confine` component and not about the `rt-app` snap.
    To know more about it, consider take a look on this reference about [the snap system architecture][snap_sys_arch].

Then you've finished the development of your snap, you can now change the `grade` field to `stable`, so it can be published on the store.

### Conclusion

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
[squashfs]: https://en.wikipedia.org/wiki/SquashFS
[proc_ctr_iface]: https://snapcraft.io/docs/process-control-interface