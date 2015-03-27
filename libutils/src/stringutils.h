/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

/**
\ingroup libutils
\brief Utilities for string manipulation
*/

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <iterator>
#include <QString>
#include <QStringList>

namespace StringUtils
{
    template<typename TReturn, typename TContainer>
    inline TReturn join(const TContainer& container, const TReturn& separator)
    {
        return join(std::begin(container), std::end(container), separator);
    }

    template<typename TSeparator>
    inline QString join(const QStringList& container, TSeparator&& separator)
    {
        return container.join(std::forward<TSeparator>(separator));
    }

    template<typename TReturn, typename ForwardIt>
    inline TReturn join(ForwardIt begin, ForwardIt end, const TReturn& separator)
    {
        TReturn result;
        while (begin != end)
        {
            result += *begin;
            if (++begin != end)
            {
                result += separator;
            }
        }

        return result;
    }
}

#endif // STRINGUTILS_H

