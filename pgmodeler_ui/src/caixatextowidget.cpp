#include "caixatextowidget.h"
//***********************************************************
CaixaTextoWidget::CaixaTextoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_CAIXA_TEXTO)
{
 Ui_CaixaTextoWidget::setupUi(this);
 configurarLayouFormulario(caixatexto_grid, OBJETO_CAIXA_TEXTO);
 connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
 connect(sel_cor_tb, SIGNAL(clicked(void)), this, SLOT(selecionarCorTexto(void)));
 janela_pai->definirBotoes(CaixaMensagem::BOTAO_OK_CANCELAR);
 janela_pai->setMinimumSize(500,250);
}
//----------------------------------------------------------
void CaixaTextoWidget::hideEvent(QHideEvent *evento)
{
 QPalette palette;

 //Limpa as caixas de texto
 texto_txt->clear();
 negrito_chk->setChecked(false);
 italico_chk->setChecked(false);

 palette.setColor(QPalette::Button, QColor(0,0,0));
 sel_cor_tb->setPalette(palette);

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void CaixaTextoWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, CaixaTexto *caixa_texto, float px_objeto, float py_objeto)
{
 /* Caso a caixa de texto esteja alocada, preenche o formulário
    com os valores deste objeto */
 if(caixa_texto)
 {
  QPalette palette;
  palette.setColor(QPalette::Button, caixa_texto->obterCorTexto());
  sel_cor_tb->setPalette(palette);

  texto_txt->setPlainText(QString::fromUtf8(caixa_texto->obterComentario()));
  negrito_chk->setChecked(caixa_texto->obterAtributoTexto(CaixaTexto::TEXTO_NEGRITO));
  italico_chk->setChecked(caixa_texto->obterAtributoTexto(CaixaTexto::TEXTO_ITALICO));
 }

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, caixa_texto, NULL, px_objeto, py_objeto);
}
//----------------------------------------------------------
void CaixaTextoWidget::selecionarCorTexto(void)
{
 QColorDialog cor_dlg;
 QPalette palette;

 cor_dlg.setWindowTitle(trUtf8("Selecionar cor de texto"));
 cor_dlg.exec();

 if(cor_dlg.result()==QDialog::Accepted)
 {
  palette.setColor(QPalette::Button, cor_dlg.selectedColor());
  sel_cor_tb->setPalette(palette);
 }
}
//----------------------------------------------------------
void CaixaTextoWidget::aplicarConfiguracao(void)
{
 try
 {
  CaixaTexto *caixa=NULL;

  iniciarConfiguracao<CaixaTexto>();

  caixa=dynamic_cast<CaixaTexto *>(this->objeto);
  //caixa->definirPosicaoObjeto(QPointF(this->px_objeto, this->py_objeto));
  caixa->definirComentario(texto_txt->toPlainText());
  caixa->definirAtributoTexto(CaixaTexto::TEXTO_ITALICO, italico_chk->isChecked());
  caixa->definirAtributoTexto(CaixaTexto::TEXTO_NEGRITO, negrito_chk->isChecked());
  caixa->definirCorTexto(sel_cor_tb->palette().color(QPalette::Button));

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
