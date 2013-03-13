/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "crashhandler.h"

const char CrashHandler::CHR_DELIMITER=static_cast<char>(3);

CrashHandler::CrashHandler(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	ifstream input;
	QString buf;
	char lin[1024];

	setupUi(this);
	connect(cancel_btn, SIGNAL(clicked(void)), this, SLOT(close(void)));
	connect(create_btn, SIGNAL(clicked(void)), this, SLOT(generateReport(void)));
	connect(actions_txt, SIGNAL(textChanged(void)), this, SLOT(enableGeneration(void)));

	//Open for reading the stack trace file generated on the last crash
	input.open((GlobalAttributes::TEMPORARY_DIR +
							GlobalAttributes::DIR_SEPARATOR +
							GlobalAttributes::STACKTRACE_FILE).toStdString().c_str());

	while(input.is_open() && !input.eof())
	{
		input.getline(lin, sizeof(lin), '\n');
		buf += QString("%1\n").arg(lin);
	}

	input.close();

	//Removes the stack trace file
	QDir stack_file;
	stack_file.remove(GlobalAttributes::TEMPORARY_DIR +
										GlobalAttributes::DIR_SEPARATOR +
										GlobalAttributes::STACKTRACE_FILE);

	//Shows the stacktrace loaded on the widget
	stack_txt->setPlainText(buf);

	//Installs a syntax highlighter on model_txt widget
	hl_model_txt=new SyntaxHighlighter(model_txt, false);
	hl_model_txt->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																	GlobalAttributes::DIR_SEPARATOR +
																	GlobalAttributes::XML_HIGHLIGHT_CONF +
																	GlobalAttributes::CONFIGURATION_EXT);

	QDir tmp_dir=QDir(GlobalAttributes::TEMPORARY_DIR, "*.dbm", QDir::Name, QDir::Files | QDir::NoDotAndDotDot);
	tmp_dir.setSorting(QDir::Time);
	QStringList lista=tmp_dir.entryList();

	if(!lista.isEmpty())
	{
		//Opens the last modified model file showing it on the proper widget
		input.open((GlobalAttributes::TEMPORARY_DIR +
								GlobalAttributes::DIR_SEPARATOR + lista[0]).toStdString().c_str());

		buf.clear();
		while(input.is_open() && !input.eof())
		{
			input.getline(lin, sizeof(lin), '\n');
			buf += QString("%1\n").arg(lin);
		}

		input.close();
		model_txt->setPlainText(Utf8String::create(buf));
	}
}

void CrashHandler::enableGeneration(void)
{
	create_btn->setEnabled(!actions_txt->toPlainText().isEmpty());
}

void CrashHandler::loadReport(const QString &filename)
{
	ifstream input;
	QFileInfo fi;
	char *buf=NULL;
	MessageBox msgbox;

	fi.setFile(filename);
	input.open(filename.toStdString().c_str());

	title_lbl->setText(trUtf8("pgModeler crash file analysis"));
	create_btn->setVisible(false);
	msg_lbl->clear();

	//Raises an error if the file could not be opened
	if(!input.is_open())
		msgbox.show(trUtf8("Error"), Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED).arg(filename), MessageBox::ERROR_ICON);
	else
	{
		QByteArray uncomp_buf;
		QString buf_aux, str_aux;
		int i, idx;
		QTextEdit *txt_widgets[]={ actions_txt, model_txt , stack_txt};

		msg_lbl->setText(trUtf8("File: %1\nSize: %2 bytes\n\n").arg(filename).arg(fi.size()));

		//Creates a text buffer
		buf=new char[fi.size()];

		//Reads the file storing it on the buffer
		input.read(buf, fi.size());
		input.close();

		//Uncompress the buffer
		uncomp_buf.append(buf, fi.size());
		uncomp_buf=qUncompress(uncomp_buf);

		delete[](buf);
		buf=NULL;

		buf_aux=QString(uncomp_buf.data());
		i=idx=0;

		//Showing the sections of the uncompressed buffer on the respective widgets
		while(i < buf_aux.size() && idx <= 2)
		{
			if(buf_aux.at(i).toAscii()!=CHR_DELIMITER)
				str_aux.append(buf_aux.at(i));
			else
			{
				txt_widgets[idx++]->setPlainText(Utf8String::create(str_aux));
				str_aux.clear();
			}
			i++;
		}
	}
}

void CrashHandler::generateReport(void)
{
	MessageBox msgbox;
	QByteArray buf, comp_buf;
	ofstream output;

	//Configures the path to the .crash file generated
	QString crash_file=(GlobalAttributes::TEMPORARY_DIR +
											GlobalAttributes::DIR_SEPARATOR +
											GlobalAttributes::CRASH_HANDLER_FILE).arg(QDateTime::currentDateTime().toString("_yyyyMMdd_hhmm"));

	//Opens the file for writting
	output.open(crash_file.toStdString().c_str());

	if(!output.is_open())
		msgbox.show(trUtf8("Error"), Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(crash_file), MessageBox::ERROR_ICON);
	else
	{
		buf.append(actions_txt->toPlainText().toUtf8());
		buf.append(CHR_DELIMITER);

		if(attach_mod_chk->isChecked())
			buf.append(model_txt->toPlainText().toUtf8());
		buf.append(CHR_DELIMITER);

		buf.append(stack_txt->toPlainText().toUtf8());
		buf.append(CHR_DELIMITER);

		//Compress the buffer (using zlib) in a compression rate at 8
		comp_buf=qCompress(buf, 8);

		//Saves the buffer
		output.write(comp_buf.data(), comp_buf.size());
		output.close();

		msgbox.show(trUtf8("Information"), trUtf8("Crash report successfuly generated! Please send the file '%1' to %2 in order be debugged. Thank you for the collaboration!").arg(crash_file).arg("rkhaotix@gmail.com"), MessageBox::INFO_ICON);
		this->close();
	}
}
