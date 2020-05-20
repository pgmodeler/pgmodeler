/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	for(unsigned i=0; i < 3; i++)
		lables[i]=nullptr;

	src_table=dst_table=nullptr;

	(*(this))=(*rel);

	custom_color=QColor(Qt::transparent);
	reference_fk = nullptr;
}

BaseRelationship::BaseRelationship(unsigned rel_type, BaseTable *src_tab, BaseTable *dst_tab, bool src_mandatory, bool dst_mandatory)

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
		this->custom_color=QColor(Qt::transparent);
		this->reference_fk=nullptr;

		for(unsigned i=0; i < 3; i++)
		{
			lables[i]=nullptr;
			lables_dist[i]=QPointF(DNaN, DNaN);
		}

		configureRelationship();

		str_aux=QApplication::translate("BaseRelationship","rel_%1_%2","")
				.arg(src_tab->getName()).arg(dst_tab->getName());

		if(str_aux.size() > BaseObject::ObjectNameMaxLength)
			str_aux.resize(BaseObject::ObjectNameMaxLength);

		setName(str_aux);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void BaseRelationship::configureRelationship()
{
	obj_type=ObjectType::BaseRelationship;

	attributes[Attributes::Type]=QString();
	attributes[Attributes::SrcRequired]=QString();
	attributes[Attributes::DstRequired]=QString();
	attributes[Attributes::SrcTable]=QString();
	attributes[Attributes::DstTable]=QString();
	attributes[Attributes::Points]=QString();
	attributes[Attributes::Columns]=QString();
	attributes[Attributes::Constraints]=QString();
	attributes[Attributes::Elements]=QString();
	attributes[Attributes::Identifier]=QString();
	attributes[Attributes::ReducedForm]=QString();
	attributes[Attributes::Deferrable]=QString();
	attributes[Attributes::DeferType]=QString();
	attributes[Attributes::TableName]=QString();
	attributes[Attributes::SpecialPkCols]=QString();
	attributes[Attributes::RelationshipNn]=QString();
	attributes[Attributes::RelationshipGen]=QString();
	attributes[Attributes::RelationshipDep]=QString();
	attributes[Attributes::RelationshipPart]=QString();
	attributes[Attributes::Relationship1n]=QString();
	attributes[Attributes::Relationship11]=QString();
	attributes[Attributes::Constraints]=QString();
	attributes[Attributes::Table]=QString();
	attributes[Attributes::AncestorTable]=QString();
	attributes[Attributes::CopyOptions]=QString();
	attributes[Attributes::CopyMode]=QString();
	attributes[Attributes::SrcColPattern]=QString();
	attributes[Attributes::DstColPattern]=QString();
	attributes[Attributes::PkPattern]=QString();
	attributes[Attributes::UqPattern]=QString();
	attributes[Attributes::SrcFkPattern]=QString();
	attributes[Attributes::DstFkPattern]=QString();
	attributes[Attributes::PkColPattern]=QString();
	attributes[Attributes::SinglePkColumn]=QString();
	attributes[Attributes::UpdAction]=QString();
	attributes[Attributes::DelAction]=QString();
	attributes[Attributes::CustomColor]=QString();
	attributes[Attributes::ReferenceFk]=QString();
	attributes[Attributes::PartitionBoundExpr]=QString();
	attributes[Attributes::OriginalPk]=QString();

	//Check if the relationship type is valid
	if(rel_type <= RelationshipFk)
	{
		//Raises an error if one of the tables is not allocated
		if(!src_table || !dst_table)
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgNotAllocatedTable)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(ObjectType::BaseRelationship)),
							ErrorCode::AsgNotAllocatedTable,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Raises an error if the relationship type is generalization or dependency
			and the source and destination table are the same. */
		if((rel_type==RelationshipGen || rel_type==RelationshipDep || rel_type==RelationshipPart) && src_table==dst_table)
			throw Exception(ErrorCode::InvInheritCopyPartRelationship,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Allocates the textbox for the name label
		lables[RelNameLabel]=new Textbox;
		lables[RelNameLabel]->setTextAttribute(Textbox::ItalicText, true);

		//Allocates the cardinality labels only when the relationship is not generalization or dependency (copy)
		if(rel_type!=RelationshipGen &&
			 rel_type!=RelationshipDep &&
			 rel_type!=RelationshipPart)
		{
			lables[SrcCardLabel]=new Textbox;
			lables[DstCardLabel]=new Textbox;
			lables[SrcCardLabel]->setTextAttribute(Textbox::ItalicText, true);
			lables[DstCardLabel]->setTextAttribute(Textbox::ItalicText, true);

			//Configures the mandatory participation for both tables
			setMandatoryTable(SrcTable,src_mandatory);
			setMandatoryTable(DstTable,dst_mandatory);
		}
	}
	else
		//Raises an error if the specified relationship typ is invalid
		throw Exception(ErrorCode::AllocationObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

BaseRelationship::~BaseRelationship()
{
	disconnectRelationship();

	//Unallocates the labels
	for(unsigned i=0; i<3; i++)
		if(lables[i]) delete lables[i];
}

void BaseRelationship::setName(const QString &name)
{
	try
	{
		BaseObject::setName(name);

		if(lables[RelNameLabel])
			lables[RelNameLabel]->setComment(name);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
	if(rel_type==Relationship11 &&
			((table_id==SrcTable && value && dst_mandatory) ||
			 (table_id==DstTable && value && src_mandatory)))
		throw Exception(ErrorCode::NotImplementedRelationshipType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Case the source table is mandatory
	if(table_id==SrcTable)
	{
		src_mandatory=value;
		//Indicates that the source cardinality label will be configured
		label_id=SrcCardLabel;
	}
	else
	{
		if(rel_type!=Relationship1n)
			dst_mandatory=value;
		else
			/* For One to many (1-n) relationship the entity on the "many" side
			will be always in the format (0,n) */
			dst_mandatory=false;

		//Indicates that the destination cardinality label will be configured
		label_id=DstCardLabel;
	}

	if(!value) cmin=QString("0");
	else cmin=QString("1");

	if(lables[label_id])
	{
		if(rel_type==Relationship11)
			lables[label_id]->setComment(cmin + QString(":1"));
		else if(rel_type==Relationship1n)
		{
			aux=(table_id==SrcTable ? QString("1") : QString("n"));
			lables[label_id]->setComment(cmin + QString(":") + aux);
		}
		else if(rel_type==RelationshipFk)
		{
			if((table_id==SrcTable && dynamic_cast<Table *>(src_table)->isReferTableOnForeignKey(dynamic_cast<Table *>(dst_table))) ||
				 (!isSelfRelationship() && table_id==DstTable && dynamic_cast<Table *>(dst_table)->isReferTableOnForeignKey(dynamic_cast<Table *>(src_table))))
			{
				if(table_id == SrcTable && canSimulateRelationship11())
					aux = "1";
				else
					aux = "n";
			}
			else
				aux = "1";

			if((table_id == DstTable && dst_mandatory) ||
				 (table_id == SrcTable && src_mandatory))
				aux.prepend("1:");
			else
				aux.prepend("0:");

			lables[label_id]->setComment(aux);
		}
		else if(rel_type==RelationshipNn)
			lables[label_id]->setComment("n");

		lables[label_id]->setModified(true);
	}
}

BaseTable *BaseRelationship::getTable(unsigned table_id)
{
	if(table_id==SrcTable)
		return src_table;
	else if(table_id==DstTable)
		return dst_table;
	else
		return nullptr;
}

bool BaseRelationship::isTableMandatory(unsigned table_id)
{
	if(table_id==SrcTable)
		return src_mandatory;
	else
		return dst_mandatory;
}

void BaseRelationship::setConnected(bool value)
{
	connected=value;

	if(!this->signalsBlocked())
	{
		src_table->setModified(true);

		if(dst_table!=src_table)
			dst_table->setModified(true);

		dynamic_cast<Schema *>(src_table->getSchema())->setModified(true);

		if(dst_table->getSchema()!=src_table->getSchema())
			dynamic_cast<Schema *>(dst_table->getSchema())->setModified(true);

		this->setModified(true);
	}
}

void BaseRelationship::disconnectRelationship()
{
	if(connected)
	{
		setConnected(false);
		setCodeInvalidated(true);
	}
}

void BaseRelationship::connectRelationship()
{
	if(!connected)
	{
		setConnected(true);
		setCodeInvalidated(true);
	}
}

Textbox *BaseRelationship::getLabel(unsigned label_id)
{
	if(label_id<=RelNameLabel)
		return lables[label_id];

	//Raises an error when the label id is invalid
	throw Exception(ErrorCode::RefLabelInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

unsigned BaseRelationship::getRelationshipType()
{
	return rel_type;
}

bool BaseRelationship::isRelationshipConnected()
{
	return connected;
}

bool BaseRelationship::isSelfRelationship()
{
	return (dst_table==src_table);
}

void BaseRelationship::setRelationshipAttributes()
{
	unsigned count, i;
	QString str_aux,
			label_attribs[3]={ Attributes::SrcLabel,
							   Attributes::DstLabel,
							   Attributes::NameLabel};

	attributes[Attributes::Layer]=QString::number(layer);
	attributes[Attributes::Type]=getRelTypeAttribute();
	attributes[Attributes::SrcRequired]=(src_mandatory ? Attributes::True : QString());
	attributes[Attributes::DstRequired]=(dst_mandatory ? Attributes::True : QString());

	if(src_table)
		attributes[Attributes::SrcTable]=src_table->getName(true);

	if(dst_table)
		attributes[Attributes::DstTable]=dst_table->getName(true);


	count=points.size();
	for(i=0; i < count; i++)
	{
		attributes[Attributes::XPos]=QString("%1").arg(points[i].x());
		attributes[Attributes::YPos]=QString("%1").arg(points[i].y());
		str_aux+=schparser.getCodeDefinition(Attributes::Position, attributes, SchemaParser::XmlDefinition);
	}
	attributes[Attributes::Points]=str_aux;

	str_aux=QString();
	for(i=0; i < 3; i++)
	{
		if(!std::isnan(lables_dist[i].x()))
		{
			attributes[Attributes::XPos]=QString("%1").arg(lables_dist[i].x());
			attributes[Attributes::YPos]=QString("%1").arg(lables_dist[i].y());
			attributes[Attributes::Position]=schparser.getCodeDefinition(Attributes::Position, attributes, SchemaParser::XmlDefinition);
			attributes[Attributes::RefType]=label_attribs[i];
			str_aux+=schparser.getCodeDefinition(Attributes::Label, attributes, SchemaParser::XmlDefinition);
		}
	}

	attributes[Attributes::LabelsPos]=str_aux;
	attributes[Attributes::CustomColor]=(custom_color!=Qt::transparent ? custom_color.name() : QString());
	attributes[Attributes::ReferenceFk]=(reference_fk ? reference_fk->getName() : QString());
	setFadedOutAttribute();
}

QString BaseRelationship::getCachedCode(unsigned def_type)
{
	if(!code_invalidated &&
			((!cached_code[def_type].isEmpty()) ||
			 (def_type==SchemaParser::XmlDefinition  && !cached_reduced_code.isEmpty())))
	{
		if(def_type==SchemaParser::XmlDefinition  && !cached_reduced_code.isEmpty())
			return cached_reduced_code;
		else
			return cached_code[def_type];
	}
	else
		return QString();
}

void BaseRelationship::setReferenceForeignKey(Constraint *ref_fk)
{
	//if(ref_fk && rel_type != RELATIONSHIP_FK)
		//Throw error...
	this->reference_fk = ref_fk;
}

Constraint *BaseRelationship::getReferenceForeignKey()
{
	return reference_fk;
}

bool BaseRelationship::canSimulateRelationship11()
{
	if(rel_type != BaseRelationship::RelationshipFk)
		return false;

	bool fake_rel11 = false;
	PhysicalTable *table = dynamic_cast<PhysicalTable *>(getTable(BaseRelationship::SrcTable));

	if(table)
	{
		Constraint *constr = nullptr, *uq_constr = nullptr;

		for(unsigned idx = 0; idx < table->getConstraintCount() && !fake_rel11; idx++)
		{
			constr = table->getConstraint(idx);

			if(constr->getConstraintType() == ConstraintType::ForeignKey)
			{
				for(unsigned idx1 = 0; idx1 < table->getConstraintCount(); idx1++)
				{
					uq_constr = table->getConstraint(idx1);

					if(uq_constr->getConstraintType() == ConstraintType::Unique &&
						 uq_constr->isColumnsExist(constr->getColumns(Constraint::SourceCols), Constraint::SourceCols))
					{
						fake_rel11 = true;
						break;
					}
				}
			}
		}
	}

	return fake_rel11;
}

QString BaseRelationship::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type);
	if(!code_def.isEmpty()) return code_def;

	if(def_type==SchemaParser::SqlDefinition)
	{
		if(rel_type!=RelationshipFk)
			return QString();
		else
		{
			cached_code[def_type] = reference_fk->getCodeDefinition(SchemaParser::SqlDefinition);
			return cached_code[def_type];
		}
	}
	else
	{
		bool reduced_form;
		setRelationshipAttributes();
		reduced_form=(attributes[Attributes::Points].isEmpty() &&
								 attributes[Attributes::LabelsPos].isEmpty());

		if(!reduced_form)
			cached_reduced_code.clear();

		return BaseObject::getCodeDefinition(SchemaParser::XmlDefinition,reduced_form);
	}
}

void BaseRelationship::setPoints(const vector<QPointF> &points)
{
	this->setCodeInvalidated(true);
	this->points=points;
}

void BaseRelationship::setLabelDistance(unsigned label_id, QPointF label_dist)
{
	if(label_id > RelNameLabel)
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->lables_dist[label_id]=label_dist;
	this->setCodeInvalidated(true);
}

QPointF BaseRelationship::getLabelDistance(unsigned label_id)
{
	if(label_id > RelNameLabel)
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return this->lables_dist[label_id];
}

void BaseRelationship::setCustomColor(const QColor &color)
{
	custom_color=color;
}

QColor BaseRelationship::getCustomColor()
{
	return custom_color;
}

void BaseRelationship::resetLabelsDistance()
{
	for(unsigned i=0; i < 3; i++)
		this->setLabelDistance(i, QPointF(DNaN,DNaN));
}

vector<QPointF> BaseRelationship::getPoints()
{
	return points;
}

void BaseRelationship::operator = (BaseRelationship &rel)
{
	(*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(rel);
	this->connected=false;
	this->src_table=rel.src_table;
	this->dst_table=rel.dst_table;
	this->rel_type=rel.rel_type;
	this->points=rel.points;
	this->custom_color=rel.custom_color;

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

	this->setMandatoryTable(SrcTable, false);
	this->setMandatoryTable(DstTable, false);

	this->setMandatoryTable(SrcTable, rel.src_mandatory);
	this->setMandatoryTable(DstTable, rel.dst_mandatory);
}

QString BaseRelationship::getRelTypeAttribute()
{
	switch(rel_type)
	{
		case Relationship11: return Attributes::Relationship11;
		case Relationship1n: return Attributes::Relationship1n;
		case RelationshipNn: return Attributes::RelationshipNn;
		case RelationshipGen: return Attributes::RelationshipGen;
		case RelationshipPart: return Attributes::RelationshipPart;
		case RelationshipFk: return Attributes::RelationshipFk;
		default:
		{
			if(src_table->getObjectType()==ObjectType::View)
				return Attributes::RelationshipTabView;
			else
				return Attributes::RelationshipDep;
		}
	}
}

QString BaseRelationship::getRelationshipTypeName(unsigned rel_type, bool is_view)
{
  switch(rel_type)
  {
		case Relationship11: return tr("One-to-one");
		case Relationship1n: return tr("One-to-many");
		case RelationshipNn: return tr("Many-to-many");
		case RelationshipGen: return tr("Inheritance");
		case RelationshipPart: return tr("Partitioning");
		case RelationshipFk: return tr("FK relationship");
	  default:
		{
			if(is_view)
				return tr("Dependency");
			else
				return tr("Copy");
		}
  }
}

QString BaseRelationship::getRelationshipTypeName()
{
	return getRelationshipTypeName(rel_type, src_table->getObjectType()==ObjectType::View);
}

void BaseRelationship::setCodeInvalidated(bool value)
{
	BaseObject::setCodeInvalidated(value);

	if(src_table)
		src_table->setCodeInvalidated(value);

	if(dst_table)
		dst_table->setCodeInvalidated(value);
}

