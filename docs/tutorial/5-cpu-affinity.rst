CPU affinity
============

In many applications, you can improve real-time performance by allocating
processor cores carefully.

There are two main contenders for processor resources: threads and interrupt
handlers. In this section you'll manage their impact by configuring their CPU
affinity --- that is, by corralling them to subsets of the available cores. (A
target with at least four cores is assumed here; the example system has 12.)

Assigning cores to threads
--------------------------

In fact you've already set CPU affinity: the scheduling examples used it to
improve consistency somewhat. In those examples, we skipped over the relevant
code; now let's examine it. Open your local :file:`cfs.c` and find this
section:

.. code-block:: C

   /*-- Run on first core only --*/

   cpu_set_t cpu_set;
   CPU_ZERO(&cpu_set);
   CPU_SET(FIRST_CORE, &cpu_set);
   if (0 != sched_setaffinity(0, sizeof(cpu_set), &cpu_set)) {
           printf("Failed to set CPU affinity\n");
           exit(EXIT_FAILURE);
   }

A ``cpu_set_t`` is a set of available processor cores. You'll need one to
designate cores in any code that affects CPU affinity. ``cpu_set_t`` is opaque,
so you're provided with system macros for manipulating it.

First you prepare the set: ``CPU_ZERO()`` empties it, leaving no cores
designated. Next, with ``CPU_SET()`` you add a core to the set: above, only the
first available core (numbered 0) is to be used. Now the set is ready.

Above, you passed it to ``sched_setaffinity()``; using 0 as the first parameter
assigned the CPU affinity of the caller, ``main()``. Both threads that were
launched from ``main()`` inherited its affinity, so this was a straightforward
way to make them run on the same core.

Configuring threads individually
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The above approach is handy for assigning a core (or cores) to several threads.
You can also call ``sched_setaffinity()`` on an individual thread (say, within
its thread function) to assign specific cores.

A more-flexible approach can be used anywhere. Download `thread-affinity.c`_
and browse to where it spawns two new threads:

.. code-block:: C

   /*-- Call thread function on separate threads --*/

   if (0 != pthread_create(&thread1,
   ...
   CPU_ZERO(&cpu_set);
   CPU_SET(FIRST_CORE, &cpu_set);
   pthread_setaffinity_np(thread1, sizeof(cpu_set), &cpu_set);

   if (0 != pthread_create(&thread2,
   ...
   CPU_ZERO(&cpu_set);
   CPU_SET(SECOND_CORE, &cpu_set);
   CPU_SET(THIRD_CORE, &cpu_set);
   pthread_setaffinity_np(thread2, sizeof(cpu_set), &cpu_set);

Instead of calling ``sched_setaffinity()``, here you'll pass the IDs of
individual threads to ``pthread_setaffinity_np()`` and assign different
core-sets to each. Build and run the code:

.. code-block:: shell-session

   $ gcc thread-affinity.c -o thread-affinity

   $ sudo ./thread-affinity
   thread1 priority: 0
   thread2 priority: 0
   Calls made on thread1: 49471
   Calls made on thread2: 50591

In combination with other techniques, specifying cores for selected threads has
a significant impact on real-time applications. Here interrupts and other
processes are vying for time, so the effect is less noticeable.

Assigning cores to interrupts
-----------------------------

In addition to threads, the other demand for processing comes from interrupts.
CPU affinity can be configured for those too. This can have significant
benefits for real-time applications, where asynchronous, critical events are
routinely assigned to interrupt handlers.

It can be risky to play with interrupts' core-assignments, but most computers
have an Ethernet interface which should be safe. Find the name of an available
one:

.. code-block:: shell-session

   $ ip -details link show
   ...
   2: eno1: ...
       link/ether ... parentbus pci parentdev 0000:00:1f.6
   ...

Above, ``eno1`` is listed as a valid Ethernet interface. (Your system may
differ.) Next find the associated interrupt, checking that its bus type (PCI)
and address (0000:00:1f.6) match the above:

.. code-block:: shell-session

   $ grep eno1 /proc/interrupts
    127: ... PCI-MSI-0000:00:1f.6    0-edge      eno1

On this system, ``eno1`` raises interrupt request (IRQ) 127. (Again, yours may
differ.) In the :file:`proc` pseudo-filesystem, enter the subdirectory for IRQ
127 then display its current interrupt-affinity mask:

.. code-block:: shell-session

   $ cd /proc/irq/127
   $ cat smp_affinity
   040

:file:`smp_affinity` contents are in hex, with a set bit for each core that may
handle the IRQ in question. (The system in the above example, with 12 cores,
requires three nibbles for :file:`smp_affinity`.) Currently only the seventh
core --- core six --- is assigned to IRQ 127.

On systems with more than one assigned core, :file:`smp_affinity_list` may be
easier to interpret than :file:`smp_affinity`. :file:`smp_affinity_list` gives
the same information, in a delimited set of decimal core-numbers:

.. code-block:: shell-session

   $ cat smp_affinity_list
   6

On the example system shown above, reallocating the ``eno1`` interrupt handler
from core six to cores seven and eight yielded this:

.. code-block:: shell-session

   $ sudo -i
   # echo 180 > smp_affinity
   # cat smp_affinity_list
   7-8

If you experiment with something similar on your target, remember to move it
back. On the example system, that was done like this:

.. code-block:: shell-session

   # sudo echo 040 > smp_affinity
   # cat smp_affinity_list
   6
   # exit


.. LINKS:
.. _`thread-affinity.c`: https://github.com/lblythen/real-time-ubuntu-docs/blob/feature/doc_first-tutorial/docs/tutorial/thread-affinity.c
