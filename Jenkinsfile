// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

def BUILD_PARAMS_FILE_PATH = "jenkins/build-params.pri"

pipeline {
    agent {
        node {
            label 'WIN10_X64'
        }
    }

    options {
        // Keep only logs of latest 5 builds for each branch,
        // and only keep artifacts of latest build of each branch
        buildDiscarder logRotator(daysToKeepStr: '',
                                  numToKeepStr: '5',
                                  artifactDaysToKeepStr: '',
                                  artifactNumToKeepStr: '1')
    }

    parameters {
        string  name: 'QT_VERSION',             defaultValue: '5.15.2',         description: 'Give the Qt version to use when building the app'
        string  name: 'MSVC_COMPILER',          defaultValue: 'msvc2019_64',    description: 'Give the compiler to use when building the app'
        string  name: 'APP_NAME',               defaultValue: 'ActQtToolbox',   description: 'Give the application name used to find dependencies'
        string  name: 'LIB_PCAN_BASIC_VERSION', defaultValue: 'v4.7',           description: 'Give the PCAN lib version used for building the Qt PeakCanLib'
        string  name: 'LIB_VISA_COM_VERSION',   defaultValue: 'v5.8',           description: 'Give the visa com lib version used for building the Qt VisaComLib'
    }

    environment {
        REL_BUILD_PATH          = "__build"
        REL_BIN_PATH            = "__binaries"
        TEST_RESULTS_PATH       = "__utest-results"
        QT_DEP_PATH             = "$REL_BIN_PATH/qt_dependencies"
        DESTDIR_LIBS            = "${WORKSPACE}/${REL_BIN_PATH}"
        QMAKE_PATH              = "${QT_DIR}/${params.QT_VERSION}/${params.MSVC_COMPILER}/bin"
        JOM_PATH                = "${QT_DIR}/Tools/QtCreator/bin"
        PATH                    = "$PATH;$QMAKE_PATH;$TOOLCHAIN_MSVC2019_64_DIR"

        //other depedencies values
        VISUAL_C_REDIST   = "VC_redist.x64.exe"
    }

    stages {

        stage('Checkout') {
            steps {
                cleanWs()
                checkout scm
                sh 'git clean -dfx'
                echo 'Checkout over'
            }
        }

        stage('Get build dependencies') {
            steps {
                echo "1. First: get the depencencies to the PCANBasic lib"
                sh "curl.exe -o libpcanbasic_dependencies.txt ${GIT_REPO_DEPS_URL}/${params.APP_NAME}/libpcanbasic_dependencies.txt"
                bat "for /F \"usebackq tokens=* delims=/\" %%A in (\"libpcanbasic_dependencies.txt\") do curl.exe -o qtlibs/qtpeakcanlib/3rdparty/%%A --create-dirs ${GIT_REPO_DEPS_URL}/lib_PCANBasic/${params.LIB_PCAN_BASIC_VERSION}/%%A"
                echo "2. Second: get the depencencies to the VisaCom lib"
                sh "curl.exe -o libvisacom_dependencies.txt ${GIT_REPO_DEPS_URL}/${params.APP_NAME}/libvisacom_dependencies.txt"
                bat "for /F \"usebackq tokens=* delims=/\" %%A in (\"libvisacom_dependencies.txt\") do curl.exe -o qtlibs/qtvisacomlib/3rdparty/%%A --create-dirs ${GIT_REPO_DEPS_URL}/lib_visacom/${params.LIB_VISA_COM_VERSION}/%%A"
                echo "3. Third: copy special jenkins .pri file on root folder"
                sh "cp ${BUILD_PARAMS_FILE_PATH} build-params.pri"
            }
        }

        stage('QMake') {
            steps {
                sh "mkdir ${REL_BUILD_PATH}"
                sh """
                    cd ${REL_BUILD_PATH}/
                    ${QMAKE_PATH}/qmake.exe -spec win32-msvc CONFIG+=release CONFIG+=qtquickcompiler ../ActQtToolboxRoot.pro
                    ${JOM_PATH}/jom.exe -j \$(nproc || echo 1) qmake_all
                """
            }
        }

        stage('Make') {
            steps {
                bat """
                    cd ${REL_BUILD_PATH}/
                    call C:/\"Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build\"/vcvarsall.bat x64
                    for /f %%i in ('nproc') do set PROC=%%i
                    ${JOM_PATH}/jom.exe /J %PROC%
                """
            }
        }

        stage('Package Qt dependencies') {
            steps {
                bat """
                    call C:/\"Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build\"/vcvarsall.bat x64
                    for %%i in ("${REL_BIN_PATH}/*.exe") do (
                        windeployqt.exe --release --dir "${QT_DEP_PATH}" --no-translations "${REL_BIN_PATH}/%%i"
                    )
                """
                sh "mv ${QT_DEP_PATH}/* ${REL_BIN_PATH}"
            }
        }

        stage('Play unit tests') {
            steps {
                sh "mkdir ${TEST_RESULTS_PATH}"
                bat """
                    call C:/\"Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build\"/vcvarsall.bat x64
                    for %%i in ("${REL_BIN_PATH}/utest-*.exe") do (
                        "${REL_BIN_PATH}/%%i" -junitxml -o "${TEST_RESULTS_PATH}/%%i.xml"
                    )
                """
                junit "${TEST_RESULTS_PATH}/*.xml"
            }
        }
    }
}
