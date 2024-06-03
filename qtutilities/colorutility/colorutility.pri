# SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

COLOR_LIB_ROOT = $$absolute_path(.)

DEFINES *= COLOR_BMS_LIB

INCLUDEPATH *= $$COLOR_LIB_ROOT

# API
HEADERS *= $$COLOR_LIB_ROOT/angularmaterialthemingcolors.hpp
SOURCES *= $$COLOR_LIB_ROOT/angularmaterialthemingcolors.cpp
HEADERS *= $$COLOR_LIB_ROOT/colorhelper.hpp
SOURCES *= $$COLOR_LIB_ROOT/colorhelper.cpp
