# SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, DEFINES_BMS_LIB) :\
    error("$${TARGET} (TRANSLATION_BMS_LIB) requires DEFINES_BMS_LIB")

isEmpty( SUPPORTED_LAN_KEYS ) {
    warning("You haven't give any language keys to support in $${TARGET}, to do so fill the \
             SUPPORTED_LAN_KEYS variable with a list of language keys before including this \
             translationutility.pri file, for instance: SUPPORTED_LAN_KEYS = es fr_ca")
} else {
    for(key, $$list( $$SUPPORTED_LAN_KEYS )) {
        TRANSLATIONS *= "$${FILENAME}-$${key}.ts"
    }
}

TRANSLATION_LIB_ROOT = $$absolute_path(.)

DEFINES *= TRANSLATION_BMS_LIB

INCLUDEPATH *= $$TRANSLATION_LIB_ROOT

# API
HEADERS *= $$TRANSLATION_LIB_ROOT/translationhelper.hpp
SOURCES *= $$TRANSLATION_LIB_ROOT/translationhelper.cpp
