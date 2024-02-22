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
\ingroup libcore
\namespace GuiUtilsNs
\brief This namespace is used to organize all functions or constants used in the libgui package.
*/

#ifndef GUI_UTILS_NS_H
#define GUI_UTILS_NS_H

#include <QTreeWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QPixmap>
#include <QFileDialog>
#include "baseobject.h"
#include "widgets/numberedtexteditor.h"
#include "csvdocument.h"

namespace GuiUtilsNs {
	static constexpr int LtMargin = 5,
	LtSpacing = 5;

	enum FontFactorId: unsigned {
		SmallFontFactor,
		MediumFontFactor,
		BigFontFactor,
		HugeFontFactor
	};

	extern __libgui void configureWidgetFont(QWidget *widget, FontFactorId factor_id);
	extern __libgui void __configureWidgetFont(QWidget *widget, double factor);

	/*! \brief Creates a NumberedTextEditor instance automatically assigning it to 'parent'.
		This method will create a layout if 'parent' doesn't has one. If parent has a layout
		the method will do nothing. If parent is null creates an orphan object which means the
		user must take care of the destruction of the object */
	extern __libgui NumberedTextEditor *createNumberedTextEditor(QWidget *parent, bool handle_ext_files = false, qreal custom_fnt_size = 0);

	/*! \brief Creates an item in the specified QTreeWidget instance.
		The new item is automatically inserted on the QTreeWidget object.
		Setting word_wrap will create a QLabel instance into item's and assign the text to it. */
	extern __libgui QTreeWidgetItem *createOutputTreeItem(QTreeWidget *output_trw, const QString &text, const QPixmap &ico=QPixmap(),
																													QTreeWidgetItem *parent=nullptr, bool expand_item=true, bool word_wrap=false);

	//! \brief Creates an item in the specified QListWidget instance with the specified text and ico
	extern __libgui void createOutputListItem(QListWidget *output_lst, const QString &text, const QPixmap &ico=QPixmap(), bool is_formated=true);

	/*! \brief Toggles the SQL code for the object. This function also toggles the SQL of the references
		related to the input object */
	extern __libgui void disableObjectSQL(BaseObject *object, bool disable);

	//! \brief Recursively toggles the specified object's references SQL
	extern __libgui void disableReferencesSQL(BaseObject *object);

	/*! \brief Fills a tree widget with all the stack trace provided by the passed Exception. A root item
	can be specified so all created child items are appended to it */
	extern __libgui void createExceptionsTree(QTreeWidget *exceptions_trw, Exception &e, QTreeWidgetItem *root);

	//! \brief Returns the path, in the icon resource, to the provided icon name
	extern __libgui QString getIconPath(const QString &icon);

	/*! \brief Returns the path, in the icon resource, to the icon of the provided object type
	 *  The parameter sub_type is used to retrive the sub type icon. Currently only relationships
	 *  and constraints have sub types being for relationships the Relationship::RelType enum and
	 *  constraints the class ConstraintType */
	extern __libgui QString getIconPath(ObjectType obj_type, int sub_type = -1);

	//! \brief Resizes the provided dialog considering font dpi changes as well screen size
	extern __libgui void resizeDialog(QWidget *dialog);

	//! brief Changes the values of the grid selection at once
	extern __libgui void openColumnDataForm(QTableWidget *results_tbw);

	//! brief Opens the bulk data edit in readonly mode and display the text of the provided model index
	extern __libgui void openColumnDataForm(const QModelIndex &index);

	//! \brief Creates drop shadown on a widget
	extern __libgui void createDropShadow(QWidget *wgt, int x_offset = 2, int y_offset = 2, int radius = 5, const QColor &color = QColor(0, 0, 0, 100));

	//! \brief Creates drop shadown on a widget based on the current UI theme
	extern __libgui void updateDropShadow(QWidget *wgt);

	//! \brief Changes drop shadows color and offset to match the current theme.
	extern __libgui void updateDropShadows(QWidgetList widgets, const QString &class_name = "QToolButton");

	/*! \brief Handles the currently provided file dialog state to file.
	 * If save_state is true, then the dialog's state is saved to file
	 * otherwise is restored */
	extern __libgui void handleFileDialogState(QFileDialog *file_dlg, bool save_state);

	//! \brief Saves the currently provided file dialog state to file
	extern  __libgui void saveFileDialogState(QFileDialog *file_dlg);

	//! \brief Restores the saved filed dialog from file/memory and applies to the provided file dialog
	extern __libgui void restoreFileDialogState(QFileDialog *file_dlg);

	/*! \brief Lists the objects in a QTableView using a list of database model objects as data source.
	 * The first column on each row of table contains the reference to the object.
	 * The parameter search_attr is used to display the attribute value in which the search was performed. */
	extern __libgui void populateObjectsTable(QTableView *tab_view, const std::vector<BaseObject *> &objects, const QString &search_attr = "");

	//! \brief Lists the objects in a QTableView using a list of object attributes (see Catalog::getObjects) as data source.
	extern __libgui void populateObjectsTable(QTableView *tab_view, const std::vector<attribs_map> &attribs);

	//! \brief Populates the provided table widget with the data in a parsed CSV document.
	extern __libgui void populateTable(QTableWidget *tab_tbw, const CsvDocument &csv_doc);

	/*! \brief Opens an instance of QFileDialog with the provided attributes.
	 *  The method returns the files/directories selected by the user in the dialog */
	extern __libgui QStringList selectFiles(const QString &title = QT_TR_NOOP("Select file..."), QFileDialog::FileMode file_mode = QFileDialog::AnyFile,
																					QFileDialog::AcceptMode accept_mode = QFileDialog::AcceptOpen,	const QStringList &name_filters = {},
																					const QStringList &mime_filters = {}, const QString &default_suffix = "", const QString &selected_file = "");

	/*! \brief Writes the provided buffer to a file which path is selected via QFileDialog.
	 * Custom attributes for the QFileDialog instance can be provided.
	 * Raises an exception in case of the file couldn't be saved.
	 * The method does nothing if file_mode is different from QFileDialog::ExistingFile and QFileDialog::AnyFile */
	extern __libgui bool selectAndSaveFile(const QByteArray &buffer, const QString &title = QT_TR_NOOP("Save file..."),
																				 QFileDialog::FileMode file_mode = QFileDialog::AnyFile, const QStringList &name_filters = {},
																				 const QStringList &mime_filters = {}, const QString &default_suffix = "", const QString &selected_file = "");

	/*! \brief Loads a file which path is selected via QFileDialog returning the buffer with the contents of the file.
	 * Custom attributes for the QFileDialog instance can be provided.
	 * Raises an exception in case of the file couldn't be loaded.
	 * The method does nothing if file_mode is different from QFileDialog::ExistingFile and QFileDialog::AnyFile */
	extern __libgui  bool selectAndLoadFile(QByteArray &buffer,
																					const QString &title = QT_TR_NOOP("Load file..."), QFileDialog::FileMode file_mode = QFileDialog::AnyFile,
																					const QStringList &name_filters = {}, const QStringList &mime_filters = {}, const QString &selected_file = "");

	//! \brief Creates an action in a QLineEdit that controls the visibility of passwords
	extern __libgui void createPasswordShowAction(QLineEdit *parent_edt);
}

#endif
