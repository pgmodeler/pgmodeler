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

const QRegExp SnippetsConfigWidget::ID_FORMAT_REGEXP=QRegExp("^([a-z])([a-z]*|(\\d)*|(_)*)+", Qt::CaseInsensitive);

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

  //Creates a combo with the accepted object type
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
  connect(update_tb, SIGNAL(clicked()), this, SLOT(handleSnippet()));
  connect(add_tb, SIGNAL(clicked()), this, SLOT(handleSnippet()));
}

void SnippetsConfigWidget::fillSnippetsCombo(map<QString, attribs_map> &config)
{
  snippets_cmb->clear();

  for(auto cfg : config)
    snippets_cmb->addItem(QString("[%1] %2").arg(cfg.first, cfg.second.at(ParsersAttributes::LABEL)), cfg.first);
}

bool SnippetsConfigWidget::isSnippetValid(attribs_map &attribs, const QString &orig_id)
{
  Messagebox msg_box;
  QString snip_id=attribs.at(ParsersAttributes::ID),
          err_msg;

  if(!orig_id.isEmpty() && snip_id!=orig_id && config_params.count(snip_id)!=0)
    err_msg=trUtf8("Duplicated snippet id <strong>%1</strong> detected. Please, specify a different one!").arg(snip_id);
  else if(!ID_FORMAT_REGEXP.exactMatch(snip_id))
    err_msg=trUtf8("Invalid ID pattern detected <strong>%1</strong>. This one must start with at leat one letter and be composed by letters, numbers and/or underscore!").arg(snip_id);
  else if(attribs[ParsersAttributes::LABEL].isEmpty())
    err_msg=trUtf8("Empty label for snippet <strong>%1</strong>. Please, specify a value for it!").arg(snip_id);
  else if(attribs[ParsersAttributes::CONTENTS])
    err_msg=trUtf8("Empty code for snippet <strong>%1</strong>. Please, specify a value for it!").arg(snip_id);

  if(!err_msg.isEmpty())
  {
    msg_box.show(trUtf8("Error"), err_msg, Messagebox::ERROR_ICON, Messagebox::OK_BUTTON);
    return(false);
  }
  else
    return(true);
}

void SnippetsConfigWidget::loadConfiguration(void)
{
	try
	{
    QStringList inv_snippets;

    BaseConfigWidget::loadConfiguration(GlobalAttributes::SNIPPETS_CONF, { ParsersAttributes::ID });

    //Check if there are invalid snippets loaded
    for(auto snip : config_params)
    {
      if(!isSnippetValid(snip.second,""))
        inv_snippets.push_back(snip.first);
    }

    //Destroy any invalid snippets
    for(QString id : inv_snippets)
      config_params.erase(id);

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
  snippet_txt->setPlainText(config_params[snip_id].at(ParsersAttributes::CONTENTS));
  id_edt->setText(snip_id);
  label_edt->setText(config_params[snip_id].at(ParsersAttributes::LABEL));
  applies_to_cmb->setCurrentText(BaseObject::getTypeName(obj_type));
}

void SnippetsConfigWidget::handleSnippet(void)
{
  QString orig_id=snippets_cmb->currentData().toString(),
          object_id=BaseObject::getSchemaName(static_cast<ObjectType>(applies_to_cmb->currentData().toUInt()));
  attribs_map snippet;

  if(object_id.isEmpty())
    object_id=ParsersAttributes::GENERAL;

  snippet=attribs_map{ {ParsersAttributes::ID, id_edt->text()},
                       {ParsersAttributes::LABEL, label_edt->text()},
                       {ParsersAttributes::OBJECT, object_id},
                       {ParsersAttributes::CONTENTS, snippet_txt->toPlainText()} };

  if(isSnippetValid(snippet, orig_id))
  {  
    config_params[id_edt->text()]=snippet;

    //If the operation is update and the snippet id changed, remove the original one
    if(sender()==update_tb && id_edt->text() != orig_id)
      config_params.erase(orig_id);

    filterSnippets(filter_cmb->currentIndex());
    resetForm();
  }
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

    if(object_id.isEmpty())
      object_id=ParsersAttributes::GENERAL;

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
    QString root_dir=GlobalAttributes::CONFIGURATIONS_DIR +
                     GlobalAttributes::DIR_SEPARATOR,

        snippet_sch=root_dir +
        GlobalAttributes::SCHEMAS_DIR +
        GlobalAttributes::DIR_SEPARATOR +
        ParsersAttributes::SNIPPET +
        GlobalAttributes::SCHEMA_EXT;

    attribs_map attribs;

    for(auto snip : config_params)
    {
      attribs[ParsersAttributes::SNIPPET]+=
       schparser.convertCharsToXMLEntities(schparser.getCodeDefinition(snippet_sch, snip.second));
    }

    config_params[GlobalAttributes::SNIPPETS_CONF]=attribs;
    BaseConfigWidget::saveConfiguration(GlobalAttributes::SNIPPETS_CONF);
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

