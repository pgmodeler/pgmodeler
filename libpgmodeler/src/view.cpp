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

#include "view.h"

View::View(void) : BaseTable()
{
	obj_type=OBJ_VIEW;
	attributes[ParsersAttributes::DECLARATION]="";
	attributes[ParsersAttributes::REFERENCES]="";
	attributes[ParsersAttributes::SELECT_EXP]="";
	attributes[ParsersAttributes::FROM_EXP]="";
	attributes[ParsersAttributes::SIMPLE_EXP]="";
	attributes[ParsersAttributes::CTE_EXPRESSION]="";
}

void View::setCommomTableExpression(const QString &expr)
{
	cte_expression=expr;
}

bool View::hasDefinitionExpression(void)
{
	vector<Reference>::iterator itr;
	bool found=false;

	itr=references.begin();
	while(itr!=references.end() && !found)
	{
		found=((*itr).isDefinitionExpression());
		itr++;
	}

	return(found);
}

QString View::getCommomTableExpression(void)
{
	return(cte_expression);
}

int View::getReferenceIndex(Reference &refer)
{
	vector<Reference>::iterator itr, itr_end;
	bool found=false;
	int idx=-1;

	itr=references.begin();
	itr_end=references.end();

	while(itr!=itr_end && !found)
	{
		found=((*itr)==refer);
		itr++;
		idx++;
	}

	if(!found) idx=-1;
	return(idx);
}

vector<unsigned> *View::getExpressionList(unsigned sql_type)
{
	if(sql_type==Reference::SQL_REFER_SELECT)
		return(&exp_select);
	else if(sql_type==Reference::SQL_REFER_FROM)
		return(&exp_from);
	else if(sql_type==Reference::SQL_REFER_WHERE)
		return(&exp_where);
	else
		return(NULL);
}

void View::addReference(Reference &refer, unsigned sql_type, int expr_id)
{
	int idx;
	vector<unsigned> *expr_list=NULL;
	Column *col=NULL;

	//Specific tests for expressions used as view definition
	if(sql_type==Reference::SQL_VIEW_DEFINITION)
	{
		//Raises an error if the expression is empty
		if(refer.getExpression().isEmpty())
			throw Exception(ERR_INV_VIEW_DEF_EXPRESSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Raises an error if already exists a definition expression
		else if(hasDefinitionExpression())
			throw Exception(ERR_ASG_SEC_VIEW_DEF_EXPRESSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		//Raises an error if the user try to add a definition expression when already exists another references
		else if(!references.empty())
			throw Exception(ERR_MIX_VIEW_DEF_EXPR_REFS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	//Raises an error if the user try to add a ordinary reference when there is a reference used as definition expression
	else if(hasDefinitionExpression())
		throw Exception(ERR_MIX_VIEW_DEF_EXPR_REFS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Checks if the reference already exists
	idx=getReferenceIndex(refer);

	//If not exists
	if(idx < 0)
	{
		//Inserts the reference on the view
		refer.setDefinitionExpression(sql_type==Reference::SQL_VIEW_DEFINITION);
		references.push_back(refer);
		idx=references.size()-1;
	}

	if(sql_type!=Reference::SQL_VIEW_DEFINITION)
	{
		//Gets the expression list
		expr_list=getExpressionList(sql_type);

		//Inserts the reference id on the expression list
		if(expr_id >= 0 && expr_id < static_cast<int>(expr_list->size()))
			expr_list->insert(expr_list->begin() + expr_id, static_cast<unsigned>(idx));
		//Raises an error if the expression id is invalid
		else if(expr_id >= 0 && expr_id >= static_cast<int>(expr_list->size()))
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else
			expr_list->push_back(static_cast<unsigned>(idx));

		col=refer.getColumn();
		if(col && col->isAddedByRelationship() &&
			 col->getObjectId() > this->object_id)
			this->object_id=BaseObject::getGlobalId();
	}
}

unsigned View::getReferenceCount(void)
{
	return(references.size());
}

unsigned View::getReferenceCount(unsigned sql_type, int ref_type)
{
	vector<unsigned> *vect_idref=getExpressionList(sql_type);

	if(!vect_idref)
	{
		if(sql_type==Reference::SQL_VIEW_DEFINITION)
			return(references.size());
		else
			return(0);
	}
	else
	{
		if(ref_type < 0)
			return(vect_idref->size());
		else
		{
			vector<unsigned>::iterator itr, itr_end;
			unsigned count=0;


			itr=vect_idref->begin();
			itr_end=vect_idref->end();
			while(itr!=itr_end)
			{
				if(references[(*itr)].getReferenceType()==static_cast<unsigned>(ref_type)) count++;
				itr++;
			}

			return(count);
		}
	}
}

Reference View::getReference(unsigned ref_id)
{
	//Raises an error if the reference id is out of bound
	if(ref_id >= references.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(references[ref_id]);
}

Reference View::getReference(unsigned ref_id, unsigned sql_type)
{
	vector<unsigned> *vect_idref=getExpressionList(sql_type);

	//Raises an error if the reference id is out of bound
	if(ref_id >= references.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(sql_type==Reference::SQL_VIEW_DEFINITION || vect_idref)
		return(references[ref_id]);
	else
		return(references[vect_idref->at(ref_id)]);
}

void View::removeReference(unsigned ref_id)
{
	vector<unsigned> *vect_idref[3]={&exp_select, &exp_from, &exp_where};
	vector<unsigned>::iterator itr, itr_end;
	unsigned i;

	//Raises an error if the reference id is out of bound
	if(ref_id >= references.size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	for(i=0; i < 3; i++)
	{
		itr=vect_idref[i]->begin();
		itr_end=vect_idref[i]->end();

		while(itr!=itr_end && !vect_idref[i]->empty())
		{
			//Removes the reference id from the expression list
			if(references[*itr]==references[ref_id])
				vect_idref[i]->erase(itr);

			itr++;
		}
	}

	//Removes the reference from the view
	references.erase(references.begin() + ref_id);
}

void View::removeReferences(void)
{
	references.clear();
	exp_select.clear();
	exp_from.clear();
	exp_where.clear();
}

void View::removeReference(unsigned expr_id, unsigned sql_type)
{
	vector<unsigned> *vect_idref=getExpressionList(sql_type);

	if(expr_id >= vect_idref->size())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vect_idref->erase(vect_idref->begin() + expr_id);
}

int View::getReferenceIndex(Reference &ref, unsigned sql_type)
{
	vector<unsigned> *vet_idref=getExpressionList(sql_type);
	vector<unsigned>::iterator itr, itr_end;
	int idx_ref;
	bool found=false;

	idx_ref=getReferenceIndex(ref);

	if(sql_type==Reference::SQL_VIEW_DEFINITION &&
		 idx_ref >=0 && ref.isDefinitionExpression())
		return(idx_ref);
	else if(sql_type!=Reference::SQL_VIEW_DEFINITION)
	{
		itr=vet_idref->begin();
		itr_end=vet_idref->end();

		while(itr!=itr_end && !found)
		{
			found=(static_cast<int>(*itr)==idx_ref);
			if(!found) itr++;
		}

		if(!found)
			return(-1);
		else
			return(itr-vet_idref->begin());
	}
	else
		return(-1);
}

void View::setDeclarationAttribute(void)
{
	QString decl;

	if(!references.empty())
	{
		if(exp_select.empty())
		{
			decl=references[0].getExpression();
		}
		else
		{
			vector<unsigned> *refs_vect[3]={&exp_select, &exp_from, &exp_where};
			vector<unsigned>::iterator itr, itr_end;
			QString palavras[3]={"SELECT ", "\n FROM ", "\n WHERE "};
			unsigned i, qtd, idx, tipo_sql[3]={Reference::SQL_REFER_SELECT,
																				 Reference::SQL_REFER_FROM,
																				 Reference::SQL_REFER_WHERE};

			for(i=0; i < 3; i++)
			{
				if(refs_vect[i]->size() > 0)
				{
					decl+=palavras[i];

					itr=refs_vect[i]->begin();
					itr_end=refs_vect[i]->end();
					while(itr!=itr_end)
					{
						idx=(*itr);
						decl+=references[idx].getSQLDefinition(tipo_sql[i]);
						itr++;
					}

					if(tipo_sql[i]==Reference::SQL_REFER_SELECT ||
						 tipo_sql[i]==Reference::SQL_REFER_FROM)
					{
						//Removing the final comma from SELECT / FROM declarations
						qtd=decl.size();
						if(decl[qtd-2]==',')
							decl.remove(qtd-2,2);
					}
				}
			}
		}
	}

	attributes[ParsersAttributes::DECLARATION]=decl;
}

void View::setReferencesAttribute(void)
{
	QString str_aux;
	QString attribs[]={ ParsersAttributes::SELECT_EXP,
											ParsersAttributes::FROM_EXP,
											ParsersAttributes::SIMPLE_EXP };
	vector<unsigned> *vect_exp[]={&exp_select, &exp_from, &exp_where};
	int qtd, i, i1;

	qtd=references.size();
	for(i=0; i < qtd; i++)
		str_aux+=references[i].getXMLDefinition();
	attributes[ParsersAttributes::REFERENCES]=str_aux;

	for(i=0; i < 3; i++)
	{
		str_aux="";
		qtd=vect_exp[i]->size();
		for(i1=0; i1 < qtd; i1++)
		{
			str_aux+=QString("%1").arg(vect_exp[i]->at(i1));
			if(i1 < qtd-1) str_aux+=",";
		}
		attributes[attribs[i]]=str_aux;
	}
}

bool View::isReferRelationshipAddedColumn(void)
{
	Column *column=NULL;
	unsigned count, i;
	bool found=false;

	count=references.size();

	for(i=0; i < count && !found; i++)
	{
		column=references[i].getColumn();
		found=(column && column->isAddedByRelationship());
	}

	return(found);
}

bool View::isReferencingTable(Table *tab)
{
	Table *aux_tab=NULL;
	unsigned count, i;
	bool found=false;

	count=references.size();

	for(i=0; i < count && !found; i++)
	{
		aux_tab=references[i].getTable();
		found=(aux_tab && (aux_tab == tab));
	}

	return(found);
}

bool View::isReferencingColumn(Column *col)
{
	unsigned count, i;
	bool found=false;

	if(col)
	{
		count=references.size();
		for(i=0; i < count && !found; i++)
			found=(col==references[i].getColumn());
	}
	return(found);
}

QString View::getCodeDefinition(unsigned def_type)
{
	attributes[ParsersAttributes::CTE_EXPRESSION]=cte_expression;

	if(def_type==SchemaParser::SQL_DEFINITION)
		setDeclarationAttribute();
	else
	{
		setPositionAttribute();
		setReferencesAttribute();
	}

	return(BaseObject::__getCodeDefinition(def_type));
}

void View::operator = (View &view)
{
	(*dynamic_cast<BaseGraphicObject *>(this))=reinterpret_cast<BaseGraphicObject &>(view);

	this->references=view.references;
	this->exp_select=view.exp_select;
	this->exp_from=view.exp_from;
	this->exp_where=view.exp_where;
	this->cte_expression=view.cte_expression;
}

