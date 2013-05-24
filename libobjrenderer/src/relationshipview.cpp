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

#include "relationshipview.h"

bool RelationshipView::hide_name_label=false;

RelationshipView::RelationshipView(BaseRelationship *rel) : BaseObjectView(rel)
{
	if(!rel)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	for(unsigned i=BaseRelationship::SRC_CARD_LABEL;
			i <= BaseRelationship::REL_NAME_LABEL; i++)
	{
		if(rel->getLabel(i))
		{
			rel->getLabel(i)->setTextColor(BaseObjectView::getFontStyle(ParsersAttributes::LABEL).foreground().color());
			labels[i]=new TextboxView(rel->getLabel(i), true);
			labels[i]->setZValue(1);
			this->addToGroup(labels[i]);
		}
		else
			labels[i]=nullptr;
	}

	sel_object=nullptr;
	sel_object_idx=-1;
	configuring_line=false;

	descriptor=new QGraphicsPolygonItem;
	descriptor->setZValue(0);
	this->addToGroup(descriptor);

	tables[0]=tables[1]=nullptr;

	//Relationship has the minor Z, being on the bottom of scene object's stack
	this->setZValue(-1);
	this->configureObject();
}

RelationshipView::~RelationshipView(void)
{
	QGraphicsItem *item=nullptr;

	for(int i=0; i < 3; i++)
	{
		if(labels[i])
		{
			this->removeFromGroup(labels[i]);
			delete(labels[i]);
		}
	}

	while(!lines.empty())
	{
		item=lines.back();
		this->removeFromGroup(item);
		lines.pop_back();
		delete(item);
	}

	while(!attributes.empty())
	{
		item=attributes.back();
		this->removeFromGroup(item);
		attributes.pop_back();
		delete(item);
	}

	this->removeFromGroup(descriptor);
	delete(descriptor);
}

void RelationshipView::hideNameLabel(bool value)
{
	hide_name_label=value;
}

bool RelationshipView::isNameLabelHidden(void)
{
	return(hide_name_label);
}

BaseRelationship *RelationshipView::getSourceObject(void)
{
	return(dynamic_cast<BaseRelationship *>(this->BaseObjectView::getSourceObject()));
}

TextboxView *RelationshipView::getLabel(unsigned lab_idx)
{
	if(lab_idx > BaseRelationship::REL_NAME_LABEL)
		return(nullptr);
	else
		return(labels[lab_idx]);
}

QVariant RelationshipView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change==ItemPositionChange)
	{
		this->setFlag(QGraphicsItem::ItemIsMovable, false);
	}
	else if(change==ItemSelectedHasChanged)
	{
		unsigned i, count;
		QPen pen;
		QColor color;

		this->setSelectionOrder(value.toBool());
		pos_info_pol->setVisible(value.toBool());
		pos_info_txt->setVisible(value.toBool());
		obj_selection->setVisible(value.toBool());
		this->configurePositionInfo();

		for(i=0; i < 3; i++)
		{
			if(labels[i])
				labels[i]->itemChange(change, value);
		}

		//Show tha graphical points if 'value' is true
		count=graph_points.size();
		for(i=0; i < count; i++)
			graph_points[i]->setVisible(value.toBool());

		//Alter the relationship line color when it is selected
		if(value.toBool())
		{
			QColor cor1=BaseObjectView::getBorderStyle(ParsersAttributes::OBJ_SELECTION).color(),
					cor2=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP).color();

			color.setRedF((cor1.redF() + cor2.greenF())/2.0f);
			color.setGreenF((cor1.greenF() + cor2.greenF())/2.0f);
			color.setBlueF((cor1.blueF() + cor2.blueF())/2.0f);
		}
		else
			color=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP).color();

		count=lines.size();
		for(i=0; i < count; i++)
		{
			pen=lines[i]->pen();
			pen.setColor(color);
			lines[i]->setPen(pen);
		}

		//Shows/hides the attribute's selection
		count=attributes.size();
		for(i=0; i < count; i++)
			attributes[i]->childItems().at(3)->setVisible(value.toBool());


		emit s_objectSelected(dynamic_cast<BaseGraphicObject *>(this->getSourceObject()),
													value.toBool());
	}

	return(value);
}

void RelationshipView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//Marks the flag ItemIsMovable in order to permit the relationship to be selected
	this->setFlag(QGraphicsItem::ItemIsMovable);
	BaseObjectView::mousePressEvent(event);
	this->setFlag(QGraphicsItem::ItemIsMovable, false);

	if(!this->getSourceObject()->isProtected())
	{
		BaseRelationship *base_rel=this->getSourceObject();

		//Resets the labels position when mid-button is pressed
		if(event->buttons()==Qt::MidButton)
		{
			for(unsigned i=0; i < 3; i++)
				base_rel->setLabelDistance(i, QPointF(NAN,NAN));
			this->configureLabels();
		}
		else if(event->modifiers()==Qt::ShiftModifier)
		{
			QLineF lin;
			QPointF p;
			QRectF rect;
			unsigned i, count;
			bool pnt_rem=false;
			vector<QPointF> pontos=base_rel->getPoints();

			if(!base_rel->isSelfRelationship())
			{
				//When the user press SHIFT and clicks on a point this is removed
				if(event->buttons()==Qt::LeftButton)
				{
					emit s_relationshipModified(base_rel);

					count=graph_points.size();
					for(i=0; i < count; i++)
					{
						//Calculating the graphical point rect
						rect.setTopLeft(graph_points[i]->pos());
						rect.setSize(graph_points[i]->boundingRect().size());

						//Case the mouse pos is inside the point rect
						if(rect.contains(event->pos()))
						{
							pontos.erase(pontos.begin()+i);
							base_rel->setPoints(pontos);
							this->configureLine();
							pnt_rem=true;
							break;
						}
					}

					//Create a point when the user press SHIFT and clicks the line
					count=lines.size();
					for(i=0; i < count && !pnt_rem; i++)
					{
						/* Creates a auxiliary line based upon the cursor position. This
				 line is used to calculate the exact point (intersection) where the new one
				 must be inserted */
						lin.setP1(QPointF(event->pos().x()-50, event->pos().y()-50));
						lin.setP2(QPointF(event->pos().x()+50, event->pos().y()+50));


						//Case the auxiliary line intercepts one relationship line
						if(lines[i]->line().intersect(lin,&p)==QLineF::BoundedIntersection)
						{
							//Inserts the point to the line
							if(i >= pontos.size())
								pontos.push_back( event->pos());
							else
								pontos.insert(pontos.begin() + i, event->pos());
							base_rel->setPoints(pontos);

							this->configureLine();
							break;
						}
					}
				}
			}
		}
		//Clicking only with the left button, checks if some child object can be selected
		else if(event->button()==Qt::LeftButton)
		{
			QRectF rect;
			unsigned count, i;

			//Checks if there is some point selected
			count=graph_points.size();
			for(i=0; i < count && !sel_object; i++)
			{
				rect.setTopLeft(graph_points[i]->pos());
				rect.setSize(graph_points[i]->boundingRect().size());

				if(rect.contains(event->pos()))
				{
					sel_object=graph_points[i];
					sel_object_idx=i;
				}
			}

			//Checks if there is some label selected
			for(i=0; i < 3 && !sel_object; i++)
			{
				if(labels[i])
				{
					rect.setTopLeft(labels[i]->pos());
					rect.setSize(labels[i]->boundingRect().size());

					if(rect.contains(event->pos()))
					{
						sel_object=labels[i];
						sel_object_idx=i;
					}
				}
			}
		}
	}
}

void RelationshipView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->isSelected() && event->buttons()==Qt::LeftButton &&
		 !this->getSourceObject()->isProtected())
	{
		if(dynamic_cast<QGraphicsPolygonItem *>(sel_object))
		{
			BaseRelationship *rel_base=this->getSourceObject();
			vector<QPointF> points=rel_base->getPoints();

			points[sel_object_idx]=event->pos();
			rel_base->setPoints(points);
			this->configureLine();
		}
		else if(dynamic_cast<TextboxView *>(sel_object))
			sel_object->setPos(event->pos());
	}

	BaseObjectView::mouseMoveEvent(event);
}

void RelationshipView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	BaseRelationship *base_rel=this->getSourceObject();

	if(event->button()==Qt::LeftButton)
	{
		if(dynamic_cast<TextboxView *>(sel_object))
		{
			//Calculates the displacement of the label from the initial pos to the current
			base_rel->setLabelDistance(sel_object_idx,
																 QPointF(sel_object->pos() - labels_ini_pos[sel_object_idx]));
		}

		sel_object_idx=-1;
		sel_object=nullptr;
	}

	BaseObjectView::mouseReleaseEvent(event);
}

void RelationshipView::disconnectTables(void)
{
	for(unsigned i=0; i < 2; i++)
		disconnect(tables[i], nullptr, this, nullptr);
}

void RelationshipView::configureObject(void)
{
	BaseRelationship *rel_base=this->getSourceObject();

	tables[0]=dynamic_cast<BaseTableView *>(rel_base->getTable(BaseRelationship::SRC_TABLE)->getReceiverObject());
	tables[1]=dynamic_cast<BaseTableView *>(rel_base->getTable(BaseRelationship::DST_TABLE)->getReceiverObject());

	this->configureLine();

	for(unsigned i=0; i < 2; i++)
		connect(tables[i], SIGNAL(s_objectMoved(void)), this, SLOT(configureLine(void)));

	connect(rel_base, SIGNAL(s_objectModified()), this, SLOT(configureLine(void)));
}

void RelationshipView::configurePositionInfo(void)
{
	if(this->isSelected())
	{
		BaseObjectView::configurePositionInfo(descriptor->pos());
		pos_info_txt->setPos(descriptor->pos().x(),
												 descriptor->pos().y() - pos_info_txt->boundingRect().height());
		pos_info_pol->setPos(descriptor->pos().x(),
												 descriptor->pos().y() - pos_info_pol->boundingRect().height());
	}
}

void RelationshipView::configureLine(void)
{
	if(!configuring_line)
	{
		BaseRelationship *base_rel=this->getSourceObject();
		Relationship *rel=dynamic_cast<Relationship *>(base_rel);
		vector<QPointF> points;
		QLineF lin_aux[2];
		QGraphicsLineItem *lin=nullptr;
		QPointF pos, p_int, p_central[2];
		QRectF rect;
		QGraphicsItem *item=nullptr;
		QPen pen;
		QGraphicsPolygonItem *pol=nullptr;
		QPolygonF pol_aux;
		QString tool_tip;
		int i, i1, count, idx_lin_desc=0;

		configuring_line=true;

		if(base_rel->isSelfRelationship())
		{
			float fator=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;

			/* Sefl-relationshihp line format:

						+----<>
						|     |
		 ----------   |
		 | Table  |---+
		 ----------

		 This line cannot be moved or have points added */
			pos=tables[0]->pos();
			rect=tables[0]->boundingRect();

			p_central[0].setX(pos.x() + rect.width());
			p_central[0].setY(pos.y() + (rect.height()/2.5f));

			p_central[1].setX(pos.x() + (rect.width()/1.5f));
			p_central[1].setY(pos.y());

			points.push_back(QPointF(p_central[0].x() + (10 * fator),  p_central[0].y()));
			points.push_back(QPointF(p_central[0].x() + (10 * fator),  p_central[1].y() - (10 * fator)));
			points.push_back(QPointF(p_central[1].x(),  p_central[1].y() - (10 * fator)));
			base_rel->setPoints(points);
		}
		else
		{
			Relationship *rel=dynamic_cast<Relationship *>(base_rel);

			if(rel &&
				 rel->getRelationshipType()==Relationship::RELATIONSHIP_11 &&
				 rel->isIdentifier())
			{
				tables[0]=dynamic_cast<BaseTableView *>(rel->getReferenceTable()->getReceiverObject());
				tables[1]=dynamic_cast<BaseTableView *>(rel->getReceiverTable()->getReceiverObject());
			}

			for(i=0; i < 2; i++)
			{
				rect=tables[i]->boundingRect();
				pos=tables[i]->pos();

				//Calculates the table's cental point
				p_central[i].setX(pos.x() + (rect.width()/2.0f));
				p_central[i].setY(pos.y() + (rect.height()/2.0f));
			}

			points=base_rel->getPoints();

			/* Case there is no user added points the auxiliary line will be the one formed by
			the tables central points */
			if(points.empty())
			{
				lin_aux[0].setPoints(p_central[0], p_central[1]);
				lin_aux[1]=lin_aux[0];
			}
			/* Case there are points on the relationship, the auxiliary lines will be:
			1) between the source table central point and the first user added point
			2) between the destination table central point and the last user added point */
			else
			{
				lin_aux[0].setPoints(p_central[0], points[0]);
				lin_aux[1].setPoints(p_central[1], points[points.size()-1]);
			}

			count=points.size();
			pol_aux.append(QPointF(0,0)); pol_aux.append(QPointF(5,0));
			pol_aux.append(QPointF(5,5)); pol_aux.append(QPointF(0,5));

			for(i=0; i < count; i++)
			{
				if(i >= static_cast<int>(graph_points.size()))
				{
					pol=new QGraphicsPolygonItem;
					graph_points.push_back(pol);
					pol->setZValue(0);
					pol->setPolygon(pol_aux);
					pol->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::OBJ_SELECTION));
					pol->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::OBJ_SELECTION));
					this->addToGroup(pol);
				}
				else
					pol=graph_points[i];

				pol->setPos(QPointF(points[i].x()-GRAPHIC_PNT_RADIUS,
														points[i].y()-GRAPHIC_PNT_RADIUS));
				pol->setVisible(this->isSelected());
			}

			//Destroy the graphical points not used
			i=graph_points.size()-1;
			while(i > count-1)
			{
				item=graph_points.back();
				graph_points.pop_back();
				this->removeFromGroup(item);
				delete(item);
				i--;
			}
		}

		//For dependency relationships the line is dashed
		pen=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP);
		if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_DEP)
			pen.setStyle(Qt::DashLine);

		points.insert(points.begin(),p_central[0]);
		points.push_back(p_central[1]);

		/* For identifier relationships an additional point is created on the center of the
		 line that supports the descriptor in order to modify the line thickness on the
		 weak entity side */
		if(rel && rel->isIdentifier())
		{
			//Calculates the index of the initial point, on the line that supports the descriptor
			idx_lin_desc=(points.size()/2);
			p_central[0]=points[idx_lin_desc];

			//Gets the second line point
			if(idx_lin_desc + 1 > static_cast<int>(points.size()))
				p_central[1]=points[idx_lin_desc+1];
			else
				p_central[1]=points[idx_lin_desc-1];

			//Calculates the middle point and inserts it on the point vector
			p_int.setX((p_central[0].x() + p_central[1].x())/2.0f);
			p_int.setY((p_central[0].y() + p_central[1].y())/2.0f);
			points.insert(points.begin() + idx_lin_desc, p_int);
		}

		//Create the relationship lines
		count=points.size();
		for(i=0; i < count-1; i++)
		{
			if(i >= static_cast<int>(lines.size()))
			{
				lin=new QGraphicsLineItem;
				lin->setZValue(-1);
				lines.push_back(lin);
				this->addToGroup(lin);
			}
			else
				lin=lines[i];

			//If the relationship is identifier, the line has its thickness modified
			if(rel && rel->isIdentifier() && i >= idx_lin_desc)
				pen.setWidthF(1.6f);
			else
				pen.setWidthF(1.0f);

			lin->setLine(QLineF(points[i], points[i+1]));
			lin->setPen(pen);
		}

		if(!base_rel->isSelfRelationship())
		{
			i=points.size()-1;
			i1=lines.size();
			while(i1 > i)
			{
				item=lines.back();
				lines.pop_back();
				this->removeFromGroup(item);
				delete(item);
				i1--;
			}
		}

		this->configureDescriptor();
		this->configureLabels();
		this->configureProtectedIcon();
		configuring_line=false;

		/* Making a little tweak on the foreign key type name. Despite being of class BaseRelationship,
		for semantics purposes shows the type of this relationship as "Relationship" unlike "Link" */
		if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
			tool_tip=Utf8String::create(base_rel->getName(true)) +
							 " (" + BaseObject::getTypeName(OBJ_RELATIONSHIP) + ")";
		else
			tool_tip=Utf8String::create(base_rel->getName(true)) +
							 " (" + base_rel->getTypeName() + ")";

		this->setToolTip(tool_tip);

		for(i=0; i < 3; i++)
		{
			if(labels[i])
				labels[i]->setToolTip(tool_tip);
		}

		descriptor->setToolTip(tool_tip);
	}
}

void RelationshipView::configureDescriptor(void)
{
	QLineF lin;
	QPolygonF pol;
	BaseRelationship *base_rel=this->getSourceObject();
	Relationship *rel=dynamic_cast<Relationship *>(base_rel);
	unsigned rel_type=base_rel->getRelationshipType();
	float x, y, factor=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;
	QPen pen;
	QPointF pnt;
	vector<QPointF> points=base_rel->getPoints();

	pen=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP);

	if(rel_type==BaseRelationship::RELATIONSHIP_DEP)
		pen.setStyle(Qt::DashLine);

	descriptor->setPen(pen);
	descriptor->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::RELATIONSHIP));

	if(rel_type==BaseRelationship::RELATIONSHIP_DEP ||
		 rel_type==BaseRelationship::RELATIONSHIP_GEN)
	{
		pol.append(QPointF(0,0)); pol.append(QPointF(21,13));
		pol.append(QPointF(0,26)); pol.append(QPointF(0,13));
	}
	else
	{
		pol.append(QPointF(13,0)); pol.append(QPointF(26,13));
		pol.append(QPointF(13,26)); pol.append(QPointF(0,13));
	}

	//Resizes the polygon according the font factor
	if(factor!=1.0f)
		this->resizePolygon(pol,
												pol.boundingRect().width() * factor ,
												pol.boundingRect().height() * factor);

	if(base_rel->isSelfRelationship())
		pnt=points.at(points.size()/2);
	else
	{
		lin=lines.at(lines.size()/2)->line();

		if(rel && rel->isIdentifier())
			pnt=lin.p1();
		else
		{
			pnt.setX((lin.p1().x() + lin.p2().x()) / 2.0f);
			pnt.setY((lin.p1().y() + lin.p2().y()) / 2.0f);
		}

		descriptor->setRotation(-lin.angle());
		obj_selection->setRotation(-lin.angle());
		obj_shadow->setRotation(-lin.angle());
	}

	x=pnt.x() - (pol.boundingRect().width()/2.0f);
	y=pnt.y() - (pol.boundingRect().height()/2.0f);

	protected_icon->setPos(x + ((pol.boundingRect().width()/2.0f) * 0.60f),
												 y + ((pol.boundingRect().height()/2.0f) * 0.55f));

	descriptor->setPolygon(pol);
	descriptor->setTransformOriginPoint(descriptor->boundingRect().center());
	descriptor->setPos(x, y);

	obj_selection->setPolygon(pol);
	obj_selection->setTransformOriginPoint(obj_selection->boundingRect().center());
	obj_selection->setPos(x,y);
	obj_selection->setBrush(this->getFillStyle(ParsersAttributes::OBJ_SELECTION));
	obj_selection->setPen(this->getBorderStyle(ParsersAttributes::OBJ_SELECTION));

	obj_shadow->setPolygon(pol);
	obj_shadow->setTransformOriginPoint(obj_shadow->boundingRect().center());
	obj_shadow->setPos(x + 2.5f, y + 3.5f);
	obj_shadow->setPen(Qt::NoPen);
	obj_shadow->setBrush(QColor(50,50,50,60));

	this->configureAttributes();
	this->configurePositionInfo();
}

void RelationshipView::configureAttributes(void)
{
	Relationship *rel=dynamic_cast<Relationship *>(this->getSourceObject());

	if(rel)
	{
		int i, count;
		Column *col=nullptr;
		QGraphicsItemGroup *attrib=nullptr;
		QGraphicsLineItem *lin=nullptr;
		QGraphicsEllipseItem *desc=nullptr;
		QGraphicsPolygonItem *sel_attrib=nullptr;
		QGraphicsSimpleTextItem *text=nullptr;
		QGraphicsItemGroup *item=nullptr;
		QPointF p_aux;
		QTextCharFormat fmt;
		QFont font;
		QRectF rect;
		QPolygonF pol;
		float py, px,
				factor=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;

		fmt=font_config[ParsersAttributes::ATTRIBUTE];
		font=fmt.font();
		font.setPointSizeF(font.pointSizeF() * 0.80f);

		//Configures the rectangle used as base for creation of attribute descriptor
		rect.setTopLeft(QPointF(0,0));
		rect.setSize(QSizeF(8 * factor, 8 * factor));

		//Calculates the first attribute position based upon the attribute count and descriptor size
		count=rel->getAttributeCount();
		px=descriptor->pos().x() + descriptor->boundingRect().width() + ((3 * HORIZ_SPACING) * factor);
		py=descriptor->pos().y() - (count * rect.height()/(4.0f * factor));

		for(i=0; i < count; i++)
		{
			col=rel->getAttribute(i);

			if(i >= static_cast<int>(attributes.size()))
			{
				attrib=new QGraphicsItemGroup;
				attrib->setZValue(-1);

				//Creates the line that connects the attribute to the relationship descriptor
				lin=new QGraphicsLineItem;
				lin->setZValue(-1);
				attrib->addToGroup(lin);

				//Creates the attribute descriptor
				desc=new QGraphicsEllipseItem;
				desc->setZValue(0);
				attrib->addToGroup(desc);

				//Creates the attribute text
				text=new QGraphicsSimpleTextItem;
				text->setZValue(0);
				attrib->addToGroup(text);

				sel_attrib=new QGraphicsPolygonItem;
				sel_attrib->setZValue(1);
				sel_attrib->setVisible(false);
				attrib->addToGroup(sel_attrib);

				this->addToGroup(attrib);
				attributes.push_back(attrib);
			}
			else
			{
				attrib=attributes[i];
				lin=dynamic_cast<QGraphicsLineItem *>(attrib->childItems().at(0));
				desc=dynamic_cast<QGraphicsEllipseItem *>(attrib->childItems().at(1));
				text=dynamic_cast<QGraphicsSimpleTextItem *>(attrib->childItems().at(2));
				sel_attrib=dynamic_cast<QGraphicsPolygonItem *>(attrib->childItems().at(3));
			}

			desc->setRect(rect);
			desc->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::ATTRIBUTE));
			desc->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::ATTRIBUTE));
			lin->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP));
			text->setBrush(fmt.foreground());
			text->setFont(font);
			sel_attrib->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::OBJ_SELECTION));
			sel_attrib->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::OBJ_SELECTION));

			attrib->setPos(px, py);

			text->setText(Utf8String::create(col->getName()));
			text->setPos(QPointF(desc->pos().x() + desc->boundingRect().width() + (HORIZ_SPACING * factor), 0));
			desc->setPos(0, VERT_SPACING * factor);

			pol.clear();
			pol.append(text->boundingRect().topLeft());
			pol.append(text->boundingRect().topRight()  + QPointF(desc->boundingRect().width() + (HORIZ_SPACING * factor), 0));
			pol.append(text->boundingRect().bottomRight() + QPointF(desc->boundingRect().width() + (HORIZ_SPACING * factor), 0));
			pol.append(text->boundingRect().bottomLeft());
			sel_attrib->setPolygon(pol);

			p_aux=this->mapToItem(attrib, descriptor->pos().x() + (descriptor->boundingRect().width()/2.0f),
														descriptor->pos().y() + (descriptor->boundingRect().height()/2.0f));
			lin->setLine(QLineF(p_aux, desc->boundingRect().center()));

			py+=desc->boundingRect().height() + (2 * VERT_SPACING);
		}

		i=attributes.size()-1;
		while(i > count-1)
		{
			item=attributes.back();
			attributes.pop_back();
			this->removeFromGroup(item);
			delete(item);
			i--;
		}
	}
}

void RelationshipView::configureLabels(void)
{
	float x=0,y=0;
	QPointF pnt;
	BaseRelationship *base_rel=this->getSourceObject();
	unsigned rel_type=base_rel->getRelationshipType();
	QPointF label_dist;

	label_dist=base_rel->getLabelDistance(BaseRelationship::REL_NAME_LABEL);

	pnt=descriptor->pos();
	x=pnt.x() -
		((labels[BaseRelationship::REL_NAME_LABEL]->boundingRect().width() -
		 descriptor->boundingRect().width())/2.0f);

	if(base_rel->isSelfRelationship())
		y=pnt.y() -
			labels[BaseRelationship::REL_NAME_LABEL]->boundingRect().height() - (2 * VERT_SPACING);
	else
		y=pnt.y() + descriptor->boundingRect().height() + VERT_SPACING;

	labels_ini_pos[BaseRelationship::REL_NAME_LABEL]=QPointF(x,y);

	if(!std::isnan(label_dist.x()))
	{
		x+=label_dist.x();
		y+=label_dist.y();
	}

	labels[BaseRelationship::REL_NAME_LABEL]->setVisible(!hide_name_label);
	labels[BaseRelationship::REL_NAME_LABEL]->setPos(x,y);
	labels[BaseRelationship::REL_NAME_LABEL]->setFontStyle(BaseObjectView::getFontStyle(ParsersAttributes::LABEL));
	labels[BaseRelationship::REL_NAME_LABEL]->setColorStyle(BaseObjectView::getFillStyle(ParsersAttributes::LABEL),
																													BaseObjectView::getBorderStyle(ParsersAttributes::LABEL));
	dynamic_cast<Textbox *>(labels[BaseRelationship::REL_NAME_LABEL]->getSourceObject())->setModified(true);

	if(rel_type!=BaseRelationship::RELATIONSHIP_GEN &&
		 rel_type!=BaseRelationship::RELATIONSHIP_DEP)
	{
		QPointF pi, pf, p_int, pos;
		unsigned idx, i1;
		float dl, da;
		QLineF lins[2], borders[2][4];
		QRectF tab_rect, rect;
		unsigned label_ids[2]={ BaseRelationship::SRC_CARD_LABEL,
														BaseRelationship::DST_CARD_LABEL };

		lins[0]=lines[0]->line();
		lins[1]=lines[lines.size()-1]->line();


		/* Creating lines that represents the tables border in order to calculate the
		 position of the cardinality labels via intersection point */
		for(idx=0; idx < 2; idx++)
		{
			rect=tables[idx]->boundingRect();
			pos=tables[idx]->pos();

			//Cria as linhas de borda da tabela
			borders[idx][0].setPoints(pos, QPointF(pos.x(), pos.y() + rect.height()));
			borders[idx][1].setPoints(QPointF(pos.x(), pos.y() + rect.height()),
																QPointF(pos.x() + rect.width(), pos.y() + rect.height()));
			borders[idx][2].setPoints(QPointF(pos.x() + rect.width(), pos.y()),
																QPointF(pos.x() + rect.width(), pos.y() + rect.height()));
			borders[idx][3].setPoints(pos, QPointF(pos.x() + rect.width(), pos.y()));
		}

		for(idx=0; idx < 2; idx++)
		{
			for(i1=0; i1 < 4; i1++)
			{
				if(lins[idx].intersect(borders[idx][i1], &p_int)==QLineF::BoundedIntersection)
				{
					if(idx==0)
						lins[idx].setP1(p_int);
					else
						lins[idx].setP2(p_int);
					break;
				}
			}
		}

		for(idx=0; idx < 2; idx++)
		{
			if(idx==0)
			{
				pi=lins[idx].p1();
				pf=lins[idx].p2();
			}
			else
			{
				pi=lins[idx].p2();
				pf=lins[idx].p1();
			}

			dl=labels[label_ids[idx]]->boundingRect().width()/2.0f;
			da=labels[label_ids[idx]]->boundingRect().height()/2.0f;

			x=pi.x() - dl;
			y=pi.y() - da;

			tab_rect.setTopLeft(tables[idx]->pos());
			tab_rect.setSize(tables[idx]->boundingRect().size());

			rect.setTopLeft(QPointF(x,y));
			rect.setSize(labels[idx]->boundingRect().size());

			if(rect.contains(tab_rect.bottomRight()))
			{ x+=dl + HORIZ_SPACING; y+=da + VERT_SPACING; }
			else if(rect.contains(tab_rect.bottomLeft()))
			{ x-=dl + HORIZ_SPACING; y+=da + VERT_SPACING; }
			else if(rect.contains(tab_rect.topLeft()))
			{ x-=dl + HORIZ_SPACING; y-=da + VERT_SPACING; }
			else if(rect.contains(tab_rect.topRight()))
			{ x+=dl + HORIZ_SPACING;  y-=da + VERT_SPACING; }
			else
			{
				if(tab_rect.contains(rect.bottomLeft()) && tab_rect.contains(rect.bottomRight()))
					y-=da + VERT_SPACING;
				else if(tab_rect.contains(rect.topLeft()) && tab_rect.contains(rect.topRight()))
					y+=da + VERT_SPACING;

				if(tab_rect.contains(rect.topRight()) && tab_rect.contains(rect.bottomRight()))
					x-=dl + HORIZ_SPACING;
				else if(tab_rect.contains(rect.topLeft()) && tab_rect.contains(rect.bottomLeft()))
					x+=dl + HORIZ_SPACING;
			}

			labels_ini_pos[label_ids[idx]]=QPointF(x,y);
			label_dist=base_rel->getLabelDistance(label_ids[idx]);
			if(!std::isnan(label_dist.x()))
			{
				x+=label_dist.x();
				y+=label_dist.y();
			}

			labels[label_ids[idx]]->setPos(x,y);
			labels[label_ids[idx]]->setFontStyle(BaseObjectView::getFontStyle(ParsersAttributes::LABEL));
			labels[label_ids[idx]]->setColorStyle(BaseObjectView::getFillStyle(ParsersAttributes::LABEL),
																						BaseObjectView::getBorderStyle(ParsersAttributes::LABEL));
			dynamic_cast<Textbox *>(labels[label_ids[idx]]->getSourceObject())->setModified(true);
		}
	}
}

QRectF RelationshipView::__boundingRect(void)
{
	float x1=0, y1=0, x2=0, y2=0;
	unsigned i, count;
	QPointF p;
	QRectF rect;
	vector<QPointF> points=dynamic_cast<BaseRelationship *>(this->getSourceObject())->getPoints();

	//The reference size will be the relationship descriptor dimension
	x1=descriptor->pos().x();
	y1=descriptor->pos().y();
	x2=descriptor->pos().x() + descriptor->boundingRect().width();
	y2=descriptor->pos().y() + descriptor->boundingRect().height();

	//Checks if some point is out of reference dimension
	count=points.size();
	for(i=0; i < count; i++)
	{
		p=points[i];
		if(x1 > p.x()) x1=p.x() - GRAPHIC_PNT_RADIUS;
		if(y1 > p.y()) y1=p.y() - GRAPHIC_PNT_RADIUS;
		if(x2 < p.x()) x2=p.x() + GRAPHIC_PNT_RADIUS;
		if(y2 < p.y()) y2=p.y() + GRAPHIC_PNT_RADIUS;
	}

	//Checks if some label is out of reference dimension
	for(i=0; i < 3; i++)
	{
		if(labels[i])
		{
			rect.setTopLeft(labels[i]->scenePos());
			rect.setSize(labels[i]->boundingRect().size());
			if(x1 > rect.left()) x1=rect.left();
			if(y1 > rect.top()) y1=rect.top();
			if(x2 < rect.right()) x2=rect.right();
			if(y2 < rect.bottom()) y2=rect.bottom();
		}
	}

	return(QRectF(x1, y1, x2, y2));
}
