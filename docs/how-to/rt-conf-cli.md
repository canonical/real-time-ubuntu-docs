# How-to tune Real-Time Ubuntu using rt-conf

RT-conf is an application that helps users tune their system for real-time responsiveness.  
This guide provides usage instructions for its command-line interface.

## Install `rt-conf` snap

The `rt-conf` application is available as a snap and can be installed with:

```shell
sudo snap install rt-conf
```
<!-- TODO: Complement information with supported systems once the tool is in a stable state -->

## Run in Terminal UI mode

The `rt-conf` also provides a [terminal UI interface][tui_wikipedia], for iterative configuration.
To be activated, it's only necessary to run the `rt-conf` tool, without any flags or arguments:
```shell
rt-conf
```

## Get help

To print help information regarding the usage info, use `-h` or `--help` flag.

```shell
rt-conf --help
```

## Define configuration file

There are three ways to define the YAML configuration file:

- Relying on the defined by `DEFAULT_CONF` environment variable.
  Which defaults to `$SNAP_COMMON/config.yaml`. 
  Checkout [snap environment variables documentation][snap_env_variables].

```shell
rt-conf --conf
```

- Pass using the `--conf` flag:
```shell
rt-conf --conf=<custom-config>.yaml
```

## Edit configuration file

The actions made by the `rt-conf` tool are mainly driven by the options set in the YAML configuration file.
Checkout the [configuration schema][config_yaml] for all the possible configuration keys on the YAML configuration file.

### Configure kernel command line args

Kernel command line are used to tune the system in a static way.
Every change needs a reboot, to take effect.
After boot, to revert the change, it's necessary to change back and reboot again.
Here are some examples of useful configurations.

  - CPU isolation with `isolcpus`.
    Useful to run real-time workloads on the isolated CPUs.
    ```yaml
    kernel_cmdline:
      isolcpus: "3-4"
    ```

  - Disable dynamic ticks on given CPUs to reduce OS jitter:
    ```yaml
    kernel_cmdline:
      nohz: "on"
      nohz_full: "3-4"
    ```

  - Allocate IRQ affinity to CPUs not used by the real-time workloads:
    ```yaml
    kernel_cmdline:
      irqaffinity: "0-2,5-n"
    ```

### Fine tune IRQ affinity

Some workloads may require specific IRQ affinity tuning.
Like being able to handle IRQs coming from a specific network card interface on an isolated CPU running a real-time workload.
For that, the `irq_tunning` top level field configuration can be set.

Examples:

- Handle IRQs coming from wifi interface on specific CPUs:
  ```yaml
  irq_tuning:
    - cpus: "3-4"
      filter:
        actions: "iwlwifi"
  ```

```{note}
The matching filters names are the same as you can found on the [sysfs ABI for kernel/irq directory][sysfs-abi] (excluding `hwirq` and `wakeup`).
```

### Fine tune CPU frequency scaling

Some heavy real-time workloads may benefit from being handle on CPUs with higher frequencies.
Doing do by running them in `performance` mode.

The CPU scaling governor can be set using `cpu_governance` top level field configuration.
The specified CPUs are the ones which will receive the set scaling governor.

```yaml
cpu_governance: 
  - cpus: "3-4"
    scaling_governor: "performance"
```

## Debug mode

To enable debug logging set the `DEBUG` environment variable to `true`. For example:

```shell
DEBUG=true rt-conf --conf
```

<!-- TODO: necessary to add session reagarding connection of snap interfaces. -->

## Execute application

Example of `rt-conf` execution, with kernel command line and IRQ affinity tuning settings.

Used configuration YAML:
```yaml
kernel_cmdline:
  isolcpus: "1-2"
  nohz: "on"
  nohz_full: "1-2"
  kthread_cpus: "0,3"
  irqaffinity: "0,3"

irq_tuning:
  - cpus: "2"
    filter:
      actions: "acpi"
```

Executign `rt-conf` from configuration file located ate home directory:

```{terminal}
   :input: sudo rt-conf --config=$HOME/config.yaml
   :user: ubuntu
   :host: ubuntu
   :dir: ~

2025/04/02 14:44:59 Final kcmdline: nohz_full=1-2 kthread_cpus=0,3 irqaffinity=0,3 isolcpus=1-2 nohz=on
Detected bootloader: GRUB
Updated default grub file: /etc/default/grub

Please run:

	sudo update-grub

to apply the changes to your bootloader.
2025/04/02 14:44:59 Set /proc/irq/9/smp_affinity_list to 2
```

% Links
[snap_env_variables]: https://snapcraft.io/docs/environment-variables
[tui_wikipedia]: https://en.wikipedia.org/wiki/Text-based_user_interface
[GRUB]: https://www.gnu.org/software/grub/
[ubuntu_desktop]: https://ubuntu.com/download/desktop
[ubuntu_server]: https://ubuntu.com/download/server
[uc]: https://ubuntu.com/core

[config_yaml]: https://documentation.ubuntu.com/real-time/en/rt-conf/reference/rt-conf-yaml/
[sysfs-abi]: https://github.com/torvalds/linux/blob/master/Documentation/ABI/testing/sysfs-kernel-ir