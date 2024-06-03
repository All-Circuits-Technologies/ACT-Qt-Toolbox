# SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

DEFINES *= COLLECTION_BMS_LIB

COLLECTION_LIB_ROOT = $$absolute_path(.)

INCLUDEPATH *= $$COLLECTION_LIB_ROOT

# API
HEADERS *= $${COLLECTION_LIB_ROOT}/qhashhelper.hpp
SOURCES *= $${COLLECTION_LIB_ROOT}/qhashhelper.cpp
HEADERS *= $${COLLECTION_LIB_ROOT}/qmaphelper.hpp
SOURCES *= $${COLLECTION_LIB_ROOT}/qmaphelper.cpp
HEADERS *= $${COLLECTION_LIB_ROOT}/qvariantlisthelper.hpp
SOURCES *= $${COLLECTION_LIB_ROOT}/qvariantlisthelper.cpp
HEADERS *= $${COLLECTION_LIB_ROOT}/qvectorhelper.hpp
SOURCES *= $${COLLECTION_LIB_ROOT}/qvectorhelper.cpp
