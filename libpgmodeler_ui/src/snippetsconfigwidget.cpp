/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "snippetsconfigwidget.h"
#include "baseobject.h"

SnippetsConfigWidget::SnippetsConfigWidget(QWidget * parent) : QWidget(parent)
{
  map<QString, ObjectType> types_map;
  vector<ObjectType> types=BaseObject::getObjectTypes(true, {OBJ_RELATIONSHIP, OBJ_TAG, OBJ_TEXTBOX,
                                                             OBJ_PERMISSION, BASE_RELATIONSHIP });

  setupUi(this);

  for(ObjectType type : types)
    types_map[BaseObject::getTypeName(type)]=type;

  for(auto itr : types_map)
    applies_to_cmb->addItem(QPixmap(QString(":/icones/icones/%1.png").arg(BaseObject::getSchemaName(itr.second))),
                            itr.first, itr.second);

  applies_to_cmb->insertItem(0, trUtf8("General purpose"), BASE_OBJECT);
  applies_to_cmb->setCurrentIndex(0);

  try
  {
    snippet_hl=new SyntaxHighlighter(snippet_txt, false);
    snippet_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void SnippetsConfigWidget::loadConfiguration(void)
{
	try
	{
    BaseConfigWidget::loadConfiguration(GlobalAttributes::SNIPPETS_CONF, { ParsersAttributes::ID });
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SnippetsConfigWidget::saveConfiguration(void)
{
	try
	{
    /* attribs_map attribs;
		vector<AppearanceConfigItem>::iterator itr, itr_end;
		AppearanceConfigItem item;
		QString attrib_id;
		QFont font;

		itr=conf_items.begin();
		itr_end=conf_items.end();

		while(itr!=itr_end)
		{
			item=(*itr);
			itr++;

			//If the item is a object color config
			if(item.obj_conf)
			{
				//Creates an attribute that stores the fill color
				attrib_id=item.conf_id + QString("-color");
				if(item.colors[0]==item.colors[1])
					attribs[attrib_id]=item.colors[0].name();
				else
					attribs[attrib_id]=item.colors[0].name() + QString(",") + item.colors[1].name();

				//Creates an attribute that stores the border color
				attrib_id=item.conf_id + QString("-bcolor");
				attribs[attrib_id]=item.colors[2].name();
			}
			//If the item is a font config
			else if(item.conf_id!=ParsersAttributes::GLOBAL && !item.obj_conf)
			{
				font=item.font_fmt.font();

				//Creates an attribute to store the font color
				attrib_id=item.conf_id + QString("-fcolor");
				attribs[attrib_id]=item.font_fmt.foreground().color().name();

				attrib_id=item.conf_id + QString("-") + ParsersAttributes::ITALIC;
				attribs[attrib_id]=(font.italic() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

				attrib_id=item.conf_id + QString("-") + ParsersAttributes::BOLD;
				attribs[attrib_id]=(font.bold() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

				attrib_id=item.conf_id + QString("-") + ParsersAttributes::UNDERLINE;
				attribs[attrib_id]=(font.underline() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);
			}
			//Special case: treating the global font element
			else
			{
				attribs["font-name"]=QFontInfo(item.font_fmt.font()).family();
				attribs["font-size"]=QString("%1").arg(item.font_fmt.font().pointSizeF());
			}
		}

		config_params[GlobalAttributes::OBJECTS_STYLE_CONF]=attribs;
    BaseConfigWidget::saveConfiguration(GlobalAttributes::OBJECTS_STYLE_CONF); */
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SnippetsConfigWidget::restoreDefaults(void)
{
	try
	{
    BaseConfigWidget::restoreDefaults(GlobalAttributes::SNIPPETS_CONF);
		this->loadConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

