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

/**
\ingroup libpgmodeler_ui
\class BaseObjectWidget
\brief Implements the basic operations to create/edit database objects via form.
*/

#ifndef BASE_OBJECT_WIDGET_H
#define BASE_OBJECT_WIDGET_H

#include <QtWidgets>
#include "databasemodel.h"
#include "operationlist.h"
#include "baseform.h"
#include "modelobjectswidget.h"
#include "objectselectorwidget.h"
#include "ui_baseobjectwidget.h"
#include "pgsqltypewidget.h"

/* Declaring the PgSQLType class as a Qt metatype in order to permit
	 that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(PgSQLType)

class BaseObjectWidget: public QDialog, public Ui::BaseObjectWidget {
	private:
		Q_OBJECT

		/*! \brief Stores the minimum and maximum height of the parent form, in order
			to control the exhibition of the alert frame when the object is protected */
		int pf_min_height, pf_max_height;

	protected:
		ModelObjectsWidget *object_selection_wgt;

		static const QColor PROT_LINE_BGCOLOR,
												PROT_LINE_FGCOLOR,
												RELINC_LINE_BGCOLOR,
												RELINC_LINE_FGCOLOR;

		//! \brief Parent form used to show the widget as a dialog.
		BaseForm *parent_form;

		//! \brief Reference database model
		DatabaseModel *model;

		//! \brief Reference table/view (used only when editing table objects)
		BaseTable *table;

		//! \brief Stores the object previous name (used to validate schema renaming)
		QString prev_name;

		//! \brief Store the object previous schema (used to update the schemas when moving tables/views from one to another)
		Schema *prev_schema;

		//! \brief Reference relationship (used only when editing relationship attributes)
		Relationship *relationship;

		//! \brief Reference operation list where all modifications in the form are registered
		OperationList *op_list;

		//! \brief Object that is being edited / created
		BaseObject *object;

		/*! \brief Stores the object position (generally the mouse position) when the dialog was called
			(used only when creating graphical objects) */
		float object_px, object_py;

		//! \brief Grid layout used to organize the widgets over the form
		QGridLayout *baseobject_grid;

		//! \brief Indicates if the object is a new one or is being edited
		bool new_object;

		//! \brief Object selectors for schema, owner, tablespace and collation
		ObjectSelectorWidget *schema_sel,
		*owner_sel,
		*tablespace_sel,
		*collation_sel;

		/*! \brief Merges the specified grid layout with the 'baseobject_grid' creating a single form.
			The obj_type parameter must be specified to show the object type icon */
		void configureFormLayout(QGridLayout *grid=nullptr, ObjectType obj_type=BASE_OBJECT);

		/*! \brief Starts a object configuration, alocating a new one if necessary, registering
			the object on the operation list. This method doens't applies to database model edition */
		template<class Class>
		void startConfiguration(void);

		/*! \brief Finishes the edition / creation of object, registering it on the operation list
			and inserts is on the parent object */
		void finishConfiguration(void);

		/*! \brief Aborts the object configuration, deallocation it if necessary or restoring it to
			its previous configuration */
		virtual void cancelConfiguration(void);

		//! \brief Apply the basic configurations to the object (name, schema, comment, owner, tablespace)
		virtual void applyConfiguration(void);

		void hideEvent(QHideEvent *);
		void showEvent(QShowEvent *);

		void setAttributes(DatabaseModel *model, OperationList *op_list,
															 BaseObject *object, BaseObject *parent_obj=nullptr,
															 float obj_px=NAN, float obj_py=NAN, bool uses_op_list=true);

		/*! \brief This method is a simplification of the original setAttributes. This method must be used
		only on forms that does not make use of operaton list and not treat graphical objects, since it calls
		this original one whit the op_list=nullptr and obj_px=NAN, obj_py=NAN */
	 void setAttributes(DatabaseModel *model, BaseObject *object, BaseObject *parent_obj);

		void setRequiredField(QWidget *widget);

	public:
		//! \brief Constants used to generate version intervals for version alert frame
		static constexpr unsigned UNTIL_VERSION=0,
															VERSIONS_INTERVAL=1,
															AFTER_VERSION=2;

		BaseObjectWidget(QWidget * parent = 0, ObjectType obj_type=BASE_OBJECT);

		virtual ~BaseObjectWidget(void);

		//! \brief Generates a string containing the specified version interval
		static QString generateVersionsInterval(unsigned ver_interv_id, const QString &ini_ver, const QString &end_ver="");

		/*! \brief Generates a alert frame highlighting the fields of exclusive use on the specified
			PostgreSQL versions. On the first map (fields) the key is the PostgreSQL versions and
			the values are the reference to the widget. The second map is used to specify the values
			of widgets specific for each version. */
		static QFrame *generateVersionWarningFrame(map<QString, vector<QWidget *> > &fields, map<QWidget *, vector<QString> > *values=nullptr);

		//! \brief Generates a informative frame containing the specified message
		static QFrame *generateInformationFrame(const QString &msg);

		//! \brief Filters the ENTER/RETURN key press forcing the button "Apply" to be clicked
		bool eventFilter(QObject *object, QEvent *event);

	protected slots:
		void editPermissions(void);

	public slots:
		void show(void);

	signals:
		//! \brief Signal emitted whenever a object is created / edited using the form
		void s_objectManipulated(void);
};

template<class Class>
void BaseObjectWidget::startConfiguration(void)
{
	try
	{
		Class *new_tmpl_obj=nullptr;

		//! \brief If the object is already allocated
		if(this->object && op_list &&
			 this->object->getObjectType()!=OBJ_DATABASE)
		{
			if(this->table)
				op_list->registerObject(this->object, Operation::OBJECT_MODIFIED, -1, this->table);
			else
				op_list->registerObject(this->object, Operation::OBJECT_MODIFIED, -1, this->relationship);
			new_object=false;
		}
		//! \brief If there is need to allocate the object
		else if(!this->object)
		{
			new_tmpl_obj=new Class;
			this->object=new_tmpl_obj;
			new_object=true;
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

#endif
