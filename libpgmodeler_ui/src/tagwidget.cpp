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

#include "tagwidget.h"

TagWidget::TagWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TAG)
{
  Ui_TagWidget::setupUi(this);
  configureFormLayout(tag_grid, OBJ_TAG);

  connect(parent_form->apply_ok_btn, &QPushButton::clicked, this, &TagWidget::applyConfiguration);
  connect(tab_name_color_tb, &QToolButton::clicked, this, &TagWidget::selectColor);
  connect(sch_name_color_tb, &QToolButton::clicked, this, &TagWidget::selectColor);
  connect(title_fcolor1_tb, &QToolButton::clicked, this, &TagWidget::selectColor);
  connect(title_fcolor2_tb, &QToolButton::clicked, this, &TagWidget::selectColor);
  connect(title_bcolor_tb, &QToolButton::clicked, this, &TagWidget::selectColor);
  connect(body_fcolor1_tb, &QToolButton::clicked, this, &TagWidget::selectColor);
  connect(body_fcolor2_tb, &QToolButton::clicked, this, &TagWidget::selectColor);
  connect(body_bcolor_tb, &QToolButton::clicked, this, &TagWidget::selectColor);
  connect(extbody_fcolor1_tb, &QToolButton::clicked, this, &TagWidget::selectColor);
  connect(extbody_fcolor2_tb, &QToolButton::clicked, this, &TagWidget::selectColor);
  connect(extbody_bcolor_tb, &QToolButton::clicked, this, &TagWidget::selectColor);

  parent_form->setMinimumSize(450, 300);
  parent_form->setMaximumHeight(300);
}

void TagWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Tag *tag)
{
  BaseObjectWidget::setAttributes(model, op_list, tag);

  if(tag)
  {
    tab_name_color_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_NAME, Tag::FILL_COLOR1)));
    sch_name_color_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_SCHEMA_NAME, Tag::FILL_COLOR1)));

    title_fcolor1_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_TITLE, Tag::FILL_COLOR1)));
    title_fcolor2_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_TITLE, Tag::FILL_COLOR2)));
    title_bcolor_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_TITLE, Tag::BORDER_COLOR)));

    body_fcolor1_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_BODY, Tag::FILL_COLOR1)));
    body_fcolor2_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_BODY, Tag::FILL_COLOR2)));
    body_bcolor_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_BODY, Tag::BORDER_COLOR)));

    extbody_fcolor1_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_EXT_BODY, Tag::FILL_COLOR1)));
    extbody_fcolor2_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_EXT_BODY, Tag::FILL_COLOR2)));
    extbody_bcolor_tb->setPalette(QPalette(tag->getElementColor(ParsersAttributes::TABLE_EXT_BODY, Tag::BORDER_COLOR)));
  }
  else
  {
    tab_name_color_tb->setPalette(QPalette(BaseObjectView::getFontStyle(ParsersAttributes::TABLE_NAME).foreground().color()));
    sch_name_color_tb->setPalette(QPalette(BaseObjectView::getFontStyle(ParsersAttributes::TABLE_SCHEMA_NAME).foreground().color()));


    title_fcolor1_tb->setPalette(QPalette(BaseObjectView::getElementColor(ParsersAttributes::TABLE_TITLE, Tag::FILL_COLOR1)));
    title_fcolor2_tb->setPalette(QPalette(BaseObjectView::getElementColor(ParsersAttributes::TABLE_TITLE, Tag::FILL_COLOR2)));
    title_bcolor_tb->setPalette(QPalette(BaseObjectView::getElementColor(ParsersAttributes::TABLE_TITLE, Tag::BORDER_COLOR)));

    body_fcolor1_tb->setPalette(QPalette(BaseObjectView::getElementColor(ParsersAttributes::TABLE_BODY, Tag::FILL_COLOR1)));
    body_fcolor2_tb->setPalette(QPalette(BaseObjectView::getElementColor(ParsersAttributes::TABLE_BODY, Tag::FILL_COLOR2)));
    body_bcolor_tb->setPalette(QPalette(BaseObjectView::getElementColor(ParsersAttributes::TABLE_BODY, Tag::BORDER_COLOR)));

    extbody_fcolor1_tb->setPalette(QPalette(BaseObjectView::getElementColor(ParsersAttributes::TABLE_EXT_BODY, Tag::FILL_COLOR1)));
    extbody_fcolor2_tb->setPalette(QPalette(BaseObjectView::getElementColor(ParsersAttributes::TABLE_EXT_BODY, Tag::FILL_COLOR2)));
    extbody_bcolor_tb->setPalette(QPalette(BaseObjectView::getElementColor(ParsersAttributes::TABLE_EXT_BODY, Tag::BORDER_COLOR)));
  }
}

void TagWidget::selectColor(void)
{
	QColorDialog color_dlg;
  QToolButton *btn=dynamic_cast<QToolButton *>(sender());

  if(btn)
  {
    color_dlg.setWindowTitle(trUtf8("Select color"));
    color_dlg.setCurrentColor(btn->palette().color(QPalette::Button));
    color_dlg.exec();

    if(color_dlg.result()==QDialog::Accepted)
      btn->setPalette(QPalette(color_dlg.selectedColor()));
  }
}

void TagWidget::applyConfiguration(void)
{
	try
	{
    Tag *tag=nullptr;
    vector<BaseObject *> tagged_tabs;

    startConfiguration<Tag>();
    tag=dynamic_cast<Tag *>(this->object);

    BaseObjectWidget::applyConfiguration();

    tag->setElementColor(ParsersAttributes::TABLE_NAME, tab_name_color_tb->palette().color(QPalette::Button), Tag::FILL_COLOR1);
    tag->setElementColor(ParsersAttributes::TABLE_SCHEMA_NAME, sch_name_color_tb->palette().color(QPalette::Button), Tag::FILL_COLOR1);

    tag->setElementColors(ParsersAttributes::TABLE_TITLE,
                          QString("%1,%2,%3")
                          .arg(title_fcolor1_tb->palette().color(QPalette::Button).name())
                          .arg(title_fcolor2_tb->palette().color(QPalette::Button).name())
                          .arg(title_bcolor_tb->palette().color(QPalette::Button).name()));

    tag->setElementColors(ParsersAttributes::TABLE_BODY,
                          QString("%1,%2,%3")
                          .arg(body_fcolor1_tb->palette().color(QPalette::Button).name())
                          .arg(body_fcolor2_tb->palette().color(QPalette::Button).name())
                          .arg(body_bcolor_tb->palette().color(QPalette::Button).name()));

    tag->setElementColors(ParsersAttributes::TABLE_EXT_BODY,
                          QString("%1,%2,%3")
                          .arg(extbody_fcolor1_tb->palette().color(QPalette::Button).name())
                          .arg(extbody_fcolor2_tb->palette().color(QPalette::Button).name())
                          .arg(extbody_bcolor_tb->palette().color(QPalette::Button).name()));

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

