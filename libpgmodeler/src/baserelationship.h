/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: BaseRelationship
# Description: Implements the basic operations to manipulate relationships between tables
# Creation date: 09/04/2008
#
# Copyright 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef BASE_RELATIONSHIP_H
#define BASE_RELATIONSHIP_H

#include "basetable.h"
#include "textbox.h"
#include <cmath>

class BaseRelationship: public BaseGraphicObject {
 private:
   QString getCodeDefinition(unsigned){ return(""); }

 protected:
  //Represents the points added by the user on the relationship line
  vector<QPointF> points;

  //Indicates whether the relationship is linked to the tables
  bool connected;

  //Indicates the mandatory participation of source and destination tables
  bool src_mandatory, dst_mandatory;

  /* Relationship lables:
     0 - Source cardinality
     1 - Destination cardinality
     2 - Relationship name */
  Textbox *lables[3];

  /* Stores the distances of the labels from its respectively origins.
     This is used to controle de position of the labels when they are
     moved by the user */
  QPointF lables_dist[3];

  //Entidades envolvidas no relacionamento
  BaseTable *src_table,
            *dst_table;

  /* Relationship type. It can be "One to One", "One to Many", "Many to Many",
     "Generalization", "Dependecy". The constants RELATIONSHIP_??? are used
     to assign a type to the relationship */
  unsigned rel_type;

  //Sets the attributes used on the generation of XML definition for relationship
  void setRelationshipAttributes(void);

  //Makes the initial configuration creating the labels
  void configureRelationship(void);

  //Marks the flag indicating that relationship is connected
  void connectRelationship(void);

  //Uncheck the flag indicating that relationship is disconnected
  void disconnectRelationship(void);

 public:
  //Constants used to assign the type to relationship
  static const unsigned RELATIONSHIP_11=10, //One to one
                        RELATIONSHIP_1N=20, //One to many
                        RELATIONSHIP_NN=30, //Many to many
                        RELATIONSHIP_GEN=40, //Generalization (Inheritance)
                        RELATIONSHIP_DEP=50; //Dependency (table-view) / Copy (table-table)

  //Constats used to reference the relationship labels
  static const unsigned LABEL_SRC_CARD=0,
                        LABEL_DST_CARD=1,
                        LABEL_REL_NAME=2;

  //Constants used to reference the source and destination tables
  static const unsigned SRC_TABLE=3,
                        DST_TABLE=4;

  BaseRelationship(BaseRelationship *rel);

  BaseRelationship(unsigned rel_type, BaseTable *src_tab, BaseTable *dst_tab,
                     bool src_mandatory, bool dst_mandatory);

  ~BaseRelationship(void);

  //Sets the name of the relationship
  void setName(const QString &name);

  //Sets the mandatory participation for the specified table (Via constants SRC_TABLE | DST_TABLE)
  void setMandatoryTable(unsigned table_id, bool value);

  //Return one relationship label using its id (Via constants LABEL_???)
  Textbox *getLabel(unsigned label_id);

  //Returns one of the participant tables (Via constants SRC_TABLE | DST_TABLE)
  BaseTable *getTable(unsigned table_id);

  //Returns the relationship type
  unsigned getRelationshipType(void);

  //Returns the mandatory participation for the specified table (Via constants SRC_TABLE | DST_TABLE)
  bool isTableMandatory(unsigned table_id);

  //Returns the relationship connection state
  bool isRelationshipConnected(void);

  //Returns the XML definition for the relationship
  QString getCodeDefinition(void);

  //Returns whether the table is linked to itself via relationship (self-relationship)
  bool isSelfRelationship(void);

  //Stores the points that defines the custom relationship line
  void setPoints(const vector<QPointF> &points);

  //Returns the relationship point list
  vector<QPointF> getPoints(void);

  //Sets the distance of the specified label in relation to its origin
  void setLabelDistance(unsigned label_id, QPointF label_dist);

  //Gets the distance of the specified label in relation to its origin
  QPointF getLabelDistance(unsigned label_id);

  //Assigns one relationship to other making the appropriate attribute copy
  void operator = (BaseRelationship &rel);

  friend class ModeloBD;
};

#endif
