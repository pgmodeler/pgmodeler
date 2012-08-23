#include "funcaowidget.h"
#include "parametrowidget.h"
extern ParametroWidget *parametro_wgt;
//***********************************************************
FuncaoWidget::FuncaoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_FUNCAO)
{
 try
 {
  QStringList tipos;
  QGridLayout *grid=NULL, *grid1=NULL;
  map<QString, vector<QWidget *> > mapa_campos;
  QFrame *frame=NULL;

  Ui_FuncaoWidget::setupUi(this);
  configurarLayouFormulario(funcao_grid, OBJETO_FUNCAO);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  //Aloca um destacador de código fonte para o campo de definição da função
  destaque_codigo=new DestaqueSintaxe(codigofonte_txt, true);

  //Aloca o widget de configuração de tipo de retorno simples
  tipo_ret=new TipoPgSQLWidget(this);

  /* Aloca a tabela que armazena as colunas da tabela de retorno.
     Esta tabela possui 2 colunas que são o Nome da coluna e o tipo de dado */
  tab_retorno=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                     TabelaObjetosWidget::BTN_ATUALIZAR_ITEM, true, this);
  tab_retorno->definirNumColunas(2);
  tab_retorno->definirRotuloCabecalho(trUtf8("Coluna"), 0);
  tab_retorno->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
  tab_retorno->definirRotuloCabecalho(trUtf8("Tipo"), 1);
  tab_retorno->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

   /* Aloca a tabela que armazena os parâmetros da função.
     Esta tabela possui 4 colunas que são:
     Nome do parâmetro, tipo do parâmetro, comportamento do parâmetro (IN/OUT),
     valor padrâo do parâmetro */
  tab_parametros=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                        TabelaObjetosWidget::BTN_ATUALIZAR_ITEM, true, this);
  tab_parametros->definirNumColunas(4);
  tab_parametros->definirRotuloCabecalho(trUtf8("Nome"),0);
  tab_parametros->definirIconeCabecalho(QPixmap(":/icones/icones/parameter.png"),0);
  tab_parametros->definirRotuloCabecalho(trUtf8("Tipo"),1);
  tab_parametros->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);
  tab_parametros->definirRotuloCabecalho(trUtf8("IN/OUT"),2);
  tab_parametros->definirRotuloCabecalho(trUtf8("Valor Padrão"),3);

  grid=new QGridLayout;
  grid->addWidget(tab_parametros,0,0,1,1);
  grid->setContentsMargins(2,2,2,2);
  funcaowidget_twg->widget(1)->setLayout(grid);

  grid=dynamic_cast<QGridLayout *>(funcaowidget_twg->widget(0)->layout());
  grid->addWidget(tipo_ret, grid->count(), 0, 1, 4);
  grid->addWidget(tabela_ret_gb, grid->count()-1, 0, 1, 4);

  grid1=new QGridLayout;
  grid1->addWidget(tab_retorno, 0, 0, 1, 1);
  grid1->setContentsMargins(2,2,2,2);
  tabela_ret_gb->setLayout(grid1);
  tabela_ret_gb->setVisible(false);

  //Define os campos exclusivos para cada versão
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_84)].push_back(tabela_rb);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_84)].push_back(func_janela_lbl);

  //Gera o frame de alerta
  frame=gerarFrameAlertaVersao(mapa_campos);
  grid->addWidget(frame, grid->count()+1, 0, 1, 0);
  frame->setParent(funcaowidget_twg->widget(0));

  janela_pai->setMinimumSize(600, 660);

  //Configura o combo de tipos de segurança da função
  TipoSeguranca::obterTipos(tipos);
  seguranca_cmb->addItems(tipos);

  //Configura o combo de tipo da função
  TipoFuncao::obterTipos(tipos);
  tipo_func_cmb->addItems(tipos);

  //Configura o combo de tipo de comportamento da função
  TipoComportamento::obterTipos(tipos);
  comportamento_cmb->addItems(tipos);

  connect(simples_rb, SIGNAL(clicked(bool)), this, SLOT(alternarTiposRetorno(void)));
  connect(conjunto_rb, SIGNAL(clicked(bool)), this, SLOT(alternarTiposRetorno(void)));
  connect(tabela_rb, SIGNAL(clicked(bool)), this, SLOT(alternarTiposRetorno(void)));
  connect(linguagem_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selecionarLinguagem(void)));

  connect(tab_parametros, SIGNAL(s_linhaAdicionada(int)), this, SLOT(exibirFormParametro()));
  connect(tab_parametros, SIGNAL(s_linhaEditada(int)), this, SLOT(exibirFormParametro()));
  connect(tab_retorno, SIGNAL(s_linhaAdicionada(int)), this, SLOT(exibirFormParametro()));
  connect(tab_retorno, SIGNAL(s_linhaEditada(int)), this, SLOT(exibirFormParametro()));
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void FuncaoWidget::manipularParametro(int res)
{
 int qtd_lin, lin;
 QString str_aux;
 TabelaObjetosWidget *tabela=NULL;

 /* Seleciona a tabela a ser manipulado o parâmetro de
  * acordo com a visibilidade da mesma */
 if(tab_parametros->isVisible())
  //Seleciona a tabela de parâmetros caso esta esteja visível (em foco)
  tabela=tab_parametros;
 else
  //Seleciona a tabela de retorno caso esta esteja visível (em foco)
  tabela=tab_retorno;

 //Obtém a quantidade de linhas da tabela selecionada
 qtd_lin=tabela->obterNumLinhas();

 //Caso o usuário clique no botão 'aplicar' da janela de conf. de parâmetro
 if(res==QDialog::Accepted)
 {
  //Obtém a linha selecionada da tabela.
  lin=tabela->obterLinhaSelecionada();

  /* Caso o índice da linha seja negativo indica que a linha em
   questão está vazia, ou seja, o usuário não está editando
   uma linha existente e sim adicionando uma nova, sendo assim
   a linha a ser considerada na tabela sempre será a última recentemente
   incluída */
  if(lin < 0) lin=qtd_lin-1;

  /* Exibe os dados do parâmetro configurado no formulário de prâmetros
     na tabela selecionada na linha selecionada */
  exibirDadosParametro(parametro_wgt->obterParametro(), tabela, lin);
 }
 //Caso o usuário clique no botão 'cancelar' da janela de conf. de parâmetro
 else if(res==QDialog::Rejected)
 {
  //Remove a última linha da tabela quando se tratar de adição de novo parâmetro
  if(qtd_lin > 0 && tabela->obterTextoCelula(qtd_lin-1,0).isEmpty())
   tabela->removerLinha(qtd_lin-1);
 }
}
//----------------------------------------------------------
void FuncaoWidget::exibirFormParametro(void)
{
 QObject *obj_sender=sender();
 TabelaObjetosWidget *tabela=NULL;
 Parametro param_aux;
 int idx_lin;

 /* O formulário de conf. de parâmetro só será exibido
   se o sender for uma das tabelas de parâmetros */
 if(obj_sender==tab_parametros || obj_sender==tab_retorno)
 {
  tabela=dynamic_cast<TabelaObjetosWidget *>(obj_sender);

  /* Desabilita alguns campos do formulário de parâmetro quando
    se tratar de um parametro da tabela de retorno da função,
    pois são atributos não usados na declaração SQL */
  parametro_wgt->param_in_chk->setEnabled(obj_sender==tab_parametros);
  parametro_wgt->param_out_chk->setEnabled(obj_sender==tab_parametros);
  parametro_wgt->valorpadrao_edt->setEnabled(obj_sender==tab_parametros);

  //Obtém a linha selecionada da tabela
  idx_lin=tabela->obterLinhaSelecionada();

  /* Caso haja uma linha selecionada obtém o parâmetro daquela linha
     para que seus dados sejam exibidos no formulário de manipulação
     de parâmetro */
  if(idx_lin >= 0 && !tabela->obterTextoCelula(idx_lin, 0).isEmpty())
   param_aux=obterParametro(tabela, idx_lin);

  //Exibe o formulário de parâmetros
  parametro_wgt->definirAtributos(param_aux, modelo);
  parametro_wgt->show();
 }
}
//----------------------------------------------------------
Parametro FuncaoWidget::obterParametro(TabelaObjetosWidget *tab, unsigned idx_lin)
{
 Parametro param;
 QString str_aux;

 if(tab)
 {
  try
  {
   /* Configura o nome do parâmetro com o texto da coluna de nome
      da linha especificada da tabela */
   param.definirNome(tab->obterTextoCelula(idx_lin,0));

   //Configura o tipo do parâmetro com o tipo armazenado na linha atual da tabela
   param.definirTipo(tab->obterDadoLinha(idx_lin).value<TipoPgSQL>());

   //Obtendo informações específicas quando se trata da tabela de parâmetros
   if(tab==tab_parametros)
   {
    //Marcando se o parâmetro é de entrada/saída de acordo com o texto da coluna da tabela
    str_aux=tab->obterTextoCelula(idx_lin, 2);
    param.definirEntrada(str_aux.contains("IN"));
    param.definirSaida(str_aux.contains("OUT"));

    //Configura o valor padrâo do parâmetro
    param.definirValorPadrao(tab->obterTextoCelula(idx_lin,3));
   }
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }

 return(param);
}
//----------------------------------------------------------
void FuncaoWidget::exibirDadosParametro(Parametro param, TabelaObjetosWidget *tab, unsigned idx_lin)
{
 if(tab)
 {
  QString str_aux;

  /* Para as duas tabelas (tab_parametros ou tab_retorno) as duas primeiras colunas
     são destinadas, respectivamente, ao nome e tipo do parâmetro */
  tab->definirTextoCelula(QString::fromUtf8(param.obterNome()),idx_lin,0);
  tab->definirTextoCelula(QString::fromUtf8(*param.obterTipo()),idx_lin,1);

  /* Armazena na linha da tabela uma cópia do tipo do parâmetro.
      Isso é usado pelo método obterParametro() quando o usuário
      solicita obter um parâmetro a partir de um a linha da tabela */
  tab->definirDadoLinha(QVariant::fromValue<TipoPgSQL>(param.obterTipo()), idx_lin);

  /* Caso a tabela passada seja a de parâmetro configura as demais
     colunas com os demais atributos do parâmetro */
  if(tab==tab_parametros)
  {
   //Exibe se o parâmetro é de entrada/saída
   if(param.parametroEntrada()) str_aux="IN";
   if(param.parametroSaida()) str_aux+="OUT";
   tab->definirTextoCelula(str_aux,idx_lin,2);

   //Exibe o valor padrão do parâmetro
   tab->definirTextoCelula(QString::fromUtf8(param.obterValorPadrao()),idx_lin,3);
  }
 }
}
//----------------------------------------------------------
void FuncaoWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Funcao *funcao)
{
 vector<ObjetoBase *> linguagens;
 Linguagem *ling=NULL;
 QStringList lista;
 unsigned qtd=0, i;
 Parametro param;
 QString str_aux;
 TipoPgSQL tipo_aux;

 connect(parametro_wgt, SIGNAL(finished(int)), this, SLOT(manipularParametro(int)));

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, funcao);

//Obtém todas as linguagens criadas no modelo para armazená-las num combo
 linguagens=modelo->obterObjetos(OBJETO_LINGUAGEM);

 //Caso existam linguagens insere-as numa QStringList para ordená-las alfabeticamente
 while(!linguagens.empty())
 {
  ling=dynamic_cast<Linguagem *>(linguagens.back());
  linguagens.pop_back();
  lista.append(ling->obterNome());
 }

 //Ordena e insere as linguagens num combobox
 lista.sort();
 linguagem_cmb->addItems(lista);

 if(funcao)
 {
  tipo_aux=funcao->obterTipoRetorno();

  //Seleciona no combobox a linguagem configurada para a função
  linguagem_cmb->setCurrentIndex(linguagem_cmb->findText(funcao->obterLinguagem()->obterNome()));

  //Seleciona no combobox o tipo da função
  tipo_func_cmb->setCurrentIndex(tipo_func_cmb->findText(~funcao->obterTipoFuncao()));

  //Marca o checkbox se a função é janela ou não de acordo com o configurado na instância de função
  func_janela_chk->setChecked(funcao->funcaoJanela());

  //Exibe o custo de execução da função no formulário
  custo_exec_spb->setValue(funcao->obterCustoExecucao());

  //Exibe a quantidade de linhas retornadas pela função
  linhas_ret_sbp->setValue(funcao->obterQuantidadeLinhas());

  //Seleciona no combo o comportamento da função de acordo com o configurado na instância
  comportamento_cmb->setCurrentIndex(comportamento_cmb->findText(~funcao->obterTipoComportamento()));

  //Seleciona no combo tipo de segurança da função de acordo com o configurado na instância
  seguranca_cmb->setCurrentIndex(seguranca_cmb->findText(~funcao->obterTipoSeguranca()));

  //Configura no formulário o tipo de retorno da função
  //Caso a função retorne um conjunto de dados marca o radiobox respecitivo
  if(funcao->retornaSetOf())
   conjunto_rb->setChecked(true);
  /* Caso a função retorna uma tabela marca o radiobox respectivo isso
     também faz com que o widget que armazena os detalhes da tabela de
     retorno seja exibido */
  else if(funcao->retornaTabela())
   tabela_rb->setChecked(true);
  else
   simples_rb->setChecked(true);

  //Obtém o número de parâmetros da função
  qtd=funcao->obterNumParams();


  /* Bloqueia os sinais dos widgets de tabela para evitar o disparo
     de sinais e execução de slots. Isso é feito pois no preenchimento do
     formulário através do método definirAtributos() não é necessário
     executar os slots ligados a estas duas tabelas, isso deve acontecer
     somente quando o usuário interagir (via mouse ou teclado) com
     esses widgets */
  tab_parametros->blockSignals(true);
  tab_retorno->blockSignals(true);

  //Preenche a tabela de parâmetros com os parâmetros da função
  for(i=0; i < qtd; i++)
  {
   tab_parametros->adicionarLinha();
   param=funcao->obterParametro(i);
   exibirDadosParametro(param,tab_parametros,i);
  }
  tab_parametros->limparSelecao();

  /* Obtém a quantidade de colunas na tabela de retorno.
     caso exista algum a tabela é exibida e as colunas
     da tabela de retorno da função exibidas */
  qtd=funcao->obterNumTiposRetTabela();
  if(qtd > 0)
  {
   tabela_ret_gb->setVisible(true);
   tipo_ret->setVisible(false);

   //Exibindo as colunas da tabela de retorno no widget
   for(i=0; i < qtd; i++)
   {
    tab_retorno->adicionarLinha();
    param=funcao->obterTipoRetTabela(i);
    exibirDadosParametro(param,tab_retorno,i);
   }
  }
  tab_retorno->limparSelecao();


  //Caso a função referencie (esteja definida) uma biblioteca
  if(!funcao->obterBiblioteca().isEmpty())
  {
   //Exibe o nome do símbolo e caminho para a biblioteca no sistema de arquivos
   simbolo_edt->setText(funcao->obterSimbolo());
   biblioteca_edt->setText(funcao->obterBiblioteca());
  }
  //Caso a função esteja definida por um código fonte
  else
  {
   //Exibe o código fonte da função na caixa de destaque de código fonte
   codigofonte_txt->setPlainText(QString::fromUtf8(funcao->obterCodigoFonte()));
  }

  /* Desbloqueia os sinais dos widgets de tabela para que o usuário possa
     interagir com estes */
  tab_parametros->blockSignals(false);
  tab_retorno->blockSignals(false);
 }

 tipo_ret->definirAtributos(tipo_aux, modelo);
}
//----------------------------------------------------------
void FuncaoWidget::hideEvent(QHideEvent *evento)
{
 linguagem_cmb->clear();
 tab_parametros->removerLinhas();
 tab_retorno->removerLinhas();
 codigofonte_txt->clear();
 simbolo_edt->clear();
 biblioteca_edt->clear();
 funcaowidget_twg->setCurrentIndex(0);
 disconnect(parametro_wgt,NULL, this, NULL);
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void FuncaoWidget::alternarTiposRetorno(void)
{
 QObject *obj_sender=sender();
 tabela_ret_gb->setVisible(obj_sender==tabela_rb);
 tipo_ret->setVisible(!tabela_ret_gb->isVisible());
}
//----------------------------------------------------------
void FuncaoWidget::selecionarLinguagem(void)
{
 bool ling_c;

 ling_c=(linguagem_cmb->currentText()==~TipoLinguagem(TipoLinguagem::c));
 codigofonte_frm->setVisible(!ling_c);
 biblioteca_frm->setVisible(ling_c);

 if(!ling_c)
 {
  try
  {
   destaque_codigo->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                         AtributosGlobais::SEP_DIRETORIO +
                                         linguagem_cmb->currentText() +
                                         AtributosGlobais::SUF_ARQ_DESTAQUE +
                                         AtributosGlobais::EXT_CONFIGURACAO);
  }
  catch(Excecao &e)
  {
   destaque_codigo->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                         AtributosGlobais::SEP_DIRETORIO +
                                         AtributosGlobais::CONF_DESTAQUE_SQL +
                                         AtributosGlobais::EXT_CONFIGURACAO);
  }

  destaque_codigo->rehighlight();
 }
}
//----------------------------------------------------------
void FuncaoWidget::validarFuncaoConfigurada(void)
{
 vector<ObjetoBase *> *lista_obj;
 vector<ObjetoBase *>::iterator itr, itr_end;
 ConversaoCodificacao *conv_cod=NULL;
 ConversaoTipo *conv_tipo=NULL;
 FuncaoAgregacao *func_ag=NULL;
 Gatilho *gatilho=NULL;
 Linguagem *ling=NULL;
 Operador *oper=NULL;
 Tipo *tipo=NULL;
 Tabela *tab=NULL;
 Funcao *funcao=NULL;
 ObjetoBase *objeto=NULL;
 unsigned i, i1, qtd;

 //Vetor de tipos de objetos os quais referenciam direta/indiretamente a função
 TipoObjetoBase tipos[7]={ OBJETO_CONV_CODIFICACAO, OBJETO_CONV_TIPO,
                           OBJETO_FUNC_AGREGACAO, OBJETO_TABELA,
                           OBJETO_LINGUAGEM, OBJETO_OPERADOR, OBJETO_TIPO };

 //Obtém a referência para a função recém configurada
 funcao=dynamic_cast<Funcao *>(this->objeto);

 try
 {
  for(i=0; i < 7; i++)
  {
   //Obtém a lista de objetos do tipo atual
   lista_obj=modelo->obterListaObjetos(tipos[i]);
   itr=lista_obj->begin();
   itr_end=lista_obj->end();

   while(itr!=itr_end)
   {
    objeto=(*itr);
    itr++;

    /* A validação da função se dá da seguinte forma:
       > Para cada tipo de objeto constante no vetor 'tipos'
         é obtida a lista de objetos.
         Caso haja elementos nesta lista, a função é atribuída
         a cada elemento e internamente nestes elementos a função
         e validada de acordo com o exigido por cada classe.

         Caso a função seja inválida as instâncias dispararão
         exceções acusando o erro isso é o suficiente para verificar
         a validade da função em relação aos objetos que a referenciam. */
    if(tipos[i]==OBJETO_CONV_CODIFICACAO)
    {
     conv_cod=dynamic_cast<ConversaoCodificacao *>(objeto);
     if(conv_cod->obterFuncaoConversao()==funcao)
      conv_cod->definirFuncaoConversao(funcao);
    }
    else if(tipos[i]==OBJETO_CONV_TIPO)
    {
     conv_tipo=dynamic_cast<ConversaoTipo *>(objeto);
     if(conv_tipo->obterFuncaoConversao()==funcao)
      conv_tipo->definirFuncaoConversao(funcao);
    }
    else if(tipos[i]==OBJETO_FUNC_AGREGACAO)
    {
     func_ag=dynamic_cast<FuncaoAgregacao *>(objeto);
     if(func_ag->obterFuncao(FuncaoAgregacao::FUNCAO_FINAL)==funcao)
      func_ag->definirFuncao(FuncaoAgregacao::FUNCAO_FINAL, funcao);
     else if(func_ag->obterFuncao(FuncaoAgregacao::FUNCAO_TRANSICAO)==funcao)
      func_ag->definirFuncao(FuncaoAgregacao::FUNCAO_TRANSICAO, funcao);
    }
    /* A única validação diferente é para gatilhos, como gatilhos são objetos,
      internos das tabelas, primeiro são obtidas as tabelas e em seguida
      com cada instância a função será validada com os gatilhos constantes
      nas tabelas */
    else if(tipos[i]==OBJETO_TABELA)
    {
     tab=dynamic_cast<Tabela *>(objeto);
     qtd=tab->obterNumGatilhos();

     for(i1=0; i1 < qtd; i1++)
     {
      gatilho=tab->obterGatilho(i1);
      if(gatilho->obterFuncao()==funcao)
       gatilho->definirFuncao(funcao);
     }
    }
    else if(tipos[i]==OBJETO_LINGUAGEM)
    {
     ling=dynamic_cast<Linguagem *>(objeto);

     for(i1=Linguagem::FUNC_VALIDATOR; i1 <= Linguagem::FUNC_INLINE; i1++)
     {
      if(ling->obterFuncao(i1)==funcao)
       ling->definirFuncao(funcao, i1);
     }
    }
    else if(tipos[i]==OBJETO_OPERADOR)
    {
     oper=dynamic_cast<Operador *>(objeto);
     for(i1=Operador::FUNC_OPERADOR; i1 <= Operador::FUNC_RESTRICAO; i1++)
     {
      if(oper->obterFuncao(i1)==funcao)
       oper->definirFuncao(funcao, i1);
     }
    }
    else if(tipos[i]==OBJETO_TIPO)
    {
     tipo=dynamic_cast<Tipo *>(objeto);
     if(tipo->obterConfiguracao()==Tipo::TIPO_BASE)
     {
      for(i1=Tipo::FUNCAO_INPUT; i1 <=Tipo::FUNCAO_ANALYZE; i1++)
      {
       if(tipo->obterFuncao(i1)==funcao)
        tipo->definirFuncao(i1, funcao);
      }
     }
    }
   }
  }
 }
 catch(Excecao &e)
 {
  //Caso alguma validação acima dispare um erro o mesmo é redirecionado
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_CONFFUNCINVALIDAOBJ)
                .arg(objeto->obterNome(true))
                .arg(objeto->obterNomeTipoObjeto()),
                ERR_PGMODELERUI_CONFFUNCINVALIDAOBJ,
                __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void FuncaoWidget::aplicarConfiguracao(void)
{
 try
 {
  Funcao *func=NULL;
  unsigned qtd, i;
  Parametro param;
  QString str_aux;

  //Inicia a configuração da função
  iniciarConfiguracao<Funcao>();

  //Faz a conversão do objeto editado (genérico) para o tipo função
  func=dynamic_cast<Funcao *>(this->objeto);

  //Atribui os valores básicos configurados no formulário para a função
  func->definirLinguagem(modelo->obterObjeto(linguagem_cmb->currentText(), OBJETO_LINGUAGEM));
  func->definirTipoFuncao(tipo_func_cmb->currentText());
  func->definirFuncaoJanela(func_janela_chk->isChecked());
  func->definirCustoExecucao(custo_exec_spb->value());
  func->definirQuantidadeLinhas(linhas_ret_sbp->value());
  func->definirTipoComportamento(comportamento_cmb->currentText());
  func->definirTipoSeguranca(seguranca_cmb->currentText());

  /* Remove todos os parâmetros da função para adicionar os que foram
     configurados no formulário */
  func->removerParametros();

  //Obtém a quantidade de parâmetros configurados no formulário
  qtd=tab_parametros->obterNumLinhas();

  for(i=0; i < qtd; i++)
  {
   /* Cria um parâmetro auxiliar e os configura de acordo com os valores
      do parâmetro atual (i) na tabela */
   param.definirNome(tab_parametros->obterTextoCelula(i,0));
   param.definirTipo(tab_parametros->obterDadoLinha(i).value<TipoPgSQL>());

   str_aux=tab_parametros->obterTextoCelula(i,2);
   param.definirEntrada(str_aux.indexOf("IN") >= 0);
   param.definirSaida(str_aux.indexOf("OUT") >= 0);

   param.definirValorPadrao(tab_parametros->obterTextoCelula(i,3));

   //Uma vez configurado esse parâmetro e inserido na função
   func->adicionarParametro(param);
  }


  /* Caso a linguagem selecionada no formulário for a C
     indica que a função está definida em uma biblioteca externa
     para tanto os valores dos campos biblioteca e simbolo
     serão atribuídos à função que está sendo configurada */
  if(linguagem_cmb->currentText()==~TipoLinguagem(TipoLinguagem::c))
  {
   func->definirBiblioteca(biblioteca_edt->text());
   func->definirSimbolo(simbolo_edt->text());
  }
  /* Caso a linguagem seja diferente de C atribui o código fonte
     definido no destacador de código do formulário */
  else
   func->definirCodigoFonte(codigofonte_txt->toPlainText());

  //Caso o tipo de retorno configurado no formulário seja simples ou conjunto
  if(simples_rb->isChecked() || conjunto_rb->isChecked())
  {
   /* Define o tipo de retorno da função obtendo o tipo definido no widget de
      configuração de tipos pgsql */
   func->definirTipoRetorno(tipo_ret->obterTipoPgSQL());

   /* Marca na função se a mesma retorna um conjunto de dados de acordo
      com o estado do radiobox 'conjunto_rb' */
   func->definirRetornaSetOf(conjunto_rb->isChecked());
  }
  //Caso a função retorna uma tabela, a mesma é atribuída à função
  else
  {
   func->removerTiposRetTabela();
   qtd=tab_retorno->obterNumLinhas();

   /* Adiciona uma coluna na tabela de retorno com os dados configurados na tabela
      respectiva do formulário */
   for(i=0; i<qtd; i++)
   {
    func->adicionarTipoRetTabela(tab_retorno->obterTextoCelula(i,0),
                                 tab_retorno->obterDadoLinha(i).value<TipoPgSQL>());
   }
  }

  //Finaliza a configuração da função
  ObjetoBaseWidget::aplicarConfiguracao();

  /* Executa a validação da função em relação aos objetos das classes
     ConversaoCodificacao, ConversaoTipo, ElemClasseOperadores, FuncaoAgregada,
     Gatilho, Linguagem, Operador, Tipo */
  validarFuncaoConfigurada();

  finalizarConfiguracao();
 }
 catch(Excecao &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelarConfiguracao();
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//***********************************************************
