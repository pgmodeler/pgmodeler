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

#include "sourcecodewidget.h"
#include "taskprogresswidget.h"
extern TaskProgressWidget *task_prog_wgt;

SourceCodeWidget::SourceCodeWidget(QWidget *parent): BaseObjectWidget(parent)
{
	try
	{
		vector<QString> versions;
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

		SchemaParser::getPgSQLVersions(versions);
		version_cmb->addItems(QStringList(QList<QString>::fromVector(QVector<QString>::fromStdVector(versions))));

		parent_form->setWindowTitle(trUtf8("Source code visualization"));
		parent_form->setButtonConfiguration(Messagebox::OK_BUTTON);
		parent_form->setMinimumSize(650, 550);

		connect(parent_form->apply_ok_btn, SIGNAL(clicked(bool)), parent_form, SLOT(close(void)));
		connect(version_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(generateSourceCode(int)));
		connect(sourcecode_twg, SIGNAL(currentChanged(int)), this, SLOT(setSourceCodeTab(int)));

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
	ObjectType tipo_obj=object->getObjectType();

	if(sourcecode_twg->currentIndex()==0)
		code_icon="codigosql.png";
	else
		code_icon="codigoxml.png";

	enabled=(sourcecode_twg->currentIndex()==0 &&
					 (tipo_obj!=BASE_RELATIONSHIP && tipo_obj!=OBJ_TEXTBOX));

	icone=QPixmap(QString(":/icones/icones/") + code_icon);
	icon_lbl->setPixmap(icone);
	version_cmb->setEnabled(enabled);
	pgsql_lbl->setEnabled(enabled);
	version_lbl->setEnabled(enabled);
}

void SourceCodeWidget::generateSourceCode(int)
{
	ObjectType obj_type;

	try
	{
		sqlcode_txt->clear();
		xmlcode_txt->clear();

		obj_type=object->getObjectType();
		if(obj_type!=BASE_RELATIONSHIP && obj_type!=OBJ_TEXTBOX)
		{
			if(obj_type==OBJ_DATABASE)
			{
				task_prog_wgt->setWindowTitle(trUtf8("Generating source code..."));
				task_prog_wgt->show();
				connect(this->model, SIGNAL(s_objectLoaded(int,QString,unsigned)),
								task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
			}

			SchemaParser::setPgSQLVersion(version_cmb->currentText());
			sqlcode_txt->setPlainText(Utf8String::create(object->getCodeDefinition(SchemaParser::SQL_DEFINITION)));
		}

		if(sqlcode_txt->toPlainText()=="")
			sqlcode_txt->setPlainText(trUtf8("-- SQL code unavailable for this type of object --"));

		xmlcode_txt->setPlainText(Utf8String::create(object->getCodeDefinition(SchemaParser::XML_DEFINITION)));

		setSourceCodeTab();
		task_prog_wgt->close();
		disconnect(this->model, nullptr, task_prog_wgt, nullptr);
	}
	catch(Exception &e)
	{
		task_prog_wgt->close();
		disconnect(this->model, nullptr, task_prog_wgt, nullptr);
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

			this->parent_form->apply_ok_btn->setEnabled(true);
			this->protected_obj_frm->setVisible(false);

			obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") +
																			BaseObject::getSchemaName(object->getObjectType()) + QString(".png")));

			comment_edt->setText(object->getTypeName());

			if(!hl_sqlcode->isConfigurationLoaded())
			{
				hl_sqlcode->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																			GlobalAttributes::DIR_SEPARATOR +
																			GlobalAttributes::SQL_HIGHLIGHT_CONF +
																			GlobalAttributes::CONFIGURATION_EXT);
			}

			if(!hl_xmlcode->isConfigurationLoaded())
			{
				hl_xmlcode->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																			GlobalAttributes::DIR_SEPARATOR +
																			GlobalAttributes::XML_HIGHLIGHT_CONF +
																			GlobalAttributes::CONFIGURATION_EXT);
			}

			generateSourceCode();
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

