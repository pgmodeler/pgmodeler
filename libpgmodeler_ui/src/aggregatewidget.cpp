#include "aggregatewidget.h"

AggregateWidget::AggregateWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_AGGREGATE)
{
	try
	{
		QGridLayout *grid=NULL;
		Ui_AggregateWidget::setupUi(this);
		QSpacerItem *spacer=NULL;
		QFrame *frame=NULL;

		initial_cond_hl=new SyntaxHighlighter(initial_cond_txt, false);
		initial_cond_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																			 GlobalAttributes::DIR_SEPARATOR +
																			 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																			 GlobalAttributes::CONFIGURATION_EXT);

		final_func_sel=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
		transition_func_sel=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
		sort_op_sel=new SeletorObjetoWidget(OBJ_OPERATOR, true, this);

		input_type=new TipoPgSQLWidget(this, trUtf8("Input Data Type"));
		state_type=new TipoPgSQLWidget(this, trUtf8("State Data Type"));

		input_types_tab=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
																						TabelaObjetosWidget::BTN_EDITAR_ITEM, true, this);
		input_types_tab->definirNumColunas(1);

		funcaoagregacao_grid->addWidget(final_func_sel,0,1,1,1);
		funcaoagregacao_grid->addWidget(transition_func_sel,1,1,1,1);
		funcaoagregacao_grid->addWidget(sort_op_sel,2,1,1,1);

		grid=new QGridLayout;
		grid->setContentsMargins(2,2,2,2);
		grid->addWidget(input_type,0,0);
		grid->addWidget(input_types_tab,1,0);
		state_input_types_twg->widget(0)->setLayout(grid);

		grid=new QGridLayout;
		spacer=new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

		grid->setContentsMargins(2,2,2,2);
		grid->addWidget(state_type,0,0);
		grid->addItem(spacer,1,0);
		state_input_types_twg->widget(1)->setLayout(grid);

		frame=generateInformationFrame(trUtf8("An aggregate function that accepts the types <em><strong>typeA</strong></em> and <em><strong>typeB</strong></em> as input types and which type of state is <em><strong>state_type</strong></em>, must obey the following rules: <br/><br/> <strong> &nbsp;&nbsp;&nbsp;• Final Function:</strong> <em>void final_function(<strong>state_type</strong>)</em><br/>  <strong> &nbsp;&nbsp;&nbsp;• Transition Function:</strong> <em><strong>state_type</strong> transition_function(<strong>state_type</strong>, <strong>typeA</strong>, <strong>typeB</strong>)</em>"));
		funcaoagregacao_grid->addWidget(frame, funcaoagregacao_grid->count()+1, 0, 1, 2);
		frame->setParent(this);

		configureFormLayout(funcaoagregacao_grid, OBJ_AGGREGATE);

		parent_form->setMinimumSize(645, 750);

		connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(input_types_tab, SIGNAL(s_linhaAdicionada(int)), this, SLOT(handleDataType(int)));
		connect(input_types_tab, SIGNAL(s_linhaAtualizada(int)), this, SLOT(handleDataType(int)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void AggregateWidget::hideEvent(QHideEvent *event)
{
	final_func_sel->removerObjetoSelecionado();
	transition_func_sel->removerObjetoSelecionado();
	sort_op_sel->removerObjetoSelecionado();
	input_types_tab->removerLinhas();
	initial_cond_txt->clear();
	BaseObjectWidget::hideEvent(event);
}

void AggregateWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Aggregate *aggregate)
{
	unsigned count, i;
	PgSQLType type;

	BaseObjectWidget::setAttributes(model,op_list,aggregate);

	input_type->definirAtributos(type, model);
	state_type->definirAtributos(type, model);
	final_func_sel->definirModelo(model);
	transition_func_sel->definirModelo(model);
	sort_op_sel->definirModelo(model);

	if(aggregate)
	{
		final_func_sel->definirObjeto(aggregate->getFunction(Aggregate::FINAL_FUNC));
		transition_func_sel->definirObjeto(aggregate->getFunction(Aggregate::TRANSITION_FUNC));
		sort_op_sel->definirObjeto(aggregate->getSortOperator());
		initial_cond_txt->setPlainText(QString::fromUtf8(aggregate->getInitialCondition()));

		input_types_tab->blockSignals(true);
		count=aggregate->getDataTypeCount();

		for(i=0; i < count; i++)
		{
			input_types_tab->adicionarLinha();
			type=aggregate->getDataType(i);
			input_types_tab->definirDadoLinha(QVariant::fromValue<PgSQLType>(type), i);
			input_types_tab->definirTextoCelula(QString::fromUtf8(*type),i,0);
		}
		input_types_tab->blockSignals(false);
		input_types_tab->limparSelecao();

		state_type->definirAtributos(aggregate->getStateType(), model);
	}
}

void AggregateWidget::handleDataType(int linha)
{
	PgSQLType type;

	type=input_type->obterTipoPgSQL();
	input_types_tab->definirDadoLinha(QVariant::fromValue<PgSQLType>(type), linha);
	input_types_tab->definirTextoCelula(QString::fromUtf8(*type),linha,0);
}

void AggregateWidget::applyConfiguration(void)
{
	try
	{
		Aggregate *aggregate=NULL;
		unsigned count, i;

		startConfiguration<Aggregate>();

		aggregate=dynamic_cast<Aggregate *>(this->object);
		aggregate->setInitialCondition(initial_cond_txt->toPlainText());
		aggregate->setStateType(state_type->obterTipoPgSQL());

		aggregate->removeDataTypes();
		count=input_types_tab->obterNumLinhas();

		for(i=0; i < count; i++)
			aggregate->addDataType(input_types_tab->obterDadoLinha(i).value<PgSQLType>());

		aggregate->setFunction(Aggregate::TRANSITION_FUNC, dynamic_cast<Function *>(transition_func_sel->obterObjeto()));
		aggregate->setFunction(Aggregate::FINAL_FUNC, dynamic_cast<Function *>(final_func_sel->obterObjeto()));
		aggregate->setSortOperator(dynamic_cast<Operator *>(sort_op_sel->obterObjeto()));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

