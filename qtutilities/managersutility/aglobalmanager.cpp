// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "aglobalmanager.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

AGlobalManager * AGlobalManager::_instance = nullptr;


AGlobalManager::AGlobalManager(QObject *parent) : QObject(parent)
{
    // Constructor voluntary empty to avoid cyclic redundancy during initialization of its members.
    // See init.
}

void AGlobalManager::setInstance(AGlobalManager *globalManager)
{
    if(Q_LIKELY(_instance == nullptr))
    {
        _instance = globalManager;
        _instance->init();
    }
    else
    {
        qWarning() << "Already instanciated";
    }
}

AGlobalManager &AGlobalManager::instance()
{
    if(Q_UNLIKELY(!_instance))
    {
        qCritical() << "The global manager wasn't previously set ; this will crash";
    }

    return *_instance;
}

void AGlobalManager::onEventLoopReady()
{
    // Add here all the initialization which needs the main event loop to be ready
    qInfo() << "The application is launched";
}

void AGlobalManager::registerQmlTypes()
{
    // This method is useful to register QML types
}

void AGlobalManager::registerMetaTypes()
{
    // Useful method to register Meta Types
}

void AGlobalManager::init()
{
    // Set the default library informations
    setDefaultLibraryInformations();

    // Register the meta types of objects
    registerMetaTypes();

    initManagers();

    // Because we register some managers, we register QML types after the managers creation
    registerQmlTypes();

    QTimer::singleShot(0, this, &AGlobalManager::onEventLoopReady);
}

void AGlobalManager::setDefaultLibraryInformations()
{
    QString organizationName;

#ifdef ORGANIZATION_NAME
    organizationName = ORGANIZATION_NAME;
#else
    organizationName = defaultOrganizationName;

    qInfo() << "No organization name has been set in the application .pro file (need to define "
            << "the global variable: ORGANIZATION_NAME), use the default one: " << organizationName;
#endif

    QCoreApplication::setOrganizationName(organizationName);
}

void AGlobalManager::registerManager(const QMetaObject *metaObject, AbstractManager *manager)
{
    _managers.insert(metaObject, manager);
}

AbstractManager *AGlobalManager::accessManagerProt(const QMetaObject *metaObject)
{
    if(Q_UNLIKELY(!_managers.contains(metaObject)))
    {
        qCritical() << "The abstract manager: " << metaObject->className() << ", hasn't been "
                       "registered, can't continue";
        return nullptr;
    }

    return _managers[metaObject];
}
