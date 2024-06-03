// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pcanbusitf.hpp"

#include <QMetaEnum>

#include "import_pcanbasic.hpp"

QVector<PCanBusItf::Enum> PCanBusItf::EnumList = {};
QVector<PCanBusItf::Enum> PCanBusItf::EnumListWithoutUnknown = {};
QHash<PCanBusItf::Enum, quint16> PCanBusItf::PcanChannels = {
    { PCanBusItf::Isa0,  PCAN_ISABUS1  },
    { PCanBusItf::Isa1,  PCAN_ISABUS2  },
    { PCanBusItf::Isa2,  PCAN_ISABUS3  },
    { PCanBusItf::Isa3,  PCAN_ISABUS4  },
    { PCanBusItf::Isa4,  PCAN_ISABUS5  },
    { PCanBusItf::Isa5,  PCAN_ISABUS6  },
    { PCanBusItf::Isa6,  PCAN_ISABUS7  },
    { PCanBusItf::Isa7,  PCAN_ISABUS8  },
    { PCanBusItf::Pci0,  PCAN_PCIBUS1  },
    { PCanBusItf::Pci1,  PCAN_PCIBUS2  },
    { PCanBusItf::Pci2,  PCAN_PCIBUS3  },
    { PCanBusItf::Pci3,  PCAN_PCIBUS4  },
    { PCanBusItf::Pci4,  PCAN_PCIBUS5  },
    { PCanBusItf::Pci5,  PCAN_PCIBUS6  },
    { PCanBusItf::Pci6,  PCAN_PCIBUS7  },
    { PCanBusItf::Pci7,  PCAN_PCIBUS8  },
    { PCanBusItf::Pci8,  PCAN_PCIBUS9  },
    { PCanBusItf::Pci9,  PCAN_PCIBUS10 },
    { PCanBusItf::Pci10, PCAN_PCIBUS11 },
    { PCanBusItf::Pci11, PCAN_PCIBUS12 },
    { PCanBusItf::Pci12, PCAN_PCIBUS13 },
    { PCanBusItf::Pci13, PCAN_PCIBUS14 },
    { PCanBusItf::Pci14, PCAN_PCIBUS15 },
    { PCanBusItf::Pci15, PCAN_PCIBUS16 },
    { PCanBusItf::Usb0,  PCAN_USBBUS1  },
    { PCanBusItf::Usb1,  PCAN_USBBUS2  },
    { PCanBusItf::Usb2,  PCAN_USBBUS3  },
    { PCanBusItf::Usb3,  PCAN_USBBUS4  },
    { PCanBusItf::Usb4,  PCAN_USBBUS5  },
    { PCanBusItf::Usb5,  PCAN_USBBUS6  },
    { PCanBusItf::Usb6,  PCAN_USBBUS7  },
    { PCanBusItf::Usb7,  PCAN_USBBUS8  },
    { PCanBusItf::Usb8,  PCAN_USBBUS9  },
    { PCanBusItf::Usb9,  PCAN_USBBUS10 },
    { PCanBusItf::Usb10, PCAN_USBBUS11 },
    { PCanBusItf::Usb11, PCAN_USBBUS12 },
    { PCanBusItf::Usb12, PCAN_USBBUS13 },
    { PCanBusItf::Usb13, PCAN_USBBUS14 },
    { PCanBusItf::Usb14, PCAN_USBBUS15 },
    { PCanBusItf::Usb15, PCAN_USBBUS16 },
    { PCanBusItf::Pcc0,  PCAN_PCCBUS1  },
    { PCanBusItf::Pcc1,  PCAN_PCCBUS2  },
    { PCanBusItf::Lan0,  PCAN_LANBUS1  },
    { PCanBusItf::Lan1,  PCAN_LANBUS2  },
    { PCanBusItf::Lan2,  PCAN_LANBUS3  },
    { PCanBusItf::Lan3,  PCAN_LANBUS4  },
    { PCanBusItf::Lan4,  PCAN_LANBUS5  },
    { PCanBusItf::Lan5,  PCAN_LANBUS6  },
    { PCanBusItf::Lan6,  PCAN_LANBUS7  },
    { PCanBusItf::Lan7,  PCAN_LANBUS8  },
    { PCanBusItf::Lan8,  PCAN_LANBUS9  },
    { PCanBusItf::Lan9,  PCAN_LANBUS10 },
    { PCanBusItf::Lan10, PCAN_LANBUS11 },
    { PCanBusItf::Lan11, PCAN_LANBUS12 },
    { PCanBusItf::Lan12, PCAN_LANBUS13 },
    { PCanBusItf::Lan13, PCAN_LANBUS14 },
    { PCanBusItf::Lan14, PCAN_LANBUS15 },
    { PCanBusItf::Lan15, PCAN_LANBUS16 },
    { PCanBusItf::Unknown, PCAN_NONEBUS },
};


QString PCanBusItf::toString(Enum value)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(value)).toLower();
}

quint16 PCanBusItf::toTPCanHandle(Enum value)
{
    return PcanChannels.value(value, PCAN_NONEBUS);
}

PCanBusItf::Enum PCanBusItf::parseFromString(const QString &value)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString strValue(metaEnum.key(idx));

        if(strValue.toLower() == value.toLower())
        {
            return static_cast<Enum>(metaEnum.value(idx));
        }
    }

    return Unknown;
}

PCanBusItf::Enum PCanBusItf::parseFromUShort(quint16 value)
{
    if(value >= Unknown)
    {
        return Unknown;
    }

    return static_cast<Enum>(value);
}

const QVector<PCanBusItf::Enum> &PCanBusItf::getAllEnums(bool excludeUnknown)
{
    if(EnumList.isEmpty())
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            Enum value = static_cast<Enum>(metaEnum.value(idx));
            if(value != Unknown)
            {
                EnumListWithoutUnknown.append(value);
            }

            EnumList.append(value);
        }
    }

    if(excludeUnknown)
    {
        return EnumListWithoutUnknown;
    }

    return EnumList;
}
