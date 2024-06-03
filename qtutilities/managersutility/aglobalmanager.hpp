// SPDX-FileCopyrightText: 2021 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QDebug>
#include <QHash>

class AbstractManager;


/** @brief The Global manager manages all the managers (singleton) and it's the entry point to get
           them all.
    @note This is an abstract class to inherit in order to have an usable GlobalManager
    @attention In the derived class don't forget to:
                - Set the global manager instance, thanks to the @ref setInstance method
                - Register the managers thanks to the @ref registerManager method */
class AGlobalManager : public QObject
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param parent The parent class instance */
        explicit AGlobalManager(QObject *parent = nullptr);

        /** @brief Set the instance of the @ref AGlobalManager and init the manager
            @note This method is the first to call in the derived class in order to set the
                  singleton
            @param globalManager The app Global manager instance */
        static void setInstance(AGlobalManager *globalManager);

    public:
        /** @brief Get the global manager instance
            @warning This will crash if the @ref setInstance has never been called before */
        static AGlobalManager &instance();

    public:
        /** @brief Generic constant getter for accessing all the(known) managers
            @return Returns nullptr if the manager hasn't been found */
        template<class T>
        const T *getManager() const;

        /** @brief Generic accessor for accessing all the(known) managers
            @return Returns nullptr if the manager hasn't been found */
        template<class T>
        T *accessManager();

        /** @brief Test if the manager is known */
        template<class T>
        bool isManagerKnown();

    protected slots:
        /** @brief Called when the main event loop is entered and ready
            @note Called at library start
            @note If you have methods to call at start which needs to use the event loop, put yours
                  methods in it */
        virtual void onEventLoopReady();

    protected:
        /** @brief This method is called at the class initialization and has to be used to register
                   the managers (thanks to the @ref registerManager method) */
        virtual void initManagers() = 0;

        /** @brief Call all the known C++ and QML modules (which needs to be registered in C++) to
                   register them */
        virtual void registerQmlTypes();

        /** @brief Call all the known meta and event types to register */
        virtual void registerMetaTypes();

        /** @brief Call to set all the default informations of the library */
        virtual void setDefaultLibraryInformations();

    protected:
        /** @brief This is an useful method to register managers in derived class
            @param manager The manager to register (has to be an AbstractManager, or this will fail
                           at build time) */
        template<class T>
        void registerManager(T *manager);

        /** @brief Access the wanted manager
            @param metaObject The unique meta object which represents the static class
            @return The manager or nullptr if it hasn't been found */
        AbstractManager *accessManagerProt(const QMetaObject *metaObject);

        /** @brief This method can be used in derived class when you create yourself named
                   accessors or template specialization
            @warning If the T class isn't known this will crash the App */
        template<class T>
        T &unsafeAccessManager();

    private:
        /*! @brief Instantiate all members.
            @warning When the method is called the main event loop isn't started yet, if you need it
                     see @ref AGlobalManager::onEventLoopReady
            @note See comment inside contructor to understand why.,*/
        void init();

        /** @brief Register a manager in the Global Manager
            @note This will use the unique @ref QMetaObject static member of each QObject class
            @param metaObject The unique metaObject of the manager
            @param manager The manager to register in the global manager */
        void registerManager(const QMetaObject *metaObject, AbstractManager *manager);

    private:
        static AGlobalManager * _instance;    //!< @brief Instance singleton

        static const constexpr char* defaultOrganizationName = "ACTechnologies";

    private:
        QHash<const QMetaObject*, AbstractManager*> _managers;
};

template<class T>
bool AGlobalManager::isManagerKnown()
{
    static_assert (std::is_base_of<AbstractManager, T>(), "The manager you want to test isn't "
                                                          "an AbstractManager");
    return _managers.contains(&T::staticMetaObject);
}

template<class T>
const T *AGlobalManager::getManager() const
{
    static_assert (std::is_base_of<AbstractManager, T>(), "The manager you want to get isn't "
                                                          "an AbstractManager");
    return accessManager<T>();
}

template<class T>
T *AGlobalManager::accessManager()
{
    static_assert (std::is_base_of<AbstractManager, T>(), "The manager you want to get isn't "
                                                          "an AbstractManager");
    AbstractManager *manager = accessManagerProt(&T::staticMetaObject);

    if(manager == nullptr)
    {
        return nullptr;
    }

    T *castedManager = qobject_cast<T*>(manager);

    if(castedManager == nullptr)
    {
        qWarning() << "The manager cannot be casted to: " << T::staticMetaObject->className();
        return nullptr;
    }

    return castedManager;
}

template<class T>
inline void AGlobalManager::registerManager(T *manager)
{
    static_assert (std::is_base_of<AbstractManager, T>(), "The manager you want to register isn't "
                                                          "an AbstractManager");
    registerManager(&T::staticMetaObject, manager);
}

template<class T>
T &AGlobalManager::unsafeAccessManager()
{
    static_assert (std::is_base_of<AbstractManager, T>(), "The manager you want to test isn't "
                                                          "an AbstractManager");
    // This can crash if the manager isn't known or if the object cast has failed
    return *(qobject_cast<T*>(_managers[&T::staticMetaObject]));
}
