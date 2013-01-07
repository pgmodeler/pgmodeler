#include "formexportacao.h"
#include "progressotarefa.h"
#include "formconfiguracao.h"
#include "caixamensagem.h"

extern ProgressoTarefa *prog_tarefa;
extern FormConfiguracao *fconfiguracao;
extern CaixaMensagem *caixa_msg;

FormExportacao::FormExportacao(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
 vector<QString> versoes;

 modelo_wgt=NULL;

 setupUi(this);
 connect(exportacao_arq_rb, SIGNAL(toggled(bool)), this, SLOT(habilitarTipoExportacao(void)));
 connect(exportacao_sgbd_rb, SIGNAL(toggled(bool)), this, SLOT(habilitarTipoExportacao(void)));
 connect(exportacao_png_rb, SIGNAL(toggled(bool)), this, SLOT(habilitarTipoExportacao(void)));
 connect(pgsqlvers_chk, SIGNAL(toggled(bool)), pgsqlvers1_cmb, SLOT(setEnabled(bool)));
 connect(fechar_btn, SIGNAL(clicked(bool)), this, SLOT(close(void)));
 connect(sel_arquivo_tb, SIGNAL(clicked(void)), this, SLOT(selecionarArquivoDestino(void)));
 connect(sel_imagem_tb, SIGNAL(clicked(void)), this, SLOT(selecionarArquivoDestino(void)));
 connect(exportar_btn, SIGNAL(clicked(void)), this, SLOT(exportarModelo(void)));

 //Obtém as versões disponíveis de esquemas SQL
 SchemaParser::getPgSQLVersions(versoes);

 //Preenche os comboboxes de versões
 pgsqlvers_cmb->addItems(QStringList(QList<QString>::fromVector(QVector<QString>::fromStdVector(versoes))));
 pgsqlvers1_cmb->addItems(QStringList(QList<QString>::fromVector(QVector<QString>::fromStdVector(versoes))));

 /** ticket#2 **/
 //Fix específico para Windows: força a aparência do frame para WinPanel
 #ifdef Q_OS_WIN
  this->frame->setFrameShape(QFrame::WinPanel);
 #endif
}

void FormExportacao::show(ModeloWidget *modelo)
{
 if(modelo)
 {
  map<QString, DBConnection *> conexoes;
  map<QString, DBConnection *>::iterator itr;
  QString host;

  /* Atualiza a lista de conexões ao exibir o formulário de exportação, obtendo-as
     do widget de configuração de conexões */
  this->modelo_wgt=modelo;
  dynamic_cast<ConfConexoesWidget *>(fconfiguracao->obterWidgetConfiguracao(FormConfiguracao::WGT_CONF_CONEXOES))->obterConexoes(conexoes);

  conexoes_cmb->clear();
  itr=conexoes.begin();

  while(itr!=conexoes.end())
  {
   host=(itr->second)->getConnectionParam(DBConnection::PARAM_SERVER_FQDN);

   if(host.isEmpty())
    host=(itr->second)->getConnectionParam(DBConnection::PARAM_SERVER_IP);

   conexoes_cmb->addItem(itr->first + QString(" (%1)").arg(host),
                         QVariant::fromValue<void *>(itr->second));
   itr++;
  }

  //Esconde os widgets de progresso da exportação
  this->ocultarProgressoExportacao();
  QDialog::show();
 }
}

void FormExportacao::hideEvent(QHideEvent *)
{
 this->modelo_wgt=NULL;
 arquivo_edt->clear();
 exportacao_arq_rb->setChecked(true);
 exportar_btn->setEnabled(false);
}

void FormExportacao::exportarModelo(void)
{
 try
 {
  //Redimensiona a janela para exibição dos widgets de progresso
  this->resize(this->maximumSize());

  //Exibe os widgets de progresso
  prog_pb->setValue(0);
  ln2_frm->setVisible(true);
  rot_prog_lbl->setVisible(true);
  prog_pb->setVisible(true);

  //Inicialmente o ícone de sucesso/erro da exportação fica escondido
  ico_lbl->setVisible(false);

  if(exportacao_png_rb->isChecked())
  {
   QPixmap pix;
   QRectF ret=modelo_wgt->cena->itemsBoundingRect();
   bool exibir_grade, exibir_lim, alin_objs;

   //Faz um backup das opções de grade da cena
   ObjectsScene::getGridOptions(exibir_grade, alin_objs, exibir_lim);

   //Redefine as opções da grade conforme os checkboxes
   ObjectsScene::setGridOptions(grade_chk->isChecked(), false, lim_paginas_chk->isChecked());

   //Cria o pixmap
   pix.resize(ret.size().toSize());

   ///Desenha a cena no pixmap e faz a escala correta
   QPainter p(&pix);
   modelo_wgt->cena->update();
   modelo_wgt->cena->render(&p, QRectF(QPointF(0,0), pix.size()), ret);

   //Restaura as definições da grade
   ObjectsScene::setGridOptions(exibir_grade, alin_objs, exibir_lim);
   modelo_wgt->cena->update();

   //Caso a imagem não possa ser salva, retorna um erro
   if(!pix.save(imagem_edt->text()))
    throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(QString::fromUtf8(imagem_edt->text())),
                  ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);



  }
  else
  {
   int id_tipo;
   QString  versao, buf_sql, cmd_sql;
   DBConnection *conexao=NULL, conex_novo_bd;
   unsigned i, qtd;
   bool bd_criado=false;
   int idx_objs[]={-1, -1};
   ObjectType vet_tipos[]={OBJ_ROLE, OBJ_TABLESPACE};
   BaseObject *objeto=NULL;
   vector<Exception> vet_erros;


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

    //Configura o widget de progresso para capturar o progresso da geração do código
    prog_tarefa->setWindowTitle(trUtf8("Generating source code..."));
    prog_tarefa->show();
    connect(this->modelo_wgt->modelo, SIGNAL(s_objetoCarregado(int,QString,unsigned)),
            prog_tarefa, SLOT(executarProgesso(int,QString,unsigned)));

    rot_prog_lbl->setText(trUtf8("Initializing model export..."));
    rot_prog_lbl->repaint();

    //Caso seja exportação em arquivo
    if(exportacao_arq_rb->isChecked())
    {
     //Define a versão do postgresql a ser adotada
     SchemaParser::setPgSQLVersion(pgsqlvers_cmb->currentText());

     rot_prog_lbl->setText(trUtf8("Saving file '%1'").arg(arquivo_edt->text()));

     //Salva o modelo em arquivo
     modelo_wgt->modelo->saveModel(arquivo_edt->text(), SchemaParser::SQL_DEFINITION);
     prog_pb->setValue(25);
    }
    //Caso seja exportação direto para o SGBD
    else
    {
     //Obtém a conexão selecionada no combo
     conexao=reinterpret_cast<DBConnection *>(conexoes_cmb->itemData(conexoes_cmb->currentIndex()).value<void *>());
     //Tenta se conectar
     conexao->connect();
     //Obtém a versão do servidor PostgreSQL. Essa versão é usada na geração de código a seguir
     versao=(conexao->getDBMSVersion()).mid(0,3);

     /* Caso o checkbox de versão esteja marcada então a versão do servidor é ignorada
      usando aquela escolhida no combo */
     if(pgsqlvers_chk->isChecked())
      SchemaParser::setPgSQLVersion(pgsqlvers1_cmb->currentText());
     else
      SchemaParser::setPgSQLVersion(versao);


     //Cria os Papéis e espaços de tabela separadamente dos demais
     for(id_tipo=0; id_tipo < 2; id_tipo++)
     {
      qtd=modelo_wgt->modelo->getObjectCount(vet_tipos[id_tipo]);
      for(i=0; i < qtd; i++)
      {
       objeto=modelo_wgt->modelo->getObject(i, vet_tipos[id_tipo]);
       rot_prog_lbl->setText(trUtf8("Creating object '%1' (%2)...").arg(QString::fromUtf8(objeto->getName())).arg(objeto->getTypeName()));
       rot_prog_lbl->repaint();

       try
       {
        conexao->executeDDLCommand(objeto->getCodeDefinition(SchemaParser::SQL_DEFINITION));
       }
       catch(Exception &e)
       {
        /* Caso o checkbox de ignorar duplicidade não esteja marcado ou se este está marcado porém a
         informação adicinal da exceção não carrega um dos códigos indicando duplicidade de objeto
         redireciona o erro, caso contrário apenas o ignora */
        if(!ignorar_dup_chk->isChecked() ||
           (ignorar_dup_chk->isChecked() &&
            std::find(vet_cod_erros.begin(), vet_cod_erros.end(), e.getExtraInfo())==vet_cod_erros.end()))
         throw Exception(e.getErrorMessage(),
                       e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
        else
         vet_erros.push_back(e);
       }

       prog_pb->setValue((10 * (id_tipo+1)) + ((i/static_cast<float>(qtd)) * 10));
       idx_objs[id_tipo]++;
      }
     }

     //Cria o banco de dados no servidor
     rot_prog_lbl->setText(trUtf8("Creating database '%1'...").arg(QString::fromUtf8(modelo_wgt->modelo->getName())));
     rot_prog_lbl->repaint();

     try
     {
      conexao->executeDDLCommand(modelo_wgt->modelo->__getCodeDefinition(SchemaParser::SQL_DEFINITION));
      bd_criado=true;
     }
     catch(Exception &e)
     {
      /* Caso o checkbox de ignorar duplicidade não esteja marcado ou se este está marcado porém a
       informação adicinal da exceção não carrega um dos códigos indicando duplicidade de objeto
       redireciona o erro, caso contrário apenas o ignora */
      if(!ignorar_dup_chk->isChecked() ||
         (ignorar_dup_chk->isChecked() &&
          std::find(vet_cod_erros.begin(), vet_cod_erros.end(), e.getExtraInfo())==vet_cod_erros.end()))
       throw Exception(e.getErrorMessage(),
                     e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
      else
       vet_erros.push_back(e);
     }

     prog_pb->setValue(30);

     conex_novo_bd=(*conexao);
     conex_novo_bd.setConnectionParam(DBConnection::PARAM_DB_NAME, modelo_wgt->modelo->getName());
     rot_prog_lbl->setText(trUtf8("Connecting to database '%1'...").arg(QString::fromUtf8(modelo_wgt->modelo->getName())));
     rot_prog_lbl->repaint();
     conex_novo_bd.connect();
     prog_pb->setValue(50);

     //Cria os demais objetos no novo banco
     rot_prog_lbl->setText(trUtf8("Creating objects on database '%1'...").arg(QString::fromUtf8(modelo_wgt->modelo->getName())));
     rot_prog_lbl->repaint();

     //Gera o código SQL de todo o banco
     buf_sql=modelo_wgt->modelo->getCodeDefinition(SchemaParser::SQL_DEFINITION, false);

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

       //Executa um trimm no comando SQL extraído
       cmd_sql=cmd_sql.trimmed();

       //Extrai os caracteres até encontrar o final do buffer ou um ';'
       while(i < qtd)
       {
        /* Caso o caracter seja um ';' e o próximo seja uma quebra de linha, significa o final do comando SQL
           assim a varredura será interrompida */
        if(buf_sql.at(i)==';' && buf_sql.at(i+1)=='\n')
        {
         cmd_sql+=';';
         break;
        }
        else
         cmd_sql+=buf_sql.at(i);
        i++;
       }

       //Executa um trimm no comando SQL extraído
       //cmd_sql=cmd_sql.trimmed();

       //Caso o comando não esteja vazio
       if(!cmd_sql.isEmpty())
       {
        i++;
        //Executa-o na conexão
        conex_novo_bd.executeDDLCommand(cmd_sql);
       }

       prog_pb->setValue(50 + ((i/static_cast<float>(qtd)) * 10));
      }
      catch(Exception &e)
      {
       /* Caso o checkbox de ignorar duplicidade não esteja marcado ou se este está marcado porém a
        informação adicinal da exceção não carrega um dos códigos indicando duplicidade de objeto
        redireciona o erro, caso contrário apenas o ignora */
       if(!ignorar_dup_chk->isChecked() ||
          (ignorar_dup_chk->isChecked() &&
           std::find(vet_cod_erros.begin(), vet_cod_erros.end(), e.getExtraInfo())==vet_cod_erros.end()))
        throw Exception(Exception::getErrorMessage(ERR_EXPORT_FAILURE).arg(QString::fromUtf8(cmd_sql)),
                      ERR_EXPORT_FAILURE,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
       else
        vet_erros.push_back(e);
      }
     }
    }

    prog_tarefa->close();
    disconnect(this->modelo_wgt->modelo, NULL, prog_tarefa, NULL);
   }
   catch(Exception &e)
   {
    QString drop_cmd=QString("DROP %1 %2;");

    prog_tarefa->close();
    disconnect(this->modelo_wgt->modelo, NULL, prog_tarefa, NULL);

    /* Caso os algum objeto tenha sido criado é preciso excluí-los do banco.
     Para isso, os mesmos são removidos na ordem contrária de criação:
     banco de dados, espaço de tabelas e papéis */
    if(bd_criado || idx_objs[0] >= 0 || idx_objs[1] >= 0)
    {
     if(conex_novo_bd.isStablished())
      conex_novo_bd.close();

     //Caso o banco de dados foi criado, exclui o mesmo
     if(bd_criado)
      conexao->executeDDLCommand(drop_cmd
                                  .arg(modelo_wgt->modelo->getSQLName())
                                  .arg(modelo_wgt->modelo->getName(true)));

     //Removendo os espaços de tabela e papéis
     for(id_tipo=1; id_tipo >=0; id_tipo--)
     {
      while(idx_objs[id_tipo] >= 0)
      {
       objeto=modelo_wgt->modelo->getObject(idx_objs[id_tipo], vet_tipos[id_tipo]);

       try
       {
        conexao->executeDDLCommand(drop_cmd
                                    .arg(objeto->getSQLName())
                                    .arg(objeto->getName(true)));
       }
       catch(Exception &e)
       {}

       idx_objs[id_tipo]--;
      }
     }
    }

    if(vet_erros.empty())
     throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
    else
    {
     vet_erros.push_back(e);
     throw Exception(e.getErrorMessage(),__PRETTY_FUNCTION__,__FILE__,__LINE__, vet_erros);
    }
   }
  }

  //Finaliza o progresso da exportação
  prog_pb->setValue(100);
  rot_prog_lbl->setText(trUtf8("Exporting process sucessfuly ended!"));
  rot_prog_lbl->repaint();
  ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));
  ico_lbl->setVisible(true);

  //Oculta os widgets de progresso após 10 segundos
  QTimer::singleShot(5000, this, SLOT(ocultarProgressoExportacao(void)));
 }
 catch(Exception &e)
 {
  //Exibe no progresso a mensagem de falha
  rot_prog_lbl->setText(trUtf8("Error on export!"));
  rot_prog_lbl->repaint();
  ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_erro.png")));
  ico_lbl->setVisible(true);

  //Oculta os widgets de progresso após 10 segundos
  QTimer::singleShot(5000, this, SLOT(ocultarProgressoExportacao(void)));
  caixa_msg->show(e);
 }
}

void FormExportacao::ocultarProgressoExportacao(void)
{
 ln2_frm->setVisible(false);
 rot_prog_lbl->setVisible(false);
 prog_pb->setVisible(false);
 ico_lbl->setVisible(false);
 this->resize(this->minimumSize());
}

void FormExportacao::habilitarTipoExportacao(void)
{
 bool exp_arq=(sender()==exportacao_arq_rb), exp_png=(sender()==exportacao_png_rb);

 //Widgets habilitados quando a exportação é para arquivo
 modelo_sql_lbl->setEnabled(exp_arq);
 arquivo_lbl->setEnabled(exp_arq);
 arquivo_edt->setEnabled(exp_arq);
 sel_arquivo_tb->setEnabled(exp_arq);
 pgsql_lbl->setEnabled(exp_arq);
 pgsqlvers_cmb->setEnabled(exp_arq);
 dica_lbl->setEnabled(exp_arq);

  //Widgets habilitados quando a exportação é para imagem
 modelo_png_lbl->setEnabled(exp_png);
 imagem_lbl->setEnabled(exp_png);
 imagem_edt->setEnabled(exp_png);
 sel_imagem_tb->setEnabled(exp_png);
 dica3_lbl->setEnabled(exp_png);
 grade_chk->setEnabled(exp_png);
 lim_paginas_chk->setEnabled(exp_png);
 opcoes_lbl->setEnabled(exp_png);

 //Widgets habilitados quando a exportação é para o sgbd
 modelo_sgbd->setEnabled(!exp_arq && !exp_png);
 conexoes_lbl->setEnabled(!exp_arq && !exp_png);
 conexoes_cmb->setEnabled(!exp_arq && !exp_png);
 pgsqlvers_chk->setEnabled(!exp_arq && !exp_png );
 pgsqlvers1_cmb->setEnabled(!exp_arq && !exp_png && pgsqlvers_chk->isChecked());
 dica1_lbl->setEnabled(!exp_arq && !exp_png);
 dica2_lbl->setEnabled(!exp_arq && !exp_png);
 ignorar_dup_lbl->setEnabled(!exp_arq && !exp_png);
 ignorar_dup_chk->setEnabled(!exp_arq && !exp_png);

 exportar_btn->setEnabled((exportacao_sgbd_rb->isChecked() && conexoes_cmb->count() > 0) ||
                          (exportacao_arq_rb->isChecked() && !arquivo_edt->text().isEmpty()) ||
                          (exportacao_png_rb->isChecked() && !imagem_edt->text().isEmpty()));
}

void FormExportacao::selecionarArquivoDestino(void)
{
 QFileDialog arquivo_dlg;

 arquivo_dlg.setWindowTitle(trUtf8("Export model as..."));

 if(exportacao_arq_rb->isChecked())
  arquivo_dlg.setFilter(trUtf8("SQL code (*.sql);;All files (*.*)"));
 else
  arquivo_dlg.setFilter(trUtf8("PNG image (*.png);;All files (*.*)"));

 arquivo_dlg.setFileMode(QFileDialog::AnyFile);
 arquivo_dlg.setAcceptMode(QFileDialog::AcceptSave);
 arquivo_dlg.setModal(true);

 if(arquivo_dlg.exec()==QFileDialog::Accepted)
 {
  if(exportacao_arq_rb->isChecked())
   arquivo_edt->setText(arquivo_dlg.selectedFile());
  else
   imagem_edt->setText(arquivo_dlg.selectedFile());
 }

 exportar_btn->setEnabled(!arquivo_edt->text().isEmpty() ||
                          !imagem_edt->text().isEmpty());
}

