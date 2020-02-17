/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

TagWidget::TagWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::Tag)
{
	Ui_TagWidget::setupUi(this);
	configureFormLayout(tag_grid, ObjectType::Tag);

	QStringList attribs={ Attributes::TableName, Attributes::TableSchemaName,
						  Attributes::TableTitle, Attributes::TableBody,
						  Attributes::TableExtBody };
	unsigned color_count=1;
	int row=0;

	for(auto &attr : attribs)
	{
		if(color_count==1 && attr!=Attributes::TableName && attr!=Attributes::TableSchemaName)
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
	QStringList attribs={ Attributes::TableName, Attributes::TableSchemaName,
						  Attributes::TableTitle, Attributes::TableBody,
						  Attributes::TableExtBody };

	BaseObjectWidget::setAttributes(model, op_list, tag);

	for(auto &attr : attribs)
	{
		if(color_count==1 && attr!=Attributes::TableName && attr!=Attributes::TableSchemaName)
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

void TagWidget::applyConfiguration()
{
	try
	{
		Tag *tag=nullptr;
		vector<BaseObject *> tagged_tabs;
		QStringList attribs={ Attributes::TableTitle, Attributes::TableBody,
							  Attributes::TableExtBody };

		startConfiguration<Tag>();
		tag=dynamic_cast<Tag *>(this->object);

		BaseObjectWidget::applyConfiguration();

		tag->setElementColor(Attributes::TableName, color_pickers[Attributes::TableName]->getColor(0), Tag::FillColor1);
		tag->setElementColor(Attributes::TableSchemaName, color_pickers[Attributes::TableSchemaName]->getColor(0), Tag::FillColor1);

		for(auto &attr : attribs)
		{
			tag->setElementColors(attr,
								  QString("%1,%2,%3")
								  .arg(color_pickers[attr]->getColor(Tag::FillColor1).name())
								  .arg(color_pickers[attr]->getColor(Tag::FillColor2).name())
								  .arg(color_pickers[attr]->getColor(Tag::BorderColor).name()));
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
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

