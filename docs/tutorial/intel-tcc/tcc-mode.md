# Time Coordinated Computing mode

Let's take a look at the *Intel® TCC Mode* and how this single UEFI BIOS/Slim Bootloader (SBL) knob can help to quickly optimize firmware settings for low latency.
The *Intel® TCC Mode* should be available on all *Intel® TCC* enabled platforms.

With the Intel® reference BIOS, you can enable *Intel® TCC Mode* by navigating to {menuselection}`Intel® Advanced Menu --> Time Coordinated Computing`. 
If *Intel® TCC Mode* is not visible, consult your board vendor or follow the steps listed in the *Firmware Configuration* section of [TCC User Guide](https://www.intel.com/content/www/us/en/content-details/851159/public-intel-time-coordinated-compute-tcc-user-guide.html?wapkw=831067) to set the options manually.

Intel® TCC Mode includes a wide range of real-time optimizations including the following:

- Power Management, for example:
  - Handling of power states 
  - Optimization of frequency transitions 
- Configuration of TCC features, for example:
  - Limiting amount of cache available to the GPU.
  - Setting up virtual channels for VC-capable endpoints.

```{note}
If your system does not have the *Intel® TCC Mode* option it its BIOS, you can still continue with this tutorial.
Just skip ahead to {doc}`Cache partitioning using Intel CAT </tutorial/intel-tcc/intel-cat>`.
```

Now, let's take a concrete example and see how the Intel® TCC Mode optimizes power states for low latency.
To do this, we will compare the statistics of the test application with the default settings of the Intel® reference BIOS, which includes Intel® TCC Mode disabled, and with Intel® TCC Mode enabled.

The block diagram below illustrates the setup used for the *Intel® TCC Mode* analysis.

```{figure} images/tcc_setup_basic.svg
   :width: 100%
   :alt: Basic system setup

   Basic system setup
```
 
## Run experiment

1. Boot the system with the default BIOS options.
   Start the real-time application with a cycle time of 1ms, and output the statistics to the Grafana dashboard.
   Let the test application run for some seconds and monitor the statistics.

   ```sh
   sudo ./rt_linux_tutorial -i 1000 -s 1
   ```

2. Reboot the system and enable *Intel® TCC Mode* in the BIOS.
   Then, restart the test application with the same command line as before.

## Results

```{figure} images/results_basic.png
   :width: 100%
   :alt: TCC Mode results

   TCC Mode results
```

Looking at the performance metrics of the real-time application -- execution time, scheduling jitter, and cache misses -- shows that, in general, all three are still a bit noisy.
The most significant change can be observed in the scheduling jitter of the real-time thread.
With *Intel® TCC Mode* disabled, the average scheduling jitter is around 100µs, while with *Intel® TCC Mode* enabled, the jitter is in the sub-10µs range.

The improvement is a result of the power state optimizations of the *Intel® TCC Mode*.
The *Intel® TCC Mode* disables C-States and all related sub-configuration options.  

C-States are power-saving modes in x86 processors that reduce power consumption and thermal output by varying the levels of inactivity and shutting down different parts of the CPU when it is idle.
Each C-State has a target residency, which is the minimum amount of time the CPU should remain in that state, and an exit latency, which is the time it takes for the CPU to return to an active state (C0) from that power-saving mode.
Deeper C-States provide greater power savings but have higher exit latencies, meaning it takes longer for the CPU to become fully operational again. 
To check if C-States are enabled, see which C-States are supported, and view their target residency and exit latency, you can use a small script like this for example:

```bash
for cpu in /sys/devices/system/cpu/cpu*/cpuidle/state*; do
    echo -n "$cpu: "; cat $cpu/name
    echo -n "  Target residency: "; cat $cpu/residency
    echo -n "  Exit latency: "; cat $cpu/latency
    echo -n "  State [disabled=1 enabled=0]: "; cat $cpu/disable
done
```

In the test case above, the core is isolated, meaning only the real-time application is running on the physical core.
The average execution latency is around 300µs, and the cycle time is 1ms.
This results in the core idling for approximately 700µs most of the time, allowing the Linux idle subsystem to send the core to a deeper C-State to save power.
Deeper C-States have higher exit latencies, which is why the scheduling jitter is significantly higher.

Keep in mind that this is one example.
The behavior highly depends on the workload characteristics and the overall use-case.
However, this example demonstrates that *Intel® TCC Mode* makes it easy to optimize the processor for low latency with just one setting in the firmware.

Now, let's see how you can further optimize real-time performance and improve temporal isolation between best-effort workloads and the real-time application using Intel® TCC features.
