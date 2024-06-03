# SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

STATE_MACHINE_LIB_ROOT = $$absolute_path(.)

DEFINES *= STATE_MACHINE_BMS_LIB

INCLUDEPATH *= $$STATE_MACHINE_LIB_ROOT

# API
HEADERS *= $$STATE_MACHINE_LIB_ROOT/unconditionaltransition.hpp
SOURCES *= $$STATE_MACHINE_LIB_ROOT/unconditionaltransition.cpp
