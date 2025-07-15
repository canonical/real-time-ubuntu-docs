# How to revert from real-time kernel to generic kernel

Reverting from the real-time kernel to the generic kernel requires manual intervention.
This guide provides step-by-step instructions for properly disabling the real-time kernel and restoring the system to a generic kernel configuration.
It applies to both **GRUB-based systems** and **Raspberry Pi** devices.

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

In case that other kernels are already installed, as shown in the example above, proceed to the [next step](#disable-rt-on-pro).
If the system has no kernels installed other than the real-time ones, you must install a generic kernel first:

``````{tabs}
`````{group-tab} GRUB system

For grub systems, install the `linux-image-generic` package.

```shell
sudo apt install linux-image-generic
```

````{note}
The previous command will install an older kernel, if you wish to install a newer one search for `linux-image` using `apt-cache` to see the available options:
```shell
apt-cache search linux-image-generic
```
From the list, choose a package whose description is: "Generic Linux kernel image"
````

`````
`````{group-tab} Raspberry Pi

For Raspberry Pi install the `linux-image-raspi` package.
```shell
sudo apt install linux-image-raspi
```

````{note}
The previous command will install an older kernel, if you wish to install a newer one search for `linux-image raspi` using `apt-cache` to see the available options:
```shell
apt-cache search linux-image raspi
```
From the list, choose a package whose description format is:

"Linux kernel image for version <`kernel-version`> on <`raspberry-pi-arch`> SMP"

````
`````
``````

```{danger}
Before proceeding, ensure that you’ve selected the correct tab based on your system's and performed the installation of the generic kernel(or checked that there is already one installed).  
If no other kernel is installed and you remove the real-time kernel, the system will become unbootable.
```

(disable-rt-on-pro)=
## Disable the real-time service on Pro

Disable the real-time kernel service of Ubuntu Pro using the `pro` command.

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

Check your kernel version using the `uname -r` command:

```{terminal}
    :input: uname -r
    :user: ubuntu
    :host: ubuntu

6.8.1-1024-realtime
```

If it ends with `-realtime`, it means you're still running the real-time kernel.
You must manually remove the corresponding packages.


## Removing the real-time kernel

Once you’ve confirmed that another kernel is installed, you can remove the real-time kernel:

```shell
sudo apt remove linux*realtime*
```

All packages matching the wildcard will be selected.
Review the list and confirm that no unintended packages are being removed.
If everything looks good, press "Y" to proceed.

An expected warning may appear during the removal.
On the warning screen, select `<No>` to avoid aborting the removal, then press {kbd}`Enter`.

After that, reboot your system.
