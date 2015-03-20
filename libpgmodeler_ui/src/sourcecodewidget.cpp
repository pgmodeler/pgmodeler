/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

SourceCodeWidget::SourceCodeWidget(QWidget *parent): BaseObjectWidget(parent)
{
	try
	{
		QFont font;

		Ui_SourceCodeWidget::setupUi(this);
		configureFormLayout(codigofonte_grid, BASE_OBJECT);
		comment_lbl->setText(trUtf8("Type:"));

		hl_sqlcode=nullptr;
		hl_xmlcode=nullptr;

		font=name_edt->font();
		font.setItalic(true);
		comment_edt->setFont(font);
		comment_edt->setReadOnly(true);
		name_edt->setFont(font);
		name_edt->setReadOnly(true);
    version_cmb->addItems(PgSQLVersions::ALL_VERSIONS);

		parent_form->setWindowTitle(trUtf8("Source code visualization"));
		parent_form->setButtonConfiguration(Messagebox::OK_BUTTON);
		parent_form->setMinimumSize(650, 550);

    incl_child_sql_ht=new HintTextWidget(incl_child_sql_hint, this);
    incl_child_sql_ht->setText(incl_child_sql_chk->statusTip());

    incl_deps_sql_ht=new HintTextWidget(incl_deps_sql_hint, this);
    incl_deps_sql_ht->setText(incl_deps_sql_chk->statusTip());

		connect(parent_form->apply_ok_btn, SIGNAL(clicked(bool)), parent_form, SLOT(close(void)));
		connect(version_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(generateSourceCode(int)));
    connect(incl_deps_sql_chk, SIGNAL(stateChanged(int)), this, SLOT(generateSourceCode(int)));
    connect(incl_child_sql_chk, SIGNAL(stateChanged(int)), this, SLOT(generateSourceCode(int)));
		connect(sourcecode_twg, SIGNAL(currentChanged(int)), this, SLOT(setSourceCodeTab(int)));
    connect(save_sql_tb, SIGNAL(clicked()), this, SLOT(saveSQLCode()));

    connect(incl_deps_sql_chk, &QCheckBox::toggled,
            [=](bool checked){
                  incl_child_sql_chk->blockSignals(true);
                  incl_child_sql_chk->setChecked(false);
                  incl_child_sql_chk->setEnabled(checked);
                  incl_child_sql_chk->blockSignals(false);
            });

		hl_sqlcode=new SyntaxHighlighter(sqlcode_txt, false);
		hl_xmlcode=new SyntaxHighlighter(xmlcode_txt, false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SourceCodeWidget::hideEvent(QHideEvent *event)
{
	version_cmb->blockSignals(true);
	sourcecode_twg->blockSignals(true);
	version_cmb->setCurrentIndex(0);
	sourcecode_twg->setCurrentIndex(0);
	version_cmb->blockSignals(false);
	sourcecode_twg->blockSignals(false);
	sqlcode_txt->clear();
	xmlcode_txt->clear();
	BaseObjectWidget::hideEvent(event);
}

void SourceCodeWidget::setSourceCodeTab(int)
{
	QString code_icon;
	bool enabled=false;
	QPixmap icone;
  ObjectType obj_type=object->getObjectType();

	if(sourcecode_twg->currentIndex()==0)
    code_icon=QString("codigosql.png");
	else
    code_icon=QString("codigoxml.png");

	enabled=(sourcecode_twg->currentIndex()==0 &&
           ((obj_type==BASE_RELATIONSHIP &&
             dynamic_cast<BaseRelationship *>(object)->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
             || (obj_type!=BASE_RELATIONSHIP && obj_type!=OBJ_TEXTBOX)));

	icone=QPixmap(QString(":/icones/icones/") + code_icon);
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
          throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_WRITTEN).arg(file_dlg.selectedFiles().at(0)),
                          ERR_FILE_DIR_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
    if(obj_type!=OBJ_TEXTBOX ||
       (obj_type==BASE_RELATIONSHIP &&
        dynamic_cast<BaseRelationship *>(object)->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK))
		{
			QString aux_def;
      BaseObject::setPgSQLVersion(version_cmb->currentText());

			if(obj_type==OBJ_DATABASE)
			{
				task_prog_wgt=new TaskProgressWidget(this);
				task_prog_wgt->setWindowTitle(trUtf8("Generating source code..."));
				task_prog_wgt->show();
        connect(this->model, SIGNAL(s_objectLoaded(int,QString,unsigned)), task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
        sqlcode_txt->setPlainText(object->getCodeDefinition(SchemaParser::SQL_DEFINITION));
			}
      else
      {
        if(!incl_deps_sql_chk->isChecked())
          sqlcode_txt->setPlainText(object->getCodeDefinition(SchemaParser::SQL_DEFINITION));
        else
        {
          vector<BaseObject *> objs=model->getCreationOrder(object);

          for(BaseObject *obj : objs)
            aux_def+=obj->getCodeDefinition(SchemaParser::SQL_DEFINITION);
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
            code+=trUtf8("\n\n-- SQL code purposely truncated at this point on demo version!");
            sqlcode_txt->setPlainText(code);
          }
			#endif
		}

    save_sql_tb->setEnabled(!sqlcode_txt->toPlainText().isEmpty());
    incl_deps_sql_wgt->setEnabled(!sqlcode_txt->toPlainText().isEmpty());

		if(sqlcode_txt->toPlainText().isEmpty())
			sqlcode_txt->setPlainText(trUtf8("-- SQL code unavailable for this type of object --"));

		#ifdef DEMO_VERSION
			#warning "DEMO VERSION: XML code preview disabled."
			xmlcode_txt->setPlainText(trUtf8("<!-- XML code preview disabled in demonstration version -->"));
		#else
      xmlcode_txt->setPlainText(object->getCodeDefinition(SchemaParser::XML_DEFINITION));
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

			this->parent_form->apply_ok_btn->setEnabled(true);
			this->protected_obj_frm->setVisible(false);
			this->obj_id_lbl->setVisible(false);

      incl_deps_sql_wgt->setVisible(true);
      incl_child_sql_wgt->setVisible(obj_type==OBJ_SCHEMA || obj_type==OBJ_TABLE || obj_type==OBJ_VIEW);

			#ifdef DEMO_VERSION
				#warning "DEMO VERSION: Referrer objects SQL code attachment disabled."
        incl_deps_sql_wgt->setEnabled(false);
        incl_child_sql_wgt->setEnabled(false);
			#endif

			obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") +
																			BaseObject::getSchemaName(object->getObjectType()) + QString(".png")));

			comment_edt->setText(object->getTypeName());

			if(!hl_sqlcode->isConfigurationLoaded())
			{
        hl_sqlcode->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);
			}

			if(!hl_xmlcode->isConfigurationLoaded())
			{
        hl_xmlcode->loadConfiguration(GlobalAttributes::XML_HIGHLIGHT_CONF_PATH);
			}

			generateSourceCode();
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

