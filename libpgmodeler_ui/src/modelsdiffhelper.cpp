/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

ModelsDiffHelper::ModelsDiffHelper(void)
{
  pgsql_version=SchemaParser::PGSQL_VERSION_94;
	source_model=imported_model=nullptr;
  resetDiffCounter();
  setDiffOptions(true, true, false, false, false);
}

void ModelsDiffHelper::setDiffOptions(bool keep_cluster_objs, bool cascade_mode, bool truncate_tables, bool force_recreation, bool recreate_unchangeble)
{
  this->keep_cluster_objs=keep_cluster_objs;
  this->cascade_mode=cascade_mode;
  this->force_recreation=force_recreation;
  this->trucante_tables=truncate_tables;
  this->recreate_unchangeble=(force_recreation && recreate_unchangeble);
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
		resetDiffCounter();

		if(!source_model || !imported_model)
			throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		diffModels(ObjectsDiffInfo::DROP_OBJECT);
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
    ref_tab=imp_table;
    comp_tab=src_table;
  }
  else if(diff_type==ObjectsDiffInfo::CREATE_OBJECT ||
          diff_type==ObjectsDiffInfo::ALTER_OBJECT)
  {
    ref_tab=src_table;
    comp_tab=imp_table;
  }

  for(unsigned i=0; i < 2; i++)
  {
    tab_objs=ref_tab->getObjectList(types[i]);

    for(auto tab_obj : *tab_objs)
    {
      aux_obj=comp_tab->getObject(tab_obj->getName(), tab_obj->getObjectType());
      constr=dynamic_cast<Constraint *>(tab_obj);

      if(aux_obj && diff_type!=ObjectsDiffInfo::DROP_OBJECT &&
         ((tab_obj->isAddedByGeneralization() || !tab_obj->isAddedByLinking()) ||
          (constr && constr->getConstraintType()!=ConstraintType::foreign_key)))
      {
        if(tab_obj->isCodeDiffersFrom(aux_obj))
          generateDiffInfo(ObjectsDiffInfo::ALTER_OBJECT, tab_obj, aux_obj);
      }
      else if(!aux_obj)
        generateDiffInfo(diff_type, tab_obj);
    }
  }
}

void ModelsDiffHelper::diffModels(unsigned diff_type)
{
	map<unsigned, BaseObject *> obj_order;
	BaseObject *object=nullptr, *aux_object=nullptr;
	ObjectType obj_type;
  QString obj_name;
	unsigned idx=0;
	DatabaseModel *aux_model=nullptr;
  bool objs_differs=false, xml_differs=false;

	if(diff_type==ObjectsDiffInfo::DROP_OBJECT)
	{
		obj_order=imported_model->getCreationOrder(SchemaParser::SQL_DEFINITION);
    aux_model=source_model;
	}
	else if(diff_type==ObjectsDiffInfo::CREATE_OBJECT ||
					diff_type==ObjectsDiffInfo::ALTER_OBJECT)
	{
		obj_order=source_model->getCreationOrder(SchemaParser::SQL_DEFINITION);
		aux_model=imported_model;
	}

	for(auto obj_itr : obj_order)
	{
		if(diff_canceled) break;

		object=obj_itr.second;
		obj_type=object->getObjectType();
		idx++;

    if(obj_type!=BASE_RELATIONSHIP &&
       !object->isSystemObject() && !object->isSQLDisabled() &&
			 ((diff_type==ObjectsDiffInfo::DROP_OBJECT && (!keep_cluster_objs || (keep_cluster_objs && obj_type!=OBJ_ROLE && obj_type!=OBJ_TABLESPACE))) ||
				(diff_type!=ObjectsDiffInfo::DROP_OBJECT)))
		{
			emit s_progressUpdated((idx/static_cast<float>(obj_order.size())) * 100,
														 trUtf8("Processing object `%1' `(%2)'...").arg(object->getName()).arg(object->getTypeName()),
														 object->getObjectType());

      if(obj_type!=OBJ_DATABASE && !TableObject::isTableObject(obj_type) && obj_type!=BASE_RELATIONSHIP)
			{
        if(obj_type==OBJ_RELATIONSHIP)
        {
          Relationship *rel=dynamic_cast<Relationship *>(object);

          if(rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_GEN)
          {
            Table *ref_tab=nullptr, *rec_tab=nullptr;

            ref_tab=aux_model->getTable(rel->getReferenceTable()->getName(true));
            rec_tab=aux_model->getTable(rel->getReceiverTable()->getName(true));

            if(rec_tab && !aux_model->getRelationship(ref_tab, rec_tab))
              generateDiffInfo(diff_type, rel);
          }
        }
        else
        {
          obj_name=object->getSignature();
          aux_object=aux_model->getObject(obj_name, obj_type);

          if(diff_type!=ObjectsDiffInfo::DROP_OBJECT && aux_object)
          {
            QString alter_def=aux_object->BaseObject::getAlterDefinition(object);
            objs_differs=!alter_def.isEmpty();

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

            if(objs_differs || xml_differs)
            {
              generateDiffInfo(ObjectsDiffInfo::ALTER_OBJECT, object, aux_object);

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
			emit s_progressUpdated((idx/static_cast<float>(obj_order.size())) * 100,
														 trUtf8("Skipping object `%1' `(%2)'...").arg(object->getName()).arg(object->getTypeName()),
														 object->getObjectType());
		}

		QThread::msleep(10);
	}
}

void ModelsDiffHelper::diffTableObject(TableObject *tab_obj, unsigned diff_type)
{
	BaseTable *base_tab=nullptr, *aux_base_tab=nullptr;
	ObjectType obj_type=tab_obj->getObjectType();
	QString obj_name=tab_obj->getName(true);
	BaseObject *aux_tab_obj=nullptr;

	base_tab=tab_obj->getParentTable();

	if(diff_type==ObjectsDiffInfo::DROP_OBJECT)
		aux_base_tab=dynamic_cast<BaseTable *>(source_model->getObject(base_tab->getName(true), base_tab->getObjectType()));
	else if(diff_type==ObjectsDiffInfo::CREATE_OBJECT ||
					diff_type==ObjectsDiffInfo::ALTER_OBJECT)
		aux_base_tab=dynamic_cast<BaseTable *>(imported_model->getObject(base_tab->getName(true), base_tab->getObjectType()));

	if(obj_type==OBJ_INDEX || obj_type==OBJ_CONSTRAINT)
	{
		Table *aux_table=dynamic_cast<Table *>(aux_base_tab);
		if(aux_table)
			aux_tab_obj=aux_table->getObject(obj_name, obj_type);
	}
	else
	{
		if(aux_base_tab)
			aux_tab_obj=aux_base_tab->getObject(obj_name, obj_type);
	}

	if(!aux_tab_obj)
		generateDiffInfo(diff_type, tab_obj);
	else if(diff_type!=ObjectsDiffInfo::DROP_OBJECT && tab_obj->isCodeDiffersFrom(aux_tab_obj))
    generateDiffInfo(ObjectsDiffInfo::ALTER_OBJECT, tab_obj, aux_tab_obj);
}

void ModelsDiffHelper::generateDiffInfo(unsigned diff_type, BaseObject *object, BaseObject *old_object)
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
    diff_info=ObjectsDiffInfo(diff_type, object, old_object);
    diff_infos.push_back(diff_info);
    diffs_counter[diff_type]++;
    emit s_objectsDiffInfoGenerated(diff_info);

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
  try
  {
    BaseObject *object=nullptr;
    Relationship *rel=nullptr;
    map<unsigned, QString> drop_objs, create_objs, alter_objs, truncate_tabs;
    vector<BaseObject *> drop_vect, create_vect;
    unsigned diff_type;
    ObjectType obj_type;
    map<unsigned, QString>::reverse_iterator ritr, ritr_end;
    attribs_map attribs;
    QString alter_def, no_inherit_def, inherit_def, set_perms, unset_perms;
    SchemaParser schparser;

    if(!diff_infos.empty())
      emit s_progressUpdated(90, trUtf8("Processing diff infos..."));

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
          create_objs[object->getObjectId()]=getCodeDefinition(object, false);
      }
      else if(diff_type==ObjectsDiffInfo::ALTER_OBJECT)
      {
        if((force_recreation && obj_type!=OBJ_DATABASE) &&
           (!recreate_unchangeble || (recreate_unchangeble && !object->acceptsAlterCommand())))
        {
          recreateObject(object, drop_vect, create_vect);

          for(auto obj : drop_vect)
            drop_objs[obj->getObjectId()]=getCodeDefinition(obj, true);

          for(auto obj : create_vect)
          {
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

    diff_def.clear();

    if(!drop_objs.empty() || !create_objs.empty() || !alter_objs.empty())
    {
      attribs[ParsersAttributes::HAS_CHANGES]="1";
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
      attribs[ParsersAttributes::FUNCTION]=(source_model->getObjectCount(OBJ_FUNCTION)!=0 ? "1" : "");

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
      attribs[ParsersAttributes::CREATE_CMDS]+=inherit_def;

      for(auto itr : truncate_tabs)
        attribs[ParsersAttributes::TRUNCATE_CMDS]+=itr.second;

      for(auto itr : alter_objs)
        attribs[ParsersAttributes::ALTER_CMDS]+=itr.second;

      schparser.setPgSQLVersion(pgsql_version);
      diff_def=schparser.getCodeDefinition(GlobalAttributes::SCHEMAS_ROOT_DIR + GlobalAttributes::DIR_SEPARATOR +
                                           GlobalAttributes::ALTER_SCHEMA_DIR + GlobalAttributes::DIR_SEPARATOR +
                                           ParsersAttributes::DIFF + GlobalAttributes::SCHEMA_EXT, attribs);
    }
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

QString ModelsDiffHelper::getCodeDefinition(BaseObject *object, bool drop_cmd)
{
  try
  {
    TableObject *tab_obj=dynamic_cast<TableObject *>(object);
    QString cmd;

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

void ModelsDiffHelper::recreateObject(BaseObject *object, vector<BaseObject *> &drop_objs, vector<BaseObject *> &create_objs)
{
  if(object &&
     object->getObjectType()!=BASE_RELATIONSHIP &&
     object->getObjectType()!=OBJ_RELATIONSHIP)
  {
    vector<BaseObject *> ref_objs;
    BaseObject *aux_obj=nullptr;

    if(!TableObject::isTableObject(object->getObjectType()))
      aux_obj=imported_model->getObject(object->getSignature(), object->getObjectType());
    else
    {
      TableObject *tab_obj=dynamic_cast<TableObject *>(object);

      if(tab_obj->getParentTable())
      {
        BaseTable *tab=dynamic_cast<BaseTable *>(imported_model->getObject(tab_obj->getParentTable()->getSignature(), tab_obj->getParentTable()->getObjectType()));
        aux_obj=tab->getObject(tab_obj->getName(true), tab_obj->getObjectType());
      }
    }

    imported_model->getObjectReferences(aux_obj, ref_objs, false, true);

    if(aux_obj)
      drop_objs.push_back(aux_obj);

    create_objs.push_back(object);

    for(auto obj : ref_objs)
      recreateObject(obj, drop_objs, create_objs);
  }
}
