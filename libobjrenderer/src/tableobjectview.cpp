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
	fake_selection=false;

	for(unsigned i=0; i < 3; i++)
		lables[i]=new QGraphicsSimpleTextItem;

	if(obj_selection)
		delete obj_selection;
}

TableObjectView::~TableObjectView()
{
	delete descriptor;

	for(unsigned i=0; i < 3; i++)
		delete lables[i];

	delete obj_selection;
}

void TableObjectView::configureDescriptor(ConstraintType constr_type)
{
	ObjectType obj_type=ObjectType::BaseObject;
	Column *column=dynamic_cast<Column *>(this->getUnderlyingObject());
	bool ellipse_desc=false;
	double factor=(font_config[Attributes::Global].font().pointSizeF()/DefaultFontSize) * BaseObjectView::getScreenDpiFactor();
	QPen pen;

	//Based upon the source object type the descriptor is allocated
	if(this->getUnderlyingObject())
		obj_type=this->getUnderlyingObject()->getObjectType();

	/* Elliptical descriptor is used to columns (with or without not-null constraint),
		for other object types, polygonal descriptor is usded */
	ellipse_desc=((column && constr_type==BaseType::Null) || (!TableObject::isTableObject(obj_type)));

	if(descriptor && ((ellipse_desc && !dynamic_cast<QGraphicsEllipseItem *>(descriptor)) ||
										(!ellipse_desc && dynamic_cast<QGraphicsEllipseItem *>(descriptor))))
	{
		delete descriptor;
		descriptor=nullptr;
	}

	if(!descriptor)
	{
		if(ellipse_desc)
			descriptor=new QGraphicsEllipseItem;
		else
			descriptor=new QGraphicsPolygonItem;
	}

	if(column)
	{
		QString attrib;
		QPolygonF pol;

		if(constr_type==BaseType::Null)
		{
			QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descriptor);

			desc->setRect(QRectF(QPointF(0,0), QSizeF(9.0 * factor, 9.0 * factor)));

			if(column->isNotNull())
				attrib=Attributes::NnColumn;
			else
				attrib=Attributes::Column;

			desc->setBrush(this->getFillStyle(attrib));

			pen = this->getBorderStyle(attrib);
			pen.setWidthF(ObjectBorderWidth * 1.15);
			desc->setPen(pen);
		}
		else
		{
			QGraphicsPolygonItem *desc=dynamic_cast<QGraphicsPolygonItem *>(descriptor);

			if(constr_type==ConstraintType::PrimaryKey)
			{
				attrib=Attributes::PkColumn;
				pol.append(QPointF(2,0)); pol.append(QPointF(0,2)); pol.append(QPointF(0,7));
				pol.append(QPointF(2,9)); pol.append(QPointF(3,8)); pol.append(QPointF(3,6));
				pol.append(QPointF(4,6)); pol.append(QPointF(5,7)); pol.append(QPointF(6,6));
				pol.append(QPointF(7,5)); pol.append(QPointF(9,7)); pol.append(QPointF(9,3));
				pol.append(QPointF(3,3)); pol.append(QPointF(3,1));
			}
			else if(constr_type==ConstraintType::ForeignKey)
			{
				attrib=Attributes::FkColumn;
				pol.append(QPointF(0,3)); pol.append(QPointF(0,6)); pol.append(QPointF(4,6));
				pol.append(QPointF(4,9)); pol.append(QPointF(5,9)); pol.append(QPointF(9,5));
				pol.append(QPointF(9,4)); pol.append(QPointF(5,0)); pol.append(QPointF(4,0));
				pol.append(QPointF(4,3));
			}
			else if(constr_type==ConstraintType::Unique)
			{
				attrib=Attributes::UqColumn;
				pol.append(QPointF(4,0)); pol.append(QPointF(0,4)); pol.append(QPointF(0,5));
				pol.append(QPointF(4,9)); pol.append(QPointF(5,9)); pol.append(QPointF(9,5));
				pol.append(QPointF(9,4)); pol.append(QPointF(5,0));
			}

			if(factor!=1.0)
				TextPolygonItem::resizePolygon(pol, pol.boundingRect().width() * factor,
																						pol.boundingRect().height()  * factor);

			desc->setPolygon(pol);
			desc->setBrush(this->getFillStyle(attrib));

			pen = this->getBorderStyle(attrib);
			pen.setWidthF(ObjectBorderWidth * 1.15);
			desc->setPen(pen);
		}
	}
	else if(obj_type != ObjectType::BaseObject)
	{
		TableObject *tab_obj=dynamic_cast<TableObject *>(this->getUnderlyingObject());
		QGraphicsPolygonItem *desc=dynamic_cast<QGraphicsPolygonItem *>(descriptor);
		QPolygonF pol;

		pol.append(QPointF(5,0)); pol.append(QPointF(0,5)); pol.append(QPointF(4,9));
		pol.append(QPointF(9,9)); pol.append(QPointF(9,4));

		if(factor!=1.0)
			TextPolygonItem::resizePolygon(pol,	pol.boundingRect().width() * factor ,	pol.boundingRect().height() * factor);

		desc->setPolygon(pol);
		desc->setBrush(this->getFillStyle(tab_obj->getSchemaName()));

		pen = this->getBorderStyle(tab_obj->getSchemaName());
		pen.setWidthF(ObjectBorderWidth * 1.15);
		desc->setPen(pen);
	}
	else
	{
		QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descriptor);

		desc->setRect(QRectF(QPointF(0,0), QSizeF(9.0 * factor, 9.0 * factor)));
		desc->setBrush(this->getFillStyle(Attributes::Reference));

		pen = this->getBorderStyle(Attributes::Reference);
		pen.setWidthF(ObjectBorderWidth * 1.15);
		desc->setPen(pen);
	}
}

void TableObjectView::configureObject()
{
	if(this->getUnderlyingObject())
	{
		QTextCharFormat fmt;
		double px = 0;
		QString str_constr, tooltip, atribs_tip;
		TableObject *tab_obj=dynamic_cast<TableObject *>(this->getUnderlyingObject());
		Column *column=dynamic_cast<Column *>(tab_obj);
		ConstraintType constr_type=ConstraintType::Null;
		bool sql_disabled=false;

		tooltip=tab_obj->getName() + QString(" (") + tab_obj->getTypeName() + QString(")");
		tooltip+=QString("\nId: %1").arg(tab_obj->getObjectId());
		sql_disabled=tab_obj->isSQLDisabled();
		fake_selection=false;

		if(column)
		{
			if(column->isAddedByRelationship())
				tooltip+=tr("\nRelationship: %1").arg(column->getParentRelationship()->getName());

			str_constr=this->getConstraintString(column);

			if(str_constr.indexOf(TextPrimaryKey)>=0)
			{
				fmt=font_config[Attributes::PkColumn];
				constr_type=ConstraintType::PrimaryKey;
			}
			else if(str_constr.indexOf(TextForeignKey)>=0)
			{
				fmt=font_config[Attributes::FkColumn];
				constr_type=ConstraintType::ForeignKey;
			}
			else if(str_constr.indexOf(TextUnique)>=0)
			{
				fmt=font_config[Attributes::UqColumn];
				constr_type=ConstraintType::Unique;
			}
			else if(str_constr.indexOf(TextNotNull)>=0)
				fmt=font_config[Attributes::NnColumn];
			else
				fmt=font_config[Attributes::Column];

			if(column->isAddedByRelationship())
				fmt=font_config[Attributes::InhColumn];
			else if(column->isProtected())
				fmt=font_config[Attributes::ProtColumn];

			if(str_constr.indexOf(TextPrimaryKey)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::PrimaryKey)).toLower() + QString(", ");

			if(str_constr.indexOf(TextForeignKey)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::ForeignKey)).toLower() + QString(", ");

			if(str_constr.indexOf(TextUnique)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::Unique)).toLower() + QString(", ");

			if(str_constr.indexOf(TextExclude)>=0)
				atribs_tip+=(~ConstraintType(ConstraintType::Exclude)).toLower() + QString(", ");

			if(str_constr.indexOf(TextNotNull)>=0)
				atribs_tip+=QString("not null");
		}
		else
		{
			if(tab_obj->isAddedByRelationship())
				fmt=font_config[Attributes::InhColumn];
			else if(tab_obj->isProtected())
				fmt=font_config[Attributes::ProtColumn];
			else
				fmt=font_config[tab_obj->getSchemaName()];
		}

		configureDescriptor(constr_type);

		descriptor->setPos(HorizSpacing, 0);
		px=HorizSpacing + descriptor->boundingRect().width() + (2 * HorizSpacing);

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
		fmt=font_config[Attributes::ObjectType];

		if(compact_view)
			lables[1]->setText(QString());
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
		fmt=font_config[Attributes::Constraints];
		if(compact_view)
			lables[2]->setText(QString());
		else if(column)
			lables[2]->setText(!str_constr.isEmpty() ? str_constr : QString(" "));
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

				for(unsigned i=EventType::OnInsert; i <= EventType::OnTruncate; i++)
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
				if(index->getIndexAttribute(Index::Unique))
				{
					str_constr+=QString("u");
					atribs_tip += QString("unique") + QString(", ");
				}

				if(index->getIndexAttribute(Index::Concurrent))
				{
					str_constr+=QString("c");
					atribs_tip += QString("concurrent") + QString(", ");
				}

				if(index->getIndexAttribute(Index::FastUpdate))
				{
					str_constr+=QString("f");
					atribs_tip += QString("fast updated");
				}

				if(index->getIndexAttribute(Index::Buffering))
				{
					str_constr+=QString("b");
					atribs_tip += QString("buffering");
				}
			}
			else if(constr)
			{
				ConstraintType type = constr->getConstraintType();

				if(type == ConstraintType::PrimaryKey)
					str_constr = TextPrimaryKey;
				else if(type == ConstraintType::ForeignKey)
					str_constr = TextForeignKey;
				else if(type == ConstraintType::Unique)
					str_constr = TextUnique;
				else if(type == ConstraintType::Exclude)
					str_constr = TextExclude;
				else if(type == ConstraintType::Check)
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
			else
				lables[2]->setText(QString());
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

		calculateBoundingRect();
		this->setToolTip(tooltip + atribs_tip);
	}
}

void TableObjectView::configureObject(Reference reference)
{
	QTextCharFormat fmt;
	double px;
	QString str_aux;

	configureDescriptor();
	descriptor->setPos(HorizSpacing, 0);
	px=descriptor->pos().x() + descriptor->boundingRect().width() + (2 * HorizSpacing);

	if(reference.getReferenceType()==Reference::ReferColumn)
	{
		//Configures the name label as: [table].[column]
		fmt=font_config[Attributes::RefTable];

		if(compact_view && !reference.getReferenceAlias().isEmpty())
			lables[0]->setText(reference.getReferenceAlias());
		else
			lables[0]->setText(reference.getTable()->getName() + ".");

		lables[0]->setFont(fmt.font());
		lables[0]->setBrush(fmt.foreground());
		lables[0]->setPos(px, 0);
		px+=lables[0]->boundingRect().width();

		fmt=font_config[Attributes::RefColumn];
		if(compact_view && !reference.getReferenceAlias().isEmpty())
			lables[1]->setText(QString());
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
		fmt=font_config[Attributes::RefTable];
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
			(!reference.getAlias().isEmpty() && reference.getReferenceType()==Reference::ReferExpression)))
	{
		if(reference.getReferenceType()==Reference::ReferExpression)
			str_aux=reference.getAlias();
		else
			str_aux=reference.getColumnAlias();

		str_aux=QString(" (") + str_aux + QString(") ");
		fmt=font_config[Attributes::Alias];
		lables[2]->setText(str_aux);
		lables[2]->setFont(fmt.font());
		lables[2]->setBrush(fmt.foreground());
		lables[2]->setPos(px, 0);
	}
	else
		lables[2]->setText(QString());

	calculateBoundingRect();
}

void TableObjectView::configureObject(const SimpleColumn &col)
{
	QTextCharFormat fmt;
	double px;

	configureDescriptor();
	descriptor->setPos(HorizSpacing, 0);
	px=descriptor->pos().x() + descriptor->boundingRect().width() + (2 * HorizSpacing);

	fmt = font_config[Attributes::Column];

	if(compact_view && !col.alias.isEmpty())
		lables[0]->setText(col.alias);
	else
		lables[0]->setText(col.name);

	lables[0]->setFont(fmt.font());
	lables[0]->setBrush(fmt.foreground());
	lables[0]->setPos(px, 0);
	px+=lables[0]->boundingRect().width() + (4 * HorizSpacing);

	if(!compact_view && !col.type.isEmpty())
	{
		fmt=font_config[Attributes::ObjectType];
		lables[1]->setText(col.type);
		lables[1]->setFont(fmt.font());
		lables[1]->setBrush(fmt.foreground());
		lables[1]->setPos(px, 0);
		px+=lables[1]->boundingRect().width() + (4 * HorizSpacing);
	}
	else
		lables[1]->setText(QString());

	lables[2]->setText(QString());
	calculateBoundingRect();
}

void TableObjectView::setChildObjectXPos(unsigned obj_idx, double px)
{
	if(obj_idx >= 4)
		throw Exception(ErrorCode::RefObjectInvalidIndex, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	if(obj_idx==0)
		descriptor->setPos(px, descriptor->pos().y());
	else
		lables[obj_idx-1]->setPos(px, lables[obj_idx-1]->pos().y());

	calculateBoundingRect();
}

void TableObjectView::calculateBoundingRect()
{
	double width = 0, height = 0, curr_w = 0, py = 0;

	width = descriptor->pos().x() + descriptor->boundingRect().width();
	height = lables[0]->boundingRect().height();

	for(int i = 0; i < 3; i++)
	{
		if(lables[i]->text().isEmpty())
			continue;

		curr_w = lables[i]->pos().x() + lables[i]->boundingRect().width();

		if(width < curr_w)
			width = lables[i]->pos().x() + lables[i]->boundingRect().width();
	}

	bounding_rect = QRectF(QPointF(0,0), QSizeF(width + (4 * HorizSpacing), height + VertSpacing * 0.80));

	//Adjusting the Y position of the objects in order to center them on the new bouding rect
	descriptor->setPos(descriptor->pos().x(),
										 (bounding_rect.height() - descriptor->boundingRect().height() + (VertSpacing * 0.4))/2);

	py = (bounding_rect.height() - lables[0]->boundingRect().height())/2;

	for(unsigned i = 0; i < 3; i++)
		lables[i]->setPos(lables[i]->pos().x(), py);
}

QGraphicsItem *TableObjectView::getChildObject(unsigned obj_idx)
{
	if(obj_idx > ConstrAliasLabel)
		throw Exception(ErrorCode::RefObjectInvalidIndex, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	if(obj_idx == ObjDescriptor)
		return descriptor;
	else
		return lables[obj_idx - 1];
}

QString TableObjectView::getConstraintString(Column *column)
{
	if(column && column->getParentTable())
	{
		PhysicalTable *table=dynamic_cast<PhysicalTable *>(column->getParentTable());
		QString str_constr;
		Constraint *constr=nullptr;
		vector<TableObject *>::iterator itr,itr_end;
		ConstraintType constr_type;

		itr=table->getObjectList(ObjectType::Constraint)->begin();
		itr_end=table->getObjectList(ObjectType::Constraint)->end();

		while(itr!=itr_end)
		{
			constr=dynamic_cast<Constraint *>(*itr);
			itr++;

			//Check if the column is referecend by the constraint
			if((constr->getConstraintType()!=ConstraintType::Exclude && constr->isColumnExists(column, Constraint::SourceCols)) ||
				 (constr->getConstraintType()==ConstraintType::Exclude && constr->isColumnReferenced(column, false)))
			{
				constr_type=constr->getConstraintType();

				if(constr_type==ConstraintType::PrimaryKey)
					str_constr=TextPrimaryKey + ConstrSeparator + str_constr;

				if(constr_type==ConstraintType::ForeignKey && str_constr.indexOf(TextForeignKey) < 0)
					str_constr+=TextForeignKey + ConstrSeparator;

				if(constr_type==ConstraintType::Unique && str_constr.indexOf(TextUnique) < 0)
					str_constr+=TextUnique + ConstrSeparator;

				if(constr_type==ConstraintType::Exclude && str_constr.indexOf(TextExclude) < 0)
					str_constr+=TextExclude + ConstrSeparator;
			}
		}

		if(column->isNotNull() && !str_constr.contains(TextPrimaryKey))
			str_constr+=TextNotNull + ConstrSeparator;

		if(!str_constr.isEmpty())
			str_constr= ConstrDelimStart + ConstrSeparator + str_constr + ConstrDelimEnd;

		return str_constr;
	}
	else return QString();
}

void TableObjectView::setFakeSelection(bool value)
{
	// Fake selection is used only by instances that own and underlying object (column, constratin, trigger,etc)
	if(!this->getUnderlyingObject())
		return;

	fake_selection = value;

	if(value)
	{
		configureObjectSelection();
		sel_order=++BaseObjectView::global_sel_order;
	}
	else
		sel_order = 0;

	update();
}

bool TableObjectView::hasFakeSelection()
{
	return fake_selection;
}

void TableObjectView::configureObjectSelection()
{
	QGraphicsItem *parent = this->parentItem();
	RoundedRectItem *rect_item=nullptr;
	QRectF rect = this->boundingRect();

	/* In order to avoid unnecessary memory usage by items that eventually will
	 * get selection we allocate the object selection rectangle only if the object
	 * itself is selected by the user, and it'll be allocated until the object's destruction */
	if(!obj_selection)
		obj_selection=new RoundedRectItem;

	rect_item = dynamic_cast<RoundedRectItem *>(obj_selection);
	rect.setX(0);
	rect.setY(0);
	rect.setHeight(rect.height() - VertSpacing);

	// An small hack to capture the width of the table in which the item is child of
	if(parent->parentItem())
		rect.setWidth(parent->parentItem()->boundingRect().width() - (2.5 * HorizSpacing));
	else
		rect.setWidth(rect.width() - (3.5 * HorizSpacing));

	rect_item->setBorderRadius(2);
	rect_item->setRect(rect);
	rect_item->setPos(0, VertSpacing/2);
	rect_item->setBrush(this->getFillStyle(Attributes::ObjSelection));
	rect_item->setPen(this->getBorderStyle(Attributes::ObjSelection));
}

void TableObjectView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();
	painter->translate(descriptor->pos());
	descriptor->paint(painter, option, widget);
	painter->restore();

	for(int i = 0 ; i < 3; i++)
	{
		if(lables[i]->text().isEmpty())
			continue;

		painter->save();
		painter->translate(lables[i]->pos());
		lables[i]->paint(painter, option, widget);
		painter->restore();
	}

	if(fake_selection)
	{
		painter->translate(obj_selection->pos());
		obj_selection->paint(painter, option, widget);
	}
}

QRectF TableObjectView::boundingRect() const
{
	return bounding_rect;
}

