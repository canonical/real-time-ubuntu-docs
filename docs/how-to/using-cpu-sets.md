# Shielding CPUs from general execution with CPUsets

[CPUsets][kdocs_cpusets_v2] is a kernel feature that allow users to assign specific CPUs and memory nodes to a set of tasks, enabling fine-grained control over resource allocation.

```{warning}
Previously, users could configure CPUsets by directly manipulating the cpuset pseudo-filesystem or using the Python-based utility [cset][cset_manpage].

However, the cset tool is only compatible with legacy systems running cgroup v1.
With the introduction of cgroup v2—adopted by Ubuntu 21.10 (Impish Indri) and latter versions, also by most other distributions around 2021, CPUset management is now handled within the unified cgroup v2 hierarchy.
As a result, configuring CPUsets requires interacting with cgroup v2 instead of relying on cset.
```

There are many ways to setup CPUsets, either by manually playing with the cgroup-v2 filesystem, or using tools like [cgcreate][manpage_cgcreate], [cgexec][manpage_cgexec] and [cgclassify][manpage_cgclassify].



% Links
[lfdocs_cset]: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cpu-partitioning/cset

[cset_src]: https://github.com/SUSE/cpuset

[cset_manpage]: https://manpages.ubuntu.com/manpages/noble/man1/cset.1.html

[kdocs_cpusets_v1]: https://docs.kernel.org/admin-guide/cgroup-v1/cpusets.html

[kdocs_cpusets_v2]: https://docs.kernel.org/admin-guide/cgroup-v2.html#cpuset

[manpage_cpuset]: https://man7.org/linux/man-pages/man7/cpuset.7.html

[manpage_cgcreate]: https://manpages.ubuntu.com/manpages/noble/man1/cgcreate.1.html

[manpage_cgexec]: https://manpages.ubuntu.com/manpages/noble/man1/cgexec.1.html

[manpage_cgclassify]: https://manpages.ubuntu.com/manpages/noble/man1/cgclassify.1.html


[archwiki_cgroups]: https://wiki.archlinux.org/title/Cgroups

% This commit introduced the 'isolated' option for cpusets v2
[kcommit_cpuset]: https://github.com/torvalds/linux/commit/f28e22441f353aa2c954a1b1e29144f8841f1e8a

