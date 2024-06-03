// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "jsonhelper.hpp"

#include <cmath>

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "definesutility/definesutility.hpp"
#include "jsonutility/jsontypedefs.hpp"


bool JsonHelper::loadJsonFile(const QString &fileName,
                              QJsonDocument &documentLoaded,
                              QString *errorString)
{
    if(fileName.isNull())
    {
        logError("Json file path can't be empty", errorString);
        return false;
    }

    QFile jsonFile(fileName);

    if(!jsonFile.exists())
    {
        logError("The JSON file: " + fileName + ", doesn't exist", errorString);
        return false;
    }

    return loadJsonDevice(jsonFile, documentLoaded, errorString);
}

bool JsonHelper::loadJsonDevice(QIODevice &device,
                                QJsonDocument &documentLoaded,
                                QString *errorString)
{
    if(!device.open(QIODevice::ReadOnly))
    {
        logError("Can't open the QIODevice for reading", errorString);
        return false;
    }

    RETURN_IF_FALSE(loadJsonByteArray(device.readAll(), documentLoaded, errorString));

    device.close();
    return true;
}

bool JsonHelper::loadJsonByteArray(const QByteArray &jsonData,
                                   QJsonDocument &documentLoaded,
                                   QString *errorString)
{
    QJsonParseError error{};

    documentLoaded = QJsonDocument::fromJson(jsonData, &error);

    if(documentLoaded.isNull())
    {
        logError("A problem occurred when parsing the QIODevice, error: " + error.errorString(),
                 errorString);
        return false;
    }

    return true;
}

bool JsonHelper::getArray(const QJsonDocument &jsonDocument, QJsonArray &jsonArray)
{
    if(!jsonDocument.isArray())
    {
        qWarning() << "The json document is not an array";
        return false;
    }

    jsonArray = jsonDocument.array();

    return true;
}

bool JsonHelper::getArray(const QJsonObject &jsonObject,
                          const QString &key,
                          QJsonArray &value,
                          bool canBeEmpty)
{
    if(!getArray(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the array: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getArray(const QJsonValue &jsonValue, QJsonArray &value, bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.isArray())
    {
        qWarning() << "Can't get the array: the value isn't a JsonArray";
        return false;
    }

    value = jsonValue.toArray();
    return true;
}

bool JsonHelper::getArray(const JsonObject &jsonObject,
                          const QString &key,
                          JsonArray &value,
                          bool canBeEmpty)
{
    if(!getArray(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the array: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getArray(const QVariant &jsonValue,
                          JsonArray &value,
                          bool canBeEmpty)
{
    if(jsonValue.isNull() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.canConvert<JsonArray>())
    {
        qWarning() << "Can't get the array: the value can't be converted to a JsonArray";
        return false;
    }

    value = jsonValue.value<JsonArray>();
    return true;
}

bool JsonHelper::getBoolean(const QJsonObject &jsonObject,
                            const QString &key,
                            bool &value,
                            bool canBeEmpty)
{
    if(!getBoolean(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the boolean: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getBoolean(const QJsonValue &jsonValue, bool &value, bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.isBool())
    {
        qWarning() << "The value isn't a bool";
        return false;
    }

    value = jsonValue.toBool();
    return true;
}

bool JsonHelper::getBoolean(const JsonObject &jsonObject,
                            const QString &key,
                            bool &value,
                            bool canBeEmpty)
{
    if(!getBoolean(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the boolean: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getBoolean(const QVariant &jsonValue, bool &value, bool canBeEmpty)
{
    if(jsonValue.isNull() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.canConvert<bool>())
    {
        qWarning() << "The value can't be converted to a bool";
        return false;
    }

    value = jsonValue.toBool();
    return true;
}

bool JsonHelper::getBooleanList(const QJsonObject &jsonObject,
                                const QString &key,
                                QList<bool> &value,
                                bool canBeEmpty)
{
    if(!getBooleanList(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the boolean list: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getBooleanList(const QJsonValue &jsonValue, QList<bool> &value, bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        return true;
    }

    QJsonArray jsonArray;

    if(!getArray(jsonValue, jsonArray))
    {
        return false;
    }

    return getBooleanList(jsonArray, value);
}

bool JsonHelper::getBooleanList(const QJsonArray &jsonArray, QList<bool> &value)
{
    QJsonArray::const_iterator iter = jsonArray.begin();

    for(; iter != jsonArray.end(); iter++)
    {
        if(!iter->isBool())
        {
            qWarning() << "The list element is not a string";
            return false;
        }

        value.append(iter->toBool());
    }

    return true;
}

bool JsonHelper::getObject(const QJsonDocument &jsonDocument, QJsonObject &jsonObject)
{
    if(!jsonDocument.isObject())
    {
        qWarning() << "The json document is not an object";
        return false;
    }

    jsonObject = jsonDocument.object();

    return true;
}

bool JsonHelper::getObject(const QJsonObject &jsonCurrentObject,
                           const QString &key,
                           QJsonObject &jsonObject,
                           bool canBeEmpty)
{
    if(!getObject(jsonCurrentObject.value(key), jsonObject, canBeEmpty))
    {
        qWarning() << "The attribute: " << key << ", is not an object";
        return false;
    }

    return true;
}

bool JsonHelper::getObject(const QJsonValue &jsonValue, QJsonObject &jsonObject, bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.isObject())
    {
        qWarning() << "The value is not an object";
        return false;
    }

    jsonObject = jsonValue.toObject();

    return true;
}

bool JsonHelper::getObject(const QJsonArray &jsonArray, int index, QJsonObject &jsonObject)
{
    if(jsonArray.count() <= index)
    {
        qWarning() << "The index: " << index << ", overflows the jsonarray length: "
                   << jsonArray.count();
        return false;
    }

    QJsonValue jsonValue = jsonArray.at(index);

    if(!jsonValue.isObject())
    {
        qWarning() << "The attribute at index: " << index << ", is not an object";
        return false;
    }

    jsonObject = jsonValue.toObject();

    return true;
}

bool JsonHelper::getObject(const JsonArray &jsonArray, int index, JsonObject &jsonObject)
{
    if(jsonArray.length() <= index)
    {
        qWarning() << "The index: " << index << ", overflows the jsonarray length: "
                   << jsonArray.count();
        return false;
    }

    return getObject(jsonArray.at(index), jsonObject);
}

bool JsonHelper::getObject(const QVariant &jsonValue, JsonObject &jsonObject, bool canBeEmpty)
{
    if(jsonValue.isNull() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.canConvert<JsonObject>())
    {
        qWarning() << "Can't convert the QVariant to a JsonObject";
        return false;
    }

    jsonObject = jsonValue.value<JsonObject>();
    return true;
}

bool JsonHelper::getValueType(const QJsonObject &jsonObject,
                              const QString &key,
                              QJsonValue::Type &valueType,
                              bool canBeEmpty)
{
    QJsonValue value = jsonObject.value(key);

    if(value.isUndefined() && !canBeEmpty)
    {
        qWarning() << "The key: " << key << ", doesn't exist in the object given";
        return false;
    }

    valueType = value.type();

    return true;
}

bool JsonHelper::getString(const QJsonObject &jsonObject,
                           const QString &key,
                           QString &value,
                           bool canBeEmpty)
{
    if(!getString(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the string: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getString(const QJsonValue &jsonValue, QString &value, bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.isString())
    {
        qWarning() << "The value isn't a string";
        return false;
    }

    value = jsonValue.toString();
    return true;
}

bool JsonHelper::getString(const JsonObject &jsonObject,
                           const QString &key,
                           QString &value,
                           bool canBeEmpty)
{
    if(!getString(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the string: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getString(const QVariant &jsonValue, QString &value, bool canBeEmpty)
{
    if(jsonValue.isNull() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.canConvert<QString>())
    {
        qWarning() << "The value can't be converted to a string";
        return false;
    }

    value = jsonValue.toString();
    return true;
}

bool JsonHelper::getStringList(const QJsonObject &jsonObject,
                               const QString &key,
                               QStringList &value,
                               bool canBeEmpty)
{
    if(!getStringList(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the string list: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getStringList(const QJsonValue &jsonValue, QStringList &value, bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        return true;
    }

    QJsonArray jsonArray;

    if(!getArray(jsonValue, jsonArray))
    {
        return false;
    }

    return getStringList(jsonArray, value);
}

bool JsonHelper::getStringList(const QJsonArray &jsonArray, QStringList &value)
{
    QJsonArray::const_iterator iter = jsonArray.begin();

    for(; iter != jsonArray.end(); iter++)
    {
        if(!iter->isString())
        {
            qWarning() << "The list element is not a string";
            return false;
        }

        value.append(iter->toString());
    }

    return true;
}

bool JsonHelper::getStringVector(const QJsonObject &jsonObject,
                                 const QString &key,
                                 QVector<QString> &value,
                                 bool canBeEmpty)
{
    if(!getStringVector(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the string list: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getStringVector(const JsonObject &jsonObject,
                                 const QString &key,
                                 QVector<QString> &values,
                                 bool canBeEmpty)
{
    return getStringVector(jsonObject.value(key), values, canBeEmpty);
}

bool JsonHelper::getStringVector(const QVariant &jsonValue,
                                 QVector<QString> &values,
                                 bool canBeEmpty)
{
    if(jsonValue.isNull() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.canConvert<JsonArray>())
    {
        qWarning() << "The value can't be converted to a string";
        return false;
    }


    return getStringVector(jsonValue.value<JsonArray>(), values);
}

bool JsonHelper::getStringVector(const JsonArray &jsonArray, QVector<QString> &values)
{
    JsonArray::const_iterator iter = jsonArray.begin();

    for(; iter != jsonArray.end(); iter++)
    {
        if(!iter->canConvert<QString>())
        {
            qWarning() << "The list element is not a string";
            return false;
        }

        values.append(iter->toString());
    }

    return true;
}

bool JsonHelper::getStringVector(const QJsonValue &jsonValue,
                                 QVector<QString> &value,
                                 bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        return true;
    }

    QJsonArray jsonArray;

    if(!getArray(jsonValue, jsonArray))
    {
        return false;
    }

    return getStringVector(jsonArray, value);
}

bool JsonHelper::getStringVector(const QJsonArray &jsonArray, QVector<QString> &value)
{
    QJsonArray::const_iterator iter = jsonArray.begin();

    for(; iter != jsonArray.end(); iter++)
    {
        if(!iter->isString())
        {
            qWarning() << "The list element is not a string";
            return false;
        }

        value.append(iter->toString());
    }

    return true;
}

bool JsonHelper::getDouble(const QJsonObject &jsonObject,
                           const QString &key,
                           double &value,
                           bool canBeEmpty)
{
    if(!getDouble(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the double: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getDouble(const QJsonValue &jsonValue, double &value, bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        value = qQNaN();
        return true;
    }

    if(!jsonValue.isDouble())
    {
        qWarning() << "The value isn't a double";
        return false;
    }

    value = jsonValue.toDouble();
    return true;
}

bool JsonHelper::getDoubleList(const QJsonObject &jsonObject,
                               const QString &key,
                               QList<double> &value,
                               bool canBeEmpty)
{
    if(!getDoubleList(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the double list : a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getDoubleList(const QJsonValue &jsonValue, QList<double> &value, bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        return true;
    }

    QJsonArray jsonArray;

    if(!getArray(jsonValue, jsonArray))
    {
        return false;
    }

    return getDoubleList(jsonArray, value);
}

bool JsonHelper::getDoubleList(const QJsonArray &jsonArray, QList<double> &value)
{
    QJsonArray::const_iterator iter = jsonArray.begin();

    for(; iter != jsonArray.end(); iter++)
    {
        if(!iter->isDouble())
        {
            qWarning() << "The list element is not a double";
            return false;
        }

        value.append(iter->toDouble());
    }

    return true;
}

bool JsonHelper::getInt32(const QJsonObject &jsonObject,
                          const QString &key,
                          qint32 &value,
                          bool canBeEmpty)
{
    if(!getInt32(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the qint32: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getInt32(const QJsonValue &jsonValue, qint32 &value, bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.isDouble())
    {
        qWarning() << "The value isn't a qint32";
        return false;
    }

    value = jsonValue.toInt();
    return true;
}

bool JsonHelper::getInt32(const JsonObject &jsonObject,
                          const QString &key,
                          qint32 &value,
                          bool canBeEmpty)
{
    if(!getInt32(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the int32: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getInt32(const QVariant &jsonValue, qint32 &value, bool canBeEmpty)
{
    if(jsonValue.isNull() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.canConvert<qint32>())
    {
        qWarning() << "The value can't be converted to a int32";
        return false;
    }

    bool convertResult;
    value = jsonValue.toInt(&convertResult);
    return convertResult;
}

bool JsonHelper::getUint32(const QJsonObject &jsonObject,
                           const QString &key,
                           quint32 &value,
                           bool canBeEmpty)
{
    if(!getUint32(jsonObject.value(key), value, canBeEmpty))
    {
        qWarning() << "Can't get the uint32: a problem occurred with the key: " << key;
        return false;
    }

    return true;
}

bool JsonHelper::getUint32(const QJsonValue &jsonValue, quint32 &value, bool canBeEmpty)
{
    if(jsonValue.isUndefined() && canBeEmpty)
    {
        return true;
    }

    if(!jsonValue.isDouble())
    {
        qWarning() << "The value isn't a number";
        return false;
    }

    double doubleValue = jsonValue.toDouble();

    double tmp;

    if(modf(doubleValue, &tmp) != 0.0)
    {
        qWarning() << "Value isn't integer: " << doubleValue;
        return false;
    }

    if(doubleValue > std::numeric_limits<quint32>::max())
    {
        qWarning() << "Value overflows quint32 max value";
        return false;
    }

    value = static_cast<quint32>(doubleValue);

    return true;
}

void JsonHelper::logError(const QString &toLog, QString *errorString)
{
    if(errorString != nullptr)
    {
        *errorString = toLog;
    }

    // "ALO: Minor notice: qWarning will always print location logError(), not caller method "
    // "name. This is fine if all messages are differents so we can track the source."
    qWarning() << toLog;
}

void JsonHelper::RegisterMetaTypes()
{
    qRegisterMetaType<JsonArray>();

    // Necessary when defining an alias type, from Qt doc:
    // This function is useful to register typedefs so they can be used by QMetaProperty, or in
    // QueuedConnections, ex:
    //      typedef QString CustomString;
    //      qRegisterMetaType<CustomString>("CustomString");
    qRegisterMetaType<JsonObject>("JsonObject");
}
