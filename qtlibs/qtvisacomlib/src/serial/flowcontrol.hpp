// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include "src/visacomglobal.hpp"


/** @brief List of the value flow control */
class VISACOM_EXPORT FlowControl : public QObject
{
    Q_OBJECT

    public:
        enum Enum:quint16
        {
            NoFlowControl = 0,  /*VI_ASRL_FLOW_NONE (0)*/
            XonXOff = 1,        /*VI_ASRL_FLOW_XON_XOFF (1)*/
            RtsCts = 2,         /*VI_ASRL_FLOW_RTS_CTS (2)*/
            DtrDsr = 4,          /*VI_ASRL_FLOW_DTR_DSR (4)*/
            Unknown = std::numeric_limits<quint16>::max()
        };

        Q_ENUM(Enum)

    public:
        /** @brief Parse the enum from string
            @param strValue The string to parse
            @return The enum parsed */
        static Enum parseFromString(const QString &strValue);

        /** @brief Return a string representation of the Flow Control
            @param flowControl The type to get the string representation from
            @return The string representation */
        static QString toString(Enum flowControl);

        /** @brief Get all the flowcontrol enum values
            @return The list of memory types */
        static const QVector<Enum> &getAllEnums();

    private:
        static QVector<Enum> enumList;
};
