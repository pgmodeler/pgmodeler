/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "schemawidget.h"

SchemaWidget::SchemaWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_SCHEMA)
{
	Ui_SchemaWidget::setupUi(this);
	QHBoxLayout *hbox=nullptr;

	configureFormLayout(nullptr, OBJ_SCHEMA);

	color_picker=new ColorPickerWidget(1, this);

	hbox=new QHBoxLayout;
	hbox->setContentsMargins(2,0,0,0);
	hbox->addWidget(fill_color_lbl);
	hbox->addWidget(color_picker);
	hbox->addWidget(show_rect_chk);

	baseobject_grid->addLayout(hbox, baseobject_grid->count(), 0, 1, baseobject_grid->columnCount());
	connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

	parent_form->setMinimumSize(500, 220);
	parent_form->setMaximumHeight(220);

	configureTabOrder({ color_picker, show_rect_chk });
}

void SchemaWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema)
{
	bool enable=false;

	BaseObjectWidget::setAttributes(model, op_list, schema);

	enable=!(schema && schema->isSystemObject());
	edt_perms_tb->setEnabled(enable);
	name_edt->setEnabled(enable);
	comment_edt->setEnabled(enable);
	owner_sel->setEnabled(enable);

	if(schema)
	{
		if(schema->isSystemObject())
		{
			protected_obj_frm->setVisible(false);
			parent_form->apply_ok_btn->setEnabled(true);
		}

		color_picker->setColor(0, schema->getFillColor());
	}
	else
		color_picker->setColor(0, QColor(225,225,225));

	show_rect_chk->setChecked(schema && schema->isRectVisible());
}

void SchemaWidget::applyConfiguration(void)
{
	try
	{
		Schema *schema=nullptr;

		startConfiguration<Schema>();
		schema=dynamic_cast<Schema *>(this->object);
		BaseObjectWidget::applyConfiguration();

		schema->setRectVisible(show_rect_chk->isChecked());
		schema->setFillColor(color_picker->getColor(0));
		model->validateSchemaRenaming(dynamic_cast<Schema *>(this->object), this->prev_name);

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

