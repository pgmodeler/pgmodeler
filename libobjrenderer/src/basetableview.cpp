#include "basetableview.h"

BaseTableView::BaseTableView(BaseTable *base_tab) : BaseObjectView(base_tab)
{
	if(!base_tab)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	body=new QGraphicsPolygonItem;
	title=new TableTitleView;
	this->addToGroup(body);
	this->addToGroup(title);
}

BaseTableView::~BaseTableView(void)
{
	this->removeFromGroup(body);
	this->removeFromGroup(title);
	delete(body);
	delete(title);
}

QVariant BaseTableView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	BaseObjectView::itemChange(change, value);

	if(change==ItemPositionHasChanged)
		emit s_objectMoved();

	return(value);
}
