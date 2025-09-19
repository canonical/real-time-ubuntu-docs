# How to enable Real-time Ubuntu

## Ubuntu Classic

To enable Real-time Ubuntu on a supported LTS release, follow the instructions in the {doc}`Ubuntu Pro Client - How to enable Real-time Ubuntu <ubu-pro-client:howtoguides/enable_realtime_kernel>` guide.

If you are running an interim release, Real-time Ubuntu can instead be installed from the universe apt repository:

```shell
sudo add-apt-repository universe
sudo apt update
sudo apt install ubuntu-realtime
```

Refer to {doc}`/reference/releases` and [Ubuntu release](https://ubuntu.com/about/release-cycle) documentation for more information about Ubuntu releases and which support real-time.

## Ubuntu Core

To install the real-time kernel on an instance of [Ubuntu Core](https://ubuntu.com/core), refer to {doc}`/how-to/uc-install-real-time-kernel`.

To create an Ubuntu Core image with the real-time kernel, refer to {doc}`/how-to/uc-install-real-time-kernel`.
