# Snapping your real-time app

Now that we have our real-time app working, it's time to package it.
This section describes how to package your real-time app as a [snap package][snapcraft.io].

## Installing requirements

Some dependencies are required when building snap packages:

- Confirm that you have [snapd][snapd] installed by running the `snap version` command.
  If you're using Ubuntu, it should come pre-installed. 
  Otherwise, refer to the [snapd installation docs][install_snapd].

- Install the [snapcraft snap][snapcraft_docs]:
  ```shell
    sudo snap install --classic snapcraft
  ```

## Packaging

It is easier to create the snap by starting from a template.
Run the following command from the same directory where the source `.c` files reside:

```{terminal}
   :input: snapcraft init
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

Go to https://docs.snapcraft.io/the-snapcraft-format/8337 for more information about the snapcraft.yaml format.
Successfully initialised project.
```
This command creates a `snap` directory containing the `snapcraft.yaml` file.


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
You may also want to review the [snapcraft.yaml schema][snapcraft_yaml], which is the reference for all the possible configuration keys for the `snapcraft.yaml` file.

Your working directory should now look like this:

```{terminal}
   :input: tree
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

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



We'll also make some modifications to the `snapcraft.yaml` file:

- Change the name from **demo** to **rt-app**.
- Provide a meaningful summary and description of what the snap does.

### Defining the snap parts

Once the Snap metadata boilerplate is in place, it's time to define the [snapcraft parts][snapcraft_parts].
In Snaps, `parts` serve as the building blocks of the package, similar to a recipe's list of ingredients and preparation steps.

Since we have five `.c` source files, we might assume that we need five separate parts.
While that approach is possible, it's much easier to create a Makefile and use that to build all the programs.
Let's create a Makefile for our real-time apps:

```{literalinclude} Makefile
:language: make
```

Because compiling sources with a Makefile is common, Snapcraft provides [built-in plugins][snap_plugins] to simplify the process.
To list the available plugins for your chosen [Snap base][snap_base] (as defined in the `base:` field of `snapcraft.yaml`), run `snapcraft plugins`.
Since we build our `C` code using a Makefile, we can use the [make plugin][make_plugin].

```{tip}
The make plugin relies on the `DESTDIR` variable being available and configurable in the Makefile.
It uses this variable to correctly place the built files inside the snap's squashfs file.
```

We're also going to rename the part in the template from `my-part` to something more meaningful, such as `src`.
Additionally, we need to specify the source location for snapcraft to fetch the code.
This can be either a remote Git repository or local files within the project directory.
Since we want to build the `.c` files that in root of the project directory, we'll set the `source` to `.`:

```yaml
parts:
  src:
    plugin: make
    source: .
```
With this part defined, we can now build the snap:

```{terminal}
   :input: snapcraft -v
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

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
It also provisions an [LXD container][lxd_container].

The build generates an artifact named `rt-app_0.1_amd64.snap`.
Each part of this name has a meaning:
- `rt-app`: The snap's name, as defined in the `name` field of `snapcraft.yaml`.
- `0.1`: The snap's version, as defined in the `version` field of `snapcraft.yaml`.
  This can be set dynamically based on a git tag or other information, as explained in [this document][using_craftctl_tool].
- `amd64`: The architecture of the built snap.
  This matches the host's architecture when doing a native build.
  The C programs and the snap can also be built for other architectures, for example using the [Canonical build farm][remote_build] or by cross compiling the sources and manually setting the target snap architecture.
  It is also possible to cross compile using a plugin if the project used [ autotools][cross_compile_autotools].

It's possible to inspect the contents of this `.snap` file.
Since a Snap is a [squashfs] file, as explained in the [Snap format document][snap_format], we can unsquash it:

```{terminal}
   :input: unsquashfs rt-app_0.1_amd64.snap
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

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

```{terminal}
   :input: tree squashfs-root/
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

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

We created the snap but installing it will not expose any of the programs on the host.
This is because we haven't specified the apps within the snap.
```{note}
A Snap doesn't necessarily need to have apps defined—it can provide only files such as libraries or binaries.
These are generally referred to as content Snaps, as they use the [content interface][content_interface].
That's why it's possible to build a Snap successfully without defining any apps.
```

Create an `apps` section with one entry per app: `cfs`, `edf`, etc.
These are the names given to the apps, exposed on the system when installing this snap.

Each app requires a `command`.
The programs are inside the `bin` directory and we can define the relative path to them:

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
Also the necessary resources are already created by the build provider (which is a lxd container in this case).

We can then install the Snap using:

```{terminal}
   :input: sudo snap install rt-app_0.1_amd64.snap
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

error: cannot find signatures with metadata for snap "rt-app_0.1_amd64.snap"
```

The command fails because we're installing a locally built artifact.
Normally, we'd bypass the signature check by installing in [Dangerous Mode][snap_install_modes], but because the snap has `confinement: devmode` we must install in Developer Mode.
That mode not only bypasses the signature verification, but also allows installing snaps with the [Developer Mode confinement][snap_confinement].

```{terminal}
   :input: sudo snap install rt-app_0.1_amd64.snap --devmode
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

rt-app 0.1 installed
```
Now, the Snap appears in the list of installed Snaps:

```{terminal}
   :input: snap info rt-app
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

name:      rt-app
summary:   Demo real time apps
publisher: –
license:   unset
description: |
  Package of a demo real time applications
commands:
  - rt-app.cfs
  - rt-app.edf
  - rt-app.fifo
  - rt-app.lock
  - rt-app.thread-affinity
refresh-date: 4 days ago, at 21:33 -03
installed:    0.1 (x1) 16kB -
```

Finally, we can run the defined real-time applications.
Within a Snap, the defined applications become [application commands][application_cmds].
By typing the name of the installed Snap and pressing the Tab key twice, we can list the available commands:

```{terminal}
   :input: rt-app.
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

rt-app.cfs              rt-app.edf              rt-app.fifo             rt-app.lock             rt-app.thread-affinity
```

Running the `cfs` program:

```{terminal}
   :input: rt-app.cfs
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

Calls made on thread1: 100000
Calls made on thread2: 1
```

### Strictly confining the snap

In the previous section, we created a Snap that works when installed in the Developer Mode.
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

```{terminal}
   :input: sudo snap install snappy-debug
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial
snappy-debug 0.36-snapd2.59.4 from Canonical✓ installed
```

Now, we need two terminals: one to run `snappy-debug` and another to execute the `rt-app` Snap applications.
Running `snappy-debug` presents the following message:

```{terminal}
   :input: snappy-debug
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

INFO: Following '/var/log/syslog'. If have dropped messages, use:
INFO: $ sudo journalctl --output=short --follow --all | sudo snappy-debug
```

````{tip}
To ensure no log messages are lost, it is recommended to use the suggested `journalctl` command.
It needs to be run with `sudo` since accessing system logs requires privileged access:

```shell
sudo journalctl --output=short --follow --all | sudo snappy-debug
``` 
````

#### Analyzing `cfs` application

- Application output:
```{terminal}
   :input: rt-app.cfs
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

Calls made on thread1: 100000
Calls made on thread2: 1
```
- Debug output:

```{terminal}
   :input: rt-app.cfs
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial


```

No output from snappy-debug indicates that the `cfs` application does not require additional permissions

#### Analyzing `edf` application

- Application output:
```{terminal}
   :input: rt-app.edf
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

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

```{terminal}
   :input: snap connections rt-app
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

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
```{terminal}
   :input: rt-app.thread-affinity
   :user: ubuntu
   :host: ubuntu
   :dir: ~/tutorial

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
Instead there is a message suggesting to just ignore the denial report.
This happens because there are some denials coming from the security policies defined in the snap sandboxing, which potentially do not affect the functionality of an application.
This is the case for this app.


### Concluding the strict confinement

Now that the required interfaces are defined, change the `confinement` property to `strict`:

```yaml
confinement: strict
```

Then:
  - Build the snap like done before with `snapcraft`.

  - Install it. This time, since we aren't using `devmode` anymore, it's possible to install it in a confined way.
    Because the snap isn't being installed from a snap store, there is no way to fully trust it.
    It is therefore necessary to acknowledge the **dangerous** nature of it.
    So we run our install command with the `--dangerous` flag.

    ```{terminal}
      :input: sudo snap install ./rt-app_0.1_amd64.snap --dangerous
      :user: ubuntu
      :host: ubuntu
      :dir: ~/tutorial

    rt-app 0.1 installed
    ```

  - Connect the `process-control` interface as shown before.
    It might still be connected if the snap wasn't uninstalled during the development steps.

  - Run all the applications.
    You may see some logs on the `snappy-debug` terminal.
    It's necessary to pay attention to the field `profile=` for [AppArmor][apparmor] denials and `exe="` for Seccomp denials. 
    If it says: `profile="/usr/lib/snapd/snap-confine"`, it's related to the `snap-confine` component and not about the `rt-app` snap.
    To learn more about this, take a look at this reference of [the snap system architecture][snap_sys_arch].

You have now finished the development of your snap.
You can change the `grade` field to `stable` if you consider it so. Snaps with the `devel` grade cannot be promoted to a lower risk level. This is related to the concept of [snap channels](https://snapcraft.io/docs/channels), important when publishing a snap.

### Conclusion

The final `snapcraft.yaml` file should look like this:

```{literalinclude} snapcraft.yaml
:language: yaml
```

A good next step is to distribute this snap via a store:
- [Publish to the global Snap Store][publish_snap]
- [Publish to a dedicated Snap Store][brandstore]

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
[remote_build]: https://snapcraft.io/docs/remote-build