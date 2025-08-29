Linux kernel locks
==================

.. note::

  This documentation covers some kernel internals specific to Linux kernel v6.8.
  The details provided here may not fully apply to newer kernel versions.

  The interface of kernel locks hasn't changed significantly over the years, nevertheless they are considered as unstable and subject to change.
  This is discussed in the `The Linux Kernel Driver Interface`_ documentation.


In the `kernel space`_, certain critical code regions across all `subsystems`_ must be protected from `context switching`_ or, in the case of real-time kernels, from `preemption`_.
On `SMP systems`_ (Symmetric Multiprocessing), the Linux kernel provides various synchronization mechanisms to safeguard shared data structures from concurrent access, preventing `race conditions`_ in the kernel space.

Locks are primarily used to prevent race conditions, which are a concern in both SMP and real-time systems.
Even in a single-processor real-time system, the preemption mechanism can lead to race conditions similar to those found in an SMP system.

There are essentially three classes of locks in the kernel:

- `Spinning locks`_
- `Sleeping locks`_ (also known as blocking locks)
- `CPU local locks`_

Spinlocks
----------

Spinlocks (found in `include/linux/spinlock.h`_) are locks that keep polling (or "spinning"), attempting to acquire the lock.
They are small, fast, and simple to use, making them generally applicable throughout the kernel.
Spinlocks are ideal for situations where the kernel process cannot sleep.
The ideal type of spinlock to use depends on the specific contexts between which the data needs to be shared.

Within the class of spinlocks, there are various types:

.. _rwlock_t:

rwlock_t
~~~~~~~~

``rwlock_t`` (defined on `include/linux/rwlock_types.h`_)  is a variant of the read/write lock for spinning locks.
Consumers of this lock are divided into two classes: readers and writers.
This lock structure allows multiple readers but only one writer at a time.
By implementing this approach, the number of locks used can be reduced, which in turn decreases the overhead associated with locking.
This lock behaves different on ``PREEMPT_RT`` kernels.

- On non-PREEMPT_RT kernels it relies on the architecture dependent assembly implementation of a ``arch_rwlock_t``.
- On PREEMPT_RT kernels it is implemented as a ``rwbase_rt``, realizing a sleeping lock.

.. _spinlock_t:

spinlock_t
~~~~~~~~~~

``spinlock_t`` is the most generic implementation of spinlock structures, as defined in `include/linux/spinlock_types.h`_.
It is implemented in two different ways, depending on whether the real-time kernel configuration (i.e., with ``PREEMPT_RT`` enabled) is used.

- In non-PREEMPT_RT scenarios, ``spinlock_t`` maps to :ref:`raw_spinlock_t`.
- In PREEMPT_RT kernels, this is implemented as a :ref:`rt_mutex`, making this a sleeping lock.

.. _raw_spinlock_t:

raw_spinlock_t
~~~~~~~~~~~~~~

``raw_spinlock_t`` (defined on `include/linux/spinlock_types_raw.h`_) is a strict spinning lock implementation that operates consistently across all kernels, including real-time kernels.
It is reserved for critical core code, low-level interrupt handling, and scenarios where disabling preemption or interrupts is necessary.
For example, this lock is used to safely access hardware state.
In some cases, ``raw_spinlock_t`` can also be employed when the critical section is very small, thereby avoiding the overhead associated with :ref:`rt_mutex`.

mutex
~~~~~

A Mutex, short for "mutual exclusion" is used to protect critical sections of code, ensuring that only one thread or process can access a shared resource at any given time.
Specifically, the ``mutex`` type (defined on `include/linux/mutex_types.h`_) is a simple mutex implementation that varies depending on whether the current kernel is a PREEMPT_RT kernel or not.

- For non-PREEMPT_RT kernels, the implementation relies on an atomic operator which holds the owner and a :ref:`raw_spinlock_t`
- On PREEMPT_RT kernels, the implementation is based on :ref:`rt_mutex`.

The primary use case for this type of lock occurs when data needs to be shared between different user contexts.

.. _rt_mutex:

rt_mutex
~~~~~~~~~~

RT-mutexes (defined on `include/linux/rtmutex.h`_) are a special type of mutex that support priority inheritance.
These mutexes are particularly useful in real-time kernels because priority inheritance is a protocol designed to mitigate the effects of `priority inversion`_.
It's important to note that priority inheritance has limitations in non-real-time kernels due to preemption and interrupt-disabled sections.
Additionally, priority inheritance cannot take effect in regions of code where preemption or interrupts are disabled.
The benefits of priority inheritance are confined to regions where preemption is enabled (i.e., in preemptive task contexts).
Essentially, RT-mutexes can be used to implement spinlock_t and read-write locks, providing the added advantage of priority inheritance where applicable.

semaphore
~~~~~~~~~~

The ``semaphore`` (defined in `include/linux/semaphore.h`_) structure discussed here follows the counting semaphore structure (as the binary semaphore equivalent is essentially a mutex).
It's a foundational implementation intended for use within other kernel lock structures.
Internally, it relies on a :ref:`raw_spinlock_t` and a counter.
Their primary use cases involve serialization and waiting.
It's important to note that newer mechanisms, such as mutexes for serialization and `completions`_ for waiting, are generally preferable to traditional semaphores.


rw_semaphore
~~~~~~~~~~~~

``rw_semaphore`` (Read-Write Semaphore, defined on `include/linux/rwsem.h`_) is a variant of the read/write lock for sleeping locks.
The goal of this locking mechanism is to reduce the overhead caused by acquiring and releasing locks by minimizing the amount of locking required.
A read-write semaphore can be held by many readers but only one writer.
This significantly reduces the number of locks needed for various types of data.

ww_mutex
~~~~~~~~

``ww_mutex`` (Wound/Wait Mutex found on `include/linux/ww_mutex.h`_) are designed to handle complex synchronization scenarios, especially in GPU drivers where multiple buffers are shared across contexts, processes, and devices.
Traditional mutexes can lead to deadlocks due to unpredictable buffer access orders.
WW-Mutexes prevent deadlocks using a reservation ticket system, where the oldest task wins in case of contention.
Two algorithms, Wound-Wait and Wait-Die, manage conflicts, with Wound-Wait generally reducing rollbacks.
WW-Mutexes support flexible locking strategies, making them suitable for dynamic and unordered lock acquisition scenarios.


percpu_rw_semaphore
~~~~~~~~~~~~~~~~~~~

The ``percpu_rw_semaphore`` (defined in `include/linux/percpu-rwsem.h`_) is a modern read-write semaphore designed with optimization for read-heavy workloads.
Traditional read-write semaphores suffer from performance degradation when multiple cores acquire the lock for reading, as the cache line containing the semaphore bounces between the L1 caches of those cores.
With percpu_rw_semaphore, reading is extremely fast due to the use of RCU (`Read-Copy-Update`_), which eliminates the need for atomic instructions in the lock and unlock paths.
However, locking for writing is significantly more expensive, as it involves calling synchronize_rcu(), which can take hundreds of milliseconds.


CPU Local Locks
---------------

On non-PREEMPT_RT kernels, ``local_lock`` functions are wrappers around preemption and interrupt-disabling primitives.
Unlike other locking mechanisms, disabling preemption or interrupts is strictly a CPU-local concurrency control technique and is not suitable for managing inter-CPU concurrency.

local_lock_t
~~~~~~~~~~~~

``local_lock_t`` (defined in `include/linux/local_lock_internal.h`_) is a cpu lock implementation that is implemented in two different ways depending on whether the real-time kernel configuration (i.e., with ``PREEMPT_RT`` enabled) is used.

- In non-preemptive scenarios it is just a simple lock definition without any special fields.
- With ``PREEMPT_RT`` enabled, it maps to a per CPU :ref:`spinlock_t`, which protects the critical section while staying preemptible. 


References
----------

- `Lock types and their rules`_
- `Unreliable Guide To Locking - Rusty Russell`_

.. Links

.. _`subsystems`: https://docs.kernel.org/subsystem-apis.html
.. _`context switching`: https://en.wikipedia.org/wiki/Context_switch
.. _`race conditions`: https://en.wikipedia.org/wiki/Race_condition
.. _`kernel space`: https://en.wikipedia.org/wiki/User_space_and_kernel_space
.. _`Sleeping locks`: https://docs.kernel.org/locking/locktypes.html#sleeping-locks
.. _`CPU local locks`: https://docs.kernel.org/locking/locktypes.html#cpu-local-locks
.. _`Spinning locks`: https://docs.kernel.org/locking/locktypes.html#spinning-locks
.. _`SMP systems`: https://en.wikipedia.org/wiki/Symmetric_multiprocessing
.. _`preemption`: https://en.wikipedia.org/wiki/Preemption_(computing)
.. _`include/linux/spinlock.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h
.. _`include/linux/spinlock_types.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock_types.h
.. _`include/linux/spinlock_types_raw.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock_types_raw.h
.. _`include/linux/rwlock_types.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/rwlock_types.h#L34
.. _`include/linux/rwsem.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/rwsem.h#L48
.. _`include/linux/local_lock.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/local_lock.h#L16
.. _`include/linux/ww_mutex.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/ww_mutex.h#L47
.. _`include/linux/rtmutex.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/rtmutex.h#L57
.. _`include/linux/local_lock_internal.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/local_lock_internal.h#L16
.. _`include/linux/semaphore.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/semaphore.h#L15
.. _`include/linux/percpu-rwsem.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/percpu-rwsem.h#L12
.. _`include/linux/mutex_types.h`: https://github.com/torvalds/linux/blob/v6.8/include/linux/mutex_types.h#L41
.. _`software interrupts`: https://en.wikipedia.org/wiki/Interrupt#Software_interrupts
.. _`hardware interrupts`: https://en.wikipedia.org/wiki/Interrupt#Hardware_interrupts
.. _`spin_lock_irq()`: https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h#L374
.. _`spin_unlock_irqrestore()`: https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h#L404
.. _`tasklets`: https://lwn.net/Articles/960041
.. _spin_lock(): https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h#L349C29-L349C39
.. _spin_unlock(): https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h#L389
.. _spin_lock_bh(): https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h#L354
.. _spin_unlock_bh(): https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h#L394
.. _`spin_lock_irqsave()`: https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h#L379
.. _`spin_lock_irq()`: https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h#L374
.. _`spin_unlock_irq()`: https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h#L399
.. _`spin_unlock_irqrestore()`: https://github.com/torvalds/linux/blob/v6.8/include/linux/spinlock.h#L404
.. _`Unreliable Guide To Locking - Rusty Russell`: https://docs.kernel.org/kernel-hacking/locking.html
.. _`Lock types and their rules`: https://docs.kernel.org/locking/locktypes.html
.. _`system call`: https://en.wikipedia.org/wiki/System_call
.. _`completions`: https://docs.kernel.org/scheduler/completion.html
.. _`priority inversion`: https://en.wikipedia.org/wiki/Priority_inversion
.. _`The Linux Kernel Driver Interface`: https://docs.kernel.org/process/stable-api-nonsense.html
.. _`Read-Copy-Update`: https://docs.kernel.org/RCU/whatisRCU.html
