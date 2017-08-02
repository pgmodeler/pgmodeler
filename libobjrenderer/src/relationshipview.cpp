/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2017 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
unsigned RelationshipView::line_conn_mode=RelationshipView::CONNECT_FK_TO_PK;

RelationshipView::RelationshipView(BaseRelationship *rel) : BaseObjectView(rel)
{
	if(!rel)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	//path = new QGraphicsPathItem;
	//this->addToGroup(path);

	for(unsigned i=BaseRelationship::SRC_CARD_LABEL;
		i <= BaseRelationship::REL_NAME_LABEL; i++)
	{
		if(rel->getLabel(i))
		{
			labels[i]=new TextboxView(rel->getLabel(i), true);
			labels[i]->setZValue(i==BaseRelationship::REL_NAME_LABEL ? 0 : 1);
			this->addToGroup(labels[i]);
		}
		else
			labels[i]=nullptr;
	}

	sel_object=nullptr;
	sel_object_idx=-1;
	configuring_line=false;
	using_placeholders=BaseObjectView::isPlaceholderEnabled();

	descriptor=new QGraphicsPolygonItem;
	descriptor->setZValue(0);
	this->addToGroup(descriptor);

	obj_shadow=new QGraphicsPolygonItem;
	obj_shadow->setZValue(-1);
	this->addToGroup(obj_shadow);

	obj_selection=new QGraphicsPolygonItem;
	obj_selection->setZValue(4);
	obj_selection->setVisible(false);
	this->addToGroup(obj_selection);

	tables[0]=tables[1]=nullptr;

	for(unsigned i=0; i < 2; i++)
	{
		line_circles[i]=new QGraphicsEllipseItem;
		line_circles[i]->setRect(QRectF(0,0,GRAPHIC_PNT_RADIUS,GRAPHIC_PNT_RADIUS));
		line_circles[i]->setZValue(0);
		line_circles[i]->setVisible(false);
		this->addToGroup(line_circles[i]);
	}

	//Relationship has the minor Z, being on the bottom of scene object's stack
	this->setZValue(-1);
	this->configureObject();
}

RelationshipView::~RelationshipView(void)
{
	QGraphicsItem *item=nullptr;

	while(!curves.empty())
	{
		this->removeFromGroup(curves.back());
		item = curves.back();
		curves.pop_back();
		delete(item);
	}

	for(int i=0; i < 2; i++)
	{
		this->removeFromGroup(line_circles[i]);
		delete(line_circles[i]);
	}

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

	while(!fk_lines.empty())
	{
		item=fk_lines.back();
		this->removeFromGroup(item);
		fk_lines.pop_back();
		delete(item);
	}

	while(!pk_lines.empty())
	{
		item=pk_lines.back();
		this->removeFromGroup(item);
		pk_lines.pop_back();
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

void RelationshipView::setHideNameLabel(bool value)
{
	hide_name_label=value;
}

bool RelationshipView::isNameLabelHidden(void)
{
	return(hide_name_label);
}

void RelationshipView::setLineConnectionMode(unsigned mode)
{
	if(mode > CONNECT_FK_TO_PK)
		mode=CONNECT_FK_TO_PK;

	line_conn_mode=mode;
}

unsigned RelationshipView::getLineConnectinMode(void)
{
	return(line_conn_mode);
}

QPointF RelationshipView::getConnectionPoint(unsigned table_idx)
{
	if(table_idx > 2)
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(conn_points[table_idx]);
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
		QColor color, line_color=this->getSourceObject()->getCustomColor();
		vector<QGraphicsLineItem *> rel_lines;

		this->setSelectionOrder(value.toBool());
		pos_info_rect->setVisible(value.toBool());
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
		if(line_color==Qt::transparent)
			line_color=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP).color();

		if(value.toBool())
		{
			QColor cor1=BaseObjectView::getBorderStyle(ParsersAttributes::OBJ_SELECTION).color(),
					cor2=line_color;

			color.setRedF((cor1.redF() + cor2.greenF())/2.0f);
			color.setGreenF((cor1.greenF() + cor2.greenF())/2.0f);
			color.setBlueF((cor1.blueF() + cor2.blueF())/2.0f);
			color.setAlphaF((cor1.alphaF() + cor2.alphaF())/2.0f);
		}
		else
			color=line_color;

		rel_lines=lines;
		rel_lines.insert(rel_lines.end(), fk_lines.begin(), fk_lines.end());
		rel_lines.insert(rel_lines.end(), pk_lines.begin(), pk_lines.end());

		for(auto &lin : rel_lines)
		{
			pen=lin->pen();
			pen.setColor(color);
			lin->setPen(pen);
		}

		for(auto &curve : curves)
		{
			pen = curve->pen();
			pen.setColor(color);
			curve->setPen(pen);
		}

		//Shows/hides the attribute's selection
		count=attributes.size();
		for(i=0; i < count; i++)
			attributes[i]->childItems().at(3)->setVisible(value.toBool());

		emit s_objectSelected(dynamic_cast<BaseGraphicObject *>(this->getSourceObject()),	value.toBool());
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
			base_rel->resetLabelsDistance();
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
	if(tables[0] && tables[1])
	{
		BaseRelationship *rel_base=this->getSourceObject();
		tables[0]->updateConnectedRelsCount(-1);

		if(!rel_base->isSelfRelationship())
			tables[1]->updateConnectedRelsCount(-1);

		for(unsigned i=0; i < 2; i++)
		{
			tables[i]->disconnect(this);
			tables[i]=nullptr;
		}
	}
}

void RelationshipView::connectTables(void)
{
	if(tables[0] && tables[1])
	{
		for(unsigned i=0; i < 2; i++)
		{
			tables[i]->disconnect(this);

			if(BaseObjectView::isPlaceholderEnabled())
				connect(tables[i], SIGNAL(s_relUpdateRequest(void)), this, SLOT(configureLine(void)));
			else
				connect(tables[i], SIGNAL(s_objectMoved(void)), this, SLOT(configureLine(void)));

			connect(tables[i], SIGNAL(s_objectDimensionChanged(void)), this, SLOT(configureLine(void)));
		}
	}
}

void RelationshipView::configureObject(void)
{
	BaseRelationship *rel_base=this->getSourceObject();

	tables[0]=dynamic_cast<BaseTableView *>(rel_base->getTable(BaseRelationship::SRC_TABLE)->getReceiverObject());
	tables[1]=dynamic_cast<BaseTableView *>(rel_base->getTable(BaseRelationship::DST_TABLE)->getReceiverObject());

	tables[0]->updateConnectedRelsCount(1);

	if(!rel_base->isSelfRelationship())
		tables[1]->updateConnectedRelsCount(1);

	configureLine();
	connectTables();
	connect(rel_base, SIGNAL(s_objectModified()), this, SLOT(configureLine(void)));
}

void RelationshipView::configurePositionInfo(void)
{
	if(this->isSelected())
	{
		BaseObjectView::configurePositionInfo(descriptor->pos());
		pos_info_txt->setPos(descriptor->pos().x(),
							 descriptor->pos().y() - pos_info_txt->boundingRect().height());
		pos_info_rect->setPos(descriptor->pos().x(),
							  descriptor->pos().y() - pos_info_rect->boundingRect().height());
	}
}

void RelationshipView::configureLine(void)
{
	//Reconnect the tables is the placeholder usage changes
	if(using_placeholders!=BaseObjectView::isPlaceholderEnabled())
	{
		connectTables();
		using_placeholders=BaseObjectView::isPlaceholderEnabled();
	}

	if(!configuring_line)
	{
		BaseRelationship *base_rel=this->getSourceObject();
		Relationship *rel=dynamic_cast<Relationship *>(base_rel);
		vector<QPointF> points, fk_points, pk_points;
		QGraphicsLineItem *lin=nullptr;
		QPointF pos, p_int, p_central[2], pk_pnt, fk_pnt;
		QRectF rect;
		QPen pen;
		QGraphicsPolygonItem *pol=nullptr;
		QPolygonF pol_aux;
		QString tool_tip;
		QGraphicsItem *item=nullptr;
		int i, i1, count, idx_lin_desc=0;
		bool bidirectional=base_rel->isBidirectional();

		configuring_line=true;
		pen.setCapStyle(Qt::RoundCap);

		if(base_rel->isSelfRelationship())
		{
			double fator=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;

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

			points.push_back(QPointF(p_central[0].x() + (11 * fator),  p_central[0].y()));
			points.push_back(QPointF(p_central[0].x() + (11 * fator),  p_central[1].y() - (11 * fator)));
			points.push_back(QPointF(p_central[1].x(),  p_central[1].y() - (11 * fator)));
			base_rel->setPoints(points);
		}
		else
		{
			Relationship *rel=dynamic_cast<Relationship *>(base_rel);
			bool rel_1n=(!bidirectional &&
									 (base_rel->getRelationshipType()==Relationship::RELATIONSHIP_11 ||
										base_rel->getRelationshipType()==Relationship::RELATIONSHIP_1N ||
										base_rel->getRelationshipType()==Relationship::RELATIONSHIP_FK));

			if(rel &&
				 rel->getRelationshipType()==Relationship::RELATIONSHIP_11 &&
				 rel->isIdentifier())
			{
				tables[0]=dynamic_cast<BaseTableView *>(rel->getReferenceTable()->getReceiverObject());
				tables[1]=dynamic_cast<BaseTableView *>(rel->getReceiverTable()->getReceiverObject());
			}

			if(line_conn_mode==CONNECT_CENTER_PNTS || !rel_1n)
			{
				vector<vector<QGraphicsLineItem *> *> ref_lines={ &fk_lines, &pk_lines };

				for(i=0; i < 2; i++)
					p_central[i]=tables[i]->getCenter();

				//Destroying the fk and pk connection lines when the line mode changes
				for(i=0; i < 2; i++)
				{
					while(!ref_lines[i]->empty())
					{
						item=ref_lines[i]->back();
						ref_lines[i]->pop_back();
						this->removeFromGroup(item);
						delete(item);
					}
				}
			}
			else if(line_conn_mode==CONNECT_FK_TO_PK && rel_1n)
			{
				QPointF pnt;
				QRectF rec_tab_rect, ref_tab_rect;
				double fk_py=0, pk_py=0, fk_px=0, pk_px=0;
				vector<Constraint *> fks;
				Table *ref_tab=nullptr, *rec_tab=nullptr;
				TableView *ref_tab_view=nullptr, *rec_tab_view=nullptr;
				unsigned cnt=0, i=0, pk_pnt_type=0, fk_pnt_type=0;

				if(!rel)
				{
					ref_tab=dynamic_cast<Table *>(base_rel->getTable(BaseRelationship::DST_TABLE));
					rec_tab=dynamic_cast<Table *>(base_rel->getTable(BaseRelationship::SRC_TABLE));
				}
				else
				{
					ref_tab=rel->getReferenceTable();
					rec_tab=rel->getReceiverTable();
				}

				rec_tab->getForeignKeys(fks, true, ref_tab);
				ref_tab_view=dynamic_cast<TableView *>(ref_tab->getReceiverObject());
				rec_tab_view=dynamic_cast<TableView *>(rec_tab->getReceiverObject());

				//Create the table's rectangles to detect where to connect the relationship
				ref_tab_rect=QRectF(ref_tab_view->pos(), ref_tab_view->boundingRect().size());

				//In this case the receiver table rect Y must be equal to reference table Y in order to do the correct comparison
				rec_tab_rect=QRectF(QPointF(rec_tab_view->pos().x(),
											ref_tab_view->pos().y()), rec_tab_view->boundingRect().size());

				if(ref_tab_rect.intersects(rec_tab_rect))
				{
					//Connects the rectangle at the same sides on both tables
					if(rec_tab_rect.center().x() >= ref_tab_rect.center().x())
						pk_pnt_type=fk_pnt_type=BaseTableView::LEFT_CONN_POINT;
					else if(rec_tab_rect.center().x() < ref_tab_rect.center().x())
						pk_pnt_type=fk_pnt_type=BaseTableView::RIGHT_CONN_POINT;
				}
				else
				{
					//Connecting the relationship on the opposite sides depending on the tables' position
					if(ref_tab_rect.right() <= rec_tab_rect.left())
					{
						pk_pnt_type=BaseTableView::RIGHT_CONN_POINT;
						fk_pnt_type=BaseTableView::LEFT_CONN_POINT;
					}
					else
					{
						pk_pnt_type=BaseTableView::LEFT_CONN_POINT;
						fk_pnt_type=BaseTableView::RIGHT_CONN_POINT;
					}
				}

				for(auto &constr : fks)
				{
					cnt=constr->getColumnCount(Constraint::SOURCE_COLS);

					for(i=0; i < cnt; i++)
					{
						pnt=rec_tab_view->getConnectionPoints(constr->getColumn(i, Constraint::SOURCE_COLS), fk_pnt_type);
						fk_py+=pnt.y();
						fk_px=pnt.x();
						fk_points.push_back(this->mapFromItem(rec_tab_view, pnt));

						pnt=ref_tab_view->getConnectionPoints(constr->getColumn(i, Constraint::REFERENCED_COLS), pk_pnt_type);
						pk_py+=pnt.y();
						pk_px=pnt.x();
						pk_points.push_back(this->mapFromItem(ref_tab_view, pnt));
					}
				}

				if(!fks.empty())
				{
					double pk_dx=(pk_pnt_type==BaseTableView::LEFT_CONN_POINT ? -CONN_LINE_LENGTH : CONN_LINE_LENGTH),
							fk_dx=(fk_pnt_type==BaseTableView::LEFT_CONN_POINT ? -CONN_LINE_LENGTH : CONN_LINE_LENGTH);

					pk_pnt=this->mapFromItem(ref_tab_view, QPointF(pk_px + pk_dx, pk_py/pk_points.size()));
					fk_pnt=this->mapFromItem(rec_tab_view, QPointF(fk_px + fk_dx, fk_py/fk_points.size()));

					if(base_rel->getRelationshipType()==Relationship::RELATIONSHIP_FK)
					{
						p_central[1]=pk_pnt;
						p_central[0]=fk_pnt;
					}
					else
					{
						p_central[0]=pk_pnt;
						p_central[1]=fk_pnt;
					}
				}
				else
				{
					/* Fallback configuration: If no fk was found in the receiver table uses
					 * the tables' center points to configure the line in order to avoid glitched lines.
					 * This situation may happen when the relationship is being validated and the needed fks was not
					 * created yet. In a second interaction of the rel. validation they are created
					 *  and the relationship is properly configured */
					if(base_rel->getRelationshipType()==Relationship::RELATIONSHIP_FK)
					{
						p_central[1]=pk_pnt=ref_tab_view->getCenter();
						p_central[0]=fk_pnt=rec_tab_view->getCenter();
					}
					else
					{
						p_central[0]=pk_pnt=ref_tab_view->getCenter();
						p_central[1]=fk_pnt=rec_tab_view->getCenter();
					}
				}
			}

			points=base_rel->getPoints();
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

				pol->setPos(points[i]);
				pol->moveBy(-GRAPHIC_PNT_RADIUS/2.5, -GRAPHIC_PNT_RADIUS/2.5);
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

		conn_points[0]=p_central[0];
		conn_points[1]=p_central[1];

		//Configuring the relationship line color
		if(base_rel->getCustomColor()!=Qt::transparent)
			//Using custom color
			pen.setColor(base_rel->getCustomColor());
		else
			//Using the default color
			pen=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP);

		//For dependency relationships the line is dashed
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

		if(line_conn_mode==CONNECT_FK_TO_PK)
		{
			vector<QPointF> ref_points={ fk_pnt, pk_pnt };
			vector<vector<QPointF> *> ref_pnt_vects={ &fk_points, &pk_points };
			vector<vector<QGraphicsLineItem *> *> ref_lines={ &fk_lines, &pk_lines };
			vector<QPointF> *ref_pnt=nullptr;
			vector<QGraphicsLineItem *> *ref_lin=nullptr;

			for(unsigned vet_idx=0; vet_idx < 2; vet_idx++)
			{
				ref_pnt=ref_pnt_vects[vet_idx];
				ref_lin=ref_lines[vet_idx];
				count=ref_pnt->size();

				for(i=0; i < count; i++)
				{
					if(i >= static_cast<int>(ref_lin->size()))
					{
						lin=new QGraphicsLineItem;
						lin->setZValue(-1);
						ref_lin->push_back(lin);
						this->addToGroup(lin);
					}
					else
						lin=ref_lin->at(i);

					//If the relationship is identifier or bidirectional, the line has its thickness modified
					if(rel && (rel->isIdentifier() && vet_idx==0))
						pen.setWidthF(1.75f);
					else
						pen.setWidthF(1.15f);

					lin->setLine(QLineF(ref_pnt->at(i), ref_points[vet_idx]));
					lin->setPen(pen);
				}

				//Destroy the unused pk or fk lines
				i=ref_lin->size()-1;
				while(i > static_cast<int>(count-1))
				{
					item=ref_lin->back();
					ref_lin->pop_back();
					this->removeFromGroup(item);
					delete(item);
					i--;
				}
			}
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

			//If the relationship is identifier or bidirectional, the line has its thickness modified
			if(bidirectional || (rel && (rel->isIdentifier() && i >= idx_lin_desc)))
				pen.setWidthF(1.75f);
			else
				pen.setWidthF(1.15f);

			lin->setLine(QLineF(points[i], points[i+1]));
			lin->setPen(pen);
		}

		//Removing unused lines
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

		//Exposing the line ending circles
		if((!base_rel->isSelfRelationship() && line_conn_mode==CONNECT_CENTER_PNTS) ||
				base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_DEP ||
				base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_GEN ||
				base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_NN)
		{
			for(i=0; i < 2; i++)
			{
				line_circles[i]->setVisible(true);
				line_circles[i]->setPen(pen);
				line_circles[i]->setBrush(pen.color());
				line_circles[i]->setPos(p_central[i]-QPointF(GRAPHIC_PNT_RADIUS/2, GRAPHIC_PNT_RADIUS/2));
			}
		}
		else
		{
			line_circles[0]->setVisible(false);
			line_circles[1]->setVisible(false);
		}

		//Using bezier curves instead of straight lines to denote the relationship line
		while(!curves.empty())
		{
			this->removeFromGroup(curves.back());
			delete(curves.back());
			curves.pop_back();
		}

		if(!base_rel->isSelfRelationship())
		{
			QGraphicsPathItem * path = nullptr;
			QRectF brect;
			QPainterPath ppath;

			for(auto &line : lines)
			{
				path = new QGraphicsPathItem;
				path->setZValue(line->zValue());

				if(line->line().p1().x() < line->line().p2().x())
				{
					brect.setTopLeft(line->line().p1());
					brect.setBottomRight(line->line().p2());
				}
				else
				{
					brect.setTopLeft(line->line().p2());
					brect.setBottomRight(line->line().p1());
				}

				ppath = QPainterPath(brect.topLeft());
				ppath.cubicTo(QPointF(brect.center().x(), brect.top()),
											QPointF(brect.center().x(), brect.bottom()),
											brect.bottomRight());

				path->setPen(line->pen());
				path->setPath(ppath);

				curves.push_back(path);
				this->addToGroup(path);
				//line->setVisible(false);
			}
		}

		this->configureDescriptor();
		this->configureLabels();
		this->configureProtectedIcon();

		configuring_line=false;

		/* Making a little tweak on the foreign key type name. Despite being of class BaseRelationship,
		for semantics purposes shows the type of this relationship as "Relationship" unlike "Link" */
		if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
			tool_tip=base_rel->getName(true) +
					 QString(" (") + BaseObject::getTypeName(OBJ_RELATIONSHIP) + QString(")");
		else
			tool_tip=base_rel->getName(true) +
					 QString(" (") + base_rel->getTypeName() + QString(")");

		tool_tip += QString("\nId: %1").arg(base_rel->getObjectId());
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
	double x, y, x1, y1, factor=(font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE) * BaseObjectView::getScreenDpiFactor();
	QPen pen;
	QPointF pnt;
	vector<QPointF> points=base_rel->getPoints();
	QColor line_color=base_rel->getCustomColor();
	QGraphicsPolygonItem *pol_item=nullptr;

	//Configuring the relationship descriptor color
	if(base_rel->getCustomColor()!=Qt::transparent)
		//Using custom color
		pen.setColor(base_rel->getCustomColor());
	else
		//Using the default color
		pen=BaseObjectView::getBorderStyle(ParsersAttributes::RELATIONSHIP);

	if(rel_type==BaseRelationship::RELATIONSHIP_DEP)
		pen.setStyle(Qt::DashLine);

	descriptor->setPen(pen);

	if(line_color!=Qt::transparent)
	{
		QColor colors[2];
		QLinearGradient grad;
		BaseObjectView::getFillStyle(ParsersAttributes::RELATIONSHIP, colors[0], colors[1]);

		for(unsigned i=0; i < 2; i++)
		{
			colors[i].setRed((colors[i].red() + line_color.red() + 255)/3);
			colors[i].setGreen((colors[i].green() + line_color.green() + 255)/3);
			colors[i].setBlue((colors[i].blue() + line_color.blue() + 255)/3);
			grad.setColorAt(i, colors[i]);
		}

		grad.setCoordinateMode(QGradient::ObjectBoundingMode);;
		descriptor->setBrush(grad);
	}
	else
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

		if(!curves.empty())
		{
			QPainterPath path(lin.p1());
			QRectF rect;

			if(lin.p1().x() < lin.p2().x())
			{
				rect.setTopLeft(lin.p1());
				rect.setBottomRight(lin.p2());
			}
			else
			{
				rect.setTopLeft(lin.p2());
				rect.setBottomRight(lin.p1());
			}

			path.cubicTo(rect.bottomLeft(), rect.topRight(), lin.p2());

			/*qreal ang = QLineF(path.pointAtPercent(0.4), path.pointAtPercent(0.7)).angle();
			qreal ang1 = lin.angle();

			QTextStream out(stdout);
			out << ang << endl;
			out << ang1 << endl;
			out << "--"<<endl;

			descriptor->setRotation((ang + ang1)/2); */

			QGraphicsLineItem * ln = new QGraphicsLineItem(QLineF(path.pointAtPercent(0.45), path.pointAtPercent(0.55)));
			this->addToGroup(ln);
			descriptor->setRotation(-lin.angle());
		}
		else
		{
			descriptor->setRotation(-lin.angle());
			obj_selection->setRotation(-lin.angle());
			obj_shadow->setRotation(-lin.angle());
		}
	}

	x=x1=pnt.x() - (pol.boundingRect().width()/2.0f);
	y=y1=pnt.y() - (pol.boundingRect().height()/2.0f);

	protected_icon->setPos(x + ((pol.boundingRect().width()/2.0f) * 0.60f),
												 y + ((pol.boundingRect().height()/2.0f) * 0.55f));

	configureSQLDisabledInfo();
	x1+=6 * HORIZ_SPACING;
	y1-=3 * VERT_SPACING;
	sql_disabled_box->setPos(x1, y1);
	sql_disabled_txt->setPos(x1 + HORIZ_SPACING, y1 + VERT_SPACING);

	descriptor->setPolygon(pol);
	descriptor->setTransformOriginPoint(descriptor->boundingRect().center());
	descriptor->setPos(x, y);

	pol_item=dynamic_cast<QGraphicsPolygonItem *>(obj_selection);
	pol_item->setPolygon(pol);
	pol_item->setTransformOriginPoint(obj_selection->boundingRect().center());
	pol_item->setPos(x,y);
	pol_item->setBrush(this->getFillStyle(ParsersAttributes::OBJ_SELECTION));
	pol_item->setPen(this->getBorderStyle(ParsersAttributes::OBJ_SELECTION));

	pol_item=dynamic_cast<QGraphicsPolygonItem *>(obj_shadow);
	pol_item->setPolygon(pol);
	pol_item->setTransformOriginPoint(obj_shadow->boundingRect().center());
	pol_item->setPos(x + 2.5f, y + 3.5f);
	pol_item->setPen(Qt::NoPen);
	pol_item->setBrush(QColor(50,50,50,60));

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
		double py, px,
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

			text->setText(col->getName());
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
	double x=0,y=0;
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
		y=pnt.y() -	labels[BaseRelationship::REL_NAME_LABEL]->boundingRect().height() - (2 * VERT_SPACING);
	else
		y=pnt.y() + descriptor->boundingRect().height() + VERT_SPACING;

	labels[BaseRelationship::REL_NAME_LABEL]->setVisible(!hide_name_label);
	configureLabelPosition(BaseRelationship::REL_NAME_LABEL, x, y);

	if(rel_type!=BaseRelationship::RELATIONSHIP_GEN &&
			rel_type!=BaseRelationship::RELATIONSHIP_DEP)
	{
		QPointF pi, pf, p_int, pos;
		unsigned idx, i1;
		double dl, da, factor, v_space=VERT_SPACING * 2.5, h_space=HORIZ_SPACING * 2.5;
		QLineF lins[2], borders[2][4];
		QRectF tab_rect, rect;
		unsigned label_ids[2]={ BaseRelationship::SRC_CARD_LABEL,
								BaseRelationship::DST_CARD_LABEL };

		if(!base_rel->isSelfRelationship() &&
				line_conn_mode==CONNECT_FK_TO_PK && rel_type!=BaseRelationship::RELATIONSHIP_NN)
		{
			for(idx=0; idx < 2; idx++)
			{
				pos=conn_points[idx];
				da=labels[idx]->boundingRect().height()/2.0f;

				if((rel_type!=BaseRelationship::RELATIONSHIP_FK && pos.x() < tables[idx]->pos().x()) ||
						(rel_type==BaseRelationship::RELATIONSHIP_FK && pos.x() >= tables[idx]->pos().x()))
				{
					factor=(rel_type==BaseRelationship::RELATIONSHIP_FK ? 0.80 : 0.55);
					x=pos.x() - (labels[idx]->boundingRect().width() * factor);
				}
				else
				{
					factor=(rel_type==BaseRelationship::RELATIONSHIP_FK ? 0.05 : 0.45);
					x=pos.x() - (labels[idx]->boundingRect().width() * factor);
				}

				configureLabelPosition(label_ids[idx], x, pos.y() - da);
			}
		}
		else
		{
			lins[0]=lines[0]->line();
			lins[1]=lines[lines.size()-1]->line();

			/* Creating lines that represents the tables border in order to calculate the
				 position of the cardinality labels via intersection point */
			for(idx=0; idx < 2; idx++)
			{
				rect=tables[idx]->boundingRect();
				pos=tables[idx]->pos();

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
				{ x+=dl + h_space; y+=da + v_space; }
				else if(rect.contains(tab_rect.bottomLeft()))
				{ x-=dl + h_space; y+=da + v_space; }
				else if(rect.contains(tab_rect.topLeft()))
				{ x-=dl + h_space; y-=da + v_space; }
				else if(rect.contains(tab_rect.topRight()))
				{ x+=dl + h_space; y-=da + v_space; }
				else
				{
					if(tab_rect.contains(rect.bottomLeft()) && tab_rect.contains(rect.bottomRight()))
						y-=da + v_space;
					else if(tab_rect.contains(rect.topLeft()) && tab_rect.contains(rect.topRight()))
						y+=da + v_space;

					if(tab_rect.contains(rect.topRight()) && tab_rect.contains(rect.bottomRight()))
						x-=dl + h_space;
					else if(tab_rect.contains(rect.topLeft()) && tab_rect.contains(rect.bottomLeft()))
						x+=dl + h_space;
				}

				configureLabelPosition(label_ids[idx], x, y);
			}
		}
	}
}

void RelationshipView::configureLabelPosition(unsigned label_id, double x, double y)
{
	if(label_id > BaseRelationship::REL_NAME_LABEL)
		throw Exception(ERR_REF_OBJ_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(labels[label_id])
	{
		BaseRelationship *base_rel=this->getSourceObject();
		QTextCharFormat char_fmt;
		QPointF label_dist;

		labels_ini_pos[label_id]=QPointF(x,y);
		label_dist=base_rel->getLabelDistance(label_id);

		if(!std::isnan(label_dist.x()))
		{
			x+=label_dist.x();
			y+=label_dist.y();
		}

		labels[label_id]->setPos(x,y);
		labels[label_id]->setToolTip(this->toolTip());

		char_fmt=BaseObjectView::getFontStyle(ParsersAttributes::LABEL);
		char_fmt.setFontPointSize(char_fmt.fontPointSize() * 0.90);
		labels[label_id]->setFontStyle(char_fmt);
		labels[label_id]->setColorStyle(BaseObjectView::getFillStyle(ParsersAttributes::LABEL),
										BaseObjectView::getBorderStyle(ParsersAttributes::LABEL));
		dynamic_cast<Textbox *>(labels[label_id]->getSourceObject())->setModified(true);
	}
}

QRectF RelationshipView::__boundingRect(void)
{
	double x1=0, y1=0, x2=0, y2=0;
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
