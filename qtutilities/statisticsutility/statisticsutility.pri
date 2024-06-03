# SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, COLLECTION_BMS_LIB) :\
    error("$${TARGET} (STATS_BMS_LIB) requires COLLECTION_BMS_LIB")

STATS_LIB_ROOT = $$absolute_path(.)

DEFINES *= STATS_BMS_LIB

INCLUDEPATH *= $$STATS_LIB_ROOT

# API
## Mixins
HEADERS *= $$STATS_LIB_ROOT/mixins/mixinprocessstats.hpp
SOURCES *= $$STATS_LIB_ROOT/mixins/mixinprocessstats.cpp
## Models
HEADERS *= $$STATS_LIB_ROOT/models/astatsinfo.hpp
SOURCES *= $$STATS_LIB_ROOT/models/astatsinfo.cpp
HEADERS *= $$STATS_LIB_ROOT/models/counterstatsinfo.hpp
SOURCES *= $$STATS_LIB_ROOT/models/counterstatsinfo.cpp
HEADERS *= $$STATS_LIB_ROOT/models/statscategory.hpp
SOURCES *= $$STATS_LIB_ROOT/models/statscategory.cpp
