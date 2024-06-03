// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

/* ------------------------------------------------------------------
 * avisacomaccesskey.hpp - COPYRIGHT (C) 2021 ALL CIRCUITS
 * ------------------------------------------------------------------
 */

#pragma once


/** @brief This access key allows to create restriction on public methods
    @note When this type is added to public methods, it means that only the @ref AVisacom class
          can call this method */
class AVisacomAccessKey {
    friend class AVisacom;

    private:
        /** @brief Private constructor can only be used by the @ref AVisacom class */
        AVisacomAccessKey() {}
};
