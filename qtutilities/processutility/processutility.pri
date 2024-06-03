# SPDX-FileCopyrightText: 2022 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, DEFINES_BMS_LIB) : error("$${TARGET} (PROCESS_BMS_LIB) requires DEFINES_BMS_LIB")
!contains(DEFINES, WAIT_BMS_LIB) : error("$${TARGET} (PROCESS_BMS_LIB) requires WAIT_BMS_LIB")

PROCESS_LIB_ROOT = $$absolute_path(.)

DEFINES *= PROCESS_BMS_LIB

INCLUDEPATH *= $$PROCESS_LIB_ROOT/

# API
HEADERS *= $$PROCESS_LIB_ROOT/processcaller.hpp
SOURCES *= $$PROCESS_LIB_ROOT/processcaller.cpp
