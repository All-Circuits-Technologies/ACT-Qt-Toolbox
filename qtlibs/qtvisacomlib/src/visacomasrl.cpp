// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "visacomasrl.hpp"

#include "visa.h"


VisacomAsrl::VisacomAsrl(const QString &interfaceId,
                         VisacomManager &visaManager,
                         QObject *parent):
    AVisacom(interfaceId, visaManager, parent)
{
}

QString VisacomAsrl::generateInterfaceId(quint16 comPort)
{
    return QString(asrlItf).arg(comPort);
}

/********************
 *      Setter      *
 ********************/

bool VisacomAsrl::setBaudrate(quint32 baud)
{
    return AVisacom::setAttribute(VI_ATTR_ASRL_BAUD, baud);
}

bool VisacomAsrl::setDataBits(quint16 dataBits)
{
    return AVisacom::setAttribute(VI_ATTR_ASRL_DATA_BITS, dataBits);
}

bool VisacomAsrl::setFlowControl(FlowControl::Enum flowcontrol)
{
    return AVisacom::setAttribute(VI_ATTR_ASRL_FLOW_CNTRL, flowcontrol);
}

bool VisacomAsrl::setParity(Parity::Enum parity)
{
    return AVisacom::setAttribute(VI_ATTR_ASRL_PARITY, parity);
}

bool VisacomAsrl::setStopBits(StopBits::Enum stopBits)
{
    return AVisacom::setAttribute(VI_ATTR_ASRL_STOP_BITS, stopBits);
}

/********************
 *      Getter      *
 ********************/

bool VisacomAsrl::getBaudrate(quint32 &baud)
{
    return AVisacom::getAttribute(VI_ATTR_ASRL_BAUD, baud);
}

bool VisacomAsrl::getDataBits(quint16 &dataBits)
{
    return AVisacom::getAttribute(VI_ATTR_ASRL_DATA_BITS, dataBits);
}

bool VisacomAsrl::getFlowControl(quint16 &flowcontrol)
{
    return AVisacom::getAttribute(VI_ATTR_ASRL_FLOW_CNTRL, flowcontrol);
}

bool VisacomAsrl::getParity(quint16 &parity)
{
    return AVisacom::getAttribute(VI_ATTR_ASRL_PARITY, parity);
}

bool VisacomAsrl::getStopBits(quint16 &stopBits)
{
    return AVisacom::getAttribute(VI_ATTR_ASRL_STOP_BITS, stopBits);
}
