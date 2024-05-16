# Contributing to Real-time Ubuntu

The [Real-time Ubuntu documentation] lives in the `docs/` directory and is deployed on Read the Docs.
It is written using the [reStructuredText] format and built using [Sphinx].

The documentation should follow the [style guide] in use for documentation at Canonical.

To install everything you need, go to the `docs/` directory and type:

```shell
make install
```

This will create a virtual environment with all the tooling dedicated to validate and render the documentation.

To get a live preview and validation of the documentation, you can then run:

```shell
make run
```

This will provide a link to a locally rendered version of the documentation that will be updated every time a file is modified.

You can validate your changes with the following recipes:

```shell
make spelling  
make linkcheck 
make woke      # to check for non-inclusive language
```

Submit your documentation changes via a pull request.


[Real-time Ubuntu documentation]: https://canonical-real-time-ubuntu-documentation.readthedocs-hosted.com
[Read the Docs]: https://www.readthedocs.com
[reStructuredText]: https://docutils.sourceforge.io/rst.html
[Sphinx]: https://www.sphinx-doc.org
[style guide]: https://docs.ubuntu.com/styleguide/en
