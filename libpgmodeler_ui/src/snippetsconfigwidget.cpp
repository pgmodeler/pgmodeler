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

#include "snippetsconfigwidget.h"
#include "baseobject.h"
#include "messagebox.h"

map<QString, attribs_map> SnippetsConfigWidget::config_params;

const QRegExp SnippetsConfigWidget::ID_FORMAT_REGEXP=QRegExp(QString("^([a-z])([a-z]*|(\\d)*|(_)*)+"), Qt::CaseInsensitive);

SnippetsConfigWidget::SnippetsConfigWidget(QWidget * parent) : BaseConfigWidget(parent)
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
  for(auto &itr : types_map)
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

  parsable_ht=new HintTextWidget(parsable_hint, this);
  parsable_ht->setText(parsable_chk->statusTip());

  placeholders_ht=new HintTextWidget(placeholders_hint, this);
  placeholders_ht->setText(placeholders_chk->statusTip());

  snippet_txt=new NumberedTextEditor(this);

  QGridLayout *grid=dynamic_cast<QGridLayout *>(frame->layout());
  grid->addWidget(snippet_txt, grid->count(), 0, 1, 3);

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
  connect(parsable_chk, SIGNAL(toggled(bool)), this, SLOT(enableSaveButtons()));
  connect(filter_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(filterSnippets(int)));
  connect(update_tb, SIGNAL(clicked()), this, SLOT(handleSnippet()));
  connect(add_tb, SIGNAL(clicked()), this, SLOT(handleSnippet()));
  connect(parse_tb, SIGNAL(clicked()), this, SLOT(parseSnippet()));
  connect(parsable_chk, SIGNAL(toggled(bool)), placeholders_chk, SLOT(setEnabled(bool)));
}

map<QString, attribs_map> SnippetsConfigWidget::getConfigurationParams(void)
{
  return(config_params);
}

attribs_map SnippetsConfigWidget::getSnippetById(const QString &snip_id)
{
  if(config_params.count(snip_id))
    return(config_params[snip_id]);
  else
    return(attribs_map());
}

QStringList SnippetsConfigWidget::getSnippetsIdsByObject(ObjectType obj_type)
{
  QStringList ids;
  QString type_name=(obj_type==BASE_OBJECT ?
                     ParsersAttributes::GENERAL : BaseObject::getSchemaName(obj_type));

  for(auto &snip : config_params)
  {
    if(snip.second[ParsersAttributes::OBJECT]==type_name)
      ids.push_back(snip.second[ParsersAttributes::ID]);
  }

  return(ids);
}

vector<attribs_map> SnippetsConfigWidget::getSnippetsByObject(ObjectType obj_type)
{
  vector<attribs_map> snippets;
  QString type_name=(obj_type==BASE_OBJECT ?
                     ParsersAttributes::GENERAL : BaseObject::getSchemaName(obj_type));

  for(auto &snip : config_params)
  {
    if(snip.second[ParsersAttributes::OBJECT]==type_name)
      snippets.push_back(snip.second);
  }

  return(snippets);
}

QStringList SnippetsConfigWidget::getAllSnippetsAttribute(const QString &attrib)
{
  QStringList attribs;

  for(auto &snip : config_params)
  {
    if(snip.second.count(attrib))
      attribs.push_back(snip.second[attrib]);
  }

  return(attribs);
}

vector<attribs_map> SnippetsConfigWidget::getAllSnippets(void)
{
  vector<attribs_map> snippets;

  for(auto &snip : config_params)
    snippets.push_back(snip.second);

  return(snippets);
}

QString SnippetsConfigWidget::parseSnippet(attribs_map snippet, attribs_map attribs)
{ 
  SchemaParser schparser;
  QStringList aux_attribs;
  QString buf=snippet[ParsersAttributes::CONTENTS];

  if(snippet[ParsersAttributes::PARSABLE]!=ParsersAttributes::_TRUE_)
    return(buf);

  try
  {
    schparser.loadBuffer(buf);

    //Assigning dummy values for empty attributes
    if(snippet[ParsersAttributes::PLACEHOLDERS]==ParsersAttributes::_TRUE_)
    {
      aux_attribs=schparser.extractAttributes();
      for(QString attr : aux_attribs)
      {
        if(attribs.count(attr)==0 ||
           (attribs.count(attr) && attribs[attr].isEmpty()))
          attribs[attr]=QString("{%1}").arg(attr);
      }
    }

    schparser.ignoreEmptyAttributes(true);
    schparser.ignoreUnkownAttributes(true);
    return(schparser.getCodeDefinition(attribs));
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

QString SnippetsConfigWidget::getParsedSnippet(const QString &snip_id, attribs_map attribs)
{
  if(config_params.count(snip_id))
  {
    try
    {
      return(parseSnippet(config_params[snip_id], attribs));
    }
    catch(Exception &e)
    {
      return(trUtf8("/* Error parsing the snippet '%1':\n\n %2 */").arg(snip_id, e.getErrorMessage()));
    }
  }
  else
    return(QString());
}

void SnippetsConfigWidget::fillSnippetsCombo(map<QString, attribs_map> &config)
{
  snippets_cmb->clear();

  for(auto &cfg : config)
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
  else if(attribs[ParsersAttributes::CONTENTS].isEmpty())
    err_msg=trUtf8("Empty code for snippet <strong>%1</strong>. Please, specify a value for it!").arg(snip_id);
  else if(attribs[ParsersAttributes::PARSABLE]==ParsersAttributes::_TRUE_)
  {
    try
    {
      QString buf=snippet_txt->toPlainText();
      attribs_map attribs;
      SchemaParser schparser;

      schparser.loadBuffer(buf);
      schparser.ignoreEmptyAttributes(true);
      schparser.ignoreUnkownAttributes(true);
      schparser.getCodeDefinition(attribs);
    }
    catch(Exception &e)
    {
      err_msg=trUtf8("The dynamic snippet contains syntax error(s). Additional info: <br/><em>%1</em>").arg(e.getErrorMessage());
    }
  }

  if(!err_msg.isEmpty())
  {
    msg_box.show(err_msg, Messagebox::ERROR_ICON, Messagebox::OK_BUTTON);
    return(false);
  }
  else
    return(true);
}

void SnippetsConfigWidget::hideEvent(QHideEvent *)
{
  enableEditMode(false);
}

void SnippetsConfigWidget::loadConfiguration(void)
{
	try
	{
    QStringList inv_snippets;

    this->resetForm();
    BaseConfigWidget::loadConfiguration(GlobalAttributes::SNIPPETS_CONF, config_params, { ParsersAttributes::ID });

    //Check if there are invalid snippets loaded
    for(auto &snip : config_params)
    {
      if(!isSnippetValid(snip.second,QString()))
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
  parsable_chk->setChecked(false);
  placeholders_chk->setChecked(false);
}

attribs_map SnippetsConfigWidget::getSnippetAttributes(void)
{
  QString object_id=BaseObject::getSchemaName(static_cast<ObjectType>(applies_to_cmb->currentData().toUInt()));

  if(object_id.isEmpty())
    object_id=ParsersAttributes::GENERAL;

  return(attribs_map{ {ParsersAttributes::ID, id_edt->text()},
                      {ParsersAttributes::LABEL, label_edt->text()},
                      {ParsersAttributes::OBJECT, object_id},
                      {ParsersAttributes::PARSABLE, (parsable_chk->isChecked() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_)},
                      {ParsersAttributes::PLACEHOLDERS, (parsable_chk->isChecked() && placeholders_chk->isChecked() ?
                                                         ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_)},
                      {ParsersAttributes::CONTENTS, snippet_txt->toPlainText()} });
}

void SnippetsConfigWidget::editSnippet(void)
{
  QString snip_id=snippets_cmb->currentData().toString();
  ObjectType obj_type=BaseObject::getObjectType(config_params[snip_id].at(ParsersAttributes::OBJECT));

  enableEditMode(true);
  snippet_txt->setPlainText(config_params[snip_id].at(ParsersAttributes::CONTENTS));
  id_edt->setText(snip_id);
  label_edt->setText(config_params[snip_id].at(ParsersAttributes::LABEL));
  parsable_chk->setChecked(config_params[snip_id].at(ParsersAttributes::PARSABLE)==ParsersAttributes::_TRUE_);
  placeholders_chk->setChecked(config_params[snip_id].at(ParsersAttributes::PLACEHOLDERS)==ParsersAttributes::_TRUE_);
  applies_to_cmb->setCurrentText(BaseObject::getTypeName(obj_type));
}

void SnippetsConfigWidget::handleSnippet(void)
{
  QString orig_id=snippets_cmb->currentData().toString();
  attribs_map snippet;

  snippet=getSnippetAttributes();

  if(isSnippetValid(snippet, orig_id))
  {  
    config_params[id_edt->text()]=snippet;

    //If the operation is update and the snippet id changed, remove the original one
    if(sender()==update_tb && id_edt->text() != orig_id)
      config_params.erase(orig_id);

    filterSnippets(filter_cmb->currentIndex());
    resetForm();
    setConfigurationChanged(true);
  }
}

void SnippetsConfigWidget::removeSnippet(void)
{
  config_params.erase(snippets_cmb->currentData().toString());
  filterSnippets(filter_cmb->currentIndex());
  setConfigurationChanged(true);
}

void SnippetsConfigWidget::removeAllSnippets(void)
{
  Messagebox msg_box;

  msg_box.show(trUtf8("Do you really want to remove all snippets?"),
               Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

  if(msg_box.result()==QDialog::Accepted)
  {
    config_params.clear();
    filterSnippets(0);
    setConfigurationChanged(true);
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
  parse_tb->setEnabled(enable && parsable_chk->isChecked());
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

    for(auto &cfg : config_params)
    {
      if(cfg.second.at(ParsersAttributes::OBJECT)==object_id)
        flt_snippets[cfg.first]=cfg.second;
    }

    fillSnippetsCombo(flt_snippets);
  }
}

void SnippetsConfigWidget::parseSnippet(void)
{
  Messagebox msg_box;

  try
  {
    parseSnippet(getSnippetAttributes(), attribs_map());
    msg_box.show(trUtf8("No syntax errors found in the snippet."), Messagebox::INFO_ICON);
  }
  catch(Exception &e)
  {
    msg_box.show(e);
  }
}

void SnippetsConfigWidget::saveConfiguration(void)
{ 
	try
	{
    QString root_dir=GlobalAttributes::TMPL_CONFIGURATIONS_DIR +
                     GlobalAttributes::DIR_SEPARATOR,

        snippet_sch=root_dir +
                    GlobalAttributes::SCHEMAS_DIR +
                    GlobalAttributes::DIR_SEPARATOR +
                    ParsersAttributes::SNIPPET +
                    GlobalAttributes::SCHEMA_EXT;

    attribs_map attribs;
    ObjectType obj_type;
    vector<attribs_map> snippets;

    //Saving the snippets sorting them by object type in the output file
    for(int i=0; i < applies_to_cmb->count(); i++)
    {
      obj_type=static_cast<ObjectType>(applies_to_cmb->itemData(i).toUInt());
      snippets=getSnippetsByObject(obj_type);

      for(auto &snip : snippets)
      {
        attribs[ParsersAttributes::SNIPPET]+=
         schparser.convertCharsToXMLEntities(schparser.getCodeDefinition(snippet_sch, snip));
      }
    }

    config_params[GlobalAttributes::SNIPPETS_CONF]=attribs;
    BaseConfigWidget::saveConfiguration(GlobalAttributes::SNIPPETS_CONF, config_params);
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
    setConfigurationChanged(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SnippetsConfigWidget::configureSnippetsMenu(QMenu *snip_menu, vector<ObjectType> types)
{
  vector<attribs_map> snippets, vet_aux;
  QAction *act=nullptr;
  QMenu *menu=nullptr;
  map<QString, QMenu *> submenus;
  QString object, snip_id, type_name;
  QPixmap ico;

  //Retrieve all snippets if the 'types' is empty
  if(types.empty())
    snippets=SnippetsConfigWidget::getAllSnippets();
  else
  {
    //Retreving only the snippets of the type specified by user
    for(ObjectType type : types)
    {
      vet_aux=SnippetsConfigWidget::getSnippetsByObject(type);
      snippets.insert(snippets.end(), vet_aux.begin(), vet_aux.end());
    }
  }

  snip_menu->clear();

  for(attribs_map snip : snippets)
  {
    object=snip[ParsersAttributes::OBJECT];
    snip_id=snip[ParsersAttributes::ID];

    //Creating the snippet submenu for the current object type
    if(submenus.count(object)==0)
    {
      type_name=BaseObject::getTypeName(object);

      if(type_name.isEmpty())
      {
        ico=QPixmap();
        type_name=trUtf8("General");
      }
      else
        ico=QPixmap(QString(":/icones/icones/%1.png").arg(object));

      menu=new QMenu(type_name, snip_menu);
      menu->setIcon(ico);
      menu->setToolTipsVisible(true);
      submenus[object]=menu;

      /* If the current group (object) is general does not include the submenu yet.
         This will be included as the last submenu */
      if(object!=ParsersAttributes::GENERAL)
        snip_menu->addMenu(menu);
    }

    //Creating the action for the current snippet
    act=new QAction(QPixmap(QString(":/icones/icones/codesnippet.png")), snip_id, submenus[object]);
    act->setToolTip(snip[ParsersAttributes::LABEL]);
    submenus[object]->addAction(act);
  }

  //Include the "general" submenu at the end of snippet menu
  if(submenus.count(ParsersAttributes::GENERAL)!=0)
    snip_menu->addMenu(submenus[ParsersAttributes::GENERAL]);
}

bool SnippetsConfigWidget::isSnippetExists(const QString &snip_id)
{
  return(config_params.count(snip_id)!=0);
}
