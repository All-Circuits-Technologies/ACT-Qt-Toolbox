<!--
SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# QtPeakCanlib

## Table of contents

- [QtPeakCanlib](#qtpeakcanlib)
  - [Table of contents](#table-of-contents)
  - [Presentation](#presentation)
  - [Constraints](#constraints)
  - [Dependencies](#dependencies)

## Presentation

This library is used to communicate through CAN with a PEAK probe.

To build the lib, you need header files to link with the libraries.

> [!IMPORTANT]
> These external files should be included manually in the Qt project in a local 3rdparty folder

More details of the expected files in the [dependencies chapter](#dependencies)

## Constraints

The list has only be built in Windows for MSVC.

## Dependencies

To build the `QtPeakCanlib` you need to add those files into the repository.

```text
./
├── 3rdparty/
|   ├── include/
|   |   └── PCANBasic.h
|   ├── lib_x64/
|   |   ├── PCANBasic.dll
|   |   └── PCANBasic.lib
|   ├── lib_x86/
|   |   ├── PCANBasic.dll
|   |   └── PCANBasic.lib
├── src/
├── .gitignore
├── qtpeakcanlib.pro
└── README.md
```

With:

- `3rdparty/`: you need to create the folder and subfolders
- `lib_x64/` this folder, and its content, is only needed if you build the lib in 64bits
- `lib_x86/` this folder, and its content, is only needed if you build the lib in 32bits
