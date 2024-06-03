<!--
SPDX-FileCopyrightText: 2023 Anthony Loiseau <anthony.loiseau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

What is reuse
=============

Reuse is a tool from the FSF to help complying EU copyright laws.

This tool mainly helps developers to:
1. add or udpate a SPDX copyright entry into a file (`reuse annotate`)
2. check that all files of the repo are covered by a copyright
   and that pointed license exists in LICENSES/ (`reuse lint`)

Main website: https://reuse.software


What reuse means for this repo
==============================

All files of this repo either:
- contains a SPDX header on top of them
- contains a SPDX header in a .licence sibling file
- is covered by a wildcard in .reuse/dep5 (discouraged)


Our use of reuse
================

- gitlab-ci/reuse.gitlab-ci.yml ensures MRs can't be merged if one SPDX is missing
  - that is, it runs `reuse lint` over repo files
  - note: this does not ensure SPDX is well updated, only ensure SPDX exists
- `tools/reuse-annotate.sh init` helps old repos migration to SPDX/ACT license
- `tools/reuse-annotate.sh update` helps day-to-day SPDX update with our license
    - See `tools/reuse-annotate.sh --help`


Optional files
==============

Those files may or may not exists in the repository.
When they exists they affect reuse behavior.

.mailmap
--------

This file is useful to fix bad commit authors names or emails when
`tools/reuse-annotate.sh init` parses git history to generate initial
SPDX headers.

This is especially useful when a single author used several names or emails
among commits, since `reuse annotate` only merges SPDX when both name and
email are strictly identical. That is, we better like to end up with
```
   SPDX-FileCopyrightText: 2022 - 2023 John Smith <john.smith@example.com>
```
instead of
```
   SPDX-FileCopyrightText: 2022 John SMITH <john.smith@example.com>
   SPDX-FileCopyrightText: 2023 John Smith <john.smith@example.com>
```

.reuse/dep5
-----------

To be used with parsymony.

This configuration file can affect copyrights to folders
when attaching a SPDX to all indivitual files is not possible
or very complicated.

We better like each file to have its copyrights, but such wildcard
can be useful especially when an entire third library is inserted
in a repo for example.

Note that this file is read by both `reuse` and `tools/reuse-annotate.sh`.
First one reads it properly when checking that all files are covered by a SPDX
while second one only handles a subset of path wildcards when skipping SPDX
update of wildcardrd files. This is very minor since the only downside is that
you may end up with a uselessly SPDX-updated file and you can freely not
commit it.


How to install reuse locally
============================

While not required, a developer can install reuse locally to check compliancy
before pushing to git server if he/she whant to avoid multiple commits until
gitlab pipeline gets happy.

Windows local installation example
----------------------------------

```shell
%PYTHON3% -m pip install pipx
%PYTHON3% -m pipx install reuse
%PYTHON3% -m pipx ensurepath
```


Linux local installation example
--------------------------------

Left to the reader.
