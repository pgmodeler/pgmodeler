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

/**
\ingroup libpgmodeler_ui
\class DatabaseExplorerWidge
\brief Implements the operations to browse and manipulate database instances
*/

#ifndef DATABASE_EXPLORER_WIDGET_H
#define DATABASE_EXPLORER_WIDGET_H

#include "ui_databaseexplorerwidget.h"
#include "databaseimporthelper.h"
#include "schemaparser.h"

class DatabaseExplorerWidget: public QWidget, public Ui::DatabaseExplorerWidget {
	private:
		Q_OBJECT

    static const QString DEP_NOT_DEFINED,
                         DEP_NOT_FOUND,
                         ELEM_SEPARATOR;

    //! brief Stores the translations of all used attributes at properties panel
    static const attribs_map attribs_i18n;

    /*! brief Connection used to handle objects on database. This connection is copied
        whenever a new operation must be performed on database */
    Connection connection;

    //! brief Database import helper used to list objects from current connection
    DatabaseImportHelper import_helper;

    //! brief Catalog instance used to retrieve object's attributes
    Catalog catalog;

    SchemaParser schparser;

    //! brief Stores the actions to be performed over the object
    QMenu handle_menu,

    //! brief Stores the snippets' actions
    snippets_menu;

    QAction *copy_action, *drop_action, *drop_cascade_action,
            *show_data_action, *refresh_action, *properties_action;

    void configureImportHelper(void);

    //! brief Drops the object represented by the specified item
    void dropObject(QTreeWidgetItem *item, bool cascade);

    bool eventFilter(QObject *object, QEvent *event);

    /*! brief Returns the properly format object name by querying it using its OID and type.
        Optional schema and table names can be specified to filter the results */
    QString getObjectName(ObjectType obj_type, const QString &oid, const QString &sch_name="", const QString tab_name="");

    /*! brief Returns the properly format list of object names by querying them using their OIDs and type.
        Optional schema and table names can be specified to filter the results */
    QStringList getObjectsNames(ObjectType obj_type, const QStringList &oids, const QString &sch_name="", const QString tab_name="");

    //! brief Format the object's name based upon the passed attributes
    QString formatObjectName(attribs_map &attribs);

    /*! brief Formats the keys and values for the passed object attributes returning a new map with the formatted attributes.
        This method do basic formattation on commom attributes and internally call the format[OBJECT]Attribs()
        method according to the object being currently processed. NOTE: the original 'attribs' has its value changed but the
        keys are preserved. */
    attribs_map formatObjectAttribs(attribs_map &attribs);

    //! brief Formats the boolean attributes translating the 1 ou "" values to true or false
    void formatBooleanAttribs(attribs_map &attribs, QStringList bool_attrs);

    //! brief Convert oid attributes (or array of oids) in object names by querying it on catalog
    void formatOidAttribs(attribs_map &attribs, QStringList oid_attrs, ObjectType obj_type, bool is_oid_array);

    void formatCastAttribs(attribs_map &attribs);
    void formatLanguageAttribs(attribs_map &attribs);
    void formatRoleAttribs(attribs_map &attribs);
    void formatEventTriggerAttribs(attribs_map &attribs);
    void formatAggregateAttribs(attribs_map &attribs);
    void formatConversionAttribs(attribs_map &attribs);
    void formatDomainAttribs(attribs_map &attribs);
    void formatExtensionAttribs(attribs_map &attribs);
    void formatFunctionAttribs(attribs_map &attribs);
    void formatOperatorAttribs(attribs_map &attribs);
    void formatTableAttribs(attribs_map &attribs);
    void formatSequenceAttribs(attribs_map &attribs);
    void formatViewAttribs(attribs_map &attribs);
    void formatTypeAttribs(attribs_map &attribs);
    void formatOperatorClassAttribs(attribs_map &attribs);
    void formatTriggerAttribs(attribs_map &attribs);
    void formatRuleAttribs(attribs_map &attribs);
    void formatColumnAttribs(attribs_map &attribs);
    void formatConstraintAttribs(attribs_map &attribs);
    void formatIndexAttribs(attribs_map &attribs);

    void handleSelectedSnippet(const QString &snip_id);

  public:
    DatabaseExplorerWidget(QWidget * parent = 0);

    //! brief Configures the connection used to retrieve and manipulate objects on database
    void setConnection(Connection conn);

    //! brief Returns a copy of the connection used by this explorer instance
    Connection getConnection(void);

    //! brief Clears the object's properties table
    void clearObjectProperties(void);

  public slots:
    //! brief Lists all objects for the current selected database
    void listObjects(void);

  private slots:
    //! brief Shows the menu to drop/show data
    void handleObject(QTreeWidgetItem *item, int);

    //! brief Updates on the tree under the current selected object
    void updateCurrentItem(void);

    //! brief Loads the catalog properties of a selected object and stores them in the current selected item
    void loadObjectProperties(void);

    //! brief Loads (calling loadObjectProperties) and expose the attributes of the object in the properties grid
    void showObjectProperties(void);

  signals:
    //! brief This signal is emmited to indicate that the data manipulation dialog need to be opened
    void s_dataGridOpenRequested(QString schema, QString table, bool hide_views);

    //! brief This signal is emmited containing the processed snippet to be shown in an input field
    void s_snippetShowRequested(QString snippet);
};

#endif
