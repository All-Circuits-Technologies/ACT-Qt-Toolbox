// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QCoreApplication>

/** @file
    @brief Contains useful macro and global elements for helping the transations in project */


/** @brief Returns the translation text for KEY, by querying the installed translation files.
           The translation files are searched from the most recently installed file back to the
           first installed file.
    @note Most of the time, the usage of QObject::tr is sufficient but it's not when defining
          elements to translate in template classes.
    @note The macro is an alias for the qApp translate method */
#define TR_WITH_CONTEXT( CONTEXT, KEY )     \
    qApp->translate(CONTEXT, KEY)
