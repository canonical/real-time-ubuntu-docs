# How-to tune Real-Time Ubuntu using rt-conf

rt-conf is an application that helps users tune their system for real-time responsiveness.  

## Install

Install rt-conf as a snap:

```shell
sudo snap install rt-conf --beta --devmode
```

````{admonition} Developer mode
The snap must be installed with `--devmode` to allow IRQ configuration.
This is due to issue [#67](https://github.com/canonical/rt-conf/issues/67). 

When installed in developer mode, the snap will not update automatically.
For force an update:
```shell
sudo snap refresh rt-conf
```
````

## Configure

The default configuration file is located at `/var/snap/rt-conf/config.yaml`:

```{literalinclude} rt-conf-config.yaml
:language: yaml
```

The configuration file includes several examples.
If a configuration is commented out, rt-conf will not perform any operations. 
Hence, anything that is set on your system remains untouched.

Uncomment useful examples and modify them to meet the tuning requirements.

Refer to the {doc}`../reference/rt-conf-yaml` for more details.


## Run

Once ready with the configurations, run rt-conf:
```shell
sudo rt-conf
```

```{important}
Pay attention to the output as it shows platform-specific instructions to complete kernel command line configurations.
```

The kernel command lines are persisted on the system, but this is not the case for other configurations.
The IRQ and CPU governance are reset to the system defaults after a reboot.

To automatically set non-persistent configurations on system startup, enable {spellexception}`rt-conf's` oneshot service:
```shell
sudo snap start --enable rt-conf
```

With the above command, rt-conf applies the configuration once and again on every boot.
The service stops after applying the configurations.

### Change configuration path

The configuration file path can be changed with `snap set`. For example:
```shell
sudo snap set rt-conf config-file=/home/ubuntu/rt-conf/config.yaml
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
