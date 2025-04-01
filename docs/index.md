# Real-time Ubuntu

<!--
TODO: The product docs' home-page must be based on a template. The template
requires the four-par intro below, which must be drawn from the Alignment
doc's *Common understanding* column. That column is arrived at by consensus
among several stakeholders. Until that content's available, the intro's been
drawn from the PM's contribution alone. It should be updated once a *Common
understanding* has been reached.
-->

[Real-time Ubuntu] is Ubuntu with a real-time kernel.

Ubuntu's real-time kernel includes the [PREEMPT_RT] patchset.
It changes Linux's default scheduler to a priority-based one, increasing
predictability by modifying the existing kernel code. Real-time Ubuntu is more
pre-emptive than mainline, delivering determinism and lower latency.

Stringent applications have mission-critical latency requirements and must
ensure high-priority processes are executed first, with deterministic response
times. Real-time Ubuntu provides an upper bound on execution time, to the most
demanding workloads in industrial, telco, automotive, and robotics
applications.

<!-- TODO: Clarify and tighten next par. -->

From assembly lines processing and delivering data in real-time to ensure
system integrity to critical telco infrastructure operating at low latency for
continuous production, the most exacting applications run on Real-time Ubuntu.

---------

## In this documentation

````{grid} 1 1 2 2

```{grid-item-card} [Tutorials](/tutorial/index)

**Start here**: a hands-on introduction to Real-time Ubuntu for new users.

- {doc}`/tutorial/first-rt-app/index`
- {doc}`/tutorial/intel-tcc/index`
```

```{grid-item-card} [How-to guides](/how-to/index)

**Step-by-step guides** covering key operations and common tasks, such as
{doc}`installing the real-time kernel </how-to/enable-real-time-ubuntu>`,
{doc}`configuring the CPUs </how-to/cpu-boot-configs>` and
{doc}`creating Ubuntu Core images </how-to/uc-image-creation>`.   
```

````

````{grid} 1 1 2 2
:reverse:

```{grid-item-card} [Reference](/reference/index)

**Technical information** about
{doc}`metric tools </reference/real-time-metrics-tools>`,
{doc}`boot parameters </reference/kernel-boot-parameters>` and
{doc}`supported releases </reference/releases>`.
```

```{grid-item-card} [Explanation](/explanation/index)

**Conceptual information** about real-time systems, such as {doc}`/explanation/schedulers` and {doc}`/explanation/priority-inversion-inheritance`.

```

````

---------

## Project and community

The {{PROJNAME_FULL}} project warmly welcomes community contributions,
suggestions, fixes and constructive feedback.
If you would like to contribute to the {{PROJNAME_FULL}}, see our
[Contributing guide].

Before raising a pull request, it's generally worth [opening an issue] to
discuss any proposed changes (unless it's trivial).
If you decide to raise a pull request, make sure you have signed the
{{C_CLA}} to grant us permission to use your contributions.

* {{U_COC}}

<!--
* Get support
* Join our online chat
* Contribute
* Roadmap
* Thinking about using Example Product for your next project? Get in touch!
-->

```{toctree}
:hidden:
:maxdepth: 1

/tutorial/index
/how-to/index
/reference/index
/explanation/index
/reference/releases
```

<!-- LINKS -->

[Real-time Ubuntu]: https://ubuntu.com/real-time
[PREEMPT_RT]: https://wiki.linuxfoundation.org/realtime/documentation/technical_details/start
[Contributing guide]: https://github.com/canonical/real-time-ubuntu-docs/blob/main/CONTRIBUTING.md
[opening an issue]: https://github.com/canonical/real-time-ubuntu-docs/issues
