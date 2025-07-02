# How-to tune Real-Time Ubuntu using rt-conf

*rt-conf* is a tool that helps users tune their system for real-time responsiveness. 
This guide describes how to install and use it on Ubuntu. 

## Install

Install rt-conf as a snap:

```shell
sudo snap install rt-conf --beta --devmode
```

````{admonition} Developer mode
The snap must be installed with `--devmode` to allow IRQ configuration.
This is due to issue [#67](https://github.com/canonical/rt-conf/issues/67). 

When installed in developer mode, the snap will not update automatically.
To force an update:
```shell
sudo snap refresh rt-conf
```
````

## Configure

The default configuration file is located at `/var/snap/rt-conf/common/config.yaml`:

```{literalinclude} rt-conf-config.yaml
:language: yaml
```

The configuration file includes several examples.
Uncomment useful examples and modify them to meet the tuning requirements.

If a configuration is commented out, rt-conf will not perform any operations. 
Anything that is set on your system remains untouched.

Refer to the {doc}`../reference/rt-conf-yaml` for more details.


## Run

Once ready with the configurations, run rt-conf:
```shell
sudo rt-conf
```

```{admonition} Kernel command line parameters
Pay attention to the output as it shows platform-specific instructions to complete kernel command line configurations.

Setting kernel command line via rt-conf is not supported on Ubuntu Core. 
Instead, refer to {ref}`ubuntu-core-kernel-cmdline`.
```

The kernel command line parameters are persisted on the system, but this is not the case for other configurations.
The IRQ and CPU governance are reset to the system defaults after a reboot.
The rt-conf snap has a oneshot service which runs on every boot to re-apply the non-persistent configurations. 
This service runs only once and stops immediately after setting the configurations.

If re-applying non-persistent configurations on boot is not wanted, disable the service:
```shell
sudo snap stop --disable rt-conf
```

### Change configuration path

The configuration file path can be changed with `snap set`. For example:
```shell
sudo snap set rt-conf config-file=/home/ubuntu/rt-conf/config.yaml
```

```{note}
The configuration file must be in a location accessible to the snap, such as a user home directory.

The file must be owned by and writable to root only.
```

To revert to the default path:
```shell
sudo snap unset rt-conf config-file
```

### Verbose logging

To enable debug logging set:
```shell
sudo snap set rt-conf verbose=true
```
