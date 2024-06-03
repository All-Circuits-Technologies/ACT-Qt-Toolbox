// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QMap>
#include <QVariant>
#include <QVariantList>


/** @brief This class provides some useful methods around QVariantList class.

    QVariantList to Collection<Type> converters
    ===========================================

      You likely need only two calls:

        1 - QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER macro
        ----------------------------------------------------------------------

            This macro easily registers a conversion function from both Collection and Type inputs.

        2 - VariantListToVariant function
        ---------------------------------

            Execute a conversion, based on registered converters.

    */
class QVariantListHelper
{
    private:
        /** @brief Function type able to convert a QVariantList into a QVariant of type C<T> */
        using VariantListToVariantConverterFunc = std::function<QVariant (const QVariantList &)>;

    private:
        /** @brief QVariantListHelper is a class only to hide a few members,
                   but no-one can instanciate it. */
        QVariantListHelper() {}

    public:
        /** @brief Register a new QVariantList to QVariant(C<T>) converter
            @param typeName "C<T>" type as text (no extra space)
            @param func Converter function
            @note You likely never want to call this method directly. Instead you will want
                  @ref QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER
            @see QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER */
        static void RegisterVariantListToVariantConverter(
                                                    const QString &typeName,
                                                    const VariantListToVariantConverterFunc &func);

        /** @brief Register a set of commonly used C<T> types.
            @note Thsi includes QList<int>, QVector<double>, etc */
        static void RegisterMostCommonVariantListToVariantConverters();

        /** @brief Convert a QVariantList into a QVariant(C<T>)
            @note A converter for type "C<T>" should have been previously registered
                  using @ref RegisterVariantListToVariantConverter
            @param vlist QVariantList to convert
            @param typeName Wanted "C<T>" type
            @return vlist converted into typename, embedded into a QVariant(C<T>)
            @retval Invalid QVariant in case of error */
        static QVariant VariantListToVariant(const QVariantList &vlist, const QString &typeName);

    private:
        /** @brief Banq of known QVariantList to QVariant of type C<T> converters.
            @note Key is the "C<T>" type
            @note Value is the QVariant-to-QVariant(C<T>) function */
        static QMap<QString, VariantListToVariantConverterFunc> _vlistToVariantConverters;
};

/** @def QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER
    @brief Easily register a new QVariantList to QVariant of type C<T> converter
    @note This macro creates a conversion function and registers it by calling
          @ref RegisterVariantListToVariantConverter.
    @param C Container type, example QVector
    @param T Contained type, example Banana */
#define QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(C, T)  \
    QVariantListHelper::RegisterVariantListToVariantConverter(              \
                QString("%1<%2>").arg(#C, #T),                              \
                [](const QVariantList& vlist) -> QVariant                   \
                {                                                           \
                    C<T> collection;                                        \
                    for(const QVariant &cell : vlist)                       \
                    {                                                       \
                        collection.append(cell.value<T>());                 \
                    }                                                       \
                    return QVariant::fromValue<C<T> >(collection);          \
                })
