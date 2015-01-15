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

#include "modelsdiffhelper.h"
#include <QThread>
#include "pgmodelerns.h"

ModelsDiffHelper::ModelsDiffHelper(void)
{
  diff_canceled=false;
  pgsql_version=PgSQLVersions::DEFAULT_VERSION;
	source_model=imported_model=nullptr;
  resetDiffCounter();
  setDiffOptions(true, true, false, false, false, true);
}

ModelsDiffHelper::~ModelsDiffHelper(void)
{
  destroyTempObjects();
}

void ModelsDiffHelper::setDiffOptions(bool keep_cluster_objs, bool cascade_mode, bool truncate_tables, bool force_recreation, bool recreate_unchangeble, bool keep_obj_perms)
{
  this->keep_cluster_objs=keep_cluster_objs;
  this->cascade_mode=cascade_mode;
  this->force_recreation=force_recreation;
  this->trucante_tables=truncate_tables;
  this->recreate_unchangeble=(force_recreation && recreate_unchangeble);
  this->keep_obj_perms=keep_obj_perms;
}

void ModelsDiffHelper::setPgSQLVersion(const QString pgsql_ver)
{
  this->pgsql_version=pgsql_ver;
}

void ModelsDiffHelper::resetDiffCounter(void)
{  
	diffs_counter[ObjectsDiffInfo::ALTER_OBJECT]=0;
	diffs_counter[ObjectsDiffInfo::DROP_OBJECT]=0;
  diffs_counter[ObjectsDiffInfo::CREATE_OBJECT]=0;
  diffs_counter[ObjectsDiffInfo::IGNORE_OBJECT]=0;
}

QString ModelsDiffHelper::getDiffDefinition(void)
{
  return(diff_def);
}

void ModelsDiffHelper::setModels(DatabaseModel *src_model, DatabaseModel *imp_model)
{
	source_model=src_model;
	imported_model=imp_model;
}

unsigned ModelsDiffHelper::getDiffTypeCount(unsigned diff_type)
{
	if(diff_type >= ObjectsDiffInfo::NO_DIFFERENCE)
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(diffs_counter[diff_type]);
}

void ModelsDiffHelper::diffModels(void)
{
	try
	{
    if(!source_model || !imported_model)
			throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    //First, we need to detect the objects to be dropped
		diffModels(ObjectsDiffInfo::DROP_OBJECT);
    //Second, we will check the objects to be created or modified
		diffModels(ObjectsDiffInfo::CREATE_OBJECT);

		if(diff_canceled)
			emit s_diffCanceled();
		else
    {
      processDiffInfos();
      emit s_diffFinished();
    }
	}
	catch(Exception &e)
	{
		emit s_diffAborted(Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, e.getExtraInfo()));
  }

  destroyTempObjects();
  resetDiffCounter();
}

void ModelsDiffHelper::cancelDiff(void)
{
	diff_canceled=true;
}

void ModelsDiffHelper::diffTables(Table *src_table, Table *imp_table, unsigned diff_type)
{
  ObjectType types[2]={ OBJ_COLUMN, OBJ_CONSTRAINT };
  vector<TableObject *> *tab_objs=nullptr;
  Constraint *constr=nullptr;
  Table *ref_tab=nullptr, *comp_tab=nullptr;
  BaseObject *aux_obj=nullptr;

  if(diff_type==ObjectsDiffInfo::DROP_OBJECT)
  {
    /* In case of DROP, the reference table is the one from the database
       and the compared table is the one from model */
    ref_tab=imp_table;
    comp_tab=src_table;
  }
  else if(diff_type==ObjectsDiffInfo::CREATE_OBJECT ||
          diff_type==ObjectsDiffInfo::ALTER_OBJECT)
  {
    ref_tab=src_table;
    comp_tab=imp_table;
  }

  for(unsigned i=0; i < 2 && !diff_canceled; i++)
  {
    tab_objs=ref_tab->getObjectList(types[i]);

    for(auto tab_obj : *tab_objs)
    {
      //Get the object from the compared table
      aux_obj=comp_tab->getObject(tab_obj->getName(), tab_obj->getObjectType());
      constr=dynamic_cast<Constraint *>(tab_obj);

      /* If the current info is ALTER or CREATE, only objects created by generalization or
         not created by common relationships will be considered on the comparison. Also,
         foreign keys are discarded here, since they will be compared on the main comparison
         at diffModels() */
      if(aux_obj && diff_type!=ObjectsDiffInfo::DROP_OBJECT &&
         ((tab_obj->isAddedByGeneralization() || !tab_obj->isAddedByLinking()) ||
          (constr && constr->getConstraintType()!=ConstraintType::foreign_key)))
      {
        //If there are some differences on the XML code of the objects
        if(tab_obj->isCodeDiffersFrom(aux_obj))
          generateDiffInfo(ObjectsDiffInfo::ALTER_OBJECT, tab_obj, aux_obj);
      }      
      /* If the object does not exists it will generate a drop info and the original
         one (tab_obj) was not included by generalization (to avoid drop inherited columns) */
      else if(!aux_obj && !tab_obj->isAddedByGeneralization())
        generateDiffInfo(diff_type, tab_obj);
    }

    QThread::msleep(15);
  }
}

void ModelsDiffHelper::diffModels(unsigned diff_type)
{
	map<unsigned, BaseObject *> obj_order;
	BaseObject *object=nullptr, *aux_object=nullptr;
	ObjectType obj_type;
  QString obj_name;
  unsigned idx=0, factor=0, prog=0;
	DatabaseModel *aux_model=nullptr;
  bool objs_differs=false, xml_differs=false;

	if(diff_type==ObjectsDiffInfo::DROP_OBJECT)
	{
    /* For DROP detection, we must gather the objects from the database in order to check
       if they exists on the model. The object drop order here is the inverse of the creation order
       on the database */
		obj_order=imported_model->getCreationOrder(SchemaParser::SQL_DEFINITION);
    aux_model=source_model;
    factor=25;
	}
	else if(diff_type==ObjectsDiffInfo::CREATE_OBJECT ||
					diff_type==ObjectsDiffInfo::ALTER_OBJECT)
	{
    /* For creation or modification of objects the order followed is the same
       as the creation order on the source model */
		obj_order=source_model->getCreationOrder(SchemaParser::SQL_DEFINITION);
		aux_model=imported_model;
    factor=50;
    prog=50;
	}

	for(auto obj_itr : obj_order)
	{
		if(diff_canceled) break;

		object=obj_itr.second;
		obj_type=object->getObjectType();
		idx++;

    /* If this checking the following objects are discarded:
       1) BASE_RELATIONSHIP objects
       2) Objects which SQL code is disabled or system objects
       3) Cluster objects such as roles and tablespaces (when the operatoin is DROP and keep_cluster_objs is true) */
    if(obj_type!=BASE_RELATIONSHIP &&
       !object->isSystemObject() && !object->isSQLDisabled() &&
			 ((diff_type==ObjectsDiffInfo::DROP_OBJECT && (!keep_cluster_objs || (keep_cluster_objs && obj_type!=OBJ_ROLE && obj_type!=OBJ_TABLESPACE))) ||
				(diff_type!=ObjectsDiffInfo::DROP_OBJECT)))
		{
      emit s_progressUpdated(prog + ((idx/static_cast<float>(obj_order.size())) * factor),
                             trUtf8("Processing object `%1' `(%2)'...").arg(object->getName()).arg(object->getTypeName()),
                             object->getObjectType());

      //Processing objects that are not database, table child object (they are processed further)
      if(obj_type!=OBJ_DATABASE && !TableObject::isTableObject(obj_type))// && obj_type!=BASE_RELATIONSHIP)
      {
        /* Processing permissions. If the operation is DROP and keep_obj_perms is true the
           the permission is ignored */
        if(obj_type==OBJ_PERMISSION &&
           ((diff_type!=ObjectsDiffInfo::DROP_OBJECT) || !keep_obj_perms))
          generateDiffInfo(diff_type, object);

        //Processing relationship (in this case only generalization ones are considered)
        else if(obj_type==OBJ_RELATIONSHIP)
        {
          Relationship *rel=dynamic_cast<Relationship *>(object);

          if(rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_GEN)
          {
            Table *ref_tab=nullptr, *rec_tab=nullptr;

            ref_tab=aux_model->getTable(rel->getReferenceTable()->getName(true));
            rec_tab=aux_model->getTable(rel->getReceiverTable()->getName(true));

            /* If the receiver table exists on the model generates a info for the relationship,
               otherwise, the generalization will be created automatically when the table is
               created (see table's code defintion) */
            if(rec_tab && !aux_model->getRelationship(ref_tab, rec_tab))
              generateDiffInfo(diff_type, rel);
          }
        }
        else if(obj_type!=OBJ_PERMISSION)
        {
          //Get the object from the database
          obj_name=object->getSignature();
          aux_object=aux_model->getObject(obj_name, obj_type);

          //Special case for many-to-many relationships
          if(obj_type==OBJ_TABLE && !aux_object)
            aux_object=getRelNNTable(obj_name, aux_model);

          if(diff_type!=ObjectsDiffInfo::DROP_OBJECT && aux_object)
          {
            /* Try to get a diff from the retrieve object and the current object,
               comparing only basic attributes like schema, tablespace and owner
               this is why the BaseObject::getAlterDefinition is called */
            objs_differs=!aux_object->BaseObject::getAlterDefinition(object).isEmpty();

            //If the objects does not differ, try to compare their XML definition
            if(!objs_differs)
              xml_differs=object->isCodeDiffersFrom(aux_object,
                                                    { ParsersAttributes::PROTECTED,
                                                      ParsersAttributes::SQL_DISABLED,
                                                      ParsersAttributes::RECT_VISIBLE,
                                                      ParsersAttributes::FILL_COLOR },
                                                    { ParsersAttributes::ROLE,
                                                      ParsersAttributes::TABLESPACE,
                                                      ParsersAttributes::COLLATION,
                                                      ParsersAttributes::POSITION,
                                                      ParsersAttributes::APPENDED_SQL,
                                                      ParsersAttributes::PREPENDED_SQL });

            //If a difference was detected between the objects
            if(objs_differs || xml_differs)
            {
              generateDiffInfo(ObjectsDiffInfo::ALTER_OBJECT, object, aux_object);

              //If the object is a table, do additional comparision between their child objects
              if((!force_recreation || recreate_unchangeble) && object->getObjectType()==OBJ_TABLE)
              {
                Table *tab=dynamic_cast<Table *>(object), *aux_tab=dynamic_cast<Table *>(aux_object);
                diffTables(tab, aux_tab, ObjectsDiffInfo::DROP_OBJECT);
                diffTables(tab, aux_tab, ObjectsDiffInfo::CREATE_OBJECT);
              }

              objs_differs=xml_differs=false;
            }
          }
          else if(!aux_object)           
            generateDiffInfo(diff_type, object);
        }
      }
      //Comparison for constraints (fks), triggers, rules, indexes
      else if(TableObject::isTableObject(obj_type))
        diffTableObject(dynamic_cast<TableObject *>(object), diff_type);
      //Comparison between model db and the imported db
      else if(diff_type==ObjectsDiffInfo::CREATE_OBJECT)
      {
        if(!source_model->getAlterDefinition(imported_model).isEmpty())
         generateDiffInfo(ObjectsDiffInfo::ALTER_OBJECT, source_model, imported_model);
      }
    }
    else
    {
      generateDiffInfo(ObjectsDiffInfo::IGNORE_OBJECT, object);
      emit s_progressUpdated(prog + ((idx/static_cast<float>(obj_order.size())) * factor),
                             trUtf8("Skipping object `%1' `(%2)'...").arg(object->getName()).arg(object->getTypeName()),
                             object->getObjectType());
    }

    QThread::msleep(15);
	}
}

void ModelsDiffHelper::diffTableObject(TableObject *tab_obj, unsigned diff_type)
{
	BaseTable *base_tab=nullptr, *aux_base_tab=nullptr;
	ObjectType obj_type=tab_obj->getObjectType();
  QString tab_name, obj_name=tab_obj->getName(true);
	BaseObject *aux_tab_obj=nullptr;

  //Get the parent table of the object
	base_tab=tab_obj->getParentTable();
  tab_name=base_tab->getSignature();

  //If the operation is a DROP, try to get the table from the source mode
	if(diff_type==ObjectsDiffInfo::DROP_OBJECT)
  {
    aux_base_tab=dynamic_cast<BaseTable *>(source_model->getObject(tab_name, base_tab->getObjectType()));

    //If the table was not found, try to find it between the many-to-many relationships
    if(!aux_base_tab)
     aux_base_tab=dynamic_cast<BaseTable *>(getRelNNTable(tab_name, source_model));
  }
	else if(diff_type==ObjectsDiffInfo::CREATE_OBJECT ||
					diff_type==ObjectsDiffInfo::ALTER_OBJECT)
  {
    aux_base_tab=dynamic_cast<BaseTable *>(imported_model->getObject(tab_name, base_tab->getObjectType()));

    //If the table was not found, try to find it between the many-to-many relationships
    if(!aux_base_tab)
     aux_base_tab=dynamic_cast<BaseTable *>(getRelNNTable(obj_name, imported_model));
  }

  if(aux_base_tab)
  {
    if(obj_type==OBJ_INDEX || obj_type==OBJ_CONSTRAINT)
    {
      Table *aux_table=dynamic_cast<Table *>(aux_base_tab);
      aux_tab_obj=aux_table->getObject(obj_name, obj_type);
    }
    else
      aux_tab_obj=aux_base_tab->getObject(obj_name, obj_type);
  }

	if(!aux_tab_obj)
		generateDiffInfo(diff_type, tab_obj);
	else if(diff_type!=ObjectsDiffInfo::DROP_OBJECT && tab_obj->isCodeDiffersFrom(aux_tab_obj))
    generateDiffInfo(ObjectsDiffInfo::ALTER_OBJECT, tab_obj, aux_tab_obj);
}

BaseObject *ModelsDiffHelper::getRelNNTable(const QString &obj_name, DatabaseModel *model)
{
  vector<BaseObject *> *rels=model->getObjectList(OBJ_RELATIONSHIP);
  Relationship *rel=nullptr;
  BaseObject *tab=nullptr;

  for(auto obj : *rels)
  {
    rel=dynamic_cast<Relationship *>(obj);
    if(rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_NN &&
       rel->getGeneratedTable() && rel->getGeneratedTable()->getSignature()==obj_name)
    {
      tab=rel->getGeneratedTable();
      break;
    }
  }

  return(tab);
}

void ModelsDiffHelper::generateDiffInfo(unsigned diff_type, BaseObject *object, BaseObject *old_object)
{
  if(object)
  {
    ObjectsDiffInfo diff_info;

    /* If the info is for ALTER and there is a DROP info on the list,
     the object will be recreated instead of modified */
    if((!force_recreation || recreate_unchangeble) &&
       diff_type==ObjectsDiffInfo::ALTER_OBJECT &&
       isDiffInfoExists(ObjectsDiffInfo::DROP_OBJECT, old_object, nullptr) &&
       !isDiffInfoExists(ObjectsDiffInfo::CREATE_OBJECT, object, nullptr))
    {
      diff_info=ObjectsDiffInfo(ObjectsDiffInfo::CREATE_OBJECT, object, nullptr);
      diff_infos.push_back(diff_info);
      diffs_counter[ObjectsDiffInfo::CREATE_OBJECT]++;
      emit s_objectsDiffInfoGenerated(diff_info);
    }
    else if(!isDiffInfoExists(diff_type, object, old_object))
    {
      /* Special case for columns marked with ALTER.
         If the type of them is "serial" or similar then a sequence will be created and the
         type of the column changed to "integer" or similar, this because the ALTER command
         for columns don't accept the type "serial" */
      if(diff_type==ObjectsDiffInfo::ALTER_OBJECT && object->getObjectType()==OBJ_COLUMN)
      {
        Column *col=dynamic_cast<Column *>(object),
               *old_col=dynamic_cast<Column *>(old_object);

        if(col->getType()!=old_col->getType() && col->getType().isSerialType())
        {
          Column *aux_col=new Column;
          Sequence *seq=new Sequence;
          BaseTable *tab=col->getParentTable();
          QString seq_name;

          //Configures the sequence
          seq->setName(QString("%1_%2_seq").arg(tab->getName()).arg(col->getName()));
          seq_name=PgModelerNS::generateUniqueName(seq, *imported_model->getObjectList(OBJ_SEQUENCE));
          seq->setName(seq_name);
          seq->setOwner(tab->getOwner());
          seq->setSchema(tab->getSchema());

          //Configure an auxiliary column with the same values of the original one
          (*aux_col)=(*col);
          aux_col->setDefaultValue("");
          //Setting the type as the alias of the serial type
          aux_col->setType(aux_col->getType().getAliasType());
          //Assigns the sequence to the column in order to configure the default value correctly
          aux_col->setSequence(seq);

          //Creates a new ALTER info with the created column
          diff_info=ObjectsDiffInfo(ObjectsDiffInfo::ALTER_OBJECT, aux_col, col);
          diff_infos.push_back(diff_info);
          diffs_counter[ObjectsDiffInfo::ALTER_OBJECT]++;
          emit s_objectsDiffInfoGenerated(diff_info);

          //Creates a CREATE info with the sequence
          diff_info=ObjectsDiffInfo(ObjectsDiffInfo::CREATE_OBJECT, seq, nullptr);
          diff_infos.push_back(diff_info);
          diffs_counter[ObjectsDiffInfo::CREATE_OBJECT]++;
          emit s_objectsDiffInfoGenerated(diff_info);

          /* Stores the created objects in the temp list in order to be destroyed at the
             end of the process. */
          tmp_objects.push_back(aux_col);
          tmp_objects.push_back(seq);
        }
      }
      else
      {
        diff_info=ObjectsDiffInfo(diff_type, object, old_object);
        diff_infos.push_back(diff_info);
        diffs_counter[diff_type]++;
        emit s_objectsDiffInfoGenerated(diff_info);
      }

      /* If the info is for DROP, generate the drop for referer objects of the
       one marked to be dropped */
      if((!force_recreation || recreate_unchangeble) &&
              diff_type==ObjectsDiffInfo::DROP_OBJECT)
      {
        vector<BaseObject *> ref_objs;
        imported_model->getObjectReferences(object, ref_objs);

        for(auto obj : ref_objs)
        {
          if(obj->getObjectType()!=BASE_RELATIONSHIP)
            generateDiffInfo(diff_type, obj);
        }
      }
    }
  }
}

bool ModelsDiffHelper::isDiffInfoExists(unsigned diff_type, BaseObject *object, BaseObject *old_object, bool exact_match)
{
  bool found_diff=false;
  ObjectsDiffInfo aux_diff(diff_type, object, old_object);

  for(ObjectsDiffInfo diff : diff_infos)
  {
    if((exact_match && diff==aux_diff) ||
       (!exact_match &&
         ((object && diff.getObject()==object) ||
          (old_object && diff.getOldObject()==old_object))))
    {
      found_diff=true;
      break;
    }
  }

  return(found_diff);
}

void ModelsDiffHelper::processDiffInfos(void)
{
  BaseObject *object=nullptr;
  Relationship *rel=nullptr;
  map<unsigned, QString> drop_objs, create_objs, alter_objs, truncate_tabs;
  vector<BaseObject *> drop_vect, create_vect;
  unsigned diff_type, schema_id=0;
  ObjectType obj_type;
  map<unsigned, QString>::reverse_iterator ritr, ritr_end;
  attribs_map attribs;
  QString alter_def, no_inherit_def, inherit_def, set_perms,
          unset_perms, fk_defs;
  SchemaParser schparser;
  Type *type=nullptr;
  vector<Type *> types;

  try
  {
    if(!diff_infos.empty())
      emit s_progressUpdated(90, trUtf8("Processing diff infos..."));

    //Separating the base types
    for(ObjectsDiffInfo diff : diff_infos)
    {
      type=dynamic_cast<Type *>(diff.getObject());

      if(type && type->getConfiguration()==Type::BASE_TYPE)
      {
        type->convertFunctionParameters();
        types.push_back(type);
      }
    }

    for(ObjectsDiffInfo diff : diff_infos)
    {
      diff_type=diff.getDiffType();
      object=diff.getObject();
      rel=dynamic_cast<Relationship *>(object);
      obj_type=object->getObjectType();

      if(diff_type==ObjectsDiffInfo::DROP_OBJECT)
      {
        if(rel)
          no_inherit_def+=rel->getInheritDefinition(true);
        else if(obj_type==OBJ_PERMISSION)
          unset_perms+=object->getDropDefinition(cascade_mode);
        else
          drop_objs[object->getObjectId()]=getCodeDefinition(object, true);
      }
      else if(diff_type==ObjectsDiffInfo::CREATE_OBJECT)
      {
        if(rel)
          inherit_def+=rel->getInheritDefinition(false);
        else if(obj_type==OBJ_PERMISSION)
          set_perms+=object->getCodeDefinition(SchemaParser::SQL_DEFINITION);
        else
        {
          //Generating fks definitions in a separated variable in order to append them at create commands maps
          if(object->getObjectType()==OBJ_CONSTRAINT &&
             dynamic_cast<Constraint *>(object)->getConstraintType()==ConstraintType::foreign_key)
            fk_defs+=getCodeDefinition(object, false);
          else
            create_objs[object->getObjectId()]=getCodeDefinition(object, false);
        }
      }
      else if(diff_type==ObjectsDiffInfo::ALTER_OBJECT)
      {
        //Recreating the object instead of generating an ALTER command for it
        if((force_recreation && obj_type!=OBJ_DATABASE) &&
           (!recreate_unchangeble || (recreate_unchangeble && !object->acceptsAlterCommand())))
        {
          recreateObject(object, drop_vect, create_vect);

          //Generating the drop for the object's reference
          for(auto obj : drop_vect)
            drop_objs[obj->getObjectId()]=getCodeDefinition(obj, true);

          //Generating the create for the object's reference
          for(auto obj : create_vect)
          {
            //The there is no ALTER info registered for an object's reference
            if(!isDiffInfoExists(ObjectsDiffInfo::ALTER_OBJECT, nullptr, obj, false))
             create_objs[obj->getObjectId()]=getCodeDefinition(obj, false);
          }

          drop_vect.clear();
          create_vect.clear();
        }
        else
        {
          if(diff.getOldObject())
            alter_def=diff.getOldObject()->getAlterDefinition(object);

          if(!alter_def.isEmpty())
          {
            alter_objs[object->getObjectId()]=alter_def;

            /* If the object is a column checks if the types of the columns are differents,
               generating a TRUNCATE TABLE for the parent table */
            if(obj_type==OBJ_COLUMN && trucante_tables)
            {
              Column *src_col=dynamic_cast<Column *>(object),
                     *imp_col=dynamic_cast<Column *>(diff.getOldObject());
              Table *tab=dynamic_cast<Table *>(src_col->getParentTable());

              //If the truncate was not generated previously
              if(src_col->getType()!=imp_col->getType() && truncate_tabs.count(tab->getObjectId())==0)
                truncate_tabs[tab->getObjectId()]=tab->getTruncateDefinition(cascade_mode);
            }
          }
        }
      }
    }

    //Creating the shell types declaration right below on the DDL that creates their schemas
    for(Type *type : types)
    {
      schema_id=type->getSchema()->getObjectId();

      if(create_objs.count(schema_id)!=0)
       create_objs[schema_id]+=type->getCodeDefinition(SchemaParser::SQL_DEFINITION, true);
      else
       attribs[ParsersAttributes::CREATE_CMDS]+=type->getCodeDefinition(SchemaParser::SQL_DEFINITION, true);

      type->convertFunctionParameters(true);
    }

    diff_def.clear();

    if(!drop_objs.empty() || !create_objs.empty() || !alter_objs.empty() ||
       !inherit_def.isEmpty() || !no_inherit_def.isEmpty() || !set_perms.isEmpty() ||
       !fk_defs.isEmpty())
    {
      //Attributes used on the diff schema file
      attribs[ParsersAttributes::HAS_CHANGES]=ParsersAttributes::_TRUE_;
      attribs[ParsersAttributes::PGMODELER_VERSION]=GlobalAttributes::PGMODELER_VERSION;
      attribs[ParsersAttributes::CHANGE]=QString::number(alter_objs.size());
      attribs[ParsersAttributes::CREATE]=QString::number(create_objs.size());
      attribs[ParsersAttributes::DROP]=QString::number(drop_objs.size());
      attribs[ParsersAttributes::TRUNCATE]=QString::number(truncate_tabs.size());
      attribs[ParsersAttributes::ALTER_CMDS]="";
      attribs[ParsersAttributes::DROP_CMDS]="";
      attribs[ParsersAttributes::CREATE_CMDS]="";
      attribs[ParsersAttributes::TRUNCATE_CMDS]="";
      attribs[ParsersAttributes::UNSET_PERMS]=unset_perms;
      attribs[ParsersAttributes::SET_PERMS]=set_perms;
      attribs[ParsersAttributes::FUNCTION]=(source_model->getObjectCount(OBJ_FUNCTION)!=0 ? ParsersAttributes::_TRUE_ : "");

      ritr=drop_objs.rbegin();
      ritr_end=drop_objs.rend();

      attribs[ParsersAttributes::DROP_CMDS]+=no_inherit_def;
      while(ritr!=ritr_end)
      {
        attribs[ParsersAttributes::DROP_CMDS]+=ritr->second;
        ritr++;
      }

      for(auto itr : create_objs)
        attribs[ParsersAttributes::CREATE_CMDS]+=itr.second;

      attribs[ParsersAttributes::CREATE_CMDS]+=fk_defs;
      attribs[ParsersAttributes::CREATE_CMDS]+=inherit_def;

      for(auto itr : truncate_tabs)
        attribs[ParsersAttributes::TRUNCATE_CMDS]+=itr.second;

      for(auto itr : alter_objs)
        attribs[ParsersAttributes::ALTER_CMDS]+=itr.second;

      //Generating the whole diff buffer
      schparser.setPgSQLVersion(pgsql_version);
      diff_def=schparser.getCodeDefinition(GlobalAttributes::SCHEMAS_ROOT_DIR + GlobalAttributes::DIR_SEPARATOR +
                                           GlobalAttributes::ALTER_SCHEMA_DIR + GlobalAttributes::DIR_SEPARATOR +
                                           ParsersAttributes::DIFF + GlobalAttributes::SCHEMA_EXT, attribs);
    }

    emit s_progressUpdated(100, trUtf8("Comparison between model and database finished."));
  }
  catch(Exception &e)
  {
    for(Type *type : types)
     type->convertFunctionParameters(true);

    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

QString ModelsDiffHelper::getCodeDefinition(BaseObject *object, bool drop_cmd)
{
  try
  {
    TableObject *tab_obj=dynamic_cast<TableObject *>(object);
    QString cmd;

    /* For columns and constraints it is needed to force the generation of
       ALTER commands on the parent table */
    if(tab_obj && (tab_obj->getObjectType()==OBJ_COLUMN || tab_obj->getObjectType()==OBJ_CONSTRAINT))
    {
      bool gen_alter=false;
      Table *table=dynamic_cast<Table *>(tab_obj->getParentTable());

      gen_alter=table->isGenerateAlterCmds();
      table->setGenerateAlterCmds(true);

      if(drop_cmd)
        cmd=tab_obj->getDropDefinition(cascade_mode);
      else
        cmd=tab_obj->getCodeDefinition(SchemaParser::SQL_DEFINITION);

      table->setGenerateAlterCmds(gen_alter);
    }
    else
    {
      if(drop_cmd)
       cmd=object->getDropDefinition(cascade_mode);
      else
       cmd=object->getCodeDefinition(SchemaParser::SQL_DEFINITION);
    }

    return(cmd);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

void ModelsDiffHelper::destroyTempObjects(void)
{
  BaseObject *tmp_obj=nullptr;

  while(!tmp_objects.empty())
  {
    tmp_obj=tmp_objects.back();
    tmp_objects.pop_back();
    delete(tmp_obj);
  }

  diff_infos.clear();
}

void ModelsDiffHelper::recreateObject(BaseObject *object, vector<BaseObject *> &drop_objs, vector<BaseObject *> &create_objs)
{
  if(object &&
     object->getObjectType()!=BASE_RELATIONSHIP &&
     object->getObjectType()!=OBJ_RELATIONSHIP)
  {
    vector<BaseObject *> ref_objs;
    BaseObject *aux_obj=nullptr;

    /* If the specified object is not a table's child object,
       try to get an object from database which name is the same as 'object' */
    if(!TableObject::isTableObject(object->getObjectType()))
      aux_obj=imported_model->getObject(object->getSignature(), object->getObjectType());
    else
    {
      TableObject *tab_obj=dynamic_cast<TableObject *>(object);

      if(tab_obj->getParentTable())
      {
        /* If the object is a table's child object, first we get a table from the database which
           name is the same as the 'object' parent table's name. Sencond, we try to get a child
           object from that table using the same name as the 'object' */
        BaseTable *tab=dynamic_cast<BaseTable *>(imported_model->getObject(tab_obj->getParentTable()->getSignature(), tab_obj->getParentTable()->getObjectType()));
        aux_obj=tab->getObject(tab_obj->getName(true), tab_obj->getObjectType());
      }
    }

    //Get all references to the retrieved object on the database
    imported_model->getObjectReferences(aux_obj, ref_objs, false, true);

    //Register a drop info for the object found on the database
    if(aux_obj)
      drop_objs.push_back(aux_obj);

    create_objs.push_back(object);

    //Executing the recreation of the object's references
    for(auto obj : ref_objs)
      recreateObject(obj, drop_objs, create_objs);
  }
}
