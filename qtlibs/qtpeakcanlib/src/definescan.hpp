// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QtCore/qglobal.h>

/** @brief Contains useful macro and global elements for this lib */

/** @brief This define helps to export and import the public library classes */
#if defined(QTPEAKCANLIB)
#  define CAN_EXPORT Q_DECL_EXPORT
#else
#  define CAN_EXPORT Q_DECL_IMPORT
#endif


