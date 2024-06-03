// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "definesutility/definesutility.hpp"


#ifdef MSVC_BUILDER

/** @brief Helper method to disable a warning at build time
    @note The warning disabling is working in two times, first we push the disabling and then we
          pop it (or it will continue on all the next lines). It's that why it's important to not
          forget to use the PUSH with the POP
    @param CODE A text representation of the warning to disable, ex: 4116
                The code has to already be a text
    @see DISABLE_WARNING_POP */
#define DISABLE_WARNING_PUSH( CODE )                \
    __pragma( warning(push) )       \
    __pragma( warning(disable:CODE) )

/** @brief Helper method to re-enable a warning at build time
    @note The warning disabling is working in two times, first we push the disabling and then we
          pop it (or it will continue on all the next lines). It's that why it's important to not
          forget to use the PUSH with the POP
    @note The POP method doesn't POP a specific rule, therefore if you disable two WARNINGs with
          two pushes, you can
    @see DISABLE_WARNING_PUSH */
#define DISABLE_WARNING_POP                         \
     __pragma( warning(pop) )

/** @brief Allow to disable the MSVC error warning: C4146
           unary minus operator applied to unsigned type, result still unsigned */
#define WARNING_INT_MINUS_PUSH                      \
    DISABLE_WARNING_PUSH( 4146 )

/** @brief Allow to enable again the MSVC error warning: C4146
           unary minus operator applied to unsigned type, result still unsigned */
#define WARNING_INT_MINUS_POP                       \
    DISABLE_WARNING_POP


/** @brief Allow to disable the MSVC error warning: C4018
           Using the token operator to compare signed and unsigned numbers required the compiler to
           convert the signed value to unsigned. */
#define WARNING_SIGNED_MISMATCH_PUSH               \
    DISABLE_WARNING_PUSH( 4018 )

/** @brief Allow to enable again the MSVC error warning: C4018
           Using the token operator to compare signed and unsigned numbers required the compiler to
           convert the signed value to unsigned. */
#define WARNING_SIGNED_MISMATCH_POP               \
    DISABLE_WARNING_POP

#endif
