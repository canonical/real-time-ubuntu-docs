How to Tune IRQ Affinity
=========================

IRQ (Interrupt Request) affinity refers to the assignment of interrupt service
routines (ISRs) to specific processor cores in a computer system. In a real-time
system, having IRQ affinity properly set up becomes crucial for improved performance
and responsiveness. 
By tuning IRQ affinity, you dedicate specific interrupts to particular 
CPU cores, preventing interruptions from being handled by multiple cores
simultaneously. This enhances efficiency, reduces latency, and ensures more
predictable and consistent processing times for time-sensitive tasks in 
real-time systems. Proper IRQ affinity configuration helps in decreasing
contention, managing resource usage, and ultimately improving the overall
responsiveness and reliability of the system, especially in scenarios where
time precision is critical.

The most common parameters to handle IRQ interrupts are `kthread_cpus`_ which
defines the CPUs that kernel threads are allowed to run on, `isolcpus`_ and
`irq_affinity`_.

First you can list the isolated CPUs:

.. code-block:: bash

    cat /sys/devices/system/cpu/isolated

An empty output means that no core is isolated.

Then you can list all the available CPUs:

.. code-block:: console

    $ cat /sys/devices/system/cpu/present

    0-19

All the IRQs are listed in the `/proc/interrupts` file. 

.. code-block:: console

    $ cat /proc/interrupts


You can tune the IRQ affinity by setting which CPUs each real-time application is
allowed to run on, in other words, in which core you don't want IRQs. 
You can do this, by setting and running manually:

.. code-block:: bash

    taskset -c <CPU-NUM[s]> <COMMAND-TO-REAL-TIME-APP>

Or also attach to a already running process:

.. code-block:: bash

    taskset -pc <CPU_NUM[s]> <PID>

Latter, you can check if th application is correctly running on the designated
CPU core(s):

.. code-block:: bash

    ps -eo psr,tid,pid,comm,%cpu,priority,nice -T | grep <PID>

To properly set the IRQ affinity, you need to set in which core you don't want
IRQs. To do so, in the  `/etc/systemd/system.conf` file you can set the
parameter `CPUAffinity` to the cores you want to isolate. For example:

.. code-block:: console

    $ cat /etc/systemd/system.conf | grep CPUAffinity

    CPUAffinity=0,1

It's also important to disable the `irqbalance` service, which is responsible for
distributing IRQs across all available cores. To do so, you can run:

.. code-block:: bash

    systemctl disable irqbalance 
    systemctl stop irqbalance
    systemctl status irqbalance


.. LINKS
.. _kthread_cpus: https://www.kernel.org/doc/Documentation/sysctl/kernel.txt
.. _isolcpus: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cpu-partitioning/isolcpus

