/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
#include "hinttextwidget.h"
#include <QWidget>

class CsvLoadWidget : public QWidget, Ui::CsvLoadWidget {
	private:
		Q_OBJECT

		HintTextWidget *col_names_ht;

		//! \brief Holds the names of columns extracted from the csv file
		QStringList csv_columns;

		//! \brief Holds the rows extracted from the csv file
		QList<QStringList> csv_rows;

	public:
		CsvLoadWidget(QWidget * parent = 0, bool cols_in_first_row = true);

		//! \brief Returns the extracted columns
		QStringList getCsvColumns(void);

		//! \brief Returns the extracted rows
		QList<QStringList> getCsvRows(void);

		//! \brief Returns a formatted CSV buffer by specifying a custom separator and line break
		QString getCsvBuffer(QString separator, QString line_break);

		bool isColumnsInFirstRow(void);

	private slots:
		void selectCsvFile(void);
		void loadCsvFile(void);

	signals:
		void s_csvFileLoaded(void);
};

#endif
