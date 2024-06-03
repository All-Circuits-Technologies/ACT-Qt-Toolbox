# SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# This file helps to set the right destdir folder for the right library/application
# It also manages different folder by compiler: to prevent to mix the building.
# All variables in this file are defined in the jenkins file.

DESTDIR_LIBS = $$(DESTDIR_LIBS)

isEmpty( DESTDIR_LIBS ) {
    error("You have to set a DESTDIR_LIBS variable which is the path to the project out path,\
            before including this the build-params.pri")
}
