# SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, DEFINES_BMS_LIB) :\
    error("$${TARGET} (MANAGERS_BMS_LIB) requires DEFINES_BMS_LIB")

MANAGERS_LIB_ROOT = $$absolute_path(.)

DEFINES *= MANAGERS_BMS_LIB

INCLUDEPATH *= $$MANAGERS_LIB_ROOT/

# API
HEADERS *= $$MANAGERS_LIB_ROOT/abstractmanager.hpp
SOURCES *= $$MANAGERS_LIB_ROOT/abstractmanager.cpp
HEADERS *= $$MANAGERS_LIB_ROOT/aglobalmanager.hpp
SOURCES *= $$MANAGERS_LIB_ROOT/aglobalmanager.cpp
