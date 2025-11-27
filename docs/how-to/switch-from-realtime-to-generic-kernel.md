# Switch from real-time to generic kernel

Switching from the real-time kernel back to the generic kernel requires manual configuration.
This guide describes how to disable the real-time kernel Pro service and restore the system to a generic kernel configuration.
It applies to both **GRUB-based systems** and **Raspberry Pi** devices running Ubuntu Server or Ubuntu Desktop.

For switching between kernel versions on Ubuntu Core, refer to [this document](uc-install-real-time-kernel).

(check-installed-kernels)=
## Check if other kernels are installed

Before removing the real-time kernel, check whether other kernel versions are already installed.

```{note}
Raspberry Pi kernel packages use the `-raspi` suffix instead of `-generic`.
```

List the installed kernels:


`````{tabs}
````{group-tab} GRUB system

```{terminal}
:user: ubuntu
:host: ubuntu

apt list linux-generic* --installed

linux-generic-6.11/noble-updates,noble-security,now 6.11.0-29.29~24.04.1 amd64 [installed]
linux-generic-6.14/noble-updates,noble-security,now 6.14.0-24.24~24.04.3 amd64 [installed]
linux-generic-hwe-24.04/noble-updates,noble-security,now 6.14.0-24.24~24.04.3 amd64 [installed]
```

````
````{group-tab} Raspberry Pi

```{terminal}
:user: ubuntu
:host: ubuntu

apt list linux-raspi* --installed

Listing... Done
linux-image-6.8.0-1030-raspi/noble-updates,noble-security,now 6.8.0-1030.34 arm64 [installed,automatic]
linux-image-raspi/noble-updates,noble-security,now 6.8.0-1030.34 arm64 [installed,automatic]
```

````
`````

If non-real-time kernels are already installed, proceed to [disable the real-time Pro service](#disable-rt-on-pro).

Otherwise, install a generic kernel first:

`````{tabs}
````{group-tab} GRUB system

For GRUB systems, search using `apt` for generic Linux kernels:

```shell
apt list linux-generic*
```

Install the required kernel package:

```shell
sudo apt install <package-name>
```

````
````{group-tab} Raspberry Pi

For Raspberry Pi, install the `linux-raspi` package:

```shell
sudo apt install linux-raspi
```

````
`````

(disable-rt-on-pro)=
## Disable the real-time service on Pro

Disable the `realtime-kernel` Pro service:

```{terminal}
:user: ubuntu
:host: ubuntu

sudo pro disable realtime-kernel

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
:user: ubuntu
:host: ubuntu

uname -r

6.8.1-1024-realtime
```

If it ends with `-realtime`, it means the system still uses the real-time kernel.
You must manually remove the real-time kernel package.

## Remove the real-time kernel

```{danger}
Before proceeding, confirm that you have [another kernel installed](#check-installed-kernels).
The system will become unbootable if the real-time kernel is removed and no other kernels are present.
```

Remove real-time kernel packages:

```shell
sudo apt remove linux*realtime*
```

All packages matching the wildcard will be selected.
Review the list and confirm.

A warning may appear during the removal.
Select `<No>` to continue the removal.

Finally, reboot the system.
It should start with the generic kernel.

Verify that by running `uname -r`:

```{terminal}
:user: ubuntu
:host: ubuntu

uname -r
6.14.0-24-generic
```