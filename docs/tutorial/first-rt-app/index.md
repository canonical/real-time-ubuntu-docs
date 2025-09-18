# Your first Real-time Ubuntu application

In this tutorial we will build a simple C program for a real-time workload.
We will tune the system's real-time parameters and observe the benefits Real-time Ubuntu provides.

## Prerequisites

Real-time Ubuntu integrates the upstream `PREEMPT_RT` patches, to improve the kernel's latency and responsiveness for real-time applications.
You'll be shown how to enable it, which will select the right version automatically for your system.

### Operating system and kernel

This tutorial requires a computer running one of the {doc}`/reference/releases`.
Ubuntu should be installed "native" --- i.e. directly on the computer's hardware, not in a Virtual Machine (VM).

### Hardware

Real-time Ubuntu employs features of modern 64-bit processors.
This tutorial is written for multi-core computers with the x86\_64 ("amd64") architecture, which are widespread.

### Sundry

Real-time Ubuntu is available through the [Ubuntu Pro](https://ubuntu.com/pro) security and compliance subscription.
If you don't already have access to "Pro", it's free for personal use.

The tutorial uses code examples to illustrate key concepts.
You'll need a C toolchain and familiarity with the language.
Sample code was written for gcc and tested on the supported Ubuntu releases.

An Internet connection is assumed.

Many operations will require administrator ("root") privileges.
You'll be shown clearly when they're necessary, usually by the `sudo` prefix to a command.

## Install Real-time Ubuntu

Refer to {doc}`/how-to/enable-real-time-ubuntu` guide.

## Pick low-hanging fruit

Merely enabling Real-time Ubuntu doesn't optimize the kernel for your application.
Tuning may be helpful, for example to reduce cycle-times and other latencies.
Real-time Ubuntu offers various configuration options and boot parameters for the purpose.

Treat tuning techniques as a toolkit, to choose from according to the demands of your application and the environment it runs in.
At each step, apply the most important and readily available improvement.
An early candidate is the kernel's task scheduler.

The following sections provide guidance on key areas you can focus on when tuning for real-time performance and finally explain how to snap your real-time app:

```{toctree}
:maxdepth: 1

1-schedulers
2-cpu-affinity
3-locks
4-snapping
```
