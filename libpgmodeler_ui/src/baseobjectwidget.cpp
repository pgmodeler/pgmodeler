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

#include "baseobjectwidget.h"
#include "permissionwidget.h"


extern PermissionWidget *permission_wgt;

const QColor BaseObjectWidget::PROT_LINE_BGCOLOR=QColor(255,180,180);
const QColor BaseObjectWidget::PROT_LINE_FGCOLOR=QColor(80,80,80);
const QColor BaseObjectWidget::RELINC_LINE_BGCOLOR=QColor(164,249,176);
const QColor BaseObjectWidget::RELINC_LINE_FGCOLOR=QColor(80,80,80);

BaseObjectWidget::BaseObjectWidget(QWidget *parent, ObjectType obj_type): QDialog(parent)
{
	try
	{
		QHBoxLayout *layout=NULL;
		QSpacerItem *spacer=NULL;

		setupUi(this);
		model=NULL;
		table=NULL;
		relationship=NULL;
		prev_schema=NULL;
		op_list=NULL;
		object=NULL;
		object_px=NAN;
		object_py=NAN;
		pf_min_height=-1;
		pf_max_height=-1;
		parent_form=NULL;
		schema_sel=NULL;
		owner_sel=NULL;
		tablespace_sel=NULL;

		object_selection_wgt=new ModelObjectsWidget(true);

		parent_form=new BaseForm(NULL, (Qt::WindowTitleHint | Qt::WindowSystemMenuHint));
		parent_form->setWindowTitle(trUtf8("Create / Edit: ") + BaseObject::getTypeName(obj_type));
		parent_form->generalwidget_wgt->insertWidget(0, this);
		parent_form->generalwidget_wgt->setCurrentIndex(0);
		parent_form->setButtonConfiguration(MessageBox::OK_CANCEL_BUTTONS);

		connect(edt_perms_tb, SIGNAL(clicked(bool)),this, SLOT(editPermissions(void)));
		connect(parent_form->cancel_btn, SIGNAL(clicked(bool)), parent_form, SLOT(reject(void)));
		connect(parent_form, SIGNAL(rejected()), this, SLOT(reject()));

		schema_sel=new ObjectSelectorWidget(OBJ_SCHEMA, true, this);
		owner_sel=new ObjectSelectorWidget(OBJ_ROLE, true, this);
		tablespace_sel=new ObjectSelectorWidget(OBJ_TABLESPACE, true, this);
		collation_sel=new ObjectSelectorWidget(OBJ_COLLATION, true, this);

		baseobject_grid = new QGridLayout;
		baseobject_grid->setObjectName(Utf8String::create("objetobase_grid"));
		baseobject_grid->addWidget(protected_obj_frm, 0, 0, 1, 0);
		baseobject_grid->addWidget(name_lbl, 1, 0, 1, 1);
		baseobject_grid->addWidget(name_edt, 1, 1, 1, 3);
		baseobject_grid->addWidget(obj_icon_lbl, 1, 4, 1, 1);
		baseobject_grid->addWidget(schema_lbl, 4, 0, 1, 1);
		baseobject_grid->addWidget(schema_sel, 4, 1, 1, 4);
		baseobject_grid->addWidget(collation_lbl, 5, 0, 1, 1);
		baseobject_grid->addWidget(collation_sel, 5, 1, 1, 4);
		baseobject_grid->addWidget(tablespace_lbl, 6, 0, 1, 1);
		baseobject_grid->addWidget(tablespace_sel, 6, 1, 1, 4);
		baseobject_grid->addWidget(owner_lbl, 7, 0, 1, 1);
		baseobject_grid->addWidget(owner_sel, 7, 1, 1, 4);
		baseobject_grid->addWidget(comment_lbl, 8, 0, 1, 1);
		baseobject_grid->addWidget(comment_edt, 8, 1, 1, 4);

		layout=new QHBoxLayout;
		spacer=new QSpacerItem(20,1,QSizePolicy::Expanding);

		layout->addItem(spacer);
		layout->addWidget(disable_sql_chk);
		layout->addWidget(edt_perms_tb);

		baseobject_grid->addLayout(layout,9,0,1,5);
		baseobject_grid->addWidget(div1_ln, 10, 0, 1, 5);
	}
	catch(Exception &e)
	{
		if(parent_form) delete(parent_form);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

BaseObjectWidget::~BaseObjectWidget(void)
{
	delete(object_selection_wgt);
	delete(parent_form);
}

void BaseObjectWidget::show(void)
{
	parent_form->exec();
	QDialog::show();
}

void BaseObjectWidget::showEvent(QShowEvent *)
{
	if(pf_min_height < 0)
	{
		pf_min_height=parent_form->minimumHeight();
		pf_max_height=parent_form->maximumHeight();
	}

	if(protected_obj_frm->isVisible())
	{
		parent_form->setMinimumHeight(pf_min_height + protected_obj_frm->height() + 10);
		parent_form->setMaximumHeight(pf_max_height + protected_obj_frm->height() + 10);
		parent_form->resize(parent_form->minimumWidth(),parent_form->minimumHeight());
	}
	else if(pf_min_height > 0)
	{
		parent_form->setMinimumHeight(pf_min_height);
		parent_form->setMaximumHeight(pf_max_height);
		parent_form->resize(parent_form->minimumWidth(), pf_min_height);
	}
}

void BaseObjectWidget::hideEvent(QHideEvent *)
{
	name_edt->clear();
	comment_edt->clear();

	tablespace_sel->clearSelector();
	schema_sel->clearSelector();
	owner_sel->clearSelector();
	collation_sel->clearSelector();

	disable_sql_chk->setChecked(false);

	parent_form->blockSignals(true);
	parent_form->apply_ok_btn->setEnabled(true);
	parent_form->close();
	parent_form->blockSignals(false);

	new_object=false;
}

void BaseObjectWidget::setRequiredField(QWidget *widget)
{
	if(widget)
	{
		QLabel *lbl=dynamic_cast<QLabel *>(widget);
		QLineEdit *edt=dynamic_cast<QLineEdit *>(widget);
		QTextEdit *txt=dynamic_cast<QTextEdit *>(widget);
		QGroupBox *grp=dynamic_cast<QGroupBox *>(widget);
		ObjectSelectorWidget *sel=dynamic_cast<ObjectSelectorWidget *>(widget);
		PgSQLTypeWidget *pgtype=dynamic_cast<PgSQLTypeWidget *>(widget);
		QString str_aux=" <span style='color: #ff0000;'>*</span> ";
		QColor bgcolor=QColor("#ffffc0");

		QFont fnt=widget->font();

		if(lbl || pgtype || grp)
		{
			fnt.setBold(true);

			if(lbl)
				lbl->setText(str_aux + lbl->text());

			if(!grp)
				widget->setFont(fnt);
			else
				grp->setStyleSheet("QGroupBox {	font-weight: bold; }");
		}
		else if(edt || txt || sel)
		{
			QPalette pal;
			pal.setColor(QPalette::Base, bgcolor);

			if(sel)
				widget=sel->obj_name_txt;
			widget->setPalette(pal);
		}

		str_aux=(!widget->toolTip().isEmpty() ? "\n" : "");
		widget->setToolTip(widget->toolTip() + str_aux + trUtf8("Required field. Leaving this empty will raise errors!"));
	}
}

void BaseObjectWidget::setAttributes(DatabaseModel *model, BaseObject *object, BaseObject *parent_obj)
{
	setAttributes(model, NULL, object, parent_obj, NAN, NAN, false);
}

void BaseObjectWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseObject *object, BaseObject *parent_obj, float obj_px, float obj_py, bool uses_op_list)
{
	ObjectType obj_type, parent_type=BASE_OBJECT;

	/* Reseting the objects attributes in order to force them to be redefined
	every time this method is called */
	this->object=NULL;
	this->model=NULL;
	this->op_list=NULL;
	this->relationship=NULL;
	this->table=NULL;

	if(!model || (uses_op_list && !op_list))
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->model=model;

	if(parent_obj)
	{
		parent_type=parent_obj->getObjectType();

		if(parent_type==OBJ_TABLE || parent_type==OBJ_VIEW)
			this->table=dynamic_cast<BaseTable *>(parent_obj);
		else if(parent_type==OBJ_RELATIONSHIP)
			this->relationship=dynamic_cast<Relationship *>(parent_obj);
		else if(parent_type!=OBJ_DATABASE && parent_type!=OBJ_SCHEMA)
			throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else
	{
		TableObject *tab_obj=dynamic_cast<TableObject *>(object);

		if(object && object->getSchema())
			parent_obj=object->getSchema();
		else if(tab_obj && tab_obj->getParentTable())
			parent_obj=tab_obj->getParentTable();
		else
			parent_obj=model;
	}

	if(dynamic_cast<BaseGraphicObject *>(object))
		dynamic_cast<BaseGraphicObject *>(object)->setModified(false);

	this->op_list=op_list;
	this->object=object;

	if(this->table)
	{
		this->object_px=this->table->getPosition().x();
		this->object_py=this->table->getPosition().y();
	}
	else
	{
		this->object_px=obj_px;
		this->object_py=obj_py;
	}

	name_edt->setFocus();
	edt_perms_tb->setEnabled(object!=NULL);

	owner_sel->setModel(model);
	schema_sel->setModel(model);
	tablespace_sel->setModel(model);
	collation_sel->setModel(model);

	if(object)
	{
		bool prot;

		name_edt->setText(Utf8String::create(object->getName()));
		comment_edt->setText(Utf8String::create(object->getComment()));
		owner_sel->setSelectedObject(object->getOwner());
		collation_sel->setSelectedObject(object->getCollation());

		//if there is no schema assigned to object, set the "public" as the default
		if(!object->getSchema())
			schema_sel->setSelectedObject(model->getObject("public", OBJ_SCHEMA));
		else
			schema_sel->setSelectedObject(object->getSchema());

		tablespace_sel->setSelectedObject(object->getTablespace());

		obj_type=object->getObjectType();
		prot=(parent_type!=OBJ_RELATIONSHIP &&
											 (object->isProtected() ||
												((obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT) &&
												 dynamic_cast<TableObject *>(object)->isAddedByRelationship())));
		protected_obj_frm->setVisible(prot);

		parent_form->apply_ok_btn->setEnabled(!prot);

		disable_sql_chk->setChecked(object->isSQLDisabled());
	}
	else
	{
		protected_obj_frm->setVisible(false);

		if(parent_obj && parent_obj->getObjectType()==OBJ_SCHEMA)
			schema_sel->setSelectedObject(parent_obj);
		else
			schema_sel->setSelectedObject(model->getObject("public", OBJ_SCHEMA));
	}
}

void BaseObjectWidget::configureFormLayout(QGridLayout *grid, ObjectType obj_type)
{
	bool show_comment;

	if(grid)
	{
		QLayoutItem *item=NULL;
		int lin, col, col_span,row_span, item_id, item_count;

		/* Move all the widgets of the passed grid layout one row down,
		 permiting the insertion of the 'baseobject_grid' at the top
		 of the items */
		item_count=grid->count();
		for(item_id=item_count-1; item_id >= 0; item_id--)
		{
			item=grid->itemAt(item_id);
			grid->getItemPosition(item_id, &lin, &col, &row_span, &col_span);
			grid->removeItem(item);
			grid->addItem(item, lin+1, col, row_span, col_span);

			/* Configuring QTextEdit to accept tabs as focus changes. This code
			only applies to widgets directly linked to the layout. If there is some
			QTextEdit nested in some child widget this is not applied */
			if(dynamic_cast<QTextEdit *>(item->widget()))
				dynamic_cast<QTextEdit *>(item->widget())->setTabChangesFocus(true);
		}

		//Adding the base layout on the top
		grid->addLayout(baseobject_grid, 0,0,1,0);
		baseobject_grid=grid;
	}
	else
		this->setLayout(baseobject_grid);

	baseobject_grid->setContentsMargins(4, 4, 4, 4);
	disable_sql_chk->setVisible(obj_type!=BASE_OBJECT && obj_type!=OBJ_PERMISSION && obj_type!=OBJ_TEXTBOX);

	if(obj_type!=OBJ_TABLE && obj_type!=OBJ_COLUMN && obj_type!=OBJ_VIEW &&
		 obj_type!=OBJ_SEQUENCE && obj_type!=OBJ_DATABASE && obj_type!=OBJ_FUNCTION &&
		 obj_type!=OBJ_AGGREGATE && obj_type!=OBJ_LANGUAGE && obj_type!=OBJ_SCHEMA &&
		 obj_type!=OBJ_TABLESPACE)
		edt_perms_tb->setVisible(false);

	schema_lbl->setVisible(BaseObject::acceptsSchema(obj_type));
	schema_sel->setVisible(BaseObject::acceptsSchema(obj_type));

	owner_lbl->setVisible(BaseObject::acceptsOwner(obj_type));
	owner_sel->setVisible(BaseObject::acceptsOwner(obj_type));

	tablespace_lbl->setVisible(BaseObject::acceptsTablespace(obj_type));
	tablespace_sel->setVisible(BaseObject::acceptsTablespace(obj_type));

	collation_lbl->setVisible(BaseObject::acceptsCollation(obj_type));
	collation_sel->setVisible(BaseObject::acceptsCollation(obj_type));

	show_comment=obj_type!=OBJ_RELATIONSHIP && obj_type!=OBJ_TEXTBOX && obj_type!=OBJ_PARAMETER;
	comment_lbl->setVisible(show_comment);
	comment_edt->setVisible(show_comment);

	div1_ln->setVisible(obj_type!=OBJ_TEXTBOX && obj_type!=OBJ_RELATIONSHIP && obj_type!=BASE_RELATIONSHIP);

	if(obj_type!=BASE_OBJECT)
	{
		obj_icon_lbl->setPixmap(QPixmap(Utf8String::create(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png")));
		obj_icon_lbl->setToolTip(BaseObject::getTypeName(obj_type));

		if(obj_type!=OBJ_PERMISSION)
		{
			setRequiredField(name_lbl);
			setRequiredField(name_edt);
		}

		setRequiredField(schema_lbl);
		setRequiredField(schema_sel);
	}

	if(BaseObject::acceptsCollation(obj_type))
	{
		QFrame *frame=NULL;
		map<QString, vector<QWidget *> > fields_map;
		fields_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_91)].push_back(collation_lbl);
		frame=generateVersionWarningFrame(fields_map);
		baseobject_grid->addWidget(frame, baseobject_grid->count()+1, 0, 1, 0);
		frame->setParent(this);
	}
}

QString BaseObjectWidget::generateVersionsInterval(unsigned ver_interv_id, const QString &ini_ver, const QString &end_ver)
{
	if(ver_interv_id==UNTIL_VERSION && !ini_ver.isEmpty())
		return(XMLParser::CHAR_LT + QString("= ") + ini_ver);
	else if(ver_interv_id==VERSIONS_INTERVAL && !ini_ver.isEmpty() && !end_ver.isEmpty())
		return(XMLParser::CHAR_GT + QString("= ") + ini_ver + XMLParser::CHAR_AMP + XMLParser::CHAR_LT + QString("= ") + end_ver);
	else if(ver_interv_id==AFTER_VERSION &&  !ini_ver.isEmpty())
		return(XMLParser::CHAR_GT + QString("= ") + ini_ver);
	else
		return("");
}

QFrame *BaseObjectWidget::generateInformationFrame(const QString &msg)
{
	QFrame *info_frm=NULL;
	QGridLayout *grid=NULL;
	QLabel *ico_lbl=NULL, *msg_lbl=NULL;
	QFont font;

	info_frm = new QFrame;

	font.setPointSize(8);
	font.setItalic(false);
	font.setBold(false);
	info_frm->setFont(font);

	info_frm->setObjectName(Utf8String::create("info_frm"));
	info_frm->setFrameShape(QFrame::Box);
	info_frm->setFrameShadow(QFrame::Sunken);
	info_frm->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	grid = new QGridLayout(info_frm);
	grid->setContentsMargins(4, 4, 4, 4);
	grid->setObjectName(Utf8String::create("grid"));

	ico_lbl = new QLabel(info_frm);
	ico_lbl->setObjectName(Utf8String::create("icone_lbl"));
	ico_lbl->setMinimumSize(QSize(32, 32));
	ico_lbl->setMaximumSize(QSize(32, 32));
	ico_lbl->setPixmap(QPixmap(Utf8String::create(":/icones/icones/msgbox_info.png")));
	ico_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

	grid->addWidget(ico_lbl, 0, 0, 1, 1);

	msg_lbl = new QLabel(info_frm);
	msg_lbl->setObjectName(Utf8String::create("message_lbl"));
	msg_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
	msg_lbl->setWordWrap(true);

	msg_lbl->setText(msg);

	grid->addWidget(msg_lbl, 0, 1, 1, 1);
	grid->setContentsMargins(4,4,4,4);

	return(info_frm);
}

QFrame *BaseObjectWidget::generateVersionWarningFrame(map<QString, vector<QWidget *> > &fields,
																											map< QWidget *, vector<QString> > *values)
{
	QFrame *alert_frm=NULL;
	QGridLayout *grid=NULL;
	QLabel *ico_lbl=NULL, *msg_lbl=NULL;
	QString field_name;
	QFont font;
	QWidget *wgt=NULL;
	QPalette pal;
	QColor color=QColor(0,0,128);
	map<QString, vector<QWidget *> >::iterator itr, itr_end;
	vector<QString> values_vect;
	unsigned i, count, count1, i1;

	itr=fields.begin();
	itr_end=fields.end();

	while(itr!=itr_end)
	{
		count=itr->second.size();

		for(i=0; i < count; i++)
		{
			wgt=itr->second.at(i);
			if(values && values->count(wgt) > 0)
			{
				values_vect=values->at(wgt);
				count1=values_vect.size();

				field_name+=QString("<br/>") + trUtf8("Value(s)") + QString(": (");
				for(i1=0; i1 < count1; i1++)
				{
					field_name+=values_vect.at(i1);
					if(i1 < count1-1) field_name+=", ";
				}
				field_name+=")";
			}

			font=wgt->font();

			pal.setBrush(QPalette::Active, QPalette::WindowText, color);
			wgt->setPalette(pal);

			font.setBold(true);
			font.setItalic(true);
			wgt->setFont(font);
			wgt->setToolTip(Utf8String::create("<em style='font-size: 8pt'>") + trUtf8("Version") +
											itr->first + Utf8String::create(" %1</em>").arg(field_name));
		}
		itr++;
	}


	alert_frm = new QFrame;

	font.setPointSize(8);
	font.setItalic(false);
	font.setBold(false);
	alert_frm->setFont(font);

	alert_frm->setObjectName(Utf8String::create("alerta_frm"));
	alert_frm->setFrameShape(QFrame::Box);
	alert_frm->setFrameShadow(QFrame::Sunken);
	alert_frm->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	grid = new QGridLayout(alert_frm);
	grid->setObjectName(Utf8String::create("grid"));

	ico_lbl = new QLabel(alert_frm);
	ico_lbl->setObjectName(Utf8String::create("icone_lbl"));
	ico_lbl->setMinimumSize(QSize(32, 32));
	ico_lbl->setMaximumSize(QSize(32, 32));
	ico_lbl->setPixmap(QPixmap(Utf8String::create(":/icones/icones/msgbox_alerta.png")));
	ico_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

	grid->addWidget(ico_lbl, 0, 0, 1, 1);

	msg_lbl = new QLabel(alert_frm);
	msg_lbl->setObjectName(Utf8String::create("mensagelm_lb"));
	msg_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
	msg_lbl->setWordWrap(true);

	msg_lbl->setText(trUtf8("The <em style='color: %1'><strong>highlighted</strong></em> fields on the form are available only on specific PostgreSQL versions. \
													When generating SQL code for versions other than those specified on field's tooltips pgModeler will ignore it's values.").arg(color.name()));

	grid->addWidget(msg_lbl, 0, 1, 1, 1);
	grid->setContentsMargins(4,4,4,4);

	return(alert_frm);
}

void BaseObjectWidget::editPermissions(void)
{
	BaseObject *parent_obj=NULL;

	if(this->relationship)
		parent_obj=this->relationship;

	permission_wgt->setAttributes(this->model, parent_obj, this->object);
	permission_wgt->show();
}

void BaseObjectWidget::applyConfiguration(void)
{
	if(object)
	{
		try
		{
			BaseObject *aux_obj=NULL, *aux_obj1=NULL, *parent_obj=NULL;
			bool new_obj;
			ObjectType obj_type;
			QString obj_name;

			obj_type=object->getObjectType();
			obj_name=BaseObject::formatName(name_edt->text().toUtf8(), obj_type==OBJ_OPERATOR);

			object->setSQLDisabled(disable_sql_chk->isChecked());

			if(this->object->acceptsSchema() &&  schema_sel->getSelectedObject())
				obj_name=schema_sel->getSelectedObject()->getName(true) + "." + obj_name;

			//Checking the object duplicity
			if(obj_type!=OBJ_DATABASE && obj_type!=OBJ_PERMISSION && obj_type!=OBJ_PARAMETER)
			{
				if(table)
				{
					//Validationg the object against the siblings on parent table
					parent_obj=table;
					aux_obj=table->getObject(obj_name,obj_type);
					aux_obj1=table->getObject(object->getName(),obj_type);
					new_obj=(!aux_obj && !aux_obj1);
				}
				else if(relationship)
				{
					//Validationg the object against the siblings on parent relationship
					parent_obj=relationship;
					aux_obj=relationship->getObject(obj_name,obj_type);
					aux_obj1=relationship->getObject(object->getName(),obj_type);
					new_obj=(!aux_obj && !aux_obj1);
				}
				//Validationg the object against the other objects on model
				else
				{
					parent_obj=model;
					aux_obj=model->getObject(obj_name,obj_type);

					if(obj_type==OBJ_FUNCTION)
						aux_obj1=model->getObject(dynamic_cast<Function *>(object)->getSignature(),obj_type);
					else if(obj_type==OBJ_OPERATOR)
						aux_obj1=model->getObject(dynamic_cast<Operator *>(object)->getSignature(),obj_type);
					else
						aux_obj1=model->getObject(object->getName(),obj_type);

					new_obj=(!aux_obj && !aux_obj1);
				}

				//Raises an error if another object is found with the same name as the editing object
				if(!new_obj && aux_obj && aux_obj!=object)
				{
					throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
													.arg(Utf8String::create(obj_name))
													.arg(Utf8String::create(BaseObject::getTypeName(obj_type)))
													.arg(Utf8String::create(parent_obj->getName(true)))
													.arg(Utf8String::create(parent_obj->getTypeName())),
													ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

			//Renames the object (only cast object aren't renamed)
			if(obj_type!=OBJ_CAST)
			{
				prev_name=object->getName();
				object->setName(name_edt->text().toUtf8());
			}

			//Sets the object's comment
			if(comment_edt->isVisible())
				object->setComment(comment_edt->text().toUtf8());

			//Sets the object's tablespace
			if(tablespace_sel->isVisible())
				object->setTablespace(tablespace_sel->getSelectedObject());

			//Sets the object's owner
			if(owner_sel->isVisible())
				object->setOwner(owner_sel->getSelectedObject());

			//Sets the object's collation
			if(collation_sel->isVisible())
				object->setCollation(collation_sel->getSelectedObject());

			//Sets the object's schema
			if(schema_sel->isVisible())
			{
				Schema *esquema=dynamic_cast<Schema *>(schema_sel->getSelectedObject());
				this->prev_schema=dynamic_cast<Schema *>(object->getSchema());
				object->setSchema(esquema);
			}
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void BaseObjectWidget::finishConfiguration(void)
{
	if(this->object)
	{
		ObjectType obj_type=this->object->getObjectType();
		BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(this->object);
		TableObject *tab_obj=dynamic_cast<TableObject *>(this->object);

		if(new_object)
		{
			//If the object is a table object and the parent table is specified, adds it to table
			if(table && (obj_type==OBJ_COLUMN || obj_type==OBJ_RULE ||
									 obj_type==OBJ_TRIGGER ||
									 obj_type==OBJ_INDEX || obj_type==OBJ_CONSTRAINT))
				table->addObject(this->object);
			//Adding the object on the relationship, if specified
			else if(relationship && (obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT))
				relationship->addObject(dynamic_cast<TableObject *>(this->object));
			//Adding the object on the model
			else if(obj_type!=OBJ_PARAMETER)
				model->addObject(this->object);

			if(op_list)
			{
				//If the object is a new one is necessary register it on the operation list
				if(this->table)
					op_list->registerObject(this->object, Operation::OBJECT_CREATED, -1, this->table);
				else if(obj_type!=OBJ_RELATIONSHIP && obj_type!=OBJ_TABLE)
					op_list->registerObject(this->object, Operation::OBJECT_CREATED, -1, this->relationship);
			}
			new_object=false;
		}
		else
		{
			//If the object is being updated, validates its SQL definition
			if(obj_type==BASE_RELATIONSHIP || obj_type==OBJ_TEXTBOX)
				this->object->getCodeDefinition(SchemaParser::SQL_DEFINITION);
			else
				this->object->getCodeDefinition(SchemaParser::XML_DEFINITION);
		}

		this->accept();
		parent_form->hide();

		//If the object is graphical (or a table object), updates it (or its parent) on the scene
		if(graph_obj || tab_obj)
		{
			if(!graph_obj && tab_obj && tab_obj->getObjectType()!=OBJ_PARAMETER)
			{
				if(this->table)
					graph_obj=dynamic_cast<BaseGraphicObject *>(this->table);
				else
					graph_obj=dynamic_cast<BaseGraphicObject *>(this->relationship);

				graph_obj->setModified(true);
			}
			else if(graph_obj)
			{
				if(!isnan(object_px) && !isnan(object_py))
					graph_obj->setPosition(QPointF(object_px, object_py));

				graph_obj->setModified(true);
			}

			//Updates the visual schemas when the objects is moved to another
			if(object->getSchema())
				dynamic_cast<Schema *>(object->getSchema())->setModified(true);

			if(prev_schema && object->getSchema()!=prev_schema)
				prev_schema->setModified(true);
		}

		emit s_objectManipulated();
	}
}

void BaseObjectWidget::cancelConfiguration(void)
{
	ObjectType obj_type;

	obj_type=this->object->getObjectType();

	if(new_object)
	{
		TableObject *tab_obj=dynamic_cast<TableObject *>(this->object);

		//Removes the object from its parent
		if(!table && !tab_obj && model->getObjectIndex(this->object) >= 0)
			model->removeObject(this->object);
		else if(table && table->getObjectIndex(tab_obj) >= 0)
			table->removeObject(tab_obj);
		else if(relationship && relationship->getObjectIndex(tab_obj) >= 0)
			relationship->removeObject(tab_obj);

		//Deallocate the object if it isn't a table or relationship
		if(obj_type!=OBJ_TABLE &&
			 obj_type!=OBJ_VIEW &&
			 obj_type!=OBJ_RELATIONSHIP)
		{
			delete(this->object);
			this->object=NULL;
		}

		if(op_list)
			op_list->removeLastOperation();
	}

	//If the object is not a new one, restore its previous state
	if(!new_object &&
		 op_list && obj_type!=OBJ_DATABASE && obj_type!=OBJ_PERMISSION)
	{
		try
		{
			op_list->undoOperation();
			op_list->removeLastOperation();
		}
		catch(Exception &e)
		{}
	}

	emit s_objectManipulated();
}
