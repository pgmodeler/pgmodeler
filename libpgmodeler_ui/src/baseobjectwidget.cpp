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

#include "baseobjectwidget.h"
#include "permissionwidget.h"
#include "customsqlwidget.h"
#include "pgmodeleruins.h"

const QColor BaseObjectWidget::PROT_LINE_BGCOLOR=QColor(255,180,180);
const QColor BaseObjectWidget::PROT_LINE_FGCOLOR=QColor(80,80,80);
const QColor BaseObjectWidget::RELINC_LINE_BGCOLOR=QColor(164,249,176);
const QColor BaseObjectWidget::RELINC_LINE_FGCOLOR=QColor(80,80,80);

BaseObjectWidget::BaseObjectWidget(QWidget *parent, ObjectType obj_type): QDialog(parent)
{
	try
	{
		QHBoxLayout *layout=nullptr;
		QSpacerItem *spacer=nullptr;

    setupUi(this);
    new_object=false;
		model=nullptr;
		table=nullptr;
		relationship=nullptr;
		prev_schema=nullptr;
		op_list=nullptr;
		object=nullptr;
		object_px=NAN;
		object_py=NAN;
		pf_min_height=-1;
		pf_max_height=-1;
		parent_form=nullptr;
		schema_sel=nullptr;
		owner_sel=nullptr;
		tablespace_sel=nullptr;

		parent_form=new BaseForm(nullptr, (Qt::WindowTitleHint | Qt::WindowSystemMenuHint));
		parent_form->setWindowTitle(trUtf8("Create / Edit: ") + BaseObject::getTypeName(obj_type));
		parent_form->generalwidget_wgt->insertWidget(0, this);
		parent_form->generalwidget_wgt->setCurrentIndex(0);
		parent_form->setButtonConfiguration(Messagebox::OK_CANCEL_BUTTONS);
    parent_form->setObjectName(QString("parent_form"));

    connect(edt_perms_tb, SIGNAL(clicked(bool)),this, SLOT(editPermissions(void)));
    connect(append_sql_tb, SIGNAL(clicked(bool)),this, SLOT(appendSQL(void)));
    connect(parent_form->cancel_btn, SIGNAL(clicked(bool)), parent_form, SLOT(reject(void)));
    connect(parent_form, SIGNAL(rejected()), this, SLOT(reject()));

    schema_sel=new ObjectSelectorWidget(OBJ_SCHEMA, true, this);
    collation_sel=new ObjectSelectorWidget(OBJ_COLLATION, true, this);
    tablespace_sel=new ObjectSelectorWidget(OBJ_TABLESPACE, true, this);
    owner_sel=new ObjectSelectorWidget(OBJ_ROLE, true, this);

		baseobject_grid = new QGridLayout;
    baseobject_grid->setObjectName("objetobase_grid");
		baseobject_grid->addWidget(protected_obj_frm, 0, 0, 1, 0);
		baseobject_grid->addWidget(name_lbl, 1, 0, 1, 1);
		baseobject_grid->addWidget(name_edt, 1, 1, 1, 1);
		baseobject_grid->addWidget(id_ico_wgt, 1, 2, 1, 3);
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
		layout->addWidget(append_sql_tb);
		layout->addWidget(edt_perms_tb);
		layout->addWidget(disable_sql_chk);

		baseobject_grid->addLayout(layout,9,0,1,5);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

BaseObjectWidget::~BaseObjectWidget(void)
{
	if(parent_form)
	{
		parent_form->generalwidget_wgt->removeWidget(this);
		this->setParent(nullptr);
		delete(parent_form);
		parent_form=nullptr;
	}
}

bool BaseObjectWidget::eventFilter(QObject *object, QEvent *event)
{
	//Filters the ENTER/RETURN pressing forcing the parent form activate the "Apply" button
  if(event->type() == QEvent::KeyPress)
	{
    QKeyEvent *kevent=dynamic_cast<QKeyEvent *>(event);

    if(kevent->key()==Qt::Key_Return || kevent->key()==Qt::Key_Enter)
    {
      parent_form->apply_ok_btn->click();
      return(true);
    }
	}

  return(QWidget::eventFilter(object, event));
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
    int max_h=(pf_max_height + protected_obj_frm->height() + 10);
    if(max_h > MAX_OBJECT_SIZE) max_h=MAX_OBJECT_SIZE;

		parent_form->setMinimumHeight(pf_min_height + protected_obj_frm->height() + 10);       
    parent_form->setMaximumHeight(max_h);
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
    QString str_aux=QString(" <span style='color: #ff0000;'>*</span> ");
    QColor bgcolor=QColor(QString("#ffffc0"));

		QFont fnt=widget->font();

		if(lbl || pgtype || grp)
		{
			fnt.setBold(true);

			if(lbl)
				lbl->setText(str_aux + lbl->text());

			if(!grp)
				widget->setFont(fnt);
			else
        grp->setStyleSheet(QString("QGroupBox {	font-weight: bold; }"));
		}
		else if(edt || txt || sel)
		{
			QPalette pal;
			pal.setColor(QPalette::Base, bgcolor);

			if(sel)
				widget=sel->obj_name_txt;
			widget->setPalette(pal);
		}

    str_aux=(!widget->toolTip().isEmpty() ? QString("\n") : QString());
		widget->setToolTip(widget->toolTip() + str_aux + trUtf8("Required field. Leaving this empty will raise errors!"));
	}
}

void BaseObjectWidget::setAttributes(DatabaseModel *model, BaseObject *object, BaseObject *parent_obj)
{
	setAttributes(model, nullptr, object, parent_obj, NAN, NAN, false);
}

void BaseObjectWidget::disableReferencesSQL(BaseObject *object)
{
	vector<BaseObject *> refs;
	TableObject *tab_obj=nullptr;

	model->getObjectReferences(object, refs);

	while(!refs.empty())
	{
		tab_obj=dynamic_cast<TableObject *>(refs.back());

		//If the object is a relationship added does not do anything since the relationship itself will be disabled
		if(!tab_obj || (tab_obj && !tab_obj->isAddedByRelationship()))
		{
			refs.back()->setSQLDisabled(disable_sql_chk->isChecked());

			//Update the parent table graphical representation to show the disabled child object
			if(tab_obj)
				tab_obj->getParentTable()->setModified(true);

			//Disable the references of the current object too
			disableReferencesSQL(refs.back());
		}

		refs.pop_back();
  }
}

void BaseObjectWidget::configureTabOrder(vector<QWidget *> widgets)
{
  ObjectSelectorWidget *obj_sel=nullptr;
  PgSQLTypeWidget *type_wgt=nullptr;
  vector<QWidget *> children, tab_order;
  int idx=0, cnt=0;

 widgets.insert(widgets.begin(),
                { name_edt, schema_sel , collation_sel, owner_sel, tablespace_sel,
                  comment_edt, append_sql_tb, edt_perms_tb, disable_sql_chk });

  for(auto &wgt : widgets)
  {
    wgt->setFocusPolicy(Qt::StrongFocus);

    obj_sel=dynamic_cast<ObjectSelectorWidget *>(wgt);
    type_wgt=dynamic_cast<PgSQLTypeWidget *>(wgt);

    if(obj_sel)
      children={ obj_sel->rem_object_tb, obj_sel->sel_object_tb };
    else if(type_wgt)
    {
      children={ type_wgt->type_cmb, type_wgt->length_sb, type_wgt->precision_sb,
                 type_wgt->dimension_sb, type_wgt->interval_cmb, type_wgt->spatial_cmb,
                 type_wgt->srid_spb, type_wgt->var_z_chk, type_wgt->var_m_chk,
                 type_wgt->timezone_chk };
    }

    tab_order.push_back(wgt);

    for(auto &child : children)
    {
      child->setFocusPolicy(Qt::StrongFocus);
      tab_order.push_back(child);
    }
  }

  cnt=tab_order.size()-1;

  for(idx=0; idx < cnt; idx++)
   QWidget::setTabOrder(tab_order[idx], tab_order[idx+1]);
}

void BaseObjectWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseObject *object, BaseObject *parent_obj, double obj_px, double obj_py, bool uses_op_list)
{
	ObjectType obj_type, parent_type=BASE_OBJECT;

	/* Reseting the objects attributes in order to force them to be redefined
	every time this method is called */
	this->object=nullptr;
	this->model=nullptr;
	this->op_list=nullptr;
	this->relationship=nullptr;
	this->table=nullptr;

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
	edt_perms_tb->setEnabled(object!=nullptr);
	append_sql_tb->setEnabled(object!=nullptr);

	owner_sel->setModel(model);
  owner_sel->setSelectedObject(model->getDefaultObject(OBJ_ROLE));

	schema_sel->setModel(model);
  schema_sel->setSelectedObject(model->getDefaultObject(OBJ_SCHEMA));

	tablespace_sel->setModel(model);
  tablespace_sel->setSelectedObject(model->getDefaultObject(OBJ_TABLESPACE));

	collation_sel->setModel(model);
  collation_sel->setSelectedObject(model->getDefaultObject(OBJ_COLLATION));

	if(object)
	{
		bool prot;

		obj_id_lbl->setVisible(true);
		obj_id_lbl->setText(QString("ID: %1").arg(object->getObjectId()));
    name_edt->setText(object->getName());
    comment_edt->setText(object->getComment());

    /* When creating a new table or relationship the object is pre allocated and the flag new_object is set.
       In order to avoid the selectors to have empty values, we check if the flag is false which means
       that the object is not new at all */
    if(!new_object)
    {
      schema_sel->setSelectedObject(object->getSchema());
      tablespace_sel->setSelectedObject(object->getTablespace());
      owner_sel->setSelectedObject(object->getOwner());
      collation_sel->setSelectedObject(object->getCollation());
    }

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
		obj_id_lbl->setVisible(false);
		protected_obj_frm->setVisible(false);

    if(parent_obj && parent_obj->getObjectType()==OBJ_SCHEMA)
			schema_sel->setSelectedObject(parent_obj);
	}
}

void BaseObjectWidget::configureFormLayout(QGridLayout *grid, ObjectType obj_type)
{
	bool show_comment;
	QObjectList chld_list;
	QWidget *wgt=nullptr;


	if(grid)
	{
		QLayoutItem *item=nullptr;
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
  disable_sql_chk->setVisible(obj_type!=BASE_OBJECT && obj_type!=OBJ_PERMISSION && obj_type!=OBJ_TEXTBOX && obj_type!=OBJ_TAG);

	edt_perms_tb->setVisible(Permission::objectAcceptsPermission(obj_type));
	append_sql_tb->setVisible(BaseObject::acceptsCustomSQL(obj_type));

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

	if(obj_type!=BASE_OBJECT)
	{
    obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png")));
		obj_icon_lbl->setToolTip(BaseObject::getTypeName(obj_type));

		if(obj_type!=OBJ_PERMISSION && obj_type!=OBJ_CAST)
		{
			setRequiredField(name_lbl);
			setRequiredField(name_edt);
		}

		setRequiredField(schema_lbl);
		setRequiredField(schema_sel);
	}

	if(BaseObject::acceptsCollation(obj_type))
	{
		QFrame *frame=nullptr;
		map<QString, vector<QWidget *> > fields_map;
    fields_map[generateVersionsInterval(AFTER_VERSION, PgSQLVersions::PGSQL_VERSION_91)].push_back(collation_lbl);
		frame=generateVersionWarningFrame(fields_map);
		baseobject_grid->addWidget(frame, baseobject_grid->count()+1, 0, 1, 0);
		frame->setParent(this);
	}

	//Install the event filter into all children object in order to capture key press
	chld_list=this->children();
	while(!chld_list.isEmpty())
	{
		wgt=dynamic_cast<QWidget *>(chld_list.front());
		if(wgt)
			wgt->installEventFilter(this);

		chld_list.pop_front();
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
    return(QString());
}

QFrame *BaseObjectWidget::generateInformationFrame(const QString &msg)
{
	QFrame *info_frm=nullptr;
	QGridLayout *grid=nullptr;
	QLabel *ico_lbl=nullptr, *msg_lbl=nullptr;
	QFont font;

	info_frm = new QFrame;

	font.setPointSize(8);
	font.setItalic(false);
	font.setBold(false);
	info_frm->setFont(font);

  info_frm->setObjectName("info_frm");
  info_frm->setFrameShape(QFrame::StyledPanel);
	info_frm->setFrameShadow(QFrame::Raised);
	info_frm->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	grid = new QGridLayout(info_frm);
	grid->setContentsMargins(4, 4, 4, 4);
  grid->setObjectName("grid");

	ico_lbl = new QLabel(info_frm);
  ico_lbl->setObjectName("icone_lbl");
	ico_lbl->setMinimumSize(QSize(24, 24));
	ico_lbl->setMaximumSize(QSize(24, 24));
	ico_lbl->setScaledContents(true);
  ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));
	ico_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

	grid->addWidget(ico_lbl, 0, 0, 1, 1);

	msg_lbl = new QLabel(info_frm);
	msg_lbl->setFont(font);
  msg_lbl->setObjectName("message_lbl");
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
	QFrame *alert_frm=nullptr;
	QGridLayout *grid=nullptr;
	QLabel *ico_lbl=nullptr, *msg_lbl=nullptr;
	QString field_name;
	QFont font;
	QWidget *wgt=nullptr;
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
      wgt->setToolTip(QString("<em style='font-size: 8pt'>") + trUtf8("Version") +
                      itr->first + QString(" %1</em>").arg(field_name));
		}
		itr++;
	}


	alert_frm = new QFrame;

	font.setPointSize(8);
	font.setItalic(false);
	font.setBold(false);
	alert_frm->setFont(font);

  alert_frm->setObjectName("alerta_frm");
  alert_frm->setFrameShape(QFrame::StyledPanel);
	alert_frm->setFrameShadow(QFrame::Raised);
	alert_frm->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	grid = new QGridLayout(alert_frm);
  grid->setObjectName("grid");

	ico_lbl = new QLabel(alert_frm);
  ico_lbl->setObjectName("icone_lbl");
	ico_lbl->setMinimumSize(QSize(24, 24));
	ico_lbl->setMaximumSize(QSize(24, 24));
	ico_lbl->setScaledContents(true);
  ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_alerta.png")));
	ico_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

	grid->addWidget(ico_lbl, 0, 0, 1, 1);

	msg_lbl = new QLabel(alert_frm);
	msg_lbl->setFont(font);
  msg_lbl->setObjectName("mensagelm_lb");
	msg_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
	msg_lbl->setWordWrap(true);

	msg_lbl->setText(trUtf8("The <em style='color: %1'><strong>highlighted</strong></em> fields on the form are available only on specific PostgreSQL versions. \
													When generating SQL code for versions other than those specified on fields' tooltips pgModeler will ignore their values.").arg(color.name()));

	grid->addWidget(msg_lbl, 0, 1, 1, 1);
	grid->setContentsMargins(4,4,4,4);

	return(alert_frm);
}

void BaseObjectWidget::editPermissions(void)
{
	PermissionWidget permission_wgt(this);
	BaseObject *parent_obj=nullptr;

	if(this->relationship)
		parent_obj=this->relationship;

	permission_wgt.setAttributes(this->model, parent_obj, this->object);
	permission_wgt.show();
}

void BaseObjectWidget::appendSQL(void)
{
	CustomSQLWidget customsql_wgt;
	customsql_wgt.setAttributes(this->model, this->object);
	customsql_wgt.show();
}

void BaseObjectWidget::applyConfiguration(void)
{
	if(object)
	{
		try
		{
			BaseObject *aux_obj=nullptr, *aux_obj1=nullptr, *parent_obj=nullptr;
			bool new_obj;
      ObjectType obj_type=object->getObjectType();
			QString obj_name;

      obj_name=BaseObject::formatName(name_edt->text().toUtf8(), obj_type==OBJ_OPERATOR);

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

					/* Special case for tables an views. Its necessary to make an additional
					checking on table list when the configured object is a view or a checking
					on view list when the configured object is a table, this because PostgreSQL
					does not accepts tables and views have the same name on the same schema */
					if(!aux_obj && obj_type==OBJ_TABLE)
						aux_obj=model->getObject(obj_name, OBJ_VIEW);
					else if(!aux_obj && obj_type==OBJ_VIEW)
						aux_obj=model->getObject(obj_name, OBJ_TABLE);

          aux_obj1=model->getObject(object->getSignature(), obj_type);
					new_obj=(!aux_obj && !aux_obj1);
				}

				//Raises an error if another object is found with the same name as the editing object
				if(!new_obj && aux_obj && aux_obj!=object)
				{
					throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
                          .arg(obj_name)
                          .arg(BaseObject::getTypeName(obj_type))
                          .arg(parent_obj->getName(true))
                          .arg(parent_obj->getTypeName()),
													ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

			//Renames the object (only cast object aren't renamed)
			if(obj_type!=OBJ_CAST)
			{
				prev_name=object->getName();
				object->setName(name_edt->text().trimmed().toUtf8());
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

      if(!object->isProtected() && !object->isSystemObject())
        PgModelerUiNS::disableObjectSQL(object, disable_sql_chk->isChecked());
		}
		catch(Exception &e)
		{     
      throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void BaseObjectWidget::finishConfiguration(void)
{
  try
  {
    if(this->object)
    {
      ObjectType obj_type=this->object->getObjectType();
      BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(this->object);
      TableObject *tab_obj=dynamic_cast<TableObject *>(this->object);
      vector<BaseObject *> ref_objs;

      if(new_object)
      {
        //If the object is a table object and the parent table is specified, adds it to table
        if(table && TableObject::isTableObject(obj_type))
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
        if(obj_type==BASE_RELATIONSHIP || obj_type==OBJ_TEXTBOX || obj_type==OBJ_TAG)
          this->object->getCodeDefinition(SchemaParser::XML_DEFINITION);
        else
          this->object->getCodeDefinition(SchemaParser::SQL_DEFINITION);
      }


      model->getObjectReferences(object, ref_objs);
      for(auto &obj : ref_objs)
      {
        obj->setCodeInvalidated(true);

        if(obj->getObjectType()==OBJ_COLUMN)
          dynamic_cast<Column *>(obj)->getParentTable()->setModified(true);
      }

      object->setCodeInvalidated(true);
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
          graph_obj->setCodeInvalidated(true);
        }
        else if(graph_obj)
        {
          if(!std::isnan(object_px) && !std::isnan(object_py))
            graph_obj->setPosition(QPointF(object_px, object_py));

          graph_obj->setModified(true);
        }

        /* Updates the visual schemas when the objects is moved to another or a
        table object is added to a table */
        if(object->getSchema())
          dynamic_cast<Schema *>(object->getSchema())->setModified(true);
        else if(tab_obj && tab_obj->getParentTable() &&
                 tab_obj->getParentTable()->getSchema())
          dynamic_cast<Schema *>(tab_obj->getParentTable() ->getSchema())->setModified(true);

        if(prev_schema && object->getSchema()!=prev_schema)
          prev_schema->setModified(true);
      }

      emit s_objectManipulated();
    }
  }
  catch(Exception &e)
  {
    if(e.getErrorType()==ERR_ASG_OBJ_INV_DEFINITION)
      throw Exception(Exception::getErrorMessage(ERR_REQ_FIELDS_NOT_FILLED)
                      .arg(this->object->getName()).arg(this->object->getTypeName()),
                      ERR_REQ_FIELDS_NOT_FILLED,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
    else
      throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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

    if(obj_type!=OBJ_TABLE &&
			 obj_type!=OBJ_VIEW &&
			 obj_type!=OBJ_RELATIONSHIP)
		{
      //delete(this->object);
			this->object=nullptr;
    }

    //this->object=nullptr;

		if(op_list)
    {
      op_list->ignoreOperationChain(true);
			op_list->removeLastOperation();
      op_list->ignoreOperationChain(false);
    }
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
    catch(Exception &)
		{}
	}

	emit s_objectManipulated();
}
