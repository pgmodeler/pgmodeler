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

#include "textboxview.h"
#include "roundedrectitem.h"

TextboxView::TextboxView(Textbox *txtbox, bool override_style) : BaseObjectView(txtbox)
{
	connect(txtbox, SIGNAL(s_objectModified(void)), this, SLOT(configureObject(void)));

  box=new QGraphicsPolygonItem;
  text=new QGraphicsSimpleTextItem;

  box->setZValue(0);
  text->setZValue(1);

  obj_shadow=new QGraphicsPolygonItem;
  obj_shadow->setZValue(-1);
  this->addToGroup(obj_shadow);

  obj_selection=new QGraphicsPolygonItem;
  obj_selection->setVisible(false);
  obj_selection->setZValue(4);
  this->addToGroup(obj_selection);

  placeholder_pol=new QGraphicsPolygonItem;
  placeholder_pol->setVisible(false);
  placeholder_pol->setZValue(-1);
  placeholder_pol->setFlag(QGraphicsItem::ItemIsMovable, false);
  placeholder_pol->setFlag(QGraphicsItem::ItemIsSelectable, false);

	this->override_style=override_style;
  this->addToGroup(text);
  this->addToGroup(box);
	this->configureObject();
}

TextboxView::~TextboxView(void)
{
	this->removeFromGroup(box);
	this->removeFromGroup(text);
	delete(box);
	delete(text);
  delete(placeholder_pol);
}

void TextboxView::setColorStyle(const QBrush &fill_style, const QPen &border_style)
{
	if(override_style)
	{
		box->setBrush(fill_style);
		box->setPen(border_style);
	}
}

void TextboxView::setFontStyle(const QTextCharFormat &fmt)
{
	if(override_style)
	{
		text->setFont(fmt.font());
    text->setBrush(fmt.foreground());
  }
}

void TextboxView::togglePlaceholder(bool visible)
{
  if(use_placeholder && this->scene())
  {
    if(visible)
    {
      placeholder_pol->setBrush(QColor(220,220,220,128));
      placeholder_pol->setPen(QPen(QColor(180,180,180), 1, Qt::DashLine));
      placeholder_pol->setPolygon(box->polygon());
      placeholder_pol->setPos(this->mapToScene(this->bounding_rect.topLeft()));
      this->scene()->addItem(placeholder_pol);
    }
    else
      this->scene()->removeItem(placeholder_pol);

    placeholder_pol->setVisible(visible);
  }
}

void TextboxView::__configureObject(void)
{
	Textbox *txtbox=dynamic_cast<Textbox *>(this->getSourceObject());
	QTextCharFormat fmt=font_config[ParsersAttributes::GLOBAL];
  QPolygonF polygon;

  polygon.append(QPointF(0.0f,0.0f));
  polygon.append(QPointF(1.0f,0.0f));
  polygon.append(QPointF(1.0f,1.0f));
  polygon.append(QPointF(0.0f,1.0f));

	if(!override_style)
	{
    QFont font;
    box->setBrush(this->getFillStyle(BaseObject::getSchemaName(OBJ_TEXTBOX)));
    box->setPen(this->getBorderStyle(BaseObject::getSchemaName(OBJ_TEXTBOX)));

    font=fmt.font();
		font.setItalic(txtbox->getTextAttribute(Textbox::ITALIC_TXT));
		font.setBold(txtbox->getTextAttribute(Textbox::BOLD_TXT));
		font.setUnderline(txtbox->getTextAttribute(Textbox::UNDERLINE_TXT));
		font.setPointSizeF(txtbox->getFontSize());

		text->setFont(font);
    text->setBrush(txtbox->getTextColor());
	}

  text->setText(txtbox->getComment());

  if(text->font().italic())
    text->setPos(HORIZ_SPACING * 1.5, VERT_SPACING * 0.90);
  else
    text->setPos(HORIZ_SPACING, VERT_SPACING);

  this->resizePolygon(polygon, roundf(text->boundingRect().width() + (2.5 * HORIZ_SPACING)),
                       roundf(text->boundingRect().height() + (1.5 * VERT_SPACING)));

  box->setPos(0,0);
  box->setPolygon(polygon);

  protected_icon->setPos(box->boundingRect().right() - (protected_icon->boundingRect().width() + 2 * HORIZ_SPACING),
                         box->boundingRect().bottom()- (protected_icon->boundingRect().height() + 2 * VERT_SPACING));

	this->bounding_rect.setTopLeft(box->boundingRect().topLeft());
	this->bounding_rect.setBottomRight(box->boundingRect().bottomRight());

  BaseObjectView::__configureObject();
}

void TextboxView::configureObject(void)
{
  this->__configureObject();
  this->configureObjectShadow();
  this->configureObjectSelection();
}

void TextboxView::configureObjectShadow(void)
{
  QGraphicsPolygonItem *pol_item=dynamic_cast<QGraphicsPolygonItem *>(obj_shadow);

  pol_item->setPen(Qt::NoPen);
  pol_item->setBrush(QColor(50,50,50,60));
  pol_item->setPolygon(box->polygon());
  pol_item->setPos(3.5,3.5);
}

void TextboxView::configureObjectSelection(void)
{
  QGraphicsPolygonItem *pol_item=dynamic_cast<QGraphicsPolygonItem *>(obj_selection);

  pol_item->setPolygon(box->polygon());
  pol_item->setPos(0,0);
  pol_item->setBrush(this->getFillStyle(ParsersAttributes::OBJ_SELECTION));
  pol_item->setPen(this->getBorderStyle(ParsersAttributes::OBJ_SELECTION));
}
