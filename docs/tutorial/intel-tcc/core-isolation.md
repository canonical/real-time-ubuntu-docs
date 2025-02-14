# Isolate a core for the Real-time App

```{figure} images/tcc_setup_CAT_isol.drawio.svg
   :width: 100%
   :alt: Isolated Core - System Setup

   Isolated Core - System Setup
```

- Edit `/etc/default/grub` and append the following to `GRUB_CMDLINE_LINUX_DEFAULT`.
  
  ```text
  clocksource=tsc tsc=reliable nmi_watchdog=0 nosoftlockup isolcpus=3 rcu_nocbs=3 nohz_full=3 irqaffinity=0
  ```
  
  The result should look similar to this:
  
  ```bash
  GRUB_CMDLINE_LINUX_DEFAULT="quiet splash clocksource=tsc tsc=reliable nmi_watchdog=0 nosoftlockup isolcpus=3 rcu_nocbs=3 nohz_full=3 irqaffinity=0"
  ```

- Update grub and reboot the system to apply the optimizations.
  
  ```bash
  sudo update-grub
  sudo reboot now 
  ``` 

- Apply Optimizations done in {doc}`default-configs` and {doc}`cache-partitioning`

- Start real-time application and check the statistics on the Grafana dashboard, you should see less cache misses and latency spikes on the real-time core.
  
  ```bash
  sudo ./rt_linux_tutorial -s 1
  ```
