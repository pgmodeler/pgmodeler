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

#include "objectsscene.h"

bool ObjectsScene::align_objs_grid=false;
bool ObjectsScene::show_grid=true;
bool ObjectsScene::show_page_delim=true;
unsigned ObjectsScene::grid_size=20;
QPrinter::PageSize ObjectsScene::paper_size=QPrinter::A4;
QPrinter::Orientation ObjectsScene::page_orientation=QPrinter::Landscape;
QRectF ObjectsScene::page_margins=QRectF(2,2,2,2);
QSizeF ObjectsScene::custom_paper_size=QSizeF(0,0);
QBrush ObjectsScene::grid;
bool ObjectsScene::corner_move=true;
bool ObjectsScene::invert_rangesel_trigger=false;

ObjectsScene::ObjectsScene()
{
	layers.push_back(tr("Default layer"));
	active_layers.push_back(layers.at(0));

	moving_objs=move_scene=false;
	enable_range_sel=true;
	this->setBackgroundBrush(grid);

	sel_ini_pnt.setX(DNaN);
	sel_ini_pnt.setY(DNaN);

	selection_rect=new QGraphicsPolygonItem;
	selection_rect->setVisible(false);
	selection_rect->setZValue(100);

	rel_line=new QGraphicsLineItem;
	rel_line->setVisible(false);
	rel_line->setZValue(-1);
	rel_line->setPen(QColor(80,80,80));

	this->addItem(selection_rect);
	this->addItem(rel_line);

	scene_move_dx=scene_move_dy=0;

	connect(&scene_move_timer, SIGNAL(timeout()), this, SLOT(moveObjectScene()));
	connect(&corner_hover_timer, SIGNAL(timeout()), this, SLOT(enableSceneMove()));

	connect(&object_move_timer, &QTimer::timeout, [&](){
		//If the timer reaches its timeout we execute the procedures to finish the objects movement
		finishObjectsMove(itemsBoundingRect(true, true).center());
		object_move_timer.stop();
	});

	scene_move_timer.setInterval(SceneMoveTimeout);
	corner_hover_timer.setInterval(SceneMoveTimeout * 10);
	object_move_timer.setInterval(SceneMoveTimeout * 10);
}

ObjectsScene::~ObjectsScene()
{
	QGraphicsItemGroup *item=nullptr;
	QList<QGraphicsItem *> items;
	vector<ObjectType> obj_types={ ObjectType::Relationship, ObjectType::Textbox, ObjectType::View,
																 ObjectType::Table, ObjectType::ForeignTable, ObjectType::Schema };

	this->removeItem(selection_rect);
	this->removeItem(rel_line);

	delete selection_rect;
	delete rel_line;

	//Destroy the objects in the order defined on obj_types vector
	for(auto &type : obj_types)
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
					((dynamic_cast<RelationshipView *>(item) && type==ObjectType::Relationship) ||
					 (dynamic_cast<TextboxView *>(item) && type==ObjectType::Textbox) ||
					 (dynamic_cast<StyledTextboxView *>(item) && type==ObjectType::Textbox) ||
					 (dynamic_cast<GraphicalView *>(item) && type==ObjectType::View) ||
					 (dynamic_cast<TableView *>(item) && (type==ObjectType::Table || type==ObjectType::ForeignTable)) ||
					 (dynamic_cast<SchemaView *>(item) && type==ObjectType::Schema)))

			{
				this->removeItem(item);
			}

			items.pop_front();
		}
	}

	//The graphical representation of db objects must be destroyed in a sorted way
	std::sort(removed_objs.begin(), removed_objs.end());
	while(!removed_objs.empty())
	{
		delete removed_objs.back();
		removed_objs.pop_back();
	}
}

QString ObjectsScene::formatLayerName(const QString &name)
{
	QString fmt_name;
	unsigned idx = 1;

	//Removing invalid chars
	for(auto &chr : name)
	{
		if(chr.isLetterOrNumber() || chr == ' ' || chr == '_')
			fmt_name.append(chr);
		else
			fmt_name.append('_');
	}

	//Doing the desambiguation (if needed)
	while(layers.contains(fmt_name))
		fmt_name = QString("%1 %2").arg(name).arg(QString::number(idx++));

	return fmt_name;
}

QString ObjectsScene::addLayer(const QString &name)
{
	if(name.isEmpty())
		return QString();

	QString fmt_name = formatLayerName(name);
	layers.push_back(fmt_name);

	emit s_layersChanged();
	return fmt_name;
}

QString ObjectsScene::renameLayer(unsigned idx, const QString &name)
{
	if(name.isEmpty() || idx >= static_cast<unsigned>(layers.size()))
		return QString();

	if(name != layers[idx])
		layers[idx] = formatLayerName(name);

	emit s_layersChanged();
	return layers[idx];
}

void ObjectsScene::removeLayer(const QString &name)
{
	int idx = layers.indexOf(name);

	if(idx > 0)
	{
		moveObjectsToLayer(idx, DefaultLayer);
		layers.removeAll(name);
		active_layers.removeAll(name);
		emit s_layersChanged();
	}
}

void ObjectsScene::removeLayers()
{
	BaseObjectView *obj_view = nullptr;
	QString def_layer = layers[DefaultLayer];
	bool is_active = active_layers.contains(def_layer);

	layers.clear();
	active_layers.clear();
	layers.push_back(def_layer);

	if(is_active)
		active_layers.push_back(def_layer);

	for(auto &item : this->items())
	{
		obj_view = dynamic_cast<BaseObjectView *>(item);

		if(obj_view && !obj_view->parentItem() && obj_view->getLayer() != DefaultLayer)
		{
			obj_view->setLayer(DefaultLayer);
			obj_view->setVisible(is_active);
		}
	}

	emit s_layersChanged();
	updateActiveLayers();
}

void ObjectsScene::setActiveLayers(QStringList act_layers)
{
	QList<unsigned> layers_idxs;
	int idx = -1;

	for(auto &layer : act_layers)
	{
		idx = layers.indexOf(layer);

		if(idx >= 0)
			layers_idxs.push_back(idx);
	}

	setActiveLayers(layers_idxs);
}

void ObjectsScene::setActiveLayers(QList<unsigned> layers_idxs)
{
	BaseObjectView *obj_view = nullptr;
	active_layers.clear();

	if(!layers_idxs.isEmpty())
	{
		bool is_in_layer = false;
		unsigned layer_cnt = static_cast<unsigned>(layers.size());
		SchemaView *sch_view = nullptr;

		for(auto &item : this->items())
		{
			obj_view = dynamic_cast<BaseObjectView *>(item);

			if(obj_view && !obj_view->parentItem() && obj_view->getLayer() < layer_cnt)
			{
				sch_view = dynamic_cast<SchemaView *>(obj_view);
				is_in_layer = layers_idxs.contains(obj_view->getLayer());

				if(!obj_view->isVisible() && is_in_layer)
				{
					if(!sch_view ||
						 (sch_view && dynamic_cast<Schema *>(sch_view->getUnderlyingObject())->isRectVisible()))
					 obj_view->setVisible(true);
				}
				else if(obj_view->isVisible() && !is_in_layer)
					obj_view->setVisible(false);
			}
		}

		for(auto &idx : layers_idxs)
		{
			if(idx < layer_cnt)
				active_layers.push_back(layers[idx]);
		}
	}
	else
	{
		for(auto &item : this->items())
		{
			obj_view = dynamic_cast<BaseObjectView *>(item);

			if(obj_view && !obj_view->parentItem())
				obj_view->setVisible(false);
		}
	}

	emit s_activeLayersChanged();
}

void ObjectsScene::moveObjectsToLayer(unsigned old_layer, unsigned new_layer)
{
	BaseObjectView *obj_view = nullptr;
	unsigned total_layers = layers.size();

	if(old_layer == new_layer || old_layer >= total_layers || new_layer >= total_layers)
		return;

	for(auto &item : this->items())
	{
		obj_view = dynamic_cast<BaseObjectView *>(item);

		if(obj_view && !obj_view->parentItem() && obj_view->getLayer() == old_layer)
		{
			obj_view->setLayer(new_layer);
			obj_view->setVisible(isLayerActive(layers[new_layer]));
		}
	}

	emit s_objectsMovedLayer();
}

bool ObjectsScene::isLayerActive(const QString &name)
{
	return active_layers.contains(name);
}

bool ObjectsScene::isLayerActive(unsigned layer_id)
{
	if(layer_id >= static_cast<unsigned>(layers.size()))
		return false;

	return active_layers.contains(layers[layer_id]);
}

QStringList ObjectsScene::getActiveLayers()
{
	return active_layers;
}

QList<unsigned> ObjectsScene::getActiveLayersIds()
{
	QList<unsigned> list;

	for(auto &layer : active_layers)
		list.push_back(layers.indexOf(layer));

	return list;
}

QStringList ObjectsScene::getLayers()
{
	return layers;
}

unsigned ObjectsScene::getLayerId(const QString &name)
{
	int idx = layers.contains(name);
	return idx < 0 ? InvalidLayer : static_cast<unsigned>(idx);
}

void ObjectsScene::updateActiveLayers()
{
	setActiveLayers(active_layers);
}

void ObjectsScene::setEnableCornerMove(bool enable)
{
	ObjectsScene::corner_move=enable;
}

void ObjectsScene::setInvertRangeSelectionTrigger(bool invert)
{
	ObjectsScene::invert_rangesel_trigger=invert;
}

bool ObjectsScene::isCornerMoveEnabled()
{
	return ObjectsScene::corner_move;
}

QPointF ObjectsScene::alignPointToGrid(const QPointF &pnt)
{
	int px = static_cast<int>(round(pnt.x()/static_cast<double>(grid_size))) * grid_size,
			py = static_cast<int>(round(pnt.y()/static_cast<double>(grid_size))) * grid_size;

	if(px < 0) px = 0;
	if(py < 0) py = 0;

	return QPointF(px,	py);
}

void ObjectsScene::setSceneRect(const QRectF &rect)
{
	QGraphicsScene::setSceneRect(0, 0, rect.width(), rect.height());
}

QRectF ObjectsScene::itemsBoundingRect(bool seek_only_db_objs, bool selected_only)
{
	if(!seek_only_db_objs)
		return QGraphicsScene::itemsBoundingRect();
	else
	{
		QRectF rect=QGraphicsScene::itemsBoundingRect();
		QList<QGraphicsItem *> items= (selected_only ? this->selectedItems() : this->items());
		double x=rect.width(), y=rect.height(), x2 = -10000, y2 = -10000;
		BaseObjectView *obj_view=nullptr;
		QPointF pnt;
		BaseGraphicObject *graph_obj=nullptr;

		for(auto &item : items)
		{
			obj_view=dynamic_cast<BaseObjectView *>(item);

			if(obj_view && obj_view->isVisible())
			{
				graph_obj=dynamic_cast<BaseGraphicObject *>(obj_view->getUnderlyingObject());

				if(graph_obj)
				{
					if(graph_obj->getObjectType()!=ObjectType::Relationship &&
							graph_obj->getObjectType()!=ObjectType::BaseRelationship)
						pnt=graph_obj->getPosition();
					else
						pnt=dynamic_cast<RelationshipView *>(obj_view)->__boundingRect().topLeft();

					if(pnt.x() < x)
						x=pnt.x();

					if(pnt.y() < y)
						y=pnt.y();

					if(selected_only)
					{
						if(graph_obj->getObjectType()!=ObjectType::Relationship &&
							 graph_obj->getObjectType()!=ObjectType::BaseRelationship)
							pnt = pnt + dynamic_cast<BaseObjectView *>(obj_view)->boundingRect().bottomRight();
						else
							pnt = pnt +  dynamic_cast<RelationshipView *>(obj_view)->__boundingRect().bottomRight();

						if(pnt.x() > x2)
							x2 = pnt.x();

						if(pnt.y() > y2)
							y2 = pnt.y();
					}
				}
			}
		}

		if(selected_only)
			return QRectF(QPointF(x, y), QPointF(x2, y2));
		else
			return QRectF(QPointF(x, y), rect.bottomRight());
	}
}

void ObjectsScene::setGridSize(unsigned size)
{
	if(size >= 20 || grid.style()==Qt::NoBrush)
	{
		QImage grid_img;
		double width, height, x, y;
		int img_w, img_h;
		QSizeF aux_size;
		QPrinter printer;
		QPainter painter;
		QPen pen;

		configurePrinter(&printer);
		aux_size=printer.paperSize(QPrinter::Point);
		aux_size-=page_margins.size();

		//Calculates where the extreme width and height where delimiter lines will be drawn
		width=aux_size.width()/static_cast<double>(size) * size;
		height=aux_size.height()/static_cast<double>(size) * size;

		//Calculates the grid pixmpa size
		img_w=ceil(width/size) * size;
		img_h=ceil(height/size) * size;

		grid_size=size;
		grid_img=QImage(img_w, img_h, QImage::Format_ARGB32);
		grid_img.fill(Qt::white);
		painter.begin(&grid_img);

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
			pen.setWidthF(1.0);
			painter.setPen(pen);
			painter.drawLine(width-1, 0,width-1,img_h-1);
			painter.drawLine(0, height-1,img_w-1,height-1);
		}

		painter.end();
		grid.setTextureImage(grid_img);
	}
}

void ObjectsScene::showRelationshipLine(bool value, const QPointF &p_start)
{
	QList<QGraphicsItem *> items=this->items();
	QGraphicsItem::GraphicsItemFlags flags;
	BaseObjectView *object=nullptr;
	TableObjectView *tab_obj_view=nullptr;
	BaseGraphicObject *base_obj=nullptr;

	if(!std::isnan(p_start.x()) && !std::isnan(p_start.y()))
		rel_line->setLine(QLineF(p_start,p_start));

	rel_line->setVisible(value);

	while(!items.isEmpty())
	{
		//When showing the relationship line all the objects cannot be moved
		flags=QGraphicsItem::ItemIsSelectable |
					QGraphicsItem::ItemSendsGeometryChanges;

		object = dynamic_cast<BaseObjectView *>(items.front());
		tab_obj_view = dynamic_cast<TableObjectView *>(object);

		// Discarding table objects views from checking since they can't be normally be selected by a single click
		if(object && !tab_obj_view && object->getUnderlyingObject())
		{
			BaseObject *ul_object = object->getUnderlyingObject();
			base_obj=dynamic_cast<BaseGraphicObject *>(ul_object);

			if(!value && base_obj &&
					base_obj->getObjectType()!=ObjectType::Relationship &&
					base_obj->getObjectType()!=ObjectType::BaseRelationship &&
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

bool ObjectsScene::isAlignObjectsToGrid()
{
	return align_objs_grid;
}

bool ObjectsScene::isShowGrid()
{
	return show_grid;
}

bool ObjectsScene::isShowPageDelimiters()
{
	return show_page_delim;
}

void ObjectsScene::setPaperConfiguration(QPrinter::PaperSize paper_sz, QPrinter::Orientation orient, QRectF margins, QSizeF custom_size)
{
	ObjectsScene::paper_size=paper_sz;
	ObjectsScene::page_orientation=orient;
	ObjectsScene::page_margins=margins;
	ObjectsScene::custom_paper_size=custom_size;
}

void ObjectsScene::getPaperConfiguration(QPrinter::PaperSize &paper_sz, QPrinter::Orientation &orient, QRectF &margins, QSizeF &custom_size)
{
	paper_sz=ObjectsScene::paper_size;
	orient=ObjectsScene::page_orientation;
	margins=ObjectsScene::page_margins;
	custom_size=ObjectsScene::custom_paper_size;
}

void ObjectsScene::configurePrinter(QPrinter *printer)
{
	if(!printer)
		throw Exception(ErrorCode::OprNotAllocatedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(paper_size!=QPrinter::Custom)
		printer->setPaperSize(paper_size);
	else
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
		//The QTBUG-33645 is fixed on Qt 5.3
		QPageLayout pl;
		QPageSize ps;
		ps=QPageSize(QSizeF(custom_paper_size.width(), custom_paper_size.height()), QPageSize::Point, QString(), QPageSize::ExactMatch);
		pl.setPageSize(ps);
		pl.setOrientation(page_orientation==QPrinter::Landscape ? QPageLayout::Landscape : QPageLayout::Portrait);
		printer->setPageSize(pl.pageSize());
#else
#warning "Custom page size bug (QTBUG-33645) workaround for Qt 5.2 or lower. NOTE: This issue is fixed on Qt 5.3"
		printer->setPaperSize(QSizeF(custom_paper_size.height(), custom_paper_size.width()), QPrinter::Point);
#endif
	}

	if(paper_size==QPrinter::Custom)
	{
		if(custom_paper_size.width() > custom_paper_size.height())
			ObjectsScene::page_orientation=QPrinter::Landscape;
		else
			ObjectsScene::page_orientation=QPrinter::Portrait;
	}
	else
		printer->setOrientation(page_orientation);

	printer->setPageMargins(page_margins.left(), page_margins.top(), page_margins.width(), page_margins.height(), QPrinter::Millimeter);
}

void ObjectsScene::configurePrinter(QPrinter *printer, const QSizeF &custom_size, QPrinter::Orientation orient)
{
	QPrinter::PaperSize orig_page_sz=paper_size;
	QPrinter::Orientation orig_orient=page_orientation;
	QSizeF orig_custom_sz=custom_paper_size;

	paper_size=QPrinter::Custom;
	page_orientation=orient;
	custom_paper_size=custom_size;

	configurePrinter(printer);

	paper_size=orig_page_sz;
	page_orientation=orig_orient;
	custom_paper_size=orig_custom_sz;
}

void ObjectsScene::handlePopupMenuRequested(TableObject *child_obj)
{
	emit s_popupMenuRequested(child_obj);
}

void ObjectsScene::handleObjectSelection(BaseGraphicObject *object, bool selected)
{
	if(object)
		emit s_objectSelected(object, selected);
}

void ObjectsScene::handleChildrenSelectionChanged()
{
	BaseTableView *tab_view = dynamic_cast<BaseTableView *>(sender());

	if(!tab_view)
		return;

	if(tab_view->getSelectedChidren().empty())
		tabs_sel_children.removeAll(tab_view);
	else if(!tabs_sel_children.contains(tab_view))
		tabs_sel_children.append(tab_view);

	emit s_childrenSelectionChanged();
}

void ObjectsScene::addItem(QGraphicsItem *item)
{
	if(item)
	{
		RelationshipView *rel=dynamic_cast<RelationshipView *>(item);
		BaseTableView *tab=dynamic_cast<BaseTableView *>(item);
		BaseObjectView *obj=dynamic_cast<BaseObjectView *>(item);

		if(rel)
			connect(rel, SIGNAL(s_relationshipModified(BaseGraphicObject*)), this, SIGNAL(s_objectModified(BaseGraphicObject*)));
		else if(tab)
		{
			connect(tab, SIGNAL(s_popupMenuRequested(TableObject*)), this, SLOT(handlePopupMenuRequested(TableObject*)));
			connect(tab, SIGNAL(s_childrenSelectionChanged()), this, SLOT(handleChildrenSelectionChanged()));
			connect(tab, SIGNAL(s_collapseModeChanged()), this, SIGNAL(s_collapseModeChanged()));
			connect(tab, SIGNAL(s_paginationToggled()), this, SIGNAL(s_paginationToggled()));
			connect(tab, SIGNAL(s_currentPageChanged()), this, SIGNAL(s_currentPageChanged()));
			connect(tab, SIGNAL(s_sceneClearRequested()), this, SLOT(clearSelection()));
		}

		if(obj)
		{		
			obj->setVisible(isLayerActive(obj->getLayer()));

			// Relationships and schemas don't have their z value changed
			if(!rel && !dynamic_cast<SchemaView *>(item))
				obj->setZValue(dynamic_cast<BaseGraphicObject *>(obj->getUnderlyingObject())->getZValue());

			connect(obj, SIGNAL(s_objectSelected(BaseGraphicObject*,bool)), this, SLOT(handleObjectSelection(BaseGraphicObject*,bool)));
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

		if(rel)
			rel->disconnectTables();

		item->setVisible(false);
		item->setActive(false);
		QGraphicsScene::removeItem(item);

		if(object)
		{
			disconnect(object, nullptr, this, nullptr);
			disconnect(object, nullptr, dynamic_cast<BaseGraphicObject*>(object->getUnderlyingObject()), nullptr);
			disconnect(dynamic_cast<BaseGraphicObject*>(object->getUnderlyingObject()), nullptr, object, nullptr);
			removed_objs.push_back(object);
		}
	}
}

void ObjectsScene::blockItemsSignals(bool block)
{
	BaseObjectView *obj_view = nullptr;

	for(auto &item : this->items())
	{
		obj_view = dynamic_cast<BaseObjectView *>(item);
		if(obj_view)
			obj_view->blockSignals(block);
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
			emit s_objectDoubleClicked(dynamic_cast<BaseGraphicObject *>(obj->getUnderlyingObject()));
	}
	else
		//Emit a signal indicating that no object was selected
		emit s_objectDoubleClicked(nullptr);
}

void ObjectsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//Gets the item at mouse position
	QGraphicsItem* item=this->itemAt(event->scenePos(), QTransform());
	bool is_deselection = !this->selectedItems().isEmpty() && !item;

	if(selectedItems().empty())
		emit s_objectsScenePressed(event->buttons());

	/* If the relationship line is visible, indicates that the user is in the middle of
	 a relationship creation, thus is needed to inform to the scene to activate the
	 the multiselection to be able to select two tables and link them. By default,
	 the multiselection modifier is the Control key */
	if(rel_line->isVisible())
		event->setModifiers(Qt::ControlModifier);

	if(is_deselection)
		this->blockItemsSignals(true);

	/* If we're handling a deselection of the user selected another object whitout being holding Control
	 * we need to deselect the tables' children objects too */
	if(is_deselection || (event->buttons()==Qt::LeftButton && (event->modifiers() & Qt::ControlModifier) != Qt::ControlModifier))
		//Forcing the clear on all selected table children object
		clearTablesChildrenSelection();

	QGraphicsScene::mousePressEvent(event);

	if(is_deselection)
	{
		this->blockItemsSignals(false);
		emit s_objectSelected(nullptr, false);
	}

	if(event->buttons()==Qt::LeftButton)
	{
		sel_ini_pnt=event->scenePos();

		if((!invert_rangesel_trigger && event->modifiers()==Qt::ShiftModifier) ||
				(invert_rangesel_trigger && event->modifiers()==Qt::NoModifier))
		{
			if(enable_range_sel && this->selectedItems().isEmpty())
			{
				selection_rect->setVisible(true);
				emit s_objectSelected(nullptr,false);
			}
		}
		else
		{
			//Selects the object (without press control) if the user is creating a relationship
			if(item && item->isEnabled() && !item->isSelected() &&  rel_line->isVisible())
				item->setSelected(true);
		}
	}
	else if(event->buttons()==Qt::RightButton)
	{
		//Case there is no item at the mouse position clears the selection on the scene
		if(!item)
		{
			this->clearSelection();
			emit s_objectSelected(nullptr,false);
		}

		emit s_popupMenuRequested();
	}
}

bool ObjectsScene::mouseIsAtCorner()
{
	QGraphicsView *view=getActiveViewport();

	if(view)
	{
		QPoint pos=view->mapFromGlobal(QCursor::pos());
		QRect rect=view->rect();

		if(rect.contains(pos))
		{
			if(pos.x() <= SceneMoveThreshold)
				scene_move_dx=-SceneMoveStep;
			else if(pos.x() >= (view->width() - view->verticalScrollBar()->width() - SceneMoveThreshold))
				scene_move_dx=SceneMoveStep;
			else
				scene_move_dx=0;

			if(pos.y() <= SceneMoveThreshold)
				scene_move_dy=-SceneMoveStep;
			else if(pos.y() >= (view->height() - view->horizontalScrollBar()->height() - SceneMoveThreshold))
				scene_move_dy=SceneMoveStep;
			else
				scene_move_dy=0;

			return scene_move_dx!=0 || scene_move_dy!=0;
		}
		else
			return false;
	}
	else
		return false;
}

QGraphicsView *ObjectsScene::getActiveViewport()
{
	QGraphicsView *view_p=nullptr;

	for(auto &view : this->views())
	{
		if(view && view->isActiveWindow())
		{
			view_p=view;
			break;
		}
	}

	return view_p;
}

void ObjectsScene::moveObjectScene()
{
	if(scene_move_dx!=0 || scene_move_dy!=0)
	{
		QGraphicsView *view=getActiveViewport();

		if(view && mouseIsAtCorner())
		{
			view->horizontalScrollBar()->setValue(view->horizontalScrollBar()->value() + scene_move_dx);
			view->verticalScrollBar()->setValue(view->verticalScrollBar()->value() + scene_move_dy);
			move_scene=true;
		}
		else
		{
			move_scene=false;
			scene_move_timer.stop();
		}
	}
}

void ObjectsScene::enableSceneMove(bool value)
{
	if(value)
	{
		scene_move_timer.start();
		corner_hover_timer.stop();
	}
	else
	{
		corner_hover_timer.stop();
		scene_move_timer.stop();
	}

	move_scene=value;
}

void ObjectsScene::enableRangeSelection(bool value)
{
	enable_range_sel=value;

	if(!value && selection_rect->isVisible())
		selection_rect->setVisible(value);
}

void ObjectsScene::adjustScenePositionOnKeyEvent(int key)
{
	QGraphicsView *view = getActiveViewport();

	if(view)
	{
		QRectF brect = itemsBoundingRect(true, true);
		QRectF view_rect = QRectF(view->mapToScene(view->rect().topLeft()),
															view->mapToScene(view->rect().bottomRight())),
				scene_rect = sceneRect();

		if(view_rect.right() < brect.right() && key == Qt::Key_Right)
		{
			/* If the objects are being moved right and the scene width is lesser than the items bounding rect's width
			we need to resize the scene prior the position adjustment */
			scene_rect.setRight(brect.right());
			setSceneRect(scene_rect);
			view->horizontalScrollBar()->setValue(view->horizontalScrollBar()->value() + ((brect.right() - view_rect.right()) * 2));
		}
		else if(view_rect.left() > brect.left()  && key == Qt::Key_Left)
			view->horizontalScrollBar()->setValue(view->horizontalScrollBar()->value() - ((view_rect.left() - brect.left()) * 2));

		if(view_rect.bottom() < brect.bottom() && key == Qt::Key_Down)
		{
			/* If the objects are being moved down and the scene hight is lesser than the items bounding rect's height
			we need to resize the scene prior the position adjustment */
			scene_rect.setBottom(brect.bottom());
			setSceneRect(scene_rect);
			view->verticalScrollBar()->setValue(view->verticalScrollBar()->value() + ((brect.bottom() - view_rect.bottom()) * 2));
		}
		else if(view_rect.top() > brect.top()  && key == Qt::Key_Up)
			view->verticalScrollBar()->setValue(view->verticalScrollBar()->value() - ((view_rect.top() - brect.top()) * 2));
	}
}

void ObjectsScene::keyPressEvent(QKeyEvent *event)
{
	if((event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
			event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) &&
		 !selectedItems().isEmpty())
	{
		double dx = 0, dy = 0;
		BaseObjectView *obj_view=nullptr;
		QRectF brect = itemsBoundingRect(true, true);

		if(!moving_objs)
		{
			sel_ini_pnt = brect.center();
			moving_objs = true;

			/* If the object move timer is not active we need to send the
			s_objectsMoved() signal in order to alert the classes like ModelWidget to
			save the current objects' position in the operation history */
			if(!object_move_timer.isActive())
				emit s_objectsMoved(false);

			for(auto item : selectedItems())
			{
				obj_view=dynamic_cast<BaseObjectView *>(item);

				if(obj_view && BaseObjectView::isPlaceholderEnabled())
					obj_view->togglePlaceholder(true);
			}
		}

		if(event->key() == Qt::Key_Up)
			dy = -1;
		else if(event->key() == Qt::Key_Down)
			dy = 1;

		if(event->key() == Qt::Key_Left)
			dx = -1;
		else if(event->key() == Qt::Key_Right)
			dx = 1;

		if(event->modifiers() == Qt::ControlModifier)
		{
			dx *= 10;
			dy *= 10;
		}
		else if(event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))
		{
			dx *= 100;
			dy *= 100;
		}

		for(auto item : selectedItems())
		{
			obj_view=dynamic_cast<BaseObjectView *>(item);

			if(obj_view && !dynamic_cast<RelationshipView *>(obj_view))
				obj_view->moveBy(dx, dy);
		}

		adjustScenePositionOnKeyEvent(event->key());
	}
	else
		QGraphicsScene::keyPressEvent(event);
}

void ObjectsScene::keyReleaseEvent(QKeyEvent *event)
{
	if((event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
			event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) &&
		 !event->isAutoRepeat() && !selectedItems().isEmpty())
	{
		if(moving_objs)
		{
			object_move_timer.start();
			adjustScenePositionOnKeyEvent(event->key());
		}
	}
	else
		QGraphicsScene::keyReleaseEvent(event);
}

void ObjectsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->buttons()==Qt::LeftButton || rel_line->isVisible())
	{
		if(corner_move)
		{
			if(mouseIsAtCorner())
			{
				if(move_scene)
					scene_move_timer.start();
				else
					corner_hover_timer.start();
			}
			else
				enableSceneMove(false);
		}

		if(!rel_line->isVisible())
		{
			int sel_items_count = this->selectedItems().size();

			//Case the user starts a object moviment
			if(sel_items_count != 0 && !moving_objs)
			{
				if(BaseObjectView::isPlaceholderEnabled())
				{
					QList<QGraphicsItem *> items=this->selectedItems();
					BaseObjectView *obj_view=nullptr;

					for(QGraphicsItem *item : items)
					{
						obj_view=dynamic_cast<BaseObjectView *>(item);
						obj_view->togglePlaceholder(true);
					}
				}

				emit s_objectsMoved(false);
				moving_objs=true;
			}

			//If the alignment to grid is active, adjust the event scene position
			if(align_objs_grid && !selection_rect->isVisible() && sel_items_count <= 1)
				event->setScenePos(this->alignPointToGrid(event->scenePos()));
			else if(selection_rect->isVisible())
			{
				QPolygonF pol;
				pol.append(sel_ini_pnt);
				pol.append(QPointF(event->scenePos().x(), sel_ini_pnt.y()));
				pol.append(QPointF(event->scenePos().x(), event->scenePos().y()));
				pol.append(QPointF(sel_ini_pnt.x(), event->scenePos().y()));
				selection_rect->setPolygon(pol);
				selection_rect->setBrush(BaseObjectView::getFillStyle(Attributes::ObjSelection));
				selection_rect->setPen(BaseObjectView::getBorderStyle(Attributes::ObjSelection));
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

	if(event->button()==Qt::LeftButton && corner_move)
		enableSceneMove(false);

	//If there is selected object and the user ends the object moviment
	if(!this->selectedItems().isEmpty() && moving_objs && event->button()==Qt::LeftButton/* && event->modifiers()==Qt::NoModifier */)
	{
		finishObjectsMove(event->scenePos());
	}
	else if(selection_rect->isVisible() && event->button()==Qt::LeftButton)
	{
		QPolygonF pol;
		QPainterPath sel_area;

		sel_area.addRect(selection_rect->polygon().boundingRect());

		this->blockItemsSignals(true);
		this->setSelectionArea(sel_area, Qt::IntersectsItemShape);
		this->blockItemsSignals(false);

		selection_rect->setVisible(false);
		selection_rect->setPolygon(pol);
		sel_ini_pnt.setX(DNaN);
		sel_ini_pnt.setY(DNaN);

		if(!this->selectedItems().isEmpty())
			emit s_objectsSelectedInRange();
	}
}

void ObjectsScene::finishObjectsMove(const QPointF &pnt_end)
{
	QList<QGraphicsItem *> items=this->selectedItems(), rel_list;
	double x1,y1,x2,y2, dx, dy;
	QRectF rect;
	SchemaView *sch_view=nullptr;
	vector<QPointF> points;
	vector<QPointF>::iterator itr;
	vector<BaseObject *> rels, base_rels;
	QSet<Schema *> schemas;
	BaseRelationship *base_rel=nullptr;
	RelationshipView *rel=nullptr;
	BaseObjectView *obj_view=nullptr;
	BaseTableView *tab_view=nullptr;
	TableObjectView *tab_obj_view=nullptr;
	QSet<BaseObjectView *> tables;

	//Gathering the relationships inside the selected schemsa in order to move their points too
	for(auto &item : items)
	{
		obj_view=dynamic_cast<BaseObjectView *>(item);
		sch_view=dynamic_cast<SchemaView *>(item);
		tab_view=dynamic_cast<BaseTableView *>(item);
		tab_obj_view=dynamic_cast<TableObjectView *>(item);

		// Ignoring table objects items
		if(tab_obj_view)
			continue;

		if(obj_view)
			obj_view->togglePlaceholder(false);

		if(tab_view)
			tables.insert(tab_view);
		else if(sch_view)
		{
			//Get the schema object
			Schema *schema=dynamic_cast<Schema *>(sch_view->getUnderlyingObject());

			if(!schema->isProtected())
			{
				//Get the table-table and table-view relationships
				rels=dynamic_cast<DatabaseModel *>(schema->getDatabase())->getObjects(ObjectType::Relationship, schema);
				base_rels=dynamic_cast<DatabaseModel *>(schema->getDatabase())->getObjects(ObjectType::BaseRelationship, schema);
				rels.insert(rels.end(), base_rels.begin(), base_rels.end());

				for(auto &rel : rels)
				{
					base_rel=dynamic_cast<BaseRelationship *>(rel);

					/* If the relationship contains points and it is not selected then it will be included on the list
						 in order to move their custom line points */
					if(!dynamic_cast<RelationshipView *>(base_rel->getOverlyingObject())->isSelected() &&
							!base_rel->getPoints().empty())
						rel_list.push_back(dynamic_cast<QGraphicsItem *>(base_rel->getOverlyingObject()));
				}

				#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
					tables.unite(sch_view->getChildren().toSet());
				#else
					QList<BaseObjectView *> list = sch_view->getChildren();
					tables.unite(QSet<BaseObjectView *>(list.begin(), list.end()));
				#endif
			}
		}
	}

	items.append(rel_list);

	/* Get the extreme points of the scene to check if some objects are out the area
	forcing the scene to be resized */
	x1=this->sceneRect().left();
	y1=this->sceneRect().top();
	x2=this->sceneRect().right();
	y2=this->sceneRect().bottom();
	dx=pnt_end.x() - sel_ini_pnt.x();
	dy=pnt_end.y() - sel_ini_pnt.y();

	for(auto &item : items)
	{
		// Ignoring table objects items
		tab_obj_view=dynamic_cast<TableObjectView *>(item);
		if(tab_obj_view) continue;

		rel=dynamic_cast<RelationshipView *>(item);

		if(!rel)
		{
			if(align_objs_grid)
				item->setPos(alignPointToGrid(item->pos()));
			else
			{
				QPointF p=item->pos();
				if(p.x() < 0) p.setX(0);
				if(p.y() < 0) p.setY(0);
				item->setPos(p);
			}

			rect.setTopLeft(item->pos());
			rect.setSize(item->boundingRect().size());
		}
		else
		{
			/* If the relationship has points added to the line is necessary to move the points
				 too. Since relationships cannot be moved naturally (by user) this will be done
				 by the scene. NOTE: this operation is done ONLY WHEN there is more than one object selected! */
			points=rel->getUnderlyingObject()->getPoints();
			if(items.size() > 1 && !points.empty())
			{
				itr=points.begin();
				while(itr!=points.end())
				{
					//Translate the points
					itr->setX(itr->x() + dx);
					itr->setY(itr->y() + dy);

					//Align to grid if the flag is set
					if(align_objs_grid)
						(*itr)=alignPointToGrid(*itr);

					itr++;
				}

				//Assing the new points to relationship and reconfigure its line
				rel->getUnderlyingObject()->setPoints(points);
				rel->configureLine();
			}

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
		rect.setWidth(rect.width() * 1.05);
		rect.setHeight(rect.height() * 1.05);
		this->setSceneRect(rect);
	}

	for(auto &obj : tables)
	{
		tab_view=dynamic_cast<BaseTableView *>(obj);

		//Realign tables if the parent schema had the position adjusted too
		if(align_objs_grid)
		{
			tab_view->setPos(alignPointToGrid(tab_view->pos()));
			schemas.insert(dynamic_cast<Schema *>(tab_view->getUnderlyingObject()->getSchema()));
		}

		if(BaseObjectView::isPlaceholderEnabled())
			tab_view->requestRelationshipsUpdate();
	}

	//Updating schemas bounding rects after moving objects
	for(auto &obj : schemas)
		obj->setModified(true);

	emit s_objectsMoved(true);
	moving_objs=false;
	sel_ini_pnt.setX(DNaN);
	sel_ini_pnt.setY(DNaN);
}

void ObjectsScene::alignObjectsToGrid()
{
	QList<QGraphicsItem *> items=this->items();
	RelationshipView *rel=nullptr;
	BaseTableView *tab=nullptr;
	TextboxView *lab=nullptr;
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
				points=rel->getUnderlyingObject()->getPoints();
				count1=points.size();
				for(i1=0; i1 < count1; i1++)
					points[i1]=this->alignPointToGrid(points[i1]);

				if(count1 > 0)
				{
					rel->getUnderlyingObject()->setPoints(points);
					rel->configureLine();
				}

				//Align the labels
				for(i1=BaseRelationship::SrcCardLabel;
					i1<=BaseRelationship::RelNameLabel; i1++)
				{
					lab=rel->getLabel(i1);
					if(lab)
						lab->setPos(this->alignPointToGrid(lab->pos()));
				}
			}
			else if(!dynamic_cast<SchemaView *>(items[i]))
				items[i]->setPos(this->alignPointToGrid(items[i]->pos()));
			else
				schemas.push_back(dynamic_cast<Schema *>(dynamic_cast<BaseObjectView *>(items[i])->getUnderlyingObject()));
		}
	}

	//Updating schemas dimensions
	while(!schemas.empty())
	{
		schemas.back()->setModified(true);
		schemas.pop_back();
	}
}

void ObjectsScene::update()
{
	this->setBackgroundBrush(grid);
	QGraphicsScene::update(this->sceneRect());
}

void ObjectsScene::clearTablesChildrenSelection()
{
	for(auto &tab_obj_view : tabs_sel_children)
		tab_obj_view->clearChildrenSelection();

	tabs_sel_children.clear();
}

void ObjectsScene::clearSelection()
{
	clearTablesChildrenSelection();
	QGraphicsScene::clearSelection();
}

vector<QRectF> ObjectsScene::getPagesForPrinting(const QSizeF &paper_size, const QSizeF &margin, unsigned &h_page_cnt, unsigned &v_page_cnt)
{
	vector<QRectF> pages;
	QRectF page_rect, max_rect;
	double width, height, page_width, page_height;
	unsigned h_page=0, v_page=0, start_h=99999, start_v=99999;
	QList<QGraphicsItem *> list;

	page_width=ceil(paper_size.width() - margin.width()-1);
	page_height=ceil(paper_size.height() - margin.height()-1);

	//Calculates the horizontal and vertical page count based upon the passed paper size
	h_page_cnt=round(this->sceneRect().width()/page_width) + 1;
	v_page_cnt=round(this->sceneRect().height()/page_height) + 1;

	//Calculates the maximum count of horizontal and vertical pages
	for(v_page=0; v_page < v_page_cnt; v_page++)
	{
		for(h_page=0; h_page < h_page_cnt; h_page++)
		{
			//Calculates the current page rectangle
			page_rect=QRectF(QPointF(h_page * page_width, v_page * page_height), QSizeF(page_width, page_height));

			//Case there is selected items recalculates the maximum page size
			list=this->items(page_rect, Qt::IntersectsItemShape);
			if(!list.isEmpty())
			{
				if(start_h > h_page) start_h=h_page;
				if(start_v > v_page) start_v=v_page;

				width=page_rect.left() + page_rect.width();
				height=page_rect.top() + page_rect.height();

				if(width > max_rect.width())
					max_rect.setWidth(width);

				if(height > max_rect.height())
					max_rect.setHeight(height);
			}
		}
	}

	//Re calculates the maximum page count based upon the maximum page size
	h_page_cnt=round(max_rect.width()/page_width);
	v_page_cnt=round(max_rect.height()/page_height);

	//Inserts the page rectangles on the list
	for(v_page=static_cast<unsigned>(start_v); v_page < v_page_cnt; v_page++)
		for(h_page=static_cast<unsigned>(start_h); h_page < h_page_cnt; h_page++)
			pages.push_back(QRectF(QPointF(h_page * page_width, v_page * page_height), QSizeF(page_width, page_height)));

	return pages;
}

bool ObjectsScene::isRangeSelectionEnabled()
{
	return enable_range_sel;
}

bool ObjectsScene::isRangeSelectionTriggerInverted()
{
	return invert_rangesel_trigger;
}

bool ObjectsScene::isRelationshipLineVisible()
{
	return rel_line->isVisible();
}

bool ObjectsScene::isMovingObjects()
{
	return moving_objs;
}

QList<QGraphicsItem *> ObjectsScene::selectedItems() const
{
	if(tabs_sel_children.empty())
		return QGraphicsScene::selectedItems();

	QList<QGraphicsItem *> items = QGraphicsScene::selectedItems();

	for(auto &tab_view :tabs_sel_children)
	{
		for(auto &tab_obj : tab_view->getSelectedChidren())
			items.append(tab_obj);
	}

	return items;
}

bool ObjectsScene::hasOnlyTableChildrenSelection() const
{
	return QGraphicsScene::selectedItems().isEmpty() && !tabs_sel_children.isEmpty();
}
