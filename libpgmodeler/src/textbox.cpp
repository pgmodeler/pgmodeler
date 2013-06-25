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

#include "textbox.h"

Textbox::Textbox(void)
{
	obj_type=OBJ_TEXTBOX;
	font_size=9.0f;
	text_attributes[0]=text_attributes[1]=text_attributes[2]=false;
	attributes[ParsersAttributes::ITALIC]="";
	attributes[ParsersAttributes::BOLD]="";
	attributes[ParsersAttributes::UNDERLINE]="";
	attributes[ParsersAttributes::COLOR]="";
	attributes[ParsersAttributes::FONT_SIZE]="";
}

QString Textbox::getCodeDefinition(unsigned def_type)
{
	if(def_type==SchemaParser::SQL_DEFINITION)
		return("");
	else
	{
		setPositionAttribute();

		if(text_attributes[ITALIC_TXT])
			attributes[ParsersAttributes::ITALIC]="1";

		if(text_attributes[BOLD_TXT])
			attributes[ParsersAttributes::BOLD]="1";

		if(text_attributes[UNDERLINE_TXT])
			attributes[ParsersAttributes::UNDERLINE]="1";

		if(text_color.name()!="#000000")
			attributes[ParsersAttributes::COLOR]=text_color.name();

		attributes[ParsersAttributes::FONT_SIZE]=QString("%1").arg(font_size);

		return(this->BaseObject::__getCodeDefinition(SchemaParser::XML_DEFINITION));
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
}

void Textbox::setTextAttribute(unsigned attrib, bool value)
{
	if(attrib > UNDERLINE_TXT)
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	text_attributes[attrib]=value;
}

void Textbox::setTextColor(const QColor &color)
{
	text_color=color;
}

QColor Textbox::getTextColor(void)
{
	return(text_color);
}

bool Textbox::getTextAttribute(unsigned attrib)
{
	if(attrib > UNDERLINE_TXT)
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(text_attributes[attrib]);
}

void Textbox::setFontSize(float size)
{
	font_size=(size <= 0 ? 1 : size);
}

float Textbox::getFontSize(void)
{
	return(font_size);
}

