# SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

!versionAtLeast(QT_VERSION, 5.15.2): error("Requires Qt 5.15 or newer")

VERSION = 1.0.0

BASENAME = $${basename(_PRO_FILE_)}
FILENAME = $$section(BASENAME, '.', 0, 0)

TARGET = $$qtLibraryTarget($${FILENAME})
TARGET_EXT =.dll

QT -= gui
QT += serialbus

TARGET = $$qtLibraryTarget($${FILENAME})

win32-msvc* {
    TARGET_EXT =.dll
}

TEMPLATE = lib
DEFINES += QTPEAKCANLIB

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIB_PATH = $$absolute_path(.)
QTLIB_PATH = $$absolute_path(..)
ROOT = $$absolute_path(../..)
QT_UTILITIES = $$absolute_path($$ROOT/qtutilities)

include($$ROOT/import-build-params.pri)

!exists($$LIB_PATH/3rdparty/include/PCANBasic.h) {
    error("To build the qtpeakcanlib, you have to include the 3rd party, see README.md to more\
           details")
}

contains(QT_ARCH, x86_64) {
    LIBS += -L"$$LIB_PATH/3rdparty/lib_x64" -lPCANBasic
} else {
    LIBS += -L"$$LIB_PATH/3rdparty/lib_x86" -lPCANBasic
}

INCLUDEPATH += "$$LIB_PATH/3rdparty/include"
DEPENDPATH += "$$LIB_PATH/3rdparty/include"

DESTDIR = $$DESTDIR_LIBS

INCLUDEPATH *= $$LIB_PATH
INCLUDEPATH *= $$QT_UTILITIES
INCLUDEPATH *= $$ROOT

HEADERS *= $$LIB_PATH/src/candevice/candevice.hpp
SOURCES *= $$LIB_PATH/src/candevice/candevice.cpp
HEADERS *= $$LIB_PATH/src/candevice/candeviceintf.hpp
SOURCES *= $$LIB_PATH/src/candevice/candeviceintf.cpp
HEADERS *= $$LIB_PATH/src/candevice/candevicethread.hpp
SOURCES *= $$LIB_PATH/src/candevice/candevicethread.cpp
HEADERS *= $$LIB_PATH/src/canmanager.hpp
SOURCES *= $$LIB_PATH/src/canmanager.cpp
HEADERS *= $$LIB_PATH/src/definescan.hpp

# Models
HEADERS *= $$LIB_PATH/src/models/candeviceconfig.hpp
SOURCES *= $$LIB_PATH/src/models/candeviceconfig.cpp
HEADERS *= $$LIB_PATH/src/models/candeviceconfigdetails.hpp
SOURCES *= $$LIB_PATH/src/models/candeviceconfigdetails.cpp
HEADERS *= $$LIB_PATH/src/models/candevicefdconfigdetails.hpp
SOURCES *= $$LIB_PATH/src/models/candevicefdconfigdetails.cpp
HEADERS *= $$LIB_PATH/src/models/candeviceinfo.hpp
SOURCES *= $$LIB_PATH/src/models/candeviceinfo.cpp
HEADERS *= $$LIB_PATH/src/models/expectedcanframemask.hpp
SOURCES *= $$LIB_PATH/src/models/expectedcanframemask.cpp

# PCAN Api
HEADERS *= $$LIB_PATH/src/pcanapi/import_pcanbasic.hpp
HEADERS *= $$LIB_PATH/src/pcanapi/pcanapi.hpp
SOURCES *= $$LIB_PATH/src/pcanapi/pcanapi.cpp
HEADERS *= $$LIB_PATH/src/pcanapi/pcanbaudrate.hpp
SOURCES *= $$LIB_PATH/src/pcanapi/pcanbaudrate.cpp
HEADERS *= $$LIB_PATH/src/pcanapi/pcanbusitf.hpp
SOURCES *= $$LIB_PATH/src/pcanapi/pcanbusitf.cpp
HEADERS *= $$LIB_PATH/src/pcanapi/pcanframedlc.hpp
SOURCES *= $$LIB_PATH/src/pcanapi/pcanframedlc.cpp
HEADERS *= $$LIB_PATH/src/pcanapi/pcanreader.hpp
SOURCES *= $$LIB_PATH/src/pcanapi/pcanreader.cpp
HEADERS *= $$LIB_PATH/src/pcanapi/pcanreadthread.hpp
SOURCES *= $$LIB_PATH/src/pcanapi/pcanreadthread.cpp

include($$QT_UTILITIES/definesutility/definesutility.pri)
include($$QT_UTILITIES/byteutility/byteutility.pri)
include($$QT_UTILITIES/handlerutility/handlerutility.pri)
include($$QT_UTILITIES/numberutility/numberutility.pri)
include($$QT_UTILITIES/waitutility/waitutility.pri)
include($$QT_UTILITIES/threadutility/threadutility.pri)

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
