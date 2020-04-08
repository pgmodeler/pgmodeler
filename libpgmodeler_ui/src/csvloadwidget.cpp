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

#include "csvloadwidget.h"
#include <QFileDialog>
#include "exception.h"
#include <QTextStream>

CsvLoadWidget::CsvLoadWidget(QWidget * parent, bool cols_in_first_row) : QWidget(parent)
{
	setupUi(this);

	file_sel = new FileSelectorWidget(this);
	file_sel->setFileMode(QFileDialog::ExistingFile);
	file_sel->setFileDialogTitle(tr("Load CSV file"));
	file_sel->setMimeTypeFilters({"text/csv", "application/octet-stream"});
	load_csv_grid->addWidget(file_sel, 0, 1, 1, 8);

	separator_edt->setVisible(false);

	if(cols_in_first_row)
	{
		col_names_chk->setVisible(false);
		col_names_chk->setChecked(true);
	}

	connect(txt_delim_chk, SIGNAL(toggled(bool)), txt_delim_edt, SLOT(setEnabled(bool)));
	connect(load_btn, SIGNAL(clicked(bool)), this, SLOT(loadCsvFile()));

	connect(separator_cmb, &QComboBox::currentTextChanged, [&](){
			separator_edt->setVisible(separator_cmb->currentIndex() == separator_cmb->count()-1);
	});

	connect(file_sel, SIGNAL(s_selectorChanged(bool)), load_btn, SLOT(setEnabled(bool)));
}

QStringList CsvLoadWidget::getCsvColumns()
{
	return csv_columns;
}

QList<QStringList> CsvLoadWidget::getCsvRows()
{
	return csv_rows;
}

QList<QStringList> CsvLoadWidget::loadCsvFromBuffer(const QString &csv_buffer, const QString &separator, const QString &text_delim, bool cols_in_first_row, QStringList &csv_cols)
{
	QList<QStringList> csv_rows;

	if(!csv_buffer.isEmpty())
	{
		QString	double_quote=QString("%1%1").arg(text_delim),
				placeholder = QString(QChar::ReplacementCharacter),
				escaped_delim = QString("\u2020"), //Unicode char to be used as a placeholder for escaped text delimiter, e.g., \"
				escaped_sep = QString("\u2052"),  //Unicode char to be used as a placeholder for escaped value separator, e.g., \;
				aux_buffer = csv_buffer,
				win_line_break = QString("%1%2").arg(QChar(QChar::CarriageReturn)).arg(QChar(QChar::LineFeed)),
				mac_line_break = QString("%1").arg(QChar(QChar::CarriageReturn));
		QStringList values, rows;
		QRegExp empty_val;

		if(aux_buffer.contains(win_line_break))
			aux_buffer.replace(win_line_break, QString(QChar::LineFeed));
		else if(aux_buffer.contains(mac_line_break))
			aux_buffer.replace(mac_line_break, QString(QChar::LineFeed));

		/* In order to avoid wrong replacement of escapade both text delimiter and value separator
		 * we replace them by their respective placeholders in order to revert them back to their
		 * original representation on the final (formated) string */
		aux_buffer.replace(QString("\\%1").arg(text_delim), escaped_delim);
		aux_buffer.replace(QString("\\%1").arg(separator), escaped_sep);

		if(cols_in_first_row)
		{
			int lf_idx = aux_buffer.indexOf(QChar::LineFeed);

			if(lf_idx < 0)
				lf_idx = aux_buffer.size();

			csv_cols=aux_buffer.mid(0, lf_idx).split(separator);
			csv_cols.replaceInStrings(text_delim, QString());

			//Replace the escaped separator and delimiter by their original form in the col names
			csv_cols.replaceInStrings(escaped_sep, separator);
			csv_cols.replaceInStrings(escaped_delim, text_delim);

			aux_buffer.replace(0, lf_idx + 1, QString());
		}

		aux_buffer.replace(QString("%1%2").arg(QChar(QChar::LineFeed)).arg(text_delim), placeholder);
		rows = aux_buffer.split(placeholder, QString::SkipEmptyParts);

		//Configuring an regexp to remove empty quoted values, e.g, "",
		if(!text_delim.isEmpty())
			empty_val = QRegExp(QString("(\\%1\\%1)(\\%2)").arg(text_delim).arg(separator));

		for(QString row : rows)
		{
			if(!empty_val.pattern().isEmpty())
				row.replace(empty_val, separator);

			/* In order to preserve double quotes (double delimiters) inside the values,
			 * we first replace them by a placeholder, erase the delimiters and restore the previous value */
			row.replace(double_quote, placeholder);
			row.replace(text_delim, QString());
			row.replace(placeholder, double_quote);

			values = row.split(separator);

			for(int i =0; i < values.count(); i++)
			{
				//Replace the escaped separator and delimiter by their original form in the final value
				values[i].replace(escaped_sep, separator);
				values[i].replace(escaped_delim, text_delim);

				values[i] = values[i].trimmed();
			}

			csv_rows.append(values);
		}
	}

	return csv_rows;
}

void CsvLoadWidget::loadCsvFile()
{
	QFile file;
	QString csv_buffer;

	file.setFileName(file_sel->getSelectedFile());

	if(!file.open(QFile::ReadOnly))
		throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(file_sel->getSelectedFile()),
										ErrorCode::FileDirectoryNotAccessed,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	csv_columns.clear();
	csv_rows.clear();

	csv_buffer.append(file.readAll());

	if(!csv_buffer.isEmpty())
	{
		csv_rows = loadCsvFromBuffer(csv_buffer, getSeparator(),
																 txt_delim_chk->isChecked() ? txt_delim_edt->text() : QString(),
																 col_names_chk->isChecked(), csv_columns);
	}

	file_sel->clearSelector();
	emit s_csvFileLoaded();
}

QString CsvLoadWidget::getSeparator()
{
	QStringList separators={ QString(";"), QString(","), QString(" "), QString("\t") };
	separators += (separator_edt->text().isEmpty() ? QString(";") : separator_edt->text());
	return separators[separator_cmb->currentIndex()];
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

	return buffer;
}

bool CsvLoadWidget::isColumnsInFirstRow()
{
	return col_names_chk->isChecked();
}

void CsvLoadWidget::loadCsvBuffer(const QString csv_buffer, const QString &separator, const QString &text_delim, bool cols_in_first_row)
{
	csv_columns.clear();
	csv_rows.clear();
	csv_rows = loadCsvFromBuffer(csv_buffer, separator, text_delim, cols_in_first_row, csv_columns);
}

void CsvLoadWidget::loadCsvBuffer(const QString csv_buffer)
{
	loadCsvBuffer(csv_buffer, getSeparator(),
								txt_delim_chk->isChecked() ? txt_delim_edt->text() : QString(),
								col_names_chk->isChecked());
}
