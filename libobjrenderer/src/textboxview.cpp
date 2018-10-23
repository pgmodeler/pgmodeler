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

void TextboxView::setToolTip(const QString &tooltip)
{
	txtbox_tooltip = tooltip;
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
		text->setPos(HorizSpacing * 1.5, VertSpacing * 0.90);
	else
		text->setPos(HorizSpacing, VertSpacing);

	this->resizePolygon(polygon, roundf(text->boundingRect().width() + (2.5 * HorizSpacing)),
						roundf(text->boundingRect().height() + (1.5 * VertSpacing)));

	box->setPos(0,0);
	box->setPolygon(polygon);

	protected_icon->setPos(box->boundingRect().right() - (protected_icon->boundingRect().width() + 2 * HorizSpacing),
						   box->boundingRect().bottom()- (protected_icon->boundingRect().height() + 2 * VertSpacing));

	this->bounding_rect.setTopLeft(box->boundingRect().topLeft());
	this->bounding_rect.setBottomRight(box->boundingRect().bottomRight());
	BaseObjectView::__configureObject();

	if(!txtbox_tooltip.isEmpty())
		this->BaseObjectView::setToolTip(txtbox_tooltip);
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
