/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "utilsns.h"

SourceCodeWidget::SourceCodeWidget(QWidget *parent): BaseObjectWidget(parent)
{
	try
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

		connect(version_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(generateSourceCode(int)));
		connect(code_options_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(generateSourceCode()));
		connect(sourcecode_twg, SIGNAL(currentChanged(int)), this, SLOT(setSourceCodeTab(int)));
		connect(save_sql_tb, SIGNAL(clicked()), this, SLOT(saveSQLCode()));

		hl_sqlcode=new SyntaxHighlighter(sqlcode_txt);
		hl_xmlcode=new SyntaxHighlighter(xmlcode_txt);

		setMinimumSize(640, 540);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
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
	version_lbl->setEnabled(enabled);
}

void SourceCodeWidget::saveSQLCode()
{
	QFileDialog file_dlg;

	file_dlg.setWindowTitle(tr("Save SQL code as..."));

	file_dlg.setFileMode(QFileDialog::AnyFile);
	file_dlg.setAcceptMode(QFileDialog::AcceptSave);
	file_dlg.setModal(true);
	file_dlg.setNameFilter(tr("SQL code (*.sql);;All files (*.*)"));
	file_dlg.selectFile(QString("%1-%2.sql").arg(object->getSchemaName()).arg(object->getName()));

	GuiUtilsNs::restoreFileDialogState(&file_dlg);

	if(file_dlg.exec() == QFileDialog::Accepted && !file_dlg.selectedFiles().isEmpty())
		UtilsNs::saveFile(file_dlg.selectedFiles().at(0), sqlcode_txt->toPlainText().toUtf8());

	GuiUtilsNs::saveFileDialogState(&file_dlg);
}

void SourceCodeWidget::generateSourceCode(int)
{
	ObjectType obj_type;
	TaskProgressWidget *task_prog_wgt=nullptr;

	try
	{
		sqlcode_txt->clear();
		xmlcode_txt->clear();

		obj_type=object->getObjectType();
		if(obj_type!=ObjectType::Textbox ||
				(obj_type==ObjectType::BaseRelationship &&
				 dynamic_cast<BaseRelationship *>(object)->getRelationshipType()==BaseRelationship::RelationshipFk))
		{
			QString aux_def;
			BaseObject::setPgSQLVersion(version_cmb->currentText());

			if(obj_type==ObjectType::Database)
			{
				task_prog_wgt=new TaskProgressWidget;
				task_prog_wgt->setWindowTitle(tr("Generating source code..."));
				task_prog_wgt->show();
				connect(this->model, SIGNAL(s_objectLoaded(int,QString,unsigned)), task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
				sqlcode_txt->setPlainText(object->getCodeDefinition(SchemaParser::SqlDefinition));
			}
			else
			{
				if(code_options_cmb->currentIndex()==OriginalSql)
					sqlcode_txt->setPlainText(object->getCodeDefinition(SchemaParser::SqlDefinition));
				else
				{
					std::vector<BaseObject *> objs=model->getCreationOrder(object, code_options_cmb->currentIndex()==ChildrenSql);

					for(BaseObject *obj : objs)
						aux_def+=obj->getCodeDefinition(SchemaParser::SqlDefinition);
				}

				if(!aux_def.isEmpty())
				{
					aux_def=tr("-- NOTE: the code below contains the SQL for the selected object\n\
-- as well for its dependencies and children (if applicable).\n\
-- \n\
-- This feature is only a convinience in order to permit you to test\n\
-- the whole object's SQL definition at once.\n\
-- \n\
-- When exporting or generating the SQL for the whole database model\n\
-- all objects will be placed at their original positions.\n\n\n") + aux_def;

					sqlcode_txt->setPlainText(sqlcode_txt->toPlainText() + aux_def);
				}
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
		xmlcode_txt->setPlainText(object->getCodeDefinition(SchemaParser::XmlDefinition));
#endif

		setSourceCodeTab();

		if(task_prog_wgt)
		{
			task_prog_wgt->close();
			disconnect(this->model, nullptr, task_prog_wgt, nullptr);
			delete task_prog_wgt;
		}
	}
	catch(Exception &e)
	{
		if(task_prog_wgt)
		{
			task_prog_wgt->close();
			disconnect(this->model, nullptr, task_prog_wgt, nullptr);
			delete task_prog_wgt;
		}
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void SourceCodeWidget::applyConfiguration()
{
	emit s_closeRequested();
}

