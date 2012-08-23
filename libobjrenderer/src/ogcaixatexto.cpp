#include "ogcaixatexto.h"
//***********************************************************
OGCaixaTexto::OGCaixaTexto(CaixaTexto *cxtexto, const QBrush &brush, const QPen &pen) : ObjetoGrafico(cxtexto)
{
 connect(cxtexto, SIGNAL(s_objetoModificado(void)), this, SLOT(configurarObjeto(void)));

 //Aloca os objetos que definem a representação gráfica de caixa de texto
 caixa=new QGraphicsPolygonItem;
 texto=new QGraphicsSimpleTextItem;

 //Caso o brush ou pen não estejam especificados usa a cor padrão da caixa de texto
 if(brush.style()==Qt::NoBrush || pen.style()==Qt::NoPen)
 {
  caixa->setBrush(this->obterEstiloPreenchimento(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_CAIXA_TEXTO)));
  caixa->setPen(this->pen=this->obterEstiloBorda(ObjetoBase::obterNomeEsquemaObjeto(OBJETO_CAIXA_TEXTO)));
 }
 else
 {
  //Configura as cores da caixa de texto
  caixa->setBrush(brush);
  caixa->setPen(pen);
 }

 //Agrupa os objetos alocados e efetua a configuração gráfica da caixa de texto
 this->addToGroup(texto);
 this->addToGroup(caixa);
 this->configurarObjeto();
}
//-----------------------------------------------------------
OGCaixaTexto::~OGCaixaTexto(void)
{
 disconnect(this, SLOT(configurarObjeto(void)));

 this->removeFromGroup(caixa);
 this->removeFromGroup(texto);
 delete(caixa);
 delete(texto);
}
//-----------------------------------------------------------
void OGCaixaTexto::configurarObjeto(void)
{
 CaixaTexto *cxtexto=dynamic_cast<CaixaTexto *>(this->obterObjetoOrigem());
 QTextCharFormat fmt=config_fonte[AtributosParsers::GLOBAL];
 QFont fonte;
 QPolygonF poligono;

 //Cria o polígono que define a caixa de texto em si
 poligono.append(QPointF(0.0f,0.0f));
 poligono.append(QPointF(1.0f,0.0f));
 poligono.append(QPointF(1.0f,1.0f));
 poligono.append(QPointF(0.0f,1.0f));

 //A caixa deve ficar abaixo dos demais objetos por isso um Z=0
 caixa->setZValue(0);

 /* Configura o estilo da fonte do texto conforme os parâmetros definidos
    na caixa de texto que deu origem ao objeto gráfico */
 fonte=fmt.font();
 fonte.setItalic(cxtexto->obterAtributoTexto(CaixaTexto::TEXTO_ITALICO));
 fonte.setBold(cxtexto->obterAtributoTexto(CaixaTexto::TEXTO_NEGRITO));
 fonte.setUnderline(cxtexto->obterAtributoTexto(CaixaTexto::TEXTO_SUBLINHADO));

 //Configura o texto e o estilo de fonte da caixa
 texto->setText(QString::fromUtf8(cxtexto->obterComentario()));
 texto->setFont(fonte);
 texto->setZValue(1);
 texto->setBrush(cxtexto->obterCorTexto());

 //Move o texto para a posição inicial considerando os espaçamentos vertical e horizontal
 texto->setPos(ESP_HORIZONTAL, ESP_VERTICAL);

 /* Redimensiona o polígono da caixa de texto para que seu tamanho seja compatível
    com o tamanho do texto */
 this->redimensionarPoligono(poligono, roundf(texto->boundingRect().width() + (2 * ESP_HORIZONTAL)),
                                       roundf(texto->boundingRect().height() + (2* ESP_VERTICAL)));
 caixa->setPos(0,0);
 caixa->setPolygon(poligono);

 /* Configura a posição do ícone de protegido do objeto, para que
    este esteja situado no canto inferior direito da caixa de texto */
 icone_protegido->setPos(caixa->boundingRect().right() - (icone_protegido->boundingRect().width() + 2 * ESP_HORIZONTAL),
                         caixa->boundingRect().bottom()- (icone_protegido->boundingRect().height() + 2 * ESP_VERTICAL));

 this->bounding_rect.setTopLeft(caixa->boundingRect().topLeft());
 this->bounding_rect.setBottomRight(caixa->boundingRect().bottomRight());

 ObjetoGrafico::configurarObjeto();
 ObjetoGrafico::configurarSombraObjeto();
 ObjetoGrafico::configurarSelecaoObjeto();
}
//***********************************************************
