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

#include "tableobjectview.h"

const QString TableObjectView::TYPE_SEPARATOR(" ");
const QString TableObjectView::CONSTR_SEPARATOR(" ");
const QString TableObjectView::TXT_UNIQUE("uq");
const QString TableObjectView::TXT_EXCLUDE("ex");
const QString TableObjectView::TXT_CHECK("ck");
const QString TableObjectView::TXT_PRIMARY_KEY("pk");
const QString TableObjectView::TXT_FOREIGN_KEY("fk");
const QString TableObjectView::TXT_NOT_nullptr("nn");
const QString TableObjectView::CONSTR_DELIM_START("«");
const QString TableObjectView::CONSTR_DELIM_END("»");

TableObjectView::TableObjectView(TableObject *object) : BaseObjectView(object)
{
	descriptor=nullptr;

	for(unsigned i=0; i < 3; i++)
	{
		lables[i]=new QGraphicsSimpleTextItem;
		this->addToGroup(lables[i]);
	}
}

TableObjectView::~TableObjectView(void)
{
	this->removeFromGroup(descriptor);
	delete(descriptor);

	for(unsigned i=0; i < 3; i++)
	{
		this->removeFromGroup(lables[i]);
		delete(lables[i]);
	}
}

void TableObjectView::configureDescriptor(ConstraintType constr_type)
{
	ObjectType obj_type=BASE_OBJECT;
	Column *column=dynamic_cast<Column *>(this->getSourceObject());
	float factor=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;
	bool ellipse_desc=false;

	//Based upon the source object type the descriptor is allocated
	if(this->getSourceObject())
		obj_type=this->getSourceObject()->getObjectType();

	/* Elliptical descriptor is used to columns (with or without not-null constraint),
		for other object types, polygonal descriptor is usded */
	ellipse_desc=((column && constr_type==BaseType::null) ||
								(obj_type!=OBJ_INDEX && obj_type!=OBJ_RULE &&
																									obj_type!=OBJ_TRIGGER && obj_type!=OBJ_COLUMN));

	if(descriptor && ((ellipse_desc && !dynamic_cast<QGraphicsEllipseItem *>(descriptor)) ||
										(!ellipse_desc && dynamic_cast<QGraphicsEllipseItem *>(descriptor))))
	{
		this->removeFromGroup(descriptor);
		delete(descriptor);
		descriptor=nullptr;
	}

	if(!descriptor)
	{
		if(ellipse_desc)
			descriptor=new QGraphicsEllipseItem;
		else
			descriptor=new QGraphicsPolygonItem;

		this->addToGroup(descriptor);
	}

	if(column)
	{
		QString attrib;
		QPolygonF pol;

		if(constr_type==BaseType::null)
		{
			QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descriptor);

			desc->setRect(QRectF(QPointF(0,0),
													 QSizeF(9.0f * factor, 9.0f * factor)));

			if(column->isNotNull())
				attrib=ParsersAttributes::NN_COLUMN;
			else
				attrib=ParsersAttributes::COLUMN;

			desc->setBrush(this->getFillStyle(attrib));
			desc->setPen(this->getBorderStyle(attrib));
		}
		else
		{
			QGraphicsPolygonItem *desc=dynamic_cast<QGraphicsPolygonItem *>(descriptor);

			if(constr_type==ConstraintType::primary_key)
			{
				attrib=ParsersAttributes::PK_COLUMN;
				pol.append(QPointF(2,0)); pol.append(QPointF(0,2)); pol.append(QPointF(0,7));
				pol.append(QPointF(2,9)); pol.append(QPointF(3,8)); pol.append(QPointF(3,6));
				pol.append(QPointF(4,6)); pol.append(QPointF(5,7)); pol.append(QPointF(6,6));
				pol.append(QPointF(7,5)); pol.append(QPointF(9,7)); pol.append(QPointF(9,3));
				pol.append(QPointF(3,3)); pol.append(QPointF(3,1));
			}
			else if(constr_type==ConstraintType::foreign_key)
			{
				attrib=ParsersAttributes::FK_COLUMN;
				pol.append(QPointF(0,3)); pol.append(QPointF(0,6)); pol.append(QPointF(4,6));
				pol.append(QPointF(4,9)); pol.append(QPointF(5,9)); pol.append(QPointF(9,5));
				pol.append(QPointF(9,4)); pol.append(QPointF(5,0)); pol.append(QPointF(4,0));
				pol.append(QPointF(4,3));
			}
			else if(constr_type==ConstraintType::unique)
			{
				attrib=ParsersAttributes::UQ_COLUMN;
				pol.append(QPointF(4,0)); pol.append(QPointF(0,4)); pol.append(QPointF(0,5));
				pol.append(QPointF(4,9)); pol.append(QPointF(5,9)); pol.append(QPointF(9,5));
				pol.append(QPointF(9,4)); pol.append(QPointF(5,0));
			}

			if(factor!=1.0f)
				this->resizePolygon(pol,
														pol.boundingRect().width() * factor,
														pol.boundingRect().height()  * factor);

			desc->setPolygon(pol);
			desc->setBrush(this->getFillStyle(attrib));
			desc->setPen(this->getBorderStyle(attrib));
		}
	}
	else if(obj_type==OBJ_INDEX ||
					obj_type==OBJ_RULE ||
					obj_type==OBJ_TRIGGER)
	{
		TableObject *tab_obj=dynamic_cast<TableObject *>(this->getSourceObject());
		QGraphicsPolygonItem *desc=dynamic_cast<QGraphicsPolygonItem *>(descriptor);
		QPolygonF pol;

		pol.append(QPointF(5,0)); pol.append(QPointF(0,5)); pol.append(QPointF(4,9));
		pol.append(QPointF(9,9)); pol.append(QPointF(9,4));

		if(factor!=1.0f)
			this->resizePolygon(pol,
													pol.boundingRect().width() * factor ,
													pol.boundingRect().height() * factor);

		desc->setPolygon(pol);
		desc->setBrush(this->getFillStyle(tab_obj->getSchemaName()));
		desc->setPen(this->getBorderStyle(tab_obj->getSchemaName()));
	}
	else
	{
		QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descriptor);

		desc->setRect(QRectF(QPointF(0,0),
												 QSizeF(9.0f * factor, 9.0f * factor)));
		desc->setBrush(this->getFillStyle(ParsersAttributes::REFERENCE));
		desc->setPen(this->getBorderStyle(ParsersAttributes::REFERENCE));
	}
}

void TableObjectView::configureObject(void)
{
	if(this->getSourceObject())
	{
		QTextCharFormat fmt;
		float px;
		QString str_constr, tooltip, atribs_tip;
		TableObject *tab_obj=dynamic_cast<TableObject *>(this->getSourceObject());
		Column *column=dynamic_cast<Column *>(tab_obj);
		ConstraintType constr_type=ConstraintType::null;

		tooltip=Utf8String::create(tab_obj->getName()) + " (" + tab_obj->getTypeName() + ")";

		if(column)
		{
			str_constr=this->getConstraintString(column);

			if(str_constr.indexOf(TXT_PRIMARY_KEY)>=0)
			{
				fmt=font_config[ParsersAttributes::PK_COLUMN];
				constr_type=ConstraintType::primary_key;
			}
			else if(str_constr.indexOf(TXT_FOREIGN_KEY)>=0)
			{
				fmt=font_config[ParsersAttributes::FK_COLUMN];
				constr_type=ConstraintType::foreign_key;
			}
			else if(str_constr.indexOf(TXT_UNIQUE)>=0)
			{
				fmt=font_config[ParsersAttributes::UQ_COLUMN];
				constr_type=ConstraintType::unique;
			}
			else if(str_constr.indexOf(TXT_NOT_nullptr)>=0)
				fmt=font_config[ParsersAttributes::NN_COLUMN];
			else
				fmt=font_config[ParsersAttributes::COLUMN];

			if(column->isAddedByRelationship())
				fmt=font_config[ParsersAttributes::INH_COLUMN];
			else if(column->isProtected())
				fmt=font_config[ParsersAttributes::PROT_COLUMN];

			if(str_constr.indexOf(TXT_PRIMARY_KEY)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::primary_key)).toLower() + ", ";

			if(str_constr.indexOf(TXT_FOREIGN_KEY)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::foreign_key)).toLower() + ", ";

			if(str_constr.indexOf(TXT_UNIQUE)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::unique)).toLower() + ", ";

			if(str_constr.indexOf(TXT_EXCLUDE)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::exclude)).toLower() + ", ";

			if(str_constr.indexOf(TXT_NOT_nullptr)>=0)
				atribs_tip+="not null";
		}
		else
		{
			if(!tab_obj->isProtected())
				fmt=font_config[tab_obj->getSchemaName()];
			else
				fmt=font_config[ParsersAttributes::PROT_COLUMN];
		}

		configureDescriptor(constr_type);

		//Set the descriptor position as the first item on the view
		descriptor->setPos(HORIZ_SPACING, 1);
		px=descriptor->pos().x() + descriptor->boundingRect().width() + (2 * HORIZ_SPACING);

		//Configuring the labels as follow: [object name] [type] [constraints]
		//Configuring tha name label
		lables[0]->setText(Utf8String::create(tab_obj->getName()));
		lables[0]->setFont(fmt.font());
		lables[0]->setBrush(fmt.foreground());
		lables[0]->setPos(px, 0);
		px+=lables[0]->boundingRect().width();

		//Configuring the type label
		fmt=font_config[ParsersAttributes::OBJECT_TYPE];
		if(column)
			lables[1]->setText(Utf8String::create(TYPE_SEPARATOR + (*column->getType())));
		else
			lables[1]->setText(Utf8String::create(TYPE_SEPARATOR + tab_obj->getSchemaName()));

		lables[1]->setFont(fmt.font());
		lables[1]->setBrush(fmt.foreground());
		lables[1]->setPos(px, 0);
		px+=lables[1]->boundingRect().width() + (3 * HORIZ_SPACING);

		//Configuring the constraints label
		fmt=font_config[ParsersAttributes::CONSTRAINTS];
		if(column)
			lables[2]->setText(Utf8String::create(str_constr));
		else
		{
			Rule *rule=dynamic_cast<Rule *>(tab_obj);
			Trigger *trigger=dynamic_cast<Trigger *>(tab_obj);
			Index *index=dynamic_cast<Index *>(tab_obj);

			if(rule)
			{
				str_constr+=(~rule->getExecutionType()).mid(0,1);
				atribs_tip+=(~rule->getExecutionType()).toLower() + ", ";

				str_constr+=CONSTR_SEPARATOR;

				str_constr+=(~rule->getEventType()).mid(3,1);
				atribs_tip+=(~rule->getEventType()).toLower();
				str_constr=str_constr.toLower();
			}
			else if(trigger)
			{
				str_constr+=(~trigger->getFiringType()).mid(0,1);
				str_constr+=CONSTR_SEPARATOR;

				atribs_tip+=(~trigger->getFiringType()).toLower() + ", ";

				for(unsigned i=EventType::on_insert; i <= EventType::on_truncate; i++)
				{
					if(trigger->isExecuteOnEvent(EventType(i)))
					{
						str_constr+=(~EventType(i)).mid(3,1);
						atribs_tip+=(~EventType(i)).toLower() + ", ";
					}
				}
				str_constr=str_constr.toLower();
			}
			else if(index)
			{
				if(index->getIndexAttribute(Index::UNIQUE))
				{
					str_constr+="u";
					atribs_tip += QString("unique") + ", ";
				}

				if(index->getIndexAttribute(Index::CONCURRENT))
				{
					str_constr+="c";
					atribs_tip += QString("concurrent") + ", ";
				}

				if(index->getIndexAttribute(Index::FAST_UPDATE))
				{
					str_constr+="f";
					atribs_tip += "fast updated";
				}
			}

			if(!str_constr.isEmpty())
				lables[2]->setText(Utf8String::create(CONSTR_DELIM_START + " " +
																						 str_constr + " " +
																						 CONSTR_DELIM_END));
		}

		if(!atribs_tip.isEmpty())
		{
			if(atribs_tip.at(atribs_tip.length()-1)==' ')
				atribs_tip.remove(atribs_tip.length()-2, 2);

			atribs_tip=Utf8String::create("\n" + CONSTR_DELIM_START + " " + atribs_tip + " " + CONSTR_DELIM_END);
		}

		lables[2]->setFont(fmt.font());
		lables[2]->setBrush(fmt.foreground());
		lables[2]->setPos(px, 0);

		//Calculating the object bounding rect that is composed by the join of the all object's child dimensions
		descriptor->setPos(HORIZ_SPACING, lables[0]->boundingRect().center().y() - descriptor->boundingRect().center().y());
		bounding_rect.setTopLeft(QPointF(descriptor->boundingRect().left(), lables[0]->boundingRect().top()));

		//Special case: when the constraint label has no text use the type label dimension
		if(lables[2]->boundingRect().width()==0)
			bounding_rect.setBottomRight(QPointF(lables[1]->boundingRect().right(), lables[0]->boundingRect().bottom()));
		else
			bounding_rect.setBottomRight(QPointF(lables[2]->boundingRect().right(), lables[0]->boundingRect().bottom()));

		this->setToolTip(tooltip + atribs_tip);
	}
}

void TableObjectView::configureObject(Reference reference)
{
	QTextCharFormat fmt;
	float px;
	QString str_aux;

	configureDescriptor();
	descriptor->setPos(HORIZ_SPACING, 1);
	px=descriptor->pos().x() + descriptor->boundingRect().width() + (2 * HORIZ_SPACING);

	if(reference.getReferenceType()==Reference::REFER_COLUMN)
	{
		//Configures the name label as: [table].[column]
		fmt=font_config[ParsersAttributes::REF_TABLE];
		lables[0]->setText(Utf8String::create(reference.getTable()->getName() + "."));
		lables[0]->setFont(fmt.font());
		lables[0]->setBrush(fmt.foreground());
		lables[0]->setPos(px, 0);
		px+=lables[0]->boundingRect().width();

		fmt=font_config[ParsersAttributes::REF_COLUMN];
		if(reference.getColumn())
			lables[1]->setText(Utf8String::create(reference.getColumn()->getName()));
		else
			lables[1]->setText("*");

		lables[1]->setFont(fmt.font());
		lables[1]->setBrush(fmt.foreground());
		lables[1]->setPos(px, 0);
		px+=lables[1]->boundingRect().width();
	}
	else
	{
		fmt=font_config[ParsersAttributes::REF_TABLE];

		str_aux=reference.getExpression().simplified().mid(0,25);
		if(reference.getExpression().size() > 25) str_aux+="...";
		str_aux.replace("\n", " ");

		lables[0]->setText(str_aux);
		lables[0]->setFont(fmt.font());
		lables[0]->setBrush(fmt.foreground());
		lables[1]->setText("");
		lables[0]->setPos(px, 0);
		px+=lables[0]->boundingRect().width();
	}

	//Configures a label for the alias (if there is one)
	if((reference.getColumn() && reference.getColumnAlias()!="") ||
		 (reference.getAlias()!="" && reference.getReferenceType()==Reference::REFER_EXPRESSION))
	{
		if(reference.getReferenceType()==Reference::REFER_EXPRESSION)
			str_aux=reference.getAlias();
		else
			str_aux=reference.getColumnAlias();

		str_aux=" (" + str_aux + ") ";
		fmt=font_config[ParsersAttributes::ALIAS];
		lables[2]->setText(Utf8String::create(str_aux));
		lables[2]->setFont(fmt.font());
		lables[2]->setBrush(fmt.foreground());
		lables[2]->setPos(px, 0);
	}

	descriptor->setPos(HORIZ_SPACING, lables[0]->boundingRect().center().y() - descriptor->boundingRect().center().y());
	bounding_rect.setTopLeft(QPointF(descriptor->pos().x(), lables[0]->pos().y()));

	if(lables[2]->text().isEmpty())
		bounding_rect.setBottomRight(QPointF(lables[1]->boundingRect().right(), lables[0]->boundingRect().bottom()));
	else
		bounding_rect.setBottomRight(QPointF(lables[2]->boundingRect().right(), lables[0]->boundingRect().bottom()));
}

void TableObjectView::setChildObjectXPos(unsigned obj_idx, float px)
{
	if(obj_idx >= 4)
		throw Exception(ERR_REF_OBJ_INV_INDEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	if(obj_idx==0)
		descriptor->setPos(px, descriptor->pos().y());
	else
		lables[obj_idx-1]->setPos(px, lables[obj_idx-1]->pos().y());
}

QGraphicsItem *TableObjectView::getChildObject(unsigned obj_idx)
{
	if(obj_idx >= 4)
		throw Exception(ERR_REF_OBJ_INV_INDEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	if(obj_idx==0)
		return(descriptor);
	else
		return(lables[obj_idx-1]);
}

QString TableObjectView::getConstraintString(Column *column)
{
	if(column && column->getParentTable())
	{
		Table *table=dynamic_cast<Table *>(column->getParentTable());
		QString str_constr;
		Constraint *constr=nullptr;
		vector<Constraint *>::iterator itr,itr_end;
		vector<Constraint *> constraints;
		ConstraintType constr_type;
		unsigned i, count;

		count=table->getConstraintCount();
		for(i=0; i < count; i++)
			constraints.push_back(table->getConstraint(i));

		itr=constraints.begin();
		itr_end=constraints.end();

		while(itr!=itr_end)
		{
			constr=(*itr);
			itr++;

			//Check if the column is referecend by the constraint
			if(constr->isColumnReferenced(column, constr_type==ConstraintType::foreign_key))
			{
				constr_type=constr->getConstraintType();

				if(constr_type==ConstraintType::primary_key)
					str_constr=TXT_PRIMARY_KEY + CONSTR_SEPARATOR + str_constr;
				if(constr_type==ConstraintType::foreign_key && str_constr.indexOf(TXT_FOREIGN_KEY) < 0)
					str_constr+=TXT_FOREIGN_KEY + CONSTR_SEPARATOR;
				if(constr_type==ConstraintType::unique && str_constr.indexOf(TXT_UNIQUE) < 0)
					str_constr+=TXT_UNIQUE + CONSTR_SEPARATOR;
				if(constr_type==ConstraintType::exclude && str_constr.indexOf(TXT_EXCLUDE) < 0)
					str_constr+=TXT_EXCLUDE + CONSTR_SEPARATOR;
			}
		}

		if(column->isNotNull()) str_constr+=TXT_NOT_nullptr + CONSTR_SEPARATOR;

		if(str_constr!="")
			str_constr= CONSTR_DELIM_START +
									CONSTR_SEPARATOR + str_constr +
									CONSTR_DELIM_END;

		return(str_constr);
	}
	else return("");
}

