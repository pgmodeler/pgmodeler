/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
bool ObjectsScene::invert_panning_rangesel=false;

ObjectsScene::ObjectsScene(void)
{
  moving_objs=move_scene=false;
  enable_range_sel=true;
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

  scene_move_dx=scene_move_dy=0;

  connect(&scene_move_timer, SIGNAL(timeout()), this, SLOT(moveObjectScene()));
  connect(&corner_hover_timer, SIGNAL(timeout()), this, SLOT(enableSceneMove()));

  scene_move_timer.setInterval(SCENE_MOVE_TIMEOUT);
  corner_hover_timer.setInterval(SCENE_MOVE_TIMEOUT * 10);
}

ObjectsScene::~ObjectsScene(void)
{
	QGraphicsItemGroup *item=nullptr;
  QList<QGraphicsItem *> items;
	ObjectType obj_types[]={ OBJ_RELATIONSHIP, OBJ_TEXTBOX,
													 OBJ_VIEW, OBJ_TABLE, OBJ_SCHEMA };
	unsigned i, count=sizeof(obj_types)/sizeof(ObjectType);

	this->removeItem(selection_rect);
	this->removeItem(rel_line);

	//Destroy the objects in the order defined on obj_types vector
	for(i=0; i < count; i++)
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
          (dynamic_cast<StyledTextboxView *>(item) && obj_types[i]==OBJ_TEXTBOX) ||
					(dynamic_cast<GraphicalView *>(item) && obj_types[i]==OBJ_VIEW) ||
					(dynamic_cast<TableView *>(item) && obj_types[i]==OBJ_TABLE) ||
					(dynamic_cast<SchemaView *>(item) && obj_types[i]==OBJ_SCHEMA)))

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
    delete(removed_objs.back());
    removed_objs.pop_back();
  }
}

void ObjectsScene::setEnableCornerMove(bool enable)
{
  ObjectsScene::corner_move=enable;
}

void ObjectsScene::setInvertPanningRangeSelection(bool invert)
{
  ObjectsScene::invert_panning_rangesel=invert;
}

bool ObjectsScene::isCornerMoveEnabled(void)
{
  return(ObjectsScene::corner_move);
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

QRectF ObjectsScene::itemsBoundingRect(bool seek_only_db_objs)
{
  if(!seek_only_db_objs)
    return(QGraphicsScene::itemsBoundingRect());
  else
  {
    QRectF rect=QGraphicsScene::itemsBoundingRect();
    QList<QGraphicsItem *> items=this->items();
    double x=rect.width(), y=rect.height();
    BaseObjectView *obj_view=nullptr;
    QPointF pnt;
    BaseGraphicObject *graph_obj=nullptr;

    for(auto &item : items)
    {
      obj_view=dynamic_cast<BaseObjectView *>(item);

      if(obj_view && obj_view->isVisible())
      {
        graph_obj=dynamic_cast<BaseGraphicObject *>(obj_view->getSourceObject());

        if(graph_obj)
        {
          if(graph_obj->getObjectType()!=OBJ_RELATIONSHIP &&
             graph_obj->getObjectType()!=BASE_RELATIONSHIP)
            pnt=graph_obj->getPosition();
          else
            pnt=dynamic_cast<RelationshipView *>(obj_view)->__boundingRect().topLeft();

          if(pnt.x() < x)
            x=pnt.x();

          if(pnt.y() < y)
            y=pnt.y();
        }
      }
    }

    return(QRectF(QPointF(x, y), rect.bottomRight()));
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
		img_w=ceil(width/size)*size;
		img_h=ceil(height/size)*size;

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
			pen.setWidthF(1.0f);
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
	BaseGraphicObject *base_obj=nullptr;

	if(!std::isnan(p_start.x()) && !std::isnan(p_start.y()))
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
    throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

    if(rel)
      rel->disconnectTables();

		item->setVisible(false);
		item->setActive(false);
    QGraphicsScene::removeItem(item);

    if(object)
    {
      disconnect(object, nullptr, this, nullptr);
      disconnect(object, nullptr, dynamic_cast<BaseGraphicObject*>(object->getSourceObject()), nullptr);
      disconnect(dynamic_cast<BaseGraphicObject*>(object->getSourceObject()), nullptr, object, nullptr);
      removed_objs.push_back(object);
      //delete(item);
    }
	}
}

void ObjectsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsScene::mouseDoubleClickEvent(event);
  enablePannigMode(false);

  if(this->selectedItems().size()==1 && event->buttons()==Qt::LeftButton && !rel_line->isVisible())
  {
    //Gets the selected graphical object
    BaseObjectView *obj=dynamic_cast<BaseObjectView *>(this->selectedItems().at(0));

    if(obj)
      emit s_objectDoubleClicked(dynamic_cast<BaseGraphicObject *>(obj->getSourceObject()));
  }
  else
    //Emit a signal indicating that no object was selected
    emit s_objectDoubleClicked(nullptr);
}

void ObjectsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsView *view=getActiveViewport();

  //Gets the item at mouse position
  QGraphicsItem* item=this->itemAt(event->scenePos().x(), event->scenePos().y(), QTransform());

  if(selectedItems().empty())
    emit s_objectsScenePressed(event->buttons());

  /* If the relationship line is visible, indicates that the user is in the middle of
     a relationship creation, thus is needed to inform to the scene to activate the
     the multiselection to be able to select two tables and link them. By default,
     the multiselection modifier is the Control key */
  if(rel_line->isVisible())
    event->setModifiers(Qt::ControlModifier);

  QGraphicsScene::mousePressEvent(event);

  if(event->buttons()==Qt::LeftButton)
  {
		sel_ini_pnt=event->scenePos();

    if((!invert_panning_rangesel && event->modifiers()==Qt::ShiftModifier) ||
       (invert_panning_rangesel && event->modifiers()==Qt::NoModifier))
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
      /* Workaround to avoid the panning mode to be activated when user is only adding a
         graphical object (table / textbox / relationship / view) */
      else if(((invert_panning_rangesel && event->modifiers()==Qt::ShiftModifier) || !invert_panning_rangesel)  &&
              view && view->cursor().shape()==Qt::ArrowCursor)
        enablePannigMode(true);
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

bool ObjectsScene::mouseIsAtCorner(void)
{
  QGraphicsView *view=getActiveViewport();

  if(view)
  {

    QPoint pos=view->mapFromGlobal(QCursor::pos());
    QRect rect=view->rect();

    if(rect.contains(pos))
    {
      if(pos.x() <= SCENE_MOVE_THRESHOLD)
        scene_move_dx=-SCENE_MOVE_STEP;
      else if(pos.x() >= (view->width() - view->verticalScrollBar()->width() - SCENE_MOVE_THRESHOLD))
        scene_move_dx=SCENE_MOVE_STEP;
      else
        scene_move_dx=0;

      if(pos.y() <= SCENE_MOVE_THRESHOLD)
        scene_move_dy=-SCENE_MOVE_STEP;
      else if(pos.y() >= (view->height() - view->horizontalScrollBar()->height() - SCENE_MOVE_THRESHOLD))
        scene_move_dy=SCENE_MOVE_STEP;
      else
        scene_move_dy=0;

      return(scene_move_dx!=0 || scene_move_dy!=0);
    }
    else
      return(false);
  }
  else
    return(false);
}

QGraphicsView *ObjectsScene::getActiveViewport(void)
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

  return(view_p);
}

void ObjectsScene::moveObjectScene(void)
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

void ObjectsScene::enablePannigMode(bool value)
{
  QGraphicsView *view=getActiveViewport();

  if(view)
   view->setDragMode((value ? QGraphicsView::ScrollHandDrag : QGraphicsView::NoDrag));
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

  if(event->button()==Qt::LeftButton)
  {
    enablePannigMode(false);

    if(corner_move)
      enableSceneMove(false);
  }

  //If there is selected object and the user ends the object moviment
  if(!this->selectedItems().isEmpty() && moving_objs &&
     event->button()==Qt::LeftButton && event->modifiers()==Qt::NoModifier)
  {
    unsigned i, count;
    QList<QGraphicsItem *> items=this->selectedItems(), rel_list;
    double x1,y1,x2,y2, dx, dy;
    QRectF rect;
    SchemaView *sch_view=nullptr;
    vector<QPointF> points;
    vector<QPointF>::iterator itr;
    vector<BaseObject *> rels, base_rels;
    BaseRelationship *base_rel=nullptr;
    RelationshipView *rel=nullptr;

    //Gathering the relationships inside the selected schemsa in order to move their points too
    for(auto &item : items)
    {
      sch_view=dynamic_cast<SchemaView *>(item);

      if(sch_view)
      {
        //Get the schema object
        Schema *schema=dynamic_cast<Schema *>(sch_view->getSourceObject());

        if(!schema->isProtected())
        {
          //Get the table-table and table-view relationships
          rels=dynamic_cast<DatabaseModel *>(schema->getDatabase())->getObjects(OBJ_RELATIONSHIP, schema);
          base_rels=dynamic_cast<DatabaseModel *>(schema->getDatabase())->getObjects(BASE_RELATIONSHIP, schema);
          rels.insert(rels.end(), base_rels.begin(), base_rels.end());

          for(auto &rel : rels)
          {
            base_rel=dynamic_cast<BaseRelationship *>(rel);

            /* If the relationship contains points and it is not selected then it will be included on the list
               in order to move their custom line points */
            if(!dynamic_cast<RelationshipView *>(base_rel->getReceiverObject())->isSelected() &&
               !base_rel->getPoints().empty())
              rel_list.push_back(dynamic_cast<QGraphicsItem *>(base_rel->getReceiverObject()));
          }
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
    dx=event->scenePos().x() - sel_ini_pnt.x();
    dy=event->scenePos().y() - sel_ini_pnt.y();

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
        /* If the relationship has points added to the line is necessary to move the points
        too. Since relationships cannot be moved naturally (by user) this will be done
        by the scene. NOTE: this operation is done ONLY WHEN there is more than one object selected! */
        points=rel->getSourceObject()->getPoints();
        if(count > 1 && !points.empty())
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
          rel->getSourceObject()->setPoints(points);
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
      rect.setWidth(rect.width() * 1.05f);
      rect.setHeight(rect.height() * 1.05f);
      this->setSceneRect(rect);
    }

    emit s_objectsMoved(true);
    moving_objs=false;
    sel_ini_pnt.setX(NAN);
    sel_ini_pnt.setY(NAN);
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
				for(i1=BaseRelationship::SRC_CARD_LABEL;
						i1<=BaseRelationship::REL_NAME_LABEL; i1++)
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

vector<QRectF> ObjectsScene::getPagesForPrinting(const QSizeF &paper_size, const QSizeF &margin, unsigned &h_page_cnt, unsigned &v_page_cnt)
{
  vector<QRectF> pages;
  QRectF page_rect, max_rect;
  double width, height, page_width, page_height;
  unsigned h_page=0, v_page=0, start_h=99999, start_v=99999;
  QList<QGraphicsItem *> list;

  page_width=ceilf(paper_size.width() - margin.width()-1);
  page_height=ceilf(paper_size.height() - margin.height()-1);

  //Calculates the horizontal and vertical page count based upon the passed paper size
  h_page_cnt=roundf(this->sceneRect().width()/page_width) + 1;
  v_page_cnt=roundf(this->sceneRect().height()/page_height) + 1;

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
  h_page_cnt=roundf(max_rect.width()/page_width);
  v_page_cnt=roundf(max_rect.height()/page_height);

  //Inserts the page rectangles on the list
  for(v_page=static_cast<unsigned>(start_v); v_page < v_page_cnt; v_page++)
    for(h_page=static_cast<unsigned>(start_h); h_page < h_page_cnt; h_page++)
      pages.push_back(QRectF(QPointF(h_page * page_width, v_page * page_height), QSizeF(page_width, page_height)));

  return(pages);
}

bool ObjectsScene::isRangeSelectionEnabled(void)
{
  return(enable_range_sel);
}

bool ObjectsScene::isPanningRangeSelectionInverted(void)
{
	return(invert_panning_rangesel);
}

bool ObjectsScene::isRelationshipLineVisible(void)
{
	return(rel_line->isVisible());
}
