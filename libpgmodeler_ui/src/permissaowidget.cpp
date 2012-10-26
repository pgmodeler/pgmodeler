#include "permissaowidget.h"
#include "visaoobjetoswidget.h"
extern VisaoObjetosWidget *selecaoobjetos_wgt;

PermissaoWidget::PermissaoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_PERMISSAO)
{
 QGridLayout *grid=NULL;
 QFont fonte;
 QCheckBox *check=NULL;
 unsigned i;
 QString vet_priv[12]={ AtributosParsers::PRIV_SELECT, AtributosParsers::PRIV_INSERT,
                        AtributosParsers::PRIV_UPDATE, AtributosParsers::PRIV_DELETE,
                        AtributosParsers::PRIV_TRUNCATE, AtributosParsers::PRIV_REFERENCES,
                        AtributosParsers::PRIV_TRIGGER, AtributosParsers::PRIV_CREATE,
                        AtributosParsers::PRIV_CONNECT, AtributosParsers::PRIV_TEMPORARY,
                        AtributosParsers::PRIV_EXECUTE, AtributosParsers::PRIV_USAGE };

 Ui_PermissaoWidget::setupUi(this);

 permissao=NULL;

 /* Configura a janela pai inserindo o formulário de
    edição de permissões como filho */
 janela_pai->widgetgeral_wgt->insertWidget(0, this);
 janela_pai->widgetgeral_wgt->setCurrentIndex(0);
 janela_pai->definirBotoes(CaixaMensagem::BOTAO_OK);
 connect(janela_pai->aplicar_ok_btn, SIGNAL(clicked(bool)), janela_pai, SLOT(close(void)));

 //Configura a dimensão da janela pai
 janela_pai->setMinimumSize(670, 480);

 /* Alterando o texto do label de comentário para 'Tipo:'.
    como este formulário é herdado do formulário padrão, o
    campo 'Comentário' passa a ser o campo que mostra o
    tipo do objeto o qual se está configurando as permissões.
    Além disso o campo nome e comentario são marcados como
    read-only pois o usuário não tem acesso a esses campos. */
 comentario_lbl->setText(trUtf8("Type:"));
 fonte=nome_edt->font();
 fonte.setItalic(true);
 comentario_edt->setFont(fonte);
 comentario_edt->setReadOnly(true);
 nome_edt->setFont(fonte);
 nome_edt->setReadOnly(true);

 //Configura o formulário adicionando os campos de edição de permissão
 configurarLayouFormulario(permissao_grid, OBJETO_PERMISSAO);

 //Cria a tabela de papéis com os botões de inserir, remover e editar item
 tab_papeis=new TabelaObjetosWidget(TabelaObjetosWidget::BTN_INSERIR_ITEM |
                                    TabelaObjetosWidget::BTN_REMOVER_ITEM |
                                    TabelaObjetosWidget::BTN_EDITAR_ITEM, false, this);
 tab_papeis->definirNumColunas(1);
 tab_papeis->definirRotuloCabecalho(trUtf8("Role"),0);
 tab_papeis->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),0);

 /* Insere a tabela de papéis em um grid layout para que a mesma
    se adapte ao layout da janela */
 grid=new QGridLayout;
 grid->addWidget(tab_papeis,0,0,1,1);
 grid->setContentsMargins(2,2,2,2);
 papeis_gb->setLayout(grid);

 //Cria a tabela de permissões com os botões de inserir, remover e editar item
 tab_permissoes=new TabelaObjetosWidget(TabelaObjetosWidget::BTN_REMOVER_ITEM |
                                        TabelaObjetosWidget::BTN_EDITAR_ITEM |
                                        TabelaObjetosWidget::BTN_LIMPAR_ITENS, true, this);
 tab_permissoes->definirNumColunas(3);
 tab_permissoes->definirRotuloCabecalho(trUtf8("Id"),0);
 tab_permissoes->definirIconeCabecalho(QPixmap(":/icones/icones/uid.png"),0);
 tab_permissoes->definirRotuloCabecalho(trUtf8("Roles"),1);
 tab_permissoes->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),1);
 tab_permissoes->definirRotuloCabecalho(trUtf8("Privileges"),2);
 tab_permissoes->definirIconeCabecalho(QPixmap(":/icones/icones/grant.png"),2);

 /* Insere a tabela de permissões em um grid layout para que a mesma
    se adapte ao layout da janela */
 grid=new QGridLayout;
 grid->addWidget(tab_permissoes,0,0,1,1);
 grid->setContentsMargins(2,2,2,2);
 permissoes_gb->setLayout(grid);

 /* Cria a lista de privilégios criando checkboxes com as
    descrições de cada privilégio */
 for(i=Permissao::PRIV_SELECT; i<=Permissao::PRIV_USAGE; i++)
 {
  //O primeiro checkbox é o que possui o nome do privilégio
  check=new QCheckBox;
  check->setText(vet_priv[i].toUpper());
  privilegios_tbw->insertRow(i);
  privilegios_tbw->setCellWidget(i,0,check);
  //Conecta o sinal de clique no chebox ao método que mapeia a marcação de privilégio
  connect(check, SIGNAL(clicked(bool)), this, SLOT(marcarPrivilegio(void)));

  //O segundo checkbox é o que indica se o privilégio é com GRANT OPTION
  check=new QCheckBox;
  check->setText("GRANT OPTION");
  privilegios_tbw->setCellWidget(i,1,check);
  //Conecta o sinal de clique no chebox ao método que mapeia a marcação de privilégio
  connect(check, SIGNAL(clicked(bool)), this, SLOT(marcarPrivilegio(void)));
 }

 connect(tab_papeis, SIGNAL(s_linhaAdicionada(int)), tab_papeis, SLOT(selecionarLinha(int)));
 connect(tab_papeis, SIGNAL(s_linhaEditada(int)), this, SLOT(selecionarPapel(void)));
 connect(tab_papeis, SIGNAL(s_linhaRemovida(int)), this, SLOT(habilitarBotoesEdicao(void)));

 connect(tab_permissoes, SIGNAL(s_linhaEditada(int)), this, SLOT(editarPermissao(void)));
 connect(tab_permissoes, SIGNAL(s_linhaSelecionada(int)), this, SLOT(selecionarPermissao(int)));
 connect(tab_permissoes, SIGNAL(s_linhaRemovida(int)), this, SLOT(removerPermissao(void)));

 connect(cancelar_tb, SIGNAL(clicked(bool)), this, SLOT(cancelarOperacao(void)));
 connect(inserir_perm_tb, SIGNAL(clicked(bool)), this, SLOT(adicionarPermissao(void)));
 connect(atualizar_perm_tb, SIGNAL(clicked(bool)), this, SLOT(atualizarPermissao(void)));
}

void PermissaoWidget::hideEvent(QHideEvent *evento)
{
 disconnect(selecaoobjetos_wgt,0,this,0);
 cancelarOperacao();

 /* Desconecta o sinal de linhasRemovidas da tabela de permissões para que todas
    as linhas existentes sejam removidas sem disparar o método removerPermissoes
    da classe PermissaoWidget */
 tab_permissoes->blockSignals(true);
 tab_permissoes->removerLinhas();
 tab_permissoes->blockSignals(false);

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}

void PermissaoWidget::definirAtributos(ModeloBD *modelo, ObjetoBase *objeto_pai, ObjetoBase *objeto)
{
 /* Chama o método de definição de atributos da classe Pai para depois
    configurar os atributos relacionados   classe PermissaoWidget */
 ObjetoBaseWidget::definirAtributos(modelo,NULL,objeto,objeto_pai);

 if(objeto)
 {
  unsigned priv;
  QCheckBox *chk=NULL, *chk1=NULL;
  TipoObjetoBase tipo_obj;

  connect(selecaoobjetos_wgt, SIGNAL(s_visibilityChanged(ObjetoBase*,bool)), this, SLOT(exibirDadosPapelSelecionado(void)));
  connect(tab_papeis, SIGNAL(s_linhaAdicionada(int)), this, SLOT(selecionarPapel(void)));
  connect(tab_permissoes, SIGNAL(s_linhasRemovidas(void)), this, SLOT(removerPermissoes(void)));

  //Preenche os campos do formulario com os atributos do objeto
  nome_edt->setText(QString::fromUtf8(objeto->obterNome(true)));
  comentario_edt->setText(QString::fromUtf8(objeto->obterNomeTipoObjeto()));
  tipo_obj=objeto->obterTipoObjeto();

  /* Faz uma varredura usando os privilégios disponíveis para os objetos.
     O objetivo é deixar visível ao usuário somente os privilégios referentes
     ao objeto atualmente em edição. */
  for(priv=Permissao::PRIV_SELECT; priv<=Permissao::PRIV_USAGE; priv++)
  {
   //Obtém os checkboxes que representam o priviléigo e a opção GRANT OPTION
   chk=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,0));
   chk1=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,1));
   //Desmarca ambos os checks
   chk->setChecked(false);
   chk1->setChecked(false);

   /* Valida o tipo de privilégio atual com o tipo do objeto editado atualmente.
      caso o par (privilégio, tipo do objeto) coincida com uma das condições a
      seguir a linha referente ao privilégio é mostrada na tabela, caso contrário
      é ocultada */
   if(((priv==Permissao::PRIV_SELECT || priv==Permissao::PRIV_UPDATE) &&
        (tipo_obj==OBJETO_TABELA || tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_SEQUENCIA)) ||

      ((priv==Permissao::PRIV_INSERT || priv==Permissao::PRIV_DELETE) &&
        (tipo_obj==OBJETO_TABELA)) ||

      ((priv==Permissao::PRIV_INSERT) && (tipo_obj==OBJETO_COLUNA)) ||

      ((priv==Permissao::PRIV_TRUNCATE || priv==Permissao::PRIV_TRIGGER) &&
        (tipo_obj==OBJETO_TABELA)) ||

       (priv==Permissao::PRIV_REFERENCES &&
        (tipo_obj==OBJETO_TABELA || tipo_obj==OBJETO_COLUNA)) ||

       (priv==Permissao::PRIV_CREATE &&
        (tipo_obj==OBJETO_BANCO_DADOS || tipo_obj==OBJETO_ESQUEMA || tipo_obj==OBJETO_ESPACO_TABELA)) ||

      ((priv==Permissao::PRIV_CONNECT || priv==Permissao::PRIV_TEMPORARY) &&
        (tipo_obj==OBJETO_BANCO_DADOS)) ||

       (priv==Permissao::PRIV_EXECUTE &&
        (tipo_obj==OBJETO_FUNCAO || tipo_obj==OBJETO_FUNC_AGREGACAO)) ||

       (priv==Permissao::PRIV_USAGE &&
        (tipo_obj==OBJETO_SEQUENCIA || tipo_obj==OBJETO_LINGUAGEM || tipo_obj==OBJETO_ESQUEMA)) ||

       (priv==Permissao::PRIV_SELECT && tipo_obj==OBJETO_VISAO))
   {
    privilegios_tbw->setRowHidden(priv, false);
   }
   else
    privilegios_tbw->setRowHidden(priv, true);
  }

  //Atualiza a listagem das permissões
  listarPermissoes();
  //Por padrão, deseleciona qualquer permissão marcada na tabela
  tab_permissoes->limparSelecao();
 }
}

void PermissaoWidget::selecionarPapel(void)
{
 selecaoobjetos_wgt->definirObjetoVisivel(OBJETO_PAPEL, true);
 selecaoobjetos_wgt->definirModelo(this->modelo);
 selecaoobjetos_wgt->show();
}

void PermissaoWidget::selecionarPermissao(int idx_perm)
{
 if(idx_perm >= 0)
  permissao=reinterpret_cast<Permissao *>(tab_permissoes->obterDadoLinha(idx_perm).value<void *>());
 else
  permissao=NULL;
}

void PermissaoWidget::listarPermissoes(void)
{
 if(modelo)
 {
  vector<Permissao *> permissoes;
  Permissao *perm=NULL;
  unsigned i, qtd, i1, qtd1;
  QString str_aux;

  //Obtém as permissões relacionadas ao objeto armazenando em uma lista
  modelo->obterPermissoes(this->objeto, permissoes);
  qtd=permissoes.size();

  /* Disconecta o sinal linhasRemovidas da tabela de permissões para remover
     todas as linhas sem que o método removerPermissoes seja chamado, ou seja,
     as linhas são removidas sem qualquer tipo de tratamento */
  tab_permissoes->blockSignals(true);
  //Remove as linhas da tabela sem qualquer tratamento
  tab_permissoes->removerLinhas();

  /* Reconecta o sinal para que interações de remoção de permissões por parte do usuário
     possam ser tratadas normalmente */
  tab_permissoes->blockSignals(false);

  //Varre a lista de permissões obtidas exibindo os dados de cada uma na tabela de permissões
  for(i=0; i < qtd; i++)
  {
   //Obtém uma permissão
   perm=permissoes[i];

   //Adiciona uma linha   tabela
   tab_permissoes->adicionarLinha();
   //Define como dado da linha adicionada a permissão atual
   tab_permissoes->definirDadoLinha(QVariant::fromValue<void *>(reinterpret_cast<void *>(perm)),i);
   //Define o texto da celuna (0) da linha atual com o nome do privilégio (gerado automaticamente)
   tab_permissoes->definirTextoCelula(perm->obterNome(),i,0);
   //A segunda coluna da linha será a string que define os privilégios da permissão
   tab_permissoes->definirTextoCelula(perm->obterStringPrivilegios(),i,2);

   /* A terceira coluna armazena os nomes concatenados de todos
      os papéis relacionado�  permissão */
   qtd1=perm->obterNumPapeis();
   for(i1=0; i1 < qtd1; i1++)
   {
    str_aux+=QString::fromUtf8(perm->obterPapel(i1)->obterNome());
    str_aux+=",";
   }
   str_aux.remove(str_aux.size()-1,1);
   tab_permissoes->definirTextoCelula(str_aux,i,1);
   str_aux.clear();
  }
 }
}

void PermissaoWidget::exibirDadosPapelSelecionado(void)
{
 int lin, idx_lin=-1;
 Papel *papel=NULL;

 //Obtém o papel selecionado na janela de seleção de objetos
 papel=dynamic_cast<Papel *>(selecaoobjetos_wgt->obterObjetoSelecao());

 //Obtém a linha selecionada da tabela atual
 lin=tab_papeis->obterLinhaSelecionada();

 /* Caso haja um objeto selecionado tenta obter o índice da linha
    a qual armazene como dado o objeto selecionado isso é feito
    para se validar se o usuário está tentando inserir na mesma
    tabela um objeto por mais de uma vez */
 if(papel)
  idx_lin=tab_papeis->obterIndiceLinha(QVariant::fromValue<void *>(dynamic_cast<void *>(papel)));

 //Se o objeto da seleção não existir na tabela exibe seus dados
 if(papel && idx_lin < 0)
 {
  tab_papeis->definirTextoCelula(QString::fromUtf8(papel->obterNome()), lin, 0);
  tab_papeis->definirDadoLinha(QVariant::fromValue<void *>(dynamic_cast<void *>(papel)), lin);
 }
 else
 {
  /* Caso a linha atual da tabela não contenha nenhum dado isso indica
     que a mesmo é uma linha vazia ou seja que foi inclúida recentemente
     e sendo assim esta linha precisa ser removida caso o objeto da seleção
     não seja usado */
  if(!tab_papeis->obterDadoLinha(lin).value<void *>())
   tab_papeis->removerLinha(lin);

  /* Caso o objeto da seleção já exista na tabela dispara uma exceção
     pois o mesmo objeto não pode aparecer mais de uma vez na mesma tabela */
  if(papel && idx_lin >= 0)
  {
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATROBJDUPLICCONT)
                               .arg(QString::fromUtf8(papel->obterNome()))
                               .arg(papel->obterNomeTipoObjeto())
                               .arg(papeis_gb->title()),
                 ERR_PGMODELER_INSITEMPAPELDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
 }
}

void PermissaoWidget::adicionarPermissao(void)
{
 Permissao *perm_aux=NULL;

 try
 {
  //Aloca uma permissão para o objeto
  perm_aux=new Permissao(this->objeto);

  //Configura a nova permissão criada
  configurarPermissao(perm_aux);

  //Adiciona a permissão ao modelo
  modelo->adicionarPermissao(perm_aux);

  //Atualiza a lista de permissões
  listarPermissoes();

  /* Executa o método de cancelamento da operação
     apenas para limpar o formulário e desabilitar
     os botões. */
  cancelarOperacao();
 }
 catch(Excecao &e)
 {
  /* Em caso de qualuqer erro for disparado
     a permissão alocada é removida do modelo e
     desalocada da memória */
  if(perm_aux)
  {
   modelo->removerPermissao(perm_aux);
   delete(perm_aux);
  }

  /* Cancela a operação de criação da permissão
     limpando o formulário e desabilitando botões
     de edição */
  cancelarOperacao();

  //Redireciona o erro capturado
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void PermissaoWidget::atualizarPermissao(void)
{
 Permissao *perm_aux=NULL,*perm_bkp=NULL;
 int idx_perm;

 try
 {
  //Aloca uma nova permissão para ser configurada
  perm_aux=new Permissao(this->objeto);

  /* Cria uma permissão de backup. Esta receberá os
     os valores atuais da permissão que está sendo editada
     pois em caso de erro seus atributos originais são restaurados */
  perm_bkp=new Permissao(this->objeto);
  (*perm_bkp)=(*permissao);

  //Configura a nova permissão com os atributos preenchidos no formulário
  configurarPermissao(perm_aux);

  /* Tenta obter o índice de uma permissão no modelo na qual
     suas configurações conincidam com as configurações da
     permissão recém configurada (perm_aux) */
  idx_perm=modelo->obterIndicePermissao(perm_aux);

  /* Caso o índice seja negativo isso indica que a configuração da permissão auxiliar (perm_aux) não
     se assemelha a nehuma permissão no modelo. Já se o índice for positivo e a permissão no
     índice seja a mesma que está sendo edita (permissao) isso indica que a permissão auxiliar é
     igual   permissão atual, podendo claramente ser atualizada. */
  if(idx_perm < 0 || (idx_perm >=0 && modelo->obterObjeto(idx_perm,OBJETO_PERMISSAO)==permissao))
  {
   /* Copia os atributos da permissão auxiliar para a permissão atual
      efetivando as alterações */
   (*permissao)=(*perm_aux);
   //Atualiza a lista de permissões para exibir os dados da permissão atualizada
   listarPermissoes();
   //Limpa o formulário e desabilita os botões de edição
   cancelarOperacao();
  }
  else
  {
   /* Qualquer outra situação além da comentado no if acima é considerada duplicação
      de permissões gerando assim um erro */
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRPERMISSAODUPLIC)
                 .arg(QString::fromUtf8(permissao->obterObjeto()->obterNome()))
                 .arg(permissao->obterObjeto()->obterNomeTipoObjeto()),
                 ERR_PGMODELER_ATRPERMISSAODUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  //Remove a permissão de backup
  delete(perm_bkp);
 }
 catch(Excecao &e)
 {
  //Em caso de erro restaura os valores originais da permissão
  (*permissao)=(*perm_bkp);

  //Desaloca as permissões de backup e auxiliar
  delete(perm_aux);
  delete(perm_bkp);

  cancelarOperacao();

  //Redireciona o erro capturado
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void PermissaoWidget::editarPermissao(void)
{
 if(permissao)
 {
  unsigned priv, i, qtd;
  QCheckBox *chk=NULL, *chk1=NULL;
  Papel *papel=NULL;

  /* Disconecta o sinal de linhaAdicionada da tabela de papéis para que
     mesma possa ter linhas adicionadas sem qualquer tratamento pelo
     método selecionarPapel */
  tab_papeis->blockSignals(true);

  //Limpa a tabela de papéis
  tab_papeis->removerLinhas();
  //Preenche o campo do formulário com o noome da permissão selecionada
  id_perm_edt->setText(permissao->obterNome());

  /* Obtém a quantidade de papéis relacionao �  permissão para que os
     mesmos possam ser exibidos na tabela de papéis */
  qtd=permissao->obterNumPapeis();
  for(i=0; i < qtd; i++)
  {
   //Adiciona uma linha   tabela de permissão
   tab_papeis->adicionarLinha();
   //Obtém um papel da permissão
   papel=permissao->obterPapel(i);
   //Define como dado da linha atual o papel obtido
   tab_papeis->definirDadoLinha(QVariant::fromValue<void *>(reinterpret_cast<void *>(papel)), i);
   //Define como texo da linha atual o nome do papel
   tab_papeis->definirTextoCelula(QString::fromUtf8(papel->obterNome()),i,0);
  }

  //Reconect o sinal de adição de linha da tabela de papéis ao método selecionarPapel
  tab_papeis->blockSignals(false);

  //Preenchendo a tabela de privilégios com os valores dos privilégios da permissão
  for(priv=Permissao::PRIV_SELECT; priv<=Permissao::PRIV_USAGE; priv++)
  {
   //Obtém os checkboxes que representam o priviléigo e a opção GRANT OPTION
   chk=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,0));
   chk1=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,1));

   chk->setChecked(permissao->obterPrivilegio(priv));
   chk1->setChecked(permissao->obterOpConcessao(priv));
  }

  //Habilita os botões de edição da permissão
  habilitarBotoesEdicao();
 }
}

void PermissaoWidget::removerPermissao(void)
{
 //Remove a permissão atualmente selecionada
 modelo->removerPermissao(permissao);
 //Limpa o formulário e desabilita os botões de edição
 cancelarOperacao();
 permissao=NULL;
 /* Limpa a seleção de itens na tabela de permissões
    forçando o usuário a selecionar novamente outro
    item para edição/remoção */
 tab_permissoes->limparSelecao();
}

void PermissaoWidget::removerPermissoes(void)
{
 //Remove todas as permissões relacionadas ao objeto
 modelo->removerPermissoes(objeto);
 cancelarOperacao();
}

void PermissaoWidget::configurarPermissao(Permissao *permissao)
{
 if(permissao)
 {
  unsigned qtd, i, priv;
  QCheckBox *chk=NULL, *chk1=NULL;

  /* Remove os papéis da permissão pois esta receberá
     os papéis da tabela de papéis */
  permissao->removerPapeis();
  qtd=tab_papeis->obterNumLinhas();

  //Adiciona cada papel da tabela   permissão
  for(i=0; i < qtd; i++)
   permissao->adicionarPapel(reinterpret_cast<Papel *>(tab_papeis->obterDadoLinha(i).value<void *>()));

  /* Configura os privilégios da permissão com base nos checkboxes
     selecionados ta tabela de privilégios */
  for(priv=Permissao::PRIV_SELECT; priv <= Permissao::PRIV_USAGE; priv++)
  {
   if(!privilegios_tbw->isRowHidden(priv))
   {
    chk=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,0));
    chk1=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,1));
    permissao->definirPrivilegio(priv, chk->isChecked(), chk1->isChecked());
   }
  }
 }
}

void PermissaoWidget::cancelarOperacao(void)
{
 unsigned priv;
 QCheckBox *chk=NULL;

 /* Define a permissão atual como NULL indicando que nenhuma permissão
    está sendo editada no momento */
 permissao=NULL;

 //Desmarca todos os privilégios na tabela de privilégios
 for(priv=Permissao::PRIV_SELECT; priv<=Permissao::PRIV_USAGE; priv++)
 {
  chk=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,0));
  chk->setChecked(false);
  chk=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,1));
  chk->setChecked(false);
 }

 //Limpa a tabela de papéis selecionados
 tab_papeis->removerLinhas();
 //Limpa o campo que armazena o nome da permissão
 id_perm_edt->clear();
 //Desabilita os botẽos de edição
 habilitarBotoesEdicao();
 //Desabilita o próprio botão de cancelar operação
 cancelar_tb->setEnabled(false);
 //Limpa a seleção na tabela de permissões
 tab_permissoes->limparSelecao();
}

void PermissaoWidget::marcarPrivilegio(void)
{
 QObject *obj_sender=sender();

 //Este método só será executado caso o objeto sender seja um checkbox
 if(obj_sender && obj_sender->metaObject()->className()==QString("QCheckBox"))
 {
  QCheckBox *chk=NULL, *chk_priv=NULL, *chk_gop=NULL;
  unsigned priv;

  //Converte o objeto sender em checkbox
  chk=dynamic_cast<QCheckBox *>(obj_sender);

  for(priv=Permissao::PRIV_SELECT; priv<=Permissao::PRIV_USAGE; priv++)
  {
   //Obtém os checkboxes que representam o priviléigo e a opção GRANT OPTION
   chk_priv=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,0));
   chk_gop=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,1));

   /* Caso o checkbox clicado (sender) seja o checkbox
      da coluna GRANT OPTION, o checkbox de privilégio
      receberá o mesmo estado do checkbox de grant option */
   if(chk==chk_gop)
   {
    chk_priv->setChecked(chk_gop->isChecked());
    break;
   }
   /* Caso o checkbox clicado (sender) seja o checkbox
      do privilégio e o mesmo for desmarcado, o checkbox de GRANT OPTION
      será também desmarcado */
   else if(chk==chk_priv && !chk->isChecked())
   {
    chk_gop->setChecked(false);
    break;
   }
  }

  //Habilita/desabilita os botões de edição
  habilitarBotoesEdicao();
 }
}

void PermissaoWidget::habilitarBotoesEdicao(void)
{
 bool priv_marcados=false;
 unsigned priv;
 QCheckBox *chk=NULL, *chk1=NULL;

 for(priv=Permissao::PRIV_SELECT; priv<=Permissao::PRIV_USAGE && !priv_marcados; priv++)
 {
  //Obtém os checkboxes que representam o priviléigo e a opção GRANT OPTION
  chk=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,0));
  chk1=dynamic_cast<QCheckBox *>(privilegios_tbw->cellWidget(priv,1));
  //Verifica se algum privilégio está marcado
  priv_marcados=(chk->isChecked() || chk1->isChecked());
 }

 /* O botão de atualizar permissão estará ativo caso haja algum privilégio marcado,
    também existam papéis selecionados e exista uma permissão sendo editada */
 atualizar_perm_tb->setEnabled(priv_marcados && tab_papeis->obterNumLinhas() > 0 && permissao!=NULL);

 /* O botão de inserção permissão estará ativo caso haja algum privilégio marcado,
    também existam papéis selecionados */
 inserir_perm_tb->setEnabled(priv_marcados && tab_papeis->obterNumLinhas() > 0);

 /* O botão de cancelar a operação estará ativo caso o obtão de inserir ou atualizar esteja habilitado
    ou caso haja alguma permissão criada (tabela de permissões com pelo menos 1 item) */
 cancelar_tb->setEnabled(inserir_perm_tb->isEnabled() || atualizar_perm_tb->isEnabled() || tab_permissoes->obterNumLinhas() > 0);
}

