# How to isolate CPUs from general execution with cpusets

[Cpusets][kdocs_cpusets_v2] is a kernel feature that allows users to assign specific CPUs and memory nodes to a set of tasks, enabling fine-grained control over resource allocation.
It also allows you to exclude certain CPUs from the system scheduler.
This is called *cpu isolation*, as it **isolates** the CPU from being assigned work by the scheduler.
In other words excluding CPUs from use by general processes.

Cpusets are particularly useful for Real-time Ubuntu, as they enable the isolation of CPUs for time-sensitive workloads.
By creating a cpuset partition without load balancing, cpusets prevent latency jitters caused by periodic load balancing to exceed the desired latency limit of a certain workload.
This ensures that real-time tasks maintain predictable execution times, which is essential for workloads requiring low latency and precise, time-critical responses.

```{warning}
Managing cpusets on Ubuntu 21.10 (Impish Indri) and later is not possible with the [cset][cset_manpage] utility. This is because `cset` utilizes cgroups v1 which is no longer supported on Ubuntu.
```

There are many ways to setup cpusets, either by manually modifying the cgroup-v2 filesystem, or using tools like [cgcreate][manpage_cgcreate], [cgexec][manpage_cgexec] and [cgclassify][manpage_cgclassify].

On Ubuntu, cgroups are managed at a higher level by [systemd][systemd.io]. It is recommended to use this tool to configure cgroups, rather than doing it manually at a lower level.

## Configure runtime CPU isolation

Isolation settings primarily depend on the number of available CPU cores.
Before doing the isolation, check the number of online cpus for further comparison:
```{terminal}
   :input: nproc
   :user: ubuntu
   :host: ubuntu
   :dir: ~

12
```

First, define a [slice][systemd_slice] and the CPU(s) that should be isolated from general execution -- the application to be isolated will belong to this `systemd` slice.
Internally, `systemd` will translate this slice to a cgroup node on the unified hierarchy of cgroup-v2.

For example, to define a `systemd` slice named `custom-workload.slice` that is restricted to run only on CPU 11, run:
```bash
sudo systemctl set-property --runtime custom-workload.slice AllowedCPUs=11
```

Then restrict the system units and the init scope (created by default on `systemd`), to use the remaining CPUs:
```bash
sudo systemctl set-property --runtime init.scope AllowedCPUs=0-10
sudo systemctl set-property --runtime system.slice AllowedCPUs=0-10
sudo systemctl set-property --runtime user.slice AllowedCPUs=0-10
```

- `init.scope`: unit where the system and the service manager itself resides.
- `system.slice`: slice where all system services started by systemd are found.
- `user.slice`: the place of all user processes and services started on behalf of the user.

Check that the properties are correctly set by fetching their values with `systemctl show`, providing the unit and the desired property (with parameter `-p`).

```{terminal}
   :user: ubuntu
   :host: ubuntu
   :dir: ~

:input: systemctl show custom-workload.slice -p AllowedCPUs
AllowedCPUs=11
:input: systemctl show init.scope -p AllowedCPUs
AllowedCPUs=0-10
```

After setting up the CPU isolation, check with `nproc` that the total online CPUs have changed based on your settings (e.g. from 12 to 11):

```{terminal}
   :input: nproc
   :user: ubuntu
   :host: ubuntu
   :dir: ~

11
```

Finally, run the application inside a new `systemd` scope and assign it to the property defined earlier.
This is done by setting the scope `Slice` property to the slice defined earlier (e.g. `custom-workload.slice`), ensuring that the application runs within the resource limits and constraints defined by that slice:

```bash
systemd-run --scope -p Slice=custom-workload.slice <my-app arg1 ...>
```

```{tip}
It is useful to run the `systemd` scope from a root shell session (using `sudo su`).
This makes sure that the application will run with proper root privileges.
```

```{terminal}
   :input: systemd-run --scope -p Slice=custom-workload.slice /home/ubuntu/my-app
   :user: root
   :host: ubuntu
   :dir: /home/ubuntu

Running as unit: run-rf31d22d4d34d4fdfbe0e87edf82e7621.scope; invocation ID: 91facec7c7a24c089a29d7a0080b4f1b
```

Confirm that your application is running on CPU 11 by checking with [ps][ps_manpage] command:

```{terminal}
   :input: ps -eLo psr,comm,args,pid, | grep my-app
   :user: ubuntu
   :host: ubuntu
   :dir: ~

11  my-app    /bin/bash /home/ubuntu/my-a    1590
```

It's also possible to confirm using `ps` that your application is running isolated on CPU 11:

```{terminal}
   :input: ps -eLo psr,comm,args,ppid,pid, | grep '^ 11'
   :user: ubuntu
   :host: ubuntu
   :dir: ~

11 cpuhp/11        [cpuhp/11]                        2      81
11 idle_inject/11  [idle_inject/11]                  2      82
11 migration/11    [migration/11]                    2      83
11 ksoftirqd/11    [ksoftirqd/11]                    2      84
11 kworker/11:0H-e [kworker/11:0H-events_highp       2      86
11 ksmd            [ksmd]                            2      96
11 kworker/R-devfr [kworker/R-devfr]                 2     108
11 kworker/R-scsi_ [kworker/R-scsi_]                 2     120
11 kworker/u25:0   [kworker/u25:0]                   2     140
11 kworker/11:1-mm [kworker/11:1-mm_percpu_wq]       2     188
11 kworker/11:1H-k [kworker/11:1H-kblockd]           2     222
11 kworker/11:3-cg [kworker/11:3-cgroup_destro       2     441
11 my-app          /bin/bash /home/ubuntu/my-a    1576    1590
11 sleep           sleep 5                        1590    1761
```

The application `my-app` with PID `1590` is listed.
The is also an entry for `sleep` which has a parent process ID (PPID) equal to our application -- this is because the application `my-app` consists of a bash script with a sleep command.
The other processes listed here are kernel threads.

```{note}
At the time of writing (Kernel v6.8.0), [cpusets' implementation][kcommit_cpuset] does not isolate CPUs from running kernel threads.
```

## Configure persistent CPU isolation

```{warning}
The machine used in this example has 12 cores, but you should adjust the CPU list based on your specific use case.

**Disclaimer:** By following the next steps, the user is expected to have already tested and debug the settings at runtime and determined a configuration that best fits their needs.
```

The configuration set via the `systemctl set-property` command does not persist across reboots.
To make them persistent, add them as configuration files under the {file}`/etc/systemd/system` directory.

Configurations need to be in subdirectories, but these do not exist by default.
Create these subdirectories by running:

```bash
sudo mkdir -p /etc/systemd/system/init.scope.d/
sudo mkdir -p /etc/systemd/system/system.slice.d/
sudo mkdir -p /etc/systemd/system/user.slice.d/
```
Then create the files for the `init.scope`, `system.slice` and `user.slice`:

```{note}
We will prefix the name of the configuration files with `50-` because systemd processes the files in lexicographical order (sorted alphabetically).
Prefixing files with numbers ensures a predictable loading sequence.
This way, a file like `50-cpu-isolation.conf` will override settings from `10-defaults.conf` but can still be overridden by a file named `60-custom.conf`.
```

Now that the directories are available, create the following three configuration files:
- `/etc/systemd/system/init.scope.d/50-cpu-isolation.conf` for the init scope with the following content:
```
[Scope]
AllowedCPUs=0-10
```

- `/etc/systemd/system/system.slice.d/50-cpu-isolation.conf` and `/etc/systemd/system/user.slice.d/50-cpu-isolation.conf` with the following:
```
[Slice]
AllowedCPUs=0-10
```

Finally, create a new config file for the slice that will be used by our workload, with the remaining CPU. Add the following to `/etc/systemd/system/custom-workload.slice`:

```
[Slice]
AllowedCPUs=11
```

After reloading the systemd daemon, the changes will take effect:
```bash
sudo systemctl daemon-reload
```

Afterwards, it is possible to check the values using `systemctl show` in the same way as before.

### Creating a Service

A reliable and reproducible way to apply CPU isolation is by creating a systemd service unit.
This allows you to assign the cpuset to the service, ensuring the application runs inside the isolated `custom-workload.slice` automatically on every reboot, without any manual intervention.

Here is a simple example of how this service could look like:
```
[Unit]
Description=app demo service

[Service]
Slice=custom-workload.slice
ExecStart=/home/ubuntu/my-app

```
The important part here is the `Slice=custom-workload.slice` which points our service to the `custom-workload.slice` that we created which has access to the isolated cpus.

The service file should be created inside the `/etc/systemd/system` directory, for example at `/etc/systemd/system/my-app.service`

Now, reload the daemon, start and enable the service:

```bash
sudo systemctl daemon-reload
sudo systemctl start my-app.service
sudo systemctl enable my-app.service
```

Then it's possible to check that our application is running on CPU 11:

```{terminal}
   :input: ps -eLo psr,comm,args,pid, | grep my-app
   :user: ubuntu
   :host: ubuntu
   :dir: ~

 11 my-app     /bin/bash /home/ubuntu/my-a       1    2417
```

In the service report, you can verify that the service is running inside the designated cgroup created by the `custom-workload` slice.

```{terminal}
   :input: systemctl status my-app.service
   :user: ubuntu
   :host: ubuntu
   :dir: ~

● my-app.service - app demo service
   ...
     CGroup: /custom.slice/custom-workload.slice/my-app.service
             ├─ 855 /bin/bash /home/ubuntu/my-app
             └─1323 sleep 5
   ...
```

% Links

[ps_manpage]: https://manpages.ubuntu.com/manpages/noble/man1/ps.1.html

[systemd.io]: https://systemd.io/

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

[systemd.special]: https://manpages.ubuntu.com/manpages/noble/man7/systemd.special.7.html

[systemd_slice]: https://www.freedesktop.org/software/systemd/man/latest/systemd.slice.html