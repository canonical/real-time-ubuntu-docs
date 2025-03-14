# rt-conf config.yaml schema

The following schema reflects all configuration keys available in `config.yaml` for `rt-conf` tool

## Top-level directives

There are three top-level dictionaries in `config.yaml`, {ref}`kcmd`, {ref}`irqt` and {ref}`cpug`.

(kcmd)=
### kernel_cmdline

Type: `dict`

_Optional_

{ref}`kernel-boot-parameters` that affects real-time behavior


#### isolcpus

Type: `string`

_Optional_

A string formated as a [cpu-list](https://www.kernel.org/doc/html/v5.15/admin-guide/kernel-parameters.html#cpu-lists).
Isolate CPUs from general execution.

#### nohz 

Type: `enum`

_Optional_

Enable/disable dynamic ticks during idle time.

Valid values are:
  * `on`: **Enables** dynamic ticks
  * `off`:**Disables** dynamic ticks

#### nohz_full

Type: `string`

_Optional_

A string formated as a [cpu-list](https://www.kernel.org/doc/html/v5.15/admin-guide/kernel-parameters.html#cpu-lists).
Specifies the adaptive-ticks cpus, which means the specified list of CPUs whose tick will be stopped whenever possible.
The boot CPU will be forced outside the range to maintain the timekeeping.

#### kthread_cpus

Type: `string`

_Optional_

A string formated as a [cpu-list](https://www.kernel.org/doc/html/v5.15/admin-guide/kernel-parameters.html#cpu-lists).
Specifies the list of CPUs to be alocated for kernel threads.

#### irqaffinity

Type: `string`

_Optional_

A string formated as a [cpu-list](https://www.kernel.org/doc/html/v5.15/admin-guide/kernel-parameters.html#cpu-lists).
Specifies the list of CPUs for IRQ handling.

(irqt)=
### irq_tunning 

Type: `list[dict]`

_Optional_

A list of configurations including the list of cpus to be aplied and the filters.
Runtime configurations that aren't persisted, related to IRQ affinity tunning.

Example:

```yaml
irq_tunning:
  - cpus: "2-3"
    filter:
      actions: "iwlwifi"
      chip_name: "IR-PCI"
      name: "edge"
      type: "edge"
```

#### irq_tunning.<list item>.cpus

Type `string`

_Required_

A string formated as a [cpu-list](https://www.kernel.org/doc/html/v5.15/admin-guide/kernel-parameters.html#cpu-lists).
Specifies the list of CPUs which will handle the matched IRQs on the {ref}`filter <irqfilter>`.

(irqfilter)=
#### irq_tunning.<list item>.filter
Type: `dict`

_Required_

A dictionary with keys related to IRQ properties of `/sys/kernel/irq/<IRQ-num>/`.

##### irq_tunning.<list item>.filter.actions
Type: `regex string`

_Optional_

The IRQ action chain. A comma-separated list of zero or more device names associated with this interrupt.
For network related, generary is the name of the network interface shown in `ip link show`. 

##### irq_tunning.<list item>.filter.chip_name
Type: `regex string`

_Optional_

Chip name supplied by the associated device driver.

Example: `IR-PCI-MSIX-0000:04:00.0`

##### irq_tunning.<list item>.filter.name
Type: `regex string`

_Optional_

Human-readable flow handler name as defined by the irq chip driver.
Commun values are:
  * `edge`
  * `fasteoi`

##### irq_tunning.<list item>.filter.type
Type: `enum`

_Optional_

The type of the interrupt.
Valid values:
  * `edge`
  * `level` 

(cpug)=
### cpu_governance

Type: `list[dict]`

_Optional_

Runtime configurations that aren't persisted, related to cpu power management.
A list of dictionaries with the CPU scaling governor and the cpu list to be aplied.

Example:

```yaml
cpu_governance: 
  - cpus: "0-1"
    scaling_governor: "performance"
```

#### cpu_governance.<list item>.cpus

Type: `string`

_Required_

A string formated as a [cpu-list](https://www.kernel.org/doc/html/v5.15/admin-guide/kernel-parameters.html#cpu-lists).
Specifies which cpus are going to be configured with the scaling governor specified in the item.


#### cpu_governance.<list item>.cpus

Type: `string`

_Required_

The chosen scaling governor.

Valid values:
  * `performance`: Run the CPU at the maximum frequency, get from `/sys/devices/system/cpu/cpuX/cpufreq/scaling_max_freq`.
  * `powersave`: Run the CPU at the minimum frequency, get from `/sys/devices/system/cpu/cpuX/cpufreq/scaling_min_freq`. 
  * `userspace`: Run the CPU at user specified frequencies, configurable via `/sys/devices/system/cpu/cpuX/cpufreq/scaling_setspeed`. 
  * `ondemand`: Scales the frequency dynamically according to current load. Jumps to the highest frequency and then possibly back off as the idle time increases.
  * `conservative`: Scales the frequency dynamically according to current load (more gradually than ondemand).
  * `schedutil`: [Scheduler-driven](https://lwn.net/Articles/682391/) CPU frequency selection.

