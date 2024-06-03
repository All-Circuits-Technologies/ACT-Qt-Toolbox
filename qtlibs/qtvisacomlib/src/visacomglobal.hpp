// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QtCore/qglobal.h>

#if defined(QTVISACOMLIB)
#  define VISACOM_EXPORT Q_DECL_EXPORT
#else
#  define VISACOM_EXPORT Q_DECL_IMPORT
#endif
