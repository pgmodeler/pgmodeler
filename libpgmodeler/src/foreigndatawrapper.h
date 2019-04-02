/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libpgmodeler
\class ForeignDataWrapper
\brief Implements the operations to manipulate foreign data wrappers on the database.
*/

#ifndef FOREIGN_DATA_WRAPPER_H
#define FOREIGN_DATA_WRAPPER_H

#include "baseobject.h"
#include "function.h"

class ForeignDataWrapper: public BaseObject{
		//! \brief Function that executes the functions related to the foreign data wrapper
		Function *handler_func,

		//! \brief Function that validates the options passed to the foreign data wrapper
		*validator_func;

		//! \brief A set of key/value options associated to the foreign data wrapper
		attribs_map options;

	public:
		//! \brief Store the character used to separate options/values in the XML code
		static const QString OptionsSeparator;
		static const QString OptionValueSeparator;

		ForeignDataWrapper(void);

		void setHandlerFunction(Function *func);
		void setValidatorFunction(Function *func);

		Function *getHandlerFunction(void);
		Function *getValidatorFunction(void);

		void setOption(const QString &opt, const QString &value);
		void setOptions(const attribs_map &options);
		void removeOption(const QString &opt);
		void removeOptions(void);
		attribs_map getOptions(void);

		virtual QString getCodeDefinition(unsigned def_type);
};

#endif
