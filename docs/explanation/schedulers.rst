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
value, which is a value that can be set by the user by using the `nice`_, 
`setpriority`_, or `sched_setattr`_ system calls (`syscalls`_) to change the 
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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


Early Deadline First Scheduler
------------------------------

`The Deadline scheduler`_ is designed for real-time applications that require
guaranteed CPU time. It uses a deadline-based scheduling algorithm to ensure
that processes meet their deadlines. The Deadline Scheduler is optimized for
real-time applications, such as multimedia and gaming, where low latency and
predictable performance are critical.


Capacity Aware Scheduling
-------------------------
`The Capacity Aware Scheduling`_ is designed to provide better performance for
multi-core systems. It uses a capacity-based scheduling algorithm to ensure that
processes are scheduled on the cores with the most capacity. This can improve
the performance of multi-threaded applications and reduce contention for shared
resources.


Energy Aware Scheduling
-----------------------

`The Energy Aware Scheduling`_ is designed to reduce the energy consumption of
the system. It uses an energy-based scheduling algorithm to ensure that processes
are scheduled on the cores with the lowest energy consumption. This can reduce
the power consumption of the system and extend the battery life of mobile devices.


.. Links

.. _`Red-black Tree`: https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
.. _`The Completely Fair Scheduler`: https://docs.kernel.org/scheduler/sched-design-CFS.html
.. _`The Deadline scheduler`: https://docs.kernel.org/scheduler/sched-deadline.html
.. _`The Real-Time Scheduler`: https://docs.kernel.org/scheduler/sched-rt-group.html
.. _`The Capacity Aware Scheduling`: https://www.kernel.org/doc/html/latest/scheduler/sched-capacity.html
.. _`The Energy Aware Scheduling`: https://www.kernel.org/doc/html/latest/scheduler/sched-energy.html
.. _`nice`: https://man7.org/linux/man-pages/man2/nice.2.html
.. _`setpriority`: https://man7.org/linux/man-pages/man2/setpriority.2.html
.. _`sched_setattr`: https://man7.org/linux/man-pages/man2/sched_setattr.2.html
.. _`syscalls`: https://man7.org/linux/man-pages/man2/syscalls.2.html