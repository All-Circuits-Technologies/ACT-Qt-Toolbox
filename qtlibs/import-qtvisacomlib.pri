# SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

!win32-msvc* {
    error("The lib qtvisacomlib can only be built with MSVC \
           Therefore the project $${basename(_PRO_FILE_)} is unavailabe for other systems or \
           compiler")
}

IMPORTER_ROOT = $$absolute_path(.)
QTUTILITIES = $$absolute_path($$IMPORTER_ROOT/../qtutilities)
QTLIBS = $$absolute_path($$IMPORTER_ROOT)

DEFINES *= IMPORT_QTVISACOMLIB

DEPENDPATH *= $$QTLIBS/qtvisacomlib
INCLUDEPATH *= $$QTLIBS/qtvisacomlib
INCLUDEPATH *= $$QTLIBS

LIBS *= -L$$DESTDIR_LIBS -l$$qtLibraryTarget(qtvisacomlib)
