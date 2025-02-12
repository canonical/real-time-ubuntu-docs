# Apply Speed Shift for Edge to boost the Real-time App

```{figure} images/tcc_setup_CAT_isol_boost.drawio.svg
   :width: 100%
   :alt: Speed Shift - System Setup

   Speed Shift - System Setup
```

With Speed Shift for Edge Computing, the frequency of the real-time core can be boosted to leverage higher single-threaded performance.
This tutorial follows the recommendations for the enveloping configuration listed in the [TCC User Guide](https://www.intel.com/content/www/us/en/content-details/831067/public-intel-time-coordinated-compute-tcc-user-guide.html) and in the Intel® 64 and IA-32 Architectures Software Developer’s Manual Vol3 section "Power and Thermal Management-Hardware Controlled Performance States - RDC#[671200](https://cdrdv2.intel.com/v1/dl/getContent/671200)".

Therefore, the maximum allowed frequency of all best-effort cores is limited to the base frequency, and the Energy Performance Preferences (EPP) is set to `power`. This configuration allows the best-effort cores to scale their frequency between the minimum and base frequency depending on core utilization.

For the real-time core, the frequency is boosted to 3.1GHz, and the Energy Performance Preferences (EPP) is set to `performance` to ensure Quality of Service (QoS) in case of power limit throttling.

````{note}
This configuration can be applied by using the convenience script, with the `rt_boost` argument, followed by the identifier of the real-time core, and the core frequency:

```sh
sudo ./setCoreFrequency.sh rt_boost '3' 3100000
```
````

Start the real-time application if it is not running and check the statistics on the Grafana dashboard.
You should see significant improvement in the execution time. 
```sh
sudo ./rt_linux_tutorial -s 1
```