Kernel configuration options
=============================================

Real-time Ubuntu kernel configuration options are crucial for achieving low-latency and high-responsiveness in systems where deterministic performance is essential.
This document outlines various kernel config options, that Real Time Ubuntu uses on it's real-time kernel.
These options enable and optimize preemption, manage IRQs, and control block I/O latency, among other features, that make Ubuntu real-time capable.

All the configurations below are kernel config options, that begin with **CONFIG_**, but this prefix is omitted in the following sections for brevity.


>>>>

Basic preemption configurations
-------------------------------
Preemption refers to the ability of an operating system to interrupt the execution of a running task or process. 
This interruption is done in order to give priority to another task.
This feature is essential to reduce latency and improve the responsiveness of the system. 
Real-time applications are the ones who most benefit from it, where deterministic response times are critical.

ARCH_SUPPORTS_RT
~~~~~~~~~~~~~~~~
This is a boolean value that indicates if the architecture supports real-time features.

PREEMPTION
~~~~~~~~~~
The primary config, that enables the preemptible kernel.
This configuration needs to be enabled in order to enable the other preemption levels and configurations.

PREEMPT_COUNT
~~~~~~~~~~~~~
Once the previous option get enabled, this configuration enables the counting of preemption.
Needed for kernel APIs like `preempt_count()`_.

PREEMPT_RT
~~~~~~~~~~
This configuration enables a fully preemptible kernel.
There are `other levels of preemption`_ available in the kernel, but the PREEMPT_RT configuration is the most aggressive.
The way that it works is by replacing some kernel primitives related to `locking`_ with preemptible priority-inheritance aware variants.
This enables mechanisms that can break up long non-preemptible sections of code.
However, this configuration doesn't apply for very low level and critical sections of the kernel, such as entry code, scheduler, and low level interrupt handlers.


>>>>

Kernel internals configurations
-------------------------------
Next configurations are related to kernel internals.

PREEMPT_RCU
~~~~~~~~~~~
Enables preemption in the context of the synchronization mechanism `RCU - Read-copy update`_.
This is done by selecting the RCU implementation designed for very large `Symmetric multiprocessing (SMP)`_ systems that usually had hundreds of CPUs and that requires real-time response times.
However this also scales down nicely to smaller systems.

RCU_BOOST
~~~~~~~~~
Working together with previous configuration, this one boosts the priority of preempted RCU readers that block the current preemptible RCU grace period for too long.
This also prevents heavy loads from blocking RCU callback invocation. 

RCU_NOCB_CPU
~~~~~~~~~~~~
Used to reduce OS jitter for aggressive HPC or real-time workloads. 
It offloads RCU callback invocation to energy-efficient CPUs in asymmetric multiprocessors.
The reduced jitter comes at the cost of increased `call_rcu()`_ overhead and potentially higher context-switch rates.
This option creates kthreads ("``rcuox/N``") to invoke callbacks on specified CPUs, where "``N``" is the CPU being offloaded and "``x``" will be "``p``" for RCU-preempt (**PREEMPTION kernels**) and "``s``" for RCU-sched (**non PREEMPTION kernels**).
Affinity or cgroups can be used to control the CPU set for the kthreads.

RCU_NOCB_CPU_CB_BOOST
~~~~~~~~~~~~~~~~~~~~~
Used to offload RCU callbacks from real-kernel threads to energy-efficient CPUs.
It invokes offloaded callbacks as `SCHED_FIFO`_ possible starvation caused by heavy background load on SCHED_OTHER.
It's necessary to ensure that latency-sensitive tasks will either run with higher priority or in some other isolated CPU.

TASKS_RCU
~~~~~~~~~
Enables task-based RCU implementation that uses only voluntary context switches (non preemptible).

CONTEXT_TRACKING
~~~~~~~~~~~~~~~~
This configuration enables context tracking, by activating Kernel APIs on the `context tracking header`_


>>>>

CPU isolation configurations
----------------------------
The next configurations are related to CPU isolation and how to track CPU time spent in the kernel.


VIRT_CPU_ACCOUNTING
~~~~~~~~~~~~~~~~~~~~
Boolean value that enables stats accounting and CPU time spent in the kernel.


VIRT_CPU_ACCOUNTING_GEN
~~~~~~~~~~~~~~~~~~~~~~~~
Related to CPU isolation, this configuration enable task and CPU time accounting on `full dynticks systems`_ also known as NO_HZ systems.
This accounting is implemented by watching every kernel-user boundaries using the context tracking subsystem.
The accounting is thus performed at the expense of some significant overhead.

HAVE_POSIX_CPU_TIMERS_TASK_WORK
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Used to handle posix CPU timers from task_work and not from the timer interrupt context.

POSIX_CPU_TIMERS_TASK_WORK
~~~~~~~~~~~~~~~~~~~~~~~~~~
Related to the previous configuration, this configuration depends on the previous one and the posix timers being enabled in the system.


>>>>

Debugging latencies issues
--------------------------
The next configurations are related to debugging and tracing latencies issues in the kernel.

TIMERLAT_TRACER
~~~~~~~~~~~~~~~
`Timerlat  tracer`_ helps to trace sources of wake up latency in the kernel.
It works by creating a per-cpu kernel thread that sets a periodic timer to wake up itself.
Then, it goes to sleep and waits for the timer to fire.
At the wake up, the thread then computes a wake up latency value as the difference between the current time and the absolute time that the timer was set to expire.

OSNOISE_TRACER
~~~~~~~~~~~~~~
In `high-performance computing (HPC)`_, system noise is the interference that an application can suffer from the system due to the action of internal system mechanisms.
In the context of Linux, this can be caused by `NMIs`_, `IRQs`_, `SoftIRQs`_ and other system threads.
Also Hardware-related jobs like `SMIs`_ can also be source of noise.


>>>>

IRQ Related configurations
--------------------------
One of the major sources of latency in the kernel are the IRQs. 
The next configurations are related to IRQs and how to account and avoid spent time on them.

NO_HZ_FULL
~~~~~~~~~~
The purpose of `NO_HZ`_ is to reduce the number of scheduling-clock interrupts generated by the kernel.
There are three possible levels of `NO_HZ`_: `CONFIG_HZ_PERIODIC`, `CONFIG_NO_HZ_IDLE` and `CONFIG_NO_HZ_FULL`.
**CONFIG_HZ_PERIODIC**, used when omitting scheduling-clock ticks is never desired.
Which is something not wanted in real-time systems. 
**CONFIG_NO_HZ_IDLE**, omits the scheduling-clock ticks when the system is idle.
**CONFIG_NO_HZ_FULL**, omits the scheduling-clock ticks when the system is idle and when there is only one task running.
This configuration is only recommended for real-time systems and certain types of `HPC`_ workloads.

IRQ_TIME_ACCOUNTING
~~~~~~~~~~~~~~~~~~~
Enables fine grained accounting of time spent in interrupt handlers.
This is done by reading a timestamp on each transitions between softirq and hardirq state.

HAVE_SCHED_AVG_IRQ
~~~~~~~~~~~~~~~~~~
Enables tracking and accounting for the CPU time consumed by IRQs within the scheduler's load tracking metrics.
This is important for accurately representing CPU usage, including time spent handling interrupts, which can affect scheduling decisions.
It works by enabling the usage of `update_irq_load_avg()`_ function in the context of `update_rq_clock_task()`_ scheduler function.


>>>>

Block I/O configurations
------------------------
The next configurations are related to block I/O and how to account and control the latency of I/O operations.

BLK_CGROUP_IOLATENCY
~~~~~~~~~~~~~~~~~~~~
Enabling this option enables the `.latency` interface for IO throttling.
This enables to have guarantees on IO latencies.


ARCH_WANT_HUGE_PMD_SHARE
~~~~~~~~~~~~~~~~~~~~~~~~
Memory management configuration that enables the sharing of huge `Page Middle Directory (PMD)` between processes.
Currently this feature is needed on modern architectures like **amd64**, **arm64** and **riscv64**.



.. LINKS
.. _other levels of preemption:  https://ubuntu.com/blog/what-is-real-time-linux-part-iii
.. _spinlock: https://docs.kernel.org/locking/spinlocks.html
.. _locking: https://docs.kernel.org/locking/locktypes.html
.. _NO_HZ: https://docs.kernel.org/timers/no_hz.html
.. _high-performance computing (HPC): https://youtu.be/tGIobcyKViI?si=u-trZgXQSjJSTedA
.. _HPC: https://youtu.be/tGIobcyKViI?si=u-trZgXQSjJSTedA
.. _Fibre Channel (FC): https://en.wikipedia.org/wiki/Fibre_Channel
.. _Timerlat  tracer: https://docs.kernel.org/trace/timerlat-tracer.html
.. _Preemption: https://en.wikipedia.org/wiki/Preemption_(computing)
.. _Page Middle Directory (PMD): https://www.kernel.org/doc/gorman/html/understand/understand006.html#htoc15
.. _NMIs: https://en.wikipedia.org/wiki/Non-maskable_interrupt
.. _IRQs: https://docs.kernel.org/core-api/irq/concepts.html#what-is-an-irq
.. _SoftIRQs: https://archive.kernel.org/oldlinux/htmldocs/kernel-hacking/basics-softirqs.html
.. _SMIs: https://docs.kernel.org/arch/x86/microcode.html#system-management-interrupt
.. _preempt_count(): https://lwn.net/Articles/831678/
.. _RCU - Read-copy update: https://docs.kernel.org/RCU/whatisRCU.html
.. _Symmetric multiprocessing (SMP): https://en.wikipedia.org/wiki/Symmetric_multiprocessing
.. _call_rcu(): https://docs.kernel.org/RCU/UP.html
.. _full dynticks systems: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/ticklesskernel
.. _context tracking header: https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/context_tracking.h?h=v5.15
.. _update_irq_load_avg(): https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/kernel/sched/pelt.c?h=v5.15#n434
.. _update_rq_clock_task(): https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/kernel/sched/core.c?h=v5.15#n610
.. _SCHED_FIFO: https://man7.org/linux/man-pages/man7/sched.7.html