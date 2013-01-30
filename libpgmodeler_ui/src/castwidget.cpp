#include "castwidget.h"

CastWidget::CastWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_CAST)
{
	try
	{
		QFont font;
		QFrame *frame=NULL;

		Ui_CastWidget::setupUi(this);

		src_datatype=new TipoPgSQLWidget(this, trUtf8("Source data type"));
		trg_datatype=new TipoPgSQLWidget(this, trUtf8("Target data type"));
		conv_func_sel=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);

		convtipo_grid->addWidget(conv_func_sel,1,1,1,3);
		convtipo_grid->addWidget(src_datatype,2,0,1,4);
		convtipo_grid->addWidget(trg_datatype,3,0,1,4);

		configureFormLayout(convtipo_grid, OBJ_CAST);

		name_edt->setReadOnly(true);
		font=name_edt->font();
		font.setItalic(true);
		name_edt->setFont(font);

		frame=generateInformationFrame(trUtf8("The function to be assigned to a cast from <em><strong>typeA</strong></em> to <em><strong>typeB</strong></em> must have the following signature: <em><strong>typeB</strong> function(<strong>typeA</strong>, integer, boolean)</em>."));
		convtipo_grid->addWidget(frame, convtipo_grid->count()+1, 0, 1, 0);
		frame->setParent(this);

		connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		parent_form->setMinimumSize(530, 500);
		parent_form->setMaximumHeight(500);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void CastWidget::hideEvent(QHideEvent *event)
{
	input_output_chk->setChecked(false);
	implicit_rb->setChecked(true);
	conv_func_sel->removerObjetoSelecionado();
	BaseObjectWidget::hideEvent(event);
}

void CastWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Cast *cast)
{
	PgSQLType src_type, trg_type;

	BaseObjectWidget::setAttributes(model, op_list, cast);
	conv_func_sel->definirModelo(model);

	if(cast)
	{
		src_type=cast->getDataType(Cast::SRC_TYPE);
		trg_type=cast->getDataType(Cast::DST_TYPE);

		conv_func_sel->definirObjeto(cast->getCastFunction());
		input_output_chk->setChecked(cast->isInOut());
		implicit_rb->setChecked(cast->getCastType()==Cast::IMPLICIT);
		assignment_rb->setChecked(!implicit_rb->isChecked());
	}

	src_datatype->definirAtributos(src_type,model);
	trg_datatype->definirAtributos(trg_type,model);
}

void CastWidget::applyConfiguration(void)
{
	try
	{
		Cast *cast=NULL;

		startConfiguration<Cast>();

		cast=dynamic_cast<Cast *>(this->object);
		cast->setDataType(Cast::SRC_TYPE, src_datatype->obterTipoPgSQL());
		cast->setDataType(Cast::DST_TYPE, trg_datatype->obterTipoPgSQL());
		cast->setInOut(input_output_chk->isChecked());

		if(implicit_rb->isChecked())
			cast->setCastType(Cast::IMPLICIT);
		else
			cast->setCastType(Cast::ASSIGNMENT);

		cast->setCastFunction(dynamic_cast<Function*>(conv_func_sel->obterObjeto()));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

