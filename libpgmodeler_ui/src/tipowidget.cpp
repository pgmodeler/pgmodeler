#include "tipowidget.h"
#include "parametrowidget.h"

extern ParametroWidget *parametro_wgt;
//***********************************************************
TipoWidget::TipoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_TIPO)
{
 try
 {
  QGridLayout *grid=NULL;
  QFrame *frame=NULL;
  QSpacerItem *spacer=NULL;
  map<QString, vector<QWidget *> > mapa_campos;
  QStringList lista;
  unsigned i;

  Ui_TipoWidget::setupUi(this);
  configurarLayouFormulario(tipo_grid, OBJETO_TIPO);

  //Aloca os widgets de configuração de tipos
  tipo_copia=NULL;
  tipo_copia=new TipoPgSQLWidget(this, trUtf8("Tipo Cópia"));
  tipo_elemento=NULL;
  tipo_elemento=new TipoPgSQLWidget(this, trUtf8("Tipo Elemento"));

  //Cria um layout e insere os widgets de tipo
  grid=dynamic_cast<QGridLayout *>(atrib_base_twg->widget(0)->layout());
  spacer=new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
  grid->addWidget(tipo_copia,6,0,1,0);
  grid->addWidget(tipo_elemento,7,0,1,0);
  grid->addItem(spacer,8,0);

  //Aloca os seletores de funções e os insere do layout da aba de funções
  grid=dynamic_cast<QGridLayout *>(atrib_base_twg->widget(1)->layout());
  for(i=Tipo::FUNCAO_INPUT; i <= Tipo::FUNCAO_ANALYZE; i++)
  {
   sel_funcoes[i]=NULL;
   sel_funcoes[i]=new SeletorObjetoWidget(OBJETO_FUNCAO, true, this);
   grid->addWidget(sel_funcoes[i],i,1,1,1);
  }

  //Aloca a tabela de enumerações e a insere no layout do grupo de atributos de enumerações
  tab_enumeracoes=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                         TabelaObjetosWidget::BTN_EDITAR_ITEM, false, this);
  grid=dynamic_cast<QGridLayout *>(enumeracoes_gb->layout());
  grid->addWidget(tab_enumeracoes,1,0,1,2);
  enumeracoes_gb->setVisible(false);

  //Aloca a tabela de atributos e a insere no layout do grupo de atributos de tipo composto
  tab_atributos=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                       TabelaObjetosWidget::BTN_ATUALIZAR_ITEM, false, this);
  tab_atributos->definirNumColunas(2);
  tab_atributos->definirRotuloCabecalho(trUtf8("Nome"),0);
  tab_atributos->definirIconeCabecalho(QPixmap(":/icones/icones/uid.png"),0);
  tab_atributos->definirRotuloCabecalho(trUtf8("Tipo"),1);
  tab_atributos->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

  grid=new QGridLayout;
  grid->setContentsMargins(2,2,2,2);
  grid->addWidget(tab_atributos,0,0);
  atributos_gb->setLayout(grid);
  atributos_gb->setVisible(false);

  //Configura um alerta de versão com os campos específicos das versões
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_83)].push_back(enumeracao_rb);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_84)].push_back(categoria_lbl);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_84)].push_back(preferido_lbl);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_84)].push_back(tipo_copia);
  frame=gerarFrameAlertaVersao(mapa_campos);
  tipo_grid->addWidget(frame, tipo_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  //Gera o frame de informação
  grid=dynamic_cast<QGridLayout *>(atrib_base_twg->widget(1)->layout());
  frame=gerarFrameInformacao(trUtf8("As funções a serem atribuídas ao tipo devem ser todas escritas em linguagem C e possuirem, respectivamente, as seguintes assinaturas:<br/>\
  <table>\
   <tr>\
    <td><strong>INPUT:</strong> <em>any funcao(cstring, oid, integer)</em></td>\
    <td><strong>OUTPUT:</strong> <em>cstring funcao(any)</em></td>\
   </tr>\
   <tr>\
    <td><strong>SEND:</strong> <em>byta funcao(any)</em></td>\
    <td><strong>RECV:</strong> <em>any funcao(internal, oid, integer)</em></td>\
   </tr>\
   <tr>\
    <td><strong>TPMOD_IN:</strong> <em>integer funcao(cstring[])</em></td>\
    <td><strong>TPMOD_OUT:</strong> <em>cstring funcao(integer)</em></td>\
   </tr>\
   <tr>\
    <td><strong>ANALYZE:</strong> <em>boolean funcao(internal)</em></td> \
   <tr>\
  </table>"));
  grid->addWidget(frame, grid->count()+1, 0, 1, 0);
  frame->setParent(atrib_base_twg->widget(1));

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
  connect(tipo_base_rb, SIGNAL(toggled(bool)), this, SLOT(selecionarConfiguracaoTipo(void)));
  connect(composto_rb, SIGNAL(toggled(bool)), this, SLOT(selecionarConfiguracaoTipo(void)));
  connect(enumeracao_rb, SIGNAL(toggled(bool)), this, SLOT(selecionarConfiguracaoTipo(void)));

  connect(tab_enumeracoes, SIGNAL(s_linhaAdicionada(int)), this, SLOT(manipularEnumeracao(int)));
  connect(tab_enumeracoes, SIGNAL(s_linhaAtualizada(int)), this, SLOT(manipularEnumeracao(int)));

  connect(tab_atributos, SIGNAL(s_linhaAdicionada(int)), this, SLOT(exibirFormAtributo(void)));
  connect(tab_atributos, SIGNAL(s_linhaEditada(int)), this, SLOT(exibirFormAtributo(void)));

  janela_pai->setMinimumSize(580, 680);

  //Preenche o combo box com os tipos de armazenamento disponíveis
  TipoArmazenamento::obterTipos(lista);
  armazenamento_cmb->addItems(lista);

  //Preenche o combo box com os tipos de categoria disponíveis
  TipoCategoria::obterTipos(lista);
  categoria_cmb->addItems(lista);
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
/*TipoWidget::~TipoWidget(void)
{
 for(unsigned i=Tipo::FUNCAO_INPUT; i <= Tipo::FUNCAO_ANALYZE; i++)
  if(sel_funcoes[i]) delete(sel_funcoes[i]);

 if(tab_enumeracoes) delete(tab_enumeracoes);
 if(tab_atributos) delete(tab_atributos);
 if(tipo_copia) delete(tipo_copia);
 if(tipo_elemento) delete(tipo_elemento);
} */
//----------------------------------------------------------
void TipoWidget::hideEvent(QHideEvent *evento)
{
 //Limpa as tabelas
 tab_enumeracoes->removerLinhas();
 tab_atributos->removerLinhas();

 //Limpa os valores dos seletores de funções
 for(unsigned i=Tipo::FUNCAO_INPUT; i <= Tipo::FUNCAO_ANALYZE; i++)
  sel_funcoes[i]->removerObjetoSelecionado();

 //Reinicia os demais campos do formulário para seus valores padrão
 tipo_base_rb->setChecked(true);
 comp_int_sb->setValue(0);
 por_valor_chk->setChecked(false);
 preferido_chk->setChecked(false);
 delimitador_edt->clear();
 valor_padrao_edt->clear();;
 categoria_cmb->setCurrentIndex(0);
 armazenamento_cmb->setCurrentIndex(0);
 alinhamento_cmb->setCurrentIndex(0);
 atrib_base_twg->setCurrentIndex(0);

 /* Desconecta todos os slots ligados ao formulário
    de configuração de parâmetro, permitindo que esse possa
    ser usado por outros formulários sem disparar os slots
    da classe atual */
 disconnect(parametro_wgt,NULL,this,NULL);

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void TipoWidget::selecionarConfiguracaoTipo(void)
{
 atrib_base_twg->setVisible(tipo_base_rb->isChecked());
 enumeracoes_gb->setVisible(enumeracao_rb->isChecked());
 atributos_gb->setVisible(composto_rb->isChecked());
}
//----------------------------------------------------------
void TipoWidget::manipularEnumeracao(int idx_linha)
{
 //Caso o nome da enumeração esteja preenchido
 if(!nome_enum_edt->text().isEmpty())
 {
  //Insere o nome na tabela e limpa o campo de nome
  tab_enumeracoes->definirTextoCelula(nome_enum_edt->text(), idx_linha, 0);
  nome_enum_edt->clear();
 }
 //Caso o nome esteja vazio mas o usuário tente inserir mesmo assim
 else if(tab_enumeracoes->obterTextoCelula(idx_linha, 0).isEmpty())
  /* Remove a linha inserido para que a tabela não fique com
     uma linha em branco */
  tab_enumeracoes->removerLinha(idx_linha);
}
//----------------------------------------------------------
void TipoWidget::manipularAtributo(int res)
{
 int lin;
 Parametro param;

 //Obtém a linha selecionada da tabela.
 lin=tab_atributos->obterLinhaSelecionada();

 //Caso o usuário clique no botão 'aplicar' da janela de conf. de parâmetro
 if(res==QDialog::Accepted)
 {
  //Obtém o parâmetro configurado
  param=parametro_wgt->obterParametro();
  //Insere-o na tabela de atributos
  tab_atributos->definirTextoCelula(QString::fromUtf8(param.obterNome()), lin, 0);
  tab_atributos->definirTextoCelula(QString::fromUtf8(*param.obterTipo()), lin, 1);
  tab_atributos->definirDadoLinha(QVariant::fromValue<Parametro>(param), lin);
 }
 //Caso o usuário clique no botão 'cancelar' da janela de conf. de parâmetro
 else if(res==QDialog::Rejected)
 {
  //Remove a última linha da tabela quando se tratar de adição de novo parâmetro
  if(tab_atributos->obterTextoCelula(lin,0).isEmpty())
   tab_atributos->removerLinha(lin);
 }
}
//----------------------------------------------------------
void TipoWidget::exibirFormAtributo(void)
{
 int idx_lin;

 /* Desabilita alguns campos do formulário de parâmetro
    pois os campos desabilitados não são se aplicam
    ao atributos de um tipo */
 parametro_wgt->param_in_chk->setEnabled(false);
 parametro_wgt->param_out_chk->setEnabled(false);
 parametro_wgt->valorpadrao_edt->setEnabled(false);

 //Obtém a linha selecionada na tabela
 idx_lin=tab_atributos->obterLinhaSelecionada();

 //Caso haja uma linha selecionada
 if(idx_lin >= 0)
  /* Preenche o formulário de edição de parâmetros com os dados do atributo
     especificado na linha selecionada */
  parametro_wgt->definirAtributos(tab_atributos->obterDadoLinha(idx_lin).value<Parametro>(), this->modelo);

 parametro_wgt->show();
}
//----------------------------------------------------------
void TipoWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Tipo *tipo)
{
 TipoPgSQL tp_copia, tp_elem;
 unsigned conf_tipo, i, qtd;
 Parametro param;

 //Conecta o método de manipulação de atributos ao formulário de parâmetros
 connect(parametro_wgt, SIGNAL(finished(int)), this, SLOT(manipularAtributo(int)));

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, tipo);

 //Define o modelo de dados de referência dos seletores de função
 for(i=Tipo::FUNCAO_INPUT; i <= Tipo::FUNCAO_ANALYZE; i++)
  sel_funcoes[i]->definirModelo(modelo);

 //Caso o tipo esteja especificado
 if(tipo)
 {
  //Obtém a configuração do mesmo
  conf_tipo=tipo->obterConfiguracao();

  //Caso o tipo seja composto
  if(conf_tipo==Tipo::TIPO_COMPOSTO)
  {
   //Marca o radiobox no formulário que indica o tipo composto
   composto_rb->setChecked(true);

   /* Desabilita os sinais da tabela de atributos para inserção
      de todos os atributos do tipo sem disparar sinais e executar slots */
   tab_atributos->blockSignals(true);

   //Obtém a quantidade de atributos do tipo
   qtd=tipo->obterNumAtributos();

   /* Preenche a tabela de atributos, obtendo cada um e
      exibindo os dados na tabela */
   for(i=0; i < qtd; i++)
   {
    //Adiciona uma linha na tabela
    tab_atributos->adicionarLinha();
    //Obtém um atributo do tipo
    param=tipo->obterAtributo(i);
    //Exibe os dados do atributo na tabela
    tab_atributos->definirTextoCelula(QString::fromUtf8(param.obterNome()), i, 0);
    tab_atributos->definirTextoCelula(QString::fromUtf8(*param.obterTipo()), i, 1);
    //Armazena o próprio atributo na linha da tabela
    tab_atributos->definirDadoLinha(QVariant::fromValue<Parametro>(param), i);
   }

   //Desbloqueia os sinais da tabela de atributos
   tab_atributos->blockSignals(false);
   //Limpa a seleção da tabela
   tab_atributos->limparSelecao();
  }
  //Caso o tipo seja enumeração
  else if(conf_tipo==Tipo::TIPO_ENUMERACAO)
  {
   //Marca o campo respectivo no formulário
   enumeracao_rb->setChecked(true);

   /* Desabilita os sinais da tabela de enumerações para inserção
     de todos as enumerações do tipo sem disparar sinais e executar slots */
   tab_enumeracoes->blockSignals(true);
   qtd=tipo->obterNumEnumeracoes();

   //Insere todos as enumerações do tipo na tabela
   for(i=0; i < qtd; i++)
   {
    tab_enumeracoes->adicionarLinha();
    tab_enumeracoes->definirTextoCelula(tipo->obterEnumeracao(i), i, 0);
   }
   //Desbloqueia os sinais da tabela de enumerações
   tab_enumeracoes->blockSignals(false);
   tab_enumeracoes->limparSelecao();
  }
  //Caso o tipo seja base
  else
  {
   //Marca o radiobox no formulário que indica o tipo base
   tipo_base_rb->setChecked(true);

   //Obtém o tipo cópia do tipo
   tp_copia=tipo->obterTipoCopia();
   //Obtém o tipo de elemento do tipo
   tp_elem=tipo->obterElemento();

   /* Configura os campos do formulário relacionados ao tipo base
      com os valores definidos na instância de tipo passada */
   comp_int_sb->setValue(tipo->obterCompInterno());
   por_valor_chk->setChecked(tipo->passadoPorValor());
   preferido_chk->setChecked(tipo->tipoPreferido());
   delimitador_edt->setText(QString(tipo->obterDelimitador()));
   valor_padrao_edt->setText(QString::fromUtf8(tipo->obterValorPadrao()));
   categoria_cmb->setCurrentIndex(categoria_cmb->findText(~tipo->obterCategoria()));
   armazenamento_cmb->setCurrentIndex(armazenamento_cmb->findText(~tipo->obterArmazenamento()));
   alinhamento_cmb->setCurrentIndex(alinhamento_cmb->findText(~tipo->obterAlinhamento()));

   //Atribui aos seletores de funções todas as funções configuradas na instância
   for(i=Tipo::FUNCAO_INPUT; i <= Tipo::FUNCAO_ANALYZE; i++)
    sel_funcoes[i]->definirObjeto(tipo->obterFuncao(i));
  }
 }

 //Configura o widget de tipo de cópia com o tipo obtido da instância de tipo
 tipo_copia->definirAtributos(tp_copia, modelo);
 //Configura o widget de tipo de elemento com o tipo obtido da instância de tipo
 tipo_elemento->definirAtributos(tp_elem, modelo);
}
//----------------------------------------------------------
void TipoWidget::aplicarConfiguracao(void)
{
 try
 {
  Tipo *tipo=NULL;
  unsigned i, qtd;

  iniciarConfiguracao<Tipo>();

  //Obtém a referência ao tipo que está sendo configurado
  tipo=dynamic_cast<Tipo *>(this->objeto);

  //Caso o mesmo seja marcado como uma enumeração no formulário
  if(enumeracao_rb->isChecked())
  {
   //Configura a instância como tipo enumeração
   tipo->definirConfiguracao(Tipo::TIPO_ENUMERACAO);
   //Remove todas as enumerações atuais do tipo
   tipo->removerEnumeracoes();

   //Insere na instância de tipo as enumerações configuradas no formulário
   qtd=tab_enumeracoes->obterNumLinhas();
   for(i=0; i < qtd; i++)
    tipo->adicionarEnumeracao(tab_enumeracoes->obterTextoCelula(i,0));
  }
  //Caso o mesmo seja marcado como  um tipo composto no formulário
  else if(composto_rb->isChecked())
  {
   //Configura a instância como tipo composto
   tipo->definirConfiguracao(Tipo::TIPO_COMPOSTO);
   //Remove todos os atributos do tipo
   tipo->removerAtributos();

   //Insere na instância de tipo os atributos configurados no formulário
   qtd=tab_atributos->obterNumLinhas();
   for(i=0; i < qtd; i++)
    tipo->adicionarAtributo(tab_atributos->obterDadoLinha(i).value<Parametro>());
  }
  //Caso o mesmo seja marcado como um tipo base no formulário
  else
  {
   //Configura a instância como tipo base
   tipo->definirConfiguracao(Tipo::TIPO_BASE);

   //Atribui todos os valores configurados no formulário à instância de tipo
   tipo->definirTipoCopia(tipo_copia->obterTipoPgSQL());
   tipo->definirElemento(tipo_elemento->obterTipoPgSQL());
   tipo->definirCompInterno(comp_int_sb->value());
   tipo->definirPorValor(por_valor_chk->isChecked());
   tipo->definirPreferido(preferido_chk->isChecked());
   tipo->definirDelimitador(delimitador_edt->text().at(0).toAscii());
   tipo->definirValorPadrao(valor_padrao_edt->text());
   tipo->definirCategoria(TipoCategoria(categoria_cmb->currentText()));
   tipo->definirAlinhamento(TipoPgSQL(alinhamento_cmb->currentText()));
   tipo->definirArmazenamento(TipoArmazenamento(armazenamento_cmb->currentText()));

   //Atribui todas as funções definidas nos seletores à instância de tipo
   for(i=Tipo::FUNCAO_INPUT; i <= Tipo::FUNCAO_ANALYZE; i++)
    tipo->definirFuncao(i, dynamic_cast<Funcao *>(sel_funcoes[i]->obterObjeto()));
  }

  ObjetoBaseWidget::aplicarConfiguracao();
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
