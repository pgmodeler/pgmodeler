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

#include "databaseexplorerwidget.h"
#include "databaseimportform.h"
#include "sqltoolwidget.h"

using namespace ParsersAttributes;

const QString DatabaseExplorerWidget::DEP_NOT_DEFINED=QT_TR_NOOP("(not defined)");
const QString DatabaseExplorerWidget::DEP_NOT_FOUND=QT_TR_NOOP("(not found)");

const attribs_map DatabaseExplorerWidget::attribs_i18n {
  {ADMIN_ROLES, QT_TR_NOOP("Admin. roles")},           {ALIGNMENT, QT_TR_NOOP("Alignment")},              {ANALYZE_FUNC, QT_TR_NOOP("Analyze func.")},
  {ARG_COUNT, QT_TR_NOOP("Arg. count")},               {ARG_DEF_COUNT, QT_TR_NOOP("Arg. default count")}, {ARG_DEFAULTS, QT_TR_NOOP("Arg. defaults")},
  {ARG_MODES, QT_TR_NOOP("Arg. modes")},               {ARG_NAMES, QT_TR_NOOP("Arg. names")},             {ARG_TYPES, QT_TR_NOOP("Arg. types")},
  {ATTRIBUTE, QT_TR_NOOP("Attribute")},                {BEHAVIOR_TYPE, QT_TR_NOOP("Behavior type")},      {BY_VALUE, QT_TR_NOOP("By value")},
  {CAST_TYPE, QT_TR_NOOP("Cast type")},                {CATEGORY, QT_TR_NOOP("Category")},                {COLLATABLE, QT_TR_NOOP("Collatable")},
  {COLLATION, QT_TR_NOOP("Collation")},                {COMMENT, QT_TR_NOOP("Comment")},                  {COMMUTATOR_OP, QT_TR_NOOP("Commutator Op.")},
  {CONFIGURATION, QT_TR_NOOP("Configuration")},        {CONN_LIMIT, QT_TR_NOOP("Conn. limit")},           {CONSTRAINT, QT_TR_NOOP("Constraint")},
  {CREATEDB, QT_TR_NOOP("Create DB")},                 {CREATEROLE, QT_TR_NOOP("Create role")},           {CUR_VERSION, QT_TR_NOOP("Curr. version")},
  {DEFAULT, QT_TR_NOOP("Default")},                    {DEFAULT_VALUE, QT_TR_NOOP("Default value")},      {DEFINITION, QT_TR_NOOP("Definition")},
  {DELIMITER, QT_TR_NOOP("Delimiter")},                {DEST_TYPE, QT_TR_NOOP("Dest. type")},             {DIMENSION, QT_TR_NOOP("Dimension")},
  {DIRECTORY, QT_TR_NOOP("Directory")},                {DST_ENCODING, QT_TR_NOOP("Dest. encoding")},      {ELEMENT, QT_TR_NOOP("Element")},
  {ENCODING, QT_TR_NOOP("Encoding")},                  {ENCRYPTED, QT_TR_NOOP("Encrypted")},              {ENUMERATIONS, QT_TR_NOOP("Enumerations")},
  {EXECUTION_COST, QT_TR_NOOP("Exec. cost")},          {EXPRESSION, QT_TR_NOOP("Expression")},            {FAMILY, QT_TR_NOOP("Op. family")},
  {FINAL_FUNC, QT_TR_NOOP("Final func.")},             {FUNCTION, QT_TR_NOOP("Function")},                {FUNCTION_TYPE, QT_TR_NOOP("Func. type")},
  {HANDLER_FUNC, QT_TR_NOOP("Handler func.")},         {HANDLES_TYPE, QT_TR_NOOP("Handles type")},        {HASHES, QT_TR_NOOP("Hashes")},
  {INDEX_TYPE, QT_TR_NOOP("Index type")},              {INHERIT, QT_TR_NOOP("Inherit")},                  {INITIAL_COND, QT_TR_NOOP("Ini. condition")},
  {INLINE_FUNC, QT_TR_NOOP("Inline func.")},           {INPUT_FUNC, QT_TR_NOOP("Input func.")},           {INTERNAL_LENGTH, QT_TR_NOOP("Internal length")},
  {INTERVAL_TYPE, QT_TR_NOOP("Interval type")},        {IO_CAST, QT_TR_NOOP("I/O cast")},                 {JOIN_FUNC, QT_TR_NOOP("Join func.")},
  {LANGUAGE, QT_TR_NOOP("Language")},                  {_LC_COLLATE_, QT_TR_NOOP("LC COLLATE")},          {_LC_CTYPE_, QT_TR_NOOP("LC CTYPE")},
  {LEAKPROOF, QT_TR_NOOP("Leak proof")},               {LEFT_TYPE, QT_TR_NOOP("Left type")},              {LENGTH, QT_TR_NOOP("Length")},
  {LIBRARY, QT_TR_NOOP("Library")},                    {LOGIN, QT_TR_NOOP("Can login")},                  {MATERIALIZED, QT_TR_NOOP("Materialized")},
  {MEMBER_ROLES, QT_TR_NOOP("Member roles")},          {MERGES, QT_TR_NOOP("Merges")},                    {NAME, QT_TR_NOOP("Name")},
  {NEGATOR_OP, QT_TR_NOOP("Negator op.")},             {NOT_NULL, QT_TR_NOOP("Not null")},                {OBJECT_TYPE, QT_TR_NOOP("Object type")},
  {OID, QT_TR_NOOP("OID")},                            {OIDS, QT_TR_NOOP("OIDs")},                        {OLD_VERSION, QT_TR_NOOP("Old version")},
  {OPERATOR, QT_TR_NOOP("Operator")},                  {OPERATOR_FUNC, QT_TR_NOOP("Operator func.")},     {OUTPUT_FUNC, QT_TR_NOOP("Output func.")},
  {OWNER, QT_TR_NOOP("Owner")},                        {OWNER_COLUMN, QT_TR_NOOP("Owner column")},        {PARENTS, QT_TR_NOOP("Parents")},
  {PASSWORD, QT_TR_NOOP("Password")},                  {PERMISSION, QT_TR_NOOP("Permissions")},           {PRECISION, QT_TR_NOOP("Precision")},
  {PREFERRED, QT_TR_NOOP("Preferred")},                {RANGE_ATTRIBS, QT_TR_NOOP("Range attributes")},   {RECV_FUNC, QT_TR_NOOP("Receive func.")},
  {REF_ROLES, QT_TR_NOOP("Ref. roles")},               {REPLICATION, QT_TR_NOOP("Replication")},          {RESTRICTION_FUNC, QT_TR_NOOP("Restriction func.")},
  {RETURN_TYPE, QT_TR_NOOP("Return type")},            {RETURNS_SETOF, QT_TR_NOOP("Returns SETOF")},      {RIGHT_TYPE, QT_TR_NOOP("Right type")},
  {ROW_AMOUNT, QT_TR_NOOP("Row amount")},              {SCHEMA, QT_TR_NOOP("Schema")},                    {SECURITY_TYPE, QT_TR_NOOP("Security type")},
  {SEND_FUNC, QT_TR_NOOP("Send func.")},               {SORT_OP, QT_TR_NOOP("Sort op.")},                 {SOURCE_TYPE, QT_TR_NOOP("Source type")},
  {SRC_ENCODING, QT_TR_NOOP("Src. encoding")},         {STATE_TYPE, QT_TR_NOOP("State type")},            {STORAGE, QT_TR_NOOP("Storage")},
  {SUPERUSER, QT_TR_NOOP("Superuser")},                {TABLESPACE, QT_TR_NOOP("Tablespace")},            {TPMOD_IN_FUNC, QT_TR_NOOP("Type mod. in func.")},
  {TPMOD_OUT_FUNC, QT_TR_NOOP("Type mod. out func.")}, {TRANSITION_FUNC, QT_TR_NOOP("Transition func.")}, {TRUSTED, QT_TR_NOOP("Trusted")},
  {TYPE, QT_TR_NOOP("Type")},                          {TYPE_ATTRIBUTE, QT_TR_NOOP("Type attribute")},    {TYPES, QT_TR_NOOP("Types")},
  {UNLOGGED, QT_TR_NOOP("Unlogged")},                  {VALIDATOR, QT_TR_NOOP("Validator func.")},        {VALIDITY, QT_TR_NOOP("Validity")},
  {WINDOW_FUNC, QT_TR_NOOP("Windows func.")},          {_FALSE_, QT_TR_NOOP("false")},                    {_TRUE_, QT_TR_NOOP("true")}
};

DatabaseExplorerWidget::DatabaseExplorerWidget(QWidget *parent): QWidget(parent)
{
  setupUi(this);

  drop_action=new QAction(QIcon(":icones/icones/excluir.png"), trUtf8("Drop object"), &handle_menu);
  drop_action->setShortcut(QKeySequence(Qt::Key_Delete));

  drop_cascade_action=new QAction(QIcon(":icones/icones/excluir.png"), trUtf8("Drop cascade"), &handle_menu);
  drop_cascade_action->setShortcut(QKeySequence("Shift+Del"));

  show_data_action=new QAction(QIcon(":icones/icones/result.png"), trUtf8("Show data"), &handle_menu);
  properties_action=new QAction(QIcon(":icones/icones/editar.png"), trUtf8("Properties"), &handle_menu);

  refresh_action=new QAction(QIcon(":icones/icones/atualizar.png"), trUtf8("Update"), &handle_menu);
  refresh_action->setShortcut(QKeySequence(Qt::Key_F5));
  objects_trw->installEventFilter(this);

  connect(refresh_tb, SIGNAL(clicked(void)), this, SLOT(listObjects(void)));
  connect(expand_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(expandAll(void)));
  connect(collapse_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(collapseAll(void)));
  connect(ext_objs_chk, SIGNAL(toggled(bool)), this, SLOT(listObjects(void)));
  connect(sys_objs_chk, SIGNAL(toggled(bool)), this, SLOT(listObjects(void)));
  connect(objects_trw, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(handleObject(QTreeWidgetItem *,int)));

  connect(properties_tbw, &QTableWidget::itemPressed,
          [=]() { SQLToolWidget::copySelection(properties_tbw, true); });

  connect(filter_edt, &QLineEdit::textChanged,
          [=](){ DatabaseImportForm::filterObjects(objects_trw, filter_edt->text(),
                                                   (by_oid_chk->isChecked() ? DatabaseImportForm::OBJECT_ID : 0)); });
}

bool DatabaseExplorerWidget::eventFilter(QObject *object, QEvent *event)
{
  if(object==objects_trw && event->type()==QEvent::KeyPress)
  {
    QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

    if(k_event->key()==Qt::Key_Delete || k_event->key()==Qt::Key_F5)
    {
     if(k_event->key()==Qt::Key_F5)
        updateCurrentItem();
     else
       dropObject(objects_trw->currentItem(), k_event->modifiers()==Qt::ShiftModifier);
     return(true);
    }
    else
      return(false);
  }

  return(QWidget::eventFilter(object, event));
}

void DatabaseExplorerWidget::formatObjectAttribs(attribs_map &attribs)
{
  ObjectType obj_type=BASE_OBJECT;
  attribs_map fmt_attribs;
  QString attr_name, attr_value;
  QRegExp oid_regexp=QRegExp("^[0-9]+");
  map<QString, ObjectType> dep_types={{ParsersAttributes::OWNER, OBJ_ROLE},
                                      {ParsersAttributes::SCHEMA, OBJ_SCHEMA},
                                      {ParsersAttributes::TABLESPACE, OBJ_TABLESPACE},
                                      {ParsersAttributes::COLLATION, OBJ_COLLATION}};

  if(attribs.count(ParsersAttributes::OBJECT_TYPE)!=0)
    obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());

  try
  {
    switch(obj_type)
    {
      case OBJ_CAST:
        formatCastAttribs(attribs);
      break;

      case OBJ_EVENT_TRIGGER:
        formatEventTriggerAttribs(attribs);
      break;

      case OBJ_LANGUAGE:
        formatLanguageAttribs(attribs);
      break;

      case OBJ_ROLE:
        formatRoleAttribs(attribs);
      break;

      case OBJ_AGGREGATE:
        formatAggregateAttribs(attribs);
      break;

      case OBJ_CONVERSION:
        formatConversionAttribs(attribs);
      break;

      default:
        qDebug("format method for %s isn't implemented!", BaseObject::getSchemaName(obj_type).toStdString().c_str());
      break;
    }
  }
  catch(Exception &e)
  {
    Messagebox msg_box;
    msg_box.show(e);
  }

  if(attribs.count(ParsersAttributes::PERMISSION)!=0)
    attribs[ParsersAttributes::PERMISSION]=Catalog::parseArrayValues(attribs[ParsersAttributes::PERMISSION]).join(",");

  if(attribs[ParsersAttributes::NAME].startsWith("pg_catalog."))
    attribs[ParsersAttributes::NAME].remove("pg_catalog.");

  for(auto attrib : attribs)
  {
    attr_name=attrib.first;
    attr_value=attrib.second;

    if(attr_name==ParsersAttributes::OBJECT_TYPE)
     attr_value=BaseObject::getTypeName(static_cast<ObjectType>(attr_value.toUInt()));
    else if(dep_types.count(attr_name)!=0 && oid_regexp.exactMatch(attr_value))
     attr_value=getObjectName(dep_types[attr_name], attr_value.toUInt());

    fmt_attribs[attribs_i18n.at(attr_name)]=attr_value;
  }

  attribs=fmt_attribs;
}

void DatabaseExplorerWidget::formatCastAttribs(attribs_map &attribs)
{
  QStringList type_attrs={ ParsersAttributes::DEST_TYPE, ParsersAttributes::SOURCE_TYPE };

  attribs[ParsersAttributes::IO_CAST]=(attribs[ParsersAttributes::IO_CAST].isEmpty() ?
                                       attribs_i18n.at(ParsersAttributes::_FALSE_) :
                                       attribs_i18n.at(ParsersAttributes::_TRUE_));
  for(QString attr : type_attrs)
   attribs[attr]=getObjectName(OBJ_TYPE, attribs[attr].toUInt());

  attribs[ParsersAttributes::FUNCTION]=getObjectName(OBJ_FUNCTION, attribs[ParsersAttributes::FUNCTION].toUInt());
}

void DatabaseExplorerWidget::formatEventTriggerAttribs(attribs_map &attribs)
{
  attribs[ParsersAttributes::VALUES]=Catalog::parseArrayValues(attribs[ParsersAttributes::VALUES]).join(',');
  attribs[ParsersAttributes::FUNCTION]=getObjectName(OBJ_FUNCTION, attribs[ParsersAttributes::FUNCTION].toUInt());
}

void DatabaseExplorerWidget::formatAggregateAttribs(attribs_map &attribs)
{
  QStringList func_attrs={ ParsersAttributes::FINAL_FUNC, ParsersAttributes::TRANSITION_FUNC },
              types;

  for(QString attr : func_attrs)
   attribs[attr]=getObjectName(OBJ_FUNCTION, attribs[attr].toUInt());

  attribs[ParsersAttributes::STATE_TYPE]=getObjectName(OBJ_TYPE, attribs[ParsersAttributes::STATE_TYPE].toUInt());
  attribs[ParsersAttributes::SORT_OP]=getObjectName(OBJ_OPERATOR, attribs[ParsersAttributes::SORT_OP].toUInt());

  types=Catalog::parseArrayValues(attribs[ParsersAttributes::TYPES]);
  for(int idx=0; idx < types.size(); idx++)
    types[idx]=getObjectName(OBJ_TYPE, types[idx].toUInt());

  attribs[ParsersAttributes::TYPES]=types.join(",");
  attribs[ParsersAttributes::INITIAL_COND]=Catalog::parseArrayValues(attribs[ParsersAttributes::INITIAL_COND]).join(",");
}

void DatabaseExplorerWidget::formatLanguageAttribs(attribs_map &attribs)
{
  QStringList func_attribs={ ParsersAttributes::VALIDATOR_FUNC,
                             ParsersAttributes::HANDLER_FUNC,
                             ParsersAttributes::INLINE_FUNC };

  attribs[ParsersAttributes::TRUSTED]=(attribs[ParsersAttributes::TRUSTED].isEmpty() ?
                                       attribs_i18n.at(ParsersAttributes::_FALSE_) :
                                       attribs_i18n.at(ParsersAttributes::_TRUE_));

  for(QString attr : func_attribs)
    attribs[attr]=getObjectName(OBJ_FUNCTION, attribs[attr].toUInt());
}

void DatabaseExplorerWidget::formatRoleAttribs(attribs_map &attribs)
{
  QStringList role_attribs={ ParsersAttributes::SUPERUSER, ParsersAttributes::INHERIT,
                             ParsersAttributes::CREATEROLE, ParsersAttributes::CREATEDB,
                             ParsersAttributes::LOGIN, ParsersAttributes::ENCRYPTED,
                             ParsersAttributes::REPLICATION },

              members_attribs={ ParsersAttributes::ADMIN_ROLES,
                                ParsersAttributes::MEMBER_ROLES,
                                ParsersAttributes::REF_ROLES },
              roles;

  for(QString attr : role_attribs)
    attribs[attr]=(attribs[attr].isEmpty() ?
                   attribs_i18n.at(ParsersAttributes::_FALSE_) :
                   attribs_i18n.at(ParsersAttributes::_TRUE_));

  for(QString attr : members_attribs)
  {
    roles=Catalog::parseArrayValues(attribs[attr]);

    for(int idx=0; idx < roles.size(); idx++)
      roles[idx]=getObjectName(OBJ_ROLE, roles[idx].toUInt());

    attribs[attr]=roles.join(",");
  }
}

void DatabaseExplorerWidget::formatConversionAttribs(attribs_map &attribs)
{
  attribs[ParsersAttributes::DEFAULT]=(attribs[ParsersAttributes::DEFAULT].isEmpty() ?
                                       attribs_i18n.at(ParsersAttributes::_FALSE_) :
                                       attribs_i18n.at(ParsersAttributes::_TRUE_));
  attribs[ParsersAttributes::FUNCTION]=getObjectName(OBJ_FUNCTION, attribs[ParsersAttributes::FUNCTION].toUInt());
}

void DatabaseExplorerWidget::formatTriggerAttribs(attribs_map &attribs)
{
  attribs[ParsersAttributes::TRIGGER_FUNC]=getObjectName(OBJ_FUNCTION, attribs[ParsersAttributes::TRIGGER_FUNC].toUInt());
}

QString DatabaseExplorerWidget::getObjectName(ObjectType obj_type, unsigned oid)
{
  try
  {
    if(oid==0)
      return(DEP_NOT_DEFINED);
    else
    {
      attribs_map attribs, aux_attribs;
      QString obj_name=DEP_NOT_FOUND, sch_name;

      attribs=catalog.getObjectAttributes(obj_type, oid);

      if(!attribs.empty())
      {
        obj_name=attribs[ParsersAttributes::NAME];

        if(!attribs[ParsersAttributes::SCHEMA].isEmpty() &&
           attribs[ParsersAttributes::SCHEMA]!="0")
        {
          aux_attribs=catalog.getObjectAttributes(obj_type, attribs[ParsersAttributes::SCHEMA].toUInt());
          sch_name=aux_attribs[ParsersAttributes::NAME];

          if(!sch_name.isEmpty() && sch_name!="pg_catalog" && sch_name!="information_schema")
            obj_name=sch_name + "." + obj_name;
        }
      }

      if(obj_type==OBJ_FUNCTION)
      {
        QStringList arg_types=Catalog::parseArrayValues(attribs[ParsersAttributes::ARG_TYPES]);

        for(int idx=0; idx < arg_types.size(); idx++)
         arg_types[idx]=getObjectName(OBJ_TYPE, arg_types[idx].toUInt());

        obj_name+=QString("(%1)").arg(arg_types.join(','));
      }
      else if(obj_type==OBJ_OPERATOR)
      {
        QStringList arg_types;
        QString type_name;
        vector<QString> attrib_ids={ ParsersAttributes::LEFT_TYPE, ParsersAttributes::RIGHT_TYPE };

        for(QString attr : attrib_ids)
        {
          type_name=getObjectName(OBJ_TYPE, attribs[attr].toUInt());
          if(type_name.isEmpty()) type_name="-";
          arg_types.push_back(type_name);
        }

        obj_name+=QString("(%1)").arg(arg_types.join(','));
      }

      return(obj_name);
    }
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void DatabaseExplorerWidget::setConnection(Connection conn)
{
  this->connection=conn;
}

Connection DatabaseExplorerWidget::getConnection(void)
{
  return(connection);
}

void DatabaseExplorerWidget::listObjects(void)
{
  try
  {
    configureImportHelper();
    DatabaseImportForm::listObjects(import_helper, objects_trw, false, false, true);
    import_helper.closeConnection();
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void DatabaseExplorerWidget::configureImportHelper(void)
{
  import_helper.setConnection(connection);
  import_helper.setCurrentDatabase(connection.getConnectionParam(Connection::PARAM_DB_NAME));
  import_helper.setImportOptions(sys_objs_chk->isChecked(), ext_objs_chk->isChecked(), false, false, false, false);

  catalog.closeConnection();
  catalog.setFilter(Catalog::LIST_ALL_OBJS);
  catalog.setConnection(connection);
}

void DatabaseExplorerWidget::handleObject(QTreeWidgetItem *item, int)
{
  if(QApplication::mouseButtons()==Qt::RightButton)
  {
    ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
    unsigned obj_id=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();

    for(auto act : handle_menu.actions())
      handle_menu.removeAction(act);

    if(obj_id > 0 && obj_type!=OBJ_DATABASE)
    {
      if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW)
        handle_menu.addAction(show_data_action);

      handle_menu.addAction(drop_action);
      handle_menu.addAction(drop_cascade_action);
    }

    handle_menu.addAction(refresh_action);

    if(obj_id > 0)
      handle_menu.addAction(properties_action);

    QAction *exec_action=handle_menu.exec(QCursor::pos());

    if(exec_action==drop_action || exec_action==drop_cascade_action)
      dropObject(item,  exec_action==drop_cascade_action);
    else if(exec_action==refresh_action)
      updateCurrentItem();
    else if(exec_action==properties_action)
      loadObjectProperties();
    else if(exec_action==show_data_action)
      emit s_dataGridOpenRequested(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString(),
                                   item->text(0),
                                   item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt()!=OBJ_VIEW);
  }
  else if(QApplication::mouseButtons()==Qt::LeftButton)
  {
    loadObjectProperties();
  }
}

void DatabaseExplorerWidget::dropObject(QTreeWidgetItem *item, bool cascade)
{
  Messagebox msg_box;

  try
  {
    if(item && static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt()) > 0)
    {
      ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
      QString msg;

      if(!cascade)
        msg=trUtf8("Do you really want to drop the object <strong>%1</strong> <em>(%2)</em>?")
            .arg(item->text(0)).arg(BaseObject::getTypeName(obj_type));
      else
        msg=trUtf8("Do you really want to <strong>cascade</strong> drop the object <strong>%1</strong> <em>(%2)</em>? This action will drop all the other objects that depends on it?")
            .arg(item->text(0)).arg(BaseObject::getTypeName(obj_type));

      msg_box.show(trUtf8("Confirmation"), msg, Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

      if(msg_box.result()==QDialog::Accepted)
      {
        QTreeWidgetItem *parent=nullptr;
        attribs_map attribs;
        QStringList types;
        QString drop_cmd, obj_name=item->text(0);
        int idx=0, idx1=0;
        Connection conn;

        attribs[ParsersAttributes::SQL_OBJECT]=BaseObject::getSQLName(obj_type);
        attribs[ParsersAttributes::DECL_IN_TABLE]="";
        attribs[BaseObject::getSchemaName(obj_type)]="1";

        //For cast, operator and function is needed to extract the name and the params types
        if(obj_type==OBJ_OPERATOR || obj_type==OBJ_FUNCTION || obj_type==OBJ_CAST)
        {
          idx=obj_name.indexOf('(');
          idx1=obj_name.indexOf(')');
          types=obj_name.mid(idx+1, idx1-idx-1).split(',');
          types.removeAll("-");
          obj_name.remove(idx, obj_name.size());
        }

        //Formatting the names
        attribs[ParsersAttributes::NAME]=BaseObject::formatName(obj_name, obj_type==OBJ_OPERATOR);
        attribs[ParsersAttributes::TABLE]=BaseObject::formatName(item->data(DatabaseImportForm::OBJECT_TABLE, Qt::UserRole).toString());
        attribs[ParsersAttributes::SCHEMA]=BaseObject::formatName(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString());

        //For table objects the "table" attribute must be schema qualified
        if(obj_type!=OBJ_INDEX && TableObject::isTableObject(obj_type))
          attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + "." + attribs[ParsersAttributes::TABLE];
        //For operators and functions there must exist the signature attribute
        else if(obj_type==OBJ_OPERATOR || obj_type==OBJ_FUNCTION)
          attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + "." + attribs[ParsersAttributes::NAME] + QString("(%1)").arg(types.join(','));
        else if(obj_type==OBJ_CAST)
          attribs[ParsersAttributes::SIGNATURE]=QString("(%1 AS %2)").arg(types[0]).arg(types[1]);
        else
        {
          if(!attribs[ParsersAttributes::SCHEMA].isEmpty() &&
             attribs[ParsersAttributes::NAME].indexOf(attribs[ParsersAttributes::SCHEMA] + ".") < 0)
            attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + "." + attribs[ParsersAttributes::NAME];
          else
            attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::NAME];
        }

        //Generate the drop command
        schparser.setIgnoreEmptyAttributes(true);
        schparser.setIgnoreUnkownAttributes(true);
        drop_cmd=schparser.getCodeDefinition(ParsersAttributes::DROP, attribs, SchemaParser::SQL_DEFINITION);

        if(cascade)
          drop_cmd.replace(";"," CASCADE;");

        //Executes the drop cmd
        conn=connection;
        conn.connect();
        conn.executeDDLCommand(drop_cmd);

        //Updates the object count on the parent item
        parent=item->parent();
        if(parent && parent->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt()==0)
        {
          unsigned cnt=parent->data(DatabaseImportForm::OBJECT_COUNT, Qt::UserRole).toUInt();
          ObjectType parent_type=static_cast<ObjectType>(parent->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());

          cnt--;
          parent->setText(0, BaseObject::getTypeName(parent_type) + QString(" (%1)").arg(cnt));
          parent->setData(DatabaseImportForm::OBJECT_COUNT, Qt::UserRole, QVariant::fromValue<unsigned>(cnt));
        }

        if(parent)
          parent->takeChild(parent->indexOfChild(item));
        else
          objects_trw->takeTopLevelItem(objects_trw->indexOfTopLevelItem(item));

      }
    }
  }
  catch(Exception &e)
  {
    msg_box.show(e);
  }
}

void DatabaseExplorerWidget::updateCurrentItem(void)
{
  QTreeWidgetItem *item=objects_trw->currentItem();

  if(item)
  {
    QTreeWidgetItem *root=nullptr, *parent=nullptr;
    ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
    unsigned obj_id=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt());
    QString sch_name, tab_name;
    vector<QTreeWidgetItem *> gen_items, gen_items1;

    if(obj_type==OBJ_DATABASE)
      listObjects();
    else
    {
      parent=item->parent();
      sch_name=item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString();
      tab_name=item->data(DatabaseImportForm::OBJECT_TABLE, Qt::UserRole).toString();

      if(parent)
      {
        if(obj_id==0)
        {
          root=parent;
          parent->takeChild(parent->indexOfChild(item));
        }
        else
        {
          if(obj_type==OBJ_SCHEMA || obj_type==OBJ_TABLE)
          {
            root=item;
            root->takeChildren();

            if(obj_type==OBJ_TABLE)
              tab_name=item->text(0);
            else
              sch_name=item->text(0);
          }
          else
          {
            root=parent->parent();
            root->takeChild(root->indexOfChild(parent));
          }
        }
      }

      configureImportHelper();

      //Updates the group type only
      if(obj_id==0 || (obj_type!=OBJ_TABLE && obj_type!=OBJ_SCHEMA))
        gen_items=DatabaseImportForm::updateObjectsTree(import_helper, objects_trw, { obj_type }, false, false, root, sch_name, tab_name);
      else
        //Updates all child objcts when the selected object is a schema or table
        gen_items=DatabaseImportForm::updateObjectsTree(import_helper, objects_trw,
                                                        BaseObject::getChildObjectTypes(obj_type), false, false, root, sch_name, tab_name);

      //Updating the subtree for schemas / tables
      if(obj_type==OBJ_SCHEMA || obj_type==OBJ_TABLE)
      {
        for(auto item : gen_items)
        {
          //When the user refresh a single schema or table
          if(obj_id > 0 || obj_type==OBJ_TABLE)
          {
            //Updates the table subtree
            DatabaseImportForm::updateObjectsTree(import_helper, objects_trw,
                                                  BaseObject::getChildObjectTypes(OBJ_TABLE),
                                                  false, false, item,
                                                  item->parent()->data(DatabaseImportForm::OBJECT_SCHEMA,Qt::UserRole).toString(),
                                                  item->text(0));
          }
          //When the user refresh the schema group
          else
          {
            //Updates the entire schema subtree
            gen_items1= DatabaseImportForm::updateObjectsTree(import_helper, objects_trw,
                                                              BaseObject::getChildObjectTypes(OBJ_SCHEMA),
                                                              false, false, item, item->text(0));

            //Updates the table group for the current schema
            for(auto item1 : gen_items1)
            {
              DatabaseImportForm::updateObjectsTree(import_helper, objects_trw,
                                                    BaseObject::getChildObjectTypes(OBJ_TABLE),
                                                    false, false, item1,
                                                    item1->parent()->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString(),
                                                    item1->text(0));
            }
          }
        }
      }

      import_helper.closeConnection();
      objects_trw->sortItems(0, Qt::AscendingOrder);
    }
  }
}

void DatabaseExplorerWidget::loadObjectProperties(void)
{
  try
  {
    QTreeWidgetItem *item=objects_trw->currentItem();
    unsigned oid=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();

    while(properties_tbw->rowCount() > 0)
      properties_tbw->removeRow(0);

    if(oid != 0)
    {
      ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
      attribs_map cached_attribs;
      QTableWidgetItem *tab_item=nullptr;
      int row=0;
      QFont font;

      cached_attribs=item->data(DatabaseImportForm::OBJECT_ATTRIBS, Qt::UserRole).value<attribs_map>();

      if(cached_attribs.empty())
      {
        cached_attribs=catalog.getObjectAttributes(obj_type, oid);
        formatObjectAttribs(cached_attribs);
        item->setData(DatabaseImportForm::OBJECT_ATTRIBS, Qt::UserRole, QVariant::fromValue<attribs_map>(cached_attribs));
      }

      properties_tbw->setSortingEnabled(false);

      if(!cached_attribs.empty())
      {
        for(auto attrib : cached_attribs)
        {
          properties_tbw->insertRow(properties_tbw->rowCount());
          row=properties_tbw->rowCount() - 1;

          tab_item=new QTableWidgetItem;
          font=tab_item->font();
          font.setItalic(true);
          tab_item->setText(attrib.first);
          tab_item->setFont(font);
          tab_item->setIcon(QPixmap(":/icones/icones/attribute.png"));
          properties_tbw->setItem(row, 0, tab_item);

          tab_item=new QTableWidgetItem;
          tab_item->setText(attrib.second);
          properties_tbw->setItem(row, 1, tab_item);
        }
      }

      properties_tbw->setSortingEnabled(true);
    }

    properties_tbw->horizontalHeader()->setVisible(properties_tbw->rowCount() > 0);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}
