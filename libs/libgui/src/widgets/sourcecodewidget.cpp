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

#include "sourcecodewidget.h"
#include "taskprogresswidget.h"
#include "guiutilsns.h"
#include "pgsqlversions.h"

SourceCodeWidget::SourceCodeWidget(QWidget *parent): BaseObjectWidget(parent)
{
	Ui_SourceCodeWidget::setupUi(this);
	configureFormLayout(codigofonte_grid, ObjectType::BaseObject);
	comment_lbl->setVisible(false);
	comment_edt->setVisible(false);

	hl_sqlcode=nullptr;
	hl_xmlcode=nullptr;

	sqlcode_txt=GuiUtilsNs::createNumberedTextEditor(sqlcode_wgt);
	sqlcode_txt->setReadOnly(true);

	xmlcode_txt=GuiUtilsNs::createNumberedTextEditor(xmlcode_wgt);
	xmlcode_txt->setReadOnly(true);

	name_edt->setReadOnly(true);
	version_cmb->addItems(PgSqlVersions::AllVersions);

	connect(version_cmb, &QComboBox::currentIndexChanged, this, &SourceCodeWidget::generateSourceCode);
	connect(code_options_cmb, &QComboBox::currentIndexChanged, this, &SourceCodeWidget::generateSourceCode);
	connect(sourcecode_twg, &QTabWidget::currentChanged, this, &SourceCodeWidget::setSourceCodeTab);
	connect(save_sql_tb, &QToolButton::clicked, this, &SourceCodeWidget::saveSQLCode);

	search_sql_wgt = new SearchReplaceWidget(sqlcode_txt, search_sql_wgt_parent);
	search_sql_wgt_parent->setVisible(false);

	QVBoxLayout *vbox = new QVBoxLayout(search_sql_wgt_parent);
	vbox->addWidget(search_sql_wgt);
	vbox->setContentsMargins(0,0,0,0);

	search_xml_wgt = new SearchReplaceWidget(xmlcode_txt, search_xml_wgt_parent);
	search_xml_wgt_parent->setVisible(false);

	vbox = new QVBoxLayout(search_xml_wgt_parent);
	vbox->addWidget(search_xml_wgt);
	vbox->setContentsMargins(0,0,0,0);

	connect(search_sql_tb, &QToolButton::toggled, search_sql_wgt_parent, &QWidget::setVisible);
	connect(search_sql_wgt, &SearchReplaceWidget::s_hideRequested, search_sql_tb, &QToolButton::toggle);

	connect(search_xml_tb, &QToolButton::toggled, search_xml_wgt_parent, &QWidget::setVisible);
	connect(search_xml_wgt, &SearchReplaceWidget::s_hideRequested, search_xml_tb, &QToolButton::toggle);

	hl_sqlcode=new SyntaxHighlighter(sqlcode_txt);
	hl_xmlcode=new SyntaxHighlighter(xmlcode_txt);

	setMinimumSize(640, 540);
}

void SourceCodeWidget::setSourceCodeTab(int)
{
	QString code_icon;
	bool enabled=false;
	ObjectType obj_type=object->getObjectType();

	enabled=(sourcecode_twg->currentIndex()==0 &&
			 ((obj_type==ObjectType::BaseRelationship &&
			   dynamic_cast<BaseRelationship *>(object)->getRelationshipType()==BaseRelationship::RelationshipFk)
			  || (obj_type!=ObjectType::BaseRelationship && obj_type!=ObjectType::Textbox)));

	version_cmb->setEnabled(enabled);
	pgsql_lbl->setEnabled(enabled);
}

void SourceCodeWidget::saveSQLCode()
{
	try
	{
		GuiUtilsNs::selectAndSaveFile(sqlcode_txt->toPlainText().toUtf8(),
																	 tr("Save SQL code as..."),
																	 QFileDialog::AnyFile,
																	 { tr("SQL code (*.sql)"), tr("All files (*.*)") }, {}, "sql",
																	 QString("%1-%2.sql").arg(object->getSchemaName(), object->getName()));
	}
	catch(Exception &e)
	{
		Messagebox::error(e, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}
}

void SourceCodeWidget::generateSourceCode(int)
{
	ObjectType obj_type;
	TaskProgressWidget *task_prog_wgt=nullptr;

	try
	{
		sqlcode_txt->clear();
		xmlcode_txt->clear();

		qApp->setOverrideCursor(Qt::WaitCursor);

		obj_type=object->getObjectType();
		if(obj_type!=ObjectType::Textbox ||
				(obj_type==ObjectType::BaseRelationship &&
				 dynamic_cast<BaseRelationship *>(object)->getRelationshipType()==BaseRelationship::RelationshipFk))
		{

			BaseObject::setPgSQLVersion(version_cmb->currentText());

			if(obj_type==ObjectType::Database)
			{
				task_prog_wgt=new TaskProgressWidget;
				task_prog_wgt->setWindowTitle(tr("Generating source code..."));
				task_prog_wgt->show();
				connect(this->model, &DatabaseModel::s_objectLoaded, task_prog_wgt, qOverload<int, QString, unsigned>(&TaskProgressWidget::updateProgress));
				sqlcode_txt->setPlainText(object->getSourceCode(SchemaParser::SqlCode));
			}
			else
			{
				sqlcode_txt->setPlainText(model->getSQLDefinition(object, static_cast<DatabaseModel::CodeGenMode>(code_options_cmb->currentIndex())));
			}

#ifdef DEMO_VERSION
#warning "DEMO VERSION: SQL code preview truncated."
			if(!sqlcode_txt->toPlainText().isEmpty())
			{
				int factor = obj_type == ObjectType::Database ? 4 : 2;
				QString code = sqlcode_txt->toPlainText();
				code = code.mid(0, code.size()/factor);
				code += tr("\n\n-- SQL code purposely truncated at this point in demo version!");
				sqlcode_txt->setPlainText(code);

				sqlcode_txt->setCustomContextMenuEnabled(false);
				sqlcode_txt->setContextMenuPolicy(Qt::NoContextMenu);
				save_sql_tb->setEnabled(false);
			}
#endif
		}

#ifndef DEMO_VERSION
		save_sql_tb->setEnabled(!sqlcode_txt->toPlainText().isEmpty());
#endif

		if(sqlcode_txt->toPlainText().isEmpty())
			sqlcode_txt->setPlainText(tr("-- SQL code unavailable for this type of object --"));

#ifdef DEMO_VERSION
#warning "DEMO VERSION: XML code preview disabled."
		xmlcode_txt->setPlainText(tr("<!-- XML code preview disabled in demonstration version -->"));
#else
		xmlcode_txt->setPlainText(object->getSourceCode(SchemaParser::XmlCode));
#endif

		setSourceCodeTab();

		if(task_prog_wgt)
		{
			task_prog_wgt->close();
			disconnect(this->model, nullptr, task_prog_wgt, nullptr);
			delete task_prog_wgt;
		}

		qApp->restoreOverrideCursor();
	}
	catch(Exception &e)
	{
		qApp->restoreOverrideCursor();

		if(task_prog_wgt)
		{
			task_prog_wgt->close();
			disconnect(this->model, nullptr, task_prog_wgt, nullptr);
			delete task_prog_wgt;
		}

		Messagebox::error(e, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}
}

void SourceCodeWidget::setAttributes(DatabaseModel *model, BaseObject *object)
{
	if(object)
	{
		try
		{
			BaseObjectWidget::setAttributes(model, object, nullptr);
			ObjectType obj_type=object->getObjectType();

			this->name_edt->setText(QString("%1 (%2)").arg(object->getSignature()).arg(object->getTypeName()));
			this->protected_obj_frm->setVisible(false);
			this->obj_id_lbl->setVisible(false);
			this->code_options_cmb->setEnabled(obj_type!=ObjectType::Database &&
																					obj_type!=ObjectType::Textbox &&
																					obj_type!=ObjectType::BaseRelationship &&
																					obj_type!=ObjectType::Relationship);

#ifdef DEMO_VERSION
#warning "DEMO VERSION: SQL code display options disabled."
			code_options_cmb->setEnabled(false);
#endif

			obj_icon_lbl->setPixmap(QPixmap(GuiUtilsNs::getIconPath(object->getObjectType())));

			if(!hl_sqlcode->isConfigurationLoaded())
				hl_sqlcode->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());

			if(!hl_xmlcode->isConfigurationLoaded())
				hl_xmlcode->loadConfiguration(GlobalAttributes::getXMLHighlightConfPath());

			generateSourceCode();
		}
		catch(Exception &e)
		{
			Messagebox::error(e, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		}
	}
}

void SourceCodeWidget::applyConfiguration()
{
	emit s_closeRequested();
}

