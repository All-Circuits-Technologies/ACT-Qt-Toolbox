// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Base class for all the application managers
    @note A manager is like a singleton, except that the instance isn't created by the class itself
          (thanks to a static mathod) but in a derived class of the @ref AGlobalManager */
class AbstractManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class instance */
        explicit AbstractManager(QObject *parent = nullptr);
};
