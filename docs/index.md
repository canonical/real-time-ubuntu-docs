---
myst:
  html_meta:
    description: Discover Real-time Ubuntu, an operating system featuring the PREEMPT_RT kernel patch for deterministic performance and latency in mission-critical applications.
---

# Real-time Ubuntu

[Real-time Ubuntu] is Ubuntu with a real-time kernel.

Ubuntu's real-time kernel includes the [PREEMPT_RT] patchset.
It changes Linux's default scheduler to a priority-based one, increasing
predictability by modifying the existing kernel code. Real-time Ubuntu is more
pre-emptive than mainline, delivering determinism and lower latency.

Stringent applications have mission-critical latency requirements and must
ensure high-priority processes are executed first, with deterministic response
times. Real-time Ubuntu provides an upper bound on execution time, to the most
demanding workloads in industrial, telco, automotive, and robotics
applications.

<!-- TODO: Clarify and tighten next par. -->

From assembly lines processing and delivering data in real-time to ensure
system integrity to critical telco infrastructure operating at low latency for
continuous production, the most exacting applications run on Real-time Ubuntu.

---------

## In this documentation

```{list-table}
:widths: 30 70
:header-rows: 0

* - **Tutorial**
  - {doc}`/tutorial/first-rt-app/index`
    • {doc}`/tutorial/intel-tcc/index`

* - **Enabling and installing**
  - {doc}`Enable Real-time Ubuntu </how-to/enable-real-time-ubuntu>`
    • {doc}`/reference/releases`
    • {doc}`Switch from real-time to generic kernel </how-to/switch-from-realtime-to-generic-kernel>`
    • {doc}`Create a Real-time Ubuntu VM using KVM </how-to/create-rt-ubuntu-vm-using-kvm>`
    • {doc}`Install the real-time kernel on Ubuntu Core </how-to/uc-install-real-time-kernel>`
    • {doc}`Create a Real-time Ubuntu Core image </how-to/uc-image-creation>`
    • {doc}`Download Real-time Ubuntu kernel source code </how-to/download-real-time-sources>`

* - **Tuning for real-time performance**
  - {doc}`Modify kernel boot parameters </how-to/modify-kernel-boot-parameters>`
    • {doc}`Configure CPUs for real-time processing </how-to/cpu-boot-configs>`
    • {doc}`Tune IRQ affinity </how-to/tune-irq-affinity>`
    • {doc}`Isolate CPUs from general execution with cpusets </how-to/isolate-workload-cpusets>`
    • {doc}`/reference/kernel-boot-parameters`
    • {doc}`/reference/kernel-config-options`

* - **Measuring and validating**
  - {doc}`Measure maximum latency in a real-time system </how-to/measure-maximum-latency>`
    • {doc}`/reference/real-time-metrics-tools`

* - **Real-time concepts**
  - {doc}`/explanation/schedulers`
    • {doc}`/explanation/locks`
    • {doc}`/explanation/priority-inversion-inheritance`
```

---------

## Project and community

The {{PROJNAME_FULL}} project warmly welcomes community contributions,
suggestions, fixes and constructive feedback.
If you would like to contribute to the {{PROJNAME_FULL}}, see our
[Contributing guide].

Before raising a pull request, it's generally worth [opening an issue] to
discuss any proposed changes (unless it's trivial).
If you decide to raise a pull request, make sure you have signed the
{{C_CLA}} to grant us permission to use your contributions.

* {{U_COC}}

<!--
* Get support
* Join our online chat
* Contribute
* Roadmap
* Thinking about using Example Product for your next project? Get in touch!
-->

```{toctree}
:hidden:
:maxdepth: 1

/tutorial/index
/how-to/index
/reference/index
/explanation/index
/reference/releases
```

<!-- LINKS -->

[Real-time Ubuntu]: https://ubuntu.com/real-time
[PREEMPT_RT]: https://wiki.linuxfoundation.org/realtime/documentation/technical_details/start
[Contributing guide]: https://github.com/canonical/real-time-ubuntu-docs/blob/main/CONTRIBUTING.md
[opening an issue]: https://github.com/canonical/real-time-ubuntu-docs/issues
