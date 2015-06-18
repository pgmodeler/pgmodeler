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

#include "tag.h"

unsigned Tag::tag_id=3000;

Tag::Tag(void)
{
  QStringList attribs={ ParsersAttributes::TABLE_NAME,  ParsersAttributes::TABLE_SCHEMA_NAME,
                        ParsersAttributes::TABLE_TITLE, ParsersAttributes::TABLE_BODY,
                        ParsersAttributes::TABLE_EXT_BODY };

  obj_type=OBJ_TAG;
  object_id=Tag::tag_id++;
  attributes[ParsersAttributes::STYLES]=QString();

  for(auto &attr : attribs)
  {
    if(attr!=ParsersAttributes::TABLE_NAME && attr!=ParsersAttributes::TABLE_SCHEMA_NAME)
      color_config[attr]=new QColor[COLOR_COUNT];
    else
      color_config[attr]=new QColor;
  }
}

void Tag::setName(const QString &name)
{
  if(name.isEmpty())
    throw Exception(ERR_ASG_EMPTY_NAME_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else if(name.size() > BaseObject::OBJECT_NAME_MAX_LENGTH)
    throw Exception(ERR_ASG_LONG_NAME_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  this->obj_name=name;
}

QString Tag::getName(bool, bool)
{
  return(this->obj_name);
}

void Tag::setElementColor(const QString &elem_id, const QColor &color, unsigned color_id)
{
  try
  {
    validateElementId(elem_id, color_id);
    color_config[elem_id][color_id]=color;
		setCodeInvalidated(true);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void Tag::setElementColors(const QString &elem_id, const QString &colors)
{
  try
  {
    QStringList color_lst=colors.split(',');
    unsigned color_id=FILL_COLOR1;

    for(auto &color : color_lst)
    {
      validateElementId(elem_id, color_id);
      color_config[elem_id][color_id]=QColor(color);
      color_id++;
    }

		setCodeInvalidated(true);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

QColor Tag::getElementColor(const QString &elem_id, unsigned color_id)
{
  try
  {
    validateElementId(elem_id, color_id);
    return(color_config[elem_id][color_id]);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void Tag::validateElementId(const QString &id, unsigned color_id)
{
  if(color_config.count(id) == 0)
    throw Exception(Exception::getErrorMessage(ERR_OPR_INV_ELEMENT_ID).arg(id),
                    ERR_OPR_INV_ELEMENT_ID ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else if((color_id > COLOR_COUNT) ||
          (color_id > 0 &&
           (id==ParsersAttributes::TABLE_NAME || id==ParsersAttributes::TABLE_SCHEMA_NAME)))
    throw Exception(Exception::getErrorMessage(ERR_REF_ELEMENT_COLOR_ID).arg(id).arg(color_id),
                    ERR_REF_ELEMENT_COLOR_ID ,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

QLinearGradient Tag::getFillStyle(const QString &elem_id)
{
  try
  {
    validateElementId(elem_id, 1);
    QLinearGradient grad(QPointF(0,0),QPointF(0,1));

    grad.setCoordinateMode(QGradient::ObjectBoundingMode);
    grad.setColorAt(0, color_config[elem_id][FILL_COLOR1]);
    grad.setColorAt(1, color_config[elem_id][FILL_COLOR2]);

    return(grad);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

QString Tag::getCodeDefinition(unsigned def_type)
{
  return(this->getCodeDefinition(def_type, false));
}

QString Tag::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	if(def_type==SchemaParser::SQL_DEFINITION)
    return(QString());
  else
  {
		QString code_def=getCachedCode(def_type, reduced_form);
		if(!code_def.isEmpty()) return(code_def);

    try
    {
      attribs_map attribs;

      for(auto &itr : color_config)
      {
        attribs[ParsersAttributes::ID]=itr.first;
        attribs[ParsersAttributes::COLORS]=QString();

        if(itr.first==ParsersAttributes::TABLE_NAME || itr.first==ParsersAttributes::TABLE_SCHEMA_NAME)
          attribs[ParsersAttributes::COLORS]=itr.second[FILL_COLOR1].name();
        else
          attribs[ParsersAttributes::COLORS]=itr.second[FILL_COLOR1].name() + QString(",") +
              itr.second[FILL_COLOR2].name() + QString(",") + itr.second[BORDER_COLOR].name();

				attributes[ParsersAttributes::STYLES]+=schparser.getCodeDefinition(ParsersAttributes::STYLE, attribs, SchemaParser::XML_DEFINITION);
      }
    }
    catch(Exception &e)
    {
      throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
    }

   return(BaseObject::getCodeDefinition(def_type, reduced_form));
  }
}

void Tag::operator = (Tag &tag)
{
  (*dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(tag);

  for(auto &attr : tag.color_config)
  {
    if(attr.first!=ParsersAttributes::TABLE_NAME && attr.first!=ParsersAttributes::TABLE_SCHEMA_NAME)
    {
      for(unsigned i=FILL_COLOR1; i < COLOR_COUNT; i++)
        this->color_config[attr.first][i]=attr.second[i];
    }
    else
      (*this->color_config[attr.first])=(*attr.second);
  }
}
