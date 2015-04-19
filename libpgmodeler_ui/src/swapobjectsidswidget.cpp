#include "swapobjectsidswidget.h"

SwapObjectsIdsWidget::SwapObjectsIdsWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	try
	{
		QGridLayout *swap_objs_grid=new QGridLayout(this);
    vector<ObjectType> types=BaseObject::getObjectTypes(true, {OBJ_PERMISSION, OBJ_ROLE, OBJ_TEXTBOX,
                                                               /*OBJ_RELATIONSHIP,*/ OBJ_COLUMN, OBJ_CONSTRAINT });
    setupUi(this);

		src_object_sel=nullptr;
		dst_object_sel=nullptr;
		src_object_sel=new ObjectSelectorWidget(types, true, this);
		dst_object_sel=new ObjectSelectorWidget(types, true, this);

    parent_form.setWindowTitle(QString("Change objects creation order"));
		parent_form.generalwidget_wgt->insertWidget(0, this);
		parent_form.generalwidget_wgt->setCurrentIndex(0);
		parent_form.setButtonConfiguration(Messagebox::OK_CANCEL_BUTTONS);

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
		hlayout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Expanding));

		swap_objs_grid->addLayout(hlayout, 2, 0, 1, 4);
		swap_objs_grid->addWidget(alert_frm, 3, 0, 1, 4);

		parent_form.setMinimumSize(540,220);
		parent_form.resize(parent_form.minimumSize());
		parent_form.setMaximumHeight(220);

    setModel(nullptr);

		connect(parent_form.cancel_btn, SIGNAL(clicked(bool)), this, SLOT(close(void)));
    connect(parent_form.apply_ok_btn, SIGNAL(clicked(bool)), this, SLOT(swapObjectsIds(void)));
		connect(src_object_sel, SIGNAL(s_objectSelected(void)), this, SLOT(showObjectId(void)));
		connect(dst_object_sel, SIGNAL(s_objectSelected(void)), this, SLOT(showObjectId(void)));
		connect(src_object_sel, SIGNAL(s_selectorCleared(void)), this, SLOT(showObjectId(void)));
		connect(dst_object_sel, SIGNAL(s_selectorCleared(void)), this, SLOT(showObjectId(void)));

		connect(swap_values_tb, &QToolButton::clicked,
						[=](){ BaseObject *obj=src_object_sel->getSelectedObject();
									 src_object_sel->setSelectedObject(dst_object_sel->getSelectedObject());
									 dst_object_sel->setSelectedObject(obj); });
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

SwapObjectsIdsWidget::~SwapObjectsIdsWidget()
{
	parent_form.generalwidget_wgt->removeWidget(this);
}

void SwapObjectsIdsWidget::show(void)
{
	QDialog::show();
	parent_form.exec();
}

void SwapObjectsIdsWidget::setModel(DatabaseModel *model)
{
  this->model=model;

  src_object_sel->setModel(model);
  dst_object_sel->setModel(model);

	parent_form.generalwidget_wgt->setEnabled(model!=nullptr);
	parent_form.apply_ok_btn->setEnabled(model!=nullptr);

	src_object_sel->clearSelector();
	dst_object_sel->clearSelector();
}

void SwapObjectsIdsWidget::close(void)
{
	this->setResult(QDialog::Rejected);
  parent_form.close();
}

void SwapObjectsIdsWidget::hideEvent(QHideEvent *)
{
  this->setModel(nullptr);
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
		id_lbl->setText(QString("ID: <strong>%1</strong>").arg(sel_obj->getObjectId()));
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/") +
															 BaseObject::getSchemaName(sel_obj->getObjectType()) +
															 QString(".png")));
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
}

void SwapObjectsIdsWidget::swapObjectsIds(void)
{
	BaseObject *src_obj=src_object_sel->getSelectedObject(),
						 *dst_obj=dst_object_sel->getSelectedObject();
	BaseGraphicObject *graph_src_obj=dynamic_cast<BaseGraphicObject *>(src_obj),
										*graph_dst_obj=dynamic_cast<BaseGraphicObject *>(dst_obj);

  //Raise an exception if the user try to swap an id of relationship by other object of different kind
  if((src_obj->getObjectType()==OBJ_RELATIONSHIP || dst_obj->getObjectType()==OBJ_RELATIONSHIP) &&
     (src_obj->getObjectType() != dst_obj->getObjectType()))
    throw Exception(ERR_INV_REL_ID_SWAP,__PRETTY_FUNCTION__,__FILE__,__LINE__);


	try
	{    
		BaseObject::swapObjectsIds(src_obj, dst_obj, false);

    //Special id swap for relationship
    if(src_obj->getObjectType()==OBJ_RELATIONSHIP)
    {
      vector<BaseObject *>::iterator itr, itr1;
      vector<BaseObject *> *list=model->getObjectList(OBJ_RELATIONSHIP);

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
    this->setResult(QDialog::Accepted);
		parent_form.close();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
