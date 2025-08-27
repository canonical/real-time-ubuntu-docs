# How to install the real-time kernel on Ubuntu Core

The Linux kernel on Ubuntu Core is a snap. Because of that, switching to another compatible kernel is fairly straightforward.

This guide assumes you have booted into Ubuntu Core, using one of the [pre-built images published by Canonical][pre-built-uc].
If you intend to build an image that includes a real-time kernel, refer to {doc}`uc-image-creation` instead.

## Find available kernels

Run the following command on the same Ubuntu Core host, or on another system with the same architecture:
```shell
snap info pc-kernel
```

```{tip}
For the list of kernel snaps for other architectures, refer to the [pc-kernel](https://snapcraft.io/pc-kernel) Snap Store page.
```

Under `channels`, look for entries that contain `<version>-rt` as prefix. 
Each [channel][channels] consists of a track followed by the risk level.
In general, look at the `stable` risk level to find the most reliable [revisions](https://snapcraft.io/docs/revisions).

Filter results with `grep`. For example, here are the stable real-time kernels compatible with Ubuntu Core 24:
```{terminal}
   :input: snap info pc-kernel | grep 24-rt | grep stable
   :user: ubuntu
   :host: machine-amd64

  24-rt-hwe-edge/stable:    6.14.0-1010.10~24.04.1      2025-08-14 (2736) 377MB -
  24-rt-hwe/stable:         6.14.0-1010.10~24.04.1      2025-08-14 (2736) 377MB -
  24-rt/stable:             6.8.1-1030.31               2025-08-18 (2760) 360MB -
```

Each row shows a snap channel, version, date, revision, and size. In reverse order:
* `24-rt/stable` channel contains the real-time kernel for Ubuntu 24.04 LTS. The release is stable. The snap version `6.8.1-1023.24` indicates that the kernel version is 6.8.1.
* `24-rt-hwe/stable` contains the real-time [Hardware Enablement (HWE)][HWE] kernel for Ubuntu 24.04 LTS. The kernel version is 6.11.0.
* `24-rt-hwe-edge/stable` provides the most recent kernel version. The `-edge` suffix in the track indicates that this is an [edge kernel][edge-kernel] published for early access to the 6.14 kernel.

```{seealso}
Read more about the [Ubuntu kernel lifecycle][kernel-lifecycle] and [HWE kernels][hwe-kernels].
```

## Switch to another kernel

Once you identify a suitable channel, switch to that channel and refresh the snap.

For example:
```{terminal}
   :input: sudo snap refresh pc-kernel --channel=24-rt/stable
   :user: ubuntu
   :host: machine-amd64

...

Broadcast message from root@uc-rt (Wed 2025-06-25 12:00:39 UTC):

reboot scheduled to update the system
The system will reboot now!

```

After the installation, the system will reboot automatically and boot with the new kernel:
```{terminal}
   :input: uname -a
   :user: ubuntu
   :host: machine-amd64

Linux localhost 6.8.1-1030-realtime #31-Ubuntu SMP PREEMPT_RT Thu Aug 14 03:10:52 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
```

````{tip}
Install and run a benchmarking tool such as rt-tests to measure the system performance:
```shell
sudo snap install rt-tests --candidate
sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0 --loops=100000
```

Refer to {doc}`../reference/real-time-metrics-tools` for more.
````

% LINKS

[pre-built-uc]: https://documentation.ubuntu.com/core/tutorials/try-pre-built-images/index.html
[channels]: https://snapcraft.io/docs/channels
[kernel-lifecycle]: https://ubuntu.com/kernel/lifecycle
[hwe-kernels]: https://canonical-kernel-docs.readthedocs-hosted.com/latest/reference/hwe-kernels/
[edge-kernel]: https://canonical-kernel-docs.readthedocs-hosted.com/latest/reference/glossary/#term-edge-kernel
[HWE]: https://canonical-kernel-docs.readthedocs-hosted.com/latest/reference/glossary/#term-HWE
