Real-time Ubuntu
================

.. TODO: The product docs' home-page must be based on a template. The template
   requires the four-par intro below, which must be drawn from the Alignment
   doc's *Common understanding* column. That column is arrived at by consensus
   among several stakeholders. Until that content's available, the intro's been
   drawn from the PM's contribution alone. It should be updated once a *Common
   understanding* has been reached.

`Real-time Ubuntu`_ is Ubuntu with a real-time kernel.

Ubuntu's real-time kernel includes the `PREEMPT_RT`_ patchset.
It changes Linux's default scheduler to a priority-based one, increasing
predictability by modifying the existing kernel code. Real-time Ubuntu is more
pre-emptive than mainline, delivering determinism and lower latency.

Stringent applications have mission-critical latency requirements and must
ensure high-priority processes are executed first, with deterministic response
times. Real-time Ubuntu provides an upper bound on execution time, to the most
demanding workloads in industrial, telco, automotive, and robotics
applications.

.. TODO: Clarify and tighten next par.

From assembly lines processing and delivering data in real-time to ensure
system integrity to critical telco infrastructure operating at low latency for
continuous production, the most exacting applications run on Real-time Ubuntu.

---------

In this documentation
---------------------

.. grid:: 1 1 2 2

   .. grid-item:: :doc:`Tutorial <tutorial/index>`

      **Start here**: a hands-on introduction to Real-time Ubuntu for new users.

   .. grid-item:: :doc:`How-to <how-to/index>`

      **Step-by-step guides** covering key operations and common tasks,
      including :doc:`installation <how-to/enable-real-time-ubuntu>` and
      :doc:`configuration <how-to/cpu-boot-configs>`.

.. grid:: 1 1 2 2
   :reverse:

   .. grid-item:: :doc:`Reference <reference/index>`

      **Technical information** about
      :doc:`metric tools <reference/real-time-metrics-tools>`,
      :doc:`boot parameters <reference/kernel-boot-parameters>` and
      :doc:`supported releases <reference/releases>`.

   .. grid-item:: :doc:`Explanation <explanation/schedulers>`

      **Conceptual information** about :doc:`explanation/schedulers`.

.. toctree::
   :maxdepth: 1
   :hidden:

   tutorial/index
   how-to/index
   reference/index
   Explanation: Linux kernel schedules <explanation/schedulers>

---------

Project and community
---------------------

.. TODO: RTU and other kernel projects are unusual in storing their docs
   separately from their code. At this stage there's no public RTU code repo
   for a community to contribute to — there's only the docs repo. For now,
   therefore, all references to contributing are aimed at the RTU docs repo.
   They should be updated when community contributions to the RTU code can be
   accepted.

The Real-time Ubuntu project welcomes community contributions, suggestions,
fixes and constructive feedback. If you wish to contribute to the Real-time
Ubuntu docs, consult the `CONTRIBUTING guide`_.

Before raising a pull request, it's generally worth `opening an issue`_ to
discuss any proposed change unless it's trivial. Then, if you decide to raise a
pull request, be sure you've signed the `Canonical contributor agreement`_ —
it's the easiest way to grant us permission to use your contributions.

*  Our `Code of Conduct`_

..  *  `Get support`_
    *  Join our `online chat`_
    *  Browse the `product roadmap`_
    *  Thinking about using Real-time Ubuntu for your next project? `Get in touch!`_


.. LINKS
.. _Real-time Ubuntu: https://ubuntu.com/real-time
.. _PREEMPT_RT: https://wiki.linuxfoundation.org/realtime/documentation/technical_details/start
.. _CONTRIBUTING guide: https://github.com/canonical/real-time-ubuntu-docs/blob/main/CONTRIBUTING.md
.. _opening an issue: https://github.com/canonical/real-time-ubuntu-docs/issues
.. _Canonical contributor agreement: https://ubuntu.com/legal/contributors
.. _Code of Conduct: https://ubuntu.com/community/ethos/code-of-conduct
..  _Get support:
..  _online chat:
..  _product roadmap:
..  _Get in touch!:
