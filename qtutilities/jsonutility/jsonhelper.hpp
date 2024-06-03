// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include <QJsonValue>

#include "jsontypedefs.hpp"

class QIODevice;

/** @brief Contains useful methods to manage a JSON file */
class JsonHelper
{
    public:
        /** @brief Load a json file
            @param fileName The name of the json file to load
            @param documentLoaded The JSON document loaded
            @param errorString If not NULL and if an error occurs, write the details in it
            @return True if no error occurs */
        static bool loadJsonFile(const QString &fileName,
                                 QJsonDocument &documentLoaded,
                                 QString *errorString = nullptr);

        /** @brief Load a json from a device
            @param device The device where to get the json document
            @param documentLoaded The JSON document loaded
            @param errorString If not NULL and if an error occurs, write the details in it
            @return True if no error occurs */
        static bool loadJsonDevice(QIODevice &device,
                                   QJsonDocument &documentLoaded,
                                   QString *errorString = nullptr);

        /** @brief Load a json from a byte array
            @param jsonData The data byte array of the JSON
            @param documentLoaded The JSON document loaded
            @param errorString If not NULL and if an error occurs, write the details in it
            @return True if no error occurs */
        static bool loadJsonByteArray(const QByteArray &jsonData,
                                      QJsonDocument &documentLoaded,
                                      QString *errorString = nullptr);

        /** @brief If the JSON document is an array, get the json array
            @param jsonDocument The jsonDocument to transform
            @param jsonArray The JSON array representation to get */
        static bool getArray(const QJsonDocument &jsonDocument, QJsonArray &jsonArray);

        /** @brief Get the QJsonArray value of the indicated attribute from the JSON Object
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the boolean value will be equal to the value given
            @return False if an error occurs or if the attribute hasn't been found */
        static bool getArray(const QJsonObject &jsonObject,
                             const QString &key,
                             QJsonArray &value,
                             bool canBeEmpty = false);

        /** @brief Parse to QJsonArray from QJsonValue
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the boolean value will be equal to the value given
            @return False if an error occurs or if the attribute hasn't been found */
        static bool getArray(const QJsonValue &jsonValue,
                             QJsonArray &value,
                             bool canBeEmpty = false);

        /** @brief Get the JsonArray value of the indicated attribute from the JsonObject
            @param jsonObject The JsonObject to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the boolean value will be equal to the value given
            @return False if an error occurs or if the attribute hasn't been found */
        static bool getArray(const JsonObject &jsonObject,
                             const QString &key,
                             JsonArray &value,
                             bool canBeEmpty = false);

        /** @brief Parse to JsonArray from QVariant
            @warning This method has only to be called with JsonObject and JsonArray system (not
                     with Qt QJsonObject)
            @param jsonValue The QVariant to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the boolean value will be equal to the value given
            @return False if an error occurs or if the attribute hasn't been found */
        static bool getArray(const QVariant &jsonValue,
                             JsonArray &value,
                             bool canBeEmpty = false);

        /** @brief Get the boolean value of the indicated attribute from the JSON Object
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the boolean value will be equal to the value given
            @return True if no error occurs */
        static bool getBoolean(const QJsonObject &jsonObject,
                               const QString &key,
                               bool &value,
                               bool canBeEmpty = false);

        /** @brief Parse the boolean value from QJsonValue
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the boolean value will be equal to the value given
            @return True if no error occurs */
        static bool getBoolean(const QJsonValue &jsonValue,
                               bool &value,
                               bool canBeEmpty = false);

        /** @brief Get the boolean value of the indicated attribute from the JsonObject
            @param jsonObject The JJsonObject to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the boolean value will be equal to the value given
            @return True if no error occurs */
        static bool getBoolean(const JsonObject &jsonObject,
                               const QString &key,
                               bool &value,
                               bool canBeEmpty = false);

        /** @brief Parse the boolean value from QVariant
            @warning This method has only to be called with JsonObject and JsonArray system (not
                     with Qt QJsonObject)
            @param jsonValue The QVariant to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the boolean value will be equal to the value given
            @return True if no error occurs */
        static bool getBoolean(const QVariant &jsonValue,
                               bool &value,
                               bool canBeEmpty = false);

        /** @brief Get the boolean list value of the indicated attribute from the JSON Object
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the bool array value will keep its values
            @return True if no error occurs */
        static bool getBooleanList(const QJsonObject &jsonObject,
                                   const QString &key,
                                   QList<bool> &value,
                                   bool canBeEmpty = false);

        /** @brief Parse the boolean list value from QJsonValue
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the bool array value will keep its values
            @return True if no error occurs */
        static bool getBooleanList(const QJsonValue &jsonValue,
                                   QList<bool> &value,
                                   bool canBeEmpty = false);

        /** @brief Parse the boolean list value from QJsonArray
            @param jsonArray The JSON array to parse in order to get the value wanted
            @param value The value found
            @return True if no error occurs */
        static bool getBooleanList(const QJsonArray &jsonArray,
                                   QList<bool> &value);

        /** @brief If the JSON document is an object, get the json object representation
            @param jsonDocument The jsonDocument to transform
            @param jsonObject The JSON object representation to get */
        static bool getObject(const QJsonDocument &jsonDocument, QJsonObject &jsonObject);

        /** @brief Get the object value of the indicated attribute from the JSON Object
            @param jsonCurrentObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param jsonObject The object found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the object value is equal to the one given
            @return True if no error occurs */
        static bool getObject(const QJsonObject &jsonCurrentObject,
                              const QString &key,
                              QJsonObject &jsonObject,
                              bool canBeEmpty = false);

        /** @brief Parse the object value from QJsonValue
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param jsonObject The object found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the object value is equal to the one given
            @return True if no error occurs */
        static bool getObject(const QJsonValue &jsonValue,
                              QJsonObject &jsonObject,
                              bool canBeEmpty = false);

        /** @brief Parse the object value from QJsonArray
            @param jsonArray The JSON array to parse in order to get the value wanted
            @param index The index in the array of the object to get
            @param jsonObject The object found
            @return True if no error occurs */
        static bool getObject(const QJsonArray &jsonArray,
                              int index,
                              QJsonObject &jsonObject);

        /** @brief Parse the object value from JsonArray
            @param jsonArray The JSON array to parse in order to get the value wanted
            @param index The index in the array of the object to get
            @param jsonObject The object found
            @return True if no error occurs */
        static bool getObject(const JsonArray &jsonArray,
                              int index,
                              JsonObject &jsonObject);

        /** @brief Parse the object value from QVariant
            @warning This method has only to be called with JsonObject and JsonArray system (not
                     with Qt QJsonObject)
            @param jsonValue The QVariant to parse in order to get the value wanted
            @param jsonObject The object found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the object value is equal to the one given
            @return True if no error occurs */
        static bool getObject(const QVariant &jsonValue,
                              JsonObject &jsonObject,
                              bool canBeEmpty = false);

        /** @brief Get the type of the indicated attribute from the JSON Object
            @param jsonObject The JSON Object to parse in order to get the type wanted
            @param key The attribute key to find
            @param valueType The value type found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the object value is equal to the one given
            @return True if no error occurs */
        static bool getValueType(const QJsonObject &jsonObject,
                                 const QString &key,
                                 QJsonValue::Type &valueType,
                                 bool canBeEmpty);

        /** @brief Get the string value of the indicated attribute from the JSON Object
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error (and may print a warning trace).
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string value is equal to the one given
            @return True if no error occurs */
        static bool getString(const QJsonObject &jsonObject,
                              const QString &key,
                              QString &value,
                              bool canBeEmpty = false);

        /** @brief Parse the string value from QJsonValue
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error (and may print a warning trace).
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string value is equal to the one given
            @return True if no error occurs */
        static bool getString(const QJsonValue &jsonValue, QString &value, bool canBeEmpty = false);

        /** @brief Get the string value of the indicated attribute from the JsonObject
            @warning This method has only to be called with JsonObject and JsonArray system (not
                     with Qt QJsonObject)
            @param jsonObject The JsonObject to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error (and may print a warning trace).
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string value is equal to the one given
            @return True if no error occurs */
        static bool getString(const JsonObject &jsonObject,
                              const QString &key,
                              QString &value,
                              bool canBeEmpty = false);

        /** @brief Parse the string value from QVariant
            @warning This method has only to be called with JsonObject and JsonArray system (not
                     with Qt QJsonObject)
            @param jsonValue The QVariant to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error (and may print a warning trace).
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string value is equal to the one given
            @return True if no error occurs */
        static bool getString(const QVariant &jsonValue, QString &value, bool canBeEmpty = false);

        /** @brief Get the string list value of the indicated attribute from the JSON Object
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getStringList(const QJsonObject &jsonObject,
                                  const QString &key,
                                  QStringList &value,
                                  bool canBeEmpty = false);

        /** @brief Parse the string list value from QJsonValue
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getStringList(const QJsonValue &jsonValue,
                                  QStringList &value,
                                  bool canBeEmpty = false);

        /** @brief Parse the string list value from QJsonArray
            @param jsonArray The JSON array to parse in order to get the value wanted
            @param value The value found
            @return True if no error occurs */
        static bool getStringList(const QJsonArray &jsonArray,
                                  QStringList &value);

        /** @brief Get the string vector list value of the indicated attribute from the JSON Object
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getStringVector(const QJsonObject &jsonObject,
                                    const QString &key,
                                    QVector<QString> &value,
                                    bool canBeEmpty = false);

        /** @brief Parse the string vector list value from QJsonValue
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getStringVector(const QJsonValue &jsonValue,
                                    QVector<QString> &value,
                                    bool canBeEmpty = false);

        /** @brief Parse the string vector list value from QJsonArray
            @param jsonArray The JSON array to parse in order to get the value wanted
            @param value The value found
            @return True if no error occurs */
        static bool getStringVector(const QJsonArray &jsonArray,
                                    QVector<QString> &value);

        /** @brief Get the string vector list value of the indicated attribute from the JsonObject
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getStringVector(const JsonObject &jsonObject,
                                    const QString &key,
                                    QVector<QString> &value,
                                    bool canBeEmpty = false);

        /** @brief Get the string vector list value of the indicated attribute from the JsonObject
            @param jsonValue The QVariant to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getStringVector(const QVariant &jsonValue,
                                    QVector<QString> &value,
                                    bool canBeEmpty = false);

        /** @brief Get the string vector list value of the indicated attribute from the JsonObject
            @param jsonArray The JSON array to parse in order to get the value wanted
            @param value The value found
            @return True if no error occurs */
        static bool getStringVector(const JsonArray &jsonArray,
                                    QVector<QString> &value);

        /** @brief Get the double value of the indicated attribute from the JSON Object
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getDouble(const QJsonObject &jsonObject,
                              const QString &key,
                              double &value,
                              bool canBeEmpty = false);

        /** @brief Parse the double value from QJsonValue
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getDouble(const QJsonValue &jsonValue,
                              double &value,
                              bool canBeEmpty = false);

        /** @brief Get the double list value of the indicated attribute from the JSON Object
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getDoubleList(const QJsonObject &jsonObject,
                                  const QString &key,
                                  QList<double> &value,
                                  bool canBeEmpty = false);

        /** @brief Parse the double list value from QJsonValue
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getDoubleList(const QJsonValue &jsonValue,
                                  QList<double> &value,
                                  bool canBeEmpty = false);

        /** @brief Parse the double list value from QJsonArray
            @param jsonArray The JSON array to parse in order to get the value wanted
            @param value The value found
            @return True if no error occurs */
        static bool getDoubleList(const QJsonArray &jsonArray,
                                  QList<double> &value);

        /** @brief Get the int32 value of the indicated attribute from the JSON Object
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getInt32(const QJsonObject &jsonObject,
                             const QString &key,
                             qint32 &value,
                             bool canBeEmpty = false);

        /** @brief Parse the int32 value from QJsonValue
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string list value is equal to the one given
            @return True if no error occurs */
        static bool getInt32(const QJsonValue &jsonValue,
                             qint32 &value,
                             bool canBeEmpty = false);

        /** @brief Get the int32 value of the indicated attribute from the JsonObject
            @param jsonObject The JsonObject to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the boolean value will be equal to the value given
            @return False if an error occurs or if the attribute hasn't been found */
        static bool getInt32(const JsonObject &jsonObject,
                             const QString &key,
                             qint32 &value,
                             bool canBeEmpty = false);

        /** @brief Parse the int32 value from QVariant
            @warning This method has only to be called with JsonObject and JsonArray system (not
                     with Qt QJsonObject)
            @param jsonValue The QVariant to parse in order to get the value wanted
            @param value The value found
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the boolean value will be equal to the value given
            @return True if no error occurs */
        static bool getInt32(const QVariant &jsonValue,
                             qint32 &value,
                             bool canBeEmpty);

        /** @brief Try to parse a QJsonValue to double and then to uint32
            @param jsonObject The JSON Object to parse in order to get the value wanted
            @param key The attribute key to find
            @param value The value found and parsed
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the uint32 value is equal to the one given
            @return True if no error occurs */
        static bool getUint32(const QJsonObject &jsonObject,
                              const QString &key,
                              quint32 &value,
                              bool canBeEmpty = false);

        /** @brief Try to parse a QJsonValue to double and then to uint32
            @param jsonValue The JSON Value to parse in order to get the value wanted
            @param value The value found and parsed
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the uint32 value is equal to the one given
            @return True if no error occurs */
        static bool getUint32(const QJsonValue &jsonValue,
                              quint32 &value,
                              bool canBeEmpty = false);

        /** @brief Useful to register the object JsonArray */
        static void RegisterMetaTypes();

    private:
        /** @brief Useful method to log in console an error and also add an string representation
                   in the errorString given
            @param toLog The string to log in console and add in string
            @param errorString If not NULL, append the log to it
            @return True if no error occurs */
        static void logError(const QString &toLog, QString *errorString);
};
