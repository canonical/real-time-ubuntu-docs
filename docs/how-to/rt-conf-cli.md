# How-to tune Real-Time Ubuntu using rt-conf

RT-conf is an application that helps users tune their system for real-time responsiveness.  
This guide provides usage instructions for its command-line interface.

## Install `rt-conf` snap

The `rt-conf` application is available as a snap and can be installed with:

```shell
sudo snap install rt-conf
```
<!-- TODO: Complement this information once the tool is in a stable state -->
The Snap is supported on `amd64` systems using the [GRUB] bootloader.
It works on both Ubuntu Classic ([Ubuntu Desktop][ubuntu_desktop] and [Ubuntu Server][ubuntu_server]) and [Ubuntu Core][uc].

## Run in Terminal UI mode

The `rt-conf` also provides a [terminal UI interface][tui_wikipedia], for iterative configuration.
To be activated, it's only necessary to run the `rt-conf` tool, without any flags or arguments:
```shell
rt-conf
```

## Get help

To print help information regarding the usage info, use `-h` or `--help` flag.

```shell
rt-conf --help
```

## Define configuration file

There are three ways to define the YAML configuration file:

- Relying on the defined by `DEFAULT_CONF` environment variable.
  Which defaults to `$SNAP_COMMON/config.yaml`. 
  Checkout [snap environment variables documentation][snap_env_variables].

```shell
rt-conf --conf
```

- Pass using the `--conf` flag:
```shell
rt-conf --conf=<custom-config>.yaml
```


## Debug mode

To enable debug logging set the `DEBUG` environment variable to `true`. For example:

```shell
DEBUG=true rt-conf
```

% Links
[snap_env_variables]: https://snapcraft.io/docs/environment-variables
[tui_wikipedia]: https://en.wikipedia.org/wiki/Text-based_user_interface
[GRUB]: https://www.gnu.org/software/grub/
[ubuntu_desktop]: https://ubuntu.com/download/desktop
[ubuntu_server]: https://ubuntu.com/download/server
[uc]: https://ubuntu.com/core
