// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>

#include "src/definescan.hpp"


/** @brief This is the key of PEAK CAN interface
    @note This describes the interface and channel used */
class CAN_EXPORT PCanBusItf : public QObject
{
    Q_OBJECT

    public:
        /** @brief The PEAK CAN bus interface enum */
        enum Enum {
            Isa0,
            Isa1,
            Isa2,
            Isa3,
            Isa4,
            Isa5,
            Isa6,
            Isa7,
            Pci0,
            Pci1,
            Pci2,
            Pci3,
            Pci4,
            Pci5,
            Pci6,
            Pci7,
            Pci8,
            Pci9,
            Pci10,
            Pci11,
            Pci12,
            Pci13,
            Pci14,
            Pci15,
            Usb0,
            Usb1,
            Usb2,
            Usb3,
            Usb4,
            Usb5,
            Usb6,
            Usb7,
            Usb8,
            Usb9,
            Usb10,
            Usb11,
            Usb12,
            Usb13,
            Usb14,
            Usb15,
            Pcc0,
            Pcc1,
            Lan0,
            Lan1,
            Lan2,
            Lan3,
            Lan4,
            Lan5,
            Lan6,
            Lan7,
            Lan8,
            Lan9,
            Lan10,
            Lan11,
            Lan12,
            Lan13,
            Lan14,
            Lan15,
            Unknown
        };
        Q_ENUM(Enum)

    public:
        /** @brief Get a string representation of the enum
            @param value The value to stringify
            @return The stringified value */
        static QString toString(Enum value);

        /** @brief Get the TPCANHandle value linked to the CAN BUS interface key
            @note The handle has an ushort format, that why we return a quint16
            @param value The value to get the handle from
            @return The handle value */
        static quint16 toTPCanHandle(Enum value);

        /** @brief Parse the CAN bus interface key from the stringified value
            @param value The value to parse
            @return The CAN bus interface key parsed, returns Unknown if the parse has failed */
        static Enum parseFromString(const QString &value);

        /** @brief Parse the CAN bus interface key from the TPCANHandle value
            @note The handle has an ushort format, that why we return a quint16
            @param value The value to parse
            @return The CAN bus interface key parsed, returns Unknown if the parse has failed */
        static Enum parseFromUShort(quint16 value);

        /** @brief Get the enums list of all the elements
            @param excludeUnknown If true, the method will return all the enum excepts the Unknown
                                  value.
                                  If false, the method will return all the enums including the
                                  Unknown value
            @return The enum list */
        static const QVector<Enum> &getAllEnums(bool excludeUnknown = false);

    private:
        static QVector<Enum> EnumList;
        static QVector<Enum> EnumListWithoutUnknown;
        static QHash<Enum, quint16> PcanChannels;
};
