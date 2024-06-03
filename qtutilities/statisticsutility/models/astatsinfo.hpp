// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief This is the base element which contains an information for the statistics */
class AStatsInfo : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        explicit AStatsInfo(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AStatsInfo() override;

    public:
        /** @brief Reset the information and return to an init state */
        virtual void reset() = 0;

        /** @brief Returns a displayable list of lines which represents the information
            @return A displayable  */
        virtual QVector<QString> toDisplayStringLines() const = 0;
};
