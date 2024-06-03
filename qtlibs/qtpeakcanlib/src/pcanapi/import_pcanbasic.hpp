// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QtCore/qglobal.h>

/** @brief This file is useful to include all the needed resources for the PCANBasic.h file
    @note Call this file instead of directly includes the "PCANBasic.h" file */

#ifdef Q_OS_WIN32
#   include <windows.h>
#   define DRV_CALLBACK_TYPE WINAPI
#else
#   define DRV_CALLBACK_TYPE
#endif

#include "PCANBasic.h"
