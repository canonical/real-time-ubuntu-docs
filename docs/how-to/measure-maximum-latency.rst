.. meta::
   :description: Discover how to stress-test your system and measure the maximum latency using standard tools like cyclictest on Real-time Ubuntu.

How to measure maximum latency in a real-time system 
====================================================

On `real-time`_ systems one of the most important metrics is the maximum latency. 
This is the maximum time it takes for a system to respond to an event. It's 
important because it can be used to determine if a system is capable of 
responding to an event in a timely manner. In other words, it can be used to
determine whether a system can meet the required deadlines.

The key tool here will be the `cyclictest`_ tool.

Install
-------

``cyclictest`` is part of the `rt-tests`_ package, which is a collection of 
programs for testing the real-time capabilities of a Linux system. Install it:

.. code-block:: shell

    sudo apt install rt-tests


Usage
-----

``cyclictest`` must be run as root, with sudo, or as a member of the realtime 
group. Get the maximum latency of the system by running:


.. code-block:: shell 
    
    sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0

.. tabs::

    .. group-tab:: Default Ubuntu kernel

        .. code-block:: text

            # /dev/cpu_dma_latency set to 0us
            policy: fifo: loadavg: 18.66 12.77 5.80 20/410 17196          

            T: 0 (17176) P:80 I:200 C:1499941 Min:   1 Act:    4 Avg:    3 Max:     129
            T: 1 (17177) P:80 I:200 C:1499981 Min:   1 Act:    3 Avg:    3 Max:      99
            T: 2 (17178) P:80 I:200 C:1499981 Min:   1 Act:    3 Avg:    3 Max:      97
            T: 3 (17179) P:80 I:200 C:1499901 Min:   1 Act:    3 Avg:    3 Max:      67
            T: 4 (17180) P:80 I:200 C:1499961 Min:   1 Act:    2 Avg:    3 Max:      99
            T: 5 (17181) P:80 I:200 C:1499941 Min:   1 Act:    3 Avg:    3 Max:     125
            T: 6 (17182) P:80 I:200 C:1499921 Min:   1 Act:    4 Avg:    3 Max:      92
            T: 7 (17183) P:80 I:200 C:1499941 Min:   1 Act:    3 Avg:    3 Max:     108
            T: 8 (17184) P:80 I:200 C:1499861 Min:   1 Act:    6 Avg:    3 Max:      26
            T: 9 (17185) P:80 I:200 C:1499861 Min:   1 Act:    6 Avg:    3 Max:      79
            T:10 (17186) P:80 I:200 C:1499801 Min:   1 Act:    2 Avg:    3 Max:     103
            T:11 (17187) P:80 I:200 C:1499881 Min:   1 Act:    3 Avg:    3 Max:     102
            T:12 (17188) P:80 I:200 C:1499901 Min:   1 Act:    6 Avg:    3 Max:      87
            T:13 (17189) P:80 I:200 C:1499882 Min:   1 Act:    3 Avg:    3 Max:      97
            T:14 (17190) P:80 I:200 C:1499861 Min:   1 Act:    2 Avg:    3 Max:      30
            T:15 (17191) P:80 I:200 C:1499853 Min:   1 Act:    3 Avg:    3 Max:      38
            T:16 (17192) P:80 I:200 C:1499782 Min:   1 Act:    3 Avg:    3 Max:      94
            T:17 (17193) P:80 I:200 C:1499782 Min:   1 Act:    2 Avg:    3 Max:      94
            T:18 (17194) P:80 I:200 C:1499845 Min:   1 Act:    3 Avg:    3 Max:      26
            T:19 (17195) P:80 I:200 C:1499842 Min:   1 Act:    4 Avg:    4 Max:     102

    .. group-tab:: Real-time Ubuntu kernel

        .. code-block:: text

            # /dev/cpu_dma_latency set to 0us
            policy: fifo: loadavg: 28.19 18.83 8.56 21/516 2798          

            T: 0 ( 2720) P:80 I:200 C:1477815 Min:   1 Act:    4 Avg:    3 Max:      22
            T: 1 ( 2721) P:80 I:200 C:1477816 Min:   1 Act:    3 Avg:    3 Max:      22
            T: 2 ( 2722) P:80 I:200 C:1477576 Min:   1 Act:    4 Avg:    3 Max:      23
            T: 3 ( 2723) P:80 I:200 C:1477556 Min:   1 Act:    4 Avg:    3 Max:      23
            T: 4 ( 2724) P:80 I:200 C:1477556 Min:   1 Act:    4 Avg:    3 Max:      23
            T: 5 ( 2725) P:80 I:200 C:1477536 Min:   2 Act:    3 Avg:    2 Max:      25
            T: 6 ( 2726) P:80 I:200 C:1477476 Min:   1 Act:    4 Avg:    2 Max:      24
            T: 7 ( 2727) P:80 I:200 C:1477536 Min:   1 Act:    3 Avg:    3 Max:      24
            T: 8 ( 2728) P:80 I:200 C:1477376 Min:   2 Act:    4 Avg:    3 Max:      34
            T: 9 ( 2729) P:80 I:200 C:1477416 Min:   1 Act:    3 Avg:    3 Max:      32
            T:10 ( 2730) P:80 I:200 C:1477256 Min:   1 Act:    4 Avg:    3 Max:      22
            T:11 ( 2731) P:80 I:200 C:1477156 Min:   1 Act:    4 Avg:    3 Max:      24
            T:12 ( 2732) P:80 I:200 C:1477036 Min:   1 Act:    4 Avg:    3 Max:      24
            T:13 ( 2733) P:80 I:200 C:1477016 Min:   1 Act:    4 Avg:    3 Max:      23
            T:14 ( 2734) P:80 I:200 C:1477056 Min:   1 Act:    4 Avg:    3 Max:      26
            T:15 ( 2735) P:80 I:200 C:1499181 Min:   1 Act:    3 Avg:    3 Max:      25
            T:16 ( 2736) P:80 I:200 C:1499141 Min:   1 Act:    4 Avg:    3 Max:      39
            T:17 ( 2737) P:80 I:200 C:1499081 Min:   1 Act:    3 Avg:    3 Max:      30
            T:18 ( 2738) P:80 I:200 C:1499061 Min:   2 Act:    3 Avg:    2 Max:      24
            T:19 ( 2739) P:80 I:200 C:1499061 Min:   1 Act:    4 Avg:    3 Max:      27

.. NOTE: Using outputs of the test: https://warthogs.atlassian.net/browse/IENG-907?focusedCommentId=365824
.. To be changed later to have consistent results and histogram plot

The **maximum latency** is indicated by the **Max** value in the output, per
CPU core. When using the real-time kernel on the same machine, the maximum
latency becomes remarkably smaller and more deterministic.

By default the last command runs until it receives a SIGINT signal (Ctrl+C), or 
other signal that stops the process. To limit the number of iterations use the 
``--loops=`` (or ``-l``) option:

.. code-block:: shell
    
    sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0 --loops=100000

This will run the test 100000 times.

Sometimes to have a comparison between different runs, it is useful to use the
``--duration=`` (or ``-D``) option appending 'm', 'h', or 'd' to specify 
minutes, hours or days.

.. code-block:: shell
    
    sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0 --duration=10m

This will run the test for 10 minutes.

The results are in microseconds(us) by default, but you can change it to nanoseconds 
with the ``--nsecs`` (or ``-N``) option:

.. code-block:: shell
    
    sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0 --nsecs

As described on this `OSAL`_ page, `cyclictest`  can also output a histogram of 
the latencies using the ``--histogram`` (or ``-h``) option, which we can plot 
with `gnuplot` after some transformation.

Install the `gnuplot`_ package:

.. code-block:: shell

    sudo apt install gnuplot

Then, download the :download:`histogram-plot.sh` script, make it executable and run it:

.. code-block:: shell

    sudo ./histogram-plot.sh

This script executes the ``cyclictest`` with specific parameters, such as 
``-h400`` (for the histogram) and ``-D1m`` (for the testing duration), then uses `gnuplot` to plot the 
histogram. The resulted histogram will be saved in the file ``plot.png``.

The resulting image should look like this:

.. tabs::

    .. group-tab:: Default Ubuntu kernel

        .. image:: default-kernel-plot.png
           :width: 80%
           :align: center
           :alt: latencies histogram plot - default kernel

    .. group-tab:: Real-time Ubuntu kernel

        .. image:: rt-kernel-plot.png
            :width: 80%
            :align: center
            :alt: latencies histogram plot - real-time kernel

The x-axis was adjusted for better visualization.

.. note:: 
    To get more accurate results on latency measurements, it is recommended to
    run the test on a system with the same or very similar load that is expected in production.
    This could be done by using :manpage:`stress-ng(1)`, so first, install it:

    .. code-block:: shell

        sudo apt install stress-ng

    Then run it with the desired parameters. For example:

    .. code-block:: shell

        sudo stress-ng --cyclic 1 --cyclic-dist 250 --cyclic-method clock_ns --cyclic-policy rr -t 3600 --log-file cyclic-stress.log --verbose
    
    Note that the above command will run the stress-ng for 1 hour, and will log
    the results in the file ``cyclic-stress.log``. The ``--cyclic-dist`` option
    specifies the distribution of the sleep times, and the ``--cyclic-method``
    option specifies the method to use for the cyclic stress. The ``--cyclic-policy``
    option specifies the scheduling policy to use for the cyclic stress.
    Note that this options are just an example, and can be changed to fit the
    specific needs of each system.

.. Links
.. _real-time: https://ubuntu.com/real-time
.. _cyclictest: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cyclictest/start
.. _rt-tests: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/rt-tests
.. _OSAL: https://www.osadl.org/Create-a-latency-plot-from-cyclictest-hi.bash-script-for-latency-plot.0.html
.. _gnuplot: http://www.gnuplot.info/

.. Tests performed in testflinger machine: 
.. queue 202008-28173
.. description:
.. 	HP Z2 Tower G5 Workstation - Islands, i9-10900K CPU
.. Characteristics: 
..          OS: Ubuntu 22.04.4 LTS
..          CPU: i9-10900K
..          RAM: 64GB

.. The results are generated based on the system load:
.. for ((i=0; i<$(nproc); i++)); do sudo taskset -c $i /bin/ping -l 65535 -q -s 10 -f localhost & done
