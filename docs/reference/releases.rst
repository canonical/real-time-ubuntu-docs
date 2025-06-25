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
   * - Ubuntu 24.10
     - Oracular Oriole
     - 6.11
     - generic
   * - Ubuntu 25.04
     - Plucky Puffin
     - 6.14
     - generic

The LTS versions of Ubuntu support newer kernel versions. Those are available as Hardware Enablement (HWE) kernels.

Refer to :doc:`../how-to/enable-real-time-ubuntu` to set up a supported Ubuntu version.

Ubuntu Core
-----------

.. list-table:: 
   :widths: 50 50
   :header-rows: 1

   * - Version
     - Real-time Kernel Version
   * - Ubuntu Core 22
     - 5.15
   * - Ubuntu Core 24
     - 6.8

Newer kernel versions are available for `Ubuntu Core`_. Those are based on interim Ubuntu or Hardware Enablement (HWE) kernels. 

To install the real-time kernel on an instance of Ubuntu Core, refer to :doc:`../how-to/uc-install-real-time-kernel`.

To create an Ubuntu Core image with the real-time kernel, refer to :doc:`../how-to/uc-image-creation`.



.. _Ubuntu Core: https://ubuntu.com/core
