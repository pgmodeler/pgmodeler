/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\brief Definition of a constexpr function that returns the underlying type of a enum class
*/

#ifndef ENUM_TYPE_H
#define ENUM_TYPE_H

#include <type_traits>

//! \brief This function causes the provided enum to be converted to its underlying datatype
template<typename Enum>
constexpr std::underlying_type_t<Enum> enum_t (Enum enum_val) noexcept
{
	return static_cast<typename std::underlying_type_t<Enum>>(enum_val);
}

#endif
