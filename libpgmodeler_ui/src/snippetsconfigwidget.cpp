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
#include "messagebox.h"

SnippetsConfigWidget::SnippetsConfigWidget(QWidget * parent) : QWidget(parent)
{
  QPixmap ico;
  QString gen_purpose=trUtf8("General purpose");
  map<QString, ObjectType> types_map;
  vector<ObjectType> types=BaseObject::getObjectTypes(true, {OBJ_RELATIONSHIP, OBJ_TAG, OBJ_TEXTBOX,
                                                             OBJ_PERMISSION, BASE_RELATIONSHIP });

  setupUi(this);

  for(ObjectType type : types)
    types_map[BaseObject::getTypeName(type)]=type;

  for(auto itr : types_map)
  {
    ico.load(QString(":/icones/icones/%1.png").arg(BaseObject::getSchemaName(itr.second)));
    applies_to_cmb->addItem(ico, itr.first, itr.second);
    filter_cmb->addItem(ico, itr.first, itr.second);
  }

  applies_to_cmb->insertItem(0, gen_purpose, BASE_OBJECT);
  applies_to_cmb->setCurrentIndex(0);

  filter_cmb->insertItem(0, gen_purpose, BASE_OBJECT);
  filter_cmb->insertItem(0, trUtf8("All snippets"));
  filter_cmb->setCurrentIndex(0);

  try
  {
    snippet_hl=new SyntaxHighlighter(snippet_txt, false);
    snippet_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }


  enableEditMode(false);

  connect(new_tb, SIGNAL(clicked()), this, SLOT(resetForm()));
  connect(edit_tb, SIGNAL(clicked()), this, SLOT(editSnippet()));
  connect(remove_tb, SIGNAL(clicked()), this, SLOT(removeSnippet()));
  connect(remove_all_tb, SIGNAL(clicked()), this, SLOT(removeAllSnippets()));
  connect(cancel_tb, &QToolButton::clicked, [=](){ enableEditMode(false); });
  connect(snippets_cmb, &QComboBox::currentTextChanged, [=](){ enableEditMode(false); });
  connect(id_edt, SIGNAL(textChanged(QString)), this, SLOT(enableSaveButtons()));
  connect(label_edt, SIGNAL(textChanged(QString)), this, SLOT(enableSaveButtons()));
  connect(snippet_txt, SIGNAL(textChanged()), this, SLOT(enableSaveButtons()));
  connect(filter_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(filterSnippets(int)));
}

void SnippetsConfigWidget::fillSnippetsCombo(map<QString, attribs_map> &config)
{
  snippets_cmb->clear();

  for(auto cfg : config)
    snippets_cmb->addItem(QString("[%1] %2").arg(cfg.first, cfg.second.at(ParsersAttributes::LABEL)), cfg.first);
}

void SnippetsConfigWidget::loadConfiguration(void)
{
	try
	{
    BaseConfigWidget::loadConfiguration(GlobalAttributes::SNIPPETS_CONF, { ParsersAttributes::ID });
    fillSnippetsCombo(config_params);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void SnippetsConfigWidget::resetForm(void)
{
  snippet_txt->clear();
  id_edt->clear();
  label_edt->clear();
  applies_to_cmb->setCurrentIndex(0);
}

void SnippetsConfigWidget::editSnippet(void)
{
  QString snip_id=snippets_cmb->currentData().toString();
  ObjectType obj_type=BaseObject::getObjectType(config_params[snip_id].at(ParsersAttributes::OBJECT));

  enableEditMode(true);
  snippet_txt->setPlainText(config_params[snip_id].at(ParsersAttributes::_CONTENTS_));
  id_edt->setText(snip_id);
  label_edt->setText(config_params[snip_id].at(ParsersAttributes::LABEL));
  applies_to_cmb->setCurrentText(BaseObject::getTypeName(obj_type));
}

void SnippetsConfigWidget::removeSnippet(void)
{
  config_params.erase(snippets_cmb->currentData().toString());
  filterSnippets(filter_cmb->currentIndex());
}

void SnippetsConfigWidget::removeAllSnippets(void)
{
  Messagebox msg_box;

  msg_box.show(trUtf8("Confirmation"), trUtf8("Do you really want to remove all snippets?"),
               Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

  if(msg_box.result()==QDialog::Accepted)
  {
    config_params.clear();
    filterSnippets(0);
  }
}

void SnippetsConfigWidget::enableEditMode(bool enable)
{
  bool has_snippets=(snippets_cmb->count() > 0);

  cancel_tb->setVisible(enable);
  new_tb->setVisible(!enable);
  snippets_cmb->setEnabled(!enable);
  filter_cmb->setEnabled(!enable);
  add_tb->setVisible(!enable);
  update_tb->setVisible(enable);

  edit_tb->setEnabled(!enable && has_snippets);
  remove_tb->setEnabled(!enable && has_snippets);
  remove_all_tb->setEnabled(!enable && has_snippets);

  if(!enable) resetForm();
}

void SnippetsConfigWidget::enableSaveButtons(void)
{
  bool enable=(!id_edt->text().isEmpty() &&
               !label_edt->text().isEmpty() &&
               !snippet_txt->toPlainText().isEmpty());

  add_tb->setEnabled(enable);
  update_tb->setEnabled(enable);
}

void SnippetsConfigWidget::filterSnippets(int idx)
{
  if(idx <= 0)
    fillSnippetsCombo(config_params);
  else
  {
    ObjectType obj_type=static_cast<ObjectType>(filter_cmb->currentData().toUInt());
    map<QString, attribs_map> flt_snippets;
    QString object_id=BaseObject::getSchemaName(obj_type);

    for(auto cfg : config_params)
    {
      if(cfg.second.at(ParsersAttributes::OBJECT)==object_id)
        flt_snippets[cfg.first]=cfg.second;
    }

    fillSnippetsCombo(flt_snippets);
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

