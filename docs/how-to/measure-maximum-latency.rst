How to measure maximum latency in a real-time system 
====================================================

On `real-time`_ systems one of the most important metrics is the maximum latency. 
This is the maximum time it takes for a system to respond to an event. It's 
important because it can be used to determine if a system is capable of 
responding to an event in a timely manner. In other words, it can be used to
determine whether a system can meet the required deadlines.

The key tool here will be the `cyclictest`_ tool.

Install
--------

``cyclictest`` is part of the `rt-tests`_ package, which is a collection of 
programs for testing the real-time capabilities of a Linux system. Install it:

.. code-block:: bash

    sudo apt install rt-tests


Usage
------

``cyclictest`` must be run as root, with sudo, or as a member of the realtime 
group. Get the maximum latency of the system by running:

.. code-block:: console 
    
    $ sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0

    T: 0 (11712) P:80 I:200 C:  45810 Min:     12 Act:   27 Avg:   25 Max:     179
    T: 1 (11713) P:80 I:200 C:  45719 Min:     12 Act:   27 Avg:   25 Max:     175
    T: 2 (11714) P:80 I:200 C:  45647 Min:     12 Act:   52 Avg:   25 Max:      77
    T: 3 (11715) P:80 I:200 C:  45558 Min:     12 Act:   25 Avg:   26 Max:     162

By default the last command runs until it receives a SIGINT signal (Ctrl+C), or 
other signal that stops the process. To limit the number of iterations use the 
``--loops=`` (or ``-l``) option:

.. code-block:: console
    
    $ sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0 --loops=100000

This will run the test 100000 times.

The results are in microseconds(us) by default, but you can change it to nanoseconds 
with the ``--nsecs`` (or ``-N``) option:

.. code-block:: console
    
    $ sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0 --nsecs

    T: 0 (12182) P:80 I:200 C:  18815 Min:  12259 Act:   27891 Avg:   26352 Max:  203576
    T: 1 (12183) P:80 I:200 C:  18750 Min:  12716 Act:   27692 Avg:   27765 Max:  101304
    T: 2 (12184) P:80 I:200 C:  18663 Min:  13354 Act:   26741 Avg:   27091 Max:   87845
    T: 3 (12185) P:80 I:200 C:  18575 Min:  12681 Act:   25550 Avg:   26843 Max:  150537

The **maximum latency** is indicated by the **Max:** value in the output. 

As described on this `OSAL`_ page, `cyclictest`  can also output a 
a histogram of the latencies using the ``--histogram`` option,
which we can plot with `gnuplot` after some transformation:

Install the `gnuplot`_ package:

.. code-block:: bash

    sudo apt install gnuplot

Then, download the `histogram-plot.sh`_ script, make it executable and run it:

.. code-block:: bash

    sudo ./histogram-plot.sh

The result histogram will be saved in the file ``plot.png``.

.. Links
.. _real-time: https://ubuntu.com/real-time
.. _cyclictest: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cyclictest/start
.. _rt-tests: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/rt-tests
.. _OSAL: https://www.osadl.org/Create-a-latency-plot-from-cyclictest-hi.bash-script-for-latency-plot.0.html
.. _gnuplot: http://www.gnuplot.vt.edu/
.. _histogram-plot.sh: histogram-plot.sh