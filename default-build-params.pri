# SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# This file helps to set the right destdir folder for the right library/application
# It also manages different folder by compiler: to prevent to mix the building.
# This file contains the default values, to overwrite them you have to create a "build-params.pri"
# file

isEmpty( BUILD_PATH )|!exists( $$BUILD_PATH ) {
    error("You have to set a BUILD_PATH variable which is the path to the project root path,\
           before including this the build-params.pri")
}

win32-msvc* {
    DESTDIR_LIBS    = $$BUILD_PATH/build-out-msvc
} else {
    DESTDIR_LIBS    = $$BUILD_PATH/build-out
}
