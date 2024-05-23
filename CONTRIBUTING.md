# Contributing to Real-time Ubuntu docs

The [Real-time Ubuntu documentation] lives in the `docs/` directory and is deployed on [Read the Docs].
It is written in [reStructuredText] and built with [Sphinx].

The documentation should follow the [Canonical documentation style guide] in use for documentation at Canonical.

## Fork and download the docs  

If you are working with this documentation set for the first time, you'll need to create a fork of this repository on your GitHub account, and clone the forked repository.

## Install dependencies

Go to the `docs/` directory and run:

```shell
make install
```

This will create a virtual environment with all the tooling dedicated to validate and render the documentation.
It will also install the [woke snap].

## Build and serve the docs

To get a live preview and validation of the documentation, run:

```shell
make run
```

This will provide a locally rendered version of the documentation at: http://localhost:8000

The rendered preview will get updated every time a document is modified.

## Validate the docs

You can validate your changes with the following recipes:

```shell
make spelling  
make linkcheck 
make woke      # to check for non-inclusive language
```

## Propose changes
Submit your documentation changes via a pull request on Github.


[Real-time Ubuntu documentation]: https://canonical-real-time-ubuntu-documentation.readthedocs-hosted.com
[Read the Docs]: https://www.readthedocs.com
[reStructuredText]: https://docutils.sourceforge.io/rst.html
[Sphinx]: https://www.sphinx-doc.org
[Canonical documentation style guide]: https://docs.ubuntu.com/styleguide/en
[woke snap]: https://snapcraft.io/woke
