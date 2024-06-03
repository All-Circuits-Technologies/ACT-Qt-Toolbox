# SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

STRING_BMS_LIB = $$absolute_path(.)

DEFINES *= STRING_BMS_LIB

INCLUDEPATH *= $$STRING_BMS_LIB

# API
HEADERS *= $$STRING_BMS_LIB/stringhelper.hpp
SOURCES *= $$STRING_BMS_LIB/stringhelper.cpp
