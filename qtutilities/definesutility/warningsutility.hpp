// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "definesutility/warningsutility-gcc.hpp"
#include "definesutility/warningsutility-msvc.hpp"

/** @file
    @brief Contains useful macro for disabling warnings at build time */

/** @brief PUSH empty macro for specific INT MINUS warning */
#ifndef WARNING_INT_MINUS_PUSH
#define WARNING_INT_MINUS_PUSH
#endif

/** @brief POP empty macro for specific INT MINUS warning */
#ifndef WARNING_INT_MINUS_POP
#define WARNING_INT_MINUS_POP
#endif

/** @brief PUSH empty macro for specific SIGNED MISMATCH warning */
#ifndef WARNING_SIGNED_MISMATCH_PUSH
#define WARNING_SIGNED_MISMATCH_PUSH
#endif

/** @brief POP empty macro for specific SIGNED MISMATCH warning */
#ifndef WARNING_SIGNED_MISMATCH_POP
#define WARNING_SIGNED_MISMATCH_POP
#endif
