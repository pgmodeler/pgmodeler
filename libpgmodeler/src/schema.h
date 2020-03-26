/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class Schema
\brief Implements the operations to manipulate schemas on the database.
\note <strong>Creation date:</strong> 07/04/2008
*/

#ifndef SCHEMA_H
#define SCHEMA_H

#include "basegraphicobject.h"
#include <QColor>

class Schema: public BaseGraphicObject {
	private:
		QColor fill_color;
		bool rect_visible;

	public:
		Schema();

		void setName(const QString &name);

		void setFillColor(const QColor &color);
		QColor getFillColor();

		void setRectVisible(bool value);
		bool isRectVisible();

		virtual QString getCodeDefinition(unsigned def_type) final;
};

#endif
