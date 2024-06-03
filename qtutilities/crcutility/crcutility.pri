# SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, BYTE_BMS_LIB) : error("$${TARGET} (CRC_BMS_LIB) requires BYTE_BMS_LIB")

CRC_LIB_ROOT = $$absolute_path(.)

DEFINES *= CRC_BMS_LIB

INCLUDEPATH *= $$CRC_LIB_ROOT/

# API
HEADERS *= $$CRC_LIB_ROOT/crcconstants.hpp
HEADERS *= $$CRC_LIB_ROOT/crchelper.hpp
SOURCES *= $$CRC_LIB_ROOT/crchelper.cpp
