# How to revert from real-time kernel to generic kernel

Reverting from the real-time kernel to the generic kernel requires manual intervention.
This guide provides step-by-step instructions for properly disabling the real-time kernel and restoring the system to a generic kernel configuration.
It applies to both **GRUB-based systems** and **Raspberry Pi** devices running Ubuntu Server or Ubuntu Desktop.

```{warning}
This guide does not apply to Ubuntu Core systems
```

## Ensure the presence of other kernels

Before removing the real-time kernel, check whether other kernel versions are already installed.
Search for installed kernels excluding the real-time ones:

```{terminal}
    :input: apt list --installed | grep -v realtime | grep linux-image
    :user: ubuntu
    :host: ubuntu

WARNING: apt does not have a stable CLI interface. Use with caution in scripts.

linux-image-6.11.0-29-generic/noble-updates,noble-security,now 6.11.0-29.29~24.04.1 amd64 [installed,automatic]
linux-image-generic-hwe-24.04/noble-updates,noble-security,now 6.11.0-29.29~24.04.1 amd64 [installed,automatic]
```

In case that other kernels are already installed, as shown in the example above, proceed to [disabling real-time Pro service](#disable-rt-on-pro).
If the system has no kernels installed other than the real-time ones, you must install a generic kernel first:

``````{tabs}
`````{group-tab} GRUB system

For GRUB systems, search using `apt` for generic Linux kernels:

```shell
apt search Complete Generic Linux kernel and headers
```
From the list, choose a package whose description is: "Complete Generic Linux kernel and headers"

Then install the selected one using `apt`:

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

```{danger}
Before proceeding, ensure that the installation of the generic kernel was performed (or checked that there was already one installed).
If no other kernel is installed and the real-time kernel got removed, the system will become unbootable.
```

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
