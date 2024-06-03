# SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# ------------------------------------------------------------------
# utest-number.pro - COPYRIGHT (C) 2019 BMS CIRCUITS
# ------------------------------------------------------------------

!versionAtLeast(QT_VERSION, 5.12.0): error("Requires Qt 5.12 or newer")

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

CONFIG *= c++17

TEMPLATE = app

ROOT = $$absolute_path(../../..)
QT_UTILITIES = $$absolute_path($$ROOT/qtutilities)
NUMBER_ROOT = $$absolute_path(..)
TEST_ROOT = $$absolute_path(.)

include($$ROOT/import-build-params.pri)

DESTDIR = $$DESTDIR_LIBS

INCLUDEPATH *= $$ROOT
INCLUDEPATH *= $$QT_UTILITIES
INCLUDEPATH *= $$TEST_ROOT

HEADERS *=  tst_number.hpp
SOURCES *=  tst_number.cpp

include($$QT_UTILITIES/definesutility/definesutility.pri)
include($$QT_UTILITIES/byteutility/byteutility.pri)
include($$QT_UTILITIES/numberutility/numberutility.pri)

unix {
    target.path = /opt/utest
    INSTALLS += target
}
