#include "dominiowidget.h"

DominioWidget::DominioWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_DOMAIN)
{
	try
	{
		Ui_DominioWidget::setupUi(this);

		//Cria um destacador de sintaxe no campo de expressão de checagem do domínio
		destaque_expr=NULL;
		destaque_expr=new SyntaxHighlighter(expr_checagem_txt, false);
		destaque_expr->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																		 GlobalAttributes::DIR_SEPARATOR +
																		 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																		 GlobalAttributes::CONFIGURATION_EXT);

		tipo_dominio=NULL;
		tipo_dominio=new TipoPgSQLWidget(this);
		dominio_grid->addWidget(tipo_dominio,4,0,1,2);

		configureFormLayout(dominio_grid, OBJ_DOMAIN);
		connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		parent_form->setMinimumSize(530, 450);
		parent_form->setMaximumHeight(450);
	}
	catch(Exception &e)
	{
		//Redireciona o erro
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DominioWidget::hideEvent(QHideEvent *evento)
{
	valor_padrao_edt->clear();
	expr_checagem_txt->clear();
	nome_rest_edt->clear();
	nao_nulo_chk->setChecked(false);
	BaseObjectWidget::hideEvent(evento);
}

void DominioWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Domain *dominio)
{
	PgSQLType tipo;

	//Define os atributos do formulários e da janela pai
	BaseObjectWidget::setAttributes(modelo, lista_op, dominio);

	//Caso o domínio passado esteja alocado
	if(dominio)
	{
		//Preenche o formulário com os atributos do domínio
		tipo=dominio->getType();
		valor_padrao_edt->setText(QString::fromUtf8(dominio->getDefaultValue()));
		expr_checagem_txt->setPlainText(QString::fromUtf8(dominio->getExpression()));
		nome_rest_edt->setText(QString::fromUtf8(dominio->getConstraintName()));
		nao_nulo_chk->setChecked(dominio->isNotNull());
	}

	//Marca o tipo do domínio no widget de tipos pgsql
	tipo_dominio->definirAtributos(tipo, modelo);
}

void DominioWidget::applyConfiguration(void)
{
	try
	{
		Domain *dominio=NULL;
		startConfiguration<Domain>();

		//Obtém a referência ao domínio que está sendo editado/criado
		dominio=dynamic_cast<Domain *>(this->object);

		//Configura os atributos do mesmo com os valores definidos no formulário
		dominio->setType(tipo_dominio->obterTipoPgSQL());
		dominio->setDefaultValue(valor_padrao_edt->text());
		dominio->setExpression(expr_checagem_txt->toPlainText());
		dominio->setConstraintName(nome_rest_edt->text());
		dominio->setNotNull(nao_nulo_chk->isChecked());

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

