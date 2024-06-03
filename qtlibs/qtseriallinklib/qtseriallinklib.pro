# SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

!versionAtLeast(QT_VERSION, 5.15.2): error("Requires Qt 5.15 or newer")

VERSION = 1.1.0

BASENAME = $${basename(_PRO_FILE_)}
FILENAME = $$section(BASENAME, '.', 0, 0)

QT -= gui
QT += serialport

TARGET = $$qtLibraryTarget($${FILENAME})
TARGET_EXT =.dll

win32-msvc* {
    TARGET_EXT =.dll
}

TEMPLATE = lib
DEFINES += QTSERIALLINKLIB

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIB_PATH = $$absolute_path(.)
QTLIB_PATH = $$absolute_path(..)
ROOT = $$absolute_path(../..)
QT_UTILITIES = $$absolute_path($$ROOT/qtutilities)

include($$ROOT/import-build-params.pri)

DESTDIR = $$DESTDIR_LIBS

INCLUDEPATH *= $$LIB_PATH
INCLUDEPATH *= $$QT_UTILITIES
INCLUDEPATH *= $$ROOT

HEADERS *= $$LIB_PATH/definesseriallink.hpp
HEADERS *= $$LIB_PATH/seriallibconstants.hpp
HEADERS *= $$LIB_PATH/seriallink.hpp
SOURCES *= $$LIB_PATH/seriallink.cpp
HEADERS *= $$LIB_PATH/seriallinkintf.hpp
SOURCES *= $$LIB_PATH/seriallinkintf.cpp
HEADERS *= $$LIB_PATH/seriallinkmanager.hpp
SOURCES *= $$LIB_PATH/seriallinkmanager.cpp
HEADERS *= $$LIB_PATH/seriallinkthread.hpp
SOURCES *= $$LIB_PATH/seriallinkthread.cpp

include($$QT_UTILITIES/definesutility/definesutility.pri)
include($$QT_UTILITIES/handlerutility/handlerutility.pri)
include($$QT_UTILITIES/waitutility/waitutility.pri)
include($$QT_UTILITIES/threadutility/threadutility.pri)

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
