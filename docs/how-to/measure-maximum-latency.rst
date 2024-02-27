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

``cyclictest`` must be run as root, with sudo, or as a member of the realtime group.
Get the maximum latency of the system by running the following command:

.. code-block:: bash
    
    sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0

To limit the number of iterations use the ``--loops=`` (or ``-l``) option:

.. code-block:: bash
    
    sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0 --loops=100000

This will run the test 100000 times.

By default the results are in microseconds(us), but you can change it to 
nanoseconds with the ``--nsecs``(or ``-N``) option:

.. code-block:: bash
    
    sudo cyclictest --mlockall --smp --priority=80 --interval=200 --distance=0 --nsecs

The **maximum latency** is indicated by the **Max:** value in the output. 

It is also possible to plot a histogram of the latencies using 
the ``--histogram`` (or ``-h``), as described on this `OSAL`_ page.


.. code-block:: bash

    wget https://www.osadl.org/uploads/media/mklatencyplot.bash
    chmod +x mklatencyplot.bash

    # Reduce number of iterations
    sed -i 's/-l100000000/-l100000/g' mklatencyplot.bash

    # Get number of cores from the system
    sed -i 's/cores=4/cores=$(nproc)/g' mklatencyplot.bash

    # Run the script
    sudo ./mklatencyplot.bash 

Then the result histogram will be saved in the file ``plot.png``.


.. Links and References session, not visible in the rendered page

.. _real-time: https://ubuntu.com/real-time
.. _cyclictest: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/cyclictest/start
.. _rt-tests: https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/rt-tests
.. _OSAL: https://www.osadl.org/Create-a-latency-plot-from-cyclictest-hi.bash-script-for-latency-plot.0.html?&no_cache=1&sword_list[0]=script