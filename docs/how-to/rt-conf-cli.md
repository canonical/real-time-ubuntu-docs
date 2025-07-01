# How-to tune Real-Time Ubuntu using rt-conf

rt-conf is an application that helps users tune their system for real-time responsiveness.  
This guide provides usage instructions for its command-line interface.

## Install

The `rt-conf` application is available as a snap and can be installed with:

```shell
sudo snap install rt-conf --beta
```

````{important}
To configure IRQs, the snap must be installed in developer mode:
```shell
sudo snap install rt-conf --beta --devmode
```

This is due to issue [#67](https://github.com/canonical/rt-conf/issues/67). 

````
<!-- TODO: Complement information with supported systems once the tool is in a stable state -->

## Configure

An example configuration file is located at `/var/snap/rt-conf/config.yaml`:

```{literalinclude} rt-conf-config.yaml
:language: yaml
```

The configuration sections include only commented out examples.
Because of that, rt-conf will not perform any operations inadvertently.

Uncomment useful examples and extend them to meet the tuning requirements.

````{tip}
The configuration file path can be changed with `snap set`. For example:
```shell
sudo snap set rt-conf config-file=/home/ubuntu/rt-conf/config.yaml
```
````

## Run

Once ready with the configurations, run rt-conf:
```shell
rt-conf
```

Pay attention to the output as it shows platform-specific instructions to complete certain configurations.

```{tip}
Kernel command line configurations require manual actions to become effective.

On systems with GRUB bootloader, the changes made by rt-conf are only effective after manually running `update-grub`.

On all systems, a reboot is required.
```


### Verbose logging

To enable debug logging set:
```shell
sudo snap set rt-conf verbose=true
```
