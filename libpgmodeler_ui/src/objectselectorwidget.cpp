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
			obj_name_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																			GlobalAttributes::DIR_SEPARATOR +
																			GlobalAttributes::SQL_HIGHLIGHT_CONF +
																			GlobalAttributes::CONFIGURATION_EXT);
		}

		connect(sel_object_tb, SIGNAL(clicked(bool)), this, SLOT(showObjectView(void)));
		connect(rem_object_tb, SIGNAL(clicked(bool)), this, SLOT(clearSelector(void)));
		connect(obj_view_wgt, SIGNAL(s_visibilityChanged(BaseObject*,bool)), this, SLOT(showSelectedObject(BaseObject*, bool)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

ObjectSelectorWidget::~ObjectSelectorWidget(void)
{
	delete(obj_view_wgt);
}

BaseObject *ObjectSelectorWidget::getSelectedObject(void)
{
	return(selected_obj);
}

void ObjectSelectorWidget::setSelectedObject(BaseObject *object)
{
	ObjectType obj_type;
	QString obj_name;

	if(object)
	{
		obj_type=object->getObjectType();

		if(obj_type==OBJ_FUNCTION)
			obj_name=dynamic_cast<Function *>(object)->getSignature();
		else if(obj_type==OBJ_OPERATOR)
			obj_name=dynamic_cast<Operator *>(object)->getSignature();
		else if(TableObject::isTableObject(obj_type))
		{
			BaseObject *tab_pai=dynamic_cast<TableObject *>(object)->getParentTable();
			if(tab_pai)
				obj_name+=tab_pai->getName(true) + ".";

			obj_name+=object->getName();
		}
		else
			obj_name=object->getName(true);
	}

	if(object && std::find(sel_obj_types.begin(), sel_obj_types.end(),obj_type)!=sel_obj_types.end())
	{
		obj_name_txt->setPlainText(Utf8String::create(obj_name));
		rem_object_tb->setEnabled(object);
		this->selected_obj=object;
		emit s_objectSelected();
	}
	else
		clearSelector();
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

