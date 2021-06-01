/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2021 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "schemaeditorform.h"
#include "pgmodeleruins.h"
#include "globalattributes.h"
#include "generalconfigwidget.h"

SchemaEditorForm::SchemaEditorForm(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	GeneralConfigWidget general_conf_wgt;
	general_conf_wgt.loadConfiguration();

	syntax_txt = PgModelerUiNs::createNumberedTextEditor(syntax_wgt);
	//syntax_hl = new SyntaxHighlighter(syntax_txt);
	//syntax_hl->loadConfiguration(GlobalAttributes::getXMLHighlightConfPath());

	syntax_conf_sel = new FileSelectorWidget(syntax_conf_wgt);

	QVBoxLayout *vbox = new QVBoxLayout(syntax_conf_wgt);
	vbox->addWidget(syntax_conf_sel);
	vbox->setContentsMargins(0, 0, 0, 0);

	syntax_conf_sel->setSelectedFile(GlobalAttributes::getSchHighlightConfPath());
	syntax_conf_sel->setNameFilters({ tr("Schema micro-language syntax file (%1.conf)").arg(GlobalAttributes::SchHighlightConf) });

	connect(syntax_conf_sel, SIGNAL(s_selectorChanged(bool)), this, SLOT(loadSyntaxConfig()));
	connect(syntax_conf_sel, SIGNAL(s_selectorCleared()), this, SLOT(clearSyntaxConfig()));

	connect(apply_conf_tb, &QToolButton::clicked, [&](){
		saveSyntaxConfig();
		applySyntaxConfig();
	});

	connect(save_conf_tb, &QToolButton::clicked, [&](){
		saveSyntaxConfig();
		applySyntaxConfig();
	});

	connect(new_sch_tb, SIGNAL(clicked(bool)), this, SLOT(addEditorTab()));
	connect(load_sch_tb, SIGNAL(clicked(bool)), this, SLOT(loadSchemaFile()));
	connect(editors_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(closeEditorTab(int)));
}

SchemaEditorForm::~SchemaEditorForm()
{
	while(editors_tbw->count() > 0)
		closeEditorTab(0);
}

void SchemaEditorForm::showEvent(QShowEvent *)
{
	splitter->setSizes({ width()/2, width()/3});
	loadSyntaxConfig();
}

void SchemaEditorForm::loadSyntaxConfig()
{
	QFile input;
	Messagebox msgbox;
	QString filename = syntax_conf_sel->getSelectedFile();

	input.setFileName(filename);
	input.open(QFile::ReadOnly);

	if(!input.isOpen())
		msgbox.show(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename), Messagebox::ErrorIcon);
	else
	{
		syntax_txt->setPlainText(input.readAll());
		input.close();

		save_conf_tb->setEnabled(true);
		apply_conf_tb->setEnabled(true);
	}
}

void SchemaEditorForm::applySyntaxConfig()
{
	for(auto &hl : highlighters)
	{
		try
		{
			if(!syntax_conf_sel->getSelectedFile().isEmpty())
			{
				hl->loadConfiguration(syntax_conf_sel->getSelectedFile());
				hl->rehighlight();
			}
		}
		catch(Exception &e)
		{
			Messagebox msg;
			msg.show(e);
			break;
		}
	}
}

void SchemaEditorForm::saveSyntaxConfig()
{
	QFile input;
	Messagebox msgbox;
	QString filename = syntax_conf_sel->getSelectedFile();

	input.setFileName(filename);
	input.open(QFile::WriteOnly);

	if(!input.isOpen())
		msgbox.show(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename), Messagebox::ErrorIcon);
	else
	{
		input.write(syntax_txt->toPlainText().toUtf8());
		input.close();
	}
}

void SchemaEditorForm::clearSyntaxConfig()
{
	syntax_txt->clear();
	save_conf_tb->setEnabled(false);
	apply_conf_tb->setEnabled(false);
}

void SchemaEditorForm::loadSchemaFile()
{
	QFileDialog file_dlg;

	try
	{
		file_dlg.setNameFilter(tr("Schema micro-language file (*.sch)"));
		file_dlg.setWindowIcon(QPixmap(PgModelerUiNs::getIconPath("pgsqlModeler48x48")));
		file_dlg.setWindowTitle(tr("Load schema file"));
		file_dlg.setFileMode(QFileDialog::ExistingFiles);
		file_dlg.setAcceptMode(QFileDialog::AcceptOpen);

		if(file_dlg.exec() == QFileDialog::Accepted)
			loadSchemaFiles(file_dlg.selectedFiles());
	}
	catch(Exception &e)
	{
		Messagebox msg_box;
		msg_box.show(e);
	}
}

void SchemaEditorForm::loadSchemaFiles(const QStringList &filenames)
{
	try
	{
		for(auto &file : filenames)
			addEditorTab(file);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

void SchemaEditorForm::addEditorTab(const QString &filename)
{
	NumberedTextEditor *editor_txt = nullptr;
	SyntaxHighlighter *editor_hl = nullptr;
	QWidget *page_wgt = nullptr;
	QVBoxLayout *vbox = nullptr;
	QFileInfo fi(filename);
	QFile input;
	QByteArray buffer;

	if(!filename.isEmpty())
	{
		input.setFileName(filename);
		input.open(QFile::ReadOnly);

		if(!input.isOpen())
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
											ErrorCode::FileDirectoryNotAccessed, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		}

		buffer = input.readAll();
		input.close();
	}

	page_wgt = new QWidget;
	vbox = new QVBoxLayout(page_wgt);
	vbox->setContentsMargins(4,4,4,4);

	editor_txt = PgModelerUiNs::createNumberedTextEditor(page_wgt);
	editor_hl = new SyntaxHighlighter(editor_txt);
	vbox->addWidget(editor_txt);

	try
	{
		if(!syntax_conf_sel->getSelectedFile().isEmpty())
			editor_hl->loadConfiguration(syntax_conf_sel->getSelectedFile());

		editor_txt->setPlainText(buffer);
		highlighters.append(editor_hl);
	}
	catch(Exception &e)
	{
		Messagebox msgbox;
		msgbox.show(e);
	}

	editors_tbw->addTab(page_wgt, filename.isEmpty() ? tr("(unsaved)") : fi.baseName());
	editors_tbw->setTabToolTip(editors_tbw->count() - 1, filename.isEmpty() ? "" : fi.absoluteFilePath());
	editors_tbw->setCurrentIndex(editors_tbw->count() - 1);
}

void SchemaEditorForm::closeEditorTab(int idx)
{
	QWidget *page = editors_tbw->widget(idx);
	editors_tbw->removeTab(idx);
	highlighters.removeAt(idx);
	delete(page);
}
