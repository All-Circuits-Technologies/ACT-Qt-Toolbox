// SPDX-FileCopyrightText: 2019 - 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "qvectorhelper.hpp"


QString QVectorHelper::join(const QVector<QString> &stringList, const QString &separator)
{
    QString result;
    int separatorSize = separator.length();

    if(stringList.isEmpty())
    {
        return result;
    }

    // Calculate the expected size to reserve the result size
    int accumulatedSize = 0;
    for(const QString &elem : stringList)
    {
        accumulatedSize += elem.size() + separatorSize;
    }

    // Because an extra separator size is added, we remove one
    accumulatedSize -= separatorSize;

    result.reserve(accumulatedSize);

    for(int idx = 0; idx < stringList.length(); ++idx)
    {
        if(idx > 0)
        {
            result.append(separator);
        }

        result.append(stringList.at(idx));
    }

    return result;
}
