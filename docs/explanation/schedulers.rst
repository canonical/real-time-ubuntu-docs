Linux Kernel Schedulers
=======================

Schedulers are like the traffic police of the kernel. They decide which process 
gets to run and for how long and when they run. The Linux kernel has several
schedulers, each with its own strengths and weaknesses. The default scheduler is 
the `Completely Fair Scheduler <#cfs-completely-fair-scheduler>`_, which is 
designed to provide fair CPU time to all processes. However, there are other 
schedulers that are optimized for specific use cases, such as the 
`Early Deadline First Scheduler <#early-deadline-first-scheduler>`_ and the 
`Real-Time Scheduler <#real-time-scheduler>`_, also many others with different
use cases.

Different classes of schedulers are available in the Linux kernel, each class has
it's own set of scheduler policies, like showed below:

.. table:: 

   +------------------+-------------------------+
   | Scheduling Class | Scheduling Policy       |
   +==================+=========================+
   | CFS              | SCHED_OTHER             |            
   +------------------+-------------------------+
   |                  | SCHED_BATCH             |           
   +------------------+-------------------------+
   |                  | SCHED_IDLE              |           
   +------------------+-------------------------+
   | RT               | SCHED_RR                |
   +------------------+-------------------------+
   |                  | SCHED_FIFO              |
   +------------------+-------------------------+
   | EDF              | SCHED_DEADLINE          |
   +------------------+-------------------------+
   | CAS              | Capacity Aware Scheduler|
   +------------------+-------------------------+
   | EAS              | Energy Aware Scheduler  |
   +------------------+-------------------------+

CFS - Completely Fair Scheduler
-------------------------------

`The Completely Fair Scheduler`_ (CFS) is the default scheduler in the Linux 
kernel and it is designed to provide fair CPU time to all processes. It uses a 
`Red-black tree`_ data structure to keep track of the processes and their CPU
time. The CFS scheduler assigns CPU time to processes based on their priority
and the amount of CPU time they have used. It is a good general-purpose scheduler, 
but it may not be the best choice for real-time or high-performance applications.

The base of the CFS is the concept of virtual runtime. This concept is based on 
an "ideal multi-tasking CPU" (which in practice doesn't exist) that can
distribute all of this physical power and run each task at precise equal speed,
in parallel. As in real hardware it's only possible to run one task at a time,
CFS uses the concept of virtual runtime to simulate this ideal CPU.

The virtual runtime is the amount of time a process has been running on the CPU. 
The CFS scheduler uses the virtual runtime as the parameter to determine which
process should run next. The process with the smallest virtual runtime gets to
run next. This ensures that all processes get a fair share of the CPU time.

Based on the virtual runtime, the CFS strategy is to select the process with the
smallest virtual runtime to run next. This is done by using a red-black tree to
keep track of the processes and their virtual runtimes. The red-black tree is a
self-balancing binary search tree that ensures that the processes are sorted by
their virtual runtimes. This allows the CFS scheduler to quickly find the process
with the smallest virtual runtime and schedule it to run next, like showed in the
image below.

.. image:: cfs_scheduler.svg
    :width: 80%
    :align: center
    :alt: Red-black tree data structure used by the CFS scheduler

SCHED_OTHER - Normal Scheduling
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``SCHED_OTHER`` policy (currently renamed to ``SCHED_NORMAL``) is the scheduler
policy used for regular tasks, it is, tasks with static priority that don't have
real-time requirements. Alongside with this policy there is the concept of nice 
value, which is a value that can be set by the user by using the `nice(2)`_, 
`setpriority(2)`_, or `sched_setattr(2)`_ system calls (`syscalls`_) to change the 
priority of a task. The nice value ranges from -20 to 19, where -20 is the 
highest priority and 19 is the lowest priority. The default nice value is 0.

SCHED_BATCH - Batch Scheduling
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``SCHED_BATCH`` policy is used for batch processing tasks, it is, tasks that
are not time-critical and can run in the background. The batch scheduler is
optimized for throughput and can run tasks at a lower priority to reduce the
impact on interactive tasks. This scheduler policy is affected by the nice value
as well.

SCHED_IDLE - Idle Scheduling
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``SCHED_IDLE`` policy is used for idle tasks, it is, tasks that are not
time-critical and can run when the system is idle (has nothing else to do). 
The idle scheduler is intended to run tasks at a extremely lower priority. Its
used by the kernel to run background tasks, such as housekeeping and maintenance
tasks, when the system is idle. This scheduler policy isn't affected by the nice 
value.

Real-Time Scheduler
-------------------

`The Real-Time Scheduler`_ is designed for real-time applications that require
guaranteed CPU time. It uses a priority-based scheduling algorithm to ensure
that high-priority processes get the CPU time they need. The Real-Time Scheduler
is optimized for real-time applications, such as industrial control systems and
embedded devices, where low latency and predictable performance are critical.

SCHED_FIFO - First In, First Out Scheduling
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``SCHED_FIFO`` policy is used for real-time tasks that require guaranteed
CPU time. It is a simple first-come, first-served (`FIFO`_) scheduler that gives
the highest priority to the task with the highest priority level. The first-in,
first-out scheduler is used to ensure that real-time tasks get the CPU time they
need without being interrupted by lower-priority tasks. In this policy, tasks 
receives a priority level from 1 to 99, where 1 is the highest priority and 99
is the lowest priority.

SCHED_RR - Round Robin Scheduling
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``SCHED_RR`` is a simple enhancement of ``SCHED_FIFO`` policy, but it allows
tasks to share the same priority level. The round-robin scheduler is used to
ensure that real-time tasks get the CPU time they need without starving 
lower-priority tasks.

As the name suggests, the ``SCHED_RR`` policy is based on 
`Round-robin scheduling`_, algorithm, which is a simple scheduling algorithm 
that assigns a fixed time slice to each task in a cyclic queue. When a task's 
time slice expires, it is moved to the end of the queue and the next task is 
scheduled to run, regardless if the task executions is finished or not. This
ensures that all tasks get a fair share of the CPU time and that no task is
starved.

Let's see an example of how the ``SCHED_RR`` policy works. Suppose we have five 
tasks with the same priority level and a time slice of 100ms. The tasks are
scheduled to run in the following order:

.. table:: 

   +--------------+--------------+----------------+
   | Process name | Arrival time | Execution time |
   +==============+==============+================+
   | Pa           | 0            | 250            |            
   +--------------+--------------+----------------+
   | Pb           | 50           | 120            |            
   +--------------+--------------+----------------+
   | Pc           | 170          | 50             |            
   +--------------+--------------+----------------+
   | Pd           | 180          | 100            |            
   +--------------+--------------+----------------+
   | Pe           | 210          | 130            |            
   +--------------+--------------+----------------+

Note that the picture bellow index of the tasks represents the remaining time in
milliseconds for each task to finish its execution.

.. image:: rr_scheduler.svg
    :width: 80%
    :align: center
    :alt: Example of Round-robin scheduling

Early Deadline First Scheduler
------------------------------

`The Deadline scheduler`_ is designed for real-time applications that require
guaranteed CPU time. It uses a deadline-based scheduling algorithm to ensure
that processes meet their deadlines. The Deadline Scheduler is optimized for
real-time applications, such as multimedia and gaming, where low latency and
predictable performance are critical.

The Deadline scheduling policy is implemented using the GEDF (Global Earliest
Deadline First) algorithm, which is a global scheduling algorithm that assigns
CPU time to processes based on their deadlines. The GEDF algorithm ensures that
processes meet their deadlines by scheduling them to run on the CPU at the
earliest possible time. This can improve the performance of real-time applications
and reduce the risk of missed deadlines.

A sporadic task involves a series of jobs, with each job activated no more than
once per **period**. Each job is assigned a **relative deadline**, indicating 
when it must finish execution, and a **computation time**, representing the CPU 
time required for its execution. The point at which a task becomes active 
(wake up) due to a new job that needs execution is termed the **arrival time**,
also known as the request time or release time. The **start time** denotes when 
a task begins its execution. Consequently, the **absolute deadline** is 
determined by adding the relative deadline to the arrival time.

.. image:: edf_scheduler.svg
    :width: 80%
    :align: center
    :alt: Diagram of scheduling

When configuring a ``SCHED_DEADLINE`` thread with `sched_setattr(2)`_ syscall, 
three parameters can be defined: **Runtime**, **Deadline**, and **Period**. 
These settings may not always match the terms mentioned before: typically, 
Runtime is set higher than the average processing time (or worst-case execution 
time for hard real-time tasks), Deadline aligns with the relative deadline, and 
Period matches the task's duration. Therefore, in the case of scheduling with
``SCHED_DEADLINE``, we have:

.. image:: edf_sched_scheduler.svg
    :width: 80%
    :align: center
    :alt: Diagram of scheduling with SCHED_DEADLINE


Capacity Aware Scheduling
-------------------------
`The Capacity Aware Scheduling`_ optimizes CPU resource allocation based on task
requirements and system capabilities. It quantifies task utilization as a
percentage of CPU capacity, allowing for efficient scheduling in systems with
variable CPU frequencies and asymmetric CPU capacities. The scheduler uses the
original capacity of a CPU and its adjusted capacity to ensure tasks are
scheduled on CPUs with sufficient capacity. The Completely Fair Scheduler (CFS)
uses `Per-Entity Load Tracking`_ (PELT) to estimate task utilization, ensuring
tasks are scheduled based on their capacity fitness criterion. This mechanism
ensures that tasks running on CPUs of different capacities yield consistent duty
cycles, regardless of their absolute performance levels. CPU invariance is
achieved by adjusting the task utilization signal based on the CPU's capacity
relative to the system's maximum capacity. The scheduler topology plays a 
significant role in capacity-aware scheduling, especially in systems with
asymmetric CPU capacities, by setting specific flags to indicate asymmetric CPU
capacities and to ensure tasks are scheduled on CPUs with sufficient capacity.
This approach ensures efficient and fair utilization of system resources across
different types of tasks and CPUs.

Energy Aware Scheduling
-----------------------

`The Energy Aware Scheduling`_ (EAS) aims to optimize CPU task placement based
on energy consumption, focusing on minimizing energy use while maintaining
performance. EAS operates on heterogeneous CPU topologies, such as Arm's
`big.LITTLE`_, where the potential for energy savings is highest. It relies on an
Energy Model (EM) to predict the energy impact of scheduling decisions, selecting
the most energy-efficient CPU for each task without significantly affecting
throughput. The EM is not maintained by the scheduler but by a dedicated
framework, ensuring it remains simple to minimize scheduler latency impact. EAS
introduces an alternative optimization objective to the traditional
performance-only approach, considering both energy efficiency and performance.
During task wake-up, the EM helps the scheduler choose the best CPU candidate
based on predicted energy consumption, taking into account the platform's
topology, CPU capacities, and energy costs. EAS calculates total energy
consumption for different CPU placements, selecting the option with the lowest
total energy. This approach considers that big CPUs are generally more
power-hungry and are used mainly when tasks don't fit the little cores. However,
the energy efficiency of little CPUs can vary, and in some cases, a small task
might be better off executing on a big core to save energy, despite fitting on a
little core. EAS requires specific hardware properties and kernel features,
including an asymmetric CPU topology, the presence of an Energy Model, and the
Schedutil governor. It also depends on scale-invariant utilization signals and
support for Multithreading (SMT). The platform must provide power cost tables to
the EM framework for EAS to function, necessitating the re-building of 
scheduling domains after the EM registration.

References
-----------

- `Man page on sched(7)`_
- `The Completely Fair Scheduler`_ 
- `The Deadline scheduler`_
- `The Real-Time Scheduler`_
- `The Capacity Aware Scheduling`_
- `The Energy Aware Scheduling`_


.. Links

.. _`Red-black Tree`: https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
.. _`The Completely Fair Scheduler`: https://docs.kernel.org/scheduler/sched-design-CFS.html
.. _`The Deadline scheduler`: https://docs.kernel.org/scheduler/sched-deadline.html
.. _`The Real-Time Scheduler`: https://docs.kernel.org/scheduler/sched-rt-group.html
.. _`The Capacity Aware Scheduling`: https://www.kernel.org/doc/html/latest/scheduler/sched-capacity.html
.. _`The Energy Aware Scheduling`: https://www.kernel.org/doc/html/latest/scheduler/sched-energy.html
.. _`nice(2)`: https://man7.org/linux/man-pages/man2/nice.2.html
.. _`setpriority(2)`: https://man7.org/linux/man-pages/man2/setpriority.2.html
.. _`sched_setattr(2)`: https://man7.org/linux/man-pages/man2/sched_setattr.2.html
.. _`syscalls`: https://man7.org/linux/man-pages/man2/syscalls.2.html
.. _`Round-robin scheduling`: https://en.wikipedia.org/wiki/Round-robin_scheduling
.. _`FIFO`: https://en.wikipedia.org/wiki/FIFO_(computing_and_electronics)
.. _`Man page on sched(7)`: https://man7.org/linux/man-pages/man7/sched.7.html
.. _`sched_setattr(2)`: https://man7.org/linux/man-pages/man2/sched_setattr.2.html
.. _`Per-Entity Load Tracking`: https://lwn.net/Articles/531853/
.. _`big.LITTLE`: https://www.arm.com/technologies/big-little