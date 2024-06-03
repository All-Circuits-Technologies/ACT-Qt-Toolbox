# SPDX-FileCopyrightText: 2018 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, DEFINES_BMS_LIB) : error("$${TARGET} (WAIT_BMS_LIB) requires DEFINES_BMS_LIB")

WAIT_LIB_ROOT = $$absolute_path(.)

DEFINES *= WAIT_BMS_LIB

INCLUDEPATH *= $$WAIT_LIB_ROOT/

# API
HEADERS *= $$WAIT_LIB_ROOT/waithelper.hpp
SOURCES *= $$WAIT_LIB_ROOT/waithelper.cpp
