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

#include "databaseexplorerwidget.h"
#include "databaseimportform.h"
#include "sqltoolwidget.h"
#include "sqlexecutionwidget.h"
#include "snippetsconfigwidget.h"

using namespace ParsersAttributes;

const QString DatabaseExplorerWidget::DEP_NOT_DEFINED=QString();
const QString DatabaseExplorerWidget::DEP_NOT_FOUND=QT_TR_NOOP("(not found, OID: %1)");
const QString DatabaseExplorerWidget::ELEM_SEPARATOR=QString("•");

const attribs_map DatabaseExplorerWidget::attribs_i18n {
  {ADMIN_ROLES, QT_TR_NOOP("Admin. roles")},           {ALIGNMENT, QT_TR_NOOP("Alignment")},                  {ANALYZE_FUNC, QT_TR_NOOP("Analyze func.")},
  {ARG_COUNT, QT_TR_NOOP("Arg. count")},               {ARG_DEF_COUNT, QT_TR_NOOP("Arg. default count")},     {ARG_DEFAULTS, QT_TR_NOOP("Arg. defaults")},
  {ARG_MODES, QT_TR_NOOP("Arg. modes")},               {ARG_NAMES, QT_TR_NOOP("Arg. names")},                 {ARG_TYPES, QT_TR_NOOP("Arg. types")},
  {ATTRIBUTE, QT_TR_NOOP("Attribute")},                {BEHAVIOR_TYPE, QT_TR_NOOP("Behavior type")},          {BY_VALUE, QT_TR_NOOP("By value")},
  {CAST_TYPE, QT_TR_NOOP("Cast type")},                {CATEGORY, QT_TR_NOOP("Category")},                    {COLLATABLE, QT_TR_NOOP("Collatable")},
  {COLLATION, QT_TR_NOOP("Collation")},                {COMMENT, QT_TR_NOOP("Comment")},                      {COMMUTATOR_OP, QT_TR_NOOP("Commutator Op.")},
  {CONFIGURATION, QT_TR_NOOP("Configuration")},        {CONN_LIMIT, QT_TR_NOOP("Conn. limit")},               {CONSTRAINT, QT_TR_NOOP("Constraint")},
  {CREATEDB, QT_TR_NOOP("Create DB")},                 {CREATEROLE, QT_TR_NOOP("Create role")},               {CUR_VERSION, QT_TR_NOOP("Curr. version")},
  {DEFAULT, QT_TR_NOOP("Default")},                    {DEFAULT_VALUE, QT_TR_NOOP("Default value")},          {DEFINITION, QT_TR_NOOP("Definition")},
  {DELIMITER, QT_TR_NOOP("Delimiter")},                {DEST_TYPE, QT_TR_NOOP("Dest. type")},                 {DIMENSION, QT_TR_NOOP("Dimension")},
  {DIRECTORY, QT_TR_NOOP("Directory")},                {DST_ENCODING, QT_TR_NOOP("Dest. encoding")},          {ELEMENT, QT_TR_NOOP("Element")},
  {ENCODING, QT_TR_NOOP("Encoding")},                  {ENCRYPTED, QT_TR_NOOP("Encrypted")},                  {ENUMERATIONS, QT_TR_NOOP("Enumerations")},
  {EXECUTION_COST, QT_TR_NOOP("Exec. cost")},          {EXPRESSION, QT_TR_NOOP("Expression")},                {FAMILY, QT_TR_NOOP("Op. family")},
  {FINAL_FUNC, QT_TR_NOOP("Final func.")},             {FUNCTION, QT_TR_NOOP("Function")},                    {FUNCTION_TYPE, QT_TR_NOOP("Func. type")},
  {HANDLER_FUNC, QT_TR_NOOP("Handler func.")},         {HANDLES_TYPE, QT_TR_NOOP("Handles type")},            {HASHES, QT_TR_NOOP("Hashes")},
  {INDEX_TYPE, QT_TR_NOOP("Index type")},              {INHERIT, QT_TR_NOOP("Inherit")},                      {INITIAL_COND, QT_TR_NOOP("Ini. condition")},
  {INLINE_FUNC, QT_TR_NOOP("Inline func.")},           {INPUT_FUNC, QT_TR_NOOP("Input func.")},               {INTERNAL_LENGTH, QT_TR_NOOP("Internal length")},
  {INTERVAL_TYPE, QT_TR_NOOP("Interval type")},        {IO_CAST, QT_TR_NOOP("I/O cast")},                     {JOIN_FUNC, QT_TR_NOOP("Join func.")},
  {LANGUAGE, QT_TR_NOOP("Language")},                  {_LC_COLLATE_, QT_TR_NOOP("LC COLLATE")},              {_LC_CTYPE_, QT_TR_NOOP("LC CTYPE")},
  {LEAKPROOF, QT_TR_NOOP("Leak proof")},               {LEFT_TYPE, QT_TR_NOOP("Left type")},                  {LENGTH, QT_TR_NOOP("Length")},
  {LIBRARY, QT_TR_NOOP("Library")},                    {LOGIN, QT_TR_NOOP("Can login")},                      {MATERIALIZED, QT_TR_NOOP("Materialized")},
  {MEMBER_ROLES, QT_TR_NOOP("Member roles")},          {MERGES, QT_TR_NOOP("Merges")},                        {NAME, QT_TR_NOOP("Name")},
  {NEGATOR_OP, QT_TR_NOOP("Negator op.")},             {NOT_NULL, QT_TR_NOOP("Not null")},                    {OBJECT_TYPE, QT_TR_NOOP("Object type")},
  {OID, QT_TR_NOOP("OID")},                            {OIDS, QT_TR_NOOP("With OIDs")},                       {OLD_VERSION, QT_TR_NOOP("Old version")},
  {OPERATOR, QT_TR_NOOP("Operator")},                  {OPERATOR_FUNC, QT_TR_NOOP("Operator func.")},         {OUTPUT_FUNC, QT_TR_NOOP("Output func.")},
  {OWNER, QT_TR_NOOP("Owner")},                        {OWNER_COLUMN, QT_TR_NOOP("Owner column")},            {PARENTS, QT_TR_NOOP("Parents")},
  {PASSWORD, QT_TR_NOOP("Password")},                  {PERMISSION, QT_TR_NOOP("Permissions")},               {PRECISION, QT_TR_NOOP("Precision")},
  {PREFERRED, QT_TR_NOOP("Preferred")},                {RANGE_ATTRIBS, QT_TR_NOOP("Range attributes")},       {RECV_FUNC, QT_TR_NOOP("Receive func.")},
  {REF_ROLES, QT_TR_NOOP("Ref. roles")},               {REPLICATION, QT_TR_NOOP("Replication")},              {RESTRICTION_FUNC, QT_TR_NOOP("Restriction func.")},
  {RETURN_TYPE, QT_TR_NOOP("Return type")},            {RETURNS_SETOF, QT_TR_NOOP("Returns SETOF")},          {RIGHT_TYPE, QT_TR_NOOP("Right type")},
  {ROW_AMOUNT, QT_TR_NOOP("Rows amount")},              {SCHEMA, QT_TR_NOOP("Schema")},                        {SECURITY_TYPE, QT_TR_NOOP("Security type")},
  {SEND_FUNC, QT_TR_NOOP("Send func.")},               {SORT_OP, QT_TR_NOOP("Sort op.")},                     {SOURCE_TYPE, QT_TR_NOOP("Source type")},
  {SRC_ENCODING, QT_TR_NOOP("Src. encoding")},         {STATE_TYPE, QT_TR_NOOP("State type")},                {STORAGE, QT_TR_NOOP("Storage")},
  {SUPERUSER, QT_TR_NOOP("Superuser")},                {TABLESPACE, QT_TR_NOOP("Tablespace")},                {TPMOD_IN_FUNC, QT_TR_NOOP("Type mod. in func.")},
  {TPMOD_OUT_FUNC, QT_TR_NOOP("Type mod. out func.")}, {TRANSITION_FUNC, QT_TR_NOOP("Transition func.")},     {TRUSTED, QT_TR_NOOP("Trusted")},
  {TYPE, QT_TR_NOOP("Type")},                          {TYPE_ATTRIBUTE, QT_TR_NOOP("Type attribute")},        {TYPES, QT_TR_NOOP("Types")},
  {UNLOGGED, QT_TR_NOOP("Unlogged")},                  {VALIDATOR, QT_TR_NOOP("Validator func.")},            {VALIDITY, QT_TR_NOOP("Validity")},
  {WINDOW_FUNC, QT_TR_NOOP("Windows func.")},          {_FALSE_, QT_TR_NOOP("false")},                        {_TRUE_, QT_TR_NOOP("true")},
  {CACHE, QT_TR_NOOP("Cache value")},                  {CYCLE, QT_TR_NOOP("Cycle")},                          {INCREMENT, QT_TR_NOOP("Increment")},
  {MAX_VALUE, QT_TR_NOOP("Max. value")},               {MIN_VALUE, QT_TR_NOOP("Min. value")},                 {START, QT_TR_NOOP("Start value")},
  {LAST_VALUE, QT_TR_NOOP("Last value")},              {SUBTYPE, QT_TR_NOOP("Subtype")},                      {OP_CLASS, QT_TR_NOOP("Op. class")},
  {CANONICAL_FUNC, QT_TR_NOOP("Canonical func.")},     {SUBTYPE_DIFF_FUNC, QT_TR_NOOP("Subtype diff func.")}, {CONSTRAINT, QT_TR_NOOP("Constraint")},
  {DEFERRABLE, QT_TR_NOOP("Deferrable")},              {PER_ROW, QT_TR_NOOP("For each row")},                 {FIRING_TYPE, QT_TR_NOOP("Firing")},
  {INS_EVENT, QT_TR_NOOP("On insert")},                {DEL_EVENT, QT_TR_NOOP("On delete")},                  {UPD_EVENT, QT_TR_NOOP("On update")},
  {TRUNC_EVENT, QT_TR_NOOP("On truncate")},            {ARGUMENTS, QT_TR_NOOP("Arguments")},                  {TABLE, QT_TR_NOOP("Table")},
  {TRIGGER_FUNC, QT_TR_NOOP("Trigger func.")},         {COLUMNS, QT_TR_NOOP("Columns")},                      {CONDITION, QT_TR_NOOP("Condition")},
  {DEFER_TYPE, QT_TR_NOOP("Deferment")},               {EVENT_TYPE, QT_TR_NOOP("Event")},                     {EXEC_TYPE, QT_TR_NOOP("Execution mode")},
  {COMMANDS, QT_TR_NOOP("Commands")},                  {POSITION, QT_TR_NOOP("Position")},                    {COMPARISON_TYPE, QT_TR_NOOP("Comparison type")},
  {DEL_ACTION, QT_TR_NOOP("On delete")},               {DST_COLUMNS, QT_TR_NOOP("Ref. columns")},             {EXPRESSIONS, QT_TR_NOOP("Expressions")},
  {FACTOR, QT_TR_NOOP("Fill factor")},                 {NO_INHERIT, QT_TR_NOOP("No inherit")},                {OP_CLASSES, QT_TR_NOOP("Op. classes")},
  {OPERATORS, QT_TR_NOOP("Operators")},                {REF_TABLE, QT_TR_NOOP("Ref. table")},                 {COLUMNS, QT_TR_NOOP("Columns")},
  {UPD_ACTION, QT_TR_NOOP("On update")},               {SRC_COLUMNS, QT_TR_NOOP("Columns")},                  {UNIQUE, QT_TR_NOOP("Unique")},
  {PREDICATE, QT_TR_NOOP("Predicate")},                {COLLATIONS, QT_TR_NOOP("Collations")},                {INHERITED, QT_TR_NOOP("Inherited")}
};

DatabaseExplorerWidget::DatabaseExplorerWidget(QWidget *parent): QWidget(parent)
{
  setupUi(this);

  snippets_menu.setTitle(trUtf8("Snippets"));
  snippets_menu.setIcon(QIcon(QString(":icones/icones/codesnippet.png")));

  drop_action=new QAction(QIcon(QString(":icones/icones/excluir.png")), trUtf8("Drop object"), &handle_menu);
  drop_action->setShortcut(QKeySequence(Qt::Key_Delete));

  drop_cascade_action=new QAction(QIcon(QString(":icones/icones/delcascade.png")), trUtf8("Drop cascade"), &handle_menu);
  drop_cascade_action->setShortcut(QKeySequence("Shift+Del"));

  truncate_action=new QAction(QIcon(QString(":icones/icones/truncate.png")), trUtf8("Truncate"), &handle_menu);
  trunc_cascade_action=new QAction(QIcon(QString(":icones/icones/trunccascade.png")), trUtf8("Trunc. cascade"), &handle_menu);

  show_data_action=new QAction(QIcon(QString(":icones/icones/result.png")), trUtf8("Show data"), &handle_menu);
  properties_action=new QAction(QIcon(QString(":icones/icones/editar.png")), trUtf8("Reload properties"), &handle_menu);

  refresh_action=new QAction(QIcon(QString(":icones/icones/atualizar.png")), trUtf8("Update"), &handle_menu);
  refresh_action->setShortcut(QKeySequence(Qt::Key_F5));
  objects_trw->installEventFilter(this);

  connect(refresh_tb, SIGNAL(clicked(void)), this, SLOT(listObjects(void)));
  connect(expand_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(expandAll(void)));
  connect(collapse_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(collapseAll(void)));
  connect(ext_objs_chk, SIGNAL(toggled(bool)), this, SLOT(listObjects(void)));
  connect(sys_objs_chk, SIGNAL(toggled(bool)), this, SLOT(listObjects(void)));
  connect(objects_trw, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(handleObject(QTreeWidgetItem *,int)));
  connect(objects_trw, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(showObjectProperties()));
  connect(raw_attrib_names_chk, SIGNAL(toggled(bool)), this, SLOT(showObjectProperties()));

  connect(data_grid_tb, &QToolButton::clicked,
          [=]() { emit s_dataGridOpenRequested(); });

  connect(runsql_tb, &QToolButton::clicked,
          [=]() { emit s_sqlExecutionRequested(); });

  connect(properties_tbw, &QTableWidget::itemPressed,
          [=]() { SQLExecutionWidget::copySelection(properties_tbw, true); });

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

attribs_map DatabaseExplorerWidget::formatObjectAttribs(attribs_map &attribs)
{
  ObjectType obj_type=BASE_OBJECT;
  attribs_map fmt_attribs;
  QString attr_name, attr_value;
  QRegExp oid_regexp=QRegExp(QString("^[0-9]+"));
  map<QString, ObjectType> dep_types={{ParsersAttributes::OWNER, OBJ_ROLE},
                                      {ParsersAttributes::SCHEMA, OBJ_SCHEMA},
                                      {ParsersAttributes::TABLESPACE, OBJ_TABLESPACE},
                                      {ParsersAttributes::COLLATION, OBJ_COLLATION},
                                      {ParsersAttributes::TABLE, OBJ_TABLE}};

  if(attribs.count(ParsersAttributes::OBJECT_TYPE)!=0)
    obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());

  try
  {
    switch(obj_type)
    {
      case OBJ_CAST: formatCastAttribs(attribs); break;
      case OBJ_EVENT_TRIGGER: formatEventTriggerAttribs(attribs); break;
      case OBJ_LANGUAGE: formatLanguageAttribs(attribs); break;
      case OBJ_ROLE: formatRoleAttribs(attribs); break;
      case OBJ_AGGREGATE: formatAggregateAttribs(attribs); break;
      case OBJ_CONVERSION: formatConversionAttribs(attribs); break;
      case OBJ_DOMAIN: formatDomainAttribs(attribs); break;
      case OBJ_EXTENSION: formatExtensionAttribs(attribs); break;
      case OBJ_FUNCTION: formatFunctionAttribs(attribs); break;
      case OBJ_OPERATOR: formatOperatorAttribs(attribs); break;
      case OBJ_OPCLASS: formatOperatorClassAttribs(attribs); break;
      case OBJ_TABLE: formatTableAttribs(attribs); break;
      case OBJ_SEQUENCE: formatSequenceAttribs(attribs); break;
      case OBJ_TYPE: formatTypeAttribs(attribs); break;
      case OBJ_VIEW: formatViewAttribs(attribs); break;
      case OBJ_TRIGGER: formatTriggerAttribs(attribs); break;
      case OBJ_RULE: formatRuleAttribs(attribs); break;
      case OBJ_COLUMN: formatColumnAttribs(attribs); break;
      case OBJ_CONSTRAINT: formatConstraintAttribs(attribs); break;
      case OBJ_INDEX: formatIndexAttribs(attribs); break;
      default: break;
    }
  }
  catch(Exception &e)
  {
    Messagebox msg_box;
    msg_box.show(e);
  }

  if(attribs.count(ParsersAttributes::PERMISSION)!=0)
    attribs[ParsersAttributes::PERMISSION]=Catalog::parseArrayValues(attribs[ParsersAttributes::PERMISSION]).join(ELEM_SEPARATOR);

  //Removing system schemas from object's name
  if(attribs[ParsersAttributes::NAME].startsWith(QLatin1String("pg_catalog.")) ||
     attribs[ParsersAttributes::NAME].startsWith(QLatin1String("information_schema.")))
    attribs[ParsersAttributes::NAME]=attribs[ParsersAttributes::NAME].split('.').at(1);

  for(auto attrib : attribs)
  {
    attr_name=attrib.first;
    attr_value=attrib.second;

    if(attr_name==ParsersAttributes::OBJECT_TYPE)
     attr_value=BaseObject::getTypeName(static_cast<ObjectType>(attr_value.toUInt()));

    //If the current attribute is related to a dependency object, retreive its real name
    else if(dep_types.count(attr_name)!=0 && oid_regexp.exactMatch(attr_value))
     attr_value=getObjectName(dep_types[attr_name], attr_value);

    attribs[attr_name]=attr_value;

    //Applying translation on the attribute
    if(attribs_i18n.count(attr_name)!=0)
      attr_name=attribs_i18n.at(attr_name);

    fmt_attribs[attr_name]=attr_value;
  }

  attribs[ParsersAttributes::SQL_OBJECT]=BaseObject::getSQLName(obj_type);
  attribs[ParsersAttributes::OBJECT_TYPE]=BaseObject::getSchemaName(obj_type);

  if(attribs.count(ParsersAttributes::SIGNATURE)==0)
    attribs[ParsersAttributes::SIGNATURE]=BaseObject::formatName(attribs[ParsersAttributes::NAME]);

  if(attribs.count(ParsersAttributes::SCHEMA)!=0)
    attribs[ParsersAttributes::SIGNATURE]=QString("%1.%2")
                                          .arg(BaseObject::formatName(attribs[ParsersAttributes::SCHEMA]))
                                          .arg(attribs[ParsersAttributes::SIGNATURE]);

  return(fmt_attribs);
}

void DatabaseExplorerWidget::formatBooleanAttribs(attribs_map &attribs, QStringList bool_attrs)
{
  for(QString attr : bool_attrs)
    attribs[attr]=(attribs[attr].isEmpty() ?
                   attribs_i18n.at(ParsersAttributes::_FALSE_) :
                   attribs_i18n.at(ParsersAttributes::_TRUE_));
}

void DatabaseExplorerWidget::formatOidAttribs(attribs_map &attribs, QStringList oid_attrs, ObjectType obj_type, bool is_oid_array)
{
  try
  {
    if(!is_oid_array)
    {
      for(QString attr : oid_attrs)
        attribs[attr]=getObjectName(obj_type, attribs[attr]);
    }
    else
    {
      QStringList names;

      for(QString attr : oid_attrs)
      {
        names=getObjectsNames(obj_type, Catalog::parseArrayValues(attribs[attr]));
        attribs[attr]=names.join(ELEM_SEPARATOR);
      }
    }
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void DatabaseExplorerWidget::formatCastAttribs(attribs_map &attribs)
{  
  formatBooleanAttribs(attribs, { ParsersAttributes::IO_CAST });

  formatOidAttribs(attribs, { ParsersAttributes::DEST_TYPE,
                              ParsersAttributes::SOURCE_TYPE }, OBJ_TYPE, false);

  attribs[ParsersAttributes::FUNCTION]=getObjectName(OBJ_FUNCTION, attribs[ParsersAttributes::FUNCTION]);
}

void DatabaseExplorerWidget::formatEventTriggerAttribs(attribs_map &attribs)
{
  attribs[ParsersAttributes::VALUES]=Catalog::parseArrayValues(attribs[ParsersAttributes::VALUES]).join(ELEM_SEPARATOR);
  attribs[ParsersAttributes::FUNCTION]=getObjectName(OBJ_FUNCTION, attribs[ParsersAttributes::FUNCTION]);
}

void DatabaseExplorerWidget::formatAggregateAttribs(attribs_map &attribs)
{
  formatOidAttribs(attribs, { ParsersAttributes::FINAL_FUNC,
                              ParsersAttributes::TRANSITION_FUNC }, OBJ_FUNCTION, false);

  formatOidAttribs(attribs, { ParsersAttributes::TYPES }, OBJ_TYPE, true);
  attribs[ParsersAttributes::SIGNATURE]=(QString("%1(%2)")
                                         .arg(BaseObject::formatName(attribs[ParsersAttributes::NAME]))
                                         .arg(attribs[ParsersAttributes::TYPES])).replace(ELEM_SEPARATOR, QString(","));

  attribs[ParsersAttributes::STATE_TYPE]=getObjectName(OBJ_TYPE, attribs[ParsersAttributes::STATE_TYPE]);
  attribs[ParsersAttributes::SORT_OP]=getObjectName(OBJ_OPERATOR, attribs[ParsersAttributes::SORT_OP]);
  attribs[ParsersAttributes::INITIAL_COND]=Catalog::parseArrayValues(attribs[ParsersAttributes::INITIAL_COND]).join(ELEM_SEPARATOR);
}

void DatabaseExplorerWidget::formatLanguageAttribs(attribs_map &attribs)
{
  formatBooleanAttribs(attribs, { ParsersAttributes::TRUSTED });

  formatOidAttribs(attribs, { ParsersAttributes::VALIDATOR_FUNC,
                              ParsersAttributes::HANDLER_FUNC,
                              ParsersAttributes::INLINE_FUNC }, OBJ_FUNCTION, false);
}

void DatabaseExplorerWidget::formatRoleAttribs(attribs_map &attribs)
{
  formatOidAttribs(attribs, { ParsersAttributes::ADMIN_ROLES,
                              ParsersAttributes::MEMBER_ROLES,
                              ParsersAttributes::REF_ROLES }, OBJ_ROLE, true);

  formatBooleanAttribs(attribs, { ParsersAttributes::SUPERUSER, ParsersAttributes::INHERIT,
                                  ParsersAttributes::CREATEROLE, ParsersAttributes::CREATEDB,
                                  ParsersAttributes::LOGIN, ParsersAttributes::ENCRYPTED,
                                  ParsersAttributes::REPLICATION });
}

void DatabaseExplorerWidget::formatConversionAttribs(attribs_map &attribs)
{
  formatBooleanAttribs(attribs, { ParsersAttributes::DEFAULT });
  attribs[ParsersAttributes::FUNCTION]=getObjectName(OBJ_FUNCTION, attribs[ParsersAttributes::FUNCTION]);
}

void DatabaseExplorerWidget::formatDomainAttribs(attribs_map &attribs)
{
  formatBooleanAttribs(attribs, { ParsersAttributes::NOT_NULL });
  attribs[ParsersAttributes::TYPE]=getObjectName(OBJ_TYPE, attribs[ParsersAttributes::TYPE]);
}

void DatabaseExplorerWidget::formatExtensionAttribs(attribs_map &attribs)
{
  formatBooleanAttribs(attribs, { ParsersAttributes::HANDLES_TYPE });
}

void DatabaseExplorerWidget::formatFunctionAttribs(attribs_map &attribs)
{
  attribs[ParsersAttributes::LANGUAGE]=getObjectName(OBJ_LANGUAGE, attribs[ParsersAttributes::LANGUAGE]);
  attribs[ParsersAttributes::RETURN_TYPE]=getObjectName(OBJ_TYPE, attribs[ParsersAttributes::RETURN_TYPE]);
  attribs[ParsersAttributes::ARG_NAMES]=Catalog::parseArrayValues(attribs[ParsersAttributes::ARG_NAMES]).join(ELEM_SEPARATOR);
  attribs[ParsersAttributes::ARG_MODES]=Catalog::parseArrayValues(attribs[ParsersAttributes::ARG_MODES]).join(ELEM_SEPARATOR);
  attribs[ParsersAttributes::ARG_DEFAULTS]=Catalog::parseArrayValues(attribs[ParsersAttributes::ARG_DEFAULTS]).join(ELEM_SEPARATOR);

  formatOidAttribs(attribs, { ParsersAttributes::ARG_TYPES }, OBJ_TYPE, true);
  attribs[ParsersAttributes::SIGNATURE]=(QString("%1(%2)")
                                         .arg(BaseObject::formatName(attribs[ParsersAttributes::NAME]))
                                         .arg(attribs[ParsersAttributes::ARG_TYPES])).replace(ELEM_SEPARATOR, QString(","));

  formatBooleanAttribs(attribs, { ParsersAttributes::WINDOW_FUNC,
                                  ParsersAttributes::LEAKPROOF,
                                  ParsersAttributes::RETURNS_SETOF });
}

void DatabaseExplorerWidget::formatOperatorAttribs(attribs_map &attribs)
{
  formatBooleanAttribs(attribs, { ParsersAttributes::HASHES,
                                  ParsersAttributes::MERGES });

  formatOidAttribs(attribs, { ParsersAttributes::LEFT_TYPE,
                              ParsersAttributes::RIGHT_TYPE}, OBJ_TYPE, false);

  formatOidAttribs(attribs, { ParsersAttributes::COMMUTATOR_OP,
                              ParsersAttributes::NEGATOR_OP}, OBJ_OPERATOR, false);

  formatOidAttribs(attribs, { ParsersAttributes::OPERATOR_FUNC,
                              ParsersAttributes::RESTRICTION_FUNC,
                              ParsersAttributes::JOIN_FUNC }, OBJ_FUNCTION, false);

  attribs[ParsersAttributes::SIGNATURE]=(QString("%1(%2,%3)")
                                         .arg(BaseObject::formatName(attribs[ParsersAttributes::NAME], true))
                                         .arg(attribs[ParsersAttributes::LEFT_TYPE])
                                         .arg(attribs[ParsersAttributes::RIGHT_TYPE])).replace(ELEM_SEPARATOR, QString(","));
}

void DatabaseExplorerWidget::formatTableAttribs(attribs_map &attribs)
{
  formatBooleanAttribs(attribs, { ParsersAttributes::OIDS,
                                  ParsersAttributes::UNLOGGED });

  formatOidAttribs(attribs, { ParsersAttributes::PARENTS }, OBJ_TABLE, true);
}

void DatabaseExplorerWidget::formatSequenceAttribs(attribs_map &attribs)
{
  QStringList owner_col,
              seq_values=Catalog::parseArrayValues(attribs[ParsersAttributes::ATTRIBUTE]),
              seq_attrs={ ParsersAttributes::START, ParsersAttributes::MIN_VALUE,
                          ParsersAttributes::MAX_VALUE, ParsersAttributes::INCREMENT,
                          ParsersAttributes::CACHE, ParsersAttributes::CYCLE };
  QString sch_name=getObjectName(OBJ_SCHEMA, attribs[ParsersAttributes::SCHEMA]);

  attribs.erase(ParsersAttributes::ATTRIBUTE);
  for(int i=0; i < seq_values.size(); i++)
    attribs[seq_attrs[i]]=seq_values[i];

  formatBooleanAttribs(attribs, { ParsersAttributes::CYCLE });

  owner_col=attribs[ParsersAttributes::OWNER_COLUMN].split(':');
  if(owner_col.size()==2)
  {
    QStringList names=getObjectName(OBJ_TABLE, owner_col[0]).split('.');
    vector<attribs_map> col_attribs=catalog.getObjectsAttributes(OBJ_COLUMN, names[0], names[1], { owner_col[1].toUInt() });

    if(!col_attribs.empty())
      attribs[ParsersAttributes::OWNER_COLUMN]=QString("%1.%2.%3").arg(names[0], names[1], col_attribs[0].at(ParsersAttributes::NAME));
  }

  //Retrieving the current value of the sequence by querying the database
  try
  {
    Connection conn=connection;
    ResultSet res;

    conn.connect();
    conn.executeDMLCommand(QString("SELECT last_value FROM %1.%2").arg(sch_name).arg(BaseObject::formatName(attribs[ParsersAttributes::NAME])), res);

    if(res.accessTuple(ResultSet::FIRST_TUPLE))
     attribs[ParsersAttributes::LAST_VALUE]=res.getColumnValue(QString("last_value"));

    conn.close();
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void DatabaseExplorerWidget::formatViewAttribs(attribs_map &attribs)
{
  formatBooleanAttribs(attribs, { ParsersAttributes::MATERIALIZED });
}

void DatabaseExplorerWidget::formatTypeAttribs(attribs_map &attribs)
{
  QStringList range_attr=Catalog::parseArrayValues(attribs[ParsersAttributes::RANGE_ATTRIBS]),
              type_attr=Catalog::parseArrayValues(attribs[ParsersAttributes::TYPE_ATTRIBUTE]);

  formatBooleanAttribs(attribs, { ParsersAttributes::BY_VALUE,
                                  ParsersAttributes::COLLATABLE,
                                  ParsersAttributes::PREFERRED });
  formatOidAttribs(attribs, { ParsersAttributes::ANALYZE_FUNC,
                              ParsersAttributes::INPUT_FUNC,
                              ParsersAttributes::OUTPUT_FUNC,
                              ParsersAttributes::RECV_FUNC,
                              ParsersAttributes::SEND_FUNC,
                              ParsersAttributes::TPMOD_IN_FUNC,
                              ParsersAttributes::TPMOD_OUT_FUNC }, OBJ_FUNCTION, false);
  attribs[ParsersAttributes::ELEMENT]=getObjectName(OBJ_TYPE, attribs[ParsersAttributes::ELEMENT]);

  if(attribs[ParsersAttributes::ENUMERATIONS].isEmpty())
    attribs.erase(ParsersAttributes::ENUMERATIONS);
  else
    attribs[ParsersAttributes::ENUMERATIONS]=Catalog::parseArrayValues(attribs[ParsersAttributes::ENUMERATIONS]).join(ELEM_SEPARATOR);

  attribs.erase(ParsersAttributes::RANGE_ATTRIBS);
  if(!range_attr.isEmpty())
  {
    attribs[ParsersAttributes::SUBTYPE]=getObjectName(OBJ_TYPE, range_attr[0]);
    attribs[ParsersAttributes::COLLATION]=getObjectName(OBJ_COLLATION, range_attr[1]);
    attribs[ParsersAttributes::OP_CLASS]=getObjectName(OBJ_OPCLASS, range_attr[2]);
    attribs[ParsersAttributes::CANONICAL_FUNC]=getObjectName(OBJ_FUNCTION, range_attr[3]);
    attribs[ParsersAttributes::SUBTYPE_DIFF_FUNC]=getObjectName(OBJ_FUNCTION, range_attr[4]);
  }

  if(!type_attr.isEmpty())
  {
    QStringList list, fmt_attribs;

    for(QString attr : type_attr)
    {
      list=attr.split(':');
      list.removeAt(2);
      fmt_attribs.push_back(list.join(QLatin1String(" ")));
    }

    attribs[ParsersAttributes::TYPE_ATTRIBUTE]=fmt_attribs.join(ELEM_SEPARATOR);
  }
  else
    attribs.erase(ParsersAttributes::TYPE_ATTRIBUTE);
}

void DatabaseExplorerWidget::formatOperatorClassAttribs(attribs_map &attribs)
{
  QStringList list, array_vals, elems;

  attribs[ParsersAttributes::FAMILY]=getObjectName(OBJ_OPFAMILY, attribs[ParsersAttributes::FAMILY]);
  formatBooleanAttribs(attribs, { ParsersAttributes::DEFAULT });
  formatOidAttribs(attribs, { ParsersAttributes::STORAGE,
                              ParsersAttributes::TYPE }, OBJ_TYPE, false);

  array_vals=Catalog::parseArrayValues(attribs[ParsersAttributes::FUNCTION]);

  if(!array_vals.isEmpty())
  {
    for(int i=0; i < array_vals.size(); i++)
    {
      list=array_vals[i].split(':');
      elems.push_back(QString("[%1] %2").arg(list[0], getObjectName(OBJ_FUNCTION, list[1])));
    }

    attribs[ParsersAttributes::FUNCTION]=elems.join(ELEM_SEPARATOR);
    elems.clear();
  }

  array_vals=Catalog::parseArrayValues(attribs[ParsersAttributes::OPERATOR]);

  if(!array_vals.isEmpty())
  {
    for(int i=0; i < array_vals.size(); i++)
    {
      list=array_vals[i].split(':');
      elems.push_back(QString("[%1] [%2] [%3]")
                      .arg(list[0],
                      getObjectName(OBJ_OPERATOR, list[1]),
          getObjectName(OBJ_OPERATOR, list[2])));
    }

    attribs[ParsersAttributes::OPERATOR]=elems.join(ELEM_SEPARATOR);
    elems.clear();
  }
}

void DatabaseExplorerWidget::formatTriggerAttribs(attribs_map &attribs)
{
  formatBooleanAttribs(attribs, { ParsersAttributes::CONSTRAINT,
                                  ParsersAttributes::DEFERRABLE,
                                  ParsersAttributes::PER_ROW,
                                  ParsersAttributes::INS_EVENT,
                                  ParsersAttributes::DEL_EVENT,
                                  ParsersAttributes::UPD_EVENT,
                                  ParsersAttributes::TRUNC_EVENT });

  attribs[ParsersAttributes::TRIGGER_FUNC]=getObjectName(OBJ_FUNCTION, attribs[ParsersAttributes::TRIGGER_FUNC]);
  attribs[ParsersAttributes::ARGUMENTS]=Catalog::parseArrayValues(attribs[ParsersAttributes::ARGUMENTS]).join(ELEM_SEPARATOR);
  attribs[ParsersAttributes::COLUMNS]=Catalog::parseArrayValues(attribs[ParsersAttributes::COLUMNS]).join(ELEM_SEPARATOR);
}

void DatabaseExplorerWidget::formatRuleAttribs(attribs_map &attribs)
{
  attribs[ParsersAttributes::COMMANDS]=Catalog::parseRuleCommands(attribs[ParsersAttributes::COMMANDS]).join(';');
}

void DatabaseExplorerWidget::formatColumnAttribs(attribs_map &attribs)
{
  formatBooleanAttribs(attribs, { ParsersAttributes::NOT_NULL,
                                  ParsersAttributes::INHERITED });
  attribs[ParsersAttributes::POSITION]=attribs[ParsersAttributes::OID];
  attribs.erase(ParsersAttributes::OID);
  attribs.erase(ParsersAttributes::TYPE_OID);
}

void DatabaseExplorerWidget::formatConstraintAttribs(attribs_map &attribs)
{
  map<QString, ConstraintType> types={{ParsersAttributes::PK_CONSTR, ConstraintType(ConstraintType::primary_key)},
                                      {ParsersAttributes::FK_CONSTR, ConstraintType(ConstraintType::foreign_key)},
                                      {ParsersAttributes::UQ_CONSTR, ConstraintType(ConstraintType::unique)},
                                      {ParsersAttributes::CK_CONSTR, ConstraintType(ConstraintType::check)},
                                      {ParsersAttributes::EX_CONSTR, ConstraintType(ConstraintType::exclude)}};

  ConstraintType constr_type=types[attribs[ParsersAttributes::TYPE]];
  QStringList names=getObjectName(OBJ_TABLE, attribs[ParsersAttributes::TABLE]).split('.');

  formatBooleanAttribs(attribs, { ParsersAttributes::DEFERRABLE,
                                  ParsersAttributes::NO_INHERIT });
  attribs[ParsersAttributes::TYPE]=~types[attribs[ParsersAttributes::TYPE]];
  attribs[ParsersAttributes::OP_CLASSES]=getObjectsNames(OBJ_OPCLASS,
                                                         Catalog::parseArrayValues(attribs[ParsersAttributes::OP_CLASSES])).join(ELEM_SEPARATOR);
  attribs[ParsersAttributes::SRC_COLUMNS]=getObjectsNames(OBJ_COLUMN,
                                                          Catalog::parseArrayValues(attribs[ParsersAttributes::SRC_COLUMNS]),
                                                          names[0], names[1]).join(ELEM_SEPARATOR);

  if(constr_type==ConstraintType::foreign_key)
  {
    attribs[ParsersAttributes::REF_TABLE]=getObjectName(OBJ_TABLE, attribs[ParsersAttributes::REF_TABLE]);
    names=attribs[ParsersAttributes::REF_TABLE].split('.');
    attribs[ParsersAttributes::DST_COLUMNS]=getObjectsNames(OBJ_COLUMN,
                                                            Catalog::parseArrayValues(attribs[ParsersAttributes::DST_COLUMNS]),
                                                            names[0], names[1]).join(ELEM_SEPARATOR);
  }
  else
  {
    attribs.erase(ParsersAttributes::DST_COLUMNS);
    attribs.erase(ParsersAttributes::REF_TABLE);
    attribs.erase(ParsersAttributes::UPD_ACTION);
    attribs.erase(ParsersAttributes::DEL_ACTION);
    attribs.erase(ParsersAttributes::COMPARISON_TYPE);
  }

  if(constr_type==ConstraintType::check)
  {
    attribs.erase(ParsersAttributes::DEFERRABLE);
    attribs.erase(ParsersAttributes::DEFER_TYPE);
  }
  else
    attribs.erase(ParsersAttributes::EXPRESSION);

  if(constr_type==ConstraintType::exclude)
  {
    attribs[ParsersAttributes::EXPRESSIONS]=Catalog::parseArrayValues(attribs[ParsersAttributes::EXPRESSIONS]).join(ELEM_SEPARATOR);
    attribs[ParsersAttributes::OPERATORS]=getObjectsNames(OBJ_OPERATOR,
                                                          Catalog::parseArrayValues(attribs[ParsersAttributes::OPERATORS])).join(ELEM_SEPARATOR);
  }
  else
  {
    attribs.erase(ParsersAttributes::CONDITION);
    attribs.erase(ParsersAttributes::EXPRESSIONS);
    attribs.erase(ParsersAttributes::OPERATORS);
  }
}

void DatabaseExplorerWidget::formatIndexAttribs(attribs_map &attribs)
{
  QStringList names=getObjectName(OBJ_TABLE, attribs[ParsersAttributes::TABLE]).split('.');

  formatBooleanAttribs(attribs, { ParsersAttributes::UNIQUE });

  attribs[ParsersAttributes::EXPRESSIONS]=Catalog::parseArrayValues(attribs[ParsersAttributes::EXPRESSIONS]).join(ELEM_SEPARATOR);

  attribs[ParsersAttributes::COLLATIONS]=getObjectsNames(OBJ_COLLATION,
                                                         Catalog::parseArrayValues(attribs[ParsersAttributes::COLLATIONS])).join(ELEM_SEPARATOR);

  attribs[ParsersAttributes::OP_CLASSES]=getObjectsNames(OBJ_OPCLASS,
                                                         Catalog::parseArrayValues(attribs[ParsersAttributes::OP_CLASSES])).join(ELEM_SEPARATOR);

  attribs[ParsersAttributes::COLUMNS]=getObjectsNames(OBJ_COLUMN,
                                                      Catalog::parseArrayValues(attribs[ParsersAttributes::COLUMNS]),
      names[0], names[1]).join(ELEM_SEPARATOR);
}

QString DatabaseExplorerWidget::formatObjectName(attribs_map &attribs)
{
  try
  {
    if(attribs.empty() ||
       attribs[ParsersAttributes::OID].isEmpty() ||
       attribs[ParsersAttributes::OID]==QString("0") ||
       attribs[ParsersAttributes::NAME].isEmpty())
      return(DEP_NOT_DEFINED);
    else
    {
      ObjectType obj_type=static_cast<ObjectType>(attribs[ParsersAttributes::OBJECT_TYPE].toUInt());
      attribs_map aux_attribs;
      QString oid=attribs[ParsersAttributes::OID],
              obj_name=DEP_NOT_FOUND.arg(oid), sch_name;

      if(obj_type!=OBJ_TYPE)
        obj_name=BaseObject::formatName(attribs[ParsersAttributes::NAME], obj_type==OBJ_OPERATOR);
      else
        obj_name=attribs[ParsersAttributes::NAME];

      //Retrieving the schema name
      if(!attribs[ParsersAttributes::SCHEMA].isEmpty() &&
         attribs[ParsersAttributes::SCHEMA]!=QString("0"))
      {
        aux_attribs=catalog.getObjectAttributes(OBJ_SCHEMA, attribs[ParsersAttributes::SCHEMA].toUInt());
        sch_name=BaseObject::formatName(aux_attribs[ParsersAttributes::NAME], false);

        if(!sch_name.isEmpty())
          obj_name=sch_name + QString(".") + obj_name;
      }

      //Formatting paramenter types for function
      if(obj_type==OBJ_FUNCTION)
      {
        QStringList names, arg_types=Catalog::parseArrayValues(attribs[ParsersAttributes::ARG_TYPES]);

        for(int idx=0; idx < arg_types.size(); idx++)
        {
          names=getObjectName(OBJ_TYPE, arg_types[idx]).split('.');
          arg_types[idx]=names[names.size()-1];
        }

        obj_name+=QString("(%1)").arg(arg_types.join(','));
      }
      //Formatting paramenter types for operator
      else if(obj_type==OBJ_OPERATOR)
      {
        QStringList arg_types, names;
        QString type_name;
        vector<QString> attrib_ids={ ParsersAttributes::LEFT_TYPE, ParsersAttributes::RIGHT_TYPE };

        for(QString attr : attrib_ids)
        {
          names=getObjectName(OBJ_TYPE, attribs[attr]).split('.');
          type_name=names[names.size()-1];

          if(type_name.isEmpty()) type_name=QString("-");
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

QStringList DatabaseExplorerWidget::getObjectsNames(ObjectType obj_type, const QStringList &oids, const QString &sch_name, const QString tab_name)
{
  try
  {
    if(oids.isEmpty())
     return(QStringList{ DEP_NOT_DEFINED });
    else
    {
      vector<attribs_map> attribs_vect;
      vector<unsigned> oids_vect;
      map<QString, attribs_map> attrs_map;
      QStringList names;

      //Converting the oids to unsigned in order to filter them on Catalog
      for(QString oid : oids)
        oids_vect.push_back(oid.toUInt());

      //Retrieve all the objects by their oids and put them on a auxiliary map in which key is their oids
      attribs_vect=catalog.getObjectsAttributes(obj_type, sch_name, tab_name, oids_vect);
      for(attribs_map attr : attribs_vect)
        attrs_map[attr[ParsersAttributes::OID]]=attr;

      //Retreving the names from the auxiliary map using the provided oids
      for(QString oid : oids)
        names.push_back(formatObjectName(attrs_map[oid]));

      return(names);
    }
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

QString DatabaseExplorerWidget::getObjectName(ObjectType obj_type, const QString &oid, const QString &sch_name, const QString tab_name)
{
  try
  {
    if(oid==QString("0") || oid.isEmpty())
      return(DEP_NOT_DEFINED);
    else
    {
      attribs_map attribs=catalog.getObjectAttributes(obj_type, oid.toUInt(), sch_name, tab_name);
      return(formatObjectName(attribs));
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

void DatabaseExplorerWidget::clearObjectProperties(void)
{
  while(properties_tbw->rowCount() > 0)
    properties_tbw->removeRow(0);
}

void DatabaseExplorerWidget::listObjects(void)
{
  try
  {
    configureImportHelper();
    objects_trw->blockSignals(true);
    clearObjectProperties();

    DatabaseImportForm::listObjects(import_helper, objects_trw, false, false, true);
    objects_trw->blockSignals(false);
    import_helper.closeConnection();
    catalog.closeConnection();
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

    SnippetsConfigWidget::configureSnippetsMenu(&snippets_menu, { obj_type, BASE_OBJECT });

    for(auto act : handle_menu.actions())
      handle_menu.removeAction(act);

    handle_menu.addAction(refresh_action);

    if(obj_id > 0)
    {
      if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW)
        handle_menu.addAction(show_data_action);

      handle_menu.addAction(properties_action);
    }

    if(obj_id > 0 && obj_type!=OBJ_DATABASE)
    {
      handle_menu.addSeparator();
      handle_menu.addAction(drop_action);
      handle_menu.addAction(drop_cascade_action);

      if(obj_type==OBJ_TABLE)
      {
        handle_menu.addAction(truncate_action);
        handle_menu.addAction(trunc_cascade_action);
      }
    }

    handle_menu.addSeparator();
    handle_menu.addMenu(&snippets_menu);

    QAction *exec_action=handle_menu.exec(QCursor::pos());

    if(exec_action==drop_action || exec_action==drop_cascade_action)
      dropObject(item,  exec_action==drop_cascade_action);
    else if(exec_action==truncate_action || exec_action==trunc_cascade_action)
      truncateTable(item,  exec_action==trunc_cascade_action);
    else if(exec_action==refresh_action)
      updateCurrentItem();
    else if(exec_action==properties_action)
      showObjectProperties(true);
    else if(exec_action==show_data_action)
      emit s_dataGridOpenRequested(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString(),
                                   item->text(0),
                                   item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt()!=OBJ_VIEW);
    else if(exec_action)
      handleSelectedSnippet(exec_action->text());
  }
}

void DatabaseExplorerWidget::handleSelectedSnippet(const QString &snip_id)
{
  attribs_map attribs;
  QTreeWidgetItem *item=objects_trw->currentItem();
  ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());

  loadObjectProperties();
  attribs=item->data(DatabaseImportForm::OBJECT_OTHER_DATA, Qt::UserRole).value<attribs_map>();

  if(attribs.empty())
  {
    QString sch_name=item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString(),
            tab_name=item->data(DatabaseImportForm::OBJECT_TABLE, Qt::UserRole).toString();

    //Formatting a schema qualified "table" attribute for table children objects
    if(TableObject::isTableObject(obj_type) && !sch_name.isEmpty() && !tab_name.isEmpty())
      attribs[ParsersAttributes::TABLE]=BaseObject::formatName(sch_name) + QString(".") + BaseObject::formatName(tab_name);
  }
  //Formatting the "name" attribute if it is not schema qualified
  else if(attribs.count(ParsersAttributes::SCHEMA) &&
          attribs.count(ParsersAttributes::NAME) &&
          !attribs[ParsersAttributes::NAME].contains('.'))
  {
    QString obj_name;

    if(obj_type!=OBJ_OPERATOR)
      obj_name=BaseObject::formatName(attribs[ParsersAttributes::NAME]);
    else
      obj_name=attribs[ParsersAttributes::NAME];

    attribs[ParsersAttributes::NAME]=BaseObject::formatName(attribs[ParsersAttributes::SCHEMA]) + QString(".") + obj_name;
  }

  if(attribs.count(ParsersAttributes::SQL_OBJECT)==0)
  {
    attribs[ParsersAttributes::SQL_OBJECT]=BaseObject::getSQLName(obj_type);
    attribs[ParsersAttributes::OBJECT_TYPE]=BaseObject::getSchemaName(obj_type);
  }

  for(auto attr : attribs)
  {
    if(attr.second.contains(ELEM_SEPARATOR))
      attribs[attr.first]=attr.second.replace(ELEM_SEPARATOR,QString(","));
  }

  emit s_snippetShowRequested(SnippetsConfigWidget::getParsedSnippet(snip_id, attribs));
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

      msg_box.show(msg, Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

      if(msg_box.result()==QDialog::Accepted)
      {
        QTreeWidgetItem *parent=nullptr;
        attribs_map attribs;
        QStringList types;
        QString drop_cmd, obj_name=item->text(0);
        int idx=0, idx1=0;
        Connection conn;

        attribs[ParsersAttributes::SQL_OBJECT]=BaseObject::getSQLName(obj_type);
        attribs[ParsersAttributes::DECL_IN_TABLE]=QString();
        attribs[BaseObject::getSchemaName(obj_type)]=ParsersAttributes::_TRUE_;

        //For cast, operator and function is needed to extract the name and the params types
        if(obj_type==OBJ_OPERATOR || obj_type==OBJ_FUNCTION || obj_type==OBJ_CAST)
        {
          idx=obj_name.indexOf('(');
          idx1=obj_name.indexOf(')');
          types=obj_name.mid(idx+1, idx1-idx-1).split(',');
          types.removeAll(QString("-"));
          obj_name.remove(idx, obj_name.size());
        }

        //Formatting the names
        attribs[ParsersAttributes::NAME]=BaseObject::formatName(obj_name, obj_type==OBJ_OPERATOR);
        attribs[ParsersAttributes::TABLE]=BaseObject::formatName(item->data(DatabaseImportForm::OBJECT_TABLE, Qt::UserRole).toString());
        attribs[ParsersAttributes::SCHEMA]=BaseObject::formatName(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString());

        //For table objects the "table" attribute must be schema qualified
        if(obj_type!=OBJ_INDEX && TableObject::isTableObject(obj_type))
          attribs[ParsersAttributes::TABLE]=attribs[ParsersAttributes::SCHEMA] + QString(".") + attribs[ParsersAttributes::TABLE];
        //For operators and functions there must exist the signature attribute
        else if(obj_type==OBJ_OPERATOR || obj_type==OBJ_FUNCTION)
          attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + QString(".") + attribs[ParsersAttributes::NAME] + QString("(%1)").arg(types.join(ELEM_SEPARATOR));
        else if(obj_type==OBJ_CAST)
          attribs[ParsersAttributes::SIGNATURE]=QString("(%1 AS %2)").arg(types[0]).arg(types[1]);
        else
        {
          if(!attribs[ParsersAttributes::SCHEMA].isEmpty() &&
             attribs[ParsersAttributes::NAME].indexOf(attribs[ParsersAttributes::SCHEMA] + QString(".")) < 0)
            attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + QString(".") + attribs[ParsersAttributes::NAME];
          else
            attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::NAME];
        }

        //Generate the drop command
        schparser.ignoreEmptyAttributes(true);
        schparser.ignoreUnkownAttributes(true);
        drop_cmd=schparser.getCodeDefinition(ParsersAttributes::DROP, attribs, SchemaParser::SQL_DEFINITION);

        if(cascade)
          drop_cmd.replace(';', QString(" CASCADE;"));

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

void DatabaseExplorerWidget::truncateTable(QTreeWidgetItem *item, bool cascade)
{
  Messagebox msg_box;

  try
  {
    if(item && static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt()) > 0)
    {
      QString msg, obj_name, sch_name;

      obj_name=item->text(0);
      sch_name=BaseObject::formatName(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString());

      if(!cascade)
        msg=trUtf8("Do you really want to truncate the table <strong>%1</strong>?").arg(obj_name);
      else
        msg=trUtf8("Do you really want to <strong>cascade</strong> truncate the table <strong>%1</strong>? This action will truncate all the tables that depends on it?").arg(obj_name);

      msg_box.show(msg, Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

      if(msg_box.result()==QDialog::Accepted)
      {
        attribs_map attribs;
        QString truc_cmd;
        Connection conn;

        attribs[ParsersAttributes::SQL_OBJECT]=BaseObject::getSQLName(OBJ_TABLE);
        attribs[ParsersAttributes::SIGNATURE]=sch_name + QString(".") + obj_name;
        attribs[ParsersAttributes::CASCADE]=(cascade ? ParsersAttributes::_TRUE_ : "");


        //Generate the truncate command
        schparser.ignoreEmptyAttributes(true);
        schparser.ignoreUnkownAttributes(true);
        truc_cmd=schparser.getCodeDefinition(GlobalAttributes::SCHEMAS_ROOT_DIR + GlobalAttributes::DIR_SEPARATOR +
                                             GlobalAttributes::ALTER_SCHEMA_DIR + GlobalAttributes::DIR_SEPARATOR +
                                             ParsersAttributes::TRUNCATE + GlobalAttributes::SCHEMA_EXT,
                                             attribs);

        //Executes the truncate cmd
        conn=connection;
        conn.connect();
        conn.executeDDLCommand(truc_cmd);
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
      clearObjectProperties();
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

void DatabaseExplorerWidget::loadObjectProperties(bool force_reload)
{
  try
  {
    QTreeWidgetItem *item=objects_trw->currentItem();
    unsigned oid=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();

    if(oid != 0)
    {
      ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
      attribs_map orig_attribs, fmt_attribs;     

      //First, retrieve the attributes stored on the item as a result of a previous properties listing
      orig_attribs=item->data(DatabaseImportForm::OBJECT_ATTRIBS, Qt::UserRole).value<attribs_map>();

      //In case of the cached attributes are empty
      if(orig_attribs.empty() || force_reload)
      {
        catalog.setConnection(connection);

        //Retrieve them from the catalog
        if(obj_type!=OBJ_COLUMN)
          orig_attribs=catalog.getObjectAttributes(obj_type, oid);
        else
        {
          QString tab_name=item->data(DatabaseImportForm::OBJECT_TABLE, Qt::UserRole).toString(),
                  sch_name=item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString();
          vector<attribs_map> vect_attribs=catalog.getObjectsAttributes(obj_type, sch_name, tab_name, { oid });

          if(!vect_attribs.empty())
            orig_attribs=vect_attribs[0];
        }

        //Format values and translate the attribute names
        fmt_attribs=formatObjectAttribs(orig_attribs);
        fmt_attribs.erase(ParsersAttributes::SIGNATURE);

        //Store the original attributes on the item to permit value replacements when using code snippets
        item->setData(DatabaseImportForm::OBJECT_OTHER_DATA, Qt::UserRole, QVariant::fromValue<attribs_map>(orig_attribs));

        //Store the attributes on the item to avoid repeatedly query the database
        item->setData(DatabaseImportForm::OBJECT_ATTRIBS, Qt::UserRole, QVariant::fromValue<attribs_map>(fmt_attribs));

        catalog.closeConnection();
      }
    }
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void DatabaseExplorerWidget::showObjectProperties(bool force_reload)
{
  try
  {
    QTreeWidgetItem *item=objects_trw->currentItem();
    unsigned oid=0;

    clearObjectProperties();

    if(item)
      oid=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();

    if(oid != 0)
    {
      attribs_map cached_attribs;
      QTableWidgetItem *tab_item=nullptr;
      QStringList values;
      int row=0;
      QFont font;

      loadObjectProperties(force_reload);
      cached_attribs=item->data((raw_attrib_names_chk->isChecked() ?
                                 DatabaseImportForm::OBJECT_OTHER_DATA : DatabaseImportForm::OBJECT_ATTRIBS),
                                Qt::UserRole).value<attribs_map>();

      properties_tbw->setSortingEnabled(false);

      if(!cached_attribs.empty())
      {
        for(auto attrib : cached_attribs)
        {
          properties_tbw->insertRow(properties_tbw->rowCount());
          row=properties_tbw->rowCount() - 1;

          //Creates the property name item
          tab_item=new QTableWidgetItem;
          font=tab_item->font();
          font.setItalic(true);
          tab_item->setText(attrib.first);
          tab_item->setFont(font);
          tab_item->setIcon(QPixmap(QString(":/icones/icones/attribute.png")));
          properties_tbw->setItem(row, 0, tab_item);

          values=attrib.second.split(ELEM_SEPARATOR);

          //Creating the value item
          if(values.size() >= 2)
          {
            //If the values contatins more the one item, the a combo box will be placed instead of the text
            QComboBox *combo=new QComboBox;
            combo->setStyleSheet(QString("border: 0px"));
            combo->addItems(values);
            properties_tbw->setCellWidget(row, 1, combo);
          }
          else
          {
            tab_item=new QTableWidgetItem;
            tab_item->setText(attrib.second);
            properties_tbw->setItem(row, 1, tab_item);

            //If the value contains multiple lines, configures the tooltip to expose the complete form of the value
            if(attrib.second.contains('\n'))
              tab_item->setToolTip(attrib.second);
          }
        }
      }

      properties_tbw->setSortingEnabled(true);
      properties_tbw->sortByColumn(0, Qt::AscendingOrder);
      properties_tbw->resizeColumnToContents(0);
    }

    properties_tbw->horizontalHeader()->setVisible(properties_tbw->rowCount() > 0);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}


