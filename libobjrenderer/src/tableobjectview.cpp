/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

const QString TableObjectView::TypeSeparator=QString(" ");
const QString TableObjectView::ConstrSeparator=QString(" ");
const QString TableObjectView::TextUnique=QString("uq");
const QString TableObjectView::TextExclude=QString("ex");
const QString TableObjectView::TextCheck=QString("ck");
const QString TableObjectView::TextPrimaryKey=QString("pk");
const QString TableObjectView::TextForeignKey=QString("fk");
const QString TableObjectView::TextNotNull=QString("nn");
const QString TableObjectView::ConstrDelimStart=QString("«");
const QString TableObjectView::ConstrDelimEnd=QString("»");

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
	ObjectType obj_type=ObjBaseObject;
	Column *column=dynamic_cast<Column *>(this->getSourceObject());
	bool ellipse_desc=false;
	double factor=(font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DefaultFontSize) * BaseObjectView::getScreenDpiFactor();
	QPen pen;

	//Based upon the source object type the descriptor is allocated
	if(this->getSourceObject())
		obj_type=this->getSourceObject()->getObjectType();

	/* Elliptical descriptor is used to columns (with or without not-null constraint),
		for other object types, polygonal descriptor is usded */
	ellipse_desc=((column && constr_type==BaseType::null) || (!TableObject::isTableObject(obj_type)));

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

			desc->setRect(QRectF(QPointF(0,0), QSizeF(9.0f * factor, 9.0f * factor)));

			if(column->isNotNull())
				attrib=ParsersAttributes::NN_COLUMN;
			else
				attrib=ParsersAttributes::COLUMN;

			desc->setBrush(this->getFillStyle(attrib));

			pen = this->getBorderStyle(attrib);
			pen.setWidthF(ObjectBorderWidth * 1.15f);
			desc->setPen(pen);
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

			pen = this->getBorderStyle(attrib);
			pen.setWidthF(ObjectBorderWidth * 1.15f);
			desc->setPen(pen);
		}
	}
	else if(obj_type != ObjBaseObject)
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

		pen = this->getBorderStyle(tab_obj->getSchemaName());
		pen.setWidthF(ObjectBorderWidth * 1.15f);
		desc->setPen(pen);
	}
	else
	{
		QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descriptor);

		desc->setRect(QRectF(QPointF(0,0), QSizeF(9.0f * factor, 9.0f * factor)));
		desc->setBrush(this->getFillStyle(ParsersAttributes::REFERENCE));

		pen = this->getBorderStyle(ParsersAttributes::REFERENCE);
		pen.setWidthF(ObjectBorderWidth * 1.15f);
		desc->setPen(pen);
	}
}

void TableObjectView::configureObject(void)
{
	if(this->getSourceObject())
	{
		QTextCharFormat fmt;
		double px;
		QString str_constr, tooltip, atribs_tip;
		TableObject *tab_obj=dynamic_cast<TableObject *>(this->getSourceObject());
		Column *column=dynamic_cast<Column *>(tab_obj);
		ConstraintType constr_type=ConstraintType::null;
		bool sql_disabled=false;

		tooltip=tab_obj->getName() + QString(" (") + tab_obj->getTypeName() + QString(")");
		tooltip+=QString("\nId: %1").arg(tab_obj->getObjectId());
		sql_disabled=tab_obj->isSQLDisabled();

		if(column)
		{
			if(column->isAddedByRelationship())
				tooltip+=trUtf8("\nRelationship: %1").arg(column->getParentRelationship()->getName());

			str_constr=this->getConstraintString(column);

			if(str_constr.indexOf(TextPrimaryKey)>=0)
			{
				fmt=font_config[ParsersAttributes::PK_COLUMN];
				constr_type=ConstraintType::primary_key;
			}
			else if(str_constr.indexOf(TextForeignKey)>=0)
			{
				fmt=font_config[ParsersAttributes::FK_COLUMN];
				constr_type=ConstraintType::foreign_key;
			}
			else if(str_constr.indexOf(TextUnique)>=0)
			{
				fmt=font_config[ParsersAttributes::UQ_COLUMN];
				constr_type=ConstraintType::unique;
			}
			else if(str_constr.indexOf(TextNotNull)>=0)
				fmt=font_config[ParsersAttributes::NN_COLUMN];
			else
				fmt=font_config[ParsersAttributes::COLUMN];

			if(column->isAddedByRelationship())
				fmt=font_config[ParsersAttributes::INH_COLUMN];
			else if(column->isProtected())
				fmt=font_config[ParsersAttributes::PROT_COLUMN];

			if(str_constr.indexOf(TextPrimaryKey)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::primary_key)).toLower() + QString(", ");

			if(str_constr.indexOf(TextForeignKey)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::foreign_key)).toLower() + QString(", ");

			if(str_constr.indexOf(TextUnique)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::unique)).toLower() + QString(", ");

			if(str_constr.indexOf(TextExclude)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::exclude)).toLower() + QString(", ");

			if(str_constr.indexOf(TextNotNull)>=0)
				atribs_tip+=QString("not null");
		}
		else
		{
			if(tab_obj->isAddedByRelationship())
				fmt=font_config[ParsersAttributes::INH_COLUMN];
			else if(tab_obj->isProtected())
				fmt=font_config[ParsersAttributes::PROT_COLUMN];
			else
				fmt=font_config[tab_obj->getSchemaName()];
		}

		configureDescriptor(constr_type);

		//Set the descriptor position as the first item on the view
		descriptor->setPos(HorizSpacing, 1);
		px=descriptor->pos().x() + descriptor->boundingRect().width() + (2 * HorizSpacing);

		//Configuring the labels as follow: [object name] [type] [constraints]
		lables[0]->setText(compact_view && !tab_obj->getAlias().isEmpty() ? tab_obj->getAlias() : tab_obj->getName());

		//Strikeout the column name when its SQL is disabled
		QFont font=fmt.font();
		font.setStrikeOut(sql_disabled);
		fmt.setFont(font);

		lables[0]->setFont(fmt.font());
		lables[0]->setBrush(fmt.foreground());
		lables[0]->setPos(px, 0);
		px+=lables[0]->boundingRect().width();

		//Configuring the type label
		fmt=font_config[ParsersAttributes::OBJECT_TYPE];

		if(compact_view)
			lables[1]->setText(" ");
		else
		{
			if(column)
				lables[1]->setText(TypeSeparator + (*column->getType()));
			else
				lables[1]->setText(TypeSeparator + tab_obj->getSchemaName());
		}

		lables[1]->setFont(fmt.font());
		lables[1]->setBrush(fmt.foreground());
		lables[1]->setPos(px, 0);
		px+=lables[1]->boundingRect().width() + (3 * HorizSpacing);

		//Configuring the constraints label
		fmt=font_config[ParsersAttributes::CONSTRAINTS];
		if(compact_view)
			lables[2]->setText(" ");
		else if(column)
			lables[2]->setText(str_constr);
		else
		{
			Rule *rule=dynamic_cast<Rule *>(tab_obj);
			Trigger *trigger=dynamic_cast<Trigger *>(tab_obj);
			Index *index=dynamic_cast<Index *>(tab_obj);
			Constraint *constr=dynamic_cast<Constraint *>(tab_obj);
			Policy *policy = dynamic_cast<Policy *>(tab_obj);

			if(rule)
			{
				str_constr+=(~rule->getExecutionType()).mid(0,1);
				atribs_tip+=(~rule->getExecutionType()).toLower() + QString(", ");

				str_constr+=ConstrSeparator;

				str_constr+=(~rule->getEventType()).mid(3,1);
				atribs_tip+=(~rule->getEventType()).toLower();
				str_constr=str_constr.toLower();
			}
			else if(trigger)
			{
				str_constr+=(~trigger->getFiringType()).mid(0,1);
				str_constr+=ConstrSeparator;

				atribs_tip+=(~trigger->getFiringType()).toLower() + QString(", ");

				for(unsigned i=EventType::on_insert; i <= EventType::on_truncate; i++)
				{
					if(trigger->isExecuteOnEvent(EventType(i)))
					{
						str_constr+=(~EventType(i)).mid(3,1);
						atribs_tip+=(~EventType(i)).toLower() + QString(", ");
					}
				}
				str_constr=str_constr.toLower();
			}
			else if(index)
			{
				if(index->getIndexAttribute(Index::UNIQUE))
				{
					str_constr+=QString("u");
					atribs_tip += QString("unique") + QString(", ");
				}

				if(index->getIndexAttribute(Index::CONCURRENT))
				{
					str_constr+=QString("c");
					atribs_tip += QString("concurrent") + QString(", ");
				}

				if(index->getIndexAttribute(Index::FAST_UPDATE))
				{
					str_constr+=QString("f");
					atribs_tip += QString("fast updated");
				}

				if(index->getIndexAttribute(Index::BUFFERING))
				{
					str_constr+=QString("b");
					atribs_tip += QString("buffering");
				}
			}
			else if(constr)
			{
				ConstraintType type = constr->getConstraintType();

				if(type == ConstraintType::primary_key)
					str_constr = TextPrimaryKey;
				else if(type == ConstraintType::foreign_key)
					str_constr = TextForeignKey;
				else if(type == ConstraintType::unique)
					str_constr = TextUnique;
				else if(type == ConstraintType::exclude)
					str_constr = TextExclude;
				else if(type == ConstraintType::check)
					str_constr = TextCheck;

				atribs_tip = (~type).toLower();
			}
			else if(policy)
			{
				if(policy->isPermissive())
				{
					str_constr += QString("p");
					atribs_tip += QString("permissive");
				}
				else
				{
					str_constr += QString("r");
					atribs_tip += QString("restrictive");
				}

				atribs_tip += QString(", ");
				str_constr += (~policy->getPolicyCommand()).toLower().at(0);
				atribs_tip += (~policy->getPolicyCommand()).toLower();
			}

			if(!str_constr.isEmpty())
				lables[2]->setText(ConstrDelimStart + QString(" ") +
								   str_constr + QString(" ") +
								   ConstrDelimEnd);
		}

		if(!atribs_tip.isEmpty())
		{
			if(atribs_tip.at(atribs_tip.length()-1)==' ')
				atribs_tip.remove(atribs_tip.length()-2, 2);

			atribs_tip=QString("\n") + ConstrDelimStart +
					   QString(" ") + atribs_tip + QString(" ") + ConstrDelimEnd;

		}

		if(!tab_obj->getComment().isEmpty())
			atribs_tip += QString("\n---\n%1").arg(tab_obj->getComment());

		lables[2]->setFont(fmt.font());
		lables[2]->setBrush(fmt.foreground());
		lables[2]->setPos(px, 0);

		//Calculating the object bounding rect that is composed by the join of the all object's child dimensions
		descriptor->setPos(HorizSpacing, lables[0]->boundingRect().center().y() - descriptor->boundingRect().center().y());
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
	double px;
	QString str_aux;

	configureDescriptor();
	descriptor->setPos(HorizSpacing, 1);
	px=descriptor->pos().x() + descriptor->boundingRect().width() + (2 * HorizSpacing);

	if(reference.getReferenceType()==Reference::REFER_COLUMN)
	{
		//Configures the name label as: [table].[column]
		fmt=font_config[ParsersAttributes::REF_TABLE];

		if(compact_view && !reference.getReferenceAlias().isEmpty())
			lables[0]->setText(reference.getReferenceAlias());
		else
			lables[0]->setText(reference.getTable()->getName() + ".");

		lables[0]->setFont(fmt.font());
		lables[0]->setBrush(fmt.foreground());
		lables[0]->setPos(px, 0);
		px+=lables[0]->boundingRect().width();

		fmt=font_config[ParsersAttributes::REF_COLUMN];
		if(compact_view && !reference.getReferenceAlias().isEmpty())
			lables[1]->setText(QString(" "));
		else
		{
			if(reference.getColumn())
				lables[1]->setText(reference.getColumn()->getName());
			else
				lables[1]->setText("*");
		}

		lables[1]->setFont(fmt.font());
		lables[1]->setBrush(fmt.foreground());
		lables[1]->setPos(px, 0);
		px+=lables[1]->boundingRect().width();
	}
	else
	{
		fmt=font_config[ParsersAttributes::REF_TABLE];
		str_aux = compact_view && !reference.getReferenceAlias().isEmpty() ? reference.getReferenceAlias() : QString();

		if(str_aux.isEmpty())
		{
			str_aux=reference.getExpression().simplified().mid(0,25);
			if(reference.getExpression().size() > 25) str_aux+=QString("...");
			str_aux.replace(QString("\n"), QString(" "));
		}

		lables[0]->setText(str_aux);
		lables[0]->setFont(fmt.font());
		lables[0]->setBrush(fmt.foreground());
		lables[1]->setText(QString());
		lables[0]->setPos(px, 0);
		px+=lables[0]->boundingRect().width();
	}

	//Configures a label for the alias (if there is one)
	if(!compact_view &&
		 ((reference.getColumn() && !reference.getColumnAlias().isEmpty()) ||
			(!reference.getAlias().isEmpty() && reference.getReferenceType()==Reference::REFER_EXPRESSION)))
	{
		if(reference.getReferenceType()==Reference::REFER_EXPRESSION)
			str_aux=reference.getAlias();
		else
			str_aux=reference.getColumnAlias();

		str_aux=QString(" (") + str_aux + QString(") ");
		fmt=font_config[ParsersAttributes::ALIAS];
		lables[2]->setText(str_aux);
		lables[2]->setFont(fmt.font());
		lables[2]->setBrush(fmt.foreground());
		lables[2]->setPos(px, 0);
	}
	else
		lables[2]->setText(QString());

	descriptor->setPos(HorizSpacing, lables[0]->boundingRect().center().y() - descriptor->boundingRect().center().y());
	bounding_rect.setTopLeft(QPointF(descriptor->pos().x(), lables[0]->pos().y()));

	if(lables[2]->text().isEmpty())
		bounding_rect.setBottomRight(QPointF(lables[1]->boundingRect().right(), lables[0]->boundingRect().bottom()));
	else
		bounding_rect.setBottomRight(QPointF(lables[2]->boundingRect().right(), lables[0]->boundingRect().bottom()));
}

void TableObjectView::setChildObjectXPos(unsigned obj_idx, double px)
{
	if(obj_idx >= 4)
		throw Exception(RefObjectInvalidIndex, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	if(obj_idx==0)
		descriptor->setPos(px, descriptor->pos().y());
	else
		lables[obj_idx-1]->setPos(px, lables[obj_idx-1]->pos().y());
}

QGraphicsItem *TableObjectView::getChildObject(unsigned obj_idx)
{
	if(obj_idx >= 4)
		throw Exception(RefObjectInvalidIndex, __PRETTY_FUNCTION__, __FILE__, __LINE__);

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
		vector<TableObject *>::iterator itr,itr_end;
		ConstraintType constr_type;

		itr=table->getObjectList(ObjConstraint)->begin();
		itr_end=table->getObjectList(ObjConstraint)->end();

		while(itr!=itr_end)
		{
			constr=dynamic_cast<Constraint *>(*itr);
			itr++;

			//Check if the column is referecend by the constraint
			if((constr->getConstraintType()!=ConstraintType::exclude && constr->isColumnExists(column, Constraint::SOURCE_COLS)) ||
				 (constr->getConstraintType()==ConstraintType::exclude && constr->isColumnReferenced(column, false)))
			{
				constr_type=constr->getConstraintType();

				if(constr_type==ConstraintType::primary_key)
					str_constr=TextPrimaryKey + ConstrSeparator + str_constr;

				if(constr_type==ConstraintType::foreign_key && str_constr.indexOf(TextForeignKey) < 0)
					str_constr+=TextForeignKey + ConstrSeparator;

				if(constr_type==ConstraintType::unique && str_constr.indexOf(TextUnique) < 0)
					str_constr+=TextUnique + ConstrSeparator;

				if(constr_type==ConstraintType::exclude && str_constr.indexOf(TextExclude) < 0)
					str_constr+=TextExclude + ConstrSeparator;
			}
		}

		if(column->isNotNull() && !str_constr.contains(TextPrimaryKey))
			str_constr+=TextNotNull + ConstrSeparator;

		if(!str_constr.isEmpty())
			str_constr= ConstrDelimStart +
						ConstrSeparator + str_constr +
						ConstrDelimEnd;

		return(str_constr);
	}
	else return(QString());
}

