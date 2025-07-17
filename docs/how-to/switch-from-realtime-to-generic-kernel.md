# Switch from real-time to generic kernel

Switching from the real-time kernel back to the generic kernel requires manual configuration
This guide provides step-by-step instructions for disabling the real-time kernel Pro service and restoring the system to a generic kernel configuration.
It applies to both **GRUB-based systems** and **Raspberry Pi** devices running Ubuntu Server or Ubuntu Desktop.

(ensure-the-presence-of-other-kernels)=
## Ensure the presence of other kernels

Before removing the real-time kernel, check whether other kernel versions are already installed.
List the installed kernels:

```{terminal}
    :input: apt list linux-image* --installed
    :user: ubuntu
    :host: ubuntu
Listing... Done
linux-image-6.14.0-24-generic/noble-updates,noble-security,now 6.14.0-24.24~24.04.3 amd64 [installed,automatic]
linux-image-6.8.1-1025-realtime/noble,now 6.8.1-1025.26 amd64 [installed,automatic]
linux-image-generic-hwe-24.04/noble-updates,noble-security,now 6.14.0-24.24~24.04.3 amd64 [installed,automatic]
linux-image-realtime/noble,now 6.8.1-1025.26 amd64 [installed,automatic]
```

In case non-real-time kernels are already installed, proceed to [disabling real-time Pro service](#disable-rt-on-pro).
Otherwise, install a generic kernel first:

``````{tabs}
`````{group-tab} GRUB system

For GRUB systems, search using `apt` for generic Linux kernels:

```shell
apt list linux-generic*
```

Install the required kernel package:

```shell
sudo apt install <package-name>
```

`````
`````{group-tab} Raspberry Pi

For Raspberry Pi install the `linux-raspi` package.

```shell
sudo apt install linux-raspi
```

`````
``````

(disable-rt-on-pro)=
## Disable the real-time service on Pro

Disable the real-time kernel service on Ubuntu Pro using the `pro` command.

On the "Are you sure?" prompt, type `y`.

```{terminal}
   :input: sudo pro disable realtime-kernel
   :user: ubuntu
   :host: ubuntu

This will remove the boot order preference for the real-time kernel and
disable updates to the real-time kernel.

This will NOT fully remove the kernel from your system.

After this operation is complete you must:
  - Ensure a different kernel is installed and configured to boot
  - Reboot into that kernel
  - Fully remove the realtime kernel packages from your system
      - This might look something like `apt remove linux*realtime`,
        but you must ensure this is correct before running it.

Are you sure? (y/N) y
Removing APT access to Real-time kernel
Updating package lists
```

Check the kernel version using the `uname -r` command:

```{terminal}
    :input: uname -r
    :user: ubuntu
    :host: ubuntu

6.8.1-1024-realtime
```

If it ends with `-realtime`, it means the system still running with the real-time kernel.
You must manually remove the corresponding packages.


## Removing the real-time kernel

```{danger}
Before proceeding, ensure that you have [another kernel installed](#ensure-the-presence-of-other-kernels).
The system will become unbootable if the real-time kernel is removed and no other kernels are present.
```

Once confirmed that another kernel is installed, remove the real-time kernel:

```shell
sudo apt remove linux*realtime*
```

All packages matching the wildcard will be selected.
Review the list and confirm that no unintended packages are being removed.
If everything looks good, press "Y" to proceed.

An expected warning may appear during the removal.
On the warning screen, select `<No>` to avoid aborting the removal, then press {kbd}`Enter`.

After that, reboot the system.
