// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "avisacom.hpp"

#include <array>
#include "visa.h"
#include "definesutility.hpp"

#include "avisacomaccesskey.hpp"
#include "visacommanager.hpp"
#include "visaasyncthread.hpp"

AVisacom::AVisacom(const QString &interfaceId,
                   VisacomManager &visaManager,
                   QObject *parent):
    QObject(parent),
    _visaManager(visaManager),
    _defaultRM(visaManager.getDefaultRm()),
    _interfaceId(interfaceId),
    _visaAsyncThread(new VisaAsyncThread())
{
    _visaAsyncThread->setVisacom(this);

    connect(_visaAsyncThread, &VisaAsyncThread::messageReceived,
            this,             &AVisacom::messageReceived);
    connect(_visaAsyncThread, &VisaAsyncThread::errorOccured,
            this,             &AVisacom::errorOccured);
}

AVisacom::~AVisacom()
{
    if(!close()){
        qWarning() << "Close session failed";
    }

    _visaAsyncThread->stopAndDeleteThread();
    _visaManager.freeAVisacom({}, _interfaceId);    
}

bool AVisacom::open()
{
    if(_isOpen)
    {
        //Already open, do nothing
        return true;
    }

    _status = viOpen(_defaultRM, const_cast<ViRsrc>(_interfaceId.toStdString().c_str())
                     , VI_NULL, VI_NULL, &_instr);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);

        qWarning() << "An error occurred opening session to " <<_interfaceId << " => "
                   << desc._Elems;

        return false;
    }

    _isOpen = true;

    return true;
}

bool AVisacom::lockMutex()
{
    if(!_mutex.tryLock(mutexTimeout))
    {
        qWarning() << "Mutex already locked";
        return false;
    }

    return true;
}

void AVisacom::unlockMutex()
{
    _mutex.unlock();
}

bool AVisacom::writePriv(const QByteArray &command)
{
    if(!_isOpen)
    {
        qWarning() << "Instrument session must be opened before write !";
        return false;
    }

    ViUInt32 retCount;
    ViUInt32 cmdLenght = static_cast<ViUInt32>(command.length());
    const unsigned char* castCmd= reinterpret_cast<const unsigned char *>(command.data());

    _status = viWrite(_instr, const_cast<ViBuf>(castCmd), cmdLenght, &retCount);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "Write error with command: " << command << " => " << desc._Elems;

        return false;
    }

    if (cmdLenght != retCount)
    {
        qWarning() << "Write error with command: " << command << " => Message length incorrect";

        return false;
    }

    return true;
}

bool AVisacom::readPriv(QByteArray &outputBuffer)
{
    if(!_isOpen)
    {
        qWarning() << "Instrument session must be opened before read !";
        return false;
    }

    ViUInt32 retCount;
    outputBuffer.resize(bufferSize);

    _status = viRead(_instr, reinterpret_cast<ViPBuf>(outputBuffer.data()), bufferSize, &retCount);
    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "Read error: " << desc._Elems;

        return false;
    }

    outputBuffer.resize(retCount);

    return true;
}

bool AVisacom::write(const QByteArray &command)
{
    RETURN_IF_FALSE(lockMutex());

    if(!writePriv(command))
    {
        unlockMutex();
        return false;
    }

    unlockMutex();

    return true;
}

bool AVisacom::write(const QString &command)
{
    return write(command.toLatin1());
}

bool AVisacom::write(const QVector<QString> &command)
{
    QString cmdList;
    for(int i=0; i<command.length(); i++)
    {
        cmdList.append(command.at(i)).append(lineFeed);
    }

    return write(cmdList.toLatin1());
}

bool AVisacom::asyncWrite(const QByteArray &cmd)
{
    return _visaAsyncThread->asyncWrite(cmd);
}

bool AVisacom::asyncWrite(const QString &cmd)
{
    return _visaAsyncThread->asyncWrite(cmd.toLatin1());
}

bool AVisacom::asyncQuery(const QByteArray &cmd)
{
    return _visaAsyncThread->asyncQuery(cmd);
}

bool AVisacom::asyncQuery(const QString &cmd)
{
    return _visaAsyncThread->asyncQuery(cmd.toLatin1());
}

bool AVisacom::read(QByteArray &outputBuffer)
{
    RETURN_IF_FALSE(lockMutex());

    if(!readPriv(outputBuffer))
    {
        unlockMutex();
        return false;
    }

    unlockMutex();

    return true;
}

bool AVisacom::read(QString &outputBuffer)
{
    QByteArray ba_buffer;

    RETURN_IF_FALSE(read(ba_buffer));

    outputBuffer = QString::fromLatin1(ba_buffer);

    return true;
}

bool AVisacom::close()
{
    if(!_isOpen)
    {
        // If no sessions are open => close() return true
        return true;
    }

    _status = viClose(_instr);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_defaultRM, _status, desc._Elems);
        qWarning() << "Could not close Instrument session => " << desc._Elems;

        return false;
    }
    _isOpen = false;

    return true;
}

bool AVisacom::clear()
{
    if(!_isOpen)
    {
        qWarning() << "Instrument session must be opened before clear !";
        return false;
    }

    _status = viClear(_instr);
    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_defaultRM, _status, desc._Elems);
        qWarning() << "Could not clear Instrument session => " << desc._Elems;

        return false;
    }

    return true;
}

bool AVisacom::setTimeout(qint32 timeout)
{
    if(timeout < 0)
    {
        return setAttribute(VI_ATTR_TMO_VALUE, static_cast<quint32>(VI_TMO_INFINITE));
    }

    return setAttribute(VI_ATTR_TMO_VALUE, timeout);
}

bool AVisacom::getTimeout(quint32 &timeout)
{
    ViUInt32 tmo;

    _status = viGetAttribute (_instr, VI_ATTR_TMO_VALUE, &tmo);
    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc;
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "An error occurred getting timeout: " << desc._Elems;

        return false;
    }

    timeout = static_cast<quint32>(tmo);

    return true;
}

unsigned long AVisacom::getInstrumentSession()
{
    return _instr;
}

/*********************************/
/*          SetAttribute         */
/*********************************/

bool AVisacom::setAttribute(quint32 attr, quint64 value)
{
    if(!_isOpen){
        qWarning() << "Instrument session must be opened before setting attributes !";
        return false;
    }

    _status = viSetAttribute(_instr, attr, value);
    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc;
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "An error occurred setting attribute: " << desc._Elems;

        return false;
    }

    return true;
}

bool AVisacom::setAttribute(quint32 attr, qint32 value)
{
    if(value < 0)
    {
        qWarning() << "can't set negative attribute value";
        return false;
    }

    return setAttribute(attr, static_cast<quint64>(value));
}

bool AVisacom::setAttribute(quint32 attr, quint32 value)
{
    return setAttribute(attr, static_cast<quint64>(value));
}

bool AVisacom::setAttribute(quint32 attr, quint16 value)
{
    return setAttribute(attr, static_cast<quint64>(value));
}

bool AVisacom::setAttribute(quint32 attr, bool value)
{
    return setAttribute(attr, static_cast<quint64>(value));
}

/*********************************/
/*          GetAttribute         */
/*********************************/

bool AVisacom::getAttributePriv(quint32 attr, void *output)
{
    if(!_isOpen)
    {
        qWarning() << "Instrument session must be opened before getting attributes !";
        return false;
    }

    _status = viGetAttribute(_instr, attr, &output);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "An error occurred getting attribute: " << desc._Elems;

        return false;
    }

    return true;
}

bool AVisacom::getAttribute(quint32 attr, quint64 &outputAttr)
{
    return getAttributePriv(attr, &outputAttr);
}

bool AVisacom::getAttribute(quint32 attr, quint32 &outputAttr)
{
    return getAttributePriv(attr, &outputAttr);
}

bool AVisacom::getAttribute(quint32 attr, quint16 &outputAttr)
{
    return getAttributePriv(attr, &outputAttr);
}

bool AVisacom::getAttribute(quint32 attr, bool &outputAttr)
{
    return getAttributePriv(attr, &outputAttr);
}
