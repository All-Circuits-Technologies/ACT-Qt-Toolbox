# SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

FILE_LIB_ROOT = $$absolute_path(.)

DEFINES *= FILE_BMS_LIB

INCLUDEPATH *= $$FILE_LIB_ROOT

# API
HEADERS *= $$FILE_LIB_ROOT/filehelper.hpp
SOURCES *= $$FILE_LIB_ROOT/filehelper.cpp
HEADERS *= $$FILE_LIB_ROOT/temporaryfile.hpp
SOURCES *= $$FILE_LIB_ROOT/temporaryfile.cpp
