// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * elfconstants.hpp - COPYRIGHT (C) 2019 ALL CIRCUITS
 * ------------------------------------------------------------------
 */

#pragma once

#include <QStringList>


namespace ElfConstants
{
    /** @brief ELF file accepted suffixes */
    const QStringList elfFileSuffixes { "elf", "out", "abs" };
}
