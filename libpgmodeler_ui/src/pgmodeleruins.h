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
\namespace PgModelerUiNS
\brief This namespace is used to organize all functions or constants used in the libpgmodeler_ui package.
*/

#ifndef PGMODELER_UI_NS_H
#define PGMODELER_UI_NS_H

#include <QTreeWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QPixmap>
#include "baseobject.h"
#include "numberedtexteditor.h"

namespace PgModelerUiNs {
	static constexpr unsigned SmallFontFactor = 0,
	MediumFontFactor = 1,
	BigFontFactor = 2,
	HugeFontFactor = 3;

	extern void configureWidgetFont(QWidget *widget, unsigned factor_id);
	extern void __configureWidgetFont(QWidget *widget, double factor);

	/*! \brief Creates a NumberedTextEditor instance automatically assigning it to 'parent'.
	  This method will create a layout if 'parent' doesn't has one. If parent has a layout
	  the method will do nothing. If parent is null creates an orphan object which means the
	  user must take care of the destruction of the object */
	extern NumberedTextEditor *createNumberedTextEditor(QWidget *parent, bool handle_ext_files = false);

	/*! \brief Creates an item in the specified QTreeWidget instance.
	  The new item is automatically inserted on the QTreeWidget object.
	  Setting word_wrap will create a QLabel instance into item's and assign the text to it. */
	extern QTreeWidgetItem *createOutputTreeItem(QTreeWidget *output_trw, const QString &text, const QPixmap &ico=QPixmap(),
												 QTreeWidgetItem *parent=nullptr, bool expand_item=true, bool word_wrap=false);

	//! \brief Creates an item in the specified QListWidget instance with the specified text and ico
	extern void createOutputListItem(QListWidget *output_lst, const QString &text, const QPixmap &ico=QPixmap(), bool is_formated=true);

	/*! \brief Toggles the SQL code for the object. This function also toggles the SQL of the references
	  related to the input object */
	extern void disableObjectSQL(BaseObject *object, bool disable);

	//! \brief Recursively toggles the specified object's references SQL
	extern void disableReferencesSQL(BaseObject *object);

	//! \brief Replaces the sequence of chars [`'] by html tags <strong></strong> and [()] by <em></em>
	extern QString formatMessage(const QString &msg);

	/*! \brief Fills a tree widget with all the stack trace provided by the passed Exception. A root item
	can be specified so all created child items are appended to it */
	extern void createExceptionsTree(QTreeWidget *exceptions_trw, Exception &e, QTreeWidgetItem *root);

	//! \brief Returns the path, in the icon resource, to the provided icon name
	extern QString getIconPath(const QString &icon);

	//! \brief Returns the path, in the icon resource, to the icon of the provided object type
	extern QString getIconPath(ObjectType obj_type);

	//! \brief Resizes the provided dialog considering font dpi changes as well screen size
	extern void resizeDialog(QWidget *dialog);

	//! brief Changes the values of the grid selection at once
	extern void bulkDataEdit(QTableWidget *results_tbw);

	//! \brief Creates drop shadown on a tool button that represents an QAction
	extern void createDropShadow(QToolButton *btn, int x_offset = 2, int y_offset = 2, int radius = 5);
}

#endif
