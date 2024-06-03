# SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, DEFINES_BMS_LIB) : error("$${TARGET} (JSON_BMS_LIB) requires DEFINES_BMS_LIB")

JSON_LIB_ROOT = $$absolute_path(.)

DEFINES *= JSON_BMS_LIB

INCLUDEPATH *= $$JSON_LIB_ROOT

# API
HEADERS *= $$JSON_LIB_ROOT/jsonconstants.hpp
HEADERS *= $$JSON_LIB_ROOT/jsoncomparator.hpp
SOURCES *= $$JSON_LIB_ROOT/jsoncomparator.cpp
HEADERS *= $$JSON_LIB_ROOT/jsonconverter.hpp
SOURCES *= $$JSON_LIB_ROOT/jsonconverter.cpp
HEADERS *= $$JSON_LIB_ROOT/jsonhelper.hpp
SOURCES *= $$JSON_LIB_ROOT/jsonhelper.cpp
HEADERS *= $$JSON_LIB_ROOT/jsonstringify.hpp
SOURCES *= $$JSON_LIB_ROOT/jsonstringify.cpp
HEADERS *= $$JSON_LIB_ROOT/jsontypedefs.hpp
