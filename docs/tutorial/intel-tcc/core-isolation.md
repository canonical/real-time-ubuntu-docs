# Isolate a core for the Real-time App

```{figure} images/tcc_setup_CAT_isol.drawio.svg
   :width: 100%
   :alt: Isolated Core - System Setup

   Isolated Core - System Setup
```

- Edit `/etc/default/grub` and append `clocksource=tsc tsc=reliable nmi_watchdog=0 nosoftlockup isolcpus=3 rcu_nocbs=3 nohz_full=3 irqaffinity=0` to `GRUB_CMDLINE_LINUX_DEFAULT`.
  It should look similar to this:
  ```sh
  GRUB_CMDLINE_LINUX_DEFAULT="quiet splash clocksource=tsc tsc=reliable nmi_watchdog=0 nosoftlockup isolcpus=3 rcu_nocbs=3 nohz_full=3 irqaffinity=0"
  ```

- Update grub and reboot the system to apply the optimizations.
  ```sh
  sudo update-grub
  sudo reboot now 
  ``` 

- Apply Optimizations done in {doc}`default-configs` and {doc}`cache-partitioning`

- Start real-time application and check the statistics on the Grafana dashboard, you should see less cache misses and latency spikes on the real-time core.
  ```sh
  sudo ./rt_linux_tutorial -s 1
  ```
