Linux Kernel Schedulers
=======================

Schedulers are like the traffic police of the kernel. They decide which process 
gets to run and for how long and when they run. The Linux kernel has several
schedulers, each with its own strengths and weaknesses. The default scheduler is 
the `Completely Fair Scheduler <#cfs-completely-fair-scheduler>`_, which is 
designed to provide fair CPU time to all processes. However, there are other 
schedulers that are optimized for specific use cases, such as the 
`Deadline Scheduler <#deadline-scheduler>`_ and the 
`Real-Time Scheduler <#real-time-scheduler>`_, also many other with different use
cases.


CFS - Completely Fair Scheduler
-------------------------------

`The Completely Fair Scheduler`_ (CFS) is the default scheduler in the Linux 
kernel. It is designed to provide fair CPU time to all processes. It uses a 
`Red-black tree`_ data structure to keep track of the processes and their CPU time. 
The CFS scheduler assigns CPU time to processes based on their priority and the
amount of CPU time they have used. It is a good general-purpose scheduler, but it
may not be the best choice for real-time or high-performance applications.

The base of the CFS is the concept of virtual runtime. The virtual runtime is
the amount of time a process has been running on the CPU. The CFS scheduler
uses the virtual runtime to determine which process should run next. The process
with the smallest virtual runtime gets to run next. This ensures that all
processes get a fair share of the CPU time.

.. image:: cfs_scheduler.svg
    :width: 80%
    :align: center
    :alt: Red-black tree data structure used by the CFS scheduler


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


Deadline Scheduler
------------------

`The Deadline scheduler`_ is designed for real-time applications that require
guaranteed CPU time. It uses a deadline-based scheduling algorithm to ensure
that processes meet their deadlines. The Deadline Scheduler is optimized for
real-time applications, such as multimedia and gaming, where low latency and
predictable performance are critical.


Real-Time Scheduler
-------------------

`The Real-Time Scheduler`_ is designed for real-time applications that require
guaranteed CPU time. It uses a priority-based scheduling algorithm to ensure
that high-priority processes get the CPU time they need. The Real-Time Scheduler
is optimized for real-time applications, such as industrial control systems and
embedded devices, where low latency and predictable performance are critical.


.. Links

.. _`Red-black Tree`: https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
.. _`The Completely Fair Scheduler`: https://docs.kernel.org/scheduler/sched-design-CFS.html
.. _`The Deadline scheduler`: https://docs.kernel.org/scheduler/sched-deadline.html
.. _`The Real-Time Scheduler`: https://docs.kernel.org/scheduler/sched-rt-group.html
.. _`The Capacity Aware Scheduling`: https://www.kernel.org/doc/html/latest/scheduler/sched-capacity.html
.. _`The Energy Aware Scheduling`: https://www.kernel.org/doc/html/latest/scheduler/sched-energy.html

