/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: OperationList
# Description: Implements the operations to maintain a list of modifications made
#              by the user on database model objects. This class permits that user
#              undo / redo all the operations made.
#
# Creation date: 17/07/2006
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
#ifndef OPERATIONLIST_H
#define OPERATIONLIST_H

#include "databasemodel.h"
#include <QObject>

class Operation {
 protected:
 /* Reference to the parent object of the original object that has suffered the operation,
    This attribute is used only in the case of table objects such as columns, indexes,
    constraints, rules. For other object types there is no need to use this attribute
    because the parent object for other objects is always the database model */
  BaseObject *parent_obj;

  //Reference (on the pool) to the copy of the original object
  BaseObject *pool_obj;

  //Reference to the original object that generates the operation.
  BaseObject *original_obj;

  /* Stores the XML definition of the special objects this means the objects
     that reference columns added by relationship. This is the case of triggers,
     indexes, sequences, constraints. */
  QString xml_definition;

  //Operation type (Constants OBJECT_[MODIFIED | CREATED | REMOVED | MOVED]
  unsigned op_type;

  //Operation chain type. This attribute is used to redo/undo several operations at once
  unsigned chain_type;

  //Object index inside the list on its parent object
  int object_idx;

 public:
  //Constants used to reference the type of operations
  static const unsigned OBJECT_MODIFIED=0,
                        OBJECT_CREATED=1,
                        OBJECT_REMOVED=2,
                        /* This type of operation has the same effect of operation OBJETO_MODIFIED
                           except that it not (re)validate relationships as happens with operations.
                           This type of operation (OBJECT_MOVED) is useful to undo position changes of
                           graphical objects without executing unnecessary revalidations of relationships */
                        OBJECT_MOVED=3;

  //Operation chain types
  static const unsigned NO_CHAIN=10, //The operation is not part of a chain
                        CHAIN_START=11, //The operation is the head of the chain
                        CHAIN_MIDDLE=12, //The operation is in the middle of the chain
                        CHAIN_END=13; //The operation is the last on the chain


  Operation(void)
  { parent_obj=NULL; pool_obj=NULL; original_obj=NULL;
    object_idx=-1; chain_type=NO_CHAIN; }

 friend class OperationList;
};

class OperationList: public QObject {
 private:
  Q_OBJECT

  //Inidcates that operation chaining is ignored temporarily
  bool ignore_chain;

  //List of objects that were removed / modified on the model
  vector<BaseObject *> object_pool;

  /* List of objects that at the time of deletion from pool were still referenced
     somehow on the model. The object is stored in this secondary list and
     deleted when the whole list of operations is destroyed */
  vector<BaseObject *> not_removed_objs;

  //Stores the operations executed by the user
  vector<Operation *> operations;

  //Database model that is linked with this operation list
  DatabaseModel *model;

  //Maximum number of stored operations (global)
  static unsigned max_size;

  /* Stores the type of chain to the next operation to be stored
     in the list. This attribute is used in conjunction with the chaining
     initialization / finalization methods. */
  unsigned next_op_chain;

  //Current operation index
  int current_index;

  /* Validates operations by checking whether they have registered objects in the pool.
     If found any operation whose object is not in the pool it will be removed
     because an object outside the pool does not give a guarantee that is being
     referenced in the model. */
  void validateOperations(void);

  //Checks whether the passed object is in the pool
  bool isObjectOnPool(BaseObject *object);

  //Adds the object on the pool according to the operation type passed
  void addToPool(BaseObject *object, unsigned op_type);

  /* Removes one object from the pool using its index and deallocating
     it in case the object is not referenced on the model */
  void removeFromPool(unsigned obj_idx);

  /* Executes the passed operation. The default behavior is the 'undo' if
     the user passes the parameter 'redo=true' the method executes the
     redo function */
  void executeOperation(Operation *operacao, bool redo);

  //Returns the chain size from the current element
  unsigned getChainSize(void);

 public:
  OperationList(DatabaseModel *model);
  ~OperationList(void);

  /* Starts chaining operations.
     This means that all operations added after calling this
     method will be considered to be performed all at once
     with a single call to the redoOperation / undoOperation methods */
  void startOperationChain(void);

  /* Finalizes the chaining marking the last operation on the list
     as the end of operation chain */
  void finishOperationChain(void);

  /* Cancels the execution of operations in the form of chaining,
     but if the list is open with chaining operations included will be chained too.
     This method helps in situations where is necessary to remove operations or
     execute them one by one but keeping the chaining created earlier.

     Note: The user must cancel the annulment of chaining
           to be able to finalize the operations chaining. If it does not
           happens the operations will be created chained indefinitely */
  void ignoreOperationChain(bool value);

  //Returns if the operation chaining where started
  bool isOperationChainStarted(void);

  //Undo the current operation on the list
  void undoOperation(void);

  //Redo the current operation on the list
  void redoOperation(void);

  //Removes all the operations from the list
  void removeOperations(void);

  //Gets the data from the operation with specified index
  void getOperationData(unsigned oper_idx, unsigned &oper_type, QString &obj_name, ObjectType &obj_type);

  //Sets the maximum size for the list
  static void setMaximumSize(unsigned max);

  /* Registers in the list of operations that the object passed suffered some kind
     of modification (modified, removed, inserted, moved) in addition the method stores
     its original content.
     This method should ALWAYS be called before the object in question
     suffers any operation in the model. If this method is called after an operation on the
     object the order of restoration / re-execution of operations can be broken and cause
     segmentations fault. */
  void registerObject(BaseObject *object, unsigned op_type, int object_idx=-1, BaseObject *parent_obj=NULL);

  //Gets the maximum size for the operation list
  unsigned getMaximumSize(void);

  //Gets the current size for the operation list
  unsigned getCurrentSize(void);

  //Gets the current operation index
  int getCurrentIndex(void);

  //Returns if the list is prepared to execute redo operations
  bool isRedoAvailable(void);

  //Returns if the list is prepared to execute undo operations
  bool isUndoAvailable(void);

  /* Removes the last operation from the list. This method should be used with
     care as it can break the chain of operations. It should be
     used only when an exception is thrown after adding
     some object on the list and if the user wants to discard this operation due
     the thrown exception. If the last operation is part of a chain the entire chain
     of operations is removed.
     Warning: The execution of this method is different from the undo method because
     the objects are removed pool but their states prior to adding it to the list are not
     restored so this method can not be used deliberately. */
  void removeLastOperation(void);

  /* Updates the index of the object when it suffers a movement in the parente object.
     Generally this method need not be called manually but in the case of table objects
     (like columns, rules, constraints, indexes and triggers) which can be moved
     (to have their position changed in the parent object). This method updates the index
     of the object with the new value for the operations which refer the object is not
     executed incorrectly using previous index */
  void updateObjectIndex(BaseObject *object, unsigned new_idx);

  signals:
   //Signal emitted when one operation is executed
   void s_operationExecuted(int progress, QString object_id, unsigned icon_id);
};


/* Template function that makes a copy from 'copy_obj' to 'psrc_obj' doing the cast to the
   correct object type. If the source object (psrc_obj) is not allocated the function allocates the attributes
   before copying. Both objects must be the same type if both are allocated.
   -- Brainfuck syntax style! :p -- */
template <class Classe>
void copyObject(BaseObject **psrc_obj, Classe *copy_obj);

/* This functions is a second way to make a copy between two objects. It simply calls
   the template function above. */
void copyObject(BaseObject **psrc_obj, BaseObject *copy_obj, ObjectType obj_type);

#endif
