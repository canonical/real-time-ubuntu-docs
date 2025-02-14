# Optimizing Real-Time Performance on Intel CPUs

```{toctree}
:maxdepth: 1
:hidden:

preparation
default-configs
cache-partitioning
core-isolation
edge-boost
results
```

```{admonition} Collaboration
This tutorial is a collaborative effort between **Intel** and **Canonical**.
All instructions and source code are provided and released under the [3-Clause BSD License](https://spdx.org/licenses/BSD-3-Clause.html).
```

## Key Takeaways of this Tutorial
- **BIOS TCC Mode**
- **Linux Kernel Command-line tuning**
- **Cache Partitioning - Intel CAT**
- **Boost RT Performance - Intel Speed Shift for Edge**

## Required hardware

The following system was used to develop and validate the results obtained in this tutorial.

- CPU: Intel® Core™ i5-1350PE processors
- RAM: 16GB
- DISK: 128GB

Other 13th and 14th generation Intel CPUs that support *Intel Time Coordinated Computing* (TCC) should also work.
See [this list](https://cdrdv2.intel.com/v1/dl/getContent/831868?explicitVersion=true) (slide 41), or check on [Intel ARK](https://www.intel.com/content/www/us/en/ark/featurefilter.html?productType=873&0_TimeCoordinatedComputing=True).

## Test Application

An example C program is provided to demonstrate the possible optimization steps to increase temporal isolation between best effort and real time workloads on Linux systems.
It includes a control task that performs random pointer chasing to simulate a workload and measures wake up jitter, execution time and cache misses.
The application also features a statistics handler thread that processes and logs timing information.

```{figure} images/rt_linux_tutorial-alt.drawio.svg
   :width: 100%
   :alt: High-level design of rt app

   High-level Design of the test application
```

```{note}
The pointer chasing buffer size should exceed the L2 cache size of your processor to see an improvement by cache partitioning described in step 2 below.
You can change the buffer size in `rt_linux_tutorial.c` using `WORKLOAD_BUFFER_SIZE` define.
```
