# SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, BYTE_BMS_LIB) : error("$${TARGET} (CAN_BMS_LIB) requires BYTE_BMS_LIB")
!qtHaveModule(serialbus) : error("$${TARGET} (CAN_BMS_LIB) requires to have serialbus QT module")

DEFINES *= CAN_BMS_LIB

CAN_BMS_LIB = $$absolute_path(.)

INCLUDEPATH *= $$CAN_BMS_LIB

# API
HEADERS *= $$CAN_BMS_LIB/canbusframehelper.hpp
SOURCES *= $$CAN_BMS_LIB/canbusframehelper.cpp
