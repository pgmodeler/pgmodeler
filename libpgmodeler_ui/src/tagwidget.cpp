/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2017 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "tagwidget.h"

TagWidget::TagWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TAG)
{
	Ui_TagWidget::setupUi(this);
	configureFormLayout(tag_grid, OBJ_TAG);

	QStringList attribs={ ParsersAttributes::TABLE_NAME, ParsersAttributes::TABLE_SCHEMA_NAME,
						  ParsersAttributes::TABLE_TITLE, ParsersAttributes::TABLE_BODY,
						  ParsersAttributes::TABLE_EXT_BODY };
	unsigned color_count=1;
	int row=0;

	for(auto &attr : attribs)
	{
		if(color_count==1 && attr!=ParsersAttributes::TABLE_NAME && attr!=ParsersAttributes::TABLE_SCHEMA_NAME)
			color_count=3;

		color_pickers[attr]=new ColorPickerWidget(color_count, this);
		colors_grid->addWidget(color_pickers[attr], row, 1);
		colors_grid->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Fixed), row, 2);
		row++;
	}

	setMinimumSize(450, 220);
}

void TagWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Tag *tag)
{
	unsigned color_count=1, i;
	QStringList attribs={ ParsersAttributes::TABLE_NAME, ParsersAttributes::TABLE_SCHEMA_NAME,
						  ParsersAttributes::TABLE_TITLE, ParsersAttributes::TABLE_BODY,
						  ParsersAttributes::TABLE_EXT_BODY };

	BaseObjectWidget::setAttributes(model, op_list, tag);

	for(auto &attr : attribs)
	{
		if(color_count==1 && attr!=ParsersAttributes::TABLE_NAME && attr!=ParsersAttributes::TABLE_SCHEMA_NAME)
			color_count=3;

		for(i=0; i < color_count; i++)
		{
			if(tag)
				color_pickers[attr]->setColor(i, tag->getElementColor(attr, i));
			else
				color_pickers[attr]->setColor(i, BaseObjectView::getElementColor(attr, i));
		}
	}
}

void TagWidget::applyConfiguration(void)
{
	try
	{
		Tag *tag=nullptr;
		vector<BaseObject *> tagged_tabs;
		QStringList attribs={ ParsersAttributes::TABLE_TITLE, ParsersAttributes::TABLE_BODY,
							  ParsersAttributes::TABLE_EXT_BODY };

		startConfiguration<Tag>();
		tag=dynamic_cast<Tag *>(this->object);

		BaseObjectWidget::applyConfiguration();

		tag->setElementColor(ParsersAttributes::TABLE_NAME, color_pickers[ParsersAttributes::TABLE_NAME]->getColor(0), Tag::FILL_COLOR1);
		tag->setElementColor(ParsersAttributes::TABLE_SCHEMA_NAME, color_pickers[ParsersAttributes::TABLE_SCHEMA_NAME]->getColor(0), Tag::FILL_COLOR1);

		for(auto &attr : attribs)
		{
			tag->setElementColors(attr,
								  QString("%1,%2,%3")
								  .arg(color_pickers[attr]->getColor(Tag::FILL_COLOR1).name())
								  .arg(color_pickers[attr]->getColor(Tag::FILL_COLOR2).name())
								  .arg(color_pickers[attr]->getColor(Tag::BORDER_COLOR).name()));
		}

		model->getObjectReferences(tag, tagged_tabs);
		while(!tagged_tabs.empty())
		{
			dynamic_cast<BaseTable *>(tagged_tabs.back())->setModified(true);
			tagged_tabs.pop_back();
		}

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

