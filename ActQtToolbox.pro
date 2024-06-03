# SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#-------------------------------------------------
#
# Project created by QtCreator 2019-01-22T09:48:36
#
#-------------------------------------------------

# Qt gui is needed to build the utilities linked to widget
QT *= gui
QT *= core
QT *= serialbus
QT *= serialport

CONFIG *= c++17

BASENAME = $${basename(_PRO_FILE_)}
FILENAME = $$section(BASENAME, '.', 0, 0)

TARGET = $$qtLibraryTarget($${FILENAME})
TARGET_EXT =.dll

TEMPLATE = lib

DEFINES += ACTQTTOOLBOX_LIBRARY

# Allow to have the logs context in release
DEFINES *= QT_MESSAGELOGCONTEXT

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

ROOT = $$absolute_path(.)
QT_LIBS = $$absolute_path($$ROOT/qtlibs)
QT_UTILITIES = $$absolute_path($$ROOT/qtutilities)

INCLUDEPATH *= $$ROOT
INCLUDEPATH *= $$QT_UTILITIES

include($$ROOT/import-build-params.pri)

DESTDIR = $$DESTDIR_LIBS

# Utilities for projects
include($$QT_UTILITIES/definesutility/definesutility.pri)
include($$QT_UTILITIES/managersutility/managersutility.pri)
include($$QT_UTILITIES/waitutility/waitutility.pri)
include($$QT_UTILITIES/asyncutility/asyncutility.pri)
include($$QT_UTILITIES/byteutility/byteutility.pri)
include($$QT_UTILITIES/fileutility/fileutility.pri)
include($$QT_UTILITIES/intelhexfileutility/intelhexfileutility.pri)
include($$QT_UTILITIES/collectionutility/collectionutility.pri)
include($$QT_UTILITIES/stringutility/stringutility.pri)
include($$QT_UTILITIES/colorutility/colorutility.pri)
include($$QT_UTILITIES/crcutility/crcutility.pri)
include($$QT_UTILITIES/elfutility/elfutility.pri)
include($$QT_UTILITIES/jsonutility/jsonutility.pri)
include($$QT_UTILITIES/statemachineutility/statemachineutility.pri)
include($$QT_UTILITIES/threadutility/threadutility.pri)
include($$QT_UTILITIES/translationutility/translationutility.pri)
include($$QT_UTILITIES/numberutility/numberutility.pri)
include($$QT_UTILITIES/ticutility/ticutility.pri)
include($$QT_UTILITIES/logsutility/logsutility.pri)
include($$QT_UTILITIES/handlerutility/handlerutility.pri)
include($$QT_UTILITIES/processutility/processutility.pri)
include($$QT_UTILITIES/statisticsutility/statisticsutility.pri)
include($$QT_UTILITIES/canutility/canutility.pri)
include($$QT_UTILITIES/yamlutility/yamlutility.pri)

# Import external lib
# To be added, the qtvisacomlib has to match what it has been done with the "default build params"
include($$QT_LIBS/import-qtvisacomlib.pri)
include($$QT_LIBS/import-qtseriallinklib.pri)
include($$QT_LIBS/import-qtpeakcanlib.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}
