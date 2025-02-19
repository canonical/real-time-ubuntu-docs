# Speed Shift for Edge Computing

In the third and final step of this tutorial, let's examine another aspect of power management: *Performance states, or P-States*.

P-States enable the scaling of the processor's frequency and voltage to reduce CPU power consumption.
They are part of the Dynamic Voltage and Frequency Scaling (DVFS) features such as Intel® Speed Step, Speed Shift, and Turbo Boost Technology.
Speed Step and Speed Shift adjust the processor's voltage and frequency within these P-States to balance power efficiency and performance, while Turbo Boost allows the processor to temporarily exceed the highest P-State to provide additional performance during demanding tasks.

Until the 11th generation of Intel Core processors, for hard real-time use cases, it was recommended to disable all DVFS features in the BIOS, which would lock the frequency of all cores permanently to the base frequency.
Starting with the 11th generation of Intel Core processors, P-State transitions were optimized.
As a result, from the 11th generation onward, Intel® Speed Step, Speed Shift, and Turbo Boost Technologies are no longer disabled if you enable *Intel® TCC Mode* in the BIOS.
You still have the option to lock core frequency during runtime using the HWP MSRs or the intel_pstate driver under Linux.

With this knowledge, let's revisit the performance metrics.
First we lock the core frequency of all cores to the base frequency.
Then we boost the frequency of the real-time core to a value within the turbo frequency range to leverage higher single-threaded performance.
We follow the recommendations for the enveloping frequency configurations which are listed in the [TCC User Guide](https://cdrdv2.intel.com/v1/dl/getContent/831067?fileName=Public+TCC+User+Guide+-+September+2024+-+RDC-831067) for the specific processor SKU.

More information about HWP and the MSR can be found in the Intel® 64 and IA-32 Architectures Software Developer’s Manual Vol3 section "Power and Thermal Management-Hardware Controlled Performance States - RDC#[671200](https://cdrdv2.intel.com/v1/dl/getContent/671200) 

```{figure} images/tcc_setup_CAT_isol_boost.svg
   :width: 100%
   :alt: Speed Shift - System Setup

   Speed Shift - System Setup
```

## Run the experiment

1. Start the real-time application if it is not already running, and output the statistics to the Grafana dashboard.
   ```bash
   sudo ./rt_linux_tutorial -i 1000 -s 1
   ```

   Monitor the statistics and in a second terminal start *stress-ng* with a memory centric stressor.
   ```bash
   stress-ng --vm 8 --vm-bytes 128M --fork 4
   ```

   Ensure that CAT partitioning is still applied or run the following script again.
   ```bash
   sudo ./setCacheAllocation.sh rt_optimized
   ```

2. Lock the core frequency of all cores to base frequency of the core and tune the Energy Performance Preferences (EPP) towards `performance`.
   This complies with disabling DVFS features in BIOS.
   
   Here is an example of how you can use the `sysfs` entries of `intel_pstate` driver:
    ```bash
    # Loop through each CPU core and set the min and max frequencies to base frequency
    for CPU in /sys/devices/system/cpu/cpu[0-11]*; do
        BASE_FREQUENCY=$(cat $CPU/cpufreq/base_frequency)
        echo $BASE_FREQUENCY | sudo tee $CPU/cpufreq/scaling_min_freq
        echo $BASE_FREQUENCY | sudo tee $CPU/cpufreq/scaling_max_freq
        echo performance | sudo tee $CPU/cpufreq/energy_performance_preference
    done
    ```

    Alternatively, you can use the script with `basefrequency` parameter:
    ```sh
    sudo ./setCoreFrequency.sh basefrequency
    ``` 

3. Boost the frequency of the core running the real-time application as described in the enveloping configuration in the TCC User Guide.
   In this configuration, the maximum allowed frequency of all best-effort cores is limited to the base frequency, and the Energy Performance Preferences (EPP) is set to power.
   This setup allows the best-effort cores to scale their frequency between the minimum and base frequency depending on core utilization.
   For the real-time core, the frequency is boosted to 3.1 GHz, and the EPP is set to performance to ensure Quality of Service (QoS) in case of power limit throttling.

   Use the script with `rt-boost`, followed by the identfier of the real-time core and the desired core frequency:
   ```sh
   sudo ./setsetCoreFrequency.sh rt-boost '3' 3100000 3100000
   ``` 

   ```{note}
   Note: The script and the specified frequencies are examples tailored for this tutorial and the Intel® Core™ i5-1350PE processor. You may need to adapt them to match your processor and use case.
   ```

For more information on directly accessing the HWP MSR instead of using the `sysfs` entries of the `intel_pstate driver`, please refer to the [TCC User Guide](https://cdrdv2.intel.com/v1/dl/getContent/831067?fileName=Public+TCC+User+Guide+-+September+2024+-+RDC-831067) and in the Intel® 64 and IA-32 Architectures Software Developer’s Manual Vol3 section "Power and Thermal Management-Hardware Controlled Performance States - RDC#[671200](https://cdrdv2.intel.com/v1/dl/getContent/671200) 


## Results

```{figure} images/result_CAT_isol_boost.png
   :width: 100%
   :alt: Speed Shift - Results

   Speed Shift - Results
```

Examining the performance metrics, you can observe the following:

- In the first step of the graph, where HWP was still able to scale the P-State, the execution time jitter band remains noisy.
- Step 2 shows that locking the frequency of all cores to the base frequency reduces the jitter band but also significantly increases the execution time.
- Finally, in step 3, locking the core frequency of the core running the real-time application to a turbo frequency and limiting the maximum allowed frequency of the best-effort (BE) cores to the base frequency, following the guidance of the enveloping configurations listed in the TCC User Guide, results in a narrower execution time jitter band and a significantly lower execution time.

As you can see, locking the core frequency of the core running the real-time application helps reduce execution time jitter. Additionally, boosting the core frequency of the real-time core can be highly beneficial for use cases that require higher single-threaded performance.
