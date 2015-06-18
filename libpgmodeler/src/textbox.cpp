/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
	attributes[ParsersAttributes::ITALIC]=QString();
	attributes[ParsersAttributes::BOLD]=QString();
	attributes[ParsersAttributes::UNDERLINE]=QString();
	attributes[ParsersAttributes::COLOR]=QString();
	attributes[ParsersAttributes::FONT_SIZE]=QString();
}

QString Textbox::getCodeDefinition(unsigned def_type)
{
	if(def_type==SchemaParser::SQL_DEFINITION)
    return(QString());
	else
	{
		QString code_def=getCachedCode(def_type, false);
		if(!code_def.isEmpty()) return(code_def);

		setPositionAttribute();

		if(text_attributes[ITALIC_TXT])
      attributes[ParsersAttributes::ITALIC]=ParsersAttributes::_TRUE_;

		if(text_attributes[BOLD_TXT])
      attributes[ParsersAttributes::BOLD]=ParsersAttributes::_TRUE_;

		if(text_attributes[UNDERLINE_TXT])
      attributes[ParsersAttributes::UNDERLINE]=ParsersAttributes::_TRUE_;

    if(text_color.name()!=QString("#000000"))
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

void Textbox::setFontSize(double size)
{
	font_size=(size <= 0 ? 1 : size);
}

double Textbox::getFontSize(void)
{
	return(font_size);
}

