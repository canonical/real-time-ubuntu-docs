# How to install the real-time kernel on Ubuntu Core

The Linux kernel on Ubuntu Core is a snap. Because of that, switching to another compatible kernel is fairly straight forward.

This guide assumes you have booted into Ubuntu Core, using for example one of the [pre-built images published by Canonical][pre-built-uc].
If you intend to build an image that includes a real-time kernel, refer to {doc}`uc-image-creation` instead.

First, find the desired kernel by running the following command on the same Ubuntu Core host, or another system with the same architecture:
```shell
snap info pc-kernel
```
For other architectures, refer to the [pc-kernel](https://snapcraft.io/pc-kernel) Snap Store web page.

Under `channels`, look for entries including `-rt` as part of the track.

We can filter the results with `grep`:
```{terminal}
   :input: snap info pc-kernel | grep -e "-rt"
   :user: ubuntu
   :host: machine-amd64

  25.04-rt/stable:          6.14.0-1004.4               2025-06-19 (2511) 368MB -
  ...
  24-rt-hwe/candidate:      6.11.0-1011.11~24.04.1      2025-06-12 (2551) 363MB -
  ...
  24-rt/stable:             6.8.1-1023.24               2025-06-19 (2550) 351MB -
  ...
```

The above output has been truncated for readability/
It shows various [channels] and versions.
Each channel consists of a track followed by the risk level.

Examples:
* `24-rt/stable:        6.8.1-1023.24` refers to the real-time kernel that is based on Ubuntu 24.04 LTS. The release is stable. The kernel version is 6.8.1.
* `24-rt-hwe/candidate: 6.11.0-1011.11~24.04.1` is a Hardware Enablement (HWE) kernel for Ubuntu 24.04 LTS. This release is on the candidate risk level. The kernel version is 6.11.0.

Read more about [Ubuntu kernel lifecycle][kernel-lifecycle] and [HWE kernels][kwe-kernels].

Once you identify a suitable channel, switch to that channel and refresh the snap.

For example:
```{terminal}
   :input: sudo snap refresh pc-kernel --channel=25.04-rt/stable
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

Linux uc-rt 6.14.0-1004-realtime #4-Ubuntu SMP PREEMPT_RT Fri May 23 22:22:42 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
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
[kwe-kernels]: https://canonical-kernel-docs.readthedocs-hosted.com/latest/reference/hwe-kernels/
