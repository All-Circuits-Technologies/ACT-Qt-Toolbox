// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "definesutility/definesutility-shared.hpp"


/** @file
    @brief In the old versions MSVC doesn't support _Pragma, but __pragma (with a different syntax).
    @note The last version of MSVC are going to support _Pragma, as you can read here:
          https://devblogs.microsoft.com/cppblog/announcing-full-support-for-a-c-c-conformant-preprocessor-in-msvc/ */

#ifdef MSVC_BUILDER

/** @brief Help to print a warning for the MSVC builder
    @note Pragma message displays a message in the Compile output, to be considered as a warning
          the given string has to be formatted like this:
           ": warning<something>: blah blah"
          But Qt Creator doesn't consider it as a warning, to make it works we have to "cheat" and
          format the message as a MSVC warning (with an error code), so like this:
            "test.hpp(12): warning C0000: <somethings> blah blah"
    @param VA_ARGS Text with the prefix (TODO, FIXME, etc...) */
#define PRINT_WARNING(...)                              \
    __pragma(message(__FILE__ "(" STRINGIFY2(__LINE__) "): warning C0000: " STRINGIFY(__VA_ARGS__)))

#endif
