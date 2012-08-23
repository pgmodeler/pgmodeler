#include "ogtituloobjeto.h"
//***********************************************************
OGTituloObjeto::OGTituloObjeto(void) : ObjetoGrafico(NULL)
{
 esquema=new QGraphicsSimpleTextItem;
 esquema->setZValue(1);

 nome=new QGraphicsSimpleTextItem;
 nome->setZValue(1);

 caixa=new QGraphicsPolygonItem;
 caixa->setZValue(0);

 this->addToGroup(caixa);
 this->addToGroup(esquema);
 this->addToGroup(nome);
}
//-----------------------------------------------------------
OGTituloObjeto::~OGTituloObjeto(void)
{
 this->removeFromGroup(esquema);
 this->removeFromGroup(nome);
 this->removeFromGroup(caixa);
 delete(esquema);
 delete(nome);
 delete(caixa);
}
//-----------------------------------------------------------
void OGTituloObjeto::configurarObjeto(ObjetoGraficoBase *objeto)
{
 QTextCharFormat fmt;
 QString atrib_nome, atrib_nome_esquema, atrib_cor_titulo;
 QPen pen;

 //Dispara uma exceção caso o objeto dono do título não esteja alocado
 if(!objeto)
  throw Excecao(ERR_PGMODELER_OPROBJNAOALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);
 //Dispara uma exceção caso o objeto dono do título não seja uma visão ou tabela
 else if(objeto->obterTipoObjeto()!=OBJETO_TABELA  &&
         objeto->obterTipoObjeto()!=OBJETO_VISAO)
  throw Excecao(ERR_PGMODELER_OPROBJTIPOINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Selecionando os atributos de cores conforme o tipo do objeto dono
 if(objeto->obterTipoObjeto()==OBJETO_VISAO)
 {
  atrib_nome=AtributosParsers::NOME_VISAO;
  atrib_nome_esquema=AtributosParsers::NOME_ESQUEMA_VISAO;
  atrib_cor_titulo=AtributosParsers::TITULO_VISAO;
 }
 else
 {
  atrib_nome=AtributosParsers::NOME_TABELA;
  atrib_nome_esquema=AtributosParsers::NOME_ESQUEMA_TABELA;
  atrib_cor_titulo=AtributosParsers::TITULO_TABELA;
 }

 //Configura o descritor de esquema do objeto
 fmt=config_fonte[atrib_nome_esquema];
 esquema->setFont(fmt.font());
 esquema->setBrush(fmt.foreground());
 esquema->setText(QString::fromUtf8(objeto->obterEsquema()->obterNome() + "."));

 //Configura o descritor de nome do objeto
 fmt=config_fonte[atrib_nome];
 nome->setFont(fmt.font());
 nome->setBrush(fmt.foreground());
 nome->setText(QString::fromUtf8(objeto->obterNome()));

 //Configura a caixa do título
 caixa->setBrush(this->obterEstiloPreenchimento(atrib_cor_titulo));
 pen=this->obterEstiloBorda(atrib_cor_titulo);

 //Caso especial para visão: a borda do título é pontilhada
 if(objeto->obterTipoObjeto()==OBJETO_VISAO)
  pen.setStyle(Qt::DashLine);
 caixa->setPen(pen);

 //Define uma dimensão padrão para o título
 this->redimensionarTitulo(nome->boundingRect().width() + esquema->boundingRect().width() + (2 * ESP_HORIZONTAL),
                           esquema->boundingRect().height() + (2 * ESP_VERTICAL));
}
//-----------------------------------------------------------
void OGTituloObjeto::redimensionarTitulo(float larg, float alt)
{
 QPolygonF pol;
 pol=caixa->polygon();

 //Caso o polígono esteja vazio, cria um com coordenadas padrão
 if(pol.isEmpty())
 {
  pol.append(QPointF(0.0f,0.0f));
  pol.append(QPointF(1.0f,0.0f));
  pol.append(QPointF(1.0f,1.0f));
  pol.append(QPointF(0.0f,1.0f));
 }

 //Redimensiona o polígono e o atribui a caixa
 this->redimensionarPoligono(pol, larg, alt);
 caixa->setPolygon(pol);

 //Define as posições das caixas de esquema e nome
 esquema->setPos((caixa->boundingRect().width() - (esquema->boundingRect().width() + nome->boundingRect().width()))/2.0f, ESP_VERTICAL);
 nome->setPos(esquema->pos().x() + esquema->boundingRect().width(), ESP_VERTICAL);

 //O retângulo de dimensão do título será o próprio retângulo de dimensão da caixa
 this->bounding_rect.setTopLeft(this->pos());
 this->bounding_rect.setSize(QSizeF(caixa->boundingRect().width(), caixa->boundingRect().height()));
}
//***********************************************************
