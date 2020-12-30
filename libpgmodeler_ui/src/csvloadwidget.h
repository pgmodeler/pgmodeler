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
\ingroup libpgmodeler_ui
\class CsvLoadWidget
\brief Implements basic features to load csv files and return the loaded buffer to the application
*/

#ifndef CSV_LOAD_WIDGET_H
#define CSV_LOAD_WIDGET_H

#include "ui_csvloadwidget.h"
#include "fileselectorwidget.h"

class CsvLoadWidget : public QWidget, Ui::CsvLoadWidget {
	private:
		Q_OBJECT

		//! \brief Holds the names of columns extracted from the csv file
		QStringList csv_columns;

		//! \brief Holds the rows extracted from the csv file
		QList<QStringList> csv_rows;

		FileSelectorWidget *file_sel;

	public:
		CsvLoadWidget(QWidget * parent = nullptr, bool cols_in_first_row = true);

		//! \brief Returns the extracted columns
		QStringList getCsvColumns();

		//! \brief Returns the extracted rows
		QList<QStringList> getCsvRows();

		//! \brief Returns a formatted CSV buffer by specifying a custom separator and line break
		QString getCsvBuffer(QString separator, QString line_break);

		bool isColumnsInFirstRow();

		/*! \brief Loads a csv document from a buffer and stores the result in the internal csv_columns and csv_rows attributes for later usage.
		 * The separator and text delimiter chars can be specified overriding the ones configured in the widget */
		void loadCsvBuffer(const QString csv_buffer, const QString &separator, const QString &text_delim, bool cols_in_first_row);

		//! \brief Loads a csv document from a buffer and stores the result in the internal csv_columns and csv_rows attributes for later usage.
		void loadCsvBuffer(const QString csv_buffer);

		QString getSeparator();

		/*! \brief Loads a csv document from a buffer. The user can specify the value separator, text delimiter and an object which will store the column names.
		 *  In that case, the column names are only extracted from the first row if the cols_in_first_row is true */
		static QList<QStringList> loadCsvFromBuffer(const QString &csv_buffer, const QString &separator, const QString &text_delim, bool cols_in_first_row, QStringList &csv_cols);

	private slots:
		void loadCsvFile();

	signals:
		void s_csvFileLoaded();
};

#endif
