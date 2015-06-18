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

#include "objectselectorwidget.h"

QObject *ObjectSelectorWidget::current_selector=nullptr;

ObjectSelectorWidget::ObjectSelectorWidget(ObjectType sel_obj_type, bool install_highlighter, QWidget *parent) : QWidget(parent)
{
	try
	{
		this->sel_obj_types.push_back(sel_obj_type);
    configureSelector(install_highlighter);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

ObjectSelectorWidget::ObjectSelectorWidget(vector<ObjectType> sel_obj_types, bool install_highlighter, QWidget * parent) : QWidget(parent)
{
	try
	{
		this->sel_obj_types=sel_obj_types;
		configureSelector(install_highlighter);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ObjectSelectorWidget::configureSelector(bool install_highlighter)
{
	try
	{
		Ui_ObjectSelectorWidget::setupUi(this);
		obj_view_wgt=new ModelObjectsWidget(true);

		model=nullptr;
		selected_obj=nullptr;
		obj_name_hl=nullptr;

		if(install_highlighter)
		{
			obj_name_hl=new SyntaxHighlighter(obj_name_txt, false);
      obj_name_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);
		}

    connect(sel_object_tb, SIGNAL(clicked(bool)), this, SLOT(showObjectView(void)));
		connect(rem_object_tb, SIGNAL(clicked(bool)), this, SLOT(clearSelector(void)));
		connect(obj_view_wgt, SIGNAL(s_visibilityChanged(BaseObject*,bool)), this, SLOT(showSelectedObject(BaseObject*, bool)));

		obj_name_txt->installEventFilter(this);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

bool ObjectSelectorWidget::eventFilter(QObject *obj, QEvent *evnt)
{
  if(this->isEnabled() && evnt->type()==QEvent::MouseButtonPress && obj==obj_name_txt)
		showObjectView();

	return(QWidget::eventFilter(obj, evnt));
}

ObjectSelectorWidget::~ObjectSelectorWidget(void)
{
	delete(obj_view_wgt);
}

BaseObject *ObjectSelectorWidget::getSelectedObject(void)
{
  return(selected_obj);
}

QString ObjectSelectorWidget::getSelectedObjectName(void)
{
  return(selected_obj->getSignature());
}

void ObjectSelectorWidget::setSelectedObject(BaseObject *object)
{
	ObjectType obj_type;

	if(object)
		obj_type=object->getObjectType();

	if(object && std::find(sel_obj_types.begin(), sel_obj_types.end(),obj_type)!=sel_obj_types.end())
	{   
		rem_object_tb->setEnabled(object);
		this->selected_obj=object;

    if(object->getObjectType()!=OBJ_CONSTRAINT)
      obj_name_txt->setPlainText(selected_obj->getSignature());
    else
      obj_name_txt->setPlainText(QString("%1.%2")
                                 .arg(dynamic_cast<TableObject *>(selected_obj)->getParentTable()->getSignature())
                                 .arg(selected_obj->getName(true)));

		emit s_objectSelected();
	}
	else
    clearSelector();
}

void ObjectSelectorWidget::setSelectedObject(const QString &obj_name, ObjectType obj_type)
{
  try
  {
    BaseObject *object=nullptr;

    if(model && std::find(sel_obj_types.begin(), sel_obj_types.end(),obj_type)!=sel_obj_types.end())
      object=model->getObject(obj_name, obj_type);

    setSelectedObject(object);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void ObjectSelectorWidget::setModel(DatabaseModel *modelo)
{
	this->model=modelo;
}

void ObjectSelectorWidget::showSelectedObject(BaseObject *obj_sel, bool)
{
	if(ObjectSelectorWidget::current_selector==this && obj_sel)
		setSelectedObject(obj_sel);
}

void ObjectSelectorWidget::clearSelector(void)
{
	this->selected_obj=nullptr;
	obj_name_txt->clear();
	rem_object_tb->setEnabled(false);
	emit s_selectorCleared();
}

void ObjectSelectorWidget::showObjectView(void)
{
	ObjectSelectorWidget::current_selector=this;

	for(unsigned i=0; i < sel_obj_types.size(); i++)
		obj_view_wgt->setObjectVisible(sel_obj_types[i], true);

	obj_view_wgt->setModel(this->model);
	obj_view_wgt->show();
}

