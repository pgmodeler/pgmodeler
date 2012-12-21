#include "caixatextowidget.h"

CaixaTextoWidget::CaixaTextoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_TEXTBOX)
{
 Ui_CaixaTextoWidget::setupUi(this);
 configurarLayouFormulario(caixatexto_grid, OBJ_TEXTBOX);
 connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
 connect(sel_cor_tb, SIGNAL(clicked(void)), this, SLOT(selecionarCorTexto(void)));
 janela_pai->definirBotoes(CaixaMensagem::BOTAO_OK_CANCELAR);
 janela_pai->setMinimumSize(500,250);
}

void CaixaTextoWidget::hideEvent(QHideEvent *evento)
{
 QPalette palette;

 //Limpa as caixas de texto
 texto_txt->clear();
 negrito_chk->setChecked(false);
 italico_chk->setChecked(false);
 sublinhado_chk->setChecked(false);

 palette.setColor(QPalette::Button, QColor(0,0,0));
 sel_cor_tb->setPalette(palette);

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}

void CaixaTextoWidget::definirAtributos(DatabaseModel *modelo, OperationList *lista_op, Textbox *caixa_texto, float px_objeto, float py_objeto)
{
 /* Caso a caixa de texto esteja alocada, preenche o formulário
    com os valores deste objeto */
 if(caixa_texto)
 {
  QPalette palette;
  palette.setColor(QPalette::Button, caixa_texto->getTextColor());
  sel_cor_tb->setPalette(palette);

  texto_txt->setPlainText(QString::fromUtf8(caixa_texto->getComment()));
  negrito_chk->setChecked(caixa_texto->getTextAttribute(Textbox::BOLD_TXT));
  italico_chk->setChecked(caixa_texto->getTextAttribute(Textbox::ITALIC_TXT));
  sublinhado_chk->setChecked(caixa_texto->getTextAttribute(Textbox::UNDERLINE_TXT));
 }

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, caixa_texto, NULL, px_objeto, py_objeto);
}

void CaixaTextoWidget::selecionarCorTexto(void)
{
 QColorDialog cor_dlg;
 QPalette palette;

 cor_dlg.setWindowTitle(trUtf8("Select text color"));
 cor_dlg.exec();

 if(cor_dlg.result()==QDialog::Accepted)
 {
  palette.setColor(QPalette::Button, cor_dlg.selectedColor());
  sel_cor_tb->setPalette(palette);
 }
}

void CaixaTextoWidget::aplicarConfiguracao(void)
{
 try
 {
  Textbox *caixa=NULL;

  iniciarConfiguracao<Textbox>();

  caixa=dynamic_cast<Textbox *>(this->objeto);
  //caixa->definirPosicaoObjeto(QPointF(this->px_objeto, this->py_objeto));
  caixa->setComment(texto_txt->toPlainText());
  caixa->setTextAttribute(Textbox::ITALIC_TXT, italico_chk->isChecked());
  caixa->setTextAttribute(Textbox::BOLD_TXT, negrito_chk->isChecked());
  caixa->setTextAttribute(Textbox::UNDERLINE_TXT, sublinhado_chk->isChecked());
  caixa->setTextColor(sel_cor_tb->palette().color(QPalette::Button));

  ObjetoBaseWidget::aplicarConfiguracao();
  finalizarConfiguracao();
 }
 catch(Exception &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelarConfiguracao();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

