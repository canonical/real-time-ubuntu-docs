# Default Configuration

```{figure} images/tcc_setup_basic.drawio.svg
   :width: 100%
   :alt: Basic - System Setup

   Basic - System Setup
```

## Lock All Cores to Base Frequency

So far the recommendation to avoid any impact on real-time (RT) performance due to x86 power management features, has been to lock all cores to their base frequency. There are two methods to achieve this.

### Method 1 - Disable DVFS Features in BIOS (Legacy Method)
Turn off Dynamic Voltage and Frequency Scaling (DVFS) features such as Intel Speed Step, Speed Shift, or Turbo Boost technology directly in the BIOS settings.

### Method 2 - Enable DVFS Features and Configure via `intel_pstate` driver

1. Keep DVFS features like Intel Speed Step, Speed Shift, or Turbo Boost technology enabled in the BIOS.
    
2. Use the `sysfs` attributes of the `intel_pstate` driver to set both the minimum (`scaling_min_freq`) and maximum (`scaling_max_freq`) allowed frequencies to the base frequency (`base_frequency`).
```sh
cpu_dir = /sys/devices/system/cpu/cpu<core id>/cpufreq
# Read the base frequency value
base_freq=$(cat "$cpu_dir/base_frequency")

# Write the base frequency value for each core to scaling_max_freq
sudo bash -c "echo $base_freq > $cpu_dir/scaling_max_freq"
# Write the base frequency value for each core to scaling_min_freq
sudo bash -c "echo $base_freq > $cpu_dir/scaling_min_freq"
```
    
3. Optimize core performance by setting the Energy Performance Preferences attribute (`energy_performance_preference`) to 0 (`performance`).
```sh
cpu_dir = /sys/devices/system/cpu/cpu<core id>/cpufreq
# Set energy_performance_preference to "performance" for each core by default
sudo bash -c "echo performance > $cpu_dir/energy_performance_preference"
```
    
4. Alternatively, you can directly access the Hardware-Controlled Performance (HWP) model-specific registers. For more details, refer to the [Intel Software Developer's Manual Vol.3 section "Hardware-Controlled Performance States"](https://cdrdv2.intel.com/v1/dl/getContent/671200)  or the [TCC User Guide](https://cdrdv2.intel.com/v1/dl/getContent/831868?explicitVersion=true). 


````{note}
A convenience script is provided to automate these steps.
Simply use the script with `basefrequency` as input parameter to lock all cores to the respective base frequency.

```sh
sudo ./setCoreFrequency.sh basefrequency
```
````

## Run first Test
Start real-time application and check the statistics on the Grafana dashboard. 
```sh
sudo ./rt_linux_tutorial -s 1
```