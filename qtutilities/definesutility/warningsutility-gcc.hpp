// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "definesutility/definesutility.hpp"


/** @file
    @brief This file contains specific macros for managing the disabling of GCC warnings */

#ifdef GCC_BUILDER

/** @brief Helper method to disable a warning at build time
    @note The warning disabling is working in two times, first we push the disabling and then we
          pop it (or it will continue on all the next lines). It's that why it's important to not
          forget to use the PUSH with the POP
    @param CODE A text representation of the warning to disable, ex: "-Wuninitialized"
                The code has to already be a text
    @see DISABLE_WARNING_POP */
#define DISABLE_WARNING_PUSH( CODE )                \
    _Pragma( STRINGIFY(GCC diagnostic push) )       \
    _Pragma( STRINGIFY(GCC diagnostic warning CODE) )

/** @brief Helper method to re-enable a warning at build time
    @note The warning disabling is working in two times, first we push the disabling and then we
          pop it (or it will continue on all the next lines). It's that why it's important to not
          forget to use the PUSH with the POP
    @note The POP method doesn't POP a specific rule, therefore if you disable two WARNINGs with
          two pushes, you cant
    @see DISABLE_WARNING_POP */
#define DISABLE_WARNING_POP                         \
    _Pragma( STRINGIFY(GCC diagnostic pop) )

#endif
