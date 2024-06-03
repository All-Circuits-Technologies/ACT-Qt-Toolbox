# SPDX-FileCopyrightText: 2018 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, WAIT_BMS_LIB) : error("$${TARGET} (THREAD_BMS_LIB) requires WAIT_BMS_LIB")

THREAD_LIB_ROOT = $$absolute_path(.)

DEFINES *= THREAD_BMS_LIB

INCLUDEPATH *= $$THREAD_LIB_ROOT/

# API
## Concurrent API
HEADERS *= $$THREAD_LIB_ROOT/concurrent/threadconcurrentrun.hpp
SOURCES *= $$THREAD_LIB_ROOT/concurrent/threadconcurrentrun.cpp
## Global
HEADERS *= $$THREAD_LIB_ROOT/basethread.hpp
SOURCES *= $$THREAD_LIB_ROOT/basethread.cpp
HEADERS *= $$THREAD_LIB_ROOT/basethreadhandler.hpp
SOURCES *= $$THREAD_LIB_ROOT/basethreadhandler.cpp
HEADERS *= $$THREAD_LIB_ROOT/threadrunninghelper.hpp
SOURCES *= $$THREAD_LIB_ROOT/threadrunninghelper.cpp
