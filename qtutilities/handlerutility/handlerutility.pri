# SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

HANDLER_BMS_LIB = $$absolute_path(.)

DEFINES *= HANDLER_BMS_LIB

INCLUDEPATH *= $$HANDLER_BMS_LIB

# API
HEADERS *= $$HANDLER_BMS_LIB/handlerclassmembersmixin.hpp
SOURCES *= $$HANDLER_BMS_LIB/handlerclassmembersmixin.cpp
