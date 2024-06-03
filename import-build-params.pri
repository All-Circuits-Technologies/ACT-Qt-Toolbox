# SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# This file helps to use the right build parameters information depending of the root git project.
# The file tries to load the existent build params of the most top level git repository, if it
# doesn't exist it will load the submodule build params, if it doesn't exist the submodule of the
# submodule build params, etc.
# Notes:
#  - Only one build-params.pri or default-build-params.pri is included
#  - If two build-params.pri and default-build-params.pri files exist, the build-params.pri file has
#    the priority over default-build-params.pri and will be included instead.
#  - However, a top project default-build-params.pri is more important than the build-params.pri of
#    a submodule.
#    With an exemple, the build params priority is:
#      - Top project:
#         - build-params.pri
#         - default-build-params.pri
#         - submodule project:
#            - build-params.pri
#            - default-build-params.pri
#            - submodule project:
#               - build-params.pri
#               - default-build-params.pri

# We get the git root repo path
MAIN_GIT_PATH = $$absolute_path(.)
BUILD_PATH = $$MAIN_GIT_PATH

# We expect to have less than 5 git level of submodules
LOOP_VALUES = $$list(1 2 3 4 5)

for(idx, $$LOOP_VALUES) {
    ABOVE_GIT_REPO = $$system(git -C $$MAIN_GIT_PATH rev-parse --show-superproject-working-tree)

    !isEmpty( ABOVE_GIT_REPO ) {
        MAIN_GIT_PATH = $$ABOVE_GIT_REPO
        exists($$MAIN_GIT_PATH/build-params.pri)|exists($$MAIN_GIT_PATH/default-build-params.pri) {
            BUILD_PATH = $$MAIN_GIT_PATH
        }
    }
}

!isEmpty( ABOVE_GIT_REPO ) {
    error("In the import build params pri file, we haven't succeeded to get the top root git\
           project, this means that we haven't enough values in LOOP_VALUES")
}

CONFIG(release, debug|release) {
    message(Release build)
    DEFINES *= ACT_BUILD_RELEASE
}

exists($$BUILD_PATH/build-params.pri):        include($$BUILD_PATH/build-params.pri)
else:exists($$BUILD_PATH/default-build-params.pri):   include($$BUILD_PATH/default-build-params.pri)
