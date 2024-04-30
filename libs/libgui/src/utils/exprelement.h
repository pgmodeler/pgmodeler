/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libgui
\class ExprElement
\brief Auxiliary class used by SyntaxHighlight to construct formatting groups expressions that
are used to determine the format groups applied to text blocks.
*/

#ifndef EXPR_ELEMENT_H
#define EXPR_ELEMENT_H

#include "guiglobal.h"

class __libgui ExprElement {
	private:
		QString pattern;

		bool initial, final,
				exact, case_sensitive;

	public:
		ExprElement();

		ExprElement(const QString &_pattern, bool _initial, bool _final,
								 bool _exact, bool _case_sensitive);

		void clear();

		QString getPattern() const;

		bool isValid() const;

		bool isInitial() const;

		bool isFinal() const;

		bool isExact() const;

		bool isCaseSensitive() const;
};
#endif
