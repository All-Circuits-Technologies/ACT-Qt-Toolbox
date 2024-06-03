# SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

IMPORTER_ROOT = $$absolute_path(.)
QTUTILITIES = $$absolute_path($$IMPORTER_ROOT/../qtutilities)
QTLIBS = $$absolute_path($$IMPORTER_ROOT)

DEFINES *= IMPORT_QTSERIALLINKLIB

DEPENDPATH *= $$QTLIBS/qtseriallinklib
INCLUDEPATH *= $$QTLIBS/qtseriallinklib
INCLUDEPATH *= $$QTLIBS

LIBS *= -L$$DESTDIR_LIBS -l$$qtLibraryTarget(qtseriallinklib)
