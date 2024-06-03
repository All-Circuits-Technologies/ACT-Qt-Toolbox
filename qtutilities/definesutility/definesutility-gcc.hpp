// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "definesutility/definesutility-shared.hpp"


/** @file
    @brief This file contains specific macros for GCC builder */

#ifdef GCC_BUILDER

/** @brief Useful method to stringify the elements before calling _Pragma */
#define PRINT_WARNING2(...)                             \
    _Pragma(STRINGIFY(__VA_ARGS__))

/** @brief Help to print a warning for the GCC builder
    @note Need to add GCC before warning or the message is misinterpreted
    @param VA_ARGS Text with the prefix (TODO, FIXME, etc...) */
#define PRINT_WARNING(...)                              \
    PRINT_WARNING2(GCC warning STRINGIFY(__VA_ARGS__))

#endif
