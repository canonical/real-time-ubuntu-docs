Task schedulers
===============

The Linux kernel treats every execution thread as a task. Real-time Ubuntu
enhances the available schedulers for choosing which pending task to process.
You can greatly improve real-time performance by assigning and tuning a
scheduler appropriately.

The default: Completely Fair Scheduling
---------------------------------------

Let's see briefly what real-time requirements are up against. The default
scheduler --- ``SCHED_OTHER``, in the Completely Fair Scheduling (CFS) class
--- ensures that every task gets some processing time. To see it in action,
download :download:`cfs.c` then build and run it:

.. code-block:: shell-session

   $ gcc cfs.c -o cfs

   $ sudo ./cfs
   Calls made on thread1: 49893
   Calls made on thread2: 50108

:file:`cfs` lets two threads contribute to the same job, then displays how many
times each one lent a hand. By default, ``SCHED_OTHER`` assigns equal
processing time to new threads.

However, suppose code on one thread has to deliver real-time performance. With
CFS, the nearest we can get is by making other threads "behave as nicely as
possible", so they'll keep out of the way. To see the effect, open your local
:file:`cfs.c` and change ``false`` to ``true`` on this line:

.. code-block:: C

   static bool make_thread1_nicer = false;

Rebuild and run it:

.. code-block:: shell-session

   $ gcc cfs.c -o cfs

   $ sudo ./cfs
   Calls made on thread1: 1413
   Calls made on thread2: 98588

Clearly some tuning's possible even with CFS. However, it remains a cooperative
balancing-act and the range of "nice" values is limited deliberately. CFS can't
approach real-time performance.

RT: priority-based scheduling
-----------------------------

In the Real-time (RT) class, schedulers grant processing time based on each
task's priority. Unlike CFS, RT doesn't use the "niceness" mechanism and isn't
constrained by its limited range.

Let's see the effect of assigning equal priorities to identical tasks. Download
the :download:`fifo.c` example; it does the same as :file:`cfs` above but uses
``SCHED_FIFO``, an RT-class scheduler. Build and run it:

.. code-block:: shell-session

   $ gcc fifo.c -o fifo

   $ sudo ./fifo
   thread1 priority: 0
   thread2 priority: 0
   Calls made on thread1: 49467
   Calls made on thread2: 50534

With both threads granted the same priority by default, again they contribute
about the same to the overall job. (For RT as for CFS, their contributions
aren't identical. The scheduler plays its part, but there are factors involved
besides priority. With Real-time Ubuntu you can control many of them.)

Changing priority at run-time
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Next open your local :file:`fifo.c` and find these commented-out lines:

.. code-block:: C

   /* param1.sched_priority = MIN(89,
                                  sched_get_priority_max(SCHED_FIFO)); */
   ...
   /* param2.sched_priority = sched_get_priority_min(SCHED_FIFO); */

Reinstate them then rerun the code, to assign maximum and minimum RT priorities
at run-time:

.. code-block:: shell-session

   $ gcc fifo.c -o fifo

   $ sudo ./fifo
   thread1 priority: 89
   thread2 priority: 1
   Calls made on thread1: 97784
   Calls made on thread2: 2217

Task priorities are under tighter control with RT than with CFS, and the range
is wider: 1 - 99. (It's advisable to leave 90 - 99 for critical kernel tasks.)
Here thread1 routinely pre-empts thread2: with each call, thread1 can make its
contribution then reappear at higher priority on the run queue.

RT affords useful control for real-time performance, but there's a trade-off.
Without due care, threads can be starved: always at a lower priority than at
least one thread, which therefore never lets them run. A particular risk is
priority inversion, described in Canonical's `Technical deep-dive`_ (with a
famous example).

SCHED_FIFO vs SCHED_RR
~~~~~~~~~~~~~~~~~~~~~~

There's another trade-off too. Consider what happens if two or more pending
tasks share the same priority: somehow an RT-class scheduler must choose only
one.

``SCHED_FIFO`` will pick the task that's ready first --- First In, First Out.
Unless that task is pre-empted by a higher-priority one, it's free to keep
running and might starve others with the same priority.

The alternative RT-class scheduler, ``SCHED_RR``, is fairer: it uses a Round
Robin algorithm to choose between tasks of the same priority. However, it may
switch tasks more often than ``SCHED_FIFO`` --- and context-switching has an
overhead. As always, tuning requires care, and detailed knowledge of your
application.

EDF: scheduling to meet deadlines
---------------------------------

Real-time Ubuntu's Earliest Deadline First (EDF) class caters to tasks that
mustn't be skipped, or even delayed beyond a critical time. An EDF-scheduled
task receives at least its specified *runtime*, within a specified *deadline*
from the start of each defined, regular *period*.

EDF provides a single scheduler: ``SCHED_DEADLINE``. If two or more EDF tasks
are pending, the one with the closest deadline is processed first. EDF suits
critical sporadic tasks; it isn't subject to CFS's "nice" mechanism or RT's
priorities.

Download :download:`edf.c` and open it. Its thread function uses a do-nothing loop, but
instead it might service a sensor: one whose data must be processed within 11
ms of the start of every two-second period, with up to 10 ms needed for
processing.

Build and run the code:

.. code-block:: shell-session

   $ gcc edf.c -o edf

   $ sudo ./edf
   thread1:   period =  2 s
             runtime = 10 ms
            deadline = 11 ms
   thread2:   period =  2 s
             runtime = 10 ms
            deadline = 11 ms

   Calls made on thread1: 49994
   Calls made on thread2: 50007

With the same parameters, again each critical task contributes about the same
to the overall job.

Points to note
~~~~~~~~~~~~~~

EDF-scheduled tasks are guaranteed regular attention without contending for
priority. However, each one must complete within its runtime, on every period.
Should a task fail to do that, by enough to overrun its deadline, it can cause
flow-on disruption to others. Whilst an interrupt may be used to terminate it,
that's risky for a critical task.

:file:`edf` configured thread-attributes differently from what you saw for
other schedulers: it used ``sched_setattr()`` wrapped in a syscall.
``sched_setattr()`` set the attributes of its caller, so you issued it on the
thread you were configuring. The technique, and the ``sched_attr`` structure,
can be used for all schedulers and scheduling classes.

``sched_attr``'s time fields are in nanoseconds.

Scheduler-class hierarchy
-------------------------

With Real-time Ubuntu installed, the run-queues of all scheduler classes are
serviced in a hierarchy: EDF's first, then RT's, then CFS's (which has
schedulers besides ``SCHED_OTHER``, of little interest here).

Therefore pending tasks with deadlines will pre-empt even high-priority RT ones
--- and the lowest-priority RT task will pre-empt even the least-nice CFS one.
Choosing the right scheduler for real-time tasks is as influential as tuning
its parameters.


.. LINKS:
.. _`Technical deep-dive`: https://ubuntu.com/blog/real-time-kernel-technical
