# Optimizing real-time performance on Intel CPUs

```{toctree}
:maxdepth: 1
:hidden:

preparation
test-application
kernel-parameters
tcc-mode
intel-cat
speed-shift
conclusion
```

```{admonition} Collaboration
This tutorial is a collaborative effort between **Intel** and **Canonical**.
All instructions and source code are provided and released under the [3-Clause BSD License](https://spdx.org/licenses/BSD-3-Clause.html).
```

In this tutorial we will run through a number of configuration options to improve the real-time performance of a system that uses Intel CPUs.
Some of the things that will be discussed include:

- [Linux kernel command-line parameters](kernel-parameters)
- [Intel® TCC Mode in BIOS/SBL](tcc-mode)
- [Cache partitioning using Intel® CAT](intel-cat)
- [Boost RT performance using Intel® Speed Shift for Edge](speed-shift)

An example C program is provided to demonstrate the possible optimization steps to increase temporal isolation between best effort and real-time workloads on Linux systems.
This test application creates a synthetic real-time load, and captures statistics on its performance.

## Required hardware

Any 13th and 14th generation Intel CPUs that support *Intel Time Coordinated Computing* (TCC) should work.
See [the list in the Real-time at the Edge Gold Deck](https://cdrdv2.intel.com/v1/dl/getContent/831868?explicitVersion=true) (slide 41), or check on [Intel ARK](https://www.intel.com/content/www/us/en/ark/featurefilter.html?productType=873&0_TimeCoordinatedComputing=True).

Ideally your system BIOS should have the *Intel® TCC Mode* setting, but this is not a hard requirement for following this tutorial.

The following system was used to develop and validate the results obtained in this tutorial:
- CPU: Intel® Core™ i5-1350PE processors
- RAM: 16GB
- DISK: 128GB
