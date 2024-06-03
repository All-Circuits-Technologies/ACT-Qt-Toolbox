# SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

!versionAtLeast(QT_VERSION, 5.15.2): error("Requires Qt 5.15 or newer")

VERSION = 1.0.0

BASENAME = $${basename(_PRO_FILE_)}
FILENAME = $$section(BASENAME, '.', 0, 0)

!win32-msvc* {
    message("For now, the library can only be built with MSVC")
    message("Therefore this lib is unavailabe for other systems or compiler")
    warning("$${FILENAME} lib can't be build with different compiler than MSVC")
}

TARGET = $$qtLibraryTarget($${FILENAME})

QT -= gui

TEMPLATE = lib
DEFINES += QTVISACOMLIB

CONFIG *= c++17

LIB_PATH = $$absolute_path(.)
QTLIB_PATH = $$absolute_path(..)
ROOT = $$absolute_path(../..)
QT_UTILITIES = $$absolute_path($$ROOT/qtutilities)

include($$ROOT/import-build-params.pri)

!exists($$LIB_PATH/3rdparty/include/visa.h) {
    error("To build the qtvisacomlib, you have to include the 3rd party, see README.md to more\
           details")
}

INCLUDEPATH += "$$LIB_PATH/3rdparty/include"
DEPENDPATH += "$$LIB_PATH/3rdparty/include"

DESTDIR = $$DESTDIR_LIBS

INCLUDEPATH *= $$LIB_PATH
INCLUDEPATH *= $$QT_UTILITIES
INCLUDEPATH *= $$ROOT

win32-msvc* {

    TARGET_EXT =.dll

    contains(QT_ARCH, x86_64) {
        LIBS += -L"$$LIB_PATH/3rdparty/lib_x64" -lvisa64
    } else {
        LIBS += -L"$$LIB_PATH/3rdparty/lib_x86" -lvisa32
    }

    # You can make your code fail to compile if it uses deprecated APIs.
    # In order to do so, uncomment the following line.
    #DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

    # Src elements
    HEADERS *= $$LIB_PATH/src/avisacom.hpp
    SOURCES *= $$LIB_PATH/src/avisacom.cpp
    HEADERS *= $$LIB_PATH/src/avisacomaccesskey.hpp
    HEADERS *= $$LIB_PATH/src/visacomasrl.hpp
    SOURCES *= $$LIB_PATH/src/visacomasrl.cpp
    HEADERS *= $$LIB_PATH/src/visacomgpib.hpp
    SOURCES *= $$LIB_PATH/src/visacomgpib.cpp
    HEADERS *= $$LIB_PATH/src/visacommanager.hpp
    SOURCES *= $$LIB_PATH/src/visacommanager.cpp
    HEADERS *= $$LIB_PATH/src/visacomusb.hpp
    SOURCES *= $$LIB_PATH/src/visacomusb.cpp
    HEADERS *= $$LIB_PATH/src/visacomglobal.hpp

    HEADERS *= $$LIB_PATH/src/visaasyncmanager.hpp
    SOURCES *= $$LIB_PATH/src/visaasyncmanager.cpp
    HEADERS *= $$LIB_PATH/src/visaasyncthread.hpp
    SOURCES *= $$LIB_PATH/src/visaasyncthread.cpp

    HEADERS *= $$LIB_PATH/src/visacomgpib4881.hpp
    SOURCES *= $$LIB_PATH/src/visacomgpib4881.cpp

    # Serial Enum elements
    HEADERS *= $$LIB_PATH/src/serial/flowcontrol.hpp
    SOURCES *= $$LIB_PATH/src/serial/flowcontrol.cpp
    HEADERS *= $$LIB_PATH/src/serial/parity.hpp
    SOURCES *= $$LIB_PATH/src/serial/parity.cpp
    HEADERS *= $$LIB_PATH/src/serial/stopbits.hpp
    SOURCES *= $$LIB_PATH/src/serial/stopbits.cpp
}

include($$QT_UTILITIES/definesutility/definesutility.pri)
include($$QT_UTILITIES/waitutility/waitutility.pri)
include($$QT_UTILITIES/threadutility/threadutility.pri)

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
