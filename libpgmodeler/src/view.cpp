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
	attributes[ParsersAttributes::EXP_SIMPLES]="";
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
	else
		return(&exp_where);
}

void View::addReference(Reference &refer, unsigned sql_type, int expr_id)
{
	int idx;
	vector<unsigned> *expr_list=NULL;
	Column *col=NULL;

	//Checks if the reference already exists
	idx=getReferenceIndex(refer);

	//If not exists
	if(idx < 0)
	{
		//Inserts the reference on the view
		references.push_back(refer);
		idx=references.size()-1;
	}

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

unsigned View::getReferenceCount(void)
{
	return(references.size());
}

unsigned View::getReferenceCount(unsigned sql_type, int ref_type)
{
	vector<unsigned> *vect_idref=getExpressionList(sql_type);

	if(ref_type < 0)
		return(vect_idref->size());
	else
	{
		vector<unsigned>::iterator itr, itr_end;
		unsigned qtd;

		qtd=0;
		itr=vect_idref->begin();
		itr_end=vect_idref->end();
		while(itr!=itr_end)
		{
			if(references[(*itr)].getReferenceType()==static_cast<unsigned>(ref_type)) qtd++;
			itr++;
		}

		return(qtd);
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
	vector<unsigned>::iterator itr, itr_aux, itr_end;
	int idx_ref;
	bool found=false;

	idx_ref=getReferenceIndex(ref);
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

void View::setDeclarationAttribute(void)
{
	QString decl;

	if(exp_select.size() > 0)
	{
		vector<unsigned> *vet_ref[3]={&exp_select, &exp_from, &exp_where};
		vector<unsigned>::iterator itr, itr_end;
		QString palavras[3]={"SELECT ", "\n FROM ", "\n WHERE "};
		unsigned i, qtd, idx, tipo_sql[3]={Reference::SQL_REFER_SELECT,
																			 Reference::SQL_REFER_FROM,
																			 Reference::SQL_REFER_WHERE};

		for(i=0; i < 3; i++)
		{
			if(vet_ref[i]->size() > 0)
			{
				decl+=palavras[i];

				itr=vet_ref[i]->begin();
				itr_end=vet_ref[i]->end();
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
	attributes[ParsersAttributes::DECLARATION]=decl;
}

void View::setReferencesAttribute(void)
{
	QString str_aux;
	QString attribs[]={ ParsersAttributes::SELECT_EXP,
											ParsersAttributes::FROM_EXP,
											ParsersAttributes::EXP_SIMPLES };
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
}

