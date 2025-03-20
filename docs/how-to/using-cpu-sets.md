# Isolate workload using CPUsets

[CPUsets][kdocs_cset] is a kernel feature that allow users to assign specific CPUs and memory nodes to a set of tasks, enabling fine-grained control over resource allocation.
Previously, users could configure CPUsets by directly manipulating the cpuset pseudo-filesystem or using the Python-based utility [cset][cset_manpage].
However, the cset tool is only compatible with legacy systems running cgroup v1.
With the introduction of cgroup v2—adopted by Ubuntu 21.10 (Impish Indri) and latter versions, also by most other distributions around 2021, CPUset management is now handled within the unified cgroup v2 hierarchy.
As a result, configuring CPUsets requires interacting with cgroup v2 instead of relying on cset.



[lfdocs_cset]: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cpu-partitioning/cset
[cset_src]: https://github.com/SUSE/cpuset
[cset_manpage]: https://manpages.ubuntu.com/manpages/noble/man1/cset.1.html
[kdocs_cset]: https://docs.kernel.org/admin-guide/cgroup-v1/cpusets.html 
[manpage_cpuset]: https://man7.org/linux/man-pages/man7/cpuset.7.html


