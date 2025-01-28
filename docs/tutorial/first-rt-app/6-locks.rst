Locks
=====

Perhaps you noticed that none of the examples so far were thread-safe. In each
one, a single thread-function ran on two threads and included this operation:

.. code-block:: C

   --calls_remaining

where ``calls-remaining`` was a global variable.

In those examples, clarity mattered more and so locks were left out. Generally
though, they're essential --- thread-safety is paramount. Download :download:`lock.c` to
see one way of making the :file:`fifo.c` example thread-safe:

.. code-block:: C

   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

   void *thread_start(void *arg)
   {
   ...
           while (calls_remaining > 0) {
           ...
                   pthread_mutex_lock(&mutex);
                   --calls_remaining;
                   pthread_mutex_unlock(&mutex);
           }

Here for convenience we declare a global, utility mutex and initialize it once
at start-up. Other approaches are possible, notably initializing locks that are
local not global and require more-specialized behavior.

Priority inversion
------------------

A note on priority inversion is in order. (Demonstrating it, and Real-time
Ubuntu's solutions to it, are out of scope here. See Canonical's `Technical
deep-dive`_ for more detail.)

Suppose a lock is held on a section of critical kernel code, by a low-priority
task that's then pre-empted by a high-priority one. If this second task needs
the same lock, it must yield and retry repeatedly until it can acquire it.

Meanwhile suppose the low-priority task, running in the critical section, is
pre-empted by a medium-priority one also in kernel code. Priorities have been
inverted: the high-priority task can't execute critical kernel code while the
medium-priority one continues --- perhaps indefinitely.

Real-time Ubuntu makes special provision for real-time tasks that must be
pre-emptible within specified time-limits. There's a range of locking
mechanisms, and locks can sleep --- they don't disable interrupts or
pre-emption. A task holding a lock can inherit the higher priority of a task
that's waiting on it. With no priority levels between them, no
intermediate-priority task can cause priority inversion.


.. LINKS:
.. _`Technical deep-dive`: https://ubuntu.com/blog/real-time-kernel-technical
