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

#include "basetableview.h"

bool BaseTableView::hide_ext_attribs = false;
bool BaseTableView::hide_tags = false;
unsigned BaseTableView::attribs_per_page[2] = { 10, 5 };

BaseTableView::BaseTableView(BaseTable *base_tab) : BaseObjectView(base_tab)
{
	if(!base_tab)
		throw Exception(ErrorCode::AsgNotAllocattedObject, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	pending_geom_update = false;
	is_ungrouped=false;
	body=new RoundedRectItem;
	body->setRoundedCorners(RoundedRectItem::BottomLeftCorner | RoundedRectItem::BottomRightCorner);

	title=new TableTitleView;
	title->setZValue(2);

	ext_attribs_body=new RoundedRectItem;
	ext_attribs_body->setRoundedCorners(RoundedRectItem::NoCorners);

	ext_attribs=new QGraphicsItemGroup;
	ext_attribs->setZValue(1);
	ext_attribs->setFlag(QGraphicsItem::ItemClipsChildrenToShape);

	columns=new QGraphicsItemGroup;
	columns->setZValue(1);
	columns->setFlag(QGraphicsItem::ItemClipsChildrenToShape);

	tag_item = new TextPolygonItem;
	tag_item->setZValue(3);

	obj_shadow=new RoundedRectItem;
	obj_shadow->setZValue(-1);

	obj_selection=new RoundedRectItem;
	obj_selection->setVisible(false);
	obj_selection->setZValue(4);

	attribs_toggler = new AttributesTogglerItem;
	attribs_toggler->setZValue(1);

	this->addToGroup(obj_selection);
	this->addToGroup(obj_shadow);
	this->addToGroup(columns);
	this->addToGroup(body);
	this->addToGroup(title);
	this->addToGroup(tag_item);
	this->addToGroup(ext_attribs);
	this->addToGroup(ext_attribs_body);
	this->addToGroup(attribs_toggler);

	this->setAcceptHoverEvents(true);
	sel_child_obj=nullptr;
	configurePlaceholder();

	connect(attribs_toggler, SIGNAL(s_collapseModeChanged(CollapseMode)), this, SLOT(configureCollapsedSections(CollapseMode)));
	connect(attribs_toggler, SIGNAL(s_paginationToggled(bool)), this, SLOT(togglePagination(bool)));
	connect(attribs_toggler, SIGNAL(s_currentPageChanged(unsigned,unsigned)), this, SLOT(configureCurrentPage(unsigned,unsigned)));
}

BaseTableView::~BaseTableView(void)
{
	this->removeFromGroup(body);
	this->removeFromGroup(title);
	this->removeFromGroup(attribs_toggler);
	this->removeFromGroup(ext_attribs_body);
	this->removeFromGroup(ext_attribs);
	this->removeFromGroup(columns);
	this->removeFromGroup(tag_item);
	delete(attribs_toggler);
	delete(ext_attribs_body);
	delete(ext_attribs);
	delete(body);
	delete(title);
	delete(columns);
	delete(tag_item);
}

void BaseTableView::setHideExtAttributes(bool value)
{
	hide_ext_attribs=value;
}

void BaseTableView::setHideTags(bool value)
{
	hide_tags=value;
}

bool BaseTableView::isExtAttributesHidden(void)
{
	return(hide_ext_attribs);
}

bool BaseTableView::isTagsHidden(void)
{
	return(hide_tags);
}

QVariant BaseTableView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change==ItemSelectedHasChanged)
	{
		this->setToolTip(this->table_tooltip);
		configureObjectSelection();
		attribs_toggler->clearButtonsSelection();
	}
	else if(change == ItemVisibleHasChanged)
	{
		if(value.toBool() && pending_geom_update)
		{
			this->configureObject();
			pending_geom_update = false;
		}
	}

	if(change==ItemPositionHasChanged)
		emit s_objectMoved();

	BaseObjectView::itemChange(change, value);

	return(value);
}

void BaseTableView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//Emit a signal containing the select child object if the user right-click the focused item
	if(!this->isSelected() && event->buttons()==Qt::RightButton && sel_child_obj)
	{
		if(this->scene())
			this->scene()->clearSelection();

		/* Deactivate the table in order not to hide the child object selection.
			 The table object is reativated when the context menu is hidden */
		this->setEnabled(false);

		emit s_childObjectSelected(sel_child_obj);
	}

	//If intended selection of a subitem
	else if(this->isSelected() && event->buttons()==Qt::LeftButton && sel_child_obj &&
			!is_ungrouped && QGuiApplication::queryKeyboardModifiers() & Qt::AltModifier)
	{
		this->ungroupCols();

		//Get a grip on the tableobjectview item that was just hovered :
		//we match on baseobject names within the columns of this table.
		//Then select it.
		for(int i=0;i< this->ungrouped_cols.size();i++)
		{
			if(sel_child_obj->getName()==
						dynamic_cast<BaseObjectView *>(this->ungrouped_cols[i])->getSourceObject()->getName())
				this->ungrouped_cols[i]->setSelected(true);
		}
	}
	else
	{
		QPointF pnt = attribs_toggler->mapFromScene(event->scenePos());

		//If the user clicks the extended attributes toggler
		if(!this->isSelected() && event->buttons()==Qt::LeftButton &&
			 attribs_toggler->isVisible() && attribs_toggler->boundingRect().contains(pnt))
			attribs_toggler->setButtonSelected(pnt, true);

		BaseObjectView::mousePressEvent(event);
	}
}

void BaseTableView::setAttributesPerPage(unsigned section_id, unsigned value)
{
	if(section_id > BaseTable::ExtAttribsSection)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(value > 0)
		attribs_per_page[section_id] = value;
}

unsigned BaseTableView::getAttributesPerPage(unsigned section_id)
{
	if(section_id > BaseTable::ExtAttribsSection)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(attribs_per_page[section_id]);
}

void BaseTableView::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	if(!this->isSelected() && obj_selection->isVisible())
		obj_selection->setVisible(false);

	attribs_toggler->clearButtonsSelection();
	sel_child_obj=nullptr;
}

void BaseTableView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	/* Case the table itself is not selected shows the child selector
		at mouse position */
	if(!this->is_ungrouped)
	{
		QList<QGraphicsItem *> items;
		double cols_height, item_idx, ext_height=0;
		QRectF rect, rect1;
		QPointF pnt = attribs_toggler->mapFromScene(event->scenePos());

		items.append(columns->childItems());

		if(!hide_ext_attribs &&
			 dynamic_cast<BaseTable *>(this->getSourceObject())->getCollapseMode() == CollapseMode::NotCollapsed)
		{
			items.append(ext_attribs->childItems());
			ext_height=ext_attribs->boundingRect().height();
		}

		//Calculates the default item height
		cols_height=(columns->boundingRect().height() + ext_height + (2*VertSpacing)) / static_cast<double>(items.size());

		//Calculates the item index based upon the mouse position
		rect=this->mapRectToItem(title, title->boundingRect());
		item_idx=(event->pos().y() - rect.bottom()) / cols_height;

		if(attribs_toggler->isVisible() && attribs_toggler->boundingRect().contains(pnt))
		{
			attribs_toggler->setButtonSelected(pnt);
		}
		//If the index is invalid clears the selection
		else if(item_idx < 0 || item_idx >= items.size())
		{
			this->hoverLeaveEvent(event);
			this->setToolTip(this->table_tooltip);
		}
		else if(!items.isEmpty())
		{
			BaseObjectView *item=dynamic_cast<TableObjectView *>(items[item_idx]);

			if(!this->isSelected())
			{
				//Configures the selection with the item's dimension
				if(obj_selection->boundingRect().height()!=item->boundingRect().height())
				{
					dynamic_cast<RoundedRectItem *>(obj_selection)->setBorderRadius(2);
					dynamic_cast<RoundedRectItem *>(obj_selection)->setRect(QRectF(0, 0,
																																				 title->boundingRect().width() - (2.5 * HorizSpacing),
																																				 item->boundingRect().height() - VertSpacing));
				}

				//Sets the selection position as same as item's position
				rect1=this->mapRectToItem(item, item->boundingRect());
				obj_selection->setVisible(true);
				obj_selection->setPos(QPointF(title->pos().x() + HorizSpacing, -rect1.top() + VertSpacing/2));
			}

			//Stores the selected child object
			sel_child_obj=dynamic_cast<TableObject *>(item->getSourceObject());
			this->setToolTip(item->toolTip());
		}
	}
}

void BaseTableView::addConnectedRelationship(BaseRelationship *base_rel)
{
	BaseTable *tab = dynamic_cast<BaseTable *>(getSourceObject());

	if(!base_rel ||
		 (base_rel &&
			base_rel->getTable(BaseRelationship::SrcTable) != tab &&
			base_rel->getTable(BaseRelationship::DstTable) != tab))
		return;

	connected_rels.push_back(base_rel);
}

void BaseTableView::removeConnectedRelationship(BaseRelationship *base_rel)
{
	connected_rels.erase(std::find(connected_rels.begin(), connected_rels.end(), base_rel));
}

int BaseTableView::getConnectedRelationshipIndex(BaseRelationship *base_rel)
{
	vector<BaseRelationship *>::iterator itr = std::find(connected_rels.begin(), connected_rels.end(), base_rel);

	if(itr != connected_rels.end())
		return(itr - connected_rels.begin());

	return(-1);
}

unsigned BaseTableView::getConnectedRelsCount(BaseTable *src_tab, BaseTable *dst_tab)
{
	unsigned count = 0;

	for(auto &rel : connected_rels)
	{
		if((rel->getTable(BaseRelationship::SrcTable) == src_tab &&
				rel->getTable(BaseRelationship::DstTable) == dst_tab) ||
			 (rel->getTable(BaseRelationship::SrcTable) == dst_tab &&
				rel->getTable(BaseRelationship::DstTable) == src_tab))
			count++;
	}

	return(count);
}

void BaseTableView::configureTag(void)
{
	BaseTable *tab=dynamic_cast<BaseTable *>(this->getSourceObject());
	Tag *tag=tab->getTag();

	tag_item->setVisible(tag!=nullptr && !hide_tags);

	if(!hide_tags && tag)
	{
		QPolygonF pol;
		QPointF p1, p2;
		double bottom;
		QFont fnt=BaseObjectView::getFontStyle(Attributes::Tag).font();

		fnt.setPointSizeF(fnt.pointSizeF() * 0.80f);
		tag_item->setFont(fnt);
		tag_item->setText(tag->getName());
		tag_item->setBrush(BaseObjectView::getFontStyle(Attributes::Tag).foreground());

		p1=tag_item->getTextBoundingRect().topLeft();
		p2=tag_item->getTextBoundingRect().bottomRight();
		bottom=this->boundingRect().bottom();

		pol.append(QPointF(p1.x()- HorizSpacing, p1.y() - VertSpacing));
		pol.append(QPointF(p2.x(), p1.y() - VertSpacing));
		pol.append(QPointF(p2.x() + HorizSpacing + 5, p2.y()/2));
		pol.append(QPointF(p2.x(), p2.y() + VertSpacing));
		pol.append(QPointF(p1.x(), p2.y() + VertSpacing));
		pol.append(QPointF(p1.x()-HorizSpacing, p2.y() + VertSpacing));

		tag_item->setPolygon(pol);
		tag_item->setPen(BaseObjectView::getBorderStyle(Attributes::Tag));
		tag_item->setBrush(BaseObjectView::getFillStyle(Attributes::Tag));
		tag_item->setPos(-5, bottom - 1.5f);
		tag_item->setTextPos(HorizSpacing/2, 0);
	}
}

void BaseTableView::__configureObject(float width)
{
	BaseTable *tab = dynamic_cast<BaseTable *>(getSourceObject());
	double height = 0,
			factor = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()))->logicalDotsPerInch() / 96.0f,
			pixel_ratio = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()))->devicePixelRatio();

	QPen pen = body->pen();
	attribs_toggler->setBrush(body->brush());
	attribs_toggler->setPen(body->pen());

	QLinearGradient grad(QPointF(0,0),QPointF(0,1));
	grad.setCoordinateMode(QGradient::ObjectBoundingMode);
	grad.setColorAt(0, body->pen().color().lighter(200));
	grad.setColorAt(1, body->pen().color().lighter());
	pen.setStyle(Qt::SolidLine);

	attribs_toggler->setButtonsBrush(grad);
	attribs_toggler->setButtonsPen(body->pen());
	attribs_toggler->setRect(QRectF(0, 0, width, 12 * factor * pixel_ratio));
	attribs_toggler->setCollapseMode(tab->getCollapseMode());

	//Set the protected icon position to the top-right on the title
	protected_icon->setPos(title->pos().x() + (2 * HorizSpacing), title->boundingRect().height() * 0.25);
	this->bounding_rect = title->boundingRect();

	body->setRoundedCorners(RoundedRectItem::NoCorners);

	height = title->boundingRect().height() + attribs_toggler->boundingRect().height() - VertSpacing;
	height += (body->isVisible() ? body->boundingRect().height() : 1);
	height += (ext_attribs_body->isVisible() ? ext_attribs_body->boundingRect().height() - VertSpacing + 1 : 0);

	this->bounding_rect.setHeight(height);

	attribs_toggler->setPos(title->pos().x(),
													height - attribs_toggler->boundingRect().height());

	this->table_tooltip=this->getSourceObject()->getName(true) +
						QString(" (") + this->getSourceObject()->getTypeName() + QString(") \n") +
						QString("Id: %1\n").arg(this->getSourceObject()->getObjectId()) +
						trUtf8("Connected rels: %1").arg(this->getConnectRelsCount());

	this->setToolTip(this->table_tooltip);

	configureObjectSelection();
	configureObjectShadow();
}

double BaseTableView::calculateWidth(void)
{
	/* Calculating the maximum width between the title, columns, extended attributes and the attribs toggler.
	 * This width is used to set the uniform width of table */
	vector<double> widths = { columns->isVisible() ? columns->boundingRect().width() : 0,
														ext_attribs->isVisible() ? ext_attribs->boundingRect().width() : 0,
														attribs_toggler->isVisible() ? attribs_toggler->getButtonsWidth() : 0,
														title->boundingRect().width() };

	std::sort(widths.begin(), widths.end());
	return (widths.back() + (2 * HorizSpacing));
}

int BaseTableView::getConnectRelsCount(void)
{
	return(connected_rels.size());
}

void BaseTableView::requestRelationshipsUpdate(void)
{
	emit s_relUpdateRequest();
}

void BaseTableView::togglePlaceholder(bool value)
{
	BaseObjectView::togglePlaceholder(!connected_rels.empty() && value);
}

void BaseTableView::configureObjectShadow(void)
{
	RoundedRectItem *rect_item=dynamic_cast<RoundedRectItem *>(obj_shadow);

	rect_item->setPen(Qt::NoPen);
	rect_item->setBrush(QColor(50,50,50,60));
	rect_item->setRect(this->boundingRect());
	rect_item->setPos(3.5, 4.5);
}

void BaseTableView::startGeometryUpdate(void)
{
	//We need to force the object to be not selectable so further calls to mousePressEvent doesn't select the object
	this->setFlag(QGraphicsItem::ItemIsSelectable, false);
}

void BaseTableView::finishGeometryUpdate(void)
{
	//Updating the object's geometry to reflect the geometry change
	this->configureObject();
	obj_selection->setVisible(false);

	// Using a single shot time to restore the selectable flag
	QTimer::singleShot(300, [&]{ this->setFlag(QGraphicsItem::ItemIsSelectable, true); });

	//Updating the schema box that holds the object (if visible)
	dynamic_cast<Schema *>(this->getSourceObject()->getSchema())->setModified(true);
}

bool BaseTableView::configurePaginationParams(unsigned section_id, unsigned total_attrs, unsigned &start_attr, unsigned &end_attr)
{
	if(section_id > BaseTable::ExtAttribsSection)
		return false;

	BaseTable *table = dynamic_cast<BaseTable *>(getSourceObject());
	unsigned attr_per_page = attribs_per_page[section_id];

	start_attr = end_attr = 0;
	attribs_toggler->setPaginationEnabled(table->isPaginationEnabled());

	/* If the pagination is enabled for the table and the amount of objects is greater than the
	 * number of objects per page we configure the pagination parameter */
	if(table->isPaginationEnabled() && total_attrs > attr_per_page)
	{
		// Calculating the proportions of columns and extended attributes displayed per page
		unsigned max_pages = 0, curr_page = table->getCurrentPage(section_id);

		// Determining the maximum amount of pages
		max_pages = ceil(total_attrs / static_cast<double>(attr_per_page));

		// Validating the current page related to the maximum determined
		if(curr_page >= max_pages)
			curr_page = max_pages - 1;

		// Calculating the start and end columns/ext. attributes for the current page
		start_attr = curr_page * attr_per_page;
		end_attr = start_attr + attr_per_page;

		// Validating the determined start/end indexes avoiding the extrapolation of limits
		if(start_attr > total_attrs)
			start_attr = total_attrs;

		if(end_attr > total_attrs)
			end_attr = total_attrs;

		// Configure the attributes toggler item withe the calculated pagination parameters
		attribs_toggler->setPaginationValues(section_id, curr_page, max_pages);
		return(true);
	}
	else
	{
		attribs_toggler->setPaginationValues(section_id, 0, 0);
		return(false);
	}
}

void BaseTableView::configureCollapsedSections(CollapseMode coll_mode)
{
	if(is_ungrouped)
	{
		this->scene()->clearSelection();
		this->checkRegroupCols();
	}

	startGeometryUpdate();
	dynamic_cast<BaseTable *>(this->getSourceObject())->setCollapseMode(coll_mode);
	finishGeometryUpdate();
	emit s_collapseModeChanged();

	// Using a single shot time to restore the selectable flag
	QTimer::singleShot(500, [&]{
		this->scene()->clearSelection();
		QKeyEvent *event = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
		QCoreApplication::postEvent(scene(), event);});
}

void BaseTableView::togglePagination(bool enabled)
{
	BaseTable *tab = dynamic_cast<BaseTable *>(this->getSourceObject());

	if(is_ungrouped)
	{
		this->scene()->clearSelection();
		this->checkRegroupCols();
	}
	startGeometryUpdate();
	tab->setPaginationEnabled(enabled);
	tab->resetCurrentPages();
	finishGeometryUpdate();
	emit s_paginationToggled();
}

void BaseTableView::configureCurrentPage(unsigned section_id, unsigned page)
{
	startGeometryUpdate();
	dynamic_cast<BaseTable *>(this->getSourceObject())->setCurrentPage(section_id, page);
	finishGeometryUpdate();
	emit s_currentPageChanged();
}

void BaseTableView::ungroupCols()
{
	QList<QGraphicsItem *> cols = this->columns->childItems();
	ungrouped_cols.clear();

	this->setFlag(QGraphicsItem::ItemIsMovable, false);
	for(auto & col : cols)
	{
		auto * subitem = dynamic_cast<TableObjectView *>(col);
		subitem->setFlag(QGraphicsItem::ItemIsMovable, false);
		this->columns->removeFromGroup(subitem);
		this->removeFromGroup(subitem);
		ungrouped_cols.append(subitem);
	}
	this->is_ungrouped=true;
	this->setSelected(false);
}

void BaseTableView::checkRegroupCols()
{
	if(!is_ungrouped) return;

	for(auto & ungrouped_col : ungrouped_cols)
	{
		if(ungrouped_col->isSelected()) return;
	}

	for (auto subitem : ungrouped_cols)
	{
		this->columns->addToGroup(subitem);
		subitem->setFlag(QGraphicsItem::ItemIsMovable, true);
	}

	ungrouped_cols.clear();
	this->is_ungrouped=false;
	this->setSelected(false);
	this->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void BaseTableView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::NoBrush);
	QGraphicsItemGroup::paint(painter,option,widget);
}
