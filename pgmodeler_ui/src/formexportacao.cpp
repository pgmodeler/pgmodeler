#include "formexportacao.h"
#include "progressotarefa.h"
#include "formconfiguracao.h"

extern ProgressoTarefa *prog_tarefa;
extern FormConfiguracao *fconfiguracao;
//***********************************************************
FormExportacao::FormExportacao(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
 vector<QString> versoes;

 modelo=NULL;

 setupUi(this);
 connect(exportacao_arq_rb, SIGNAL(toggled(bool)), this, SLOT(habilitarTipoExportacao(void)));
 connect(exportacao_sgbd_rb, SIGNAL(toggled(bool)), this, SLOT(habilitarTipoExportacao(void)));
 connect(pgsqlvers_chk, SIGNAL(toggled(bool)), pgsqlvers1_cmb, SLOT(setEnabled(bool)));
 connect(fechar_btn, SIGNAL(clicked(bool)), this, SLOT(close(void)));
 connect(sel_arquivo_tb, SIGNAL(clicked(void)), this, SLOT(selecionarArquivoDestino(void)));
 connect(exportar_btn, SIGNAL(clicked(void)), this, SLOT(exportarModelo(void)));

 //Obtém as versões disponíveis de esquemas SQL
 ParserEsquema::obterVersoesPgSQL(versoes);

 //Preenche os comboboxes de versões
 pgsqlvers_cmb->addItems(QStringList(QList<QString>::fromVector(QVector<QString>::fromStdVector(versoes))));
 pgsqlvers1_cmb->addItems(QStringList(QList<QString>::fromVector(QVector<QString>::fromStdVector(versoes))));

 /** ticket#2 **/
 //Fix específico para Windows: força a aparência do frame para WinPanel
 #ifdef Q_OS_WIN32
  this->frame->setFrameShape(QFrame::WinPanel);
 #endif
}
//-----------------------------------------------------------
void FormExportacao::show(ModeloBD *modelo)
{
 if(modelo)
 {
  map<QString, ConexaoBD *> conexoes;
  map<QString, ConexaoBD *>::iterator itr;
  QString host;

  /* Atualiza a lista de conexões ao exibir o formulário de exportação, obtendo-as
     do widget de configuração de conexões */
  this->modelo=modelo;
  dynamic_cast<ConfConexoesWidget *>(fconfiguracao->obterWidgetConfiguracao(FormConfiguracao::WGT_CONF_CONEXOES))->obterConexoes(conexoes);

  conexoes_cmb->clear();
  itr=conexoes.begin();

  while(itr!=conexoes.end())
  {
   host=(itr->second)->obterParamConexao(ConexaoBD::PARAM_FQDN_SERVIDOR);

   if(host.isEmpty())
    host=(itr->second)->obterParamConexao(ConexaoBD::PARAM_IP_SERVIDOR);

   conexoes_cmb->addItem(itr->first + QString(" (%1)").arg(host),
                         QVariant::fromValue<void *>(itr->second));
   itr++;
  }

  //Esconde os widgets de progresso da exportação
  this->ocultarProgressoExportacao();
  QDialog::show();
 }
}
//-----------------------------------------------------------
void FormExportacao::hideEvent(QHideEvent *)
{
 this->modelo=NULL;
 arquivo_edt->clear();
 exportacao_arq_rb->setChecked(true);
 exportar_btn->setEnabled(false);
}
//-----------------------------------------------------------
void FormExportacao::exportarModelo(void)
{
 int id_tipo;
 QString  versao, buf_sql, cmd_sql;
 ConexaoBD *conexao=NULL, conex_novo_bd;
 unsigned i, qtd;
 bool bd_criado=false;
 int idx_objs[]={-1, -1};
 TipoObjetoBase vet_tipos[]={OBJETO_PAPEL, OBJETO_ESPACO_TABELA};
 ObjetoBase *objeto=NULL;


 /* Vetor que armazena os códigos de erros referentes a objetos duplicados no PostgreSQL:
    Estes erros podem ser consultados na íntegra em:
      http://www.postgresql.org/docs/current/static/errcodes-appendix.html

    Códigos usados neste método:
     42P04 	duplicate_database
     42723 	duplicate_function
     42P06 	duplicate_schema
     42P07 	duplicate_table
     42710 	duplicate_object
  */
 QString cod_erros[]={"42P04", "42723", "42P06", "42P07", "42710"};
 vector<QString> vet_cod_erros;

 try
 {
  for(i=0; i < 5; i++)
   vet_cod_erros.push_back(cod_erros[i]);

  //Redimensiona a janela para exibição dos widgets de progresso
  this->resize(this->maximumSize());

  //Exibe os widgets de progresso
  ln2_frm->setVisible(true);
  rot_prog_lbl->setVisible(true);
  prog_pb->setVisible(true);

  //Inicialmente o ícone de sucesso/erro da exportação fica escondido
  ico_lbl->setVisible(false);

  //Configura o widget de progresso para capturar o progresso da geração do código
  prog_tarefa->setWindowTitle(trUtf8("Gerando código-fonte..."));
  connect(this->modelo, SIGNAL(s_objetoCarregado(int,QString,unsigned)),
          prog_tarefa, SLOT(executarProgesso(int,QString,unsigned)));

  rot_prog_lbl->setText(trUtf8("Iniciando exportação do modelo..."));
  rot_prog_lbl->repaint();

  //Caso seja exportação em arquivo
  if(exportacao_arq_rb->isChecked())
  {
   //Define a versão do postgresql a ser adotada
   ParserEsquema::definirVersaoPgSQL(pgsqlvers_cmb->currentText());

   rot_prog_lbl->setText(trUtf8("Salvando arquivo '%1'").arg(arquivo_edt->text()));

   //Salva o modelo em arquivo
   modelo->salvarModelo(arquivo_edt->text(), ParserEsquema::DEFINICAO_SQL);
   prog_pb->setValue(25);
  }
  //Caso seja exportação direto para o SGBD
  else
  {
   //Obtém a conexão selecionada no combo
   conexao=reinterpret_cast<ConexaoBD *>(conexoes_cmb->itemData(conexoes_cmb->currentIndex()).value<void *>());
   //Tenta se conectar
   conexao->conectar();
   //Obtém a versão do servidor PostgreSQL. Essa versão é usada na geração de código a seguir
   versao=(conexao->obterVersaoSGBD()).mid(0,3);

   /* Caso o checkbox de versão esteja marcada então a versão do servidor é ignorada
      usando aquela escolhida no combo */
   if(pgsqlvers_chk->isChecked())
    ParserEsquema::definirVersaoPgSQL(pgsqlvers1_cmb->currentText());
   else
    ParserEsquema::definirVersaoPgSQL(versao);


   //Cria os Papéis e espaços de tabela separadamente dos demais
   for(id_tipo=0; id_tipo < 2; id_tipo++)
   {
    qtd=modelo->obterNumObjetos(vet_tipos[id_tipo]);
    for(i=0; i < qtd; i++)
    {
     objeto=modelo->obterObjeto(i, vet_tipos[id_tipo]);
     rot_prog_lbl->setText(trUtf8("Criando objeto '%1' (%2)...").arg(QString::fromUtf8(objeto->obterNome())).arg(objeto->obterNomeTipoObjeto()));
     rot_prog_lbl->repaint();

     try
     {
      conexao->executarComandoDDL(objeto->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_SQL));
     }
     catch(Excecao &e)
     {
      /* Caso o checkbox de ignorar duplicidade não esteja marcado ou se este está marcado porém a
         informação adicinal da exceção não carrega um dos códigos indicando duplicidade de objeto
         redireciona o erro, caso contrário apenas o ignora */
      if(!ignorar_dup_chk->isChecked() ||
         (ignorar_dup_chk->isChecked() &&
          std::find(vet_cod_erros.begin(), vet_cod_erros.end(), e.obterInfoAdicional())==vet_cod_erros.end()))
       throw Excecao(e.obterMensagemErro(),
                     e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
     }

     prog_pb->setValue((10 * (id_tipo+1)) + ((i/static_cast<float>(qtd)) * 10));
     idx_objs[id_tipo]++;
    }
   }

   //Cria o banco de dados no servidor
   rot_prog_lbl->setText(trUtf8("Criando banco de dados '%1'...").arg(QString::fromUtf8(modelo->obterNome())));
   rot_prog_lbl->repaint();

   try
   {
    conexao->executarComandoDDL(modelo->__obterDefinicaoObjeto(ParserEsquema::DEFINICAO_SQL));
   }
   catch(Excecao &e)
   {
    /* Caso o checkbox de ignorar duplicidade não esteja marcado ou se este está marcado porém a
       informação adicinal da exceção não carrega um dos códigos indicando duplicidade de objeto
       redireciona o erro, caso contrário apenas o ignora */
    if(!ignorar_dup_chk->isChecked() ||
       (ignorar_dup_chk->isChecked() &&
        std::find(vet_cod_erros.begin(), vet_cod_erros.end(), e.obterInfoAdicional())==vet_cod_erros.end()))
     throw Excecao(e.obterMensagemErro(),
                   e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
   }

   prog_pb->setValue(30);
   bd_criado=true;

   conex_novo_bd=(*conexao);
   conex_novo_bd.definirParamConexao(ConexaoBD::PARAM_NOME_BD, modelo->obterNome());
   rot_prog_lbl->setText(trUtf8("Conectando ao banco de dados '%1'...").arg(QString::fromUtf8(modelo->obterNome())));
   rot_prog_lbl->repaint();
   conex_novo_bd.conectar();
   prog_pb->setValue(50);

   //Cria os demais objetos no novo banco
   rot_prog_lbl->setText(trUtf8("Criando objetos No banco de dados '%1'...").arg(QString::fromUtf8(modelo->obterNome())));
   rot_prog_lbl->repaint();

   //Gera o código SQL de todo o banco
   buf_sql=modelo->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_SQL, false);

   /* Extrai cada comando SQL do buffeer e o executa separadamente, isso é feito
      para que, em caso de erro, o usuário saiba exatamente a SQL que gerou a
      exceção uma vez que a libpq não proporciona este nível de detalhes */
   i=0;
   qtd=buf_sql.size();

   while(i < qtd)
   {
    try
    {
     cmd_sql.clear();

     //Extrai os caracteres até encontrar o final do buffer ou um ';'
     while(i < qtd && buf_sql.at(i)!=';')
     {
      cmd_sql+=buf_sql.at(i);
      i++;
     }

     //Executa um trimm no comando SQL extraído
     cmd_sql=cmd_sql.trimmed();

     //Caso o comando não esteja vazio
     if(!cmd_sql.isEmpty())
     {
      i++;
      //Concatena um ';' para finalizar o comando
      cmd_sql+=';';
      //Executa-o na conexão
      conex_novo_bd.executarComandoDDL(cmd_sql);
     }

     prog_pb->setValue(50 + ((i/static_cast<float>(qtd)) * 10));
    }
    catch(Excecao &e)
    {
     /* Caso o checkbox de ignorar duplicidade não esteja marcado ou se este está marcado porém a
        informação adicinal da exceção não carrega um dos códigos indicando duplicidade de objeto
        redireciona o erro, caso contrário apenas o ignora */
     if(!ignorar_dup_chk->isChecked() ||
        (ignorar_dup_chk->isChecked() &&
         std::find(vet_cod_erros.begin(), vet_cod_erros.end(), e.obterInfoAdicional())==vet_cod_erros.end()))
      throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_FALHAEXPORTACAO).arg(cmd_sql),
                    ERR_PGMODELERUI_FALHAEXPORTACAO,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
    }
   }
  }

  //Finaliza o progresso da exportação
  prog_pb->setValue(100);
  rot_prog_lbl->setText(trUtf8("Exportação finalizada com sucesso!"));
  rot_prog_lbl->repaint();
  ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));
  ico_lbl->setVisible(true);

  prog_tarefa->close();
  disconnect(this->modelo, NULL, prog_tarefa, NULL);

  //Oculta os widgets de progresso após 10 segundos
  QTimer::singleShot(10000, this, SLOT(ocultarProgressoExportacao(void)));
 }
 catch(Excecao &e)
 {
  QString drop_cmd=QString("DROP %1 %2;");

  prog_tarefa->close();
  disconnect(this->modelo, NULL, prog_tarefa, NULL);

  /* Caso os algum objeto tenha sido criado é preciso excluí-los do banco.
     Para isso, os mesmos são removidos na ordem contrária de criação:
     banco de dados, espaço de tabelas e papéis */
  if(bd_criado || idx_objs[0] >= 0 || idx_objs[1] >= 0)
  {
   if(conex_novo_bd.conexaoEstabelecida())
    conex_novo_bd.fechar();

   //Caso o banco de dados foi criado, exclui o mesmo
   if(bd_criado)
    conexao->executarComandoDDL(drop_cmd
                               .arg(modelo->obterNomeSQLObjeto())
                               .arg(modelo->obterNome(true)));

   //Removendo os espaços de tabela e papéis
   for(id_tipo=1; id_tipo >=0; id_tipo--)
   {
    while(idx_objs[id_tipo] >= 0)
    {
     objeto=modelo->obterObjeto(idx_objs[id_tipo], vet_tipos[id_tipo]);

     try
     {
      conexao->executarComandoDDL(drop_cmd
                                  .arg(objeto->obterNomeSQLObjeto())
                                  .arg(objeto->obterNome(true)));
     }
     catch(Excecao &e)
     {}

     idx_objs[id_tipo]--;
    }
   }
  }

  //Exibe no progresso a mensagem de falha
  rot_prog_lbl->setText(trUtf8("Falha na exportação!"));
  rot_prog_lbl->repaint();
  ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_erro.png")));
  ico_lbl->setVisible(true);

  //Oculta os widgets de progresso após 10 segundos
  QTimer::singleShot(10000, this, SLOT(ocultarProgressoExportacao(void)));
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void FormExportacao::ocultarProgressoExportacao(void)
{
 ln2_frm->setVisible(false);
 rot_prog_lbl->setVisible(false);
 prog_pb->setVisible(false);
 ico_lbl->setVisible(false);
 this->resize(this->minimumSize());
}
//-----------------------------------------------------------
void FormExportacao::habilitarTipoExportacao(void)
{
 bool exp_arq=(sender()==exportacao_arq_rb);

 //Widgets habilitados quando a exportação é para arquivo
 modelo_sql_lbl->setEnabled(exp_arq);
 arquivo_lbl->setEnabled(exp_arq);
 arquivo_edt->setEnabled(exp_arq);
 sel_arquivo_tb->setEnabled(exp_arq);
 pgsql_lbl->setEnabled(exp_arq);
 pgsqlvers_cmb->setEnabled(exp_arq);
 dica_lbl->setEnabled(exp_arq);

 //Widgets habilitados quando a exportação é para o sgbd
 modelo_sgbd->setEnabled(!exp_arq);
 conexoes_lbl->setEnabled(!exp_arq);
 conexoes_cmb->setEnabled(!exp_arq);
 pgsqlvers_chk->setEnabled(!exp_arq);
 pgsqlvers1_cmb->setEnabled(!exp_arq && pgsqlvers_chk->isChecked());
 dica1_lbl->setEnabled(!exp_arq);
 dica2_lbl->setEnabled(!exp_arq);
 ignorar_dup_lbl->setEnabled(!exp_arq);
 ignorar_dup_chk->setEnabled(!exp_arq);

 exportar_btn->setEnabled((exportacao_sgbd_rb->isChecked() && conexoes_cmb->count() > 0) ||
                          (exportacao_arq_rb->isChecked() && !arquivo_edt->text().isEmpty()));
}
//-----------------------------------------------------------
void FormExportacao::selecionarArquivoDestino(void)
{
 QFileDialog arquivo_dlg;

 arquivo_dlg.setWindowTitle(trUtf8("Exportar modelo como..."));
 arquivo_dlg.setFilter(trUtf8("Código SQL (*.sql);;Todos os Arquivos (*.*)"));
 arquivo_dlg.setFileMode(QFileDialog::AnyFile);
 arquivo_dlg.setAcceptMode(QFileDialog::AcceptSave);
 arquivo_dlg.setModal(true);

 if(arquivo_dlg.exec()==QFileDialog::Accepted)
  arquivo_edt->setText(arquivo_dlg.selectedFile());

 exportar_btn->setEnabled(!arquivo_edt->text().isEmpty());
}
//***********************************************************
