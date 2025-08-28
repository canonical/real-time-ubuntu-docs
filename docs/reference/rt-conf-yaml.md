# RT Conf YAML configuration schema

The following schema reflects all settings available in the configuration file of the `rt-conf` tool

There are three top-level dictionaries in the configuration file:, {ref}`kcmd`, {ref}`irqt` and {ref}`cpug`.

(kcmd)=
## kernel_cmdline

Type: `dict`

_Optional_

### kernel_cmdline.append

Type: `list`

{ref}`kernel-boot-parameters` that affects real-time behavior

```{admonition} Validated parameters

Some parameters are well-known to be valuable for real-time tuning.
So in case of present on the `kernel_cmdline.parameters` list, their **values** are also validated.

| Parameter | Type | Description |
|-----------|------|-------------|
| isolcpus | `string` | A string formatted as {ref}`cpu-lists`. Isolate CPUs from general execution. |
| nohz | `enum` | Enable/disable dynamic ticks during idle time. Valid values are: `on` (**Enables** dynamic ticks), `off` (**Disables** dynamic ticks) |
| nohz_full | `string` | A string formatted as {ref}`cpu-lists`. Specifies the adaptive-ticks cpus, which means the specified list of CPUs whose tick will be stopped whenever possible. The boot CPU will be forced outside the range to maintain the timekeeping. |
| kthread_cpus | `string` | A string formatted as {ref}`cpu-lists`. Specifies the list of CPUs to be allocated for kernel threads. |
| irqaffinity | `string` | A string formatted as {ref}`cpu-lists`. Specifies the list of CPUs for IRQ handling. |
| rcu_nocbs | `string` | A string formatted as {ref}`cpu-lists`. Enables the no-callback CPU mode, which prevents such CPUs from executing [RCU callbacks][rcu_callbacks]. |
```

(irqt)=
## irq_tuning 

Type: `list[dict]`

_Optional_

A list of configurations including the list of cpus to be applied and the filters.
Runtime configurations that aren't persisted, related to IRQ affinity tuning.

Example:

```yaml
irq_tuning:
  - cpus: "2-3"
    filter:
      actions: "iwlwifi"
      chip_name: "IR-PCI"
      name: "edge"
      type: "edge"
```

### irq_tuning[*].cpus

Type `string`

_Required_

A string formatted as {ref}`cpu-lists`. 
Specifies the list of CPUs which will handle the matched IRQs on the {ref}`filter <irqfilter>`.

(irqfilter)=
### irq_tuning[*].filter

Type: `dict`

_Required_

A dictionary with keys related to IRQ properties of `/sys/kernel/irq/<IRQ-num>/`.

#### irq_tuning[*].filter.actions

Type: `regex string`

_Optional_

The IRQ action chain. A comma-separated list of zero or more device names associated with this interrupt.
For network related, generally is the name of the network interface shown in `ip link show`. 

#### irq_tuning[*].filter.chip_name

Type: `regex string`

_Optional_

Chip name supplied by the associated device driver.

Example: `IR-PCI-MSIX-0000:04:00.0`

#### irq_tuning[*].filter.name

Type: `regex string`

_Optional_

Human-readable flow handler name as defined by the irq chip driver.
Example values are:
  * `edge`
  * `fasteoi`

#### irq_tuning[*].filter.type

Type: `enum`

_Optional_

The type of the interrupt.
Valid values:
  * `edge`
  * `level` 

(cpug)=
## cpu_governance

Type: `list[dict]`

_Optional_

Runtime configurations that aren't persisted, related to cpu power management.
A list of dictionaries with the CPU scaling governor and the cpu list to be applied.

Example:

```yaml
cpu_governance: 
  - cpus: "0-1"
    scaling_governor: "performance"
```

### cpu_governance[*].cpus

Type: `string`

_Required_

A string formatted as {ref}`cpu-lists`. 
Specifies which cpus are going to be configured with the scaling governor specified in the item.


### cpu_governance[*].scaling_governor

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


<!-- Links -->
[rcu_callbacks]: https://wiki.linuxfoundation.org/realtime/documentation/technical_details/rcu

