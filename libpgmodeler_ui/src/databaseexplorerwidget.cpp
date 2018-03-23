/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "plaintextitemdelegate.h"
#include "pgmodeleruins.h"

using namespace ParsersAttributes;

const QString DatabaseExplorerWidget::DEP_NOT_DEFINED=QString();
const QString DatabaseExplorerWidget::DEP_NOT_FOUND=QT_TR_NOOP("(not found, OID: %1)");
const QString DatabaseExplorerWidget::ELEM_SEPARATOR=QString("•");
const QString DatabaseExplorerWidget::DEFAULT_SOURCE_CODE=QT_TR_NOOP("-- Source code not generated! Hit F7 or middle-click the item to load it. --");

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
	{PREDICATE, QT_TR_NOOP("Predicate")},                {COLLATIONS, QT_TR_NOOP("Collations")},                {INHERITED, QT_TR_NOOP("Inherited")},
	{CLIENT_ENCODING, QT_TR_NOOP("Client encoding")},    {CONFIG_FILE, QT_TR_NOOP("Configuration file")},       {DATA_DIRECTORY, QT_TR_NOOP("Data directory")},
	{DYNAMIC_LIBRARY_PATH, QT_TR_NOOP("Dynamic library path")},	{DYNAMIC_SHARED_MEMORY, QT_TR_NOOP("Dynamic shared memory")}, {HBA_FILE, QT_TR_NOOP("Hba file")},
	{LISTEN_ADDRESSES, QT_TR_NOOP("Listen addresses")},  {MAX_CONNECTIONS, QT_TR_NOOP("Max. connections")},     {PORT, QT_TR_NOOP("Listen port")},
	{SERVER_ENCODING, QT_TR_NOOP("Server encoding")},    {SSL, QT_TR_NOOP("SSL")},                              {SSL_CA_FILE, QT_TR_NOOP("SSL ca file")},
	{SSL_CERT_FILE, QT_TR_NOOP("SSL cert file")},        {SSL_CRL_FILE, QT_TR_NOOP("SSL crl file")},            {SSL_KEY_FILE, QT_TR_NOOP("SSL key file")},
	{SERVER_VERSION, QT_TR_NOOP("Server version")},      {IDENT_FILE, QT_TR_NOOP("Ident file")},                {PASSWORD_ENCRYPTION, QT_TR_NOOP("Password encryption")},
	{CONNECTION, QT_TR_NOOP("Connection ID")},           {SERVER_PID, QT_TR_NOOP("Server PID")},                {SERVER_PROTOCOL, QT_TR_NOOP("Server protocol")},
	{REFERRERS, QT_TR_NOOP("Referrers")},                {IDENTITY_TYPE, QT_TR_NOOP("Identity")},               {COMMAND, QT_TR_NOOP("Command")},
	{USING_EXP, QT_TR_NOOP("USING expr.")},              {CHECK_EXP, QT_TR_NOOP("CHECK expr.")},                {ROLES, QT_TR_NOOP("Roles")}
};

DatabaseExplorerWidget::DatabaseExplorerWidget(QWidget *parent): QWidget(parent)
{
	setupUi(this);

	filter_parent->setVisible(false);

	sort_column = 0;
	splitter->setSizes({ 80, 20 });

	properties_tbw->setItemDelegate(new PlainTextItemDelegate(this, true));
	rename_item=nullptr;

	data_grid_tb->setToolTip(data_grid_tb->toolTip() + QString(" (%1)").arg(data_grid_tb->shortcut().toString()));
	runsql_tb->setToolTip(runsql_tb->toolTip() + QString(" (%1)").arg(runsql_tb->shortcut().toString()));
	refresh_tb->setToolTip(refresh_tb->toolTip() + QString(" (%1)").arg(refresh_tb->shortcut().toString()));

	QAction *act = nullptr;

	act = toggle_disp_menu.addAction(trUtf8("Show objects filter"));
	act->setCheckable(true);
	connect(act, SIGNAL(toggled(bool)), filter_parent, SLOT(setVisible(bool)));

	toggle_disp_menu.addSeparator();

	act = toggle_disp_menu.addAction(trUtf8("Show system objects"));
	act->setCheckable(true);
	connect(act, SIGNAL(toggled(bool)), this, SLOT(listObjects(void)));

	act = toggle_disp_menu.addAction(trUtf8("Show extension objects"));
	act->setCheckable(true);
	connect(act, SIGNAL(toggled(bool)), this, SLOT(listObjects(void)));

	toggle_display_tb->setMenu(&toggle_disp_menu);

	snippets_menu.setTitle(trUtf8("Snippets"));
	snippets_menu.setIcon(QIcon(QString(":icones/icones/codesnippet.png")));

	drop_action=new QAction(QIcon(QString(":icones/icones/excluir.png")), trUtf8("Drop object"), &handle_menu);
	drop_action->setShortcut(QKeySequence(Qt::Key_Delete));

	drop_cascade_action=new QAction(QIcon(QString(":icones/icones/delcascade.png")), trUtf8("Drop cascade"), &handle_menu);
	drop_cascade_action->setShortcut(QKeySequence("Shift+Del"));

	truncate_action=new QAction(QIcon(QString(":icones/icones/truncate.png")), trUtf8("Truncate"), &handle_menu);
	trunc_cascade_action=new QAction(QIcon(QString(":icones/icones/trunccascade.png")), trUtf8("Trunc. cascade"), &handle_menu);

	show_data_action=new QAction(QIcon(QString(":icones/icones/result.png")), trUtf8("Show data"), &handle_menu);
	show_data_action->setShortcut(QKeySequence(Qt::Key_Space));
	properties_action=new QAction(QIcon(QString(":icones/icones/editar.png")), trUtf8("Reload properties"), &handle_menu);

	refresh_action=new QAction(QIcon(QString(":icones/icones/atualizar.png")), trUtf8("Update"), &handle_menu);
	refresh_action->setShortcut(QKeySequence(Qt::Key_F5));

	rename_action=new QAction(QIcon(QString(":icones/icones/rename.png")), trUtf8("Rename"), &handle_menu);
	rename_action->setShortcut(QKeySequence(Qt::Key_F2));

	source_action=new QAction(QIcon(QString(":icones/icones/codigosql.png")), trUtf8("Source code"), &handle_menu);
	source_action->setShortcut(QKeySequence(Qt::Key_F7));

	objects_trw->installEventFilter(this);

	connect(refresh_tb, SIGNAL(clicked(void)), this, SLOT(listObjects(void)));
	connect(objects_trw, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(handleObject(QTreeWidgetItem *,int)));
	connect(objects_trw, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(showObjectProperties()));
	connect(raw_attrib_names_chk, SIGNAL(toggled(bool)), this, SLOT(showObjectProperties()));

	connect(objects_trw, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(cancelObjectRename()));
	connect(objects_trw, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(cancelObjectRename()));
	connect(objects_trw, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(cancelObjectRename()));

	connect(data_grid_tb, SIGNAL(clicked(bool)), this, SLOT(openDataGrid()));
	connect(drop_db_tb, SIGNAL(clicked(bool)), this, SLOT(dropDatabase()));
	connect(collapse_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(collapseAll(void)));
	connect(by_oid_chk, SIGNAL(toggled(bool)), this, SLOT(filterObjects(void)));
	connect(filter_edt, SIGNAL(textChanged(QString)), this, SLOT(filterObjects(void)));

	connect(runsql_tb, &QToolButton::clicked,
			[&]() { emit s_sqlExecutionRequested(); });

	connect(properties_tbw, &QTableWidget::itemPressed,
			[&]() { SQLExecutionWidget::copySelection(properties_tbw, true); });

	connect(expand_all_tb, &QToolButton::clicked,
			[&](){
						objects_trw->blockSignals(true);
						objects_trw->expandAll();
						objects_trw->blockSignals(false);
			});

	connect(objects_trw, &QTreeWidget::itemExpanded,
			[&](QTreeWidgetItem *item){
				ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
				unsigned oid=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();

				if((obj_type==OBJ_SCHEMA || obj_type==OBJ_TABLE || obj_type==OBJ_VIEW) && oid > 0 && item->childCount() <= 1)
				{
					updateItem(item);
				}
			});

	connect(sort_by_name_tb, &QToolButton::clicked,
	[&]() {
			sort_column = sort_by_name_tb->isChecked() ? 0 : DatabaseImportForm::OBJECT_ID;
			objects_trw->sortByColumn(sort_column);
	});

	QMenu *refresh_menu=new QMenu(refresh_tb);

	act=refresh_menu->addAction(trUtf8("Quick refresh"), this, SLOT(listObjects()), QKeySequence("Alt+F5"));
	act->setData(QVariant::fromValue<bool>(true));

	act=refresh_menu->addAction(trUtf8("Full refresh"), this, SLOT(listObjects()), QKeySequence("Ctrl+F5"));
	act->setData(QVariant::fromValue<bool>(false));

	refresh_tb->setPopupMode(QToolButton::InstantPopup);
	refresh_tb->setMenu(refresh_menu);

	filter_ht=new HintTextWidget(filter_hint, this);
	filter_ht->setText(filter_lbl->statusTip());
}

bool DatabaseExplorerWidget::eventFilter(QObject *object, QEvent *event)
{
	if(object==objects_trw && event->type()==QEvent::KeyPress)
	{
		QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

		if(k_event->key()==Qt::Key_Delete || k_event->key()==Qt::Key_F5 ||
				k_event->key()==Qt::Key_Space ||  k_event->key()==Qt::Key_F2 ||
				k_event->key()==Qt::Key_Escape ||  k_event->key()==Qt::Key_Return ||
				k_event->key()==Qt::Key_Enter || k_event->key()==Qt::Key_F7)
		{
			if(k_event->key()==Qt::Key_Space)
			{
				QTreeWidgetItem *item=objects_trw->currentItem();
				ObjectType obj_type=BASE_OBJECT;

				if(item)
				{
					unsigned oid=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();
					obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());

					if(oid!=0 && (obj_type==OBJ_TABLE || obj_type==OBJ_VIEW))
					{
						openDataGrid(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString(),
												 item->text(0), obj_type!=OBJ_VIEW);
					}
				}
			}
			else if(k_event->key()==Qt::Key_F5)
				updateItem(objects_trw->currentItem());
			else if(k_event->key()==Qt::Key_F2)
				startObjectRename(objects_trw->currentItem());
			else if(k_event->key()==Qt::Key_F7)
				loadObjectSource();
			else if(k_event->key()==Qt::Key_Escape)
				cancelObjectRename();
			else if(k_event->key()==Qt::Key_Enter ||
					k_event->key()==Qt::Key_Return)
				finishObjectRename();
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
			case OBJ_POLICY: formatPolicyAttribs(attribs); break;
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
		if(attribs.count(ParsersAttributes::NAME)!=0 &&
			 (attribs[ParsersAttributes::NAME].startsWith(QString("pg_catalog.")) ||
				attribs[ParsersAttributes::NAME].startsWith(QString("information_schema."))))
			attribs[ParsersAttributes::NAME]=attribs[ParsersAttributes::NAME].split('.').at(1);

	for(auto &attrib : attribs)
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

	if(attribs[ParsersAttributes::OID].toUInt() > 0)
	{
		attribs[ParsersAttributes::SQL_OBJECT]=BaseObject::getSQLName(obj_type);
		attribs[ParsersAttributes::OBJECT_TYPE]=BaseObject::getSchemaName(obj_type);

		if(attribs.count(ParsersAttributes::SIGNATURE)==0)
			attribs[ParsersAttributes::SIGNATURE]=BaseObject::formatName(attribs[ParsersAttributes::NAME]);

		if(attribs.count(ParsersAttributes::SCHEMA)!=0)
			attribs[ParsersAttributes::SIGNATURE]=QString("%1.%2")
																						.arg(BaseObject::formatName(attribs[ParsersAttributes::SCHEMA]))
																						.arg(attribs[ParsersAttributes::SIGNATURE]);
	}

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
		conn.executeDMLCommand(QString("SELECT last_value FROM \"%1\".\"%2\"").arg(sch_name).arg(BaseObject::formatName(attribs[ParsersAttributes::NAME])), res);

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

	if(names.isEmpty() || names.size() == 1)
		names=getObjectName(OBJ_VIEW, attribs[ParsersAttributes::TABLE]).split('.');

	formatBooleanAttribs(attribs, { ParsersAttributes::UNIQUE });

	attribs[ParsersAttributes::EXPRESSIONS]=Catalog::parseArrayValues(attribs[ParsersAttributes::EXPRESSIONS]).join(ELEM_SEPARATOR);

	attribs[ParsersAttributes::COLLATIONS]=getObjectsNames(OBJ_COLLATION,
														   Catalog::parseArrayValues(attribs[ParsersAttributes::COLLATIONS])).join(ELEM_SEPARATOR);

	attribs[ParsersAttributes::OP_CLASSES]=getObjectsNames(OBJ_OPCLASS,
														   Catalog::parseArrayValues(attribs[ParsersAttributes::OP_CLASSES])).join(ELEM_SEPARATOR);

	attribs[ParsersAttributes::COLUMNS]=getObjectsNames(OBJ_COLUMN,
														Catalog::parseArrayValues(attribs[ParsersAttributes::COLUMNS]),	names[0], names[1]).join(ELEM_SEPARATOR);
}

void DatabaseExplorerWidget::formatPolicyAttribs(attribs_map &attribs)
{
	attribs[ParsersAttributes::ROLES] = getObjectsNames(OBJ_ROLE, Catalog::parseArrayValues(attribs[ParsersAttributes::ROLES])).join(ELEM_SEPARATOR);
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

void DatabaseExplorerWidget::setConnection(Connection conn, const QString &default_db)
{
	this->connection=conn;
	this->default_db=(default_db.isEmpty() ? QString("postgres") : default_db);
}

Connection DatabaseExplorerWidget::getConnection(void)
{
	return(connection);
}

void DatabaseExplorerWidget::clearObjectProperties(void)
{
	properties_tbw->clearContents();
	properties_tbw->setRowCount(0);

	emit s_sourceCodeShowRequested(QString());
}

void DatabaseExplorerWidget::listObjects(void)
{
	try
	{
		QAction *act=qobject_cast<QAction *>(sender());
		bool quick_refresh=(act ? act->data().toBool() : true);

		configureImportHelper();
		objects_trw->blockSignals(true);

		clearObjectProperties();

		if(quick_refresh)
			QApplication::setOverrideCursor(Qt::WaitCursor);

		DatabaseImportForm::listObjects(import_helper, objects_trw, false, false, true, quick_refresh, sort_column);

		QTreeWidgetItem *root = new QTreeWidgetItem, *curr_root = nullptr;

		//Changing the root item of the generated tree to be a special item containing info about the connected server
		curr_root = objects_trw->topLevelItem(0);
		objects_trw->takeTopLevelItem(0);
		root->setText(0, connection.getConnectionId(true));
		root->setIcon(0, QPixmap(PgModelerUiNS::getIconPath("server")));
		root->setData(DatabaseImportForm::OBJECT_ID, Qt::UserRole, -1);
		root->setData(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole, BASE_OBJECT);
		root->setData(DatabaseImportForm::OBJECT_SOURCE, Qt::UserRole, trUtf8("-- Source code unavailable for this kind of object --"));
		root->addChild(curr_root);
		objects_trw->addTopLevelItem(root);
		root->setExpanded(true);

		QApplication::restoreOverrideCursor();

		objects_trw->blockSignals(false);

		import_helper.closeConnection();
		catalog.closeConnection();
	}
	catch(Exception &e)
	{
		QApplication::restoreOverrideCursor();
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseExplorerWidget::configureImportHelper(void)
{
	import_helper.setConnection(connection);
	import_helper.setCurrentDatabase(connection.getConnectionParam(Connection::PARAM_DB_NAME));
	import_helper.setImportOptions(toggle_disp_menu.actions().at(0)->isChecked(),
																 toggle_disp_menu.actions().at(1)->isChecked(),
																 false, false, false, false, false);

	catalog.closeConnection();
	catalog.setFilter(Catalog::LIST_ALL_OBJS);
	catalog.setConnection(connection);
}

void DatabaseExplorerWidget::handleObject(QTreeWidgetItem *item, int)
{
	if(item->data(DatabaseImportForm::OBJECT_OTHER_DATA, Qt::UserRole).toInt() < 0)
	{
		updateItem(item->parent());
	}
	else if(QApplication::mouseButtons()==Qt::MiddleButton && item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toInt() >= 0)
	{
		loadObjectSource();
	}
	else if(QApplication::mouseButtons()==Qt::RightButton && item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toInt() >= 0)
	{
		ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
		unsigned obj_id=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();

		SnippetsConfigWidget::configureSnippetsMenu(&snippets_menu, { obj_type, BASE_OBJECT });

		for(auto &act : handle_menu.actions())
			handle_menu.removeAction(act);

		handle_menu.addAction(refresh_action);

		if(obj_id > 0)
		{
			if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW)
				handle_menu.addAction(show_data_action);

			handle_menu.addAction(properties_action);
			handle_menu.addAction(source_action);

			if(obj_type!=OBJ_CAST && obj_type!=OBJ_DATABASE)
				handle_menu.addAction(rename_action);

			if(obj_type!=OBJ_DATABASE)
			{
				handle_menu.addSeparator();
				handle_menu.addAction(drop_action);

				if(obj_type!=OBJ_ROLE && obj_type!=OBJ_TABLESPACE)
					handle_menu.addAction(drop_cascade_action);

				if(obj_type==OBJ_TABLE)
				{
					handle_menu.addAction(truncate_action);
					handle_menu.addAction(trunc_cascade_action);
				}
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
			updateItem(objects_trw->currentItem());
		else if(exec_action==rename_action)
			startObjectRename(item);
		else if(exec_action==properties_action)
			showObjectProperties(true);
		else if(exec_action==source_action)
			loadObjectSource();
		else if(exec_action==show_data_action)
		{
			openDataGrid(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString(),
									 item->text(0),
									 item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt()!=OBJ_VIEW);
		}
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

	for(auto &attr : attribs)
	{
		if(attr.second.contains(ELEM_SEPARATOR))
			attribs[attr.first]=attr.second.replace(ELEM_SEPARATOR,QString(","));
	}

	emit s_snippetShowRequested(SnippetsConfigWidget::getParsedSnippet(snip_id, attribs));
}

attribs_map DatabaseExplorerWidget::extractAttributesFromItem(QTreeWidgetItem *item)
{
	if(!item)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
	QString obj_name=item->text(0);
	int idx=0, idx1=0;
	attribs_map attribs;
	QStringList types;

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
	else if(obj_type==OBJ_OPFAMILY || obj_type==OBJ_OPCLASS)
		obj_name.remove(QRegExp("( )+(\\[)(.)+(\\])"));

	//Formatting the names
	attribs[ParsersAttributes::NAME]=BaseObject::formatName(obj_name, obj_type==OBJ_OPERATOR);
	attribs[ParsersAttributes::TABLE]=BaseObject::formatName(item->data(DatabaseImportForm::OBJECT_TABLE, Qt::UserRole).toString());
	attribs[ParsersAttributes::SCHEMA]=BaseObject::formatName(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString());

	//For table objects the "table" attribute must be schema qualified
	if(obj_type!=OBJ_INDEX && TableObject::isTableObject(obj_type))
	{
		attribs[ParsersAttributes::TABLE]=attribs[ParsersAttributes::SCHEMA] + QString(".") + attribs[ParsersAttributes::TABLE];
		attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::NAME] + QString(" ON %1").arg(attribs[ParsersAttributes::TABLE]);
	}
	//For operators and functions there must exist the signature attribute
	else if(obj_type==OBJ_OPERATOR || obj_type==OBJ_FUNCTION)
		attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + QString(".") + attribs[ParsersAttributes::NAME] + QString("(%1)").arg(types.join(ELEM_SEPARATOR));
	else if(obj_type==OBJ_CAST)
		attribs[ParsersAttributes::SIGNATURE]=QString("(%1 AS %2)").arg(types[0]).arg(types[1]);
	else if(obj_type==OBJ_OPFAMILY || obj_type==OBJ_OPCLASS)
	{
		attribs_map aux_attribs=item->data(DatabaseImportForm::OBJECT_OTHER_DATA, Qt::UserRole).value<attribs_map>();
		attribs[ParsersAttributes::SIGNATURE]=QString("%1 USING %2").arg(attribs[ParsersAttributes::NAME]).arg(aux_attribs[ParsersAttributes::INDEX_TYPE]);
	}
	else
	{
		/* If we are handling a view we need to append the MATERIALIZED keyword in the sql-object in order
		 * to construct DDL commands correctly for this kind of object */
		if(obj_type==OBJ_VIEW)
		{
			attribs_map aux_attribs=item->data(DatabaseImportForm::OBJECT_OTHER_DATA, Qt::UserRole).value<attribs_map>();

			if(aux_attribs[ParsersAttributes::MATERIALIZED] == ParsersAttributes::_TRUE_)
			{
				attribs[ParsersAttributes::SQL_OBJECT] =
						QString("%1 %2").arg(ParsersAttributes::MATERIALIZED.toUpper())
														.arg(BaseObject::getSQLName(OBJ_VIEW));
			}
		}

		if(!attribs[ParsersAttributes::SCHEMA].isEmpty() &&
				attribs[ParsersAttributes::NAME].indexOf(attribs[ParsersAttributes::SCHEMA] + QString(".")) < 0)
			attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + QString(".") + attribs[ParsersAttributes::NAME];
		else
			attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::NAME];
	}

	return(attribs);
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
			QString obj_name=item->data(DatabaseImportForm::OBJECT_NAME, Qt::UserRole).toString();

			//Roles and tablespaces can't be removed in cascade mode
			if(cascade && (obj_type==OBJ_ROLE || obj_type==OBJ_TABLESPACE))
				return;

			if(!cascade)
				msg=trUtf8("Do you really want to drop the object <strong>%1</strong> <em>(%2)</em>?")
					.arg(obj_name).arg(BaseObject::getTypeName(obj_type));
			else
				msg=trUtf8("Do you really want to <strong>cascade</strong> drop the object <strong>%1</strong> <em>(%2)</em>? This action will drop all the other objects that depends on it.")
					.arg(obj_name).arg(BaseObject::getTypeName(obj_type));

			msg_box.show(msg, Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

			if(msg_box.result()==QDialog::Accepted)
			{
				QTreeWidgetItem *parent=nullptr;
				attribs_map attribs;
				QString drop_cmd;
				Connection conn;

				attribs=extractAttributesFromItem(item);

				if(obj_type==OBJ_OPERATOR || obj_type==OBJ_FUNCTION)
						attribs[ParsersAttributes::SIGNATURE].replace(ELEM_SEPARATOR, QChar(','));

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

				objects_trw->setCurrentItem(nullptr);
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

			obj_name=item->data(DatabaseImportForm::OBJECT_NAME, Qt::UserRole).toString();
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
				attribs[ParsersAttributes::SIGNATURE]=sch_name + QString(".\"%1\"").arg(obj_name);
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

void DatabaseExplorerWidget::updateItem(QTreeWidgetItem *item)
{
	if(item && item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toInt() >= 0)
	{
		QTreeWidgetItem *root=nullptr, *parent=nullptr, *aux_item=nullptr;
		ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
		unsigned obj_id=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt());
		QString sch_name, tab_name;
		vector<QTreeWidgetItem *> gen_items;

		QApplication::setOverrideCursor(Qt::WaitCursor);

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
					if(obj_type==OBJ_SCHEMA || obj_type==OBJ_TABLE || obj_type == OBJ_VIEW)
					{
						root=item;
						root->takeChildren();

						if(obj_type == OBJ_SCHEMA)
							sch_name=item->text(0);
						else
							tab_name=item->text(0);
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
			if(obj_id==0 || (obj_type!=OBJ_TABLE && obj_type!=OBJ_VIEW && obj_type!=OBJ_SCHEMA))
				gen_items=DatabaseImportForm::updateObjectsTree(import_helper, objects_trw, { obj_type }, false, false, root, sch_name, tab_name, sort_column);
			else
				//Updates all child objcts when the selected object is a schema or table or view
				gen_items=DatabaseImportForm::updateObjectsTree(import_helper, objects_trw,
																BaseObject::getChildObjectTypes(obj_type), false, false, root, sch_name, tab_name, sort_column);

			//Creating dummy items for schemas and tables
			if(obj_type==OBJ_SCHEMA || obj_type==OBJ_TABLE || obj_type==OBJ_VIEW)
			{
				for(auto &item : gen_items)
				{
					aux_item=new QTreeWidgetItem(item);
					aux_item->setText(0, QString("..."));
					aux_item->setData(DatabaseImportForm::OBJECT_OTHER_DATA, Qt::UserRole, QVariant::fromValue<int>(-1));
				}
			}

			import_helper.closeConnection();
			objects_trw->sortItems(sort_column, Qt::AscendingOrder);
			objects_trw->setCurrentItem(nullptr);

			if(obj_type==OBJ_TABLE)
			{
				objects_trw->blockSignals(true);
				objects_trw->setCurrentItem(item);
				showObjectProperties(true);
				objects_trw->setCurrentItem(nullptr);
				objects_trw->blockSignals(false);
			}
		}

		QApplication::restoreOverrideCursor();
	}
}

void DatabaseExplorerWidget::loadObjectProperties(bool force_reload)
{
	try
	{
		QTreeWidgetItem *item=objects_trw->currentItem();
		unsigned oid=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();

		if(oid != 0 || (item == objects_trw->topLevelItem(0)))
		{
			ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
			attribs_map orig_attribs, fmt_attribs;

			//First, retrieve the attributes stored on the item as a result of a previous properties listing
			orig_attribs=item->data(DatabaseImportForm::OBJECT_ATTRIBS, Qt::UserRole).value<attribs_map>();

			//In case of the cached attributes are empty
			if(orig_attribs.empty() || force_reload)
			{
				QApplication::setOverrideCursor(Qt::WaitCursor);
				catalog.setConnection(connection);

				//Loading the server properties
				if(item == objects_trw->topLevelItem(0))
					orig_attribs=catalog.getServerAttributes();
				//Retrieve them from the catalog
				else if(obj_type!=OBJ_COLUMN)
				{
					orig_attribs=catalog.getObjectAttributes(obj_type, oid);

					if(obj_type == OBJ_TABLE)
					{
						vector<attribs_map> ref_fks;
						attribs_map ref_table, ref_schema;
						QStringList tab_list;

						ref_fks = catalog.getObjectsAttributes(OBJ_CONSTRAINT, QString(), QString(), {}, {{ ParsersAttributes::CUSTOM_FILTER, QString("contype='f' AND cs.confrelid=%1").arg(orig_attribs[ParsersAttributes::OID])}});

						for(auto &fk : ref_fks)
						{
							ref_table = catalog.getObjectAttributes(OBJ_TABLE, fk[ParsersAttributes::TABLE].toUInt());
							ref_schema = catalog.getObjectAttributes(OBJ_SCHEMA, ref_table[ParsersAttributes::SCHEMA].toUInt());
							tab_list.push_back(QString("%1.%2").arg(ref_schema[ParsersAttributes::NAME]).arg(ref_table[ParsersAttributes::NAME]));
						}

						if(!tab_list.isEmpty())
							orig_attribs[ParsersAttributes::REFERRERS] = tab_list.join(Table::DATA_SEPARATOR);
					}
				}
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

				if(item != objects_trw->topLevelItem(0))
					item->setData(DatabaseImportForm::OBJECT_SOURCE, Qt::UserRole, DEFAULT_SOURCE_CODE);

				catalog.closeConnection();
				QApplication::restoreOverrideCursor();
			}
		}
	}
	catch(Exception &e)
	{
		QApplication::restoreOverrideCursor();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseExplorerWidget::showObjectProperties(bool force_reload)
{
	try
	{
		QTreeWidgetItem *item=objects_trw->currentItem();
		clearObjectProperties();

		if(item &&
			 ((item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toInt() >= 0) ||
				(item == objects_trw->topLevelItem(0))))
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
				for(auto &attrib : cached_attribs)
				{
					properties_tbw->insertRow(properties_tbw->rowCount());
					row=properties_tbw->rowCount() - 1;

					//Creates the property name item
					tab_item=new QTableWidgetItem;
					font=tab_item->font();
					font.setItalic(true);
					tab_item->setText(attrib.first);
					tab_item->setFont(font);
					tab_item->setIcon(QPixmap(PgModelerUiNS::getIconPath("attribute")));
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
						if(attrib.second.contains('\n') || attrib.second.length() > 30)
							tab_item->setToolTip(attrib.second);
					}
				}

				cached_attribs=item->data(DatabaseImportForm::OBJECT_OTHER_DATA,Qt::UserRole).value<attribs_map>();

				if(cached_attribs[ParsersAttributes::OBJECT_TYPE]==BaseObject::getSchemaName(OBJ_CONSTRAINT) &&  item->childCount()==0)
				{
					QTreeWidgetItem *fk_item=nullptr, *src_item=nullptr;

					if(cached_attribs[ParsersAttributes::TYPE]==~ConstraintType(ConstraintType::foreign_key))
					{
						/* Creates two items denoting the source columns and referenced tables.
							These items have a negative id indicating that no popup menu will be show if user
							right-click them. */

						src_item=new QTreeWidgetItem(item);
						src_item->setData(DatabaseImportForm::OBJECT_ID, Qt::UserRole, QVariant::fromValue<int>(-1));
						src_item->setIcon(0, QPixmap(PgModelerUiNS::getIconPath("column")));
						src_item->setText(0, QString("%1(%2)")
															.arg(cached_attribs[ParsersAttributes::TABLE])
															.arg(cached_attribs[ParsersAttributes::SRC_COLUMNS]));
						src_item->setToolTip(0, trUtf8("Src. table: %1\nSrc. column(s): %2")
																	.arg(cached_attribs[ParsersAttributes::TABLE])
																	.arg(cached_attribs[ParsersAttributes::SRC_COLUMNS]));
						src_item->setFlags(Qt::ItemIsEnabled);

						fk_item=new QTreeWidgetItem(item);
						fk_item->setData(DatabaseImportForm::OBJECT_ID, Qt::UserRole, QVariant::fromValue<int>(-1));
						fk_item->setIcon(0, QPixmap(PgModelerUiNS::getIconPath("referenced")));
						fk_item->setText(0, QString("%1(%2)")
														.arg(cached_attribs[ParsersAttributes::REF_TABLE])
														.arg(cached_attribs[ParsersAttributes::DST_COLUMNS]));
						fk_item->setToolTip(0, trUtf8("Ref. table: %1\nRef. column(s): %2")
																.arg(cached_attribs[ParsersAttributes::REF_TABLE])
																.arg(cached_attribs[ParsersAttributes::DST_COLUMNS]));
						fk_item->setFlags(Qt::ItemIsEnabled);
					}
					else if(cached_attribs[ParsersAttributes::TYPE]==~ConstraintType(ConstraintType::unique) ||
									cached_attribs[ParsersAttributes::TYPE]==~ConstraintType(ConstraintType::primary_key))
					{
						QStringList columns=cached_attribs[ParsersAttributes::SRC_COLUMNS].split(ELEM_SEPARATOR);

						for(auto &col : columns)
						{
							src_item=new QTreeWidgetItem(item);
							src_item->setData(DatabaseImportForm::OBJECT_ID, Qt::UserRole, QVariant::fromValue<int>(-1));
							src_item->setIcon(0, QPixmap(PgModelerUiNS::getIconPath("column")));
							src_item->setText(0, col);
							src_item->setFlags(Qt::ItemIsEnabled);
						}
					}
				}
				else if(cached_attribs[ParsersAttributes::OBJECT_TYPE] == BaseObject::getSchemaName(OBJ_TABLE) &&
								!cached_attribs[ParsersAttributes::REFERRERS].isEmpty() && item->childCount() == 5)
				{
					QTreeWidgetItem *refs_item=nullptr, *tab_item=nullptr;
					QStringList ref_tab_names = cached_attribs[ParsersAttributes::REFERRERS].split(Table::DATA_SEPARATOR);
					QFont font;

					refs_item=new QTreeWidgetItem(item);
					font = refs_item->font(0);
					font.setItalic(true);

					refs_item->setFont(0, font);
					refs_item->setData(DatabaseImportForm::OBJECT_ID, Qt::UserRole, QVariant::fromValue<int>(-1));
					refs_item->setIcon(0, QPixmap(PgModelerUiNS::getIconPath("referrer")));
					refs_item->setText(0, QString("%1 (%2)")
															.arg(attribs_i18n.at(ParsersAttributes::REFERRERS))
															.arg(ref_tab_names.length()));

					for(QString tab_name : ref_tab_names)
					{
						tab_item=new QTreeWidgetItem(refs_item);
						tab_item->setData(DatabaseImportForm::OBJECT_ID, Qt::UserRole, QVariant::fromValue<int>(-1));
						tab_item->setIcon(0, QPixmap(PgModelerUiNS::getIconPath("table")));
						tab_item->setText(0, tab_name);
						tab_item->setFlags(Qt::ItemIsEnabled);
					}
				}
			}

			emit s_sourceCodeShowRequested(item->data(DatabaseImportForm::OBJECT_SOURCE, Qt::UserRole).toString());

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

void DatabaseExplorerWidget::startObjectRename(QTreeWidgetItem *item)
{
	if(item && static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt()) > 0)
	{
		ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());

		if(obj_type!=OBJ_CAST && obj_type!=OBJ_DATABASE)
		{
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			objects_trw->openPersistentEditor(item);
			rename_item=item;
			rename_item->setData(DatabaseImportForm::OBJECT_OTHER_DATA, Qt::UserRole, rename_item->text(0));
		}
	}
}

void DatabaseExplorerWidget::finishObjectRename(void)
{
	Messagebox msg_box;

	try
	{
		if(rename_item)
		{
			QString rename_cmd;
			Connection conn=connection;
			attribs_map attribs=extractAttributesFromItem(rename_item);
			ObjectType obj_type=static_cast<ObjectType>(rename_item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());

			objects_trw->closePersistentEditor(rename_item);
			attribs[ParsersAttributes::NEW_NAME]=BaseObject::formatName(rename_item->text(0), obj_type==OBJ_OPERATOR);

			//Generate the drop command
			schparser.ignoreEmptyAttributes(true);
			schparser.ignoreUnkownAttributes(true);
			rename_cmd=schparser.getCodeDefinition(GlobalAttributes::SCHEMAS_ROOT_DIR + GlobalAttributes::DIR_SEPARATOR +
												   GlobalAttributes::ALTER_SCHEMA_DIR + GlobalAttributes::DIR_SEPARATOR +
												   ParsersAttributes::RENAME + GlobalAttributes::SCHEMA_EXT,
												   attribs);

			//Executes the rename cmd
			conn.connect();
			conn.executeDDLCommand(rename_cmd);

			rename_item->setFlags(rename_item->flags() ^ Qt::ItemIsEditable);
			rename_item=nullptr;
		}
	}
	catch(Exception &e)
	{
		cancelObjectRename();
		msg_box.show(e);
	}
}

void DatabaseExplorerWidget::cancelObjectRename(void)
{
	if(rename_item)
	{
		objects_trw->closePersistentEditor(rename_item);
		rename_item->setFlags(rename_item->flags() ^ Qt::ItemIsEditable);
		rename_item->setText(0, rename_item->data(DatabaseImportForm::OBJECT_OTHER_DATA, Qt::UserRole).toString());
		rename_item=nullptr;
	}
}

void DatabaseExplorerWidget::loadObjectSource(void)
{
	QTreeWidgetItem *item=objects_trw->currentItem();

	try
	{
		if(item == objects_trw->topLevelItem(0))
		{
			QString n = item->text(0);
			emit s_sourceCodeShowRequested(item->data(DatabaseImportForm::OBJECT_SOURCE, Qt::UserRole).toString());
		}
		else if(item)
		{
			QString source=item->data(DatabaseImportForm::OBJECT_SOURCE, Qt::UserRole).toString();

			if(source!=DEFAULT_SOURCE_CODE)
			{
				emit s_sourceCodeShowRequested(source);
			}
			else
			{
				DatabaseModel dbmodel;
				DatabaseImportHelper import_hlp;
				ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
				QString sch_name, tab_name, name;
				QTreeWidgetItem *sch_item=nullptr;
				BaseObject *object=nullptr;
				BaseObject *schema=nullptr;
				attribs_map attribs=item->data(DatabaseImportForm::OBJECT_OTHER_DATA, Qt::UserRole).value<attribs_map>();
				bool is_column=false;
				unsigned oid=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt(),
						db_oid=objects_trw->topLevelItem(0)->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt(),
						sys_oid=0;
				int sbar_value=(objects_trw->verticalScrollBar() ? objects_trw->verticalScrollBar()->value() : 0);

				QApplication::setOverrideCursor(Qt::WaitCursor);
				sch_name=item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString();
				tab_name=item->data(DatabaseImportForm::OBJECT_TABLE, Qt::UserRole).toString();
				name=item->data(DatabaseImportForm::OBJECT_NAME, Qt::UserRole).toString();

				if(!sch_name.isEmpty() && obj_type!=OBJ_EXTENSION)
				{
					if(tab_name.isEmpty())
						name.prepend(sch_name + QChar('.'));
					else
						tab_name.prepend(sch_name + QChar('.'));
				}

				//Special case for columns. We will retrieve the table from database and then generate the code for the column
				if(obj_type==OBJ_COLUMN)
				{
					oid=item->parent()->parent()->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();
					is_column=true;
					obj_type=OBJ_TABLE;
				}

				//Importing the object and its dependencies
				dbmodel.createSystemObjects(false);
				import_hlp.setConnection(connection);
				import_hlp.setCurrentDatabase(connection.getConnectionParam(Connection::PARAM_DB_NAME));
				import_hlp.setImportOptions(toggle_disp_menu.actions().at(0)->isChecked(),
																		toggle_disp_menu.actions().at(1)->isChecked(),
																		true, false, false, false, false);
				import_hlp.setSelectedOIDs(&dbmodel, {{OBJ_DATABASE, {db_oid}}, {obj_type,{oid}}}, {});
				sys_oid=import_hlp.getLastSystemOID();

				//Currently pgModeler does not support the visualization of base types and built-in ones
				if(obj_type==OBJ_TYPE &&
					 (oid <= sys_oid || attribs[ParsersAttributes::CONFIGURATION]==ParsersAttributes::BASE_TYPE))
				{
					source=trUtf8("-- Source code genaration for buil-in and base types currently unavailable --");
					emit s_sourceCodeShowRequested(source);
				}
				else
				{
					import_hlp.importDatabase();

					if(obj_type==OBJ_DATABASE)
						source=getObjectSource(&dbmodel, &dbmodel);
					else
					{
						/* Fixing the signature of opclasses and opfamilies.
								The name is in form "name [index type]", so we change it to "name USING [index type]" */
						if(obj_type==OBJ_OPCLASS || obj_type==OBJ_OPFAMILY)
						{
							QString idx_type=item->text(0);

							idx_type.remove(0, idx_type.indexOf(QChar('[')) + 1);
							idx_type.remove(QChar(']'));

							name=QString("%1 USING %2").arg(name).arg(idx_type);
						}

						//Generating the code for table child object
						if(TableObject::isTableObject(obj_type) || is_column)
						{
							Table *table=nullptr;
							table=dynamic_cast<Table *>(dbmodel.getObject(tab_name, OBJ_TABLE));
							QTreeWidgetItem *table_item=nullptr;

							//If the table was imported then the source code of it will be placed on the respective item
							if(table)
							{
								table_item=item->parent()->parent();
								objects_trw->setCurrentItem(item->parent()->parent());
								table_item->setData(DatabaseImportForm::OBJECT_SOURCE, Qt::UserRole, getObjectSource(table, &dbmodel));

								sch_item=table_item->parent()->parent();
								schema=table->getSchema();

								//Generate the code of table children objects as ALTER commands
								table->setGenerateAlterCmds(true);
								object=table->getObject(name, (is_column ? OBJ_COLUMN : obj_type));
							}
						}
						else
						{
							object=dbmodel.getObject(name, obj_type);
							schema=object->getSchema();
						}

						if(object)
							source=getObjectSource(object, &dbmodel);
						else
							source=trUtf8("-- Source code unavailable for the object %1 (%2). --").arg(name).arg(BaseObject::getTypeName(obj_type));
					}
				}

				//Generating the schema code and assigning it to the respective items
				if(schema)
				{
					if(!sch_item) sch_item=item->parent()->parent();
					objects_trw->setCurrentItem(sch_item);
					sch_item->setData(DatabaseImportForm::OBJECT_SOURCE, Qt::UserRole, getObjectSource(schema, &dbmodel));
				}

				if(obj_type != OBJ_DATABASE)
				{
					//Generating the code for the database itself and storing it in the first child of the root item in the tree
					objects_trw->setCurrentItem(objects_trw->topLevelItem(0));
					objects_trw->topLevelItem(0)->child(0)->setData(DatabaseImportForm::OBJECT_SOURCE, Qt::UserRole, getObjectSource(&dbmodel, &dbmodel));
				}

				item->setData(DatabaseImportForm::OBJECT_SOURCE, Qt::UserRole, source);
				objects_trw->setCurrentItem(item);

				/* Restore the position of the scrollbar in the tree because the usage of setCurrentItem in previous lines
					 may cause the scrollbar to change its original value */
				if(objects_trw->verticalScrollBar())
					objects_trw->verticalScrollBar()->setValue(sbar_value);

				QApplication::restoreOverrideCursor();
				emit s_sourceCodeShowRequested(source);
			}
		}
	}
	catch (Exception &e)
	{
		QApplication::restoreOverrideCursor();
		emit s_sourceCodeShowRequested(QString("/* Could not generate source code due to one or more errors! \n \n %1 */").arg(e.getExceptionsText()));
	}
}

void DatabaseExplorerWidget::filterObjects(void)
{
	DatabaseImportForm::filterObjects(objects_trw, filter_edt->text(),
																		(by_oid_chk->isChecked() ? DatabaseImportForm::OBJECT_ID : 0), false);
}

QString DatabaseExplorerWidget::getObjectSource(BaseObject *object, DatabaseModel *dbmodel)
{
	if(!object || !dbmodel)
		return QString();

	vector<Permission *> perms;
	QString source;

	dbmodel->getPermissions(object, perms);
	object->setSystemObject(false);
	object->setSQLDisabled(false);
	object->setCodeInvalidated(true);

	if(object!=dbmodel)
		source=object->getCodeDefinition(SchemaParser::SQL_DEFINITION);
	else
		source=dbmodel->__getCodeDefinition(SchemaParser::SQL_DEFINITION);

	for(auto &perm : perms)
		source+=perm->getCodeDefinition(SchemaParser::SQL_DEFINITION);

	return(source);
}

void DatabaseExplorerWidget::openDataGrid(const QString &schema, const QString &table, bool hide_views)
{
#ifdef DEMO_VERSION
#warning "DEMO VERSION: data manipulation feature disabled warning."
	Messagebox msg_box;
	msg_box.show(trUtf8("Warning"),
				 trUtf8("You're running a demonstration version! The data manipulation feature is available only in the full version!"),
				 Messagebox::ALERT_ICON, Messagebox::OK_BUTTON);
#else
	DataManipulationForm *data_manip=new DataManipulationForm;
	Connection conn=Connection(this->connection.getConnectionParams());

	data_manip->setWindowModality(Qt::NonModal);
	data_manip->setAttribute(Qt::WA_DeleteOnClose, true);
	data_manip->hide_views_chk->setChecked(hide_views);

	data_manip->setAttributes(conn, schema, table);

	PgModelerUiNS::resizeDialog(data_manip);
	data_manip->show();
#endif
}

void DatabaseExplorerWidget::dropDatabase(void)
{
	Messagebox msg_box;
	QString dbname = connection.getConnectionParam(Connection::PARAM_DB_NAME);

	msg_box.show(trUtf8("Warning"),
				 trUtf8("<strong>CAUTION:</strong> You are about to drop the entire database <strong>%1</strong>! All data will be completely wiped out. Do you really want to proceed?").arg(dbname),
				 Messagebox::ALERT_ICON, Messagebox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
	{
		try
		{
			Connection conn=Connection(connection.getConnectionParams());
			conn.setConnectionParam(Connection::PARAM_DB_NAME, default_db);
			conn.connect();
			conn.executeDDLCommand(QString("DROP DATABASE \"%1\";").arg(dbname));
			conn.close();
			this->setEnabled(false);
			emit s_databaseDropped(dbname);
		}
		catch(Exception &e)
		{
			if(connection.getConnectionParam(Connection::PARAM_DB_NAME) == default_db)
				throw Exception(Exception::getErrorMessage(ERR_DROP_CURRDB_DEFAULT)
												.arg(dbname).arg(connection.getConnectionParam(Connection::PARAM_ALIAS)),
												ERR_DROP_CURRDB_DEFAULT,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			else
				throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}
