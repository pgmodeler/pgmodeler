#include "swapobjectsidswidget.h"
#include "pgmodeleruins.h"

const QString SwapObjectsIdsWidget::IdLabel("ID: <strong>%1</strong>");

SwapObjectsIdsWidget::SwapObjectsIdsWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
	try
	{
		QGridLayout *swap_objs_grid=new QGridLayout(this);
		vector<ObjectType> types=BaseObject::getObjectTypes(true, {ObjectType::Permission,
																															 ObjectType::Role, ObjectType::Textbox,
																															 ObjectType::Column, ObjectType::Constraint });
		setupUi(this);

		PgModelerUiNs::configureWidgetFont(message_lbl, PgModelerUiNs::MediumFontFactor);

		selector_idx = 0;
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
		swap_objs_grid->addWidget(swap_values_tb, 0, 2, 2, 1);
		swap_objs_grid->addWidget(src_id_lbl, 0, 3);
		swap_objs_grid->addWidget(src_ico_lbl, 0, 4);

		swap_objs_grid->addWidget(before_lbl, 1, 0);
		swap_objs_grid->addWidget(dst_object_sel, 1, 1);
		swap_objs_grid->addWidget(dst_id_lbl, 1, 3);
		swap_objs_grid->addWidget(dst_ico_lbl, 1, 4);

		swap_objs_grid->addWidget(filter_btn, 2, 0, 1, 1);
		swap_objs_grid->addWidget(filter_wgt, 2, 1, 1, 4);

		swap_objs_grid->addWidget(objects_tbw, 3, 0, 1, 5);
		swap_objs_grid->addWidget(alert_frm, 4, 0, 1, 5);

		setModel(nullptr);
		filter_wgt->setVisible(false);

		connect(filter_btn, SIGNAL(toggled(bool)), filter_wgt, SLOT(setVisible(bool)));
		connect(src_object_sel, SIGNAL(s_objectSelected()), this, SLOT(showObjectId()));
		connect(dst_object_sel, SIGNAL(s_objectSelected()), this, SLOT(showObjectId()));
		connect(src_object_sel, SIGNAL(s_selectorCleared()), this, SLOT(showObjectId()));
		connect(dst_object_sel, SIGNAL(s_selectorCleared()), this, SLOT(showObjectId()));

		connect(swap_values_tb, &QToolButton::clicked,
		[&](){
			BaseObject *obj=src_object_sel->getSelectedObject();
			src_object_sel->setSelectedObject(dst_object_sel->getSelectedObject());
			dst_object_sel->setSelectedObject(obj);
		});

		connect(objects_tbw, &QTableWidget::itemDoubleClicked,
		[&](QTableWidgetItem *item){
			if(QApplication::mouseButtons() == Qt::LeftButton)
				selectItem(item);
		});

		setMinimumSize(640,480);

		//connect(swap_ids_tb, SIGNAL(clicked(bool)), this, SLOT(swapObjectsIds()));
		connect(filter_edt, SIGNAL(textChanged(QString)), this, SLOT(filterObjects()));
		connect(hide_rels_chk, SIGNAL(toggled(bool)), this, SLOT(filterObjects()));
		connect(hide_sys_objs_chk, SIGNAL(toggled(bool)), this, SLOT(filterObjects()));

		objects_tbw->installEventFilter(this);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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

void SwapObjectsIdsWidget::setSelectedObjects(BaseObject *src_object, BaseObject *dst_objct)
{
	src_object_sel->setSelectedObject(src_object);
	dst_object_sel->setSelectedObject(dst_objct);
	selector_idx = (src_object && !dst_objct ? 1 : 0);
}

void SwapObjectsIdsWidget::fillCreationOrderGrid()
{
	objects_tbw->clearContents();
	objects_tbw->setRowCount(0);

	if(!model)
		return;

	map<unsigned, BaseObject *> creation_order = model->getCreationOrder(SchemaParser::SqlDefinition);
	vector<BaseObject *> objects;

	//Using an stl function to extract all the values (objects) from the map and put them into a list
	std::for_each(creation_order.begin(), creation_order.end(), [&](const std::pair<unsigned, BaseObject *> &itr) {
		if(itr.second->getObjectType() != ObjectType::Constraint) {
			objects.push_back(itr.second);
		}
	});

	ObjectFinderWidget::updateObjectTable(objects_tbw, objects);
	objects_tbw->resizeColumnsToContents();

	if(!filter_edt->text().isEmpty() || hide_rels_chk->isChecked() || hide_sys_objs_chk->isChecked())
		filterObjects();
}

bool SwapObjectsIdsWidget::eventFilter(QObject *object, QEvent *event)
{
	if(object == objects_tbw && event->type() == QEvent::KeyPress)
	{
		QKeyEvent *k_event = dynamic_cast<QKeyEvent *>(event);
		QTableWidgetItem *item = objects_tbw->currentItem();
		int row = item->row();

		if(k_event->key() == Qt::Key_Space)
			selectItem(item);
		else if((k_event->key() == Qt::Key_Down || k_event->key() == Qt::Key_Up) && k_event->modifiers() == Qt::ControlModifier)
		{
			QTableWidgetItem *aux_item = nullptr;
			int key_code = k_event->key();

			clearSelectors();
			selectItem(item);

			while(!aux_item)
			{
				// If user presses down we get the next item below
				if(key_code == Qt::Key_Down && row < objects_tbw->rowCount() - 1)
					aux_item = objects_tbw->item(row + 1, 0);
				// If user presses up we get the next item above
				else if(key_code == Qt::Key_Up && row > 1)
					aux_item =  objects_tbw->item(row - 1, 0);

				// If we reach a hidden row we need to jump to the next one that is not hidden
				if(aux_item && objects_tbw->isRowHidden(aux_item->row()))
				{
					aux_item = nullptr;
					row += (key_code == Qt::Key_Down ? 1 : -1);
				}

				// Breaking if we've reached the top or down limits (avoiding swap ids with null objects)
				if((key_code == Qt::Key_Down && row >= objects_tbw->rowCount() - 1) ||
					 (key_code == Qt::Key_Up && row <= 0))
					break;
			}

			if(aux_item)
			{
				selectItem(aux_item);
				swapObjectsIds();
				clearSelectors();
				objects_tbw->setCurrentItem(objects_tbw->item(row , 0));
			}
		}
	}

	return QWidget::eventFilter(object, event);
}

void SwapObjectsIdsWidget::showObjectId()
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
		id_lbl->setText(IdLabel.arg(sel_obj->getObjectId()));
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

	emit s_objectsIdsSwapReady(src_object_sel->getSelectedObject() &&
														 dst_object_sel->getSelectedObject());
}

void SwapObjectsIdsWidget::swapObjectsIds()
{
	BaseObject *src_obj=src_object_sel->getSelectedObject(),
			*dst_obj=dst_object_sel->getSelectedObject();
	BaseGraphicObject *graph_src_obj=dynamic_cast<BaseGraphicObject *>(src_obj),
			*graph_dst_obj=dynamic_cast<BaseGraphicObject *>(dst_obj);

	if(!src_obj && !dst_obj)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raise an exception if the user try to swap an id of relationship by other object of different kind
	else if((src_obj->getObjectType()==ObjectType::Relationship || dst_obj->getObjectType()==ObjectType::Relationship) &&
					(src_obj->getObjectType() != dst_obj->getObjectType()))
		throw Exception(ErrorCode::InvRelationshipIdSwap,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		BaseObject::swapObjectsIds(src_obj, dst_obj, false);

		//Special id swap for relationship
		if(src_obj->getObjectType()==ObjectType::Relationship)
		{
			vector<BaseObject *>::iterator itr, itr1;
			vector<BaseObject *> *list=model->getObjectList(ObjectType::Relationship);

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

		src_id_lbl->setText(IdLabel.arg(src_object_sel->getSelectedObject()->getObjectId()));
		dst_id_lbl->setText(IdLabel.arg(dst_object_sel->getSelectedObject()->getObjectId()));

		emit s_objectsIdsSwapped();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SwapObjectsIdsWidget::filterObjects()
{
	BaseObject *object = nullptr;
	bool is_rel = false, is_sys_obj = false;
	QList<QTableWidgetItem*> items=objects_tbw->findItems(filter_edt->text(), Qt::MatchStartsWith | Qt::MatchRecursive);
	QTableWidgetItem *item = nullptr;

	for(int row=0; row < objects_tbw->rowCount(); row++)
		objects_tbw->setRowHidden(row, true);

	while(!items.isEmpty())
	{
		item = items.front();
		object = reinterpret_cast<BaseObject *>(objects_tbw->item(item->row(), 0)->data(Qt::UserRole).value<void *>());
		is_rel = (object->getObjectType() == ObjectType::BaseRelationship || object->getObjectType() == ObjectType::Relationship);
		is_sys_obj = object->isSystemObject();

		if((!is_rel && !is_sys_obj) ||
			 (!hide_rels_chk->isChecked() && is_rel) ||
			 (!hide_sys_objs_chk->isChecked() && is_sys_obj))
			objects_tbw->setRowHidden(items.front()->row(), false);

		items.pop_front();
	}
}

void SwapObjectsIdsWidget::selectItem(QTableWidgetItem *item)
{
	QTableWidgetItem *item_aux = (item->column() == 1 ? item : objects_tbw->item(item->row(), 1));
	BaseObject *obj = reinterpret_cast<BaseObject *>(item_aux->data(Qt::UserRole).value<void *>());

	if(selector_idx == 0)
	{
		src_object_sel->setSelectedObject(obj);
		selector_idx = 1;
	}
	else
	{
		dst_object_sel->setSelectedObject(obj);
		selector_idx = 0;
	}
}

void SwapObjectsIdsWidget::clearSelectors()
{
	selector_idx = 0;
	src_object_sel->clearSelector();
	dst_object_sel->clearSelector();
}
