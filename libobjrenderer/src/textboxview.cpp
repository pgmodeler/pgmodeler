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

#include "textboxview.h"

TextboxView::TextboxView(Textbox *txtbox, bool override_style) : BaseObjectView(txtbox)
{
	connect(txtbox, SIGNAL(s_objectModified(void)), this, SLOT(configureObject(void)));

	box=new QGraphicsPolygonItem;
	text=new QGraphicsSimpleTextItem;

	this->override_style=override_style;
	this->addToGroup(text);
	this->addToGroup(box);
	this->configureObject();
}

TextboxView::~TextboxView(void)
{
	disconnect(this, SLOT(configureObject(void)));

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

void TextboxView::configureObject(void)
{
	Textbox *txtbox=dynamic_cast<Textbox *>(this->getSourceObject());
	QTextCharFormat fmt=font_config[ParsersAttributes::GLOBAL];
	QFont font;
	QPolygonF polygon;

	polygon.append(QPointF(0.0f,0.0f));
	polygon.append(QPointF(1.0f,0.0f));
	polygon.append(QPointF(1.0f,1.0f));
	polygon.append(QPointF(0.0f,1.0f));

	//The textbox view must be at the bottom of objects stack (Z = 0)
	box->setZValue(0);
	text->setZValue(1);

	if(!override_style)
	{
		box->setBrush(this->getFillStyle(BaseObject::getSchemaName(OBJ_TEXTBOX)));
		box->setPen(this->getBorderStyle(BaseObject::getSchemaName(OBJ_TEXTBOX)));

		font=fmt.font();
		font.setItalic(txtbox->getTextAttribute(Textbox::ITALIC_TXT));
		font.setBold(txtbox->getTextAttribute(Textbox::BOLD_TXT));
		font.setUnderline(txtbox->getTextAttribute(Textbox::UNDERLINE_TXT));

		text->setFont(font);
		text->setBrush(txtbox->getTextColor());
	}

	text->setText(Utf8String::create(txtbox->getComment()));
	text->setPos(HORIZ_SPACING, VERT_SPACING);
	this->resizePolygon(polygon, roundf(text->boundingRect().width() + (2 * HORIZ_SPACING)),
											roundf(text->boundingRect().height() + (2* VERT_SPACING)));
	box->setPos(0,0);
	box->setPolygon(polygon);

	protected_icon->setPos(box->boundingRect().right() - (protected_icon->boundingRect().width() + 2 * HORIZ_SPACING),
												 box->boundingRect().bottom()- (protected_icon->boundingRect().height() + 2 * VERT_SPACING));

	this->bounding_rect.setTopLeft(box->boundingRect().topLeft());
	this->bounding_rect.setBottomRight(box->boundingRect().bottomRight());

	BaseObjectView::__configureObject();
	BaseObjectView::configureObjectShadow();
	BaseObjectView::configureObjectSelection();
}

