# Contributing to Real-time Ubuntu documentation

We believe that everyone has something valuable to contribute, whether you're a
coder, a writer, or a tester.

The guidelines below will help keep your contributions effective and meaningful.

## Code of conduct

When contributing, you must abide by the [Ubuntu Code of Conduct].

## License and copyright

Unless explicitly stated in the license headers of source files, all
contributions to Real-time Ubuntu documentation are licensed under the Creative
Commons Attribution-Share Alike (CC-BY-SA) 3.0 Unported License.
To view a copy of this license, visit
http://creativecommons.org/licenses/by-sa/3.0/ or send a letter to Creative
Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

All contributors must sign the [Canonical contributor license agreement],
which grants Canonical permission to use the contributions. The author of a
change remains the copyright owner of their code (no copyright assignment
occurs).

## Releases and versions

Real-time Ubuntu releases align with Ubuntu version updates, with major releases
occurring twice annually.
Find the list of releases here: [Real-time Ubuntu release].

## Environment setup

<!-- TODO: Update with your prerequisites or drop if excessive -->

Real-time Ubuntu documentation is built on top of [Canonical's Sphinx starter
pack] and hosted on [Read the Docs].
It is written in reStructuredText or MyST Markdown and built with [Sphinx].

- For general guidance, refer to the [starter pack guide]
- For syntax help and guidelines, refer to the [Canonical style guides]
- In structuring, the documentation employs the [Diátaxis] approach

### Fork and download the documentation repository

If you are working with this documentation set for the first time, you'll need
to create a fork of this repository on your GitHub account, and clone the
forked repository.

### Install dependencies

Go into the `docs/` directory and run:

```
make install
```

This will create a virtual environment with all the tooling and packages
required to validate and render the documentation.

### Build and serve the docs

To get a live preview and validation of the documentation, run:

```
make run
```

This will provide a locally rendered version of the documentation at:
<http://localhost:8000>

The rendered preview will get updated every time a document is modified and
saved.

### Validate the docs

Before you submit a PR, make sure to validate your changes with the following
recipes:

```
make spelling  
make linkcheck  
make woke       # to check for non-inclusive language
```

For more information about the inclusive language check, see [woke].

## Propose changes

Submit your documentation changes via a pull request on GitHub.

Your changes will be reviewed in due time; if approved, they will eventually be
merged.

### Describing pull requests

<!-- TODO: Update with your own checklist or drop if excessive -->

To be properly considered, reviewed, and merged, your pull request must provide
the following details:

- **Title**: Summarise the change in a short, descriptive title.
- **Description**: Explain the problem that your pull request solves. Mention
any new features, bug fixes, or refactoring.
- **Relevant issues**: Reference any [related issues, pull requests, and
repositories].

### Commit structure and messages

Use separate commits for each logical change, and for changes to different
sections in the Real-time Ubuntu documentation.
Prefix your commit messages with the names of sections or pages that they
affect, using the code tree structure. For example, start a commit that updates
the reference page kernel boot parameters with `ref/kernel-boot:`.

Use [conventional commits] to ensure consistency across the project:

```none
docs(ref/kernel-boot): Add example for nohz parameter

* Additional description goes here
```

Such structure makes it easier to review contributions and simplifies porting
fixes to other branches.

### Sign off on commits

To improve contribution tracking, we use the developer certificate of origin
([DCO 1.1](https://developercertificate.org/)) and require a "sign-off" for any
changes going into each branch.

The sign-off is a simple line at the end of the commit message certifying that
you wrote it or have the right to commit it as an open-source contribution.

To sign off on a commit, use the `--signoff` or `-s` option in `git commit`.
For example:

```
git commit -m "docs(ref/kernel-boot): Add example for nohz parameter" -s
```


<!-- LINKS -->

[Ubuntu Code of Conduct]: https://ubuntu.com/community/ethos/code-of-conduct
[Canonical contributor license agreement]: https://ubuntu.com/legal/contributors
[Real-time Ubuntu releases]: https://documentation.ubuntu.com/real-time/en/latest/reference/releases/
[Canonical's Sphinx starter pack]: https://github.com/canonical/sphinx-docs-starter-pack
[Read the Docs]: https://about.readthedocs.com/
[Sphinx]: https://www.sphinx-doc.org/
[reStructuredText]: https://www.sphinx-doc.org/en/master/usage/restructuredtext/index.html
[MyST Markdown]: https://myst-parser.readthedocs.io/en/latest/
[starter pack guide]: https://canonical-starter-pack.readthedocs-hosted.com/latest/
[Canonical style guides]: https://canonical-documentation-with-sphinx-and-readthedocscom.readthedocs-hosted.com/#style-guides
[Diátaxis]: https://diataxis.fr/
[woke]: https://github.com/get-woke/woke
[related issues, pull requests, and repositories]: https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/autolinked-references-and-urls
[conventional commits]: https://www.conventionalcommits.org/
