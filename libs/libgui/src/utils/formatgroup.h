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
\class FormatGroup
\brief Auxiliary class used by SyntaxHighlight to construct formatting groups that
are used to apply formatting (color, style, etc) to text blocks that matches group's
expressions.
*/

#ifndef FORMAT_GROUP_H
#define FORMAT_GROUP_H

#include "exprelement.h"
#include <QTextCharFormat>

class __libgui FormatGroup {
	private:
		QString name;

		QTextCharFormat format;

		bool allow_completion,

				persistent;

		QList<ExprElement> expr_elements;

	public:
		FormatGroup();
		FormatGroup(const QString &_name, const QTextCharFormat &_format,
								 bool _allow_compl, bool _persistent);

		void addExpression(const ExprElement &expr);

		QString getName() const;
		QTextCharFormat getFormat() const;
		bool isPersistent() const;
		bool isMultiline() const;
		bool isCompletionAllowed() const;

		const QList<ExprElement> &getExpressions();
};

#endif
