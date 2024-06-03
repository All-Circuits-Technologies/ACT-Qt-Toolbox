// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once


/** @brief This access key allows to create restriction on public methods
    @note When this type is added to public methods, it means that only the @ref TicHandler class
          can call this method */
class TicHandlerAccessKey {
    friend class TicHandler;

    private:
        /** @brief Private constructor can only be used by the @ref TicHandler class */
        TicHandlerAccessKey() {}
};
