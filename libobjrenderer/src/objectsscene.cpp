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

#include "objectsscene.h"

bool ObjectsScene::align_objs_grid=false;
bool ObjectsScene::show_grid=true;
bool ObjectsScene::show_page_delim=true;
unsigned ObjectsScene::grid_size=20;
QPrinter::PageSize ObjectsScene::paper_size=QPrinter::A4;
QPrinter::Orientation ObjectsScene::page_orientation=QPrinter::Landscape;
QRectF ObjectsScene::page_margins=QRectF(10,10,10,10);
QBrush ObjectsScene::grid;

ObjectsScene::ObjectsScene(void)
{
	moving_objs=false;
	this->setBackgroundBrush(grid);

	sel_ini_pnt.setX(NAN);
	sel_ini_pnt.setY(NAN);

	selection_rect=new QGraphicsPolygonItem;
	selection_rect->setVisible(false);
	selection_rect->setZValue(100);

	rel_line=new QGraphicsLineItem;
	rel_line->setVisible(false);
	rel_line->setZValue(-1);
	rel_line->setPen(QColor(80,80,80));

	this->addItem(selection_rect);
	this->addItem(rel_line);
}

ObjectsScene::~ObjectsScene(void)
{
	QGraphicsItemGroup *item=NULL;
	QList<QGraphicsItem *> items;
	ObjectType obj_types[]={ OBJ_RELATIONSHIP, OBJ_TEXTBOX,
													 OBJ_VIEW, OBJ_TABLE };
	unsigned i;

	this->removeItem(selection_rect);
	this->removeItem(rel_line);

	//Destroy the objects in the order defined on obj_types vector
	for(i=0; i < 4; i++)
	{
		items=this->items();

		while(!items.isEmpty())
		{
			/* Try to convert the item to QGraphicsItemGroup because all the objects
			used to represent database object are derived from this class */
			item=dynamic_cast<QGraphicsItemGroup *>(items.front());

			/* Case the object is converted to a item group and can be converted to database
			objects, indicates that the object can be removed from the scene */
			if(item && !item->parentItem() &&
				 ((dynamic_cast<RelationshipView *>(item) && obj_types[i]==OBJ_RELATIONSHIP) ||
					(dynamic_cast<TextboxView *>(item) && obj_types[i]==OBJ_TEXTBOX) ||
					(dynamic_cast<GraphicalView *>(item) && obj_types[i]==OBJ_VIEW) ||
					(dynamic_cast<TableView *>(item) && obj_types[i]==OBJ_TABLE)))

			{
				this->removeItem(item);
			}

			items.pop_front();
		}
	}
}

QPointF ObjectsScene::alignPointToGrid(const QPointF &pnt)
{
	QPointF p(roundf(pnt.x()/grid_size) * grid_size,
						roundf(pnt.y()/grid_size) * grid_size);

	if(p.x() < 0) p.setX(0);
	if(p.y() < 0) p.setY(0);

	return(p);
}

void ObjectsScene::setSceneRect(const QRectF &rect)
{
	QGraphicsScene::setSceneRect(0, 0, rect.width(), rect.height());
}

void ObjectsScene::setGridSize(unsigned size)
{
	if(size >= 20 || grid.style()==Qt::NoBrush)
	{
		QImage grid_img;
		float width, height, x, y;
		QSizeF aux_size;
		QPrinter printer;
		QPainter painter;
		QPen pen;

		if(paper_size!=QPrinter::Custom)
		{
			printer.setPageSize(paper_size);
			printer.setOrientation(page_orientation);
			printer.setPageMargins(page_margins.left(), page_margins.top(),
														 page_margins.right(), page_margins.bottom(), QPrinter::Millimeter);
			aux_size=printer.pageRect(QPrinter::DevicePixel).size();
		}
		//Case the paper size is custom use the margins as the paper size
		else
			aux_size=page_margins.size();


		width=fabs(roundf(aux_size.width()/static_cast<float>(size)) * size);
		height=fabs(roundf(aux_size.height()/static_cast<float>(size)) * size);

		grid_size=size;
		grid_img=QImage(width, height, QImage::Format_ARGB32);

		painter.begin(&grid_img);
		painter.fillRect(QRect(0,0,width,height), QColor(255,255,255));

		if(show_grid)
		{
			pen.setColor(QColor(225, 225, 225));
			painter.setPen(pen);

			//Draws the grid
			for(x=0; x < width; x+=size)
				for(y=0; y < height; y+=size)
					painter.drawRect(QRectF(QPointF(x,y),QPointF(x + size,y + size)));
		}

		//Creates the page delimiter lines
		if(show_page_delim)
		{
			pen.setColor(QColor(75,115,195));
			pen.setStyle(Qt::DashLine);
			pen.setWidthF(1.85f);
			painter.setPen(pen);
			painter.drawLine(width-1, 0,width-1,height-1);
			painter.drawLine(0, height-1,width-1,height-1);
		}

		painter.end();
		grid.setTextureImage(grid_img);
	}
}

void ObjectsScene::showRelationshipLine(bool value, const QPointF &p_start)
{
	QList<QGraphicsItem *> items=this->items();
	QGraphicsItem::GraphicsItemFlags flags;
	BaseObjectView *object=NULL;
	BaseGraphicObject *base_obj=NULL;

	if(!isnan(p_start.x()) && !isnan(p_start.y()))
		rel_line->setLine(QLineF(p_start,p_start));

	rel_line->setVisible(value);

	while(!items.isEmpty())
	{
		//When showing the relationship line all the objects cannot be moved
		flags=QGraphicsItem::ItemIsSelectable |
					QGraphicsItem::ItemSendsGeometryChanges;

		object=dynamic_cast<BaseObjectView *>(items.front());

		if(object && object->getSourceObject())
		{
			base_obj=dynamic_cast<BaseGraphicObject *>(object->getSourceObject());

			if(!value && base_obj &&
				 base_obj->getObjectType()!=OBJ_RELATIONSHIP &&
				 base_obj->getObjectType()!=BASE_RELATIONSHIP &&
				 !base_obj->isProtected())
				flags=QGraphicsItem::ItemIsMovable |
							QGraphicsItem::ItemIsSelectable |
							QGraphicsItem::ItemSendsGeometryChanges;
			else
				flags=QGraphicsItem::ItemIsSelectable |
							QGraphicsItem::ItemSendsGeometryChanges;
		}

		items.front()->setFlags(flags);
		items.pop_front();
	}
}

void ObjectsScene::setGridOptions(bool show_grd, bool align_objs_grd, bool show_pag_dlm)
{
	bool redef_grid=(ObjectsScene::show_grid!=show_grd ||
																						ObjectsScene::show_page_delim!=show_pag_dlm ||
																																					 grid.style()==Qt::NoBrush);

	ObjectsScene::show_grid=show_grd;
	ObjectsScene::show_page_delim=show_pag_dlm;
	ObjectsScene::align_objs_grid=align_objs_grd;

	if(redef_grid)
	{
		grid.setStyle(Qt::NoBrush);
		setGridSize(ObjectsScene::grid_size);
	}
}

void ObjectsScene::getGridOptions(bool &show_grd, bool &align_objs_grd, bool &show_pag_dlm)
{
	show_grd=ObjectsScene::show_grid;
	align_objs_grd=ObjectsScene::align_objs_grid;
	show_pag_dlm=ObjectsScene::show_page_delim;
}

void ObjectsScene::setPageConfiguration(QPrinter::PaperSize paper_sz, QPrinter::Orientation orient, QRectF margins)
{
	ObjectsScene::paper_size=paper_sz;
	ObjectsScene::page_orientation=orient;
	ObjectsScene::page_margins=margins;
}

void ObjectsScene::getPageConfiguration(QPrinter::PaperSize &paper_sz, QPrinter::Orientation &orient, QRectF &margins)
{
	paper_sz=ObjectsScene::paper_size;
	orient=ObjectsScene::page_orientation;
	margins=ObjectsScene::page_margins;
}

void ObjectsScene::emitObjectModification(BaseGraphicObject *object)
{
	emit s_objectModified(object);
}

void ObjectsScene::emitChildObjectSelection(TableObject *child_obj)
{
	/* Treats the TableView::s_childObjectSelect() only when there is no
		other object selected on the scene */
	if(this->selectedItems().isEmpty())
		emit s_popupMenuRequested(child_obj);
}

void ObjectsScene::emitObjectSelection(BaseGraphicObject *object, bool selected)
{
	if(object)
		emit s_objectSelected(object, selected);
}

void ObjectsScene::addItem(QGraphicsItem *item)
{
	if(item)
	{
		RelationshipView *rel=dynamic_cast<RelationshipView *>(item);
		BaseTableView *tab=dynamic_cast<BaseTableView *>(item);
		BaseObjectView *obj=dynamic_cast<BaseObjectView *>(item);

		if(rel)
			connect(rel, SIGNAL(s_relationshipModified(BaseGraphicObject*)),
							this, SLOT(emitObjectModification(BaseGraphicObject*)));
		else if(tab)
			connect(tab, SIGNAL(s_childObjectSelected(TableObject*)),
							this, SLOT(emitChildObjectSelection(TableObject*)));

		if(obj)
		{
			connect(obj, SIGNAL(s_objectSelected(BaseGraphicObject*,bool)),
							this, SLOT(emitObjectSelection(BaseGraphicObject*,bool)));
		}

		QGraphicsScene::addItem(item);
	}
}

void ObjectsScene::removeItem(QGraphicsItem *item)
{
	if(item)
	{
		BaseObjectView *object=dynamic_cast<BaseObjectView *>(item);
		RelationshipView *rel=dynamic_cast<RelationshipView *>(item);
		BaseTableView *tab=dynamic_cast<BaseTableView *>(item);

		if(rel)
		{
			disconnect(rel, NULL, this, NULL);
			rel->disconnectTables();
		}
		else if(tab)
			disconnect(tab, NULL, this, NULL);
		else if(object)
			disconnect(object, NULL, this, NULL);

		item->setVisible(false);
		item->setActive(false);
		QGraphicsScene::removeItem(item);
	}
}

void ObjectsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseDoubleClickEvent(event);

	if(this->selectedItems().size()==1 && event->buttons()==Qt::LeftButton && !rel_line->isVisible())
	{
		//Gets the selected graphical object
		BaseObjectView *obj=dynamic_cast<BaseObjectView *>(this->selectedItems().at(0));

		if(obj)
			emit s_objectDoubleClicked(dynamic_cast<BaseGraphicObject *>(obj->getSourceObject()));
	}
}

void ObjectsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//Gets the item at mouse position
	QGraphicsItem* item=this->itemAt(event->scenePos().x(), event->scenePos().y());

	/* If the relationship line is visible, indicates that the user is in the middle of
		 a relationship creation, thus is needed to inform to the scene to activate the
		 the multiselection to be able to select two tables and link them. By default,
		 the multiselection modifier is the Control key */
	if(rel_line->isVisible())
		event->setModifiers(Qt::ControlModifier);

	QGraphicsScene::mousePressEvent(event);

	if(event->buttons()==Qt::LeftButton)
	{
		//Selects the object (without press control) if the user is creating a relationship
		if(item && item->isEnabled() && !item->isSelected() &&  rel_line->isVisible())
			item->setSelected(true);
		else if(this->selectedItems().isEmpty())
		{
			sel_ini_pnt=event->scenePos();
			selection_rect->setVisible(true);
			emit s_objectSelected(NULL,false);
		}
	}
	else if(event->buttons()==Qt::RightButton)
	{
		//Case there is no item at the mouse position clears the selection on the scene
		if(!item)
			this->clearSelection();

		emit s_popupMenuRequested();
	}
}

void ObjectsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->buttons()==Qt::LeftButton)
	{
		if(!rel_line->isVisible())
		{
			//Case the user starts a object moviment
			if(!this->selectedItems().isEmpty() && !moving_objs && event->modifiers()==Qt::NoModifier)
			{
				emit s_objectsMoved(false);
				moving_objs=true;
			}

			//If the alignment to grid is active, adjust the event scene position
			if(align_objs_grid && !selection_rect->isVisible())
				event->setScenePos(this->alignPointToGrid(event->scenePos()));
			else if(selection_rect->isVisible())
			{
				QPolygonF pol;
				pol.append(sel_ini_pnt);
				pol.append(QPointF(event->scenePos().x(), sel_ini_pnt.y()));
				pol.append(QPointF(event->scenePos().x(), event->scenePos().y()));
				pol.append(QPointF(sel_ini_pnt.x(), event->scenePos().y()));
				selection_rect->setPolygon(pol);
				selection_rect->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::OBJ_SELECTION));
				selection_rect->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::OBJ_SELECTION));
			}
		}
	}

	if(rel_line->isVisible())
		rel_line->setLine(QLineF(rel_line->line().p1(), event->scenePos()));

	QGraphicsScene::mouseMoveEvent(event);
}

void ObjectsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseReleaseEvent(event);

	//If there is selected object and the user ends the object moviment
	if(!this->selectedItems().isEmpty() && moving_objs &&
		 event->button()==Qt::LeftButton && event->modifiers()==Qt::NoModifier)
	{
		unsigned i, count;
		QList<QGraphicsItem *> items=this->selectedItems();
		float x1,y1,x2,y2;
		QRectF rect;
		RelationshipView *rel=NULL;

		/* Get the extreme points of the scene to check if some objects are out the area
		 forcing the scene to be resized */
		x1=this->sceneRect().left();
		y1=this->sceneRect().top();
		x2=this->sceneRect().right();
		y2=this->sceneRect().bottom();

		count=items.size();
		for(i=0; i < count; i++)
		{
			rel=dynamic_cast<RelationshipView *>(items[i]);

			if(!rel)
			{
				if(align_objs_grid)
					items[i]->setPos(alignPointToGrid(items[i]->pos()));
				else
				{
					QPointF p=items[i]->pos();
					if(p.x() < 0) p.setX(0);
					if(p.y() < 0) p.setY(0);
					items[i]->setPos(p);
				}

				rect.setTopLeft(items[i]->pos());
				rect.setSize(items[i]->boundingRect().size());
			}
			else
			{
				rect=rel->__boundingRect();
			}

			//Made the comparisson between the scene extremity and the object's bounding rect
			if(rect.left() < x1) x1=rect.left();
			if(rect.top() < y1) y1=rect.top();
			if(rect.right() > x2) x2=rect.right();
			if(rect.bottom() > y2) y2=rect.bottom();
		}

		//Reconfigures the rectangle with the most extreme points
		rect.setCoords(x1, y1, x2, y2);

		//If the new rect is greater than the scene bounding rect, this latter is resized
		if(rect!=this->sceneRect())
		{
			rect=this->itemsBoundingRect();
			rect.setTopLeft(QPointF(0,0));
			rect.setWidth(rect.width() * 1.05f);
			rect.setHeight(rect.height() * 1.05f);
			this->setSceneRect(rect);
		}

		emit s_objectsMoved(true);
		moving_objs=false;
	}
	else if(selection_rect->isVisible() && event->button()==Qt::LeftButton)
	{
		QPolygonF pol;
		QPainterPath sel_area;

		sel_area.addRect(selection_rect->polygon().boundingRect());
		this->setSelectionArea(sel_area, Qt::IntersectsItemShape);

		selection_rect->setVisible(false);
		selection_rect->setPolygon(pol);
		sel_ini_pnt.setX(NAN);
		sel_ini_pnt.setY(NAN);
	}
}

void ObjectsScene::alignObjectsToGrid(void)
{
	QList<QGraphicsItem *> items=this->items();
	RelationshipView *rel=NULL;
	BaseTableView *tab=NULL;
	TextboxView *lab=NULL;
	vector<QPointF> points;
	vector<Schema *> schemas;
	unsigned i, count, i1, count1;

	count=items.size();
	for(i=0; i < count; i++)
	{
		if(dynamic_cast<QGraphicsItemGroup *>(items[i]) && !items[i]->parentItem())
		{
			tab=dynamic_cast<BaseTableView *>(items[i]);
			rel=dynamic_cast<RelationshipView *>(items[i]);

			if(tab)
				tab->setPos(this->alignPointToGrid(tab->pos()));
			else if(rel)
			{
				//Align the relationship points
				points=rel->getSourceObject()->getPoints();
				count1=points.size();
				for(i1=0; i1 < count1; i1++)
					points[i1]=this->alignPointToGrid(points[i1]);

				if(count1 > 0)
				{
					rel->getSourceObject()->setPoints(points);
					rel->configureLine();
				}

				//Align the labels
				for(i1=BaseRelationship::LABEL_SRC_CARD;
						i1<=BaseRelationship::LABEL_REL_NAME; i1++)
				{
					lab=rel->getLabel(i1);
					if(lab)
						lab->setPos(this->alignPointToGrid(lab->pos()));
				}
			}
			else if(!dynamic_cast<SchemaView *>(items[i]))
				items[i]->setPos(this->alignPointToGrid(items[i]->pos()));
			else
				schemas.push_back(dynamic_cast<Schema *>(dynamic_cast<BaseObjectView *>(items[i])->getSourceObject()));
		}
	}

	//Updating schemas dimensions
	while(!schemas.empty())
	{
		schemas.back()->setModified(true);
		schemas.pop_back();
	}
}

void ObjectsScene::update(void)
{
	this->setBackgroundBrush(grid);
	QGraphicsScene::update(this->sceneRect());
}
