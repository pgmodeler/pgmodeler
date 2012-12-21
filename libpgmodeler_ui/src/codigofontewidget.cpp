#include "codigofontewidget.h"
#include "progressotarefa.h"
extern ProgressoTarefa *prog_tarefa;

CodigoFonteWidget::CodigoFonteWidget(QWidget *parent): ObjetoBaseWidget(parent)
{
 try
 {
  vector<QString> versoes;
  QFont fonte;

  Ui_CodigoFonteWidget::setupUi(this);
  configurarLayouFormulario(codigofonte_grid, BASE_OBJECT);
  comentario_lbl->setText(trUtf8("Type:"));

  destaque_sql=NULL;
  destaque_xml=NULL;

  fonte=nome_edt->font();
  fonte.setItalic(true);
  comentario_edt->setFont(fonte);
  comentario_edt->setReadOnly(true);
  nome_edt->setFont(fonte);
  nome_edt->setReadOnly(true);

  //Obtém as versões disponíveis de esquemas SQL
  SchemaParser::getPgSQLVersions(versoes);

  //Preenche o combobox de versões
  versoes_cmb->addItems(QStringList(QList<QString>::fromVector(QVector<QString>::fromStdVector(versoes))));

  //Define os atributos do formulários e da janela pai
  janela_pai->setWindowTitle(trUtf8("Source code visualization"));
  janela_pai->definirBotoes(CaixaMensagem::BOTAO_OK);
  janela_pai->setMinimumSize(550, 450);

  //Conecta o botão ok do formulário pai com o método de fechamento do formulário
  connect(janela_pai->aplicar_ok_btn, SIGNAL(clicked(bool)), janela_pai, SLOT(close(void)));

  /* Conecta o sinal de mudança de itens no combo box com a geração da sql do objeto.
     Isso permite que a SQL seja gerada para a versão apropriada, toda vez que o
     usuário mudar um item no combobox */
  connect(versoes_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(gerarCodigoFonteObjeto(int)));
  connect(codigofonte_twg, SIGNAL(currentChanged(int)), this, SLOT(definirAbaCodigoFonte(int)));

  destaque_sql=new DestaqueSintaxe(codigosql_txt, false);
  destaque_xml=new DestaqueSintaxe(codigoxml_txt, false);
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void CodigoFonteWidget::hideEvent(QHideEvent *evento)
{
 versoes_cmb->blockSignals(true);
 codigofonte_twg->blockSignals(true);
 versoes_cmb->setCurrentIndex(0);
 codigofonte_twg->setCurrentIndex(0);
 versoes_cmb->blockSignals(false);
 codigofonte_twg->blockSignals(false);

 //Limpa a caixa de texto de código fonte
 codigosql_txt->clear();
 codigoxml_txt->clear();
 ObjetoBaseWidget::hideEvent(evento);
}

void CodigoFonteWidget::definirAbaCodigoFonte(int)
{
 QString icone_codigo;
 bool campos_ativos=false;
 QPixmap icone;
 ObjectType tipo_obj=objeto->getObjectType();

 //Caso a aba código SQL esteja visível configura o icone de código SQL
 if(codigofonte_twg->currentIndex()==0)
  icone_codigo="codigosql.png";
 else
 //Caso contrário configura o ícone de código XML
  icone_codigo="codigoxml.png";

 /* Os campos de seleção de versão e seus respectivos labels
    devem estar ativados quando a aba de código SQL está ativa
    ou quando o tipo de objeto seja um dos muitos os quais
    possuem definição SQL, caso contrário os campos citados
    aparecerão desabilitados no formulário */
 campos_ativos=(codigofonte_twg->currentIndex()==0 &&
                (tipo_obj!=BASE_RELATIONSHIP && tipo_obj!=OBJ_TEXTBOX));

 //Configura o ícone de tipo de código fonte a ser exibido
 icone=QPixmap(QString(":/icones/icones/") + icone_codigo);
 iconecodigo_lbl->setPixmap(icone);

 //Ativa/desativa os campos de acordo com a aba visível
 versoes_cmb->setEnabled(campos_ativos);
 pgsql_lbl->setEnabled(campos_ativos);
 versao_lbl->setEnabled(campos_ativos);
}

void CodigoFonteWidget::gerarCodigoFonteObjeto(int)
{
 ObjectType tipo_obj;

 try
 {
  //Limpa os campos antes de preenche-los com o código fonte do objeto
  codigosql_txt->clear();
  codigoxml_txt->clear();

  //Obtém o código SQL do objeto caso o mesmo não seja um relacionamento ou caixa de texto
  tipo_obj=objeto->getObjectType();
  if(tipo_obj!=BASE_RELATIONSHIP && tipo_obj!=OBJ_TEXTBOX)
  {
   if(tipo_obj==OBJ_DATABASE)
   {
    prog_tarefa->setWindowTitle(trUtf8("Generating source code..."));
    prog_tarefa->show();
    connect(this->modelo, SIGNAL(s_objetoCarregado(int,QString,unsigned)),
            prog_tarefa, SLOT(executarProgesso(int,QString,unsigned)));
   }

   SchemaParser::setPgSQLVersion(versoes_cmb->currentText());
   codigosql_txt->setPlainText(QString::fromUtf8(DatabaseModel::validarDefinicaoObjeto(objeto, SchemaParser::SQL_DEFINITION)));
  }

  //Caso o objeto não possua uma definição SQL exibe o texto avisando o fato
  if(codigosql_txt->toPlainText()=="")
   codigosql_txt->setPlainText(trUtf8("-- SQL code unavailable for this type of object --"));

  //Configura a caixa de código fonte XML com o código XML do objeto
  codigoxml_txt->setPlainText(QString::fromUtf8(DatabaseModel::validarDefinicaoObjeto(objeto, SchemaParser::XML_DEFINITION)));

  //Configura a aba e os ícones de acordo com a que está em foco
  definirAbaCodigoFonte();

  prog_tarefa->close();
  disconnect(this->modelo, NULL, prog_tarefa, NULL);
 }
 catch(Exception &e)
 {
  prog_tarefa->close();
  disconnect(this->modelo, NULL, prog_tarefa, NULL);
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void CodigoFonteWidget::definirAtributos(DatabaseModel *modelo, BaseObject *objeto)
{
 /* Caso o objeto esteja alocado, preenche o formulário
    com o codigo SQL que o define em banco de dados */
 if(objeto)
 {
  QPixmap icone_obj;

  try
  {
   //Executa a configuração específica do widget
   ObjetoBaseWidget::definirAtributos(modelo, NULL, objeto);
   this->janela_pai->aplicar_ok_btn->setEnabled(true);
   this->obj_protegido_frm->setVisible(false);

   //Configura o icone do objeto a ser exibido
   icone_obj=QPixmap(QString(":/icones/icones/") +
                     BaseObject::getSchemaName(objeto->getObjectType()) + QString(".png"));
   iconeobj_lbl->setPixmap(icone_obj);

   //Preenche os campos do formulario com os atributos do objeto
   comentario_edt->setText(objeto->getTypeName());

   if(!destaque_sql->configuracaoCarregada())
   {
    destaque_sql->carregarConfiguracao(GlobalAttributes::CONFIGURATIONS_DIR +
                                       GlobalAttributes::DIR_SEPARATOR +
                                       GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                       GlobalAttributes::CONFIGURATION_EXT);
   }

   if(!destaque_xml->configuracaoCarregada())
   {
    destaque_xml->carregarConfiguracao(GlobalAttributes::CONFIGURATIONS_DIR +
                                       GlobalAttributes::DIR_SEPARATOR +
                                       GlobalAttributes::XML_HIGHLIGHT_CONF +
                                       GlobalAttributes::CONFIGURATION_EXT);
   }

   gerarCodigoFonteObjeto();
  }
  catch(Exception &e)
  {
   //Redireciona o erro
   throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}

