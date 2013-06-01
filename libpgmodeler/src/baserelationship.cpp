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

#include "baserelationship.h"
#include <QApplication>

BaseRelationship::BaseRelationship(BaseRelationship *rel)
{
	if(!rel)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	for(unsigned i=0; i < 3; i++)
		lables[i]=nullptr;

	(*(this))=(*rel);
}

BaseRelationship::BaseRelationship(unsigned rel_type, BaseTable *src_tab, BaseTable *dst_tab,
																	 bool src_mandatory, bool dst_mandatory)

{
	try
	{
		QString str_aux;

		this->connected=false;
		this->src_mandatory=src_mandatory;
		this->dst_mandatory=dst_mandatory;
		this->src_table=src_tab;
		this->dst_table=dst_tab;
		this->rel_type=rel_type;

		for(unsigned i=0; i < 3; i++)
		{
			lables[i]=nullptr;
			lables_dist[i]=QPointF(NAN, NAN);
		}

		configureRelationship();

		str_aux=QApplication::translate("BaseRelationship","rel_%1_%2","")
						.arg(src_tab->getName()).arg(dst_tab->getName());

		if(str_aux.size() > BaseObject::OBJECT_NAME_MAX_LENGTH)
			str_aux.resize(BaseObject::OBJECT_NAME_MAX_LENGTH);

		setName(str_aux);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void BaseRelationship::configureRelationship(void)
{
	obj_type=BASE_RELATIONSHIP;

	attributes[ParsersAttributes::TYPE]="";
	attributes[ParsersAttributes::SRC_REQUIRED]="";
	attributes[ParsersAttributes::DST_REQUIRED]="";
	attributes[ParsersAttributes::SRC_TABLE]="";
	attributes[ParsersAttributes::DST_TABLE]="";
	attributes[ParsersAttributes::POINTS]="";
	attributes[ParsersAttributes::COLUMNS]="";
	attributes[ParsersAttributes::CONSTRAINTS]="";
	attributes[ParsersAttributes::ELEMENTS]="";
	attributes[ParsersAttributes::IDENTIFIER]="";
	attributes[ParsersAttributes::REDUCED_FORM]="";
	attributes[ParsersAttributes::DEFERRABLE]="";
	attributes[ParsersAttributes::DEFER_TYPE]="";
	attributes[ParsersAttributes::TABLE_NAME]="";
	attributes[ParsersAttributes::SPECIAL_PK_COLS]="";
	attributes[ParsersAttributes::RELATIONSHIP_NN]="";
	attributes[ParsersAttributes::RELATIONSHIP_GEN]="";
	attributes[ParsersAttributes::RELATIONSHIP_DEP]="";
	attributes[ParsersAttributes::RELATIONSHIP_1N]="";
	attributes[ParsersAttributes::RELATIONSHIP_11]="";
	attributes[ParsersAttributes::CONSTRAINTS]="";
	attributes[ParsersAttributes::TABLE]="";
	attributes[ParsersAttributes::ANCESTOR_TABLE]="";
	attributes[ParsersAttributes::COPY_OPTIONS]="";
	attributes[ParsersAttributes::COPY_MODE]="";
	attributes[ParsersAttributes::SRC_COL_PATTERN]="";
	attributes[ParsersAttributes::DST_COL_PATTERN]="";
	attributes[ParsersAttributes::PK_PATTERN]="";
	attributes[ParsersAttributes::UQ_PATTERN]="";
	attributes[ParsersAttributes::SRC_FK_PATTERN]="";
	attributes[ParsersAttributes::DST_FK_PATTERN]="";
	attributes[ParsersAttributes::COL_INDEXES]="";
	attributes[ParsersAttributes::CONSTR_INDEXES]="";
	attributes[ParsersAttributes::ATTRIB_INDEXES]="";

	//Check if the relationship type is valid
	if(rel_type <= RELATIONSHIP_FK)
	{
		//Raises an error if one of the tables is not allocated
		if(!src_table || !dst_table)
			throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_TABLE)
											.arg(Utf8String::create(this->getName()))
											.arg(BaseObject::getTypeName(BASE_RELATIONSHIP)),
											ERR_ASG_NOT_ALOC_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Raises an error if the relationship type is generalization or dependency
			and the source and destination table are the same. */
		if((rel_type==RELATIONSHIP_GEN ||
				rel_type==RELATIONSHIP_DEP) && src_table==dst_table)
			throw Exception(ERR_INV_INH_COPY_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Allocates the textbox for the name label
		lables[REL_NAME_LABEL]=new Textbox;
		lables[REL_NAME_LABEL]->setTextAttribute(Textbox::ITALIC_TXT, true);

		//Allocates the cardinality labels only when the relationship is not generalization or dependency (copy)
		if(rel_type!=RELATIONSHIP_GEN &&
			 rel_type!=RELATIONSHIP_DEP)
		{
			lables[SRC_CARD_LABEL]=new Textbox;
			lables[DST_CARD_LABEL]=new Textbox;
			lables[SRC_CARD_LABEL]->setTextAttribute(Textbox::ITALIC_TXT, true);
			lables[DST_CARD_LABEL]->setTextAttribute(Textbox::ITALIC_TXT, true);

			//Configures the mandatory participation for both tables
			setMandatoryTable(SRC_TABLE,src_mandatory);
			setMandatoryTable(DST_TABLE,dst_mandatory);
		}
	}
	else
		//Raises an error if the specified relationship typ is invalid
		throw Exception(ERR_ALOC_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

BaseRelationship::~BaseRelationship(void)
{
	disconnectRelationship();

	//Unallocates the labels
	for(unsigned i=0; i<3; i++)
		if(lables[i]) delete(lables[i]);
}

void BaseRelationship::setName(const QString &name)
{
	try
	{
		BaseObject::setName(name);

		if(lables[REL_NAME_LABEL])
			lables[REL_NAME_LABEL]->setComment(name);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void BaseRelationship::setMandatoryTable(unsigned table_id, bool value)
{
	QString cmin, aux;
	unsigned label_id;

	/* Raises an error if the user tries to create an relationship
		One to One where both tables are mandatory partitipation
		(1,1)-<>-(1,1). This type of relationship is not implemented because
		it requires the table fusion. */
	if(rel_type==RELATIONSHIP_11 &&
		 ((table_id==SRC_TABLE && value && dst_mandatory) ||
			(table_id==DST_TABLE && value && src_mandatory)))
		throw Exception(ERR_NOT_IMPL_REL_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Case the source table is mandatory
	if(table_id==SRC_TABLE)
	{
		src_mandatory=value;
		//Indicates that the source cardinality label will be configured
		label_id=SRC_CARD_LABEL;
	}
	else
	{
		if(rel_type!=RELATIONSHIP_1N)
			dst_mandatory=value;
		else
			/* For One to many (1-n) relationship the entity on the "many" side
			will be always in the format (0,n) */
			dst_mandatory=false;

		//Indicates that the destination cardinality label will be configured
		label_id=DST_CARD_LABEL;
	}

	if(!value) cmin="0";
	else cmin="1";

	if(lables[label_id])
	{
		if(rel_type==RELATIONSHIP_11)
			lables[label_id]->setComment("(" + cmin + ",1)");
		else if(rel_type==RELATIONSHIP_1N)
		{
			aux=(table_id==SRC_TABLE ? "1" : "n");
			lables[label_id]->setComment("(" + cmin + "," + aux + ")");
		}
		else if(rel_type==RELATIONSHIP_FK)
		{
			aux=(table_id==SRC_TABLE ? "n" : "1");
			lables[label_id]->setComment("(" + aux + ")");
		}
		else if(rel_type==RELATIONSHIP_NN)
			lables[label_id]->setComment("(n)");

		lables[label_id]->setModified(true);
	}
}

BaseTable *BaseRelationship::getTable(unsigned table_id)
{
	if(table_id==SRC_TABLE)
		return(src_table);
	else if(table_id==DST_TABLE)
		return(dst_table);
	else
		return(nullptr);
}

bool BaseRelationship::isTableMandatory(unsigned table_id)
{
	if(table_id==SRC_TABLE)
		return(src_mandatory);
	else
		return(dst_mandatory);
}

void BaseRelationship::setConnected(bool value)
{
	connected=value;

	src_table->setModified(true);

	if(dst_table!=src_table)
	 dst_table->setModified(true);

	dynamic_cast<Schema *>(src_table->getSchema())->setModified(true);

	if(dst_table->getSchema()!=src_table->getSchema())
		dynamic_cast<Schema *>(dst_table->getSchema())->setModified(true);

	this->setModified(true);
}

void BaseRelationship::disconnectRelationship(void)
{
	if(connected)
		setConnected(false);
}

void BaseRelationship::connectRelationship(void)
{
	if(!connected)
		setConnected(true);
}

Textbox *BaseRelationship::getLabel(unsigned label_id)
{
	if(label_id<=REL_NAME_LABEL)
		return(lables[label_id]);
	else
		//Raises an error when the label id is invalid
		throw Exception(ERR_REF_LABEL_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

unsigned BaseRelationship::getRelationshipType(void)
{
	return(rel_type);
}

bool BaseRelationship::isRelationshipConnected(void)
{
	return(connected);
}

bool BaseRelationship::isSelfRelationship(void)
{
	return(dst_table==src_table);
}

void BaseRelationship::setRelationshipAttributes(void)
{
	unsigned count, i;
	QString str_aux,
			label_attribs[3]={ ParsersAttributes::SRC_LABEL,
												 ParsersAttributes::DST_LABEL,
												 ParsersAttributes::NAME_LABEL};

	switch(rel_type)
	{
		case RELATIONSHIP_11: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_11; break;
		case RELATIONSHIP_1N: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_1N; break;
		case RELATIONSHIP_NN: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_NN; break;
		case RELATIONSHIP_GEN: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_GEN; break;
		case RELATIONSHIP_FK: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_FK; break;
		default:
			if(src_table->getObjectType()==OBJ_VIEW)
				attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATION_TAB_VIEW;
			else
				attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_DEP;
		break;
	}

	attributes[ParsersAttributes::SRC_REQUIRED]=(src_mandatory ? "1" : "");
	attributes[ParsersAttributes::DST_REQUIRED]=(dst_mandatory ? "1" : "");

	if(src_table)
		attributes[ParsersAttributes::SRC_TABLE]=src_table->getName(true);

	if(dst_table)
		attributes[ParsersAttributes::DST_TABLE]=dst_table->getName(true);


	count=points.size();
	for(i=0; i < count; i++)
	{
		attributes[ParsersAttributes::X_POS]=QString("%1").arg(points[i].x());
		attributes[ParsersAttributes::Y_POS]=QString("%1").arg(points[i].y());
		str_aux+=SchemaParser::getCodeDefinition(ParsersAttributes::POSITION, attributes, SchemaParser::XML_DEFINITION);
	}
	attributes[ParsersAttributes::POINTS]=str_aux;

	str_aux="";
	for(i=0; i < 3; i++)
	{
		if(!std::isnan(lables_dist[i].x()))
		{
			attributes[ParsersAttributes::X_POS]=QString("%1").arg(lables_dist[i].x());
			attributes[ParsersAttributes::Y_POS]=QString("%1").arg(lables_dist[i].y());
			attributes[ParsersAttributes::POSITION]=SchemaParser::getCodeDefinition(ParsersAttributes::POSITION, attributes, SchemaParser::XML_DEFINITION);
			attributes[ParsersAttributes::REF_TYPE]=label_attribs[i];
			str_aux+=SchemaParser::getCodeDefinition(ParsersAttributes::LABEL, attributes, SchemaParser::XML_DEFINITION);
		}
	}
	attributes[ParsersAttributes::LABELS_POS]=str_aux;
}

QString BaseRelationship::getCodeDefinition(unsigned def_type)
{
	if(def_type==SchemaParser::SQL_DEFINITION)
		return("");
	else
	{
		bool reduced_form;
		setRelationshipAttributes();
		reduced_form=(attributes[ParsersAttributes::POINTS].isEmpty() &&
									attributes[ParsersAttributes::LABELS_POS].isEmpty());
		return(BaseObject::getCodeDefinition(SchemaParser::XML_DEFINITION,reduced_form));
	}
}

void BaseRelationship::setPoints(const vector<QPointF> &points)
{
	this->points=points;
}

void BaseRelationship::setLabelDistance(unsigned label_id, QPointF label_dist)
{
	if(label_id > REL_NAME_LABEL)
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->lables_dist[label_id]=label_dist;
}

QPointF BaseRelationship::getLabelDistance(unsigned label_id)
{
	if(label_id > REL_NAME_LABEL)
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(this->lables_dist[label_id]);
}

vector<QPointF> BaseRelationship::getPoints(void)
{
	return(points);
}

void BaseRelationship::operator = (BaseRelationship &rel)
{
	(*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(rel);
	this->connected=false;
	this->src_table=rel.src_table;
	this->dst_table=rel.dst_table;
	this->rel_type=rel.rel_type;
	this->points=rel.points;

	for(int i=0; i < 3; i++)
	{
		if(rel.lables[i])
		{
			if(!this->lables[i])
				this->lables[i]=new Textbox;

			(*this->lables[i])=(*rel.lables[i]);
		}
		this->lables_dist[i]=rel.lables_dist[i];
	}

	this->setMandatoryTable(SRC_TABLE, false);
	this->setMandatoryTable(DST_TABLE, false);

	this->setMandatoryTable(SRC_TABLE, rel.src_mandatory);
	this->setMandatoryTable(DST_TABLE, rel.dst_mandatory);
}

