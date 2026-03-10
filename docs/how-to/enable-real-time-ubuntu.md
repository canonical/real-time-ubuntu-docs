---
myst:
  html_meta:
    description: Enable and install the real-time kernel on your system, with guidance for different Ubuntu editions.
---

# How to enable Real-time Ubuntu

The approach to enable and install Real-time Ubuntu differs according to your Ubuntu edition.
Follow the instructions that match your edition to ensure a correct installation.

## Ubuntu Server / Desktop

`````{tabs}

````{group-tab} Ubuntu 24.04 LTS (Noble Numbat) or earlier
Follow the instructions in the {doc}`Ubuntu Pro Client - How to enable Real-time Ubuntu <ubu-pro-client:howtoguides/enable_realtime_kernel>` guide.
````

````{group-tab} Ubuntu 25.10 (Questing Quokka)
Real-time Ubuntu can instead be installed from the universe apt repository:

```shell
sudo add-apt-repository universe
sudo apt update
sudo apt install ubuntu-realtime
```
````

````{group-tab} Ubuntu 26.04 LTS (Resolute Raccoon)

```{note}
This will be available in upcoming Ubuntu 26.04 LTS (Resolute Raccoon) release.
```
Real-time Ubuntu can instead be installed from the universe apt repository:

```shell
sudo add-apt-repository universe
sudo apt update
sudo apt install ubuntu-realtime
```
````

`````

Refer to {doc}`/reference/releases` and [Ubuntu release](https://ubuntu.com/about/release-cycle) documentation for more information about Ubuntu releases and which support real-time.

## Ubuntu Core

To install the real-time kernel on an instance of [Ubuntu Core](https://ubuntu.com/core), refer to {doc}`/how-to/uc-install-real-time-kernel`.

To create an Ubuntu Core image with the real-time kernel, refer to {doc}`/how-to/uc-install-real-time-kernel`.
