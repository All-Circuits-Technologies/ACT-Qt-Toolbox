# SPDX-FileCopyrightText: 2018 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++14, if not returns an error when compiling
c++14 | c++17 | c++2* {
    # At least C++14 - OK
} else {
    error($${TARGET} requires at least c++14)
}

!contains(DEFINES, DEFINES_BMS_LIB) : error("$${TARGET} (NUMBER_BMS_LIB) requires DEFINES_BMS_LIB")
!contains(DEFINES, BYTE_BMS_LIB) : error("$${TARGET} (NUMBER_BMS_LIB) requires BYTE_BMS_LIB")

NUMBER_LIB_ROOT = $$absolute_path(.)

DEFINES *= NUMBER_BMS_LIB

INCLUDEPATH *= $$NUMBER_LIB_ROOT

# API
HEADERS *= $$NUMBER_LIB_ROOT/number.hpp
SOURCES *= $$NUMBER_LIB_ROOT/number.cpp
