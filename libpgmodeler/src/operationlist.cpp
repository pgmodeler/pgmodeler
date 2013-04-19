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

#include "operationlist.h"

unsigned OperationList::max_size=500;

OperationList::OperationList(DatabaseModel *model)
{
	/* Raises an error if the user tries to allocate an operation list linked to
		to an unallocated model */
	if(!model)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->model=model;
	current_index=0;
	next_op_chain=Operation::NO_CHAIN;
	ignore_chain=false;
}

OperationList::~OperationList(void)
{
	removeOperations();
}

unsigned OperationList::getCurrentSize(void)
{
	return(operations.size());
}

unsigned OperationList::getMaximumSize(void)
{
	return(max_size);
}

int OperationList::getCurrentIndex(void)
{
	return(current_index);
}

void OperationList::startOperationChain(void)
{
	/* If the chaining is started and the user try it initializes
		again, the earlier chaining is finished */
	if(next_op_chain!=Operation::NO_CHAIN)
		finishOperationChain();

	/* The next operation inserted on the list will be the start
		of the chaining */
	next_op_chain=Operation::CHAIN_START;
}

void OperationList::finishOperationChain(void)
{
	/* If the chain is not ignored indicates that the next
		element of the list no longer will be part of chaining */
	if(!ignore_chain)
		next_op_chain=Operation::NO_CHAIN;
	else if(ignore_chain)
		/* If the chain is canceled indicates that the next element
		 of the list still part of the chaining */
		next_op_chain=Operation::CHAIN_MIDDLE;

	if(operations.size() > 0 && !ignore_chain)
	{
		unsigned idx=operations.size()-1;

		/* Marks the last operatin as being the end of chaining in case it is
		 on the middle of chain */
		if(operations[idx]->chain_type==Operation::CHAIN_MIDDLE)
			operations[idx]->chain_type=Operation::CHAIN_END;

		/* If the last operation is marked as CHAIN_START indicates that
		 the chaining was open but only one operation is recorded
		 and thus the operation is marked as NO_CHAIN because as it is
		 only one operation there is no need to treat it as chaining */
		else if(operations[idx]->chain_type==Operation::CHAIN_START)
			operations[idx]->chain_type=Operation::NO_CHAIN;
	}
}

void OperationList::ignoreOperationChain(bool value)
{
	ignore_chain=value;
}

bool OperationList::isOperationChainStarted(void)
{
	return(next_op_chain==Operation::CHAIN_START ||
				 next_op_chain==Operation::CHAIN_MIDDLE);
}

bool OperationList::isRedoAvailable(void)
{
	/* The redo operation only can be performed
		if the current index from the list of operations is at most
		the penultimate element and the list can not be empty */
	return(!operations.empty() && current_index < static_cast<int>(operations.size()));
}

bool OperationList::isUndoAvailable(void)
{
	/* For the undo operation be performed is
		enough that the list of operations is not empty */
	return(!operations.empty() && current_index > 0);
}

void OperationList::setMaximumSize(unsigned max)
{
	//Raises an error if a zero max size is assigned to the list
	if(max==0)
		throw Exception(ERR_ASG_INV_MAX_SIZE_OP_LIST,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	max_size=max;
}

void OperationList::addToPool(BaseObject *object, unsigned op_type)
{
	ObjectType obj_type;

	try
	{

		//Raises an error if the object to be added is not allocated
		if(!object)
			throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		obj_type=object->getObjectType();

		//Stores a copy of the object if its about to be moved or modified
		if(op_type==Operation::OBJECT_MODIFIED ||
			 op_type==Operation::OBJECT_MOVED)
		{
			BaseObject *copy_obj=NULL;

			if(obj_type!=BASE_OBJECT && obj_type!=OBJ_DATABASE)
				PgModeler::copyObject(&copy_obj, object, obj_type);
			else
				throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//Raises an error if the copy fails (returning a null object)
			if(!copy_obj)
				throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			else
				//Inserts the copy on the pool
				object_pool.push_back(copy_obj);
		}
		else
			//Inserts the original object on the pool (in case of adition or deletion operations)
			object_pool.push_back(object);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void OperationList::removeOperations(void)
{
	BaseObject *object=NULL;
	TableObject *tab_obj=NULL;
	BaseTable *tab=NULL;

	//Destroy the operations
	while(!operations.empty())
	{
		delete(operations.back());
		operations.pop_back();
	}

	//Destroy the object pool
	while(!object_pool.empty())
		removeFromPool(0);

	//Case there is not removed object
	while(!not_removed_objs.empty())
	{
		object=not_removed_objs.back();

		if(unallocated_objs.count(object)==0)
			tab_obj=dynamic_cast<TableObject *>(object);

		//Deletes the object if its not unallocated already or referenced on the model
		if(unallocated_objs.count(object)==0 &&
			 (!tab_obj && model->getObjectIndex(object) < 0))
		{
			unallocated_objs[object]=true;
			delete(object);
		}
		else if(tab_obj && unallocated_objs.count(tab_obj)==0)
		{
			tab=dynamic_cast<BaseTable *>(tab_obj->getParentTable());

			//Deletes the object if its not unallocated already or referenced by some table
			if(!tab ||
				 (unallocated_objs.count(tab)==1) ||
				 (tab && unallocated_objs.count(tab)==0 && tab->getObjectIndex(tab_obj) < 0))
			{
				unallocated_objs[tab_obj]=true;
				delete(tab_obj);
			}
		}

		not_removed_objs.pop_back();
		tab_obj=NULL;
	}

	current_index=0;
	unallocated_objs.clear();
}

void OperationList::validateOperations(void)
{
	vector<Operation *>::iterator itr, itr_end;
	Operation *oper=NULL;

	itr=operations.begin();
	itr_end=operations.end();
	while(itr!=itr_end)
	{
		oper=(*itr);
		//Case the object isn't on the pool
		if(!isObjectOnPool((*itr)->pool_obj))
		{
			//Remove the operation
			operations.erase(itr);
			delete(oper);
			itr=operations.begin();
			itr_end=operations.end();
		}
		else itr++;
	}
}

bool OperationList::isObjectOnPool(BaseObject *object)
{
	bool found=false;
	vector<BaseObject *>::iterator itr, itr_end;

	if(!object)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=object_pool.begin();
	itr_end=object_pool.end();
	while(!found && itr!=itr_end)
	{
		found=*(itr)==object;
		itr++;
	}

	return(found);
}

void OperationList::removeFromPool(unsigned obj_idx)
{
	BaseObject *object=NULL;
	vector<BaseObject *>::iterator itr;

	//Raises an error if the object index is invalid (out of bound)
	if(obj_idx >= object_pool.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Gets the element to bo removed through its index
	itr=object_pool.begin() + obj_idx;
	object=(*itr);

	//Removes the object from pool
	object_pool.erase(itr);

	/* Stores the object that was in the pool on the 'not_removed_objs' vector.
		The object will be deleted in the destructor of the list. Note: The object is not
		deleted immediately because the model / table / list of operations may still
		referencing the object or it becomes referenced when an operation is performed on the list. */
	not_removed_objs.push_back(object);
}


void OperationList::registerObject(BaseObject *object, unsigned op_type, int object_idx,  BaseObject *parent_obj)
{
	ObjectType obj_type;
	Operation *operation=NULL;
	BaseTable *parent_tab=NULL;
	Relationship *parent_rel=NULL;
	TableObject *tab_obj=NULL;
	tab_obj=dynamic_cast<TableObject *>(object);
	int obj_idx=-1;

	try
	{
		//Raises an error if the user tries to register an operation with null object
		if(!object)
			throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		obj_type=object->getObjectType();
		if(tab_obj && !parent_obj)
			throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		else if(parent_obj &&
						(((obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT) &&
							(parent_obj->getObjectType()!=OBJ_RELATIONSHIP && parent_obj->getObjectType()!=OBJ_TABLE)) ||

						 ((obj_type==OBJ_TRIGGER || obj_type==OBJ_RULE) && !dynamic_cast<BaseTable *>(parent_obj)) ||

						 (obj_type==OBJ_INDEX && parent_obj->getObjectType()!=OBJ_TABLE)))
			throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//If the operations list is full makes the automatic cleaning before inserting a new operation
		if(current_index == static_cast<int>(max_size-1))
			removeOperations();

		/* If adding an operation and the current index is not pointing
		 to the end of the list (available redo / user ran undo operations)
		 all elements from the current index to the end of the list will be deleted
		 as well as the objects in the pool that were linked to the excluded operations */
		if(current_index>=0 && static_cast<unsigned>(current_index)!=operations.size())
		{
			//Gets the last operation index
			int i=operations.size()-1;

			//Removes all the operation while the current index isn't reached
			while(i >= current_index)
			{
				removeFromPool(i);
				i--;
			}

			//Validates the remaining operatoins after the deletion
			validateOperations();
		}

		//Creates the new operation
		operation=new Operation;
		operation->op_type=op_type;
		operation->chain_type=next_op_chain;
		operation->original_obj=object;

		//Adds the object on te pool
		addToPool(object, op_type);

		//Assigns the pool object to the operation
		operation->pool_obj=object_pool.back();

		//Stores the object's permission befor its removal
		if(op_type==Operation::OBJECT_REMOVED)
			model->getPermissions(object, operation->permissions);

		if(next_op_chain==Operation::CHAIN_START)
			next_op_chain=Operation::CHAIN_MIDDLE;

		/*  Performing specific operations according to the type of object.
		 If the object has a parent object, it must be discovered
		 and moreover it is necessary to find and store the index of the object
		 in the list on the parent object */
		if(tab_obj)
		{
			if(parent_obj->getObjectType()==OBJ_RELATIONSHIP)
				parent_rel=dynamic_cast<Relationship *>(parent_obj);
			else
				parent_tab=dynamic_cast<BaseTable *>(parent_obj);

			/* Specific case to columns: on removal operations the permissions of the objects
			must be removed too */
			//if(obj_type==OBJ_COLUMN && op_type==Operation::OBJECT_REMOVED)
				//model->removePermissions(tab_obj);
			//else
				if(((obj_type==OBJ_TRIGGER && dynamic_cast<Trigger *>(tab_obj)->isReferRelationshipAddedColumn()) ||
							 (obj_type==OBJ_INDEX && dynamic_cast<Index *>(tab_obj)->isReferRelationshipAddedColumn()) ||
							 (obj_type==OBJ_CONSTRAINT && dynamic_cast<Constraint *>(tab_obj)->isReferRelationshipAddedColumn())))
			{
				if(op_type==Operation::OBJECT_REMOVED)
					tab_obj->setParentTable(parent_tab);

				operation->xml_definition=tab_obj->getCodeDefinition(SchemaParser::XML_DEFINITION);
			}

			operation->parent_obj=parent_obj;

			/* If there is a parent relationship will get the index of the object.
			Only columns and constraints are handled case the parent is a relationship */
			if(parent_rel && (obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT))
			{
				//Case a specific index wasn't specified
				if(object_idx < 0)
					//Stores on the operation the index on its parent
					obj_idx=parent_rel->getObjectIndex(tab_obj);
				else
					//Assigns the specific index to operation
					obj_idx=object_idx;
			}
			//Case there is a parent table will get the object's index
			else if(parent_tab)
			{
				if(object_idx < 0)
					//Stores on the operation the index of object on its parent
					obj_idx=parent_tab->getObjectIndex(object->getName(false), obj_type);
				else
					obj_idx=object_idx;
			}
			//Raises an error if both parent table / relationship isn't allocated
			else
				throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		else
		{
			//Case a specific index wasn't specified
			if(object_idx < 0)
				//Stores on the operation the object index on the model
				obj_idx=model->getObjectIndex(object);
			else
				//Assigns the specific index to object
				obj_idx=object_idx;
		}

		operation->object_idx=obj_idx;
		operations.push_back(operation);
		current_index=operations.size();
	}
	catch(Exception &e)
	{
		if(operation)
		{
			removeFromPool(object_pool.size()-1);
			delete(operation);
		}
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void OperationList::getOperationData(unsigned oper_idx, unsigned &oper_type, QString &obj_name, ObjectType &obj_type)
{
	Operation *operation=NULL;

	if(oper_idx >= operations.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	operation=operations[oper_idx];
	oper_type=operation->op_type;

	obj_type=operation->pool_obj->getObjectType();

	if(obj_type==OBJ_CAST)
		obj_name=operation->pool_obj->getName();
	else
		obj_name=operation->pool_obj->getName(true);

	if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT || obj_type==OBJ_RULE ||
		 obj_type==OBJ_TRIGGER || obj_type==OBJ_INDEX)
	{
		obj_name=operation->parent_obj->getName(true) + "." + obj_name;
	}
}

unsigned OperationList::getChainSize(void)
{
	int i=current_index-1;
	unsigned size=0;

	if(i < 0 && !operations.empty())
		i=0;

	//Checks if the current operations is chained
	if(!operations.empty() &&
		 operations[i]->chain_type!=Operation::NO_CHAIN)
	{
		unsigned chain_type=0;
		int inc=0;

		//Case the operation is the end of a chain  runs the list in reverse order (from end to start)
		if(operations[i]->chain_type==Operation::CHAIN_END)
		{
			chain_type=Operation::CHAIN_START;
			inc=-1;
		}
		//Case the operation is the start of a chain  runs the list in normal order (from start to end)
		else if(operations[i]->chain_type==Operation::CHAIN_START)
		{
			chain_type=Operation::CHAIN_END;
			inc=1;
		}

		//Calculates the size of chain
		while(i>=0 && i < static_cast<int>(operations.size()) && operations[i]->chain_type!=chain_type)
		{
			i+=inc;
			size++;
		}
	}

	return(size);
}

void OperationList::undoOperation(void)
{
	if(isUndoAvailable())
	{
		Operation *operation=NULL;
		bool chain_active=false;
		Exception error;
		unsigned chain_size=0, pos=0;

		if(!this->signalsBlocked())
			chain_size=getChainSize();

		do
		{
			/* In the undo operation, it is necessary to obtain the object
			at current index decremented by 1 since the current index always
			points to the element after the last element */
			operation=operations[current_index-1];

			/* If it is detected that the operation is chained with other
			and active chaining flag is cleared marks the flag to start
			the execution several operations at once */
			if(!ignore_chain && !chain_active &&
				 operation->chain_type!=Operation::NO_CHAIN)
				chain_active=true;

			/* If the chaining is active and the current operation is not part of
			chain, aborts execution of the operation */
			else if(chain_active &&
							(operation->chain_type==Operation::CHAIN_END ||
							 operation->chain_type==Operation::NO_CHAIN))
				break;

			try
			{
				if(!this->signalsBlocked() && chain_size > 0)
				{
					//Emits a signal with the current progress of operation execution
					pos++;
					emit s_operationExecuted((pos/static_cast<float>(chain_size))*100,
																	 trUtf8("Undoing operation on object: %1 (%2)")
																	 .arg(operation->pool_obj->getName())
																	 .arg(operation->pool_obj->getTypeName()),
																	 operation->pool_obj->getObjectType());
				}

				//Executes the undo operation
				executeOperation(operation, false);
			}
			catch(Exception &e)
			{
				error=e;
			}

			current_index--;
		}
		/* Performs the operations while the current operation is part of chain
		 or the undo option is available */
		while(!ignore_chain && isUndoAvailable() && operation->chain_type!=Operation::NO_CHAIN);

		if(error.getErrorType()!=ERR_CUSTOM)
			throw Exception(error.getErrorMessage(), error.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

void OperationList::redoOperation(void)
{
	if(isRedoAvailable())
	{
		Operation *operation=NULL;
		bool chain_active=false;
		Exception error;
		unsigned chain_size=0, pos=0;

		if(!this->signalsBlocked())
			chain_size=getChainSize();

		do
		{
			//Gets the current operation
			operation=operations[current_index];

			/* If it is detected that the operation is chained with other
			and active chaining flag is cleared marks the flag to start
			the execution several operations at once */
			if(!ignore_chain && !chain_active &&
				 operation->chain_type!=Operation::NO_CHAIN)
				chain_active=true;

			/* If the chaining is active and the current operation is not part of
			chain or it is at the start of chain, aborts execution of the operation */
			else if(chain_active &&
							(operation->chain_type==Operation::CHAIN_START ||
							 operation->chain_type==Operation::NO_CHAIN))
				break;

			try
			{
				if(chain_size > 0)
				{
					//Emits a signal with the current progress of operation execution
					pos++;
					emit s_operationExecuted((pos/static_cast<float>(chain_size))*100,
																	 trUtf8("Redoing operation on object:: %1 (%2)")
																	 .arg(operation->pool_obj->getName())
																	 .arg(operation->pool_obj->getTypeName()),
																	 operation->pool_obj->getObjectType());
				}

				//Executes the redo operation (second argument as 'true')
				executeOperation(operation, true);
			}
			catch(Exception &e)
			{
				error=e;
			}
			current_index++;
		}
		/* Performs the operations while the current operation is part of chain
		 or the redo option is available */
		while(!ignore_chain && isRedoAvailable()  && operation->chain_type!=Operation::NO_CHAIN);

		if(error.getErrorType()!=ERR_CUSTOM)
			throw Exception(error.getErrorMessage(), error.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

void OperationList::executeOperation(Operation *oper, bool redo)
{
	if(oper)
	{
		BaseObject *orig_obj=NULL, *bkp_obj=NULL, *object=NULL, *aux_obj=NULL;
		ObjectType obj_type;
		BaseTable *parent_tab=NULL;
		Relationship *parent_rel=NULL;

		object=oper->pool_obj;
		obj_type=object->getObjectType();

		/* Converting the parent object, if any, to the correct class according
			to the type of the parent object. If OBJ_TABLE|OBJ_VIEW, the pointer
			'parent_tab' get the reference to table/view and will be used as referential
			in the operations below. If the parent object is a relationship, the pointer
					'parent_rel' get the reference to the relationship */
		if(oper->parent_obj)
		{
			if(oper->parent_obj->getObjectType()==OBJ_RELATIONSHIP)
				parent_rel=dynamic_cast<Relationship *>(oper->parent_obj);
			else
				parent_tab=dynamic_cast<BaseTable *>(oper->parent_obj);
		}

		/* If the XML definition of object is set indicates that it is referencing a column
			included by relationship (special object) */
		if(!oper->xml_definition.isEmpty() &&
			 ((oper->op_type==Operation::OBJECT_REMOVED && !redo) ||
				(oper->op_type==Operation::OBJECT_CREATED && redo) ||
				(oper->op_type==Operation::OBJECT_MODIFIED ||
				 oper->op_type==Operation::OBJECT_MOVED)))
		{
			//Resets the XML parser and loads the buffer xml from the operation
			XMLParser::restartParser();
			XMLParser::loadXMLBuffer(oper->xml_definition);

			if(obj_type==OBJ_TRIGGER)
				aux_obj=model->createTrigger(parent_tab);
			else if(obj_type==OBJ_INDEX)
				aux_obj=model->createIndex(dynamic_cast<Table *>(parent_tab));
			else if(obj_type==OBJ_CONSTRAINT)
				aux_obj=model->createConstraint(oper->parent_obj);
			else if(obj_type==OBJ_SEQUENCE)
				aux_obj=model->createSequence();
		}

		/* If the operation is a modified/moved object, the object copy
			stored in the pool will be restored */
		if(oper->op_type==Operation::OBJECT_MODIFIED ||
			 oper->op_type==Operation::OBJECT_MOVED)
		{
			if(obj_type==OBJ_RELATIONSHIP)
			{
				/* Due to the complexity of the class Relationship and the strong link between all
				relationships of the model it is necessary to store XML for special objects and
				disconnect ALL relationships, perform the modification at it and the revalidating
				all relationships again. */
				model->storeSpecialObjectsXML();
				model->disconnectRelationships();
			}

			//Gets the object in the current state from the parent object
			if(parent_tab)
				orig_obj=dynamic_cast<TableObject *>(parent_tab->getObject(oper->object_idx, obj_type));
			else if(parent_rel)
				orig_obj=dynamic_cast<TableObject *>(parent_rel->getObject(oper->object_idx, obj_type));
			else
				orig_obj=model->getObject(oper->object_idx, obj_type);

			if(aux_obj)
				oper->xml_definition=orig_obj->getCodeDefinition(SchemaParser::XML_DEFINITION);

			/* The original object (obtained from the table, relationship or model) will have its
				previous values restored with the existing copy on the pool. After restoring the object
				on the pool will have the same attributes as the object before being restored
				to enable redo operations */
			PgModeler::copyObject(reinterpret_cast<BaseObject **>(&bkp_obj), orig_obj, obj_type);
			PgModeler::copyObject(reinterpret_cast<BaseObject **>(&orig_obj), object, obj_type);
			PgModeler::copyObject(reinterpret_cast<BaseObject **>(&object), bkp_obj, obj_type);
			object=orig_obj;

			if(aux_obj)
				PgModeler::copyObject(reinterpret_cast<BaseObject **>(&object), aux_obj, obj_type);
		}

		/* If the operation is of object removed and is not a redo, or
			if the object was previously created and wants to redo the operation
			the existing pool object will be inserted into table or in your relationship
			on its original index */
		else if((oper->op_type==Operation::OBJECT_REMOVED && !redo) ||
						(oper->op_type==Operation::OBJECT_CREATED && redo))
		{
			if(aux_obj)
				PgModeler::copyObject(reinterpret_cast<BaseObject **>(&object), aux_obj, obj_type);

			if(parent_tab)
			{
				parent_tab->addObject(dynamic_cast<TableObject *>(object), oper->object_idx);

				if(object->getObjectType()==OBJ_CONSTRAINT &&
					 dynamic_cast<Constraint *>(object)->getConstraintType()==ConstraintType::foreign_key)
					model->updateTableFKRelationships(dynamic_cast<Table *>(parent_tab));
			}
			else if(parent_rel)
				parent_rel->addObject(dynamic_cast<TableObject *>(object), oper->object_idx);
			else
				if(dynamic_cast<Table *>(object))
					dynamic_cast<Table *>(object)->getCodeDefinition(SchemaParser::SQL_DEFINITION);
			model->addObject(object, oper->object_idx);

			if(oper->op_type==Operation::OBJECT_REMOVED)
				model->addPermissions(oper->permissions);
		}
		/* If the operation is a previously created object or if the object
			was removed and wants to redo the operation it'll be
			excluded from the table or relationship */
		else if((oper->op_type==Operation::OBJECT_CREATED && !redo) ||
						(oper->op_type==Operation::OBJECT_REMOVED && redo))
		{
			if(parent_tab)
				parent_tab->removeObject(oper->object_idx,obj_type);
			else if(parent_rel)
				parent_rel->removeObject(oper->object_idx,obj_type);
			else
				model->removeObject(object, oper->object_idx);
		}

		/* If the parent table or parent relationship is set
			indicates that the operation was performed on a child object of these objects */
		if(parent_tab || parent_rel)
		{
			//Marks the parent object as modified to for its redraw
			if(parent_tab)
				parent_tab->setModified(true);
			else
				parent_rel->forceInvalidate();

			if(parent_tab &&
				 (object->getObjectType()==OBJ_COLUMN ||
					object->getObjectType()==OBJ_CONSTRAINT))
			{
				model->validateRelationships(dynamic_cast<TableObject *>(object), dynamic_cast<Table *>(parent_tab));

				if(object->getObjectType()==OBJ_CONSTRAINT &&
					 dynamic_cast<Constraint *>(object)->getConstraintType()==ConstraintType::foreign_key)
					model->updateTableFKRelationships(dynamic_cast<Table *>(parent_tab));
			}
			else if(parent_rel)
				model->validateRelationships();
		}

		/* If the object in question is graphical it has the modified flag
			marked to force the redraw at the time of its restoration */
		else if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW ||
						obj_type==BASE_RELATIONSHIP || obj_type==OBJ_RELATIONSHIP ||
						obj_type==OBJ_TEXTBOX)
		{
			BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

			if(oper->op_type==Operation::OBJECT_MODIFIED ||
				 oper->op_type==Operation::OBJECT_MOVED)
				graph_obj->setModified(true);

			//Case the object is a view is necessary to update the table-view relationships on the model
			if(obj_type==OBJ_VIEW && oper->op_type==Operation::OBJECT_MODIFIED)
				model->updateViewRelationships(dynamic_cast<View *>(graph_obj));
			else if((obj_type==OBJ_RELATIONSHIP ||
							 (obj_type==OBJ_TABLE && model->getRelationship(dynamic_cast<BaseTable *>(object), NULL))) &&
							oper->op_type==Operation::OBJECT_MODIFIED)
				model->validateRelationships();

			//If a object had its schema restored is necessary to update the envolved schemas
			if((obj_type==OBJ_TABLE || obj_type==OBJ_VIEW) &&
				 ((bkp_obj && graph_obj->getSchema()!=bkp_obj->getSchema() && oper->op_type==Operation::OBJECT_MODIFIED) ||
					oper->op_type==Operation::OBJECT_MOVED))
			{
				dynamic_cast<BaseGraphicObject *>(graph_obj->getSchema())->setModified(true);

				if(bkp_obj)
				 dynamic_cast<BaseGraphicObject *>(bkp_obj->getSchema())->setModified(oper->op_type==Operation::OBJECT_MODIFIED);
			}
		}
		else if(obj_type==OBJ_SCHEMA && oper->op_type==Operation::OBJECT_MODIFIED)
		{
			model->validateSchemaRenaming(dynamic_cast<Schema *>(object), bkp_obj->getName());
			dynamic_cast<Schema *>(object)->setModified(true);
		}
	}
}

void OperationList::removeLastOperation(void)
{
	if(!operations.empty())
	{
		Operation *oper=NULL;
		bool end=false;
		vector<Operation *>::reverse_iterator itr;
		unsigned obj_idx=operations.size()-1;

		//Gets the last operation on the list using reverse iterator
		itr=operations.rbegin();

		while(!end)
		{
			oper=(*itr);

			//Removes the object related to the operation from the pool
			removeFromPool(obj_idx);

			/* Stop condition for removing the operation:
			 1) The operation is not chained with others, or
			 2) If the last operation at the end of a chain
					all chained operations are removed including
					the first operation of the chain, when the latter
					is removed the iteration is stopped.*/
			end=(ignore_chain ||
					 (!ignore_chain &&
						(oper->chain_type==Operation::NO_CHAIN ||
						 oper->chain_type==Operation::CHAIN_START)));

			itr++; obj_idx--;
		}

		/* If the head of chaining is removed (CHAIN_START)
		 marks that the next element in the list is the new
		 start of chain */
		if(oper && oper->chain_type==Operation::CHAIN_START)
			next_op_chain=Operation::CHAIN_START;

		//Validates the remaining operations
		validateOperations();

		/* Points the current index to the end of list if it is beyond
		 the last element */
		if(static_cast<unsigned>(current_index) > operations.size())
			current_index=operations.size();
	}
}

void OperationList::updateObjectIndex(BaseObject *object, unsigned new_idx)
{
	vector<Operation *>::iterator itr, itr_end;
	Operation *oper=NULL;

	if(!object)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	itr=operations.begin();
	itr_end=operations.end();

	while(itr!=itr_end)
	{
		oper=(*itr);
		if(oper->original_obj==object)
			oper->object_idx=new_idx;
		itr++;
	}
}

