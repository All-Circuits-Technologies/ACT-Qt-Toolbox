# SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

win32-msvc* {
    DEFINES *= MSVC_BUILDER
}

win32-g++*|linux-g++ {
    DEFINES *= GCC_BUILDER
}

BASENAME = $${basename(_PRO_FILE_)}
FILENAME = $$section(BASENAME, '.', 0, 0)

# Make the pro file filename available in the source code
DEFINES *= PRO_FILE_NAME=$$shell_quote(\"$${FILENAME}\")

DEFINES_LIB_ROOT = $$absolute_path(.)

DEFINES *= DEFINES_BMS_LIB

INCLUDEPATH *= $$DEFINES_LIB_ROOT/

# API
HEADERS *= $$DEFINES_LIB_ROOT/definesutility.hpp
HEADERS *= $$DEFINES_LIB_ROOT/definesutility-gcc.hpp
HEADERS *= $$DEFINES_LIB_ROOT/definesutility-msvc.hpp
HEADERS *= $$DEFINES_LIB_ROOT/definesutility-shared.hpp
HEADERS *= $$DEFINES_LIB_ROOT/errormanagement.hpp
HEADERS *= $$DEFINES_LIB_ROOT/translationsutility.hpp
HEADERS *= $$DEFINES_LIB_ROOT/warningsutility-gcc.hpp
HEADERS *= $$DEFINES_LIB_ROOT/warningsutility-msvc.hpp
HEADERS *= $$DEFINES_LIB_ROOT/warningsutility.hpp
