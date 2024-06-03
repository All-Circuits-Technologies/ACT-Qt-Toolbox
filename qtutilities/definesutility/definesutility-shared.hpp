// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

/** @file
    @brief Contains useful macro shared by all builder and used in their files */

/** @def STRINGIFY
   Easily stringify a raw unquoted text given as argument.
   Text can be multiline and can contain comas.
   Every spaces, blanks and new lines will be compressed as a single space.

   Limitations:

   Can not be used in a #define due to preprocessor single-pass.

   VAARG is used to accept comas but I think VAAARGS in macros are not
   ANSI C89 so you may get warnings and need STRINGIFY_C89

   @see STRINGIFY_C89 */
#define STRINGIFY(...) #__VA_ARGS__

/** @brief Use a second level for stringify */
#define STRINGIFY2(...) STRINGIFY(__VA_ARGS__)
