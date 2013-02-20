#include "sequencewidget.h"

SequenceWidget::SequenceWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_SEQUENCE)
{
	try
	{
		map<QString, vector<QWidget *> > field_map;
		QFrame *frame=NULL;

		Ui_SequenceWidget::setupUi(this);

		column_sel=NULL;
		column_sel=new ObjectSelectorWidget(OBJ_COLUMN, true, this);

		sequence_grid->addWidget(column_sel,3,1,1,3);

		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_83)].push_back(owner_col_lbl);
		frame=generateVersionWarningFrame(field_map);
		sequence_grid->addWidget(frame, sequence_grid->count()+1, 0, 1, 0);
		frame->setParent(this);

		configureFormLayout(sequence_grid, OBJ_SEQUENCE);
		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		parent_form->resize(540, 410);
		parent_form->setMinimumWidth(540);
		parent_form->setMinimumHeight(430);
		parent_form->setMaximumHeight(430);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SequenceWidget::hideEvent(QHideEvent *event)
{
	column_sel->clearSelector();
	BaseObjectWidget::hideEvent(event);
}

void SequenceWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Sequence *sequence)
{
	column_sel->setModel(model);

	if(sequence)
	{
		column_sel->setSelectedObject(sequence->getOwnerColumn());
		cyclic_chk->setChecked(sequence->isCycle());
		start_edt->setText(sequence->getStart());
		maximum_edt->setText(sequence->getMaxValue());
		minimum_edt->setText(sequence->getMinValue());
		cache_edt->setText(sequence->getCache());
		increment_edt->setText(sequence->getIncrement());
	}
	else
	{
		cyclic_chk->setChecked(false);
		start_edt->setText("1");
		maximum_edt->setText(Sequence::MAX_POSITIVE_VALUE);
		minimum_edt->setText("0");
		cache_edt->setText("1");
		increment_edt->setText("1");
	}

	BaseObjectWidget::setAttributes(model,op_list,sequence,schema);
}

void SequenceWidget::applyConfiguration(void)
{
	try
	{
		Sequence *sequence=NULL;
		startConfiguration<Sequence>();

		sequence=dynamic_cast<Sequence *>(this->object);
		sequence->setCycle(cyclic_chk->isChecked());
		sequence->setValues(minimum_edt->text(), maximum_edt->text(), increment_edt->text(),
												 start_edt->text(), cache_edt->text());
		sequence->setOwnerColumn(dynamic_cast<Column *>(column_sel->getSelectedObject()));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

