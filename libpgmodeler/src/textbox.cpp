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

#include "textbox.h"

Textbox::Textbox()
{
	obj_type=ObjectType::Textbox;
	font_size=9.0;
	text_attributes[0]=text_attributes[1]=text_attributes[2]=false;
	attributes[Attributes::Italic]=QString();
	attributes[Attributes::Bold]=QString();
	attributes[Attributes::Underline]=QString();
	attributes[Attributes::Color]=QString();
	attributes[Attributes::FontSize]=QString();
}

QString Textbox::getCodeDefinition(unsigned def_type)
{
	if(def_type==SchemaParser::SqlDefinition)
		return QString();
	else
	{
		QString code_def=getCachedCode(def_type, false);
		if(!code_def.isEmpty()) return code_def;

		setPositionAttribute();
		setFadedOutAttribute();

		if(text_attributes[ItalicText])
			attributes[Attributes::Italic]=Attributes::True;

		if(text_attributes[BoldText])
			attributes[Attributes::Bold]=Attributes::True;

		if(text_attributes[UnderlineText])
			attributes[Attributes::Underline]=Attributes::True;

		if(text_color.name()!=QString("#000000"))
			attributes[Attributes::Color]=text_color.name();

		attributes[Attributes::FontSize]=QString("%1").arg(font_size);
		attributes[Attributes::Layer]=QString::number(layer);
		attributes[Attributes::ZValue]=QString::number(z_value);

		return this->BaseObject::__getCodeDefinition(SchemaParser::XmlDefinition);
	}
}

void Textbox::operator = (Textbox &txtbox)
{
	(*dynamic_cast<BaseGraphicObject *>(this))=reinterpret_cast<BaseGraphicObject &>(txtbox);
	this->comment=txtbox.comment;
	this->text_attributes[0]=txtbox.text_attributes[0];
	this->text_attributes[1]=txtbox.text_attributes[1];
	this->text_attributes[2]=txtbox.text_attributes[2];
	this->text_color=txtbox.text_color;
	this->font_size=txtbox.font_size;
	this->layer = txtbox.layer;
}

void Textbox::setTextAttribute(unsigned attrib, bool value)
{
	if(attrib > UnderlineText)
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	text_attributes[attrib]=value;
}

void Textbox::setTextColor(const QColor &color)
{
	text_color=color;
}

QColor Textbox::getTextColor()
{
	return text_color;
}

bool Textbox::getTextAttribute(unsigned attrib)
{
	if(attrib > UnderlineText)
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return text_attributes[attrib];
}

void Textbox::setFontSize(double size)
{
	font_size=(size <= 0 ? 1 : size);
}

double Textbox::getFontSize()
{
	return font_size;
}

void Textbox::setZValue(int z_value)
{
	setCodeInvalidated(this->z_value != z_value);
	BaseGraphicObject::setZValue(z_value);
}
