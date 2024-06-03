# SPDX-FileCopyrightText: 2021 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# ------------------------------------------------------------------
# ActQtToolboxRoot.pro - COPYRIGHT (C) 2021 BMS CIRCUITS
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# This project file is a super-project over all ActQtToolbox.
#
# It includes those sub-projects as well as few generic extra files
# ------------------------------------------------------------------

TEMPLATE = subdirs

ROOT = $$absolute_path(.)
QT_LIBS = $$absolute_path($$ROOT/qtlibs)
QT_UTILITIES = $$absolute_path($$ROOT/qtutilities)

SUBDIRS *= ActQtToolbox.pro

LIBS_LIST = $$files($$QT_LIBS/qt*lib)
UTILITIES_LIST = $$files($$QT_UTILITIES/*utility)

# We add the ACT libs
SUBDIRS *= $$LIBS_LIST

# We add the unit tests from the utilities code and ACT lib
for(utilityPath, $$list($$UTILITIES_LIST, $$LIBS_LIST)) {
    SUBDIRS *= $$files($$utilityPath/utest-*)
}
