/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

ObjectSelectorWidget::ObjectSelectorWidget(ObjectType sel_obj_type, bool install_highlighter, QWidget *parent) : QWidget(parent)
{
	try
	{
		this->sel_obj_types.push_back(sel_obj_type);
		configureSelector(install_highlighter);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
			obj_name_hl=new SyntaxHighlighter(obj_name_txt, true);
			obj_name_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());
		}
		else
		{
			//Adding a custom height to the input if the highlighter isn't installed
			QFontMetrics fm=obj_name_txt->fontMetrics();
			obj_name_txt->setMaximumHeight(fm.height() + (fm.lineSpacing()/1.8));
			this->adjustSize();
		}

		connect(sel_object_tb, SIGNAL(clicked(bool)), this, SLOT(showObjectView()));
		connect(rem_object_tb, SIGNAL(clicked(bool)), this, SLOT(clearSelector()));
		connect(obj_view_wgt, SIGNAL(s_visibilityChanged(BaseObject*,bool)), this, SLOT(showSelectedObject(BaseObject*, bool)));

		obj_name_txt->installEventFilter(this);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

bool ObjectSelectorWidget::eventFilter(QObject *obj, QEvent *evnt)
{
	if(this->isEnabled() && evnt->type()==QEvent::FocusIn &&
		 QApplication::mouseButtons()==Qt::LeftButton && obj==obj_name_txt)
	{
		QFocusEvent *focus_evnt = dynamic_cast<QFocusEvent *>(evnt);

		if(focus_evnt->reason() == Qt::MouseFocusReason)
		{
			showObjectView();
			return true;
		}
	}

	return QWidget::eventFilter(obj, evnt);
}

ObjectSelectorWidget::~ObjectSelectorWidget()
{
	delete obj_view_wgt;
}

void ObjectSelectorWidget::enableObjectCreation(bool value)
{
	obj_view_wgt->enableObjectCreation(value);
}

BaseObject *ObjectSelectorWidget::getSelectedObject()
{
	return selected_obj;
}

QString ObjectSelectorWidget::getSelectedObjectName()
{
	return selected_obj->getSignature();
}

void ObjectSelectorWidget::setSelectedObject(BaseObject *object)
{
	ObjectType obj_type = ObjectType::BaseObject;

	if(object)
		obj_type=object->getObjectType();

	if(object && std::find(sel_obj_types.begin(), sel_obj_types.end(),obj_type)!=sel_obj_types.end())
	{
		rem_object_tb->setEnabled(object);
		this->selected_obj=object;

		if(obj_type != ObjectType::Constraint)
		{
			if(obj_type != ObjectType::UserMapping)
				obj_name_txt->setPlainText(selected_obj->getSignature());
			else
				obj_name_txt->setPlainText(selected_obj->getName());
		}
		else
			obj_name_txt->setPlainText(QString("%1.%2")
									   .arg(dynamic_cast<TableObject *>(selected_obj)->getParentTable()->getSignature())
									   .arg(selected_obj->getName(true)));

		emit s_objectSelected();
		emit s_selectorChanged(true);
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
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void ObjectSelectorWidget::setModel(DatabaseModel *modelo)
{
	this->model=modelo;
}

void ObjectSelectorWidget::showSelectedObject(BaseObject *obj_sel, bool)
{
	if(obj_sel)
		setSelectedObject(obj_sel);
}

void ObjectSelectorWidget::clearSelector()
{
	this->selected_obj=nullptr;
	obj_name_txt->clear();
	rem_object_tb->setEnabled(false);
	emit s_selectorCleared();
	emit s_selectorChanged(false);
}

void ObjectSelectorWidget::showObjectView()
{
	obj_name_txt->clearFocus();

	for(unsigned i=0; i < sel_obj_types.size(); i++)
		obj_view_wgt->setObjectVisible(sel_obj_types[i], true);

	if(sel_obj_types.size()==1)
		obj_view_wgt->setWindowTitle(tr("Select %1").arg(BaseObject::getTypeName(sel_obj_types[0]).toLower()));

	obj_view_wgt->setModel(this->model);
	obj_view_wgt->show();
}

