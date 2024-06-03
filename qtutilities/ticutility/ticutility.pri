# SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++14, if not returns an error when compiling
c++14 | c++17 | c++2* {
    # At least C++14 - OK
} else {
    error($${TARGET} requires at least c++14)
}

!contains(DEFINES, MANAGERS_BMS_LIB) : error("$${TARGET} (TIC_BMS_LIB) requires MANAGERS_BMS_LIB")

TIC_LIB_ROOT = $$absolute_path(.)

DEFINES *= TIC_BMS_LIB

INCLUDEPATH *= $$TIC_LIB_ROOT

# API
HEADERS *= $$TIC_LIB_ROOT/tichandler.hpp
SOURCES *= $$TIC_LIB_ROOT/tichandler.cpp
HEADERS *= $$TIC_LIB_ROOT/tichandleraccesskey.hpp
HEADERS *= $$TIC_LIB_ROOT/ticmanager.hpp
SOURCES *= $$TIC_LIB_ROOT/ticmanager.cpp
HEADERS *= $$TIC_LIB_ROOT/tictimer.hpp
SOURCES *= $$TIC_LIB_ROOT/tictimer.cpp
HEADERS *= $$TIC_LIB_ROOT/tictype.hpp
SOURCES *= $$TIC_LIB_ROOT/tictype.cpp
