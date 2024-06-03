# SPDX-FileCopyrightText: 2018 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, DEFINES_BMS_LIB) : error("$${TARGET} (BYTE_BMS_LIB) requires DEFINES_BMS_LIB")

BYTE_LIB_ROOT = $$absolute_path(.)

DEFINES *= BYTE_BMS_LIB

INCLUDEPATH *= $$BYTE_LIB_ROOT/

# API
HEADERS *= $$BYTE_LIB_ROOT/bytearrayhelper.hpp
SOURCES *= $$BYTE_LIB_ROOT/bytearrayhelper.cpp
HEADERS *= $$BYTE_LIB_ROOT/checksum8bitshelper.hpp
SOURCES *= $$BYTE_LIB_ROOT/checksum8bitshelper.cpp
HEADERS *= $$BYTE_LIB_ROOT/endianesshelper.hpp
HEADERS *= $$BYTE_LIB_ROOT/mathhelper.hpp
SOURCES *= $$BYTE_LIB_ROOT/mathhelper.cpp
