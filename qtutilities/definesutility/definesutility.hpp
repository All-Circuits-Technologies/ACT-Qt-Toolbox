// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "definesutility/definesutility-shared.hpp"
#include "definesutility/definesutility-msvc.hpp"
#include "definesutility/definesutility-gcc.hpp"

/** @file
    @brief Contains useful macro and global elements shared in project */

/** @brief Return false if the given boolean is false */
#define RETURN_IF_FALSE( FUNC_RETURN )      \
    do                                      \
    {                                       \
        if(!(FUNC_RETURN))                  \
        {                                   \
            return false;                   \
        }                                   \
    } while(0)

/** @brief Do nothing if TEST resolves to true, otherwise execute optional instructions and exit
           with value false */
#define TEST_AND_RETURN_IF_FALSE( FUNC_RETURN, EXTRA_INSTRUCTIONS_IF_FALSE )    \
    do                                                                          \
    {                                                                           \
        if(!(FUNC_RETURN))                                                      \
        {                                                                       \
            EXTRA_INSTRUCTIONS_IF_FALSE                                         \
            return false;                                                       \
        }                                                                       \
    } while(0)

/** @brief Return void if the given object is null */
#define RETURN_VOID_IF_NULL( OBJECT )       \
    do                                      \
    {                                       \
        if((OBJECT) == nullptr)             \
        {                                   \
            return;                         \
        }                                   \
    } while(0)

#ifndef PRINT_WARNING
/** @brief Help to print a warning for a generic builder
    @param VA_ARGS Text with the prefix (TODO, FIXME, etc...) */
#define PRINT_WARNING(...)                              \
    _Pragma(STRINGIFY( message(#__VA_ARGS__) ))
#endif

/** @brief Create a compile-time TODO trace
    @param VA_ARG Text for the todo message
    @note This implementation accepts commas in arguments,
          at the price of not being C89 I guess.
    @see TODO_C89 */
#define TODO(...)                                       \
    PRINT_WARNING(TODO: __VA_ARGS__)

/** @brief Create a compile-time FIXME trace
    @param VA_ARG Text for the fixme message
    @note This implementation accepts commas in arguments,
          at the price of not being C89 I guess.
    @see FIXME_C89 */
#define FIXME(...)                                      \
    PRINT_WARNING(FIXME: __VA_ARGS__)

/** @brief Create a compile-time WARNING trace
    @param VA_ARG Text for the warning message
    @note This implementation accepts commas in arguments,
          at the price of not being C89 I guess.
    @see WARNING_C89 */
#define WARNING(...)                                    \
    PRINT_WARNING(__VA_ARGS__)
