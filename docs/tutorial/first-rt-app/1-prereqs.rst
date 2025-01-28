Prerequisites
=============

Real-time Ubuntu integrates the upstream ``PREEMPT_RT`` patches, to improve the
kernel's latency and responsiveness for real-time applications. You'll be shown
how to enable it, which will select the right version automatically for your
system.

Operating system and kernel
---------------------------

This tutorial requires a computer running one of the :doc:`/reference/releases`.
Ubuntu should be installed "native" --- i.e. directly on the computer's hardware, not in a Virtual Machine (VM).

Hardware
--------

Real-time Ubuntu employs features of modern 64-bit processors. This tutorial is
written for multi-core computers with the x86_64 ("amd64") architecture, which
are widespread.


Sundry
------

Real-time Ubuntu is available through the `Ubuntu Pro`_ security and compliance
subscription. If you don't already have access to "Pro", it's free for personal
use.

The tutorial uses code examples to illustrate key concepts. You'll need a C
toolchain and familiarity with the language. Sample code was written for gcc
and tested on the supported Ubuntu releases.

An Internet connection is assumed.

Many operations will require administrator ("root") privileges. You'll be shown
clearly when they're necessary, usually by the ``sudo`` prefix to a command.


.. LINKS
.. _Ubuntu Pro: https://ubuntu.com/pro
