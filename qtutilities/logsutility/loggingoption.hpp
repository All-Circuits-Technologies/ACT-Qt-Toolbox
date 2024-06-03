// SPDX-FileCopyrightText: 2020 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QDebug>
#include <QHash>


/** @brief Logging option for log strategies action */
class LoggingOption : public QObject
{
    Q_OBJECT

    public:
        /** @brief The meaning of the enum can vary depending of the log strategy */
        enum Enum : quint8
        {
            Other   = 0x00,
            Id      = 0x01,
            AtStart = 0x02
        };
        Q_ENUM(Enum)

    public:
        /** @brief Register meta type linked to this enum */
        static void RegisterMetaTypes();

        /** @brief Get a string representation of the enum
            @param value The enum value to stringify
            @return The string representation of the enum */
        static QString toString(Enum value);

        /** @brief Get the value of the LoggingOption::AtStart from the options parameter
            @param options The LoggingOptions to read
            @param value The value got from the Logging options
            @param canBeEmpty If false and if the option is not found, the function returns an
                              error.
                              If true and if the option is not found, the function returns ok and
                              the boolean value will be equal to the value given
            @return False if an error occurs or if the option hasn't been found */
        static bool isAtStart(const QHash<LoggingOption::Enum, QVariant> &options,
                              bool &value,
                              bool canBeEmpty = false);

        /** @brief Get the value of the LoggingOption::Id from the options parameter
            @param options The LoggingOptions to read
            @param value The value got from the Logging options
            @param canBeEmpty If false and if the option is not found, the function returns an
                              error.
                              If true and if the option is not found, the function returns ok and
                              the boolean value will be equal to the value given
            @return False if an error occurs or if the option hasn't been found */
        static bool getId(const QHash<LoggingOption::Enum, QVariant> &options,
                          QString &value,
                          bool canBeEmpty = false);

    private:
        /** @brief Get the value of an option from the options parameter
            @param options The LoggingOptions to read
            @param option The logging option to get its value
            @param value The value got from the Logging options
            @param canBeEmpty If false and if the option is not found, the function returns an
                              error.
                              If true and if the option is not found, the function returns ok and
                              the boolean value will be equal to the value given
            @return False if an error occurs or if the option hasn't been found */
        template<class T>
        static bool getOptionValue(const QHash<LoggingOption::Enum, QVariant> &options,
                                   LoggingOption::Enum option,
                                   T &value,
                                   bool canBeEmpty);
};

template<class T>
bool LoggingOption::getOptionValue(const QHash<LoggingOption::Enum, QVariant> &options,
                                   LoggingOption::Enum option,
                                   T &value,
                                   bool canBeEmpty)
{
    if(!options.contains(option))
    {
        if(!canBeEmpty)
        {
            qWarning() << "The " << toString(option) << " logging option is not present and it was "
                       << "required";
            return false;
        }

        return true;
    }

    QVariant tmpValue = options[option];

    if(!tmpValue.canConvert<T>())
    {
        qWarning() << "The option: " << toString(option) << ", value has the wrong type, can't "
                   << "convert it";
        return false;
    }

    value = tmpValue.value<T>();

    return true;
}

using LoggingOptions = QHash<LoggingOption::Enum, QVariant>;
