# SPDX-FileCopyrightText: 2022 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, DEFINES_BMS_LIB) : error("$${TARGET} (YAML_BMS_LIB) requires DEFINES_BMS_LIB")
!contains(DEFINES, MANAGERS_BMS_LIB) : error("$${TARGET} (YAML_BMS_LIB) requires MANAGERS_BMS_LIB")
!contains(DEFINES, PROCESS_BMS_LIB) : error("$${TARGET} (YAML_BMS_LIB) requires PROCESS_BMS_LIB")

YAML_LIB_ROOT = $$absolute_path(.)

DEFINES *= YAML_BMS_LIB

INCLUDEPATH *= $$YAML_LIB_ROOT

# API
HEADERS *= $$YAML_LIB_ROOT/yamltojsonmanager.hpp
SOURCES *= $$YAML_LIB_ROOT/yamltojsonmanager.cpp
HEADERS *= $$YAML_LIB_ROOT/yqprocesscaller.hpp
SOURCES *= $$YAML_LIB_ROOT/yqprocesscaller.cpp
