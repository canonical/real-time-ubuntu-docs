# Kernel command-line parameters

For this tutorial we add a couple of Linux Kernel command line parameters for real-time optimization:

- `clocksource=tsc`: Sets the Time Stamp Counter (TSC) as the clock source for better precision.
- `tsc=reliable`: Marks the TSC as reliable, ensuring it is used consistently.
- `nmi_watchdog=0`: Disables the Non-Maskable Interrupt (NMI) watchdog to reduce latency.
- `nosoftlockup`: Disables the soft lockup detector to prevent unnecessary interruptions.
- `isolcpus=3`: Isolates CPU core 3 from the general scheduler, dedicating it to real-time tasks.
- `rcu_nocbs=3`: Prevents the specified CPU core (3) from handling RCU (Read-Copy-Update) callbacks, reducing latency.
- `nohz_full=3`: Enables full dynamic ticks on CPU core 3, reducing timer interrupts.
- `irqaffinity=0`: Directs all hardware interrupts to CPU core 0, keeping core 3 free for real-time tasks.


These parameters need to be applied by GRUB during boot.
To do this:

1. Edit `/etc/default/grub` and append the following to `GRUB_CMDLINE_LINUX_DEFAULT`.
  
   ```text
   clocksource=tsc tsc=reliable nmi_watchdog=0 nosoftlockup isolcpus=3 rcu_nocbs=3 nohz_full=3 irqaffinity=0
   ```
  
   The result should look similar to this:
  
   ```bash
   GRUB_CMDLINE_LINUX_DEFAULT="quiet splash clocksource=tsc tsc=reliable nmi_watchdog=0 nosoftlockup isolcpus=3 rcu_nocbs=3 nohz_full=3 irqaffinity=0"
   ```

1. Update grub and reboot the system to apply the optimizations.
  
   ```bash
   sudo update-grub
   sudo reboot
   ``` 

## Related topics

- {doc}`/how-to/modify-kernel-boot-parameters`