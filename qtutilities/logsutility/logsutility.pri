# SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, THREAD_BMS_LIB) : error("$${TARGET} (LOGS_BMS_LIB) requires THREAD_BMS_LIB")
!contains(DEFINES, FILE_BMS_LIB) : error("$${TARGET} (LOGS_BMS_LIB) requires FILE_BMS_LIB")

LOGS_LIB_ROOT = $$absolute_path(.)

DEFINES *= LOGS_BMS_LIB

INCLUDEPATH *= $$LOGS_LIB_ROOT

# API
## Saving logs in file strategies
HEADERS *= $$LOGS_LIB_ROOT/filestrategies/afilelogsstrategy.hpp
SOURCES *= $$LOGS_LIB_ROOT/filestrategies/afilelogsstrategy.cpp
HEADERS *= $$LOGS_LIB_ROOT/filestrategies/aonefilelogsstrategy.hpp
SOURCES *= $$LOGS_LIB_ROOT/filestrategies/aonefilelogsstrategy.cpp
HEADERS *= $$LOGS_LIB_ROOT/filestrategies/onefileperdaylogsstrategy.hpp
SOURCES *= $$LOGS_LIB_ROOT/filestrategies/onefileperdaylogsstrategy.cpp
HEADERS *= $$LOGS_LIB_ROOT/filestrategies/onefileperobjectlogsstrategy.hpp
SOURCES *= $$LOGS_LIB_ROOT/filestrategies/onefileperobjectlogsstrategy.cpp
## Global
HEADERS *= $$LOGS_LIB_ROOT/loggingoption.hpp
SOURCES *= $$LOGS_LIB_ROOT/loggingoption.cpp
HEADERS *= $$LOGS_LIB_ROOT/loggingstrategy.hpp
SOURCES *= $$LOGS_LIB_ROOT/loggingstrategy.cpp
HEADERS *= $$LOGS_LIB_ROOT/loggingstrategyoption.hpp
SOURCES *= $$LOGS_LIB_ROOT/loggingstrategyoption.cpp
HEADERS *= $$LOGS_LIB_ROOT/logsmanager.hpp
SOURCES *= $$LOGS_LIB_ROOT/logsmanager.cpp
HEADERS *= $$LOGS_LIB_ROOT/logmsgtype.hpp
SOURCES *= $$LOGS_LIB_ROOT/logmsgtype.cpp
HEADERS *= $$LOGS_LIB_ROOT/saveloginfilesthread.hpp
SOURCES *= $$LOGS_LIB_ROOT/saveloginfilesthread.cpp
