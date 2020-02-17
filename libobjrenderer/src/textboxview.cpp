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

#include "textboxview.h"
#include "roundedrectitem.h"

TextboxView::TextboxView(Textbox *txtbox, bool override_style) : BaseObjectView(txtbox)
{
	connect(txtbox, SIGNAL(s_objectModified()), this, SLOT(configureObject()));

	box=new QGraphicsPolygonItem;
	text=new QGraphicsSimpleTextItem;

	text_item = new TextPolygonItem;
	this->addToGroup(text_item);

	obj_shadow=new QGraphicsPolygonItem;
	obj_shadow->setZValue(-1);
	this->addToGroup(obj_shadow);

	obj_selection=new QGraphicsPolygonItem;
	obj_selection->setVisible(false);
	obj_selection->setZValue(4);
	this->addToGroup(obj_selection);

	this->override_style=override_style;
	this->configureObject();
}

TextboxView::~TextboxView()
{
	this->removeFromGroup(text_item);
	delete text_item;
}

void TextboxView::setColorStyle(const QBrush &fill_style, const QPen &border_style)
{
	if(override_style)
	{
		text_item->setBrush(fill_style);
		text_item->setPen(border_style);
	}
}

void TextboxView::setFontStyle(const QTextCharFormat &fmt)
{
	if(override_style)
	{
		text_item->setFont(fmt.font());
		text_item->setTextBrush(fmt.foreground());
	}
}

void TextboxView::setToolTip(const QString &tooltip)
{
	txtbox_tooltip = tooltip;
}

void TextboxView::__configureObject()
{
	Textbox *txtbox=dynamic_cast<Textbox *>(this->getUnderlyingObject());
	QTextCharFormat fmt=font_config[Attributes::Global];
	QPolygonF polygon;

	polygon.append(QPointF(0.0,0.0));
	polygon.append(QPointF(1.0,0.0));
	polygon.append(QPointF(1.0,1.0));
	polygon.append(QPointF(0.0,1.0));

	if(!override_style)
	{
		QFont font;

		text_item->setBrush(this->getFillStyle(BaseObject::getSchemaName(ObjectType::Textbox)));
		text_item->setPen(this->getBorderStyle(BaseObject::getSchemaName(ObjectType::Textbox)));

		font=fmt.font();
		font.setItalic(txtbox->getTextAttribute(Textbox::ItalicText));
		font.setBold(txtbox->getTextAttribute(Textbox::BoldText));
		font.setUnderline(txtbox->getTextAttribute(Textbox::UnderlineText));
		font.setPointSizeF(txtbox->getFontSize());

		text_item->setFont(font);
		text_item->setTextBrush(txtbox->getTextColor());
	}

	text_item->setText(txtbox->getComment());
	text_item->setTextPos(HorizSpacing * 2, VertSpacing * (text_item->getFont().italic() ? 0.90 : 0.50));

	TextPolygonItem::resizePolygon(polygon, round(text_item->getTextBoundingRect().width() + (2.5 * HorizSpacing)),
																					round(text_item->getTextBoundingRect().height() + (1.5 * VertSpacing)));

	text_item->setPos(0,0);
	text_item->setPolygon(polygon);

	protected_icon->setPos(text_item->boundingRect().width() + 2 * HorizSpacing,
												 text_item->boundingRect().height() * 0.70);

	this->bounding_rect.setTopLeft(text_item->boundingRect().topLeft());
	this->bounding_rect.setBottomRight(text_item->boundingRect().bottomRight());

	this->setZValue(dynamic_cast<Textbox *>(getUnderlyingObject())->getZValue());
	BaseObjectView::__configureObject();

	if(!txtbox_tooltip.isEmpty())
		this->BaseObjectView::setToolTip(txtbox_tooltip);
}

void TextboxView::configureObject()
{
	this->__configureObject();
	this->configureObjectShadow();
	this->configureObjectSelection();
}

void TextboxView::configureObjectShadow()
{
	QGraphicsPolygonItem *pol_item=dynamic_cast<QGraphicsPolygonItem *>(obj_shadow);

	pol_item->setPen(Qt::NoPen);
	pol_item->setBrush(QColor(50,50,50,60));
	pol_item->setPolygon(text_item->polygon());
	pol_item->setPos(3.5,3.5);
}

void TextboxView::configureObjectSelection()
{
	QGraphicsPolygonItem *pol_item=dynamic_cast<QGraphicsPolygonItem *>(obj_selection);

	pol_item->setPolygon(text_item->polygon());
	pol_item->setPos(0,0);
	pol_item->setBrush(this->getFillStyle(Attributes::ObjSelection));
	pol_item->setPen(this->getBorderStyle(Attributes::ObjSelection));
}

QVariant TextboxView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change == ItemZValueHasChanged)
	{
		Textbox *txtbox = dynamic_cast<Textbox *>(getUnderlyingObject());
		txtbox->setZValue(zValue());
	}

	return BaseObjectView::itemChange(change, value);
}
