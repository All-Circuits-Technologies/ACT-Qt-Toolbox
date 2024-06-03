// SPDX-FileCopyrightText: 2023 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "candeviceintf.hpp"

#include "definesutility/definesutility.hpp"
#include "threadutility/concurrent/threadconcurrentrun.hpp"

#include "src/candevice/candevice.hpp"
#include "src/candevice/candevicethread.hpp"
#include "src/models/expectedcanframemask.hpp"


CanDeviceIntf::CanDeviceIntf(const CanDeviceConfig &config, QObject *parent)
    : QObject{parent},
    _config(config),
    _canDeviceThread(new CanDeviceThread())
{
}

CanDeviceIntf::~CanDeviceIntf()
{
    // We first unitialize the device before stopping and deleting the thread
    unInitialize();
    _canDeviceThread->stopAndDeleteThread();
}

bool CanDeviceIntf::initDevice()
{
    RETURN_IF_FALSE(_canDeviceThread->createCanDevice(_config));

    CanDevice *device = accessDeviceThroughThread(QStringLiteral("initialize the CAN device"));

    if(device == nullptr)
    {
        return false;
    }

    connect(device, &CanDevice::framesReceived,
            this,   &CanDeviceIntf::framesReceived, Qt::UniqueConnection);
    
    return ThreadConcurrentRun::run(*device, &CanDevice::initialize);
}

bool CanDeviceIntf::unInitialize()
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("uninitialize the CAN device"));

    if(device == nullptr)
    {
        return false;
    }

    RETURN_IF_FALSE(ThreadConcurrentRun::run(*device, &CanDevice::unInitialize));

    disconnect(device, &CanDevice::framesReceived, this, &CanDeviceIntf::framesReceived);

    return true;
}

bool CanDeviceIntf::isInitialized() const
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("test if the CAN device is "
                                                                 "initialized"));

    if(device == nullptr)
    {
        return false;
    }

    return ThreadConcurrentRun::run(*device, &CanDevice::isInitialized);
}

bool CanDeviceIntf::getParamBusOffAutoReset(bool &autoReset)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("get param bus off auto reset "
                                                                 "value"));

    if(device == nullptr)
    {
        return false;
    }

    return ThreadConcurrentRun::run(*device, &CanDevice::getParamBusOffAutoReset, &autoReset);
}

bool CanDeviceIntf::setParamBusOffAutoReset(bool autoReset)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("set param bus off auto reset "
                                                                 "value"));

    if(device == nullptr)
    {
        return false;
    }

    return ThreadConcurrentRun::run(*device, &CanDevice::setParamBusOffAutoReset, autoReset);
}

bool CanDeviceIntf::write(const QCanBusFrame &frame)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("write a frame"));

    if(device == nullptr)
    {
        return false;
    }

    return ThreadConcurrentRun::run(*device, &CanDevice::write, frame);
}

QVector<QCanBusFrame> CanDeviceIntf::writeAndWaitAnswer(
    const QCanBusFrame &frame,
    const ExpectedCanFrameMask &expectedFrameMask,
    int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("write a frame and wait its "
                                                                 "answer"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::writeAndWaitAnswer,
                                    frame,
                                    expectedFrameMask,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::writeAndWaitAnswerById(const QCanBusFrame &frame,
                                                            quint32 answerId,
                                                            int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("write a frame and wait its "
                                                                 "answer by id"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::writeAndWaitAnswerById,
                                    frame,
                                    answerId,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::writeAndWaitOneOfAnswers(
    const QCanBusFrame &frame,
    const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
    int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("write a frame and wait one the "
                                                                 "given answers"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::writeAndWaitOneOfAnswers,
                                    frame,
                                    expectedFrameMasks,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::writeAndWaitOneOfAnswersByIds(
    const QCanBusFrame &frame,
    const QVector<quint32> &answersIds,
    int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("write a frame and wait one the "
                                                                 "given answers by ids"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::writeAndWaitOneOfAnswersByIds,
                                    frame,
                                    answersIds,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::writeAndWaitAllTheAnswers(
    const QCanBusFrame &frame,
    const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
    int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("write a frame and wait all the "
                                                                 "given answers"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::writeAndWaitAllTheAnswers,
                                    frame,
                                    expectedFrameMasks,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::writeAndWaitAllTheAnswersByIds(
    const QCanBusFrame &frame,
    const QVector<quint32> &answersIds,
    int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("write a frame and wait all the "
                                                                 "given answers by ids"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::writeAndWaitAllTheAnswersByIds,
                                    frame,
                                    answersIds,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::waitCanMsg(const ExpectedCanFrameMask &expectedFrameMask,
                                                int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("Wait a specific frame"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::waitCanMsg,
                                    expectedFrameMask,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::waitCanMsgById(quint32 answerId, int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("Wait a specific frame by id"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::waitCanMsgById,
                                    answerId,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::waitOneOfTheCanMsgs(
    const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
    int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("Wait one of the specific "
                                                                 "frames"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::waitOneOfTheCanMsgs,
                                    expectedFrameMasks,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::waitOneOfTheCanMsgsByIds(const QVector<quint32> &answersIds,
                                                              int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("Wait one of the specific "
                                                                 "frames by ids"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::waitOneOfTheCanMsgsByIds,
                                    answersIds,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::waitAllTheCanMsgs(
    const QVector<ExpectedCanFrameMask> &expectedFrameMasks,
    int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("Wait all the specific frames"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::waitAllTheCanMsgs,
                                    expectedFrameMasks,
                                    timeoutInMs);
}

QVector<QCanBusFrame> CanDeviceIntf::waitAllTheCanMsgsByIds(const QVector<quint32> &answersIds,
                                                            int timeoutInMs)
{
    CanDevice *device = accessDeviceThroughThread(QStringLiteral("Wait all the specific frames by "
                                                                 "ids"));

    if(device == nullptr)
    {
        return {};
    }

    return ThreadConcurrentRun::run(*device,
                                    &CanDevice::waitAllTheCanMsgsByIds,
                                    answersIds,
                                    timeoutInMs);
}

CanDevice *CanDeviceIntf::accessDeviceThroughThread(const QString &action) const
{
    if(!_canDeviceThread->isValid())
    {
        qWarning() << "Failed to: " << action << ", with the CAN device: "
                   << _config.getCanBusItfName() << ", the linked thread isn't valid, may be the "
                   << "thread hasn't be initialized or it's stopped";
        return nullptr;
    }

    return _canDeviceThread->accessCanDevice();
}
