.. meta::
   :description: Consult the list of supported Real-time Ubuntu releases and supported kernel variants for Ubuntu Server, Desktop, and Core.

Real-time Ubuntu releases
=========================

Ubuntu Server / Desktop
-----------------------

.. list-table:: 
   :widths: 25 25 25 50
   :header-rows: 1

   * - Version
     - Code name
     - Real-time Kernel Version
     - Variants
   * - Ubuntu 22.04 LTS
     - Jammy Jellyfish 
     - 5.15
     - generic, intel-iotg
   * - Ubuntu 24.04 LTS
     - Noble Numbat
     - 6.8
     - generic, raspi
   * - Ubuntu 25.04
     - Plucky Puffin
     - 6.14
     - generic
   * - Ubuntu 25.10
     - Questing Quokka
     - 6.17
     - generic

.. note::

  The LTS releases of Ubuntu support newer kernel versions. Those are available as Hardware Enablement (HWE) kernels; see `Ubuntu kernel lifecycle`_.

Refer to :doc:`../how-to/enable-real-time-ubuntu` to set up a supported Ubuntu version.

Ubuntu Core
-----------

.. list-table:: 
   :widths: 50 50
   :header-rows: 1

   * - Version
     - Real-time Kernel Version
   * - Ubuntu Core 22
     - 5.15; 6.8 (HWE)
   * - Ubuntu Core 24
     - 6.8; 6.14 (HWE)

.. note::

  Newer kernels (6.8, 6.14, etc) can be installed on Ubuntu Core via Hardware Enablement (HWE) kernel snaps.
  The list of publicly available kernel snaps can be found in the `pc-kernel`_ Snap Store page.

To install the real-time kernel on an instance of Ubuntu Core, refer to :doc:`../how-to/uc-install-real-time-kernel`.

To create an Ubuntu Core image with the real-time kernel, refer to :doc:`../how-to/uc-image-creation`.


.. _Ubuntu kernel lifecycle: https://ubuntu.com/kernel/lifecycle
.. _pc-kernel: https://snapcraft.io/pc-kernel
