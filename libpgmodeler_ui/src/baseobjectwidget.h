/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler UI library (libpgmodeler_ui)
# Class: BaseObjectWidget
# Description: Implements the basic operations to create/edit database objects via form.
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
#ifndef BASE_OBJECT_WIDGET_H
#define BASE_OBJECT_WIDGET_H

#include <QtGui>
#include "databasemodel.h"
#include "operationlist.h"
#include "formbasico.h"
#include "seletorobjetowidget.h"
#include "ui_baseobjectwidget.h"

/* Declaring the PgSQLType class as a Qt metatype in order to permit
   that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(PgSQLType)

class BaseObjectWidget: public QDialog, public Ui::BaseObjectWidget {
 Q_OBJECT

 private:
   /* Stores the minimum and maximum height of the parent form, in order
      to control the exhibition of the alert frame when the object is protected */
   int pf_min_height, pf_max_height;

 protected:
   static const QColor PROT_LINE_BGCOLOR,
                       PROT_LINE_FGCOLOR,
                       RELINC_LINE_BGCOLOR,
                       RELINC_LINE_FGCOLOR;

   //Parent form used to show the widget as a dialog.
   FormBasico *parent_form;

   //Reference database model
   DatabaseModel *model;

   //Reference table (used only when editing table objects)
   Table *table;

   //Used to store the object previous name (used to validate schema renaming)
   QString prev_name;

   //Reference relationship (used only when editing relationship attributes)
   Relationship *relationship;

   //Reference operation list where all modifications in the form are registered
   OperationList *op_list;

   //Object that is being edited / created
   BaseObject *object;

   /* Stores the object position (generally the mouse position) when the dialog was called
      (used only when creating graphical objects) */
   float object_px, object_py;

   //Grid layout used to organize the widgets over the form
   QGridLayout *baseobject_grid;

   //Indicates if the object is a new one or is being edited
   bool new_object;

   //Syntax highlighter used on the parent object name widget
   DestaqueSintaxe *hl_parentname_txt;

   //Object selectors for schema, owner an tablespace
   SeletorObjetoWidget *schema_sel,
                       *owner_sel,
                       *tablespace_sel;

   //Constants used to generate version intervals for version alert frame
   static const unsigned UNTIL_VERSION=0,
                         VERSIONS_INTERVAL=1,
                         AFTER_VERSION=2;

   //Generates a string containing the specified version interval
   static QString generateVersionsInterval(unsigned ver_interv_id, const QString &ini_ver, const QString &end_ver="");

   /* Generates a alert frame highlighting the fields of exclusive use on the specified
      PostgreSQL versions. On the first map (fields) the key is the PostgreSQL versions and
      the values are the reference to the widget. The second map is used to specify the values
      of widgets specific for each version. */
   QFrame *generateVersionWarningFrame(map<QString, vector<QWidget *> > &fields, map<QWidget *, vector<QString> > *values=NULL);

   //Generates a informative frame containing the specified message
   QFrame *generateInformationFrame(const QString &msg);

   /* Merges the specified grid layout with the 'baseobject_grid' creating a single form.
      The obj_type parameter must be specified to show the object type icon */
   void configureFormLayout(QGridLayout *grid=NULL, ObjectType obj_type=BASE_OBJECT);

   /* Starts a object configuration, alocating a new one if necessary, registering
      the object on the operation list. This method doens't applies to database model edition */
   template<class Class>
   void startConfiguration(void);

   /* Finishes the edition / creation of object, registering it on the operation list
      and inserts is on the parent object */
   void finishConfiguration(void);

   /* Aborts the object configuration, deallocation it if necessary or restoring it to
      its previous configuration */
   virtual void cancelConfiguration(void);

   //Apply the basic configurations to the object (name, schema, comment, owner, tablespace)
   virtual void applyConfiguration(void);

   void hideEvent(QHideEvent *);
   void showEvent(QShowEvent *);

 public:
   BaseObjectWidget(QWidget * parent = 0, ObjectType obj_type=BASE_OBJECT);
   virtual ~BaseObjectWidget(void);

   virtual void setAttributes(DatabaseModel *model, OperationList *op_list,
                              BaseObject *object, BaseObject *parent_obj=NULL,
                              float obj_px=NAN, float obj_py=NAN);

 protected slots:
   void editPermissions(void);

 public slots:
   void show(void);

 signals:
   //Signal emitted whenever a object is created / edited using the form
   void s_objectManipulated(void);
};

template<class Class>
void BaseObjectWidget::startConfiguration(void)
{
 try
 {
  Class *new_tmpl_obj=NULL;

  //If the object is already allocated
  if(this->object && op_list &&
     this->object->getObjectType()!=OBJ_DATABASE)
  {
   if(this->table)
    op_list->registerObject(this->object, Operation::OBJECT_MODIFIED, -1, this->table);
   else
    op_list->registerObject(this->object, Operation::OBJECT_MODIFIED, -1, this->relationship);
   new_object=false;
  }
  //If there is need to allocate the object
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
