/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "pgmodeleruins.h"

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

		sqlcode_txt=PgModelerUiNs::createNumberedTextEditor(sqlcode_wgt);
		sqlcode_txt->setReadOnly(true);

		xmlcode_txt=PgModelerUiNs::createNumberedTextEditor(xmlcode_wgt);
		xmlcode_txt->setReadOnly(true);

		name_edt->setReadOnly(true);
		version_cmb->addItems(PgSqlVersions::AllVersions);

		code_options_ht=new HintTextWidget(code_options_hint, this);
		code_options_ht->setText(
					trUtf8("<strong>Original:</strong> displays only the original object's SQL code.<br/><br/>\
						   <strong>Dependencies:</strong> displays the original code including all dependencies needed to properly create the selected object.<br/><br/>\
						   <strong>Children:</strong> displays the original code including all object's children SQL code. This option is used only by schemas, tables and views."));

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
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SourceCodeWidget::setSourceCodeTab(int)
{
	QString code_icon;
	bool enabled=false;
	QPixmap icone;
	ObjectType obj_type=object->getObjectType();

	if(sourcecode_twg->currentIndex()==0)
		code_icon=QString("codigosql");
	else
		code_icon=QString("codigoxml");

	enabled=(sourcecode_twg->currentIndex()==0 &&
			 ((obj_type==ObjectType::BaseRelationship &&
			   dynamic_cast<BaseRelationship *>(object)->getRelationshipType()==BaseRelationship::RelationshipFk)
			  || (obj_type!=ObjectType::BaseRelationship && obj_type!=ObjectType::Textbox)));

	icone=QPixmap(PgModelerUiNs::getIconPath(code_icon));
	icon_lbl->setPixmap(icone);
	version_cmb->setEnabled(enabled);
	pgsql_lbl->setEnabled(enabled);
	version_lbl->setEnabled(enabled);
}

void SourceCodeWidget::saveSQLCode(void)
{
	QFileDialog file_dlg;

	file_dlg.setWindowTitle(trUtf8("Save SQL code as..."));

	file_dlg.setFileMode(QFileDialog::AnyFile);
	file_dlg.setAcceptMode(QFileDialog::AcceptSave);
	file_dlg.setModal(true);
	file_dlg.setNameFilter(trUtf8("SQL code (*.sql);;All files (*.*)"));
	file_dlg.selectFile(QString("%1-%2.sql").arg(object->getSchemaName()).arg(object->getName()));

	if(file_dlg.exec()==QFileDialog::Accepted)
	{
		QFile out;
		QByteArray buf;

		if(!file_dlg.selectedFiles().isEmpty())
		{
			out.setFileName(file_dlg.selectedFiles().at(0));

			if(!out.open(QFile::WriteOnly))
				throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(file_dlg.selectedFiles().at(0)),
												ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			buf.append(sqlcode_txt->toPlainText());
			out.write(buf.data(), buf.size());
			out.close();
		}
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
				task_prog_wgt->setWindowTitle(trUtf8("Generating source code..."));
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
					vector<BaseObject *> objs=model->getCreationOrder(object, code_options_cmb->currentIndex()==ChildrenSql);

					for(BaseObject *obj : objs)
						aux_def+=obj->getCodeDefinition(SchemaParser::SqlDefinition);
				}

				if(!aux_def.isEmpty())
				{
					aux_def=trUtf8("-- NOTE: the code below contains the SQL for the selected object\n\
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
				QString code=sqlcode_txt->toPlainText();
				code=code.mid(0, code.size()/2);
				code+=trUtf8("\n\n-- SQL code purposely truncated at this point in demo version!");
				sqlcode_txt->setPlainText(code);
			}
#endif
		}

		save_sql_tb->setEnabled(!sqlcode_txt->toPlainText().isEmpty());

		if(sqlcode_txt->toPlainText().isEmpty())
			sqlcode_txt->setPlainText(trUtf8("-- SQL code unavailable for this type of object --"));

#ifdef DEMO_VERSION
#warning "DEMO VERSION: XML code preview disabled."
		xmlcode_txt->setPlainText(trUtf8("<!-- XML code preview disabled in demonstration version -->"));
#else
		xmlcode_txt->setPlainText(object->getCodeDefinition(SchemaParser::XmlDefinition));
#endif

		setSourceCodeTab();

		if(task_prog_wgt)
		{
			task_prog_wgt->close();
			disconnect(this->model, nullptr, task_prog_wgt, nullptr);
			delete(task_prog_wgt);
		}
	}
	catch(Exception &e)
	{
		if(task_prog_wgt)
		{
			task_prog_wgt->close();
			disconnect(this->model, nullptr, task_prog_wgt, nullptr);
			delete(task_prog_wgt);
		}
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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

			obj_icon_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath(object->getObjectType())));

			if(!hl_sqlcode->isConfigurationLoaded())
				hl_sqlcode->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);

			if(!hl_xmlcode->isConfigurationLoaded())
				hl_xmlcode->loadConfiguration(GlobalAttributes::XMLHighlightConfPath);

			generateSourceCode();
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void SourceCodeWidget::applyConfiguration(void)
{
	emit s_closeRequested();
}

void SourceCodeWidget::displayQuery(QString query_txt)
{
	//load config
	try
	{

		this->name_edt->setText("GQB output");
		this->protected_obj_frm->setVisible(false);
		this->obj_id_lbl->setVisible(false);
		this->code_options_cmb->setEnabled(false);

		if(!hl_sqlcode->isConfigurationLoaded())
			hl_sqlcode->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);

		//if(!hl_xmlcode->isConfigurationLoaded())
		//    hl_xmlcode->loadConfiguration(GlobalAttributes::XMLHighlightConfPath);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	//write sql and see
	try
	{
	sqlcode_txt->clear();
	xmlcode_txt->clear();

	sqlcode_txt->setPlainText(query_txt);

	save_sql_tb->setEnabled(!sqlcode_txt->toPlainText().isEmpty());

	if(sqlcode_txt->toPlainText().isEmpty())
		sqlcode_txt->setPlainText(trUtf8("-- Something went wrong for _some_ reason --"));

	sourcecode_twg->setTabPosition(QTabWidget::TabPosition (0));
	sourcecode_twg->removeTab(1);

	version_cmb->setVisible(false);
	pgsql_lbl->setVisible(false);
	version_lbl->setVisible(false);

	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
