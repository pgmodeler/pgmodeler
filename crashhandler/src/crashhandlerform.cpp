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

#include "crashhandlerform.h"
#include "messagebox.h"
#include "pgmodeleruins.h"

const QString CrashHandlerForm::AnalysisMode("-analysis-mode");

CrashHandlerForm::CrashHandlerForm(bool analysis_mode, QWidget *parent, Qt::WindowFlags f) : BugReportForm(parent, f)
{
	QFile input;
	QString buf;
	QWidget *wgt=new QWidget;
	QHBoxLayout *layout=new QHBoxLayout;

	setWindowTitle(tr("pgModeler crash handler"));

	stack_txt=new QPlainTextEdit(this);
	stack_txt->setReadOnly(true);
	stack_txt->setFont(QFont("Source Code Pro"));
	stack_txt->setLineWrapMode(QPlainTextEdit::NoWrap);

	layout->addWidget(stack_txt);
	layout->setContentsMargins(4,4,4,4);
	wgt->setLayout(layout);

	logo_lbl->setPixmap(QPixmap(QString(":/imagens/imagens/crashhandler.png")));
	report_twg->addTab(wgt, tr("Stack trace"));

	//Open for reading the stack trace file generated on the last crash
	input.setFileName(GlobalAttributes::getTemporaryFilePath(GlobalAttributes::StacktraceFile));
	input.open(QFile::ReadOnly);

	if(input.isOpen())
	{
		buf=input.readAll();
		input.close();

		//Removes the stack trace file
		QDir stack_file;
		stack_file.remove(GlobalAttributes::getTemporaryFilePath(GlobalAttributes::StacktraceFile));

		//Shows the stacktrace loaded on the widget
		stack_txt->setPlainText(buf);
	}

	//Creating an input field in order to select the input report file
	input_wgt=new QWidget(this);
	input_wgt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	layout=new QHBoxLayout(input_wgt);
	layout->setContentsMargins(0, 0, 0, 0);

	input_lbl=new QLabel(input_wgt);
	input_lbl->setText(tr("Input:"));
	layout->addWidget(input_lbl);

	input_sel = new FileSelectorWidget(this);
	input_sel->setFileDialogTitle(tr("Select bug report file"));
	input_sel->setFileMode(QFileDialog::ExistingFile);
	input_sel->setAcceptMode(QFileDialog::AcceptOpen);
	input_sel->setNameFilters({ tr("pgModeler bug report (*.bug)"), tr("All files (*.*)") });
	input_sel->setToolTip(tr("Load report file for analysis"));
	layout->addWidget(input_sel);

	save_tb=new QToolButton(input_wgt);
	save_tb->setIcon(QPixmap(PgModelerUiNs::getIconPath("salvar")));
	save_tb->setSizePolicy(attach_tb->sizePolicy());
	save_tb->setToolButtonStyle(attach_tb->toolButtonStyle());
	save_tb->setIconSize(attach_tb->iconSize());
	save_tb->setToolTip(tr("Save the attached model file on the filesystem"));
	save_tb->setEnabled(false);
	attach_wgt->layout()->addWidget(save_tb);

	report_tab_grid->removeWidget(details_gb);
	report_tab_grid->removeWidget(output_wgt);
	report_tab_grid->removeWidget(message_frm);

	report_tab_grid->addWidget(input_wgt);
	report_tab_grid->addWidget(details_gb);
	report_tab_grid->addWidget(output_wgt);
	report_tab_grid->addWidget(message_frm);

	setAnalysisMode(analysis_mode);

	connect(input_sel, SIGNAL(s_fileSelected(QString)), this, SLOT(loadReport(QString)));
	connect(input_sel, SIGNAL(s_selectorCleared()), model_txt, SLOT(clear()));
	connect(input_sel, SIGNAL(s_selectorCleared()), details_txt, SLOT(clear()));
	connect(input_sel, SIGNAL(s_selectorCleared()), stack_txt, SLOT(clear()));
	connect(save_tb, SIGNAL(clicked()), this, SLOT(saveModel()));

	connect(model_txt, &QPlainTextEdit::textChanged, [&](){
			save_tb->setEnabled(!model_txt->toPlainText().isEmpty());
	});
}

void CrashHandlerForm::loadReport(QString filename)
{
	QFile input;
	QFileInfo fi;
	char *buf=nullptr;
	Messagebox msgbox;

	fi.setFile(filename);
	input.setFileName(filename);
	input.open(QFile::ReadOnly);

	//Raises an error if the file could not be opened
	if(!input.isOpen())
		msgbox.show(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename), Messagebox::ErrorIcon);
	else
	{
		QByteArray uncomp_buf;
		QString buf_aux, str_aux;
		int i, idx;
		QPlainTextEdit *txt_widgets[]={ details_txt, model_txt , stack_txt};

		//Creates a text buffer
		buf=new char[fi.size()];

		//Reads the file storing it on the buffer
		input.read(buf, fi.size());
		input.close();

		//Uncompress the buffer
		uncomp_buf.append(buf, fi.size());
		uncomp_buf=qUncompress(uncomp_buf);

		delete[](buf);
		buf=nullptr;

		buf_aux=QString(uncomp_buf.data());
		i=idx=0;

		//Showing the sections of the uncompressed buffer on the respective widgets
		while(i < buf_aux.size() && idx <= 2)
		{
			if(buf_aux.at(i).toLatin1()!=CharDelimiter)
				str_aux.append(buf_aux.at(i));
			else
			{
				txt_widgets[idx++]->setPlainText(str_aux);
				str_aux.clear();
			}
			i++;
		}
	}
}

void CrashHandlerForm::saveModel()
{
	QFileDialog file_dlg;

	try
	{
		file_dlg.setDefaultSuffix(QString("dbm"));
		file_dlg.setWindowTitle(tr("Save model"));
		file_dlg.setNameFilter(tr("Database model (*.dbm);;All files (*.*)"));
		file_dlg.setFileMode(QFileDialog::AnyFile);
		file_dlg.setAcceptMode(QFileDialog::AcceptSave);
		file_dlg.setModal(true);

		if(file_dlg.exec()==QFileDialog::Accepted)
		{
			QFile output(file_dlg.selectedFiles().at(0));
			QByteArray buf;

			output.open(QFile::WriteOnly);

			if(!output.isOpen())
				throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(file_dlg.selectedFiles().at(0)),
												ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			buf.append(model_txt->toPlainText());
			output.write(buf.data(),buf.size());
			output.close();
		}
	}
	catch(Exception &e)
	{
		Messagebox msgbox;
		msgbox.show(e);
	}
}

void CrashHandlerForm::setAnalysisMode(bool value)
{
	output_wgt->setEnabled(!value);
	attach_tb->setEnabled(!value);
	attach_mod_chk->setEnabled(!value);
	save_tb->setVisible(value);

	create_btn->setVisible(!value);
	input_wgt->setVisible(value);

	if(value)
	{
		title_lbl->setText(tr("pgModeler crash handler"));
		msg_lbl->setText(tr("Bug report analysis mode activated."));
	}
	else
	{
		title_lbl->setText(tr("Oops! pgModeler just crashed!"));
		msg_lbl->setText(tr("We apologize for what happened! It is clear that a nasty bug caused that. Please fill out the form below describing your actions before pgModeler quit unexpectedly. This will help on bug extermination and improve the software."));
	}
}

QByteArray CrashHandlerForm::generateReportBuffer()
{
	QByteArray buf=BugReportForm::generateReportBuffer();
	buf.append(stack_txt->toPlainText().toUtf8());
	buf.append(CharDelimiter);

	return buf;
}
