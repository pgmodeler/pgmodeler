#include "operadorwidget.h"

OperadorWidget::OperadorWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_OPERATOR)
{
	try
	{
		QGridLayout *grid=NULL;
		unsigned i;
		map<QString, vector<QWidget *> > mapa_campos;
		QFrame *frame=NULL;

		Ui_OperadorWidget::setupUi(this);

		//Alocando os widget de configuração de argumento do operador
		tipo_args[0]=NULL;
		tipo_args[0]=new TipoPgSQLWidget(this, trUtf8("Right Argument Type"));
		tipo_args[1]=NULL;
		tipo_args[1]=new TipoPgSQLWidget(this, trUtf8("Left Argument Type"));

		grid=new QGridLayout;
		grid->setContentsMargins(2,2,2,2);
		grid->addWidget(tipo_args[0],0,0);
		grid->addWidget(tipo_args[1],1,0);

		//Gera o frame de informação sobre a criação de operador unário
		frame=generateInformationFrame(trUtf8("To create a unary operator it is necessary to specify as <strong><em>'any'</em></strong> one of its arguments. Additionally, the function that defines the operator must have only one parameter and this, in turn, must have the same data type of the the argument of unary operator."));
		grid->addWidget(frame, 2, 0);
		atributos_twg->widget(0)->setLayout(grid);

		//Alocando os seletores de função
		grid=dynamic_cast<QGridLayout *>(atributos_twg->widget(1)->layout());
		for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICTION; i++)
		{
			sel_funcoes[i]=NULL;
			sel_funcoes[i]=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
			grid->addWidget(sel_funcoes[i],i,1,1,1);
		}

		//Alocando os seletores de operador
		grid=dynamic_cast<QGridLayout *>(atributos_twg->widget(2)->layout());
		for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_GREATER; i++)
		{
			sel_operadores[i]=NULL;
			sel_operadores[i]=new SeletorObjetoWidget(OBJ_OPERATOR, true, this);
			grid->addWidget(sel_operadores[i],i,1,1,1);
		}

		//Gera o frame de alerta com os campos que são exclusivos para versões do postgresql
		mapa_campos[generateVersionsInterval(UNTIL_VERSION, SchemaParser::PGSQL_VERSION_82)].push_back(op_ordenacao1_lbl);
		mapa_campos[generateVersionsInterval(UNTIL_VERSION, SchemaParser::PGSQL_VERSION_82)].push_back(op_ordenacao2_lbl);
		mapa_campos[generateVersionsInterval(UNTIL_VERSION, SchemaParser::PGSQL_VERSION_82)].push_back(op_menorque_lbl);
		mapa_campos[generateVersionsInterval(UNTIL_VERSION, SchemaParser::PGSQL_VERSION_82)].push_back(op_maiorque_lbl);
		frame=generateVersionWarningFrame(mapa_campos);
		grid->addWidget(frame, grid->count()+1, 0, 1, 0);
		frame->setParent(atributos_twg->widget(2));

		configureFormLayout(operador_grid, OBJ_OPERATOR);

		connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		parent_form->resize(530, 560);
		parent_form->setMinimumWidth(530);
		parent_form->setMinimumHeight(560);
		//janela_pai->setMaximumHeight(560);
	}
	catch(Exception &e)
	{
		//Redireciona o erro
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void OperadorWidget::hideEvent(QHideEvent *evento)
{
	unsigned i;

	//Desmarca os checkboxes
	hashes_chk->setChecked(false);
	merges_chk->setChecked(false);

	//Zera os valores do seletores de operador
	for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICTION; i++)
		sel_funcoes[i]->removerObjetoSelecionado();

	//Zera os valores do seletores de função
	for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_GREATER; i++)
		sel_operadores[i]->removerObjetoSelecionado();

	atributos_twg->setCurrentIndex(0);
	BaseObjectWidget::hideEvent(evento);
}

void OperadorWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Operator *operador)
{
	unsigned i;
	PgSQLType tipo_esq, tipo_dir;

	//Preenchendo os campos básicos do formulário com os atributos do operador
	BaseObjectWidget::setAttributes(modelo,lista_op,operador);

	//Define o modelo de objetos usado pelos seletores de função e operador
	for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICTION; i++)
		sel_funcoes[i]->definirModelo(modelo);

	for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_GREATER; i++)
		sel_operadores[i]->definirModelo(modelo);

	/* Caso o operador esteja alocado configura os atributos do formulário
		com os valores preenchidos na instância do operador */
	if(operador)
	{
		hashes_chk->setChecked(operador->isHashes());
		merges_chk->setChecked(operador->isMerges());

		//Preenche os widget seletores de função com as funções usadas pelo operador
		for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICTION; i++)
			sel_funcoes[i]->definirObjeto(operador->getFunction(i));

		//Preenche os widget seletores de operador com os operadores usadas pelo operador
		for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_GREATER; i++)
			sel_operadores[i]->definirObjeto(operador->getOperator(i));

		//Obtém os tipos dos argumentos do operador
		tipo_esq=operador->getArgumentType(Operator::LEFT_ARG);
		tipo_dir=operador->getArgumentType(Operator::RIGHT_ARG);
	}

	tipo_args[0]->definirAtributos(tipo_esq, modelo);
	tipo_args[1]->definirAtributos(tipo_dir, modelo);
}
//---------------------------------------------------------
void OperadorWidget::applyConfiguration(void)
{
	try
	{
		unsigned i;
		Operator *operador=NULL;
		startConfiguration<Operator>();

		//Obtém a referêni   sequência que está sendo editada/criada
		operador=dynamic_cast<Operator *>(this->object);

		/* Atribui os valores configurados no formulári  instância do
		 operador que está sendo configurado */
		operador->setHashes(hashes_chk->isChecked());
		operador->setMerges(merges_chk->isChecked());

		//Atribuindo os tipos de argumentos configurados no formulário ao operador
		for(i=Operator::LEFT_ARG; i <= Operator::RIGHT_ARG; i++)
			operador->setArgumentType(tipo_args[i]->obterTipoPgSQL(), i);

		//Atribuindo as funções selecionadas no formulário ao operador
		for(i=Operator::FUNC_OPERATOR; i <= Operator::FUNC_RESTRICTION; i++)
			operador->setFunction(dynamic_cast<Function *>(sel_funcoes[i]->obterObjeto()), i);

		//Atribuindo os operadores selecionados no formulário ao operador
		for(i=Operator::OPER_COMMUTATOR; i <= Operator::OPER_GREATER; i++)
			operador->setOperator(dynamic_cast<Operator *>(sel_operadores[i]->obterObjeto()), i);

		//Finaliza a configuração
		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		/* Cancela a configuração o objeto removendo a ultima operação adicionada
		 referente ao objeto editado/criado e desaloca o objeto
		 caso o mesmo seja novo */
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

