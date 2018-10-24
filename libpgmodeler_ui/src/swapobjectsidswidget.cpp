#include "swapobjectsidswidget.h"
#include "pgmodeleruins.h"

const QString SwapObjectsIdsWidget::ID_LABEL = QString("ID: <strong>%1</strong>");

SwapObjectsIdsWidget::SwapObjectsIdsWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
	try
	{
		QGridLayout *swap_objs_grid=new QGridLayout(this);
		vector<ObjectType> types=BaseObject::getObjectTypes(true, {ObjPermission, ObjRole, ObjTextbox,
																   ObjColumn, ObjConstraint });
		setupUi(this);

		PgModelerUiNs::configureWidgetFont(message_lbl, PgModelerUiNs::MEDIUM_FONT_FACTOR);

		src_object_sel=nullptr;
		dst_object_sel=nullptr;

		src_object_sel=new ObjectSelectorWidget(types, true, this);
		src_object_sel->enableObjectCreation(false);

		dst_object_sel=new ObjectSelectorWidget(types, true, this);
		dst_object_sel->enableObjectCreation(false);

		swap_objs_grid->setContentsMargins(4,4,4,4);
		swap_objs_grid->setSpacing(6);

		swap_objs_grid->addWidget(create_lbl, 0, 0);
		swap_objs_grid->addWidget(src_object_sel, 0, 1);
		swap_objs_grid->addWidget(src_id_lbl, 0, 2);
		swap_objs_grid->addWidget(src_ico_lbl, 0, 3);

		swap_objs_grid->addWidget(before_lbl, 1, 0);
		swap_objs_grid->addWidget(dst_object_sel, 1, 1);
		swap_objs_grid->addWidget(dst_id_lbl, 1, 2);
		swap_objs_grid->addWidget(dst_ico_lbl, 1, 3);

		QHBoxLayout *hlayout=new QHBoxLayout;
		hlayout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Expanding));
		hlayout->addWidget(swap_values_tb);
		hlayout->addWidget(swap_ids_tb);
		hlayout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Expanding));

		swap_objs_grid->addLayout(hlayout, 2, 0, 1, 4);
		swap_objs_grid->addWidget(filter_wgt, swap_objs_grid->count(), 0, 1, 4);
		swap_objs_grid->addWidget(objects_tbw, swap_objs_grid->count(), 0, 1, 4);
		swap_objs_grid->addWidget(alert_frm, swap_objs_grid->count(), 0, 1, 4);

		setModel(nullptr);

		connect(src_object_sel, SIGNAL(s_objectSelected(void)), this, SLOT(showObjectId(void)));
		connect(dst_object_sel, SIGNAL(s_objectSelected(void)), this, SLOT(showObjectId(void)));
		connect(src_object_sel, SIGNAL(s_selectorCleared(void)), this, SLOT(showObjectId(void)));
		connect(dst_object_sel, SIGNAL(s_selectorCleared(void)), this, SLOT(showObjectId(void)));

		connect(swap_values_tb, &QToolButton::clicked,
		[&](){
			BaseObject *obj=src_object_sel->getSelectedObject();
			src_object_sel->setSelectedObject(dst_object_sel->getSelectedObject());
			dst_object_sel->setSelectedObject(obj);
		});

		connect(objects_tbw, &QTableWidget::itemDoubleClicked,
		[&](QTableWidgetItem *item){

			static unsigned sel_idx = 0;
			QTableWidgetItem *item_aux = (item->column() == 1 ? item : objects_tbw->item(item->row(), 1));
			BaseObject *obj = reinterpret_cast<BaseObject *>(item_aux->data(Qt::UserRole).value<void *>());

			if(QApplication::mouseButtons() == Qt::LeftButton)
			{
				if(sel_idx == 0)
				{
					src_object_sel->setSelectedObject(obj);
					sel_idx = 1;
				}
				else
				{
					dst_object_sel->setSelectedObject(obj);
					sel_idx = 0;
				}
			}
		});

		setMinimumSize(640,480);

		connect(swap_ids_tb, SIGNAL(clicked(bool)), this, SLOT(swapObjectsIds()));
		connect(filter_edt, SIGNAL(textChanged(QString)), this, SLOT(filterObjects()));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

SwapObjectsIdsWidget::~SwapObjectsIdsWidget()
{

}

void SwapObjectsIdsWidget::setModel(DatabaseModel *model)
{
	this->model=model;

	src_object_sel->setModel(model);
	dst_object_sel->setModel(model);

	src_object_sel->clearSelector();
	dst_object_sel->clearSelector();

	fillCreationOrderGrid();
}

void SwapObjectsIdsWidget::fillCreationOrderGrid(void)
{
	objects_tbw->clearContents();
	objects_tbw->setRowCount(0);

	if(!model)
		return;

	map<unsigned, BaseObject *> creation_order = model->getCreationOrder(SchemaParser::SqlDefinition);
	vector<BaseObject *> objects;

	//Using an stl function to extract all the values (objects) from the map and put them into a list
	std::for_each(creation_order.begin(), creation_order.end(), [&](const std::pair<unsigned, BaseObject *> &itr) {
		if(itr.second->getObjectType() != ObjConstraint) {
			objects.push_back(itr.second);
		}
	});

	ObjectFinderWidget::updateObjectTable(objects_tbw, objects);
	objects_tbw->resizeColumnsToContents();
}

void SwapObjectsIdsWidget::showObjectId(void)
{
	QLabel *ico_lbl=nullptr, *id_lbl=nullptr;
	BaseObject *sel_obj=nullptr;

	if(sender()==src_object_sel)
	{
		ico_lbl=src_ico_lbl;
		id_lbl=src_id_lbl;
		sel_obj=src_object_sel->getSelectedObject();
	}
	else
	{
		ico_lbl=dst_ico_lbl;
		id_lbl=dst_id_lbl;
		sel_obj=dst_object_sel->getSelectedObject();
	}

	id_lbl->clear();
	if(sel_obj)
	{
		id_lbl->setText(ID_LABEL.arg(sel_obj->getObjectId()));
		ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath(sel_obj->getObjectType())));
		ico_lbl->setToolTip(sel_obj->getTypeName());

		id_lbl->setVisible(true);
		ico_lbl->setVisible(true);
	}
	else
	{
		id_lbl->setVisible(false);
		ico_lbl->setVisible(false);
	}

	swap_values_tb->setEnabled(src_object_sel->getSelectedObject() &&
														 dst_object_sel->getSelectedObject());

	swap_ids_tb->setEnabled(src_object_sel->getSelectedObject() &&
													dst_object_sel->getSelectedObject());
}

void SwapObjectsIdsWidget::swapObjectsIds(void)
{
	BaseObject *src_obj=src_object_sel->getSelectedObject(),
			*dst_obj=dst_object_sel->getSelectedObject();
	BaseGraphicObject *graph_src_obj=dynamic_cast<BaseGraphicObject *>(src_obj),
			*graph_dst_obj=dynamic_cast<BaseGraphicObject *>(dst_obj);

	if(!src_obj && !dst_obj)
		throw Exception(OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raise an exception if the user try to swap an id of relationship by other object of different kind
	else if((src_obj->getObjectType()==ObjRelationship || dst_obj->getObjectType()==ObjRelationship) &&
			(src_obj->getObjectType() != dst_obj->getObjectType()))
		throw Exception(InvRelationshipIdSwap,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		BaseObject::swapObjectsIds(src_obj, dst_obj, false);

		//Special id swap for relationship
		if(src_obj->getObjectType()==ObjRelationship)
		{
			vector<BaseObject *>::iterator itr, itr1;
			vector<BaseObject *> *list=model->getObjectList(ObjRelationship);

			//Find the relationships in the list and swap the memory position too
			itr=std::find(list->begin(), list->end(), src_obj);
			itr1=std::find(list->begin(), list->end(), dst_obj);
			(*itr)=dst_obj;
			(*itr1)=src_obj;

			model->validateRelationships();
		}
		else
		{
			if(graph_src_obj)
				graph_src_obj->setModified(true);

			if(graph_dst_obj)
				graph_dst_obj->setModified(true);
		}

		model->setInvalidated(true);
		fillCreationOrderGrid();

		src_id_lbl->setText(ID_LABEL.arg(src_object_sel->getSelectedObject()->getObjectId()));
		dst_id_lbl->setText(ID_LABEL.arg(dst_object_sel->getSelectedObject()->getObjectId()));

		emit s_objectsIdsSwapped();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SwapObjectsIdsWidget::filterObjects(void)
{
	QList<QTableWidgetItem*> items=objects_tbw->findItems(filter_edt->text(), Qt::MatchStartsWith | Qt::MatchRecursive);

	for(int row=0; row < objects_tbw->rowCount(); row++)
		objects_tbw->setRowHidden(row, true);

	while(!items.isEmpty())
	{
		objects_tbw->setRowHidden(items.front()->row(), false);
		items.pop_front();
	}
}
