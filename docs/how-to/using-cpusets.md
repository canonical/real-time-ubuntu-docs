# Shielding CPUs from general execution with Cpusets

[Cpusets][kdocs_cpusets_v2] is a kernel feature that allow users to assign specific CPUs and memory nodes to a set of tasks, enabling fine-grained control over resource allocation.

It also allows to exclude certain CPUs from the system scheduler.
In other words excluding CPUs from use by general processes.
This is called *shielding*, as it shields the CPU from being assigned work by the scheduler.

```{warning}
Managing cpusets on Ubuntu 21.10 (Impish Indri) and later is no longer possible with the [cset][cset_manpage] utility. This is because `cset` utilizes cgroups v1 which is no longer supported on Ubuntu.
```

There are many ways to setup cpusets, either by manually modifying the cgroup-v2 filesystem, or using tools like [cgcreate][manpage_cgcreate], [cgexec][manpage_cgexec] and [cgclassify][manpage_cgclassify].

On Ubuntu, cgroups are managed at a higher level by [systemd][systemd.io]. It is recommended to use this tool to configure cgroups, rather than doing it manually at a lower level.

## Runtime shielding

Shielding settings primarily depend on the number of available CPU cores.  
Before doing the shielding, let's check the number of online cpus for further comparison:
```console
$ nproc
12
```
The machine used in this example has 12 cores, but you should adjust the CPU list based on your specific use case.

It’s often useful to first test shield isolation at runtime to verify and debug its functionality before applying it persistently.

First, it's necessary to define a slice, let's arbitrarily call it `custom-workload.slice`.
Internally, `systemd` will translate this slice to a cgroup node on the unified hierarchy of cgroup-v2.
The application to be shielded will belong to this `systemd` slice.
On this property we define the CPUs that should be isolated from general execution.
```bash
sudo systemctl set-property --runtime custom-workload.slice AllowedCPUs=11
```

Then we restrict the system units and the init scope (created by default on systemd), to use the remaining CPUs:
```bash
sudo systemctl set-property --runtime init.scope AllowedCPUs=0-10
sudo systemctl set-property --runtime system.slice AllowedCPUs=0-10
sudo systemctl set-property --runtime user.slice AllowedCPUs=0-10
```

- `init.scope`: unit where the system and the service manager itself resides.
Being the service manager the PID 1. It's active as long the system is running.
- `system.slice`: slice where all system services started by systemd are found.
- `user.slice`: the place of all user processes and services started on behalf of the user.

It's possible to check that the properties are correctly set by fetching their values with `systemctl show`, providing the unit and the desired property (with parameter `-p`). For example:

```console
$ systemctl show custom-workload.slice -p AllowedCPUs
AllowedCPUs=11
$ systemctl show init.scope -p AllowedCPUs
AllowedCPUs=0-10
```

Finally, we run our application inside a new systemd scope and assign it to the property defined earlier.
This is done by setting the scope `Slice` property to `custom-workload.slice`, ensuring that the application runs within the resource limits and constraints defined by that slice:

```bash
systemd-run --scope -p Slice=custom-workload.slice <my-app arg1 ...>
```

```{tip}
It is useful to run the systemd scope from a root shell session (using `sudo su`).
This makes sure that the application will run with proper root privileges.

```console
$ sudo su
#
# systemd-run --scope -p Slice=custom-workload.slice /home/ubuntu/my-app
Running as unit: run-rf31d22d4d34d4fdfbe0e87edf82e7621.scope; invocation ID: 91facec7c7a24c089a29d7a0080b4f1b
```

After setting up the CPU shielding, it is possible to check with `nproc` that the online CPUs have changed:

```console
$ nproc
11
```

We can confirm that our application is running on the designated cpus by checking with [ps][ps_manpage] command:

```console
$ ps -eLo psr,comm,args,pid, | grep my-app
 11  my-app    /bin/bash /home/ubuntu/my-a    1590
```

It's also possible to confirm using `ps` that our application is running isolated on the `cpu 11`:

```console
$ ps -eLo psr,comm,args,ppid,pid, | grep '^ 11'
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

Our application `my-app` with PID `1590` is listed.
We also see a `sleep` which has a parent process ID (PPID) equal to our application.
This is because the application `my-app` consists of a bash script with a sleep command.
The other processes listed here are kernel threads.
`cpusets` doesn't currently isolate CPUs from running kthreads.

## Persistent shielding

The configuration set via the `systemctl set-property` command do not persist across reboots.
To make them persistent, we need to add them as configuration files under the `/etc/systemd/system` directory.

Configurations need to be in subdirectories, but these do not exist by default.
They can be created with these commands:

```bash
sudo mkdir -p /etc/systemd/system/init.scope.d/
sudo mkdir -p /etc/systemd/system/system.slice.d/
sudo mkdir -p /etc/systemd/system/user.slice.d/
```
Then we create the files for the `init.scope`, `system.slice` and `user.slice`:

```{note}
Here we're prefixing the name of our slice with `50-` because systemd processes configuration files in lexicographical order (sorted alphabetically).
So, prefixing files with numbers ensures a predictable loading sequence.
This way, a file like `50-cpu-shielding.conf` will override settings from `10-defaults.conf` but can still be overridden by a file named `60-custom.conf`.
```

Create the configuration file for the init scope:

- `/etc/systemd/system/init.scope.d/50-cpu-shielding.conf`


Then add the `AllowedCPUs` property to the configuration of this scope:
```
[Scope]
AllowedCPUs=0-10
```

Similarly, create these two files for the remaining default slices:

- `/etc/systemd/system/system.slice.d/50-cpu-shielding.conf`
- `/etc/systemd/system/user.slice.d/50-cpu-shielding.conf`

In both of them add the `AllowedCPUs` property:

```
[Slice]
AllowedCPUs=0-10
```

Then create a new config file for the slice that will be used by our workload:

- `/etc/systemd/system/custom-workload.slice`

Finally, add the remaining cpu to the configuration of this slice:
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

A reliable and reproducible way to apply CPU shielding is by creating a systemd service unit.
This allows you to assign the cpuset to the service, ensuring the application runs inside the isolated `custom-workload.slice` automatically on every reboot, without any manual intervention.

Here is a simple example of how this service could look like:
```
[Unit]
Description=app demo service
After=network.target
StartLimitIntervalSec=0

[Service]
Slice=custom-workload.slice
Type=simple
Restart=always
RestartSec=1
User=root
ExecStart=/home/ubuntu/my-app

[Install]
WantedBy=multi-user.target
```
The important part here is the `Slice=custom-workload.slice` which points our service to the `custom-workload.slice` that we created which has access to the isolated cpus.

The service file should be created inside `/etc/systemd/system`.

Now we: reload the daemon, start and enable the service:

```bash
sudo systemctl daemon-reload
sudo systemctl start my-app.service
sudo systemctl enable my-app.service
```

Then it's possible to check that our application is running on the designated CPU:

```bash
$ ps -eLo psr,comm,args,ppid,pid, | grep my-app
 11 my-app     /bin/bash /home/ubuntu/my-a       1    2417
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