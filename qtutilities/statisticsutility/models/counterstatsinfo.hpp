// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/astatsinfo.hpp"


/** @brief This allows to keep counter information for statistics purpose
    @note The counter value may be negative */
class CounterStatsInfo : public AStatsInfo
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        explicit CounterStatsInfo(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CounterStatsInfo() override;

    public:
        /** @brief Set the current counter value */
        void setValue(int newValue) { _counter = newValue; }

        /** @brief Incremment by one the current counter */
        void incrementByOneValue() { ++_counter; }

        /** @brief Decremment by one the current counter */
        void decrementByOneValue() { --_counter; }

        /** @copydoc AStatsInfo::reset
            @note Reset the counter value to the @ref DefaultCounterValue value */
        virtual void reset() override { _counter = DefaultCounterValue; }

        /** @copydoc AStatsInfo::toDisplayStringLines */
        virtual QVector<QString> toDisplayStringLines() const override;

    private:
        /** @brief The default counter value */
        static const constexpr int DefaultCounterValue = 0;

    private:
        int _counter{DefaultCounterValue};
};
