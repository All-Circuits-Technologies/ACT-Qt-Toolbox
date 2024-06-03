# SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

ELF_LIB_ROOT = $$absolute_path(.)

DEFINES *= ELF_BMS_LIB

INCLUDEPATH *= $$ELF_LIB_ROOT/

EXT_LIBS_PATH = $$ELF_LIB_ROOT/3rdparty
ELFIO_ROOT_PATH = $$EXT_LIBS_PATH/elfio
ELFIO_SRC_PATH = $$ELFIO_ROOT_PATH/elfio

# Needed for compiling the external library, also need to have the elfio intermediate directory, for
# the same reason.
INCLUDEPATH *= $$ELFIO_ROOT_PATH
INCLUDEPATH *= $$ELFIO_SRC_PATH

# API
HEADERS *= $$ELF_LIB_ROOT/elfconstants.hpp
HEADERS *= $$ELF_LIB_ROOT/elffilereader.hpp
SOURCES *= $$ELF_LIB_ROOT/elffilereader.cpp

# External API
HEADERS *= $$ELFIO_SRC_PATH/elf_types.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio_dump.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio_dynamic.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio_header.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio_note.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio_relocation.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio_section.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio_segment.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio_strings.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio_symbols.hpp
HEADERS *= $$ELFIO_SRC_PATH/elfio_utils.hpp
