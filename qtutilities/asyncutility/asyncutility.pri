# SPDX-FileCopyrightText: 2018 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, WAIT_BMS_LIB) : error("$${TARGET} (ASYNC_BMS_LIB) requires WAIT_BMS_LIB")

ASYNC_LIB_ROOT = $$absolute_path(.)

DEFINES *= ASYNC_BMS_LIB

INCLUDEPATH *= $$ASYNC_LIB_ROOT/

# API
HEADERS *= $$ASYNC_LIB_ROOT/asynctypes.hpp
HEADERS *= $$ASYNC_LIB_ROOT/asyncwaithelper.hpp
SOURCES *= $$ASYNC_LIB_ROOT/asyncwaithelper.cpp
HEADERS *= $$ASYNC_LIB_ROOT/asyncwaterfallhelper.hpp
SOURCES *= $$ASYNC_LIB_ROOT/asyncwaterfallhelper.cpp
