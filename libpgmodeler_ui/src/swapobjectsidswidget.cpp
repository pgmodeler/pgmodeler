#include "swapobjectsidswidget.h"

SwapObjectsIdsWidget::SwapObjectsIdsWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	try
	{
		vector<ObjectType> types=BaseObject::getObjectTypes(false);
		setupUi(this);

		//Remove unused object types from vector
		types.erase(std::find(types.begin(), types.end(), OBJ_PERMISSION));
		types.erase(std::find(types.begin(), types.end(), OBJ_ROLE));
		types.erase(std::find(types.begin(), types.end(), OBJ_TEXTBOX));
		types.erase(std::find(types.begin(), types.end(), OBJ_RELATIONSHIP));

		src_object_sel=nullptr;
		dst_object_sel=nullptr;
		src_object_sel=new ObjectSelectorWidget(types, true, this);
		dst_object_sel=new ObjectSelectorWidget(types, true, this);

		parent_form.setWindowTitle("Change objects creation order");
		parent_form.generalwidget_wgt->insertWidget(0, this);
		parent_form.generalwidget_wgt->setCurrentIndex(0);
		parent_form.setButtonConfiguration(Messagebox::OK_CANCEL_BUTTONS);

		change_objs_grid->addWidget(src_object_sel, 0, 1,1,1);
		change_objs_grid->addWidget(src_id_lbl, 0, 2,1,1);
		change_objs_grid->addWidget(src_ico_lbl, 0, 3,1,1);

		change_objs_grid->addWidget(dst_object_sel, 1, 1,1,1);
		change_objs_grid->addWidget(dst_id_lbl, 1, 2,1,1);
		change_objs_grid->addWidget(dst_ico_lbl, 1, 3,1,1);

		change_objs_grid->addWidget(alert_frm, 2, 0,1,4);

		parent_form.setMinimumSize(540,190);
		parent_form.resize(parent_form.minimumSize());
		parent_form.setMaximumHeight(190);

		setModel(nullptr);

		connect(parent_form.cancel_btn, SIGNAL(clicked(bool)), this, SLOT(close(void)));
		connect(parent_form.apply_ok_btn, SIGNAL(clicked(bool)), this, SLOT(changeObjectsIds(void)));
		connect(src_object_sel, SIGNAL(s_objectSelected(void)), this, SLOT(showObjectId(void)));
		connect(dst_object_sel, SIGNAL(s_objectSelected(void)), this, SLOT(showObjectId(void)));
		connect(src_object_sel, SIGNAL(s_selectorCleared(void)), this, SLOT(showObjectId(void)));
		connect(dst_object_sel, SIGNAL(s_selectorCleared(void)), this, SLOT(showObjectId(void)));
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
}

void SwapObjectsIdsWidget::changeObjectsIds(void)
{
	BaseObject *src_obj=src_object_sel->getSelectedObject(),
						 *dst_obj=dst_object_sel->getSelectedObject();

	try
	{
		BaseObject::swapObjectsIds(src_obj, dst_obj, false);
		this->accept();
		parent_form.close();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
