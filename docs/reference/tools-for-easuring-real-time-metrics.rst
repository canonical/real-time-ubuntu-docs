Tools for Measuring Real-time Metrics
=====================================

Various tools are available for measuring real-time metrics.

Here are some tools along with example commands:

ps
---

The `ps`_ command reports a snapshot of the current processes.

.. code-block:: shell-session

   ps -A --format psr,tid,pid,comm,%cpu,priority,nice -T | sort -g | grep irq

This command monitors IRQs (Interrupt Requests).

perf
----

The `perf`_ stat command gathers performance counter statistics.

.. code-block:: shell-session

   perf stat -B dd if=/dev/zero of=/dev/null count=1000000

This command runs a command dd with the specified parameters and gathers performance statistics during its execution.


.. LINKS

.. _ps: https://www.man7.org/linux/man-pages/man1/ps.1.html
.. _perf: https://www.man7.org/linux/man-pages/man1/perf.1.html



