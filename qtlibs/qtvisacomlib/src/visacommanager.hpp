// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDebug>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QSharedPointer>

#include "visacomglobal.hpp"

class AVisacom;
class AVisacomAccessKey;
class VisacomGpib;
class VisacomAsrl;
class VisacomUsb;
class VisacomGpib4881;


/** @brief Useful class used to manage visa communication and instrument session */
class VISACOM_EXPORT VisacomManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit VisacomManager(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~VisacomManager() override;

    public:
        /** @brief Return the singleton instance of the Visacom manager
            @note If no instance created, getInsantce force Visacom manager instance creation
            @return The Visacom manager instance */
        static VisacomManager &getInstance();

        /** @brief Return a pointer to the instance of AVisacom
            @param interfaceId a token to return the corresponding AVisacom instance
            @note The function must be cast to a derived class of AVisacom
            @return A pointer to AVisacom instance */
        QSharedPointer<AVisacom> getVisaCom(const QString &interfaceId);

        /** @brief Return a pointer to the instance of AVisacom
            @param interfaceId a token to return the corresponding AVisacom instance
            @note The class template must be base on AVisacom
            @return A pointer to AVisacom instance */
        template<class T>
        QSharedPointer<T> getVisaComExt(const QString &interfaceId);

        /** @brief Generate VisaComGpib instance and open Instrument session
            @param gpibNumber define the gpib board/controller used
            @param address the gpib address defined on the instrument
            @note Interface: [  GPIB[gpibNumber]::[address]::INSTR   ]
            @return A pointer to VisacomGpib instance */
        QSharedPointer<VisacomGpib> createAndOpenGpib(quint16 gpibNumber, quint16 address);

        /** @brief Generate PulseBenchGpib instance and open Instrument session
            @param gpibNumber define the gpib board/controller used
            @param address the gpib address defined on the instrument
            @note Interface: [  GPIB[gpibNumber]::[address]::INSTR   ]
            @return A pointer to VisacomGpib instance */
        QSharedPointer<VisacomGpib4881> createAndOpenGpib4881(quint16 gpibNumber,
                                                                   quint16 address);

        /** @brief Generate VisaComAsrl instance and open Instrument session
            @param comPort the serial com port defined on the instrument
            @note Interface: [  ASRL[comPort]::INSTR   ]
            @return A pointer to VisacomAsrl instance */
        QSharedPointer<VisacomAsrl> createAndOpenAsrl(quint16 comPort);

        /** @brief Generate VisaComUsb instance and open Instrument session
            @param usbNumber define the usb board/controller used
            @param vid the vid of the instrument used
            @param pid the pid of the instrument used
            @param serialNumber the serial number of the instrument used
            @param interfaceNumber the usb interface defined on the instrument
            @note Interface: [  USB[usbNumber]::pid::vid::serialNumber::interfaceNumber::INSTR   ]
            @return A pointer to VisacomUsb instance */
        QSharedPointer<VisacomUsb> createAndOpenUsb(quint16 usbNumber,
                                                    quint32 vid,
                                                    quint32 pid,
                                                    const QString &serialNumber,
                                                    quint16 interfaceNumber);

        /** @brief Return the resource manager session */
        unsigned long getDefaultRm() const { return _defaultRM; }

    public:
        /** @brief Called by the AVisacom when it's destroyed, in order to free the QHash instances
            @param key This access key prevents other classes than @ref AVisaCom to use this
                       method
            @param interfaceId The unique token linked to the AVisacom class */
        void freeAVisacom(const AVisacomAccessKey &key, const QString &interfaceId);

    private:
        /** @brief Generate a visa com instance and open Instrument session
            @note The visa com object is generated thanks to the factory function given
            @param generateItfId This function is used to generate an interface id
            @param factoryVisacom Factory method to generated a AVisaCom, the method has to return
                                  the visa com created and takes the interface id previously
                                  generated
            @return The generated visa com object */
        QSharedPointer<AVisacom> createAndOpenVisacom(
                                        std::function<QString()> generateItfId,
                                        std::function<AVisacom*(const QString&)> factoryVisacom);

    private:
        static VisacomManager *_instance;

        static const constexpr qint32 mutexTimeout = 5000;

    private:
        QHash<QString, QWeakPointer<AVisacom>> _visaComInstances;

        QMutex _mutex;

        /** @brief Visa ressource manager session for default visa communication */
        /*ViSession*/ unsigned long  _defaultRM{0};

        /** @brief Visa function status (status<0 = function error) */
        /*ViStatus*/  qint32  _status{0};
};

template<class T>
QSharedPointer<T> VisacomManager::getVisaComExt(const QString &interfaceId)
{
    static_assert (std::is_base_of<AVisacom, T>(), "The class given isn't based on AVisacom");

    return qSharedPointerObjectCast<T>(getVisaCom(interfaceId));
}
