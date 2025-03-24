# Shielding CPUs from general execution with CPUsets

[CPUsets][kdocs_cpusets_v2] is a kernel feature that allow users to assign specific CPUs and memory nodes to a set of tasks, enabling fine-grained control over resource allocation.

```{warning}
Previously, users could configure CPUsets by directly manipulating the cpuset pseudo-filesystem or using the Python-based utility [cset][cset_manpage].

However, the cset tool is only compatible with legacy systems running cgroup v1.
With the introduction of cgroup v2—adopted by Ubuntu 21.10 (Impish Indri) and latter versions, also by most other distributions around 2021, CPUset management is now handled within the unified cgroup v2 hierarchy.
As a result, configuring CPUsets requires interacting with cgroup v2 instead of relying on cset.
```

There are many ways to setup CPUsets, either by manually playing with the cgroup-v2 filesystem, or using tools like [cgcreate][manpage_cgcreate], [cgexec][manpage_cgexec] and [cgclassify][manpage_cgclassify].

The most recommended way of doing this, is by using a higher level tool, on Ubuntu, this tool is the [systemd][systemd.io] init system.

## Runtime shielding

It’s often useful to first test shield isolation at runtime to verify and debug its functionality before applying it persistently.

First, it's necessary to define a `custom-workload.slice` which our application to be shielded will belong.
On this property we're going to define the CPUs that are going to be isolated from general execution.

```bash
sudo systemctl set-property --runtime custom-workload.scope AllowedCPUs=11
```

Then we isolate further away those cpus from the system units and the init scope (created by default on systemd):
```bash
sudo systemctl set-property --runtime init.scope AllowedCPUs=0-10
sudo systemctl set-property --runtime system.slice AllowedCPUs=0-10
sudo systemctl set-property --runtime user.slice AllowedCPUs=0-10
```

- `init.scope`: unit where the system and the service manager itself resides.
Being the service manager the PID 1. It's active as long the system is running.
- `system.slice`: slice where all system services started by systemd are found.
- `user.slice`: the place of all user processes and services started on behalf of the user.

It's possible to check that the properties are correctly set by fetching their values with `systemctl show`, providing the unit and the desired property (with parameter `-p`):

```console
ubuntu@dt:~$ systemctl show custom-workload.slice -p AllowedCPUs
AllowedCPUs=11
ubuntu@dt:~$ systemctl show init.scope -p AllowedCPUs
AllowedCPUs=0-10
ubuntu@dt:~$ systemctl show system.slice -p AllowedCPUs
AllowedCPUs=0-10
ubuntu@dt:~$ systemctl show user.slice -p AllowedCPUs
AllowedCPUs=0-10
ubuntu@dt:~$
```

Finally, we run our application inside our systemd scope:

```bash
systemd-run --scope -p Slice=custom-workload.slice application arg1 ...
```

```{tip}
Is useful to run the systemd scope from a root shell session (with `sudo su`).
To make sure that the application will run with proper root privileges.
```

```console
ubuntu@dt:~$ sudo su
root@dt:/home/ubuntu#
root@dt:/home/ubuntu# systemd-run --scope -p Slice=custom-workload.slice /home/ubuntu/application
Running as unit: run-rf31d22d4d34d4fdfbe0e87edf82e7621.scope; invocation ID: 91facec7c7a24c089a29d7a0080b4f1b
```

After setting up the cpu shielding is possible to check with `nproc` that the online cpus changed:

```console
ubuntu@dt:~$ nproc
11
ubuntu@dt:~$ nproc --all
12
```

We can confirm that our application is running on the designated cpus by checking with [ps][ps_manpage] command:

```console
ubuntu@dt:~$ ps -eLo psr,comm,args,pid, | grep application
 11 application     /bin/bash /home/ubuntu/appl    1590
```

It's also possible to confirm using `ps` that our application is running isolated on the `cpu 11`:

```console
ubuntu@dt:~$ ps -eLo psr,comm,args,ppid,pid, | grep '^ 11'
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
 11 application     /bin/bash /home/ubuntu/appl    1576    1590
 11 sleep           sleep 5                        1590    1761
ubuntu@dt:~$
```

Our `application` with PID `1590` is there, and also an `sleep` which was a parent process id (PPID) equal to our `application`.
This is because my application consists in a bash script with a sleep command.
The other processes showing up there are kernel threads, unfortunately, cpusets doesn't isolate cpus from running kthreads yet.

## Persistent shielding

When it comes to apply settings in a persistence way on systemd, we're going to write our configurations to the `/etc/systemd/system` directory.

For persistent shielding, it's necessary to create the directories which will hold the system configurations:

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

```bash
sudo tee /etc/systemd/system/init.scope.d/50-cpu-shielding.conf > /dev/null << 'EOF'
[Scope]
AllowedCPUs=0-10
EOF
```

```bash
sudo tee /etc/systemd/system/system.slice.d/50-cpu-shielding.conf > /dev/null << 'EOF'
[Slice]
AllowedCPUs=0-10
EOF
```

```bash
sudo tee /etc/systemd/system/user.slice.d/50-cpu-shielding.conf > /dev/null << 'EOF'
[Slice]
AllowedCPUs=0-10
EOF
```

Then, we create a slice for the our workload:

```bash
sudo tee /etc/systemd/system/custom-workload.slice > /dev/null << 'EOF'
[Slice]
AllowedCPUs=11
EOF
```

After reloading the systemd daemon, the changes will take effect:
```bash
sudo systemctl daemon-reload
```

Then after this setup, is possible to check the values using `systemctl show` in the same way as done before.

### Creating the service

One good way of doing this in a persistent and reproducible way is by creating a systemd service unit, so we can attribute the cpuset to it.

Here is a simple sample of how this service could look like:
```toml
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
ExecStart=/home/ubuntu/application

[Install]
WantedBy=multi-user.target
```
The important part here is the `Slice=custom-workload.slice` which points our service to the `custom-workload.slice` that we created which has access to the isolated cpus.

So I'm going to create a file on `/etc/systemd/system/custom.service.d/customapp.service`

Creating the directory which will hold the configuration files for application service.

Now we: reload the daemon, start and enable the service:

```bash
sudo systemctl daemon-reload
sudo systemctl start customapp.service
sudo systemctl enable customapp.service
```

Then it's possible to check that our application is running on the designated cpu with it's PPID being `1`:

```bash
ubuntu@dt:~$ ps -eLo psr,comm,args,ppid,pid, | grep application
 11 application     /bin/bash /home/ubuntu/appl       1    2417
```

<!-- ```bash -->
<!-- # for dir in /proc/[0-9]*; do awk '{print $39}' $dir/stat; done  | grep 11 -->
<!-- ``` -->

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

