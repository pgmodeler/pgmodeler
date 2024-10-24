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

	prev_pg_ver = prev_code_opt = -1;
	hl_sqlcode = nullptr;
	hl_xmlcode = nullptr;

	sqlcode_txt = GuiUtilsNs::createNumberedTextEditor(sqlcode_wgt);
	sqlcode_txt->setReadOnly(true);

	xmlcode_txt = GuiUtilsNs::createNumberedTextEditor(xmlcode_wgt);
	xmlcode_txt->setReadOnly(true);

	name_edt->setReadOnly(true);
	version_cmb->addItems(PgSqlVersions::AllVersions);

	connect(save_sql_tb, &QToolButton::clicked, this, &SourceCodeWidget::saveSQLCode);
	connect(sourcecode_twg, &QTabWidget::currentChanged, this, &SourceCodeWidget::generateSourceCode);
	connect(sourcecode_twg, &QTabWidget::currentChanged, this, &SourceCodeWidget::setSourceCodeTab);

	connect(version_cmb, &QComboBox::currentIndexChanged, this, [this](int){
		generateSourceCode(SchemaParser::SqlCode);
	});

	connect(code_options_cmb, &QComboBox::currentIndexChanged, this, [this](int) {
		generateSourceCode(SchemaParser::SqlCode);
	});

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

	setMinimumSize(800, 600);
}

void SourceCodeWidget::setSourceCodeTab(int tab_idx)
{
	bool enabled = false;
	ObjectType obj_type = object->getObjectType();

	enabled = (tab_idx == 0 &&
						 ((obj_type == ObjectType::BaseRelationship &&
							 dynamic_cast<BaseRelationship *>(object)->getRelationshipType() == BaseRelationship::RelationshipFk)
							 || (obj_type != ObjectType::BaseRelationship && obj_type != ObjectType::Textbox)));

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

void SourceCodeWidget::generateSQLCode()
{
	ObjectType obj_type = object->getObjectType();

	sqlcode_txt->clear();
	prev_pg_ver = version_cmb->currentIndex();
	prev_code_opt = code_options_cmb->currentIndex();

	if(obj_type != ObjectType::Textbox ||
			(obj_type == ObjectType::BaseRelationship &&
			 dynamic_cast<BaseRelationship *>(object)->getRelationshipType() == BaseRelationship::RelationshipFk))
	{
		BaseObject::setPgSQLVersion(version_cmb->currentText());

		if(obj_type == ObjectType::Database)
			sqlcode_txt->setPlainText(object->getSourceCode(SchemaParser::SqlCode));
		else
			sqlcode_txt->setPlainText(model->getSQLDefinition(object, static_cast<DatabaseModel::CodeGenMode>(code_options_cmb->currentIndex())));
	}

#ifndef DEMO_VERSION
	save_sql_tb->setEnabled(!sqlcode_txt->toPlainText().isEmpty());
#else
	if(!sqlcode_txt->toPlainText().isEmpty())
	{
		QString code = tr("/*******************************************************/\n\
/* ATTENTION: The SQL code of the objects is purposely */\n\
/* truncated in the demo version!                      */\n\
/*******************************************************/\n\n") +
		sqlcode_txt->toPlainText();
		sqlcode_txt->setPlainText(code);
	}
#endif

	if(sqlcode_txt->toPlainText().isEmpty())
		sqlcode_txt->setPlainText(tr("-- SQL code unavailable for this type of object --"));		
}

void SourceCodeWidget::generateXMLCode()
{
	xmlcode_txt->clear();

#ifdef DEMO_VERSION
#warning "DEMO VERSION: XML code preview disabled."
	xmlcode_txt->setPlainText(tr("<!-- XML code preview disabled in demonstration version -->"));
#else
	xmlcode_txt->setPlainText(object->getSourceCode(SchemaParser::XmlCode));
#endif
}

void SourceCodeWidget::generateSourceCode(int def_type)
{
	if((def_type == SchemaParser::XmlCode && !xmlcode_txt->document()->isEmpty()) ||
		 (def_type == SchemaParser::SqlCode &&
			(prev_pg_ver == version_cmb->currentIndex() &&
			 prev_code_opt == code_options_cmb->currentIndex())))
		return;

	ObjectType obj_type;
	TaskProgressWidget task_prog_wgt;

	try
	{
		qApp->setOverrideCursor(Qt::WaitCursor);

		obj_type = object->getObjectType();

		if(obj_type == ObjectType::Database)
		{
			task_prog_wgt.setWindowTitle(tr("Generating source code..."));
			task_prog_wgt.show();
			connect(this->model, &DatabaseModel::s_objectLoaded, &task_prog_wgt, qOverload<int, QString, unsigned>(&TaskProgressWidget::updateProgress));
		}

		if(def_type == SchemaParser::SqlCode)
			generateSQLCode();
		else
			generateXMLCode();

		task_prog_wgt.close();
		disconnect(this->model, nullptr, &task_prog_wgt, nullptr);

		qApp->restoreOverrideCursor();
	}
	catch(Exception &e)
	{
		qApp->restoreOverrideCursor();

		task_prog_wgt.close();
		disconnect(this->model, nullptr, &task_prog_wgt, nullptr);

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

			generateSourceCode(SchemaParser::SqlCode);
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

