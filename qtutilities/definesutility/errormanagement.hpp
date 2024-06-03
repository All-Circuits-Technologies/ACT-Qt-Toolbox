// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDebug>

#include "definesutility/definesutility.hpp"


/** @file
    @brief Contains useful macro and global elements to help the error management in the project */


/** @brief Manage the display of a log and it's adding in a QString
    @param VAR The QString to set the message into
    @param MSG The message QString or char * to set and display */
#define MANAGE_LOG_ERR(VAR, MSG)    \
    do                              \
    {                               \
        qWarning() << (MSG);        \
        VAR = MSG;                  \
    } while(0)

/** @brief Manage the display of a log and it's adding in a QString
    @param PTR_VAR A pointer to a QString variable, to set the message into, the pointer can be null
    @param MSG The message QString or char * to set and display */
#define MANAGE_PTR_LOG_ERR(PTR_VAR, MSG)    \
    do                                      \
    {                                       \
        qWarning() << (MSG);                \
        if((PTR_VAR) != nullptr)            \
        {                                   \
            (*PTR_VAR) = MSG;               \
        }                                   \
    } while(0)


/** @brief Allow to add a Warning when compiling and a log a warning if the method is called at
           runtime */
#define NOT_IMPLEMENTED()                           \
    do                                              \
    {                                               \
        TODO("Function or method not implemented")  \
        qWarning() << "Unimplemented code.";        \
    } while(0)
