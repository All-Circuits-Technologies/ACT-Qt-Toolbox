# SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, BYTE_BMS_LIB) : \
    error("$${TARGET} (INTEL_HEX_FILE_BMS_LIB) requires BYTE_BMS_LIB")

INTEL_HEX_FILE_ROOT = $$absolute_path(.)

DEFINES *= INTEL_HEX_FILE_BMS_LIB

INCLUDEPATH *= $$INTEL_HEX_FILE_ROOT

# API
HEADERS *= $$INTEL_HEX_FILE_ROOT/hexchecksumhelper.hpp
SOURCES *= $$INTEL_HEX_FILE_ROOT/hexchecksumhelper.cpp
HEADERS *= $$INTEL_HEX_FILE_ROOT/hexline.hpp
SOURCES *= $$INTEL_HEX_FILE_ROOT/hexline.cpp
HEADERS *= $$INTEL_HEX_FILE_ROOT/intelhexdevice.hpp
SOURCES *= $$INTEL_HEX_FILE_ROOT/intelhexdevice.cpp
HEADERS *= $$INTEL_HEX_FILE_ROOT/recordtype.hpp
SOURCES *= $$INTEL_HEX_FILE_ROOT/recordtype.cpp
