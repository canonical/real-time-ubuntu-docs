Install Real-time Ubuntu
========================

When you subscribe to Pro, you're provided with a management dashboard. You'll
need a token from it, to attach your subscription to the tutorial's target
computer. If no token is visible, click the Free Personal Token section and it
should appear.

Launch a shell and attach the token from your Pro dashboard to the target. Then
enable Real-time Ubuntu, which will select the appropriate version for your OS
and processor:

.. code-block:: shell-session

   sudo pro attach <token>
   sudo pro enable realtime-kernel
