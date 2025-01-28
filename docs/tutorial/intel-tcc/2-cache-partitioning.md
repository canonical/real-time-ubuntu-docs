# Partition the LLC(L3) Cache

```{figure} images/setup_CAT.png
   :width: 100%
   :alt: Cache Partitioning - System Setup

   Cache Partitioning - System Setup
```

Intel provides the ability to partition caches at various levels in the caching hierarchy through Cache Allocation Technology (*CAT*), part of Intel® Resource Director Technology (Intel® RDT).

We will partition the Last Level Cache (*LLC*) to exclusively provide a portion of the L3 cache to the core the real-time app is running on.

As depicted above the LLC of i5-1350PE has a Capacity BitMask (*CBM*) length of 8 bits (`0xFF`).
For this tutorial LLC is partitioned in half.
P cores 0-2, all E cores and GFX operates with Class of Service 0 (*CLOS 0*), being assigned half the cache.
P cores 3 which will be used for real-time workloads, operating on Class of Service 1 (*CLOS 1*) is assigned the other half.

Each class of service will be assigned a CBM length of 4 (`0xF0` and `0xF0`) when partitioning the cache in half.
The MSRs used for configuring the cache masks are `L3_MASK_0` (address `0xC90`), `L3_MASK_1` (address `0xC91`) and `GT_MASK0` (address `0x18B0`).
The class the MSR used for associating a class of service to a core is `PQR_ASSOC` (address `0xC8F`). 

This is just an example, and the configuration needs to be adjusted to your specific use case and processor.
You can determine the cache topology, including the size and number of ways supported for a particular processor, by using the CPUID leaf "Deterministic Cache Parameters Leaf - 0x4".
Additionally, Linux utilities like `lstopo` are very useful for getting an overview of the cache topology of a processor.
Here are some references if you need more information:

- Public Intel® Time Coordinated Computing (TCC) User Guide - RDC #[831067](https://cdrdv2.intel.com/v1/dl/getContent/831067?fileName=Public+TCC+User+Guide+-+September+2024+-+RDC-831067.pdf)
- Intel® Resource Director Technology (Intel® RDT) Architecture Specification - RDC #[789566](https://cdrdv2.intel.com/v1/dl/getContent/789566?fileName=356688-intel-rdt-arch-spec.pdf)
- Intel® 64 and IA-32 Architectures Software Developer’s Manual - RDC#[671200](https://cdrdv2.intel.com/v1/dl/getContent/671200)
  
Here is an example how the LLC can be partitioned with the Linux `msr-tools`:
```sh
#define LLC Core Masks
wrmsr 0xc90 0xF0 # best effort
wrmsr 0xc91 0x0F # real-time
#define LLC GT Mask
wrmsr 0x18b0 0x80

#assign the masks to the cores.
#This has to match with the core selected for the rt app
wrmsr -a 0xc8f 0x0 # assign all cores to the CLOS0 
wrmsr -p 3 0xc8f 0x100000000 # assign real-time core to CLOS1
```

````{note}
A convenience script is provided to automate the cache partitioning.
Simply use the script with the `rt_optimized` argument to partition the cache like depicted above.
To reset the cache back to default flat partitioning, run the script again with the `default` argument.
```sh
sudo ./setCacheAllocation.sh rt_optimized
```
````

Start the real-time application if it is not already running and check the statistics on the Grafana dashboard. You should observe an improvement in cache misses and latency. However, there may still be some spikes in latency and cache misses, which we will address in the next optimization steps.
```sh
sudo ./rt_time_linux_tutorial -s 1
  ```