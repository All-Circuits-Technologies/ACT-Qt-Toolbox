// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "visacomgpib4881.hpp"

#include <array>
#include "visa.h"
#include "definesutility.hpp"
#include "concurrent/threadconcurrentrun.hpp"


VisacomGpib4881::VisacomGpib4881(const QString &interfaceId,
                                 VisacomManager &visaManager,
                                 QObject *parent) :
    VisacomGpib(interfaceId, visaManager, parent)
{
    initTimeout();
}

VisacomGpib4881::~VisacomGpib4881()
{
    close();
}

bool VisacomGpib4881::open()
{
    if(!VisacomGpib::open())
    {
        qWarning() << "An error occured while opening instrument session !";
        return false;
    }

    _instr = getInstrumentSession();

    if(!installHandler())
    {
        qWarning() << "An error occured while initializing event handlers !";
        return false;
    }

    return true;
}

bool VisacomGpib4881::close()
{
    if(!uninstallHandler())
    {
        qWarning() << "An error occured while disabling event handlers !";
        return false;
    }

    if(!VisacomGpib::close())
    {
        qWarning() << "An error occured while closing instrument session !";
        return false;
    }

    return true;
}

void VisacomGpib4881::initTimeout()
{
    _timer = new QTimer(this);
    _timer->setSingleShot(true);

    connect(_timer, &QTimer::timeout, this, &VisacomGpib4881::onTimeout);
}

bool VisacomGpib4881::setTimeout(qint32 timeout)
{
    RETURN_IF_FALSE(AVisacom::setTimeout(timeout));

    _tmoValue = timeout;

    return true;
}

bool VisacomGpib4881::installHandler()
{
    _status = viInstallHandler(_instr, VI_EVENT_SERVICE_REQ, queryCallback, this);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << " SRQ Event handler init error" << " => " << desc._Elems;

        return false;
    }

    _status = viInstallHandler(_instr, VI_EVENT_IO_COMPLETION, queryCallback, this);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "IO Event handler init error" << " => " << desc._Elems;

        return false;
    }

    return true;
}

bool VisacomGpib4881::uninstallHandler()
{
    _status = viUninstallHandler(_instr, VI_EVENT_SERVICE_REQ, queryCallback, this);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "Uninstall Handler error" << " => " << desc._Elems;

        return false;
    }

    _status = viUninstallHandler(_instr, VI_EVENT_IO_COMPLETION, queryCallback, this);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "Uninstall Handler error" << " => " << desc._Elems;

        return false;
    }

    return true;
}

bool VisacomGpib4881::enableEvent()
{
    _status = viEnableEvent(_instr, VI_EVENT_SERVICE_REQ, VI_HNDLR, VI_NULL);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "Enable SERVICE REQUEST Event error" << " => " << desc._Elems;

        return false;
    }

    _status = viEnableEvent(_instr, VI_EVENT_IO_COMPLETION, VI_HNDLR, VI_NULL);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "Enable IO_COMPLETION Event error" << " => " << desc._Elems;

        return false;
    }

    return true;
}

bool VisacomGpib4881::disableEvent()
{
    _status = viDisableEvent(_instr, VI_ALL_ENABLED_EVENTS, VI_ALL_MECH);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "Disable Event error" << " => " << desc._Elems;

        return false;
    }

    return true;
}

bool VisacomGpib4881::deactivateRead()
{
    qWarning() << "Synchronous read deactivated: "
               << "If synchronous read used, asynchronous read won't worked correctly";

    return false;
}

bool VisacomGpib4881::read(QByteArray &outputBuffer)
{
    Q_UNUSED(outputBuffer);
    return deactivateRead();
}

bool VisacomGpib4881::read(QString &outputBuffer)
{
    Q_UNUSED(outputBuffer);
    return deactivateRead();
}

bool VisacomGpib4881::lockFunction()
{
    if(!_isFunctionUsed){
        _isFunctionUsed = true;
        return true;
    }

    qWarning() << "Function already locked";
    return false;
}

void VisacomGpib4881::unlockFunction()
{
    if(!_isFunctionUsed){
        qWarning() << "Function already unlocked";
    }
    _isFunctionUsed = false;
}

bool VisacomGpib4881::asyncQuery(const QByteArray &cmd)
{
    RETURN_IF_FALSE(lockFunction());
    RETURN_IF_FALSE(enableEvent());

    _buffer.resize(bufferSize);
    _asyncQueryState = Writing;

    QElapsedTimer elapsedTimer;
    elapsedTimer.start();

    // This function will generate the VI_EVENT_SERVICE_REQ event
    RETURN_IF_FALSE(writePriv(cmd));

    _timer->start(_tmoValue - elapsedTimer.elapsed());

    return true;
}

bool VisacomGpib4881::asyncQuery(const QString &cmd)
{
    return asyncQuery(cmd.toLatin1());
}

bool VisacomGpib4881::manageSRQEvent(ViSession session)
{
    ViUInt16 stb;
    // This function will read the status byte of the SRQ
    // Must be used to check if the SRQ is correct
    _status = viReadSTB(session, &stb);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(session, _status, desc._Elems);
        qWarning() << "Read STB error " << " => " << desc._Elems;

        manageError();

        return false;
    }

    _asyncQueryState = Reading;

    // This function will generate the VI_EVENT_IO_COMPLETION event
    _status = viReadAsync(session, reinterpret_cast<ViPBuf>(_buffer.data()), bufferSize, &_jobID);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(session, _status, desc._Elems);
        qWarning() << "Asynchronous read error " << " => " << desc._Elems;

        manageError();

        return false;
    }

    return true;
}

bool VisacomGpib4881::manageIOEvent(ViSession session, ViEvent event)
{
    //In case of Reading timeout, the terminateOperation() will generate an IO Event and the
    //_terminateEvent will also be true.
    //To end the asynchronous operation properly manageError() will be called and manageIOEvent
    //will return false to skip the next operations
    if(_asyncQueryState == Terminating){
        manageError();
        return false;
    }

    //If !_terminateEvent, the function will try to get the size of the buffer
    ViUInt32 retCount;
    _status = viGetAttribute(event, VI_ATTR_RET_COUNT, &retCount);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(session, _status, desc._Elems);
        qWarning() << "Get attribute error" << " => " << desc._Elems;

        manageError();
        return false;
    }

    //If no error has occured, the function will resizes the buffer to the correct size,
    //ends the asynchronous operation properly and then emits a success signal
   _buffer = _buffer.left(retCount);

   disableEvent();
   unlockFunction();
   emit messageReceived(_buffer);

   _timer->stop();

   return true;
}

signed long VisacomGpib4881::queryCallback(unsigned long session,
                                          unsigned long eventType,
                                          unsigned long event,
                                          void *userHandle)
{
    VisacomGpib4881* gpib4881 = static_cast<VisacomGpib4881 *>(userHandle);
    if(gpib4881 == nullptr)
    {
        qWarning() << "An error occured : VisacomGpib4881 instance = nullptr";
        return _VI_ERROR;
    }

    if(eventType == VI_EVENT_SERVICE_REQ)
    {
        if(!ThreadConcurrentRun::run(*gpib4881, &VisacomGpib4881::manageSRQEvent, session))
        {
            qWarning() << "An error occured running SRQ event manager";
            return _VI_ERROR;
        }
    }
    else if(eventType == VI_EVENT_IO_COMPLETION)
    {
        if(!ThreadConcurrentRun::run(*gpib4881, &VisacomGpib4881::manageIOEvent, session, event))
        {
            qWarning() << "An error occured running IO event manager";
            return _VI_ERROR;
        }
    }
    else
    {
        qWarning() << "Unknown event occured";
        return _VI_ERROR;
    }

    return VI_SUCCESS;
}

void VisacomGpib4881::onTimeout()
{
    switch (_asyncQueryState) {
        case Writing:
            manageError();
        break;

        case Reading:
            _asyncQueryState = Terminating;
        break;

        case Terminating:
            if(!terminateOperation())
            {
                qWarning() << "Failed to terminate reading operation";
            }

        default:
            qWarning() << "Unknown timout state !";
    }
}

bool VisacomGpib4881::terminateOperation()
{
    _status = viTerminate(_instr, VI_NULL, _jobID);

    if (_status < VI_SUCCESS)
    {
        std::array<ViChar, descriptionBufferSize> desc{};
        viStatusDesc(_instr, _status, desc._Elems);
        qWarning() << "Terminate failed" << " => " << desc._Elems;

        return false;
    }

    return true;
}

void VisacomGpib4881::manageError()
{
    if(_timer->isActive())
    {
        _timer->stop();
    }
    disableEvent();
    unlockFunction();
    emit errorOccured();
}
