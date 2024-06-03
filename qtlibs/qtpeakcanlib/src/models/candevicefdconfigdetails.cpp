// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "candevicefdconfigdetails.hpp"


CanDeviceFdConfigDetails::CanDeviceFdConfigDetails(QObject *parent)
    : QObject{parent}
{
}

CanDeviceFdConfigDetails::CanDeviceFdConfigDetails(const CanDeviceFdConfigDetails &copy) :
    _fClockInMHz{copy._fClockInMHz},
    _nomBrp{copy._nomBrp},
    _nomTseg1{copy._nomTseg1},
    _nomTseg2{copy._nomTseg2},
    _nomSjw{copy._nomSjw},
    _dataBrp{copy._dataBrp},
    _dataTseg1{copy._dataTseg1},
    _dataTseg2{copy._dataTseg2},
    _dataSjw{copy._dataSjw}
{
}

CanDeviceFdConfigDetails::~CanDeviceFdConfigDetails()
{
}

bool CanDeviceFdConfigDetails::isValid() const
{
    return (_fClockInMHz != 0) &&
           (_nomBrp != std::numeric_limits<quint16>::max()) &&
           (_nomTseg1 != std::numeric_limits<quint8>::max()) &&
           (_nomTseg2 != std::numeric_limits<quint8>::max()) &&
           (_nomSjw != std::numeric_limits<quint8>::max()) &&
           (_dataBrp != std::numeric_limits<quint16>::max()) &&
           (_dataTseg1 != std::numeric_limits<quint8>::max()) &&
           (_dataTseg2 != std::numeric_limits<quint8>::max()) &&
           (_dataSjw != std::numeric_limits<quint8>::max());
}

CanDeviceFdConfigDetails &CanDeviceFdConfigDetails::operator=(
                                                        const CanDeviceFdConfigDetails &otherConfig)
{
    _fClockInMHz    = otherConfig._fClockInMHz;
    _nomBrp         = otherConfig._nomBrp;
    _nomTseg1       = otherConfig._nomTseg1;
    _nomTseg2       = otherConfig._nomTseg2;
    _nomSjw         = otherConfig._nomSjw;
    _dataBrp        = otherConfig._dataBrp;
    _dataTseg1      = otherConfig._dataTseg1;
    _dataTseg2      = otherConfig._dataTseg2;
    _dataSjw        = otherConfig._dataSjw;

    return *this;
}
