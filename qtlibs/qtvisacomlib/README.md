<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# QtVisaComLib

## Table of contents

- [QtVisaComLib](#qtvisacomlib)
  - [Table of contents](#table-of-contents)
  - [Presentation](#presentation)
  - [Constraints](#constraints)
  - [Dependencies](#dependencies)

## Presentation

This is a Qt library used to communicate with Visa. The library uses generic visa libraries for its
proper functioning.

To build the lib, you need header files to link with the libraries.

> [!IMPORTANT]
> These external files should be included manually in the Qt project in a local 3rdparty folder

More details of the expected files in the [dependencies chapter](#dependencies)

## Constraints

The list has only be built in Windows for MSVC.

## Dependencies

To build the `QtVisaComLib` needs you to add those files into the repository.

```ini
./
├── 3rdparty/
|   ├── include/
|   |   ├── visa.h
|   |   ├── visatype.h
|   |   └── vpptype.h
|   ├── lib_x64/
|   |   ├── nivisa64.lib  # If you use the national instrument visa com lib
|   |   └── visa64.lib
|   ├── lib_x86/
|   |   ├── nivisa32.lib  # If you use the national instrument visa com lib
|   |   └── visa32.lib
├── src/
├── .gitignore
├── qtvisacomlib.pro
└── README.md
```

With:

- `3rdparty/`: you need to create the folder and subfolders
- `lib_x64/` this folder, and its content, is only needed if you build the lib in 64bits
- `lib_x86/` this folder, and its content, is only needed if you build the lib in 32bits
