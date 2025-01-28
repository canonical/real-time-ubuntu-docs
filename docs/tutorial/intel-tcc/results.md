# Results

```{figure} images/results.png
   :width: 100%
   :alt: Results - System Setup

   Results - System Setup
```

```{warning}
Performance varies by use, configuration and other factors. Learn more at [Intel Performance Index](https://www.intel.com/PerformanceIndex).
See above for workloads and configurations. Results may vary.
```

As depicted in the graphs above, you can still observe high execution time jitter and LLC cache misses with the basic optimizations from Step 1.

Applying LLC partitioning in Step 2 helps to reduce the number of execution time and LLC miss outliers.

Step 3 demonstrates that isolating the real-time core, combined with the optimizations from Steps 1 and 2, significantly improves execution time and reduces cache misses.

Finally, applying Speed Shift for Edge Computing in Step 4 results in a substantial improvement in execution time.
