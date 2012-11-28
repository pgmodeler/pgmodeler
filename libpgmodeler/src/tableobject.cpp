#include "tableobject.h"

TableObject::TableObject(void)
{
 parent_table=NULL;
 add_by_linking=add_by_generalization=add_by_copy=false;
}

void TableObject::setParentTable(BaseTable *table)
{
 //Raises an error if the parent object is not a table
 if(table && table->getObjectType()!=OBJ_TABLE)
  throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 parent_table=table;
}

BaseTable *TableObject::getParentTable(void)
{
 return(parent_table);
}

void TableObject::setAddedByLinking(bool value)
{
 add_by_linking=value;
 add_by_generalization=false;
 add_by_copy=false;
}

bool TableObject::isAddedByLinking(void)
{
 return(add_by_linking);
}

void TableObject::setAddedByGeneralization(bool value)
{
 add_by_generalization=value;
 add_by_linking=false;
 add_by_copy=false;
}

bool TableObject::isAddedByGeneralization(void)
{
 return(add_by_generalization);
}

void TableObject::setAddedByCopy(bool value)
{
 add_by_copy=value;
 add_by_generalization=false;
 add_by_linking=false;
}

bool TableObject::isAddedByCopy(void)
{
 return(add_by_copy);
}

bool TableObject::isAddedByRelationship(void)
{
 return(add_by_linking || add_by_generalization || add_by_copy);
}

void TableObject::operator = (TableObject &object)
{
 *(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(object);
 this->parent_table=object.parent_table;
 this->add_by_copy=false;
 this->add_by_generalization=false;
 this->add_by_linking=false;
}
