# How-to use RT-conf command line interface

RT-conf is an application that helps users tune their system for real-time responsiveness.  
This guide provides usage instructions for its command-line interface.

## Running in Terminal UI mode

The `rt-conf` also provides a [terminal UI interface][tui_wikipedia], for iterative configuration.
To be activated, it's only necessary to run the `rt-conf` tool, without any flags or arguments:
```shell
rt-conf
```

## Getting help

To print help information regarding the usage info, use `-h` or `--help` flag.

```shell
rt-conf --help
```

## Defining configuration file

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

- Overriding the `DEFAULT_CONF` environment variable
```shell
export DEFAULT_CONF=~/<custom-config>.yaml

rt-conf --conf
```

## Debug mode

To enable debug logging set the `DEBUG` environment variable to `true`.

```shell
export DEBUG=true
```

% Links
[snap_env_variables]: https://snapcraft.io/docs/environment-variables
[tui_wikipedia]: https://en.wikipedia.org/wiki/Text-based_user_interface
