// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "src/definescan.hpp"


/** @brief This is the config details for the CAN FD */
class CAN_EXPORT CanDeviceFdConfigDetails : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class contructor
            @param parent The parent instance */
        explicit CanDeviceFdConfigDetails(QObject *parent = nullptr);

        /** @brief Copy constructor
            @param copy The element to copy */
        explicit CanDeviceFdConfigDetails(const CanDeviceFdConfigDetails& copy);

        /** @brief Class destructor */
        virtual ~CanDeviceFdConfigDetails() override;

    public:
        /** @brief Get the clock frequency in MHz used to connect to CAN FD
            @return The value got */
        quint16 getFClockInMHz() const { return _fClockInMHz; }

        /** @brief Set the clock frequency in MHz to connect to CAN FD
            @param newFClockInMHz The new value to set */
        void setFClockInMHz(quint16 newFClockInMHz) { _fClockInMHz = newFClockInMHz; }

        /** @brief Get the BRP value for the nominal bit rate
            @return The value got */
        quint16 getNomBrp() const { return _nomBrp; }

        /** @brief Set the BRP value for the nominal bit rate
            @param newNomBrp The new value to set */
        void setNomBrp(quint16 newNomBrp) { _nomBrp = newNomBrp; }

        /** @brief Get the Tseg1 value for the nominal bit rate
            @return The value got */
        quint8 getNomTseg1() const { return _nomTseg1; }

        /** @brief Set the Tseg1 value for the nominal bit rate
            @param newNomTseg1 The new value to set */
        void setNomTseg1(quint8 newNomTseg1) { _nomTseg1 = newNomTseg1; }

        /** @brief Get the Tseg2 value for the nominal bit rate
            @return The value got */
        quint8 getNomTseg2() const { return _nomTseg2; }

        /** @brief Set the Tseg2 value for the nominal bit rate
            @param newNomTseg1 The new value to set */
        void setNomTseg2(quint8 newNomTseg2) { _nomTseg2 = newNomTseg2; }

        /** @brief Get the SJW value for the nominal bit rate
            @return The value got */
        quint8 getNomSjw() const { return _nomSjw; }

        /** @brief Set the SJW value for the nominal bit rate
            @param newNomTseg1 The new value to set */
        void setNomSjw(quint8 newNomSjw) { _nomSjw = newNomSjw; }

        /** @brief Get the BRP value for the data bit rate
            @return The value got */
        quint16 getDataBrp() const { return _dataBrp; }

        /** @brief Set the BRP value for the data bit rate
            @param newNomBrp The new value to set */
        void setDataBrp(quint16 newDataBrp) { _dataBrp = newDataBrp; }

        /** @brief Get the Tseg1 value for the data bit rate
            @return The value got */
        quint8 getDataTseg1() const { return _dataTseg1; }

        /** @brief Set the Tseg1 value for the data bit rate
            @param newNomTseg1 The new value to set */
        void setDataTseg1(quint8 newDataTseg1) { _dataTseg1 = newDataTseg1; }

        /** @brief Get the Tseg2 value for the data bit rate
            @return The value got */
        quint8 getDataTseg2() const { return _dataTseg2; }

        /** @brief Set the Tseg2 value for the data bit rate
            @param newNomTseg1 The new value to set */
        void setDataTseg2(quint8 newDataTseg2) { _dataTseg2 = newDataTseg2; }

        /** @brief Get the SJW value for the data bit rate
            @return The value got */
        quint8 getDataSjw() const { return _dataSjw; }

        /** @brief Set the SJW value for the data bit rate
            @param newNomTseg1 The new value to set */
        void setDataSjw(quint8 newDataSjw) { _dataSjw = newDataSjw; }

        /** @brief Test if the config is valid
            @return True if the class is valid */
        bool isValid() const;

    public:
        /** @brief Equal operator
            @param otherConfig The config to copy
            @return The current object instance */
        CanDeviceFdConfigDetails &operator=(const CanDeviceFdConfigDetails &otherConfig);

    private:
        quint16 _fClockInMHz{0};
        quint16 _nomBrp{std::numeric_limits<quint16>::max()};
        quint8 _nomTseg1{std::numeric_limits<quint8>::max()};
        quint8 _nomTseg2{std::numeric_limits<quint8>::max()};
        quint8 _nomSjw{std::numeric_limits<quint8>::max()};
        quint16 _dataBrp{std::numeric_limits<quint16>::max()};
        quint8 _dataTseg1{std::numeric_limits<quint8>::max()};
        quint8 _dataTseg2{std::numeric_limits<quint8>::max()};
        quint8 _dataSjw{std::numeric_limits<quint8>::max()};
};
