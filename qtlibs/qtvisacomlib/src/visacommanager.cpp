// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "visacommanager.hpp"

#include <array>
#include "visa.h"

#include "avisacomaccesskey.hpp"
#include "avisacom.hpp"
#include "visacomasrl.hpp"
#include "visacomgpib.hpp"
#include "visacomusb.hpp"
#include "visacomgpib4881.hpp"

VisacomManager* VisacomManager::_instance = nullptr;


VisacomManager::VisacomManager(QObject *parent) : QObject(parent)
{
    _status = viOpenDefaultRM(&_defaultRM);
    if(_status < VI_SUCCESS)
    {
        //used to read error corresponding to status error
        std::array<ViChar, AVisacom::descriptionBufferSize> desc{};
        viStatusDesc(_defaultRM, _status, desc._Elems);
        qWarning() << "Could not open session to the VISA Resource Manager. " << desc._Elems;
    }
}

VisacomManager::~VisacomManager()
{
    // Clear all current instance, which will fire there destructions
    _visaComInstances.clear();

    _status = viClose(_defaultRM);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, AVisacom::descriptionBufferSize> desc{};
        viStatusDesc(_defaultRM, _status, desc._Elems);
        qWarning() << "Could not close session to the VISA Resource Manager. " << desc._Elems;
    }
}

VisacomManager &VisacomManager::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new VisacomManager();
    }

    return *_instance;
}

QSharedPointer<VisacomGpib> VisacomManager::createAndOpenGpib(quint16 gpibNumber, quint16 address)
{
    auto itfIdGenerate = [&gpibNumber, &address]() {
        return VisacomGpib::generateInterfaceId(gpibNumber, address);
    };

    auto factory = [this](const QString &interfaceId) {
        return new VisacomGpib(interfaceId, *this);
    };

    return qSharedPointerCast<VisacomGpib>(createAndOpenVisacom(itfIdGenerate, factory));
}

QSharedPointer<VisacomGpib4881> VisacomManager::createAndOpenGpib4881(quint16 gpibNumber,
                                                                      quint16 address)
{
    auto itfIdGenerate = [&gpibNumber, &address]() {
        return VisacomGpib4881::generateInterfaceId(gpibNumber, address);
    };

    auto factory = [this](const QString &interfaceId) {
        return new VisacomGpib4881(interfaceId, *this);
    };

    return qSharedPointerCast<VisacomGpib4881>(createAndOpenVisacom(itfIdGenerate, factory));
}

QSharedPointer<VisacomAsrl> VisacomManager::createAndOpenAsrl(quint16 comPort)
{

    auto itfIdGenerate = [&comPort]() {
        return VisacomAsrl::generateInterfaceId(comPort);
    };

    auto factory = [this](const QString &interfaceId) {
        return new VisacomAsrl(interfaceId, *this);
    };

    return qSharedPointerCast<VisacomAsrl>(createAndOpenVisacom(itfIdGenerate, factory));
}

QSharedPointer<VisacomUsb> VisacomManager::createAndOpenUsb(quint16 usbNumber,
                                                            quint32 vid,
                                                            quint32 pid,
                                                            const QString &serialNumber,
                                                            quint16 interfaceNumber)
{
    auto itfIdGenerate = [&usbNumber, &vid, &pid, &serialNumber, &interfaceNumber]() {
        return VisacomUsb::generateInterfaceId(usbNumber, vid, pid, serialNumber, interfaceNumber);
    };

    auto factory = [this](const QString &interfaceId) {
        return new VisacomUsb(interfaceId, *this);
    };

    return qSharedPointerCast<VisacomUsb>(createAndOpenVisacom(itfIdGenerate, factory));
}

void VisacomManager::freeAVisacom(const AVisacomAccessKey &key, const QString &interfaceId)
{
    Q_UNUSED(key)
    _visaComInstances.remove(interfaceId);
}

QSharedPointer<AVisacom> VisacomManager::createAndOpenVisacom(
                                        std::function<QString ()> generateItfId,
                                        std::function<AVisacom *(const QString &)> factoryVisacom)
{
    if(_status < VI_SUCCESS){
        qWarning() << "Visa Ressource Manager error, open visa session impossible !";
        return nullptr;
    }

    if(!_mutex.tryLock(mutexTimeout))
    {
        qWarning() << "Mutex already locked";
        return nullptr;
    }

    QString interfaceId = generateItfId();

    if(_visaComInstances.contains(interfaceId))
    {
        qWarning() << "Interface already used";
        _mutex.unlock();
        return nullptr;
    }

    QSharedPointer<AVisacom> visacom = QSharedPointer<AVisacom>(factoryVisacom(interfaceId));

    if(!visacom->open())
    {
        qWarning() << "Instrument Session not opened";
        _mutex.unlock();
        return nullptr;
    }

    _visaComInstances.insert(interfaceId, visacom.toWeakRef());

    _mutex.unlock();

    return visacom;
}

QSharedPointer<AVisacom> VisacomManager::getVisaCom(const QString &interfaceId)
{
    if(!_visaComInstances.contains(interfaceId))
    {
        qWarning() << "The interface id: " << interfaceId << ", is not known it hasn't been "
                      "setup";
        return nullptr;
    }

    const QWeakPointer<AVisacom> &weakPointer = _visaComInstances[interfaceId];

    QSharedPointer<AVisacom> pointer = weakPointer.toStrongRef();

    if(pointer == nullptr)
    {
        _visaComInstances.remove(interfaceId);
        qWarning() << "The interface id: " << interfaceId << ", was known but it has been "
                      "released";
        return nullptr;
    }

    return pointer;
}
