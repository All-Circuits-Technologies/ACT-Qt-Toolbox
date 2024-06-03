// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

/** @brief This namespace contains serial-link lib constants */
namespace SerialLibConstants
{
    namespace Debug
    {
        /** @brief Should serial activity be printed on debug channel
            @note Developers may want to set it to true while working on this plugin */

#ifdef ACT_BUILD_RELEASE
        constexpr const bool Stream = false;
#else
        constexpr const bool Stream = true;
#endif
    }

    namespace Defaults
    {
        /** @brief Timeout of the mutex used in the serial lib in milliseconds */
        const constexpr int MutexTimeoutInMs = 5000;
    }
}
