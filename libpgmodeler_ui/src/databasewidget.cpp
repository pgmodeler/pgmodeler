/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "databasewidget.h"

DatabaseWidget::DatabaseWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_DATABASE)
{
  try
  {
    QStringList loc_list, encodings;
    QFrame *frame=nullptr;
    QGridLayout *grid=nullptr;

    Ui_DatabaseWidget::setupUi(this);

    connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
    configureFormLayout(database_grid, OBJ_DATABASE);

    def_schema_sel=new ObjectSelectorWidget(OBJ_SCHEMA, true, this);
    def_collation_sel=new ObjectSelectorWidget(OBJ_COLLATION, true, this);
    def_owner_sel=new ObjectSelectorWidget(OBJ_ROLE, true, this);
    def_tablespace_sel=new ObjectSelectorWidget(OBJ_TABLESPACE, true, this);

    frame=generateInformationFrame(trUtf8("The fields <strong>LC_COLLATE</strong> and <strong>LC_CTYPE</strong> have pre-configured values based upon the running system. You can freely modify those values if you intend to export the model to another host."));
    grid=dynamic_cast<QGridLayout *>(attributes_twg->widget(0)->layout());
    grid->addWidget(frame, grid->count()+1, 0, 1, 0);

    frame=generateInformationFrame(trUtf8("Use the above fields to specify the default attributes assigned to new objects created on the database model. Leaving a field empty will cause PostgreSQL to use the default values when exporting the model."));
    grid=dynamic_cast<QGridLayout *>(attributes_twg->widget(1)->layout());

    grid->addWidget(def_collation_sel, 0, 1);
    grid->addWidget(def_schema_sel, 1, 1);
    grid->addWidget(def_owner_sel, 2, 1);
    grid->addWidget(def_tablespace_sel, 3, 1);
    grid->addWidget(frame, grid->count()+1, 0, 1, 0);
    frame->setParent(attributes_twg->widget(1));

    parent_form->setMinimumWidth(640);
    parent_form->setMinimumHeight(520);
    /*parent_form->setMaximumHeight(450);*/

    //Configures the encoding combobox
    EncodingType::getTypes(encodings);
    encodings.push_front(trUtf8("Default"));
    encoding_cmb->addItems(encodings);

    //Configures the localizations combobox
    for(int i=QLocale::C; i <= QLocale::Chewa; i++)
    {
      for(int i1=QLocale::Afghanistan; i1 <= QLocale::Zimbabwe; i1++)
        loc_list.append(QLocale(static_cast<QLocale::Language>(i),static_cast<QLocale::Country>(i1)).name());
    }

    loc_list.removeDuplicates();
    loc_list.sort();
    loc_list.push_front(trUtf8("Default"));

    lccollate_cmb->addItems(loc_list);
    lcctype_cmb->addItems(loc_list);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void DatabaseWidget::setAttributes(DatabaseModel *model)
{
	if(model)
	{
		int idx;

		connlim_sb->setValue(model->getConnectionLimit());
    templatedb_edt->setText(model->getTemplateDB());
    author_edt->setText(model->getAuthor());

		idx=encoding_cmb->findText(~model->getEncoding());
		if(idx < 0) idx=0;
		encoding_cmb->setCurrentIndex(idx);

    if(!model->getLocalization(Collation::_LC_COLLATE).isEmpty())
      lccollate_cmb->setCurrentText(model->getLocalization(Collation::_LC_COLLATE));

    if(!model->getLocalization(Collation::_LC_CTYPE).isEmpty())
      lcctype_cmb->setCurrentText(model->getLocalization(Collation::_LC_CTYPE));

    def_schema_sel->setModel(model);
    def_schema_sel->setSelectedObject(model->getDefaultObject(OBJ_SCHEMA));

    def_collation_sel->setModel(model);
    def_collation_sel->setSelectedObject(model->getDefaultObject(OBJ_COLLATION));

    def_owner_sel->setModel(model);
    def_owner_sel->setSelectedObject(model->getDefaultObject(OBJ_ROLE));

    def_tablespace_sel->setModel(model);
    def_tablespace_sel->setSelectedObject(model->getDefaultObject(OBJ_TABLESPACE));

		BaseObjectWidget::setAttributes(model, model, nullptr);
	}
}

void DatabaseWidget::applyConfiguration(void)
{
	try
	{
		//Apply the basic configurations
		BaseObjectWidget::applyConfiguration();

		model->setAuthor(author_edt->text().toUtf8());
		model->setTemplateDB(templatedb_edt->text());
    model->setConnectionLimit(connlim_sb->value());

		if(encoding_cmb->currentIndex() > 0)
			model->setEncoding(EncodingType(encoding_cmb->currentText()));
    else
      model->setEncoding(EncodingType(BaseType::null));

    if(lccollate_cmb->currentText()!=trUtf8("Default"))
			model->setLocalization(Collation::_LC_COLLATE, lccollate_cmb->currentText());
    else
      model->setLocalization(Collation::_LC_COLLATE, QString());

    if(lcctype_cmb->currentText()!=trUtf8("Default"))
			model->setLocalization(Collation::_LC_CTYPE, lcctype_cmb->currentText());
    else
      model->setLocalization(Collation::_LC_CTYPE, QString());

    model->setDefaultObject(def_schema_sel->getSelectedObject(), OBJ_SCHEMA);
    model->setDefaultObject(def_owner_sel->getSelectedObject(), OBJ_ROLE);
    model->setDefaultObject(def_collation_sel->getSelectedObject(), OBJ_COLLATION);
    model->setDefaultObject(def_tablespace_sel->getSelectedObject(), OBJ_TABLESPACE);

		finishConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
