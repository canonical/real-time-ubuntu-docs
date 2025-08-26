# How to create a Real-time Ubuntu VM using KVM

This guide outlines the process and considerations for setting up a virtual machine (VM) to handle real-time workloads, drawing upon various resources to ensure optimal performance and minimal latency.

## Overview of real-time workloads in VMs

Real-time systems require predictable and timely responses to events.
Achieving this within a virtualized environment presents unique challenges, primarily related to scheduling, latency, and resource isolation.

### Key concepts

-   Scheduling: A single scheduler must manage tasks across all VMs, which can lead to a lack of isolation and one VM affecting another.
-   Latency: System Management Interrupts (SMIs) from the BIOS can introduce millisecond latencies, interrupting the RT OS. Hardware that allows disabling or modifying SMIs is crucial.
-   Virtualization Type: Hardware virtualization (KVM) generally performs much better than OS-level virtualization or full emulation (QEMU).

### Performance considerations

KVM (Kernel-based Virtual Machine) is a popular choice for running real-time workloads due to its performance advantages over other virtualization technologies like Xen or QEMU.

-   KVM vs. Xen: KVM outperforms Xen in real-time scenarios.
-   QEMU Emulation: Using QEMU for full emulation is not recommended for real-time applications due to poor performance.

The guide will assume the use of kernel-based virtual machines (KVMs) due to its popularity and advantages over other virtualization technologies like Xen or QEMU.

## Host configuration

To run a real-time VM, the host must first be prepared for real-time operation. This involves applying the usual tuning steps, isolating as many CPUs as possible.

### Configure BIOS

To enable real-time determinism and take advantage of hardware features, specific BIOS settings must be configured.

Please refer to the official documentation corresponding to your processor and platform.

### Identify CPU topology

When configuring a system to host real-time VMs, it is important to understand the underlying processor architecture.
VMs should be assigned CPUs that share the same memory and cache hierarchy to ensure optimal performance and low latency.

Check the number of online CPUs for further comparison by running `nproc`: 

```{terminal}
    :input: nproc
    :user: ubuntu
    :host: ubuntu
16
```

For example, the output below shows the system has 16 CPU cores.

Ensure the online CPU list contains `0-15` (or the value corresponding to your configuration):

```{terminal}
    :input: lscpu
    :user: ubuntu
    :host: ubuntu
Architecture:                x86_64
  CPU op-mode(s):            32-bit, 64-bit
  Address sizes:             39 bits physical, 48 bits virtual
  Byte Order:                Little Endian
CPU(s):                      16
  On-line CPU(s) list:       0-15
Vendor ID:                   GenuineIntel
  Model name:                13th Gen Intel(R) Core(TM) i7-13700K
    CPU family:              6
    Model:                   154
    Thread(s) per core:      1
    Core(s) per socket:      16
    Socket(s):               1
    Stepping:                3
    Frequency boost:         enabled
    CPU(s) scaling MHz:      40%
    CPU max MHz:             2701.0000
    CPU min MHz:             400.0000
    BogoMIPS:                5376.00
    Flags:                   [...]
Virtualization features:     
  Virtualization:            VT-x
Caches (sum of all):         
  L1d:                       544 KiB
  L1i:                       704 KiB
  L2:                        11.5 MiB
  L3:                        24 MiB
NUMA:                        
  NUMA node(s):              1
  NUMA node0 CPU(s):         0-15
[...]
```

### Configure kernel boot parameters

Kernel settings should ensure that real-time workloads are bound to CPUs sharing the same memory and cache hierarchy.

Update the GRUB configuration to isolate a defined set of CPUs. The specific CPUs to isolate depend on the total CPU count and any host workloads that must continue running outside of KVM.

```{tip}
One should avoid isolating CPU 0, as Linux binds essential housekeeping tasks (e.g. interrupts, RCU callbacks, kernel threads) there by default.
As a best practice, leave at least one non-isolated CPU per socket.
```

In this example, CPUs `0-7` will be reserved, while CPUs `8-15` will be isolated.

Open `/etc/default/grub.d/98-vm-realtime.cfg` in a text editor and insert the following line:

```shell
GRUB_CMDLINE_LINUX_DEFAULT="$GRUB_CMDLINE_LINUX_DEFAULT clocksource=tsc tsc=reliable nmi_watchdog=0 nosoftlockup kthread_cpus=0-7 isolcpus=domain,managed_irq,8-15 rcu_nocb_poll rcu_nocbs=8-15 nohz=on nohz_full=8-15 irqaffinity=0-7 idle=poll"
```

Apply the changes made to the kernel command line parameters and reboot:

```shell
sudo update-grub
sudo reboot
```

Verify the kernel boot command line:

```{terminal}
    :input: cat /proc/cmdline
    :user: ubuntu
    :host: ubuntu
BOOT_IMAGE=/vmlinuz-6.14.0-28-generic root=/dev/mapper/ubuntu--vg-ubuntu--lv ro quiet splash clocksource=tsc tsc=reliable nmi_watchdog=0 nosoftlockup kthread_cpus=0-7 isolcpus=domain,managed_irq,8-15 rcu_nocb_poll rcu_nocbs=8-15 nohz=on nohz_full=8-15 irqaffinity=0-7 idle=poll
```

Check which CPUs the kernel considers isolated (`8-15`):

```{terminal}
    :input: cat /sys/devices/system/cpu/isolated
    :user: ubuntu
    :host: ubuntu
8-15
```

### Install dependencies

Use the following command to install the packages required for `KVM` and `virsh`:

```shell
sudo apt update
sudo apt install util-linux wget whois qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils qemu-utils virtinst libosinfo-bin osinfo-db-tools genisoimage
```

Add current user to the `libvirt` group:

```shell
sudo usermod --append --groups libvirt "$(whoami)"
```

Refresh group membership:

```shell
newgrp libvirt
```

Ensure that `libvirt` and the default NAT network are running:

```shell
systemctl status libvirtd --no-pager
virsh net-start default || true
```

## Create VM image

This section describes how to create a new real-time VM image using
virsh and cloud-init.

Create the base image directory if it doesn't exist and navigate to that directory:

```shell
sudo mkdir -p /var/lib/libvirt/images/ubuntu-rt-vm
cd /var/lib/libvirt/images/ubuntu-rt-vm
```

Download the Ubuntu 24.04 LTS cloud image:

```{terminal}
    :input: sudo wget -q --show-progress -O ubuntu-cloud.img https://cloud-images.ubuntu.com/noble/current/noble-server-cloudimg-amd64.img
    :user: ubuntu
    :host: ubuntu
    :dir: /var/lib/libvirt/images/ubuntu-rt-vm
ubuntu-cloud.img          100%[====================================================================>] 590.25M  30.4MB/s    in 20s     
```

```{hint}
Using cloud images makes it straightforward to automate the initial configuration and real-time tuning steps (kernel parameters, user setup, package installs).
```

Create a `20GB` size `qcow2` disk based on the downloaded base image:

```{terminal}
    :input: sudo qemu-img create -f qcow2 -F qcow2 -o backing_file=ubuntu-cloud.img ubuntu-rt-vm.qcow2
    :user: ubuntu
    :host: ubuntu
    :dir: /var/lib/libvirt/images/ubuntu-rt-vm
Formatting 'ubuntu-rt-vm.qcow2', fmt=qcow2 cluster_size=65536 extended_l2=off compression_type=zlib size=3758096384 backing_file=ubuntu-cloud.img backing_fmt=qcow2 lazy_refcounts=off refcount_bits=16
```

```{terminal}
    :input: sudo qemu-img resize ubuntu-rt-vm.qcow2 20G
    :user: ubuntu
    :host: ubuntu
    :dir: /var/lib/libvirt/images/ubuntu-rt-vm
Image resized.
```

### Create `cloud-init` image

A `cloud-init` image is required to provide initial configuration to the VM, such as user credentials, package installation, and kernel parameters.

Open `/var/lib/libvirt/images/ubuntu-rt-vm/user-data` in a text editor and insert the following content:

```{literalinclude} create-rt-ubuntu-vm-using-kvm/user-data
:language: yaml
```

Open `/var/lib/libvirt/images/ubuntu-rt-vm/meta-data` in a text editor and insert the following content:

```{literalinclude} create-rt-ubuntu-vm-using-kvm/meta-data
:language: yaml
```

Generate `seed.iso` with {file}`user-data` and {file}`meta-data` files:

```{terminal}
    :input: sudo genisoimage -input-charset "utf-8" -volid cidata -joliet -rock -output seed.iso user-data meta-data
    :user: ubuntu
    :host: ubuntu
    :dir: /var/lib/libvirt/images/ubuntu-rt-vm
Total translation table size: 0
Total rockridge attributes bytes: 331
Total directory bytes: 0
Path table size(bytes): 10
Max brk space used 0
183 extents written (0 MB)
```

```{terminal}
    :input: ls -l
    :user: ubuntu
    :host: ubuntu
    :dir: /var/lib/libvirt/images/ubuntu-rt-vm
total 605000
-rw-r--r-- 1 root root        55 Aug 28 13:32 meta-data
-rw-r--r-- 1 root root    374784 Aug 28 13:32 seed.iso
-rw-r--r-- 1 root root 618925568 Aug  5 15:22 ubuntu-cloud.img
-rw-r--r-- 1 root root    262464 Aug 28 12:50 ubuntu-rt-vm.qcow2
-rw-r--r-- 1 root root       586 Aug 28 13:32 user-data
```

## Install and define VM image

Run the following commands to define the VM image:

```{terminal}
    :input: sudo virt-install --connect qemu:///system --virt-type kvm --name ubuntu-rt-vm --vcpus 8 --ram 8192 --os-variant ubuntu24.04 --disk path=ubuntu-rt-vm.qcow2,format=qcow2 --disk path=seed.iso,device=cdrom --import --network network=default --noautoconsole --nographics --print-xml | sudo tee /etc/libvirt/qemu/ubuntu-rt-vm.xml
    :user: ubuntu
    :host: ubuntu
    :dir: /var/lib/libvirt/images/ubuntu-rt-vm
<domain type="kvm">
  <name>ubuntu-rt-vm</name>
  <uuid>af8bd2eb-7151-4aec-92a7-ffc61f2df203</uuid>
  <metadata>
    <libosinfo:libosinfo xmlns:libosinfo="http://libosinfo.org/xmlns/libvirt/domain/1.0">
      <libosinfo:os id="http://ubuntu.com/ubuntu/24.04"/>
[...]
</domain>
```

```{terminal}
    :input: sudo virsh define /etc/libvirt/qemu/ubuntu-rt-vm.xml
    :user: ubuntu
    :host: ubuntu
    :dir: /var/lib/libvirt/images/ubuntu-rt-vm
Domain 'ubuntu-rt-vm' defined from /etc/libvirt/qemu/ubuntu-rt-vm.xml
```

The VM is defined meaning its configuration (the XML definition) is registered within the hypervisor.

### Apply real-time tuning

In this section, real-time tuning will be applied by mapping the host's isolated CPUs directly to the VM virtual CPUs, ensuring that the VM has dedicated processing resources.

Edit VM configuration:

```shell
sudo virsh edit ubuntu-rt-vm
```

Determine how many vCPU to assign to the real-time VM:

```xml
<vcpu placement='static'>8</vcpu>
```

The `<cputune>` section assigns specific host CPUs to the VM vCPUs and applies real-time scheduling policies.

Configure `<cputune>` section:

```xml
<cputune>
    <vcpupin vcpu='0' cpuset='8'/>
    <vcpupin vcpu='1' cpuset='9'/>
    <vcpupin vcpu='2' cpuset='10'/>
    <vcpupin vcpu='3' cpuset='11'/>
    <vcpupin vcpu='4' cpuset='12'/>
    <vcpupin vcpu='5' cpuset='13'/>
    <vcpupin vcpu='6' cpuset='14'/>
    <vcpupin vcpu='7' cpuset='15'/>
    <emulatorpin cpuset='0-7'/>
    <vcpusched vcpus='0' scheduler='fifo' priority='1'/>
    <vcpusched vcpus='1-7' scheduler='fifo' priority='10'/>
</cputune>
```

```{note}
This configuration:

-   Ensures 1:1 CPU pinning, where each VM vCPU is tied to a dedicated isolated host CPU `8–15`.
-   Keeps the emulator and host background tasks on non-isolated CPUs `0–7`.
-   Assigns vCPU 0 a lower FIFO priority for housekeeping inside the VM.
-   Gives vCPUs `1–7` a higher FIFO priority so real-time applications run with minimal latency.
```

Set the VM to use the host CPU model in passthrough mode, ensuring the VM sees the same instruction set and features as the host.
Match the CPU topology to the number of vCPUs assigned.

Remove any existing `<cpu>` block and replace it with the following:

```xml
<cpu mode='host-passthrough' check='none' migratable='off'>
    <topology sockets='1' dies='1' cores='8' threads='1'/>
    <feature policy='require' name='tsc-deadline'/>
</cpu>
```

Ensure that the VM memory is directly allocated and locked into RAM, preventing paging and sharing with other VMs.
Insert the `<memoryBacking>` element immediately after the `<currentMemory>` block:

```xml
<memoryBacking>
    <nosharepages/>
    <locked/>
</memoryBacking>
```

Turn off Power Management Unit (PMU) virtualization inside the VM to avoid unnecessary overhead.
Add the `<pmu>` entry within the `<features>` section:

```xml
<features>
    <pmu state='off'/>
    <...>
</features>
```

Prevent the hypervisor from reclaiming memory from the VM, ensuring the real-time VM always has exclusive use of its allocated RAM.
Remove any existing `<memballoon>` definition and replace it with the following under `<devices>`:

```xml
<devices>
    <memballoon model='none'/>
    <...>
</devices>
```

For more information, refers to [libvirt documentation](https://libvirt.org/formatdomain.html).

Redefine the VM with updated XML:

```shell
sudo virsh define /etc/libvirt/qemu/ubuntu-rt-vm.xml
```

### Start and connect to the VM

Start `ubuntu-rt-vm` VM and ensure it is running:

```{terminal}
    :input: sudo virsh start ubuntu-rt-vm
    :user: ubuntu
    :host: ubuntu
    :dir: /var/lib/libvirt/images/ubuntu-rt-vm
Domain 'ubuntu-rt-vm' started
```

```{terminal}
    :input: virsh list
    :user: ubuntu
    :host: ubuntu
    :dir: /var/lib/libvirt/images/ubuntu-rt-vm
 Id   Name            State
-------------------------------
 1    ubuntu-rt-vm    running
```

Connect to the VM console and login using `ubuntu` as user and `ubuntu` as password

```{warning}
The VM will reboot after booting first time to install real-time kernel, make sure to let some time before logging in.
```

```shell
sudo virsh console ubuntu-rt-vm
```

Ensure real-time kernel version is running:

```{terminal}
    :input: uname -a
    :user: ubuntu
    :host: ubuntu-rt-vm
Linux ubuntu-rt-vm 6.8.1-1015-realtime #16-Ubuntu SMP PREEMPT_RT Wed Jan 15 21:03:54 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
```

Check kernel boot line includes `isolcpus=1-7`:

```{terminal}
    :input: cat /proc/cmdline
    :user: ubuntu
    :host: ubuntu-rt-vm
BOOT_IMAGE=/vmlinuz-6.8.1-1015-realtime root=UUID=d36f7414-beb7-45e0-900e-9ab79cdbcb2d ro console=tty1 console=ttyS0 kthread_cpus=0 irqaffinity=0 isolcpus=domain,managed_irq,1-7 rcu_nocb_poll rcu_nocbs=1-7 nohz=on nohz_full=1-7
```

Check isolated CPUs (`1-7`):

```{terminal}
    :input: cat /sys/devices/system/cpu/isolated
    :user: ubuntu
    :host: ubuntu-rt-vm
1-7
```

## Benchmark the VM

Refer to [How to measure maximum latency in a real-time system](measure-maximum-latency)
