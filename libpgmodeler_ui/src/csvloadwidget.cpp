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

#include "csvloadwidget.h"
#include <QFileDialog>
#include "exception.h"
#include <QTextStream>

CsvLoadWidget::CsvLoadWidget(QWidget * parent, bool cols_in_first_row) : QWidget(parent)
{
	setupUi(this);
	separator_edt->setVisible(false);

	if(!cols_in_first_row)
	{
		col_names_ht=new HintTextWidget(col_names_hint, this);
		col_names_ht->setText(col_names_chk->statusTip());
	}
	else
	{
		col_names_ht=nullptr;
		col_names_chk->setVisible(false);
		col_names_chk->setChecked(true);
	}

	connect(select_file_tb, SIGNAL(clicked(bool)), this, SLOT(selectCsvFile()));
	connect(txt_delim_chk, SIGNAL(toggled(bool)), txt_delim_edt, SLOT(setEnabled(bool)));
	connect(load_btn, SIGNAL(clicked(bool)), this, SLOT(loadCsvFile()));

	connect(separator_cmb, &QComboBox::currentTextChanged, [&](){
			separator_edt->setVisible(separator_cmb->currentIndex() == separator_cmb->count()-1);
	});

	connect(file_edt, &QLineEdit::textChanged, [&](){
		load_btn->setEnabled(!file_edt->text().isEmpty());
	});
}

QStringList CsvLoadWidget::getCsvColumns(void)
{
	return(csv_columns);
}

QList<QStringList> CsvLoadWidget::getCsvRows(void)
{
	return(csv_rows);
}

void CsvLoadWidget::selectCsvFile(void)
{
	QFileDialog file_dlg;

	file_dlg.setWindowTitle(trUtf8("Load CSV file"));
	file_dlg.setModal(true);
	file_dlg.setNameFilter(trUtf8("Comma-separted values (*.csv);;All files (*.*)"));

	if(file_dlg.exec()==QFileDialog::Accepted)
	{
		QString file;

		if(!file_dlg.selectedFiles().isEmpty())
			file = file_dlg.selectedFiles().at(0);

		file_edt->setText(file);
	}
}

void CsvLoadWidget::loadCsvFile(void)
{
	QFile file;
	QString csv_buffer;

	file.setFileName(file_edt->text());

	if(!file.open(QFile::ReadOnly))
		throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED).arg(file_edt->text()),
										ERR_FILE_DIR_NOT_ACCESSED,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	csv_columns.clear();
	csv_rows.clear();

	csv_buffer.append(file.readAll());

	if(!csv_buffer.isEmpty())
	{
		QString	double_quote=QString("%1%1").arg(txt_delim_edt->text()),
				placeholder = QString("⁋"), separator;
		QStringList separators={ QString(";"), QString(","), QString(" "), QString("\t") },
				values, rows;
		QRegExp empty_val;

		//If no custom separator is specified we use the default ';'
		separators += (separator_edt->text().isEmpty() ? QString(";") : separator_edt->text());
		separator = separators[separator_cmb->currentIndex()];
		rows=csv_buffer.split(QChar::LineFeed, QString::SkipEmptyParts);

		if(col_names_chk->isChecked())
		{
			csv_columns=rows[0].split(separator);
			rows.removeAt(0);
		}

		//Configuring an regexp to remove empty quoted values, e.g, "",
		if(txt_delim_chk->isChecked() && !txt_delim_edt->text().isEmpty())
			empty_val = QRegExp(QString("(\\%1\\%1)(\\%2)").arg(txt_delim_edt->text()).arg(separator));

		for(QString row : rows)
		{
			if(!empty_val.pattern().isEmpty())
				row.replace(empty_val, separator);

			/* In order to preserve double quotes (double delimiters) inside the values,
			 we first replace them by a placeholder, erase the delimiters and restore the previous value */
			row.replace(double_quote, placeholder);
			row.replace(txt_delim_edt->text(), QString());
			row.replace(placeholder, double_quote);

			values = row.split(separator);
			for(int i =0; i < values.count(); i++)
				values[i] = values[i].trimmed();

			csv_rows.append(values);
		}
	}

	file_edt->clear();
	emit s_csvFileLoaded();
}

QString CsvLoadWidget::getCsvBuffer(QString separator, QString line_break)
{
	QString buffer;
	QStringList rows;

	if(separator.isEmpty())
		separator = QString(";");

	if(line_break.isEmpty())
		line_break = QString("\n");

	buffer+=csv_columns.join(separator) + line_break;

	for(QStringList row : csv_rows)
		rows+=row.join(separator);

	buffer+=rows.join(line_break);

	return(buffer);
}

bool CsvLoadWidget::isColumnsInFirstRow(void)
{
	return(col_names_chk->isChecked());
}
