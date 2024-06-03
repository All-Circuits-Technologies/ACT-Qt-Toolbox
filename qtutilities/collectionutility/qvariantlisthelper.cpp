// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "qvariantlisthelper.hpp"

#include <QDebug>
#include <QList>
#include <QVector>
#include <QString>


/* Defaults to empty converters set */
QMap<QString, QVariantListHelper::VariantListToVariantConverterFunc> \
    QVariantListHelper::_vlistToVariantConverters {};



void QVariantListHelper::RegisterVariantListToVariantConverter(
                                const QString &typeName,
                                const QVariantListHelper::VariantListToVariantConverterFunc &func)
{
    _vlistToVariantConverters.insert(typeName, func);
}

void QVariantListHelper::RegisterMostCommonVariantListToVariantConverters()
{
    #define REGISTER_CONVERTERS_FOR_TYPE(TYPE) \
        QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QList, TYPE); \
        QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector, TYPE);

    // Types peeked from qmetatype.h
    REGISTER_CONVERTERS_FOR_TYPE(bool);
    REGISTER_CONVERTERS_FOR_TYPE(int);
    REGISTER_CONVERTERS_FOR_TYPE(uint);
    REGISTER_CONVERTERS_FOR_TYPE(qlonglong);
    REGISTER_CONVERTERS_FOR_TYPE(qulonglong);
    REGISTER_CONVERTERS_FOR_TYPE(double);
    REGISTER_CONVERTERS_FOR_TYPE(long);
    REGISTER_CONVERTERS_FOR_TYPE(short);
    REGISTER_CONVERTERS_FOR_TYPE(char);
    REGISTER_CONVERTERS_FOR_TYPE(ulong);
    REGISTER_CONVERTERS_FOR_TYPE(ushort);
    REGISTER_CONVERTERS_FOR_TYPE(uchar);
    REGISTER_CONVERTERS_FOR_TYPE(QString);
}

QVariant QVariantListHelper::VariantListToVariant(const QVariantList &vlist,
                                                  const QString &typeName)
{
    QVariant result;

    if(_vlistToVariantConverters.contains(typeName))
    {
        result = _vlistToVariantConverters[typeName](vlist);
    }
    else
    {
        qWarning() << "No registered QVariantList to" << typeName << "converter";
    }

    return result;
}
