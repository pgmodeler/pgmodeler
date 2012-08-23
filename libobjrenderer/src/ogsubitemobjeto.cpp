#include "ogsubitemobjeto.h"
//***********************************************************
const QString OGSubItemObjeto::SEPARADOR_TIPO(" ");
const QString OGSubItemObjeto::SEPARADOR_REST(" ");
const QString OGSubItemObjeto::TEXTO_UNIQUE("uq");
const QString OGSubItemObjeto::TEXTO_PRIMARY_KEY("pk");
const QString OGSubItemObjeto::TEXTO_FOREIGN_KEY("fk");
const QString OGSubItemObjeto::TEXTO_NOT_NULL("nn");
const QString OGSubItemObjeto::DELIMITADOR_REST_INI("«");
const QString OGSubItemObjeto::DELIMITADOR_REST_FIM("»");
//-----------------------------------------------------------
OGSubItemObjeto::OGSubItemObjeto(ObjetoTabela *objeto) : ObjetoGrafico(objeto)
{
 //O descritor é alocado no método de configuração conforme o tipo do objeto de origem
 descritor=NULL;

 //Aloca os rótulos e os adiciona ao grupo
 for(unsigned i=0; i < 3; i++)
 {
  rotulos[i]=new QGraphicsSimpleTextItem;
  this->addToGroup(rotulos[i]);
 }
}
//-----------------------------------------------------------
OGSubItemObjeto::~OGSubItemObjeto(void)
{
 //Desaloca e remove do grupo todos os objetos filhos
 this->removeFromGroup(descritor);
 delete(descritor);

 for(unsigned i=0; i < 3; i++)
 {
  this->removeFromGroup(rotulos[i]);
  delete(rotulos[i]);
 }
}
//-----------------------------------------------------------
void OGSubItemObjeto::configurarDescritor(TipoRestricao tipo_rest)
{
 TipoObjetoBase tipo_obj=OBJETO_BASE;
 Coluna *coluna=dynamic_cast<Coluna *>(this->obterObjetoOrigem());
 float fator=config_fonte[AtributosParsers::GLOBAL].font().pointSizeF()/TAM_PADRAO_FONTE;
 bool desc_elipse=false;

 //Obtém o tipo do objeto de origem, é com base nele que o descritor será alocado
 if(this->obterObjetoOrigem())
  tipo_obj=this->obterObjetoOrigem()->obterTipoObjeto();

 /* Descritores em forma de elipse são alocados para colunas (com ou sem not-null)
    todos os demais tipos têm descritores poligonais */
 desc_elipse=((coluna && tipo_rest==TipoBase::nulo) ||
              (tipo_obj!=OBJETO_INDICE && tipo_obj!=OBJETO_REGRA &&
               tipo_obj!=OBJETO_GATILHO && tipo_obj!=OBJETO_COLUNA));

 //Destrói o descritor quando o tipo atual é diferente do novo
 if(descritor && ((desc_elipse && !dynamic_cast<QGraphicsEllipseItem *>(descritor)) ||
                  (!desc_elipse && dynamic_cast<QGraphicsEllipseItem *>(descritor))))
 {
  this->removeFromGroup(descritor);
  delete(descritor);
  descritor=NULL;
 }

 //Aloca o descritor e o adiciona ao grupo
 if(!descritor)
 {
  //Caso seja em elipse, cria um QGraphicsEllipseItem
  if(desc_elipse)
   descritor=new QGraphicsEllipseItem;
  //Caso contrário que um QGraphicsPolygonItem
  else
   descritor=new QGraphicsPolygonItem;

  this->addToGroup(descritor);
 }

 //Caso o objeto de origem seja coluna
 if(coluna)
 {
  QString atrib;
  QPolygonF pol;

  /* Caso o tipo da restrição não esteja definido (a coluna não tem nenhuma restrição ligada a ela)
     cria um descritor elíptico */
  if(tipo_rest==TipoBase::nulo)
  {
   QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descritor);

   //Cria um descritor elíptico de 10x10 (por padrão) porém aplica o fator entre a fonte padrão e fonte configurada
   desc->setRect(QRectF(QPointF(0,0),
                        QSizeF(9.0f * fator, 9.0f * fator)));

   if(coluna->naoNulo())
    atrib=AtributosParsers::COLUNA_NN;
   else
    atrib=AtributosParsers::COLUNA;

   //Configura o preenchimento de acordo com o atributo selecionado acima
   desc->setBrush(this->obterEstiloPreenchimento(atrib));
   desc->setPen(this->obterEstiloBorda(atrib));
  }
  //Configura o descritor poligonal
  else
  {
   QGraphicsPolygonItem *desc=dynamic_cast<QGraphicsPolygonItem *>(descritor);

   //Cria um polígono conforme o tipo de restrição da coluna
   if(tipo_rest==TipoRestricao::primary_key)
   {
    atrib=AtributosParsers::COLUNA_PK;
    pol.append(QPointF(2,0)); pol.append(QPointF(0,2)); pol.append(QPointF(0,7));
    pol.append(QPointF(2,9)); pol.append(QPointF(3,8)); pol.append(QPointF(3,6));
    pol.append(QPointF(4,6)); pol.append(QPointF(5,7)); pol.append(QPointF(6,6));
    pol.append(QPointF(7,5)); pol.append(QPointF(9,7)); pol.append(QPointF(9,3));
    pol.append(QPointF(3,3)); pol.append(QPointF(3,1));
   }
   else if(tipo_rest==TipoRestricao::foreign_key)
   {
    atrib=AtributosParsers::COLUNA_FK;
    pol.append(QPointF(0,3)); pol.append(QPointF(0,6)); pol.append(QPointF(4,6));
    pol.append(QPointF(4,9)); pol.append(QPointF(5,9)); pol.append(QPointF(9,5));
    pol.append(QPointF(9,4)); pol.append(QPointF(5,0)); pol.append(QPointF(4,0));
    pol.append(QPointF(4,3));
   }
   else if(tipo_rest==TipoRestricao::unique)
   {
    atrib=AtributosParsers::COLUNA_UQ;
    pol.append(QPointF(4,0)); pol.append(QPointF(0,4)); pol.append(QPointF(0,5));
    pol.append(QPointF(4,9)); pol.append(QPointF(5,9)); pol.append(QPointF(9,5));
    pol.append(QPointF(9,4)); pol.append(QPointF(5,0));
   }

   if(fator!=1.0f)
    this->redimensionarPoligono(pol,
                                pol.boundingRect().width() * fator,
                                pol.boundingRect().height()  * fator);

   //Atribui o polígono configurado e configura o estilo de cores do descritor
   desc->setPolygon(pol);
   desc->setBrush(this->obterEstiloPreenchimento(atrib));
   desc->setPen(this->obterEstiloBorda(atrib));
  }
 }
 //Configura um descritor poligonal para indice, regra ou gatilho
 else if(tipo_obj==OBJETO_INDICE ||
         tipo_obj==OBJETO_REGRA ||
         tipo_obj==OBJETO_GATILHO)
 {
  ObjetoTabela *objeto_tab=dynamic_cast<ObjetoTabela *>(this->obterObjetoOrigem());
  QGraphicsPolygonItem *desc=dynamic_cast<QGraphicsPolygonItem *>(descritor);
  QPolygonF pol;

  pol.append(QPointF(5,0)); pol.append(QPointF(0,5)); pol.append(QPointF(4,9));
  pol.append(QPointF(9,9)); pol.append(QPointF(9,4));

  if(fator!=1.0f)
   this->redimensionarPoligono(pol,
                               pol.boundingRect().width() * fator ,
                               pol.boundingRect().height() * fator);

  desc->setPolygon(pol);
  desc->setBrush(this->obterEstiloPreenchimento(objeto_tab->obterNomeEsquemaObjeto()));
  desc->setPen(this->obterEstiloBorda(objeto_tab->obterNomeEsquemaObjeto()));
 }
 //Configura um descritor elíptico padrão (usado para referências de visões)
 else
 {
  QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descritor);

  desc->setRect(QRectF(QPointF(0,0),
                       QSizeF(9.0f * fator, 9.0f * fator)));
  desc->setBrush(this->obterEstiloPreenchimento(AtributosParsers::REFERENCIA));
  desc->setPen(this->obterEstiloBorda(AtributosParsers::REFERENCIA));
 }
}
//-----------------------------------------------------------
void OGSubItemObjeto::configurarObjeto(void)
{
 //Caso haja um objeto de tabela atribuído ao subitem
 if(this->obterObjetoOrigem())
 {
  QTextCharFormat fmt;
  float px;
  QString str_rest;
  ObjetoTabela *objeto_tab=dynamic_cast<ObjetoTabela *>(this->obterObjetoOrigem());
  Coluna *coluna=dynamic_cast<Coluna *>(objeto_tab);
  TipoRestricao tipo_rest=TipoRestricao::nulo;

  //Caso seja uma coluna
  if(coluna)
  {
   //Obtém a string de restrições relacionadas à coluna
   str_rest=this->obterStringRestricoes(coluna);

   /* Determina o tipo primário de restrição conforme as checagens a seguir.
      É com base no tipo da restrição que o descritor será criado. Adicionalmente
      obtém a formatação de fonte para o tipo da restrição */
   if(str_rest.find(TEXTO_PRIMARY_KEY)>=0)
   {
    fmt=config_fonte[AtributosParsers::COLUNA_PK];
    tipo_rest=TipoRestricao::primary_key;
   }
   else if(str_rest.find(TEXTO_FOREIGN_KEY)>=0)
   {
    fmt=config_fonte[AtributosParsers::COLUNA_FK];
    tipo_rest=TipoRestricao::foreign_key;
   }
   else if(str_rest.find(TEXTO_UNIQUE)>=0)
   {
    fmt=config_fonte[AtributosParsers::COLUNA_UQ];
    tipo_rest=TipoRestricao::unique;
   }
   else if(str_rest.find(TEXTO_NOT_NULL)>=0)
    fmt=config_fonte[AtributosParsers::COLUNA_NN];
   else
    fmt=config_fonte[AtributosParsers::COLUNA];

   if(coluna->incluidoPorRelacionamento())
    fmt=config_fonte[AtributosParsers::COLUNA_HERDADA];
   else if(coluna->objetoProtegido())
    fmt=config_fonte[AtributosParsers::COLUNA_PROTEGIDA];
  }
  //Caso não seja uma coluna, obtém a formatação para o tipo do objeto de tabela
  else
   fmt=config_fonte[objeto_tab->obterNomeEsquemaObjeto()];

  //Configura o descritor com o tipo da restrição
  configurarDescritor(tipo_rest);

  //Posiciona o descritor como o primeiro item
  descritor->setPos(ESP_HORIZONTAL, 1);
  px=descritor->pos().x() + descritor->boundingRect().width() + (2 * ESP_HORIZONTAL);

  /* Configurando os rótulos do subitem.
     Os rótulos do subitem têm o seguinte esquema: [nome do objeto] [tipo] [restrições] */
  rotulos[0]->setText(QString::fromUtf8(objeto_tab->obterNome()));
  rotulos[0]->setFont(fmt.font());
  rotulos[0]->setBrush(fmt.foreground());
  rotulos[0]->setPos(px, 0);
  px+=rotulos[0]->boundingRect().width();

  //Configura o rótulo de tipo
  fmt=config_fonte[AtributosParsers::TIPO_OBJETO];
  if(coluna)
   rotulos[1]->setText(QString::fromUtf8(SEPARADOR_TIPO + (*coluna->obterTipo())));
  else
   rotulos[1]->setText(QString::fromUtf8(SEPARADOR_TIPO + objeto_tab->obterNomeEsquemaObjeto()));

  rotulos[1]->setFont(fmt.font());
  rotulos[1]->setBrush(fmt.foreground());
  rotulos[1]->setPos(px, 0);
  px+=rotulos[1]->boundingRect().width() + (3 * ESP_HORIZONTAL);

  /* Configura o rótulo de restrições. Para objetos índice, regras e gatilho
     o rótulo de restrições armazena informações sobre modo de disparo,
     eventos, entre outros */
  fmt=config_fonte[AtributosParsers::RESTRICOES];
  if(coluna)
   rotulos[2]->setText(QString::fromUtf8(str_rest));
  else
  {
   Regra *regra=dynamic_cast<Regra *>(objeto_tab);
   Gatilho *gatilho=dynamic_cast<Gatilho *>(objeto_tab);
   Indice *indice=dynamic_cast<Indice *>(objeto_tab);

   //Configurando a string de restrições para regra
   if(regra)
   {
    str_rest+=(~regra->obterTipoEvento()).mid(3,1);
    str_rest+=SEPARADOR_REST;
    str_rest+=(~regra->obterTipoExecucao()).mid(0,1);
    str_rest=str_rest.lower();
   }
   //Configurando a string de restrições para gatilho
   else if(gatilho)
   {
    str_rest+=(~gatilho->obterTipoDisparo()).mid(0,1);
    str_rest+=SEPARADOR_REST;

    for(unsigned i=TipoEvento::on_insert; i < TipoEvento::on_truncate; i++)
    {
     if(gatilho->executaNoEvento(TipoEvento(i)))
      str_rest+=(~TipoEvento(i)).mid(3,1);
    }
    str_rest=str_rest.lower();
   }
   //Configurando a string de restrições para índice
   else if(indice)
   {
    if(indice->obterAtributo(Indice::UNIQUE))
     str_rest+="u";

    if(indice->obterAtributo(Indice::CONCORRENTE))
     str_rest+="c";

    if(indice->obterAtributo(Indice::ATUAL_RAPIDA))
     str_rest+="f";
   }

   if(!str_rest.isEmpty())
    rotulos[2]->setText(QString::fromUtf8(DELIMITADOR_REST_INI + " " +
                                          str_rest + " " +
                                          DELIMITADOR_REST_FIM));

  }

  rotulos[2]->setFont(fmt.font());
  rotulos[2]->setBrush(fmt.foreground());
  rotulos[2]->setPos(px, 0);

  //Calcula o retângulo de dimensão do subitem, que é composto pela junção de todas as dimensões dos objetos (descritor e rótulos)
  descritor->setPos(ESP_HORIZONTAL, rotulos[0]->boundingRect().center().y() - descritor->boundingRect().center().y());
  bounding_rect.setTopLeft(QPointF(descritor->boundingRect().left(), rotulos[0]->boundingRect().top()));

  //Caso particular: Caso o rótulo de restrições esteja vazio usa a dimensão do rótulo de tipo
  if(rotulos[2]->boundingRect().width()==0)
   bounding_rect.setBottomRight(QPointF(rotulos[1]->boundingRect().right(), rotulos[0]->boundingRect().bottom()));
  else
   bounding_rect.setBottomRight(QPointF(rotulos[2]->boundingRect().right(), rotulos[0]->boundingRect().bottom()));
 }
}
//-----------------------------------------------------------
void OGSubItemObjeto::configurarObjeto(Referencia referencia)
{
 QTextCharFormat fmt;
 float px;
 QString str_aux;

 //Configura e posiciona o descritor da referência
 configurarDescritor();
 descritor->setPos(ESP_HORIZONTAL, 1);
 px=descritor->pos().x() + descritor->boundingRect().width() + (2 * ESP_HORIZONTAL);

 //Caso o tipo da referência seja a uma coluna
 if(referencia.obterTipoReferencia()==Referencia::REFER_COLUNA)
 {
  //Configura o rótulo de nome no formato: [tabela].[coluna]
  fmt=config_fonte[AtributosParsers::TABELA_REF];
  rotulos[0]->setText(referencia.obterTabela()->obterNome() + ".");
  rotulos[0]->setFont(fmt.font());
  rotulos[0]->setBrush(fmt.foreground());
  rotulos[0]->setPos(px, 0);
  px+=rotulos[0]->boundingRect().width();

  fmt=config_fonte[AtributosParsers::COLUNA_REF];
  if(referencia.obterColuna())
   rotulos[1]->setText(referencia.obterColuna()->obterNome());
  else
   rotulos[1]->setText("*");

  rotulos[1]->setFont(fmt.font());
  rotulos[1]->setBrush(fmt.foreground());
  rotulos[1]->setPos(px, 0);
  px+=rotulos[1]->boundingRect().width();
 }
 //Caso a referência seja a uma expressão
 else
 {
  fmt=config_fonte[AtributosParsers::TABELA_REF];

  //Trunca a expressão em 20 caracters caso a mesma ultrapasse este comprimento
  str_aux=referencia.obterExpressao().mid(0,20);
  if(referencia.obterExpressao().size() > 20) str_aux+="...";

  rotulos[0]->setText(str_aux);
  rotulos[0]->setFont(fmt.font());
  rotulos[0]->setBrush(fmt.foreground());
  rotulos[1]->setText("");
  rotulos[0]->setPos(px, 0);
  px+=rotulos[0]->boundingRect().width();
 }

 //Caso a referência possua um alias configura o rótulo para exibi-lo
 if((referencia.obterColuna() && referencia.obterAliasColuna()!="") ||
    (referencia.obterAlias()!="" && referencia.obterTipoReferencia()==Referencia::REFER_EXPRESSAO))
 {
  if(referencia.obterTipoReferencia()==Referencia::REFER_EXPRESSAO)
   str_aux=referencia.obterAlias();
  else
   str_aux=referencia.obterAliasColuna();

  str_aux=" (" + str_aux + ") ";
  fmt=config_fonte[AtributosParsers::ALIAS];
  rotulos[2]->setText(str_aux);
  rotulos[2]->setFont(fmt.font());
  rotulos[2]->setBrush(fmt.foreground());
  rotulos[2]->setPos(px, 0);
 }

 //Configura o retângulo de dimensão do subitem
 descritor->setPos(ESP_HORIZONTAL, rotulos[0]->boundingRect().center().y() - descritor->boundingRect().center().y());
 bounding_rect.setTopLeft(QPointF(descritor->pos().x(), rotulos[0]->pos().y()));

 if(rotulos[2]->text().isEmpty())
  bounding_rect.setBottomRight(QPointF(rotulos[1]->boundingRect().right(), rotulos[0]->boundingRect().bottom()));
 else
  bounding_rect.setBottomRight(QPointF(rotulos[2]->boundingRect().right(), rotulos[0]->boundingRect().bottom()));
}
//-----------------------------------------------------------
void OGSubItemObjeto::definirPosXObjetoFilho(unsigned idx_obj, float px)
{
 if(idx_obj >= 4)
  throw Excecao(ERR_PGMODELER_REFOBJIDXINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 if(idx_obj==0)
  descritor->setPos(px, descritor->pos().y());
 else
  rotulos[idx_obj-1]->setPos(px, rotulos[idx_obj-1]->pos().y());
}
//-----------------------------------------------------------
QGraphicsItem *OGSubItemObjeto::obterObjetoFilho(unsigned idx_obj)
{
 if(idx_obj >= 4)
  throw Excecao(ERR_PGMODELER_REFOBJIDXINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 if(idx_obj==0)
  return(descritor);
 else
  return(rotulos[idx_obj-1]);
}
//-----------------------------------------------------------
QString OGSubItemObjeto::obterStringRestricoes(Coluna *coluna)
{
 if(coluna && coluna->obterTabelaPai())
 {
  Tabela *tabela=dynamic_cast<Tabela *>(coluna->obterTabelaPai());
  QString str_rest;
  Restricao *restricao=NULL;
  vector<Restricao *>::iterator itr,itr_end;
  vector<Restricao *> restricoes;
  TipoRestricao tipo_rest;
  unsigned i, qtd;

  qtd=tabela->obterNumRestricoes();
  for(i=0; i < qtd; i++)
   restricoes.push_back(tabela->obterRestricao(i));

  //Obtém as referências ao primeiro e último elemento da lita de constraints
  itr=restricoes.begin();
  itr_end=restricoes.end();

  while(itr!=itr_end)
  {
   restricao=(*itr); //Obtém uma constraint
   itr++; //Passa para a próxima da lista

   //Verfica se a coluna está sendo referenciada dentro da constraint
   if(restricao->obterColuna(coluna->obterNome(), Restricao::COLUNA_ORIGEM))
   {
    //Obtém o tipo da constraint
    tipo_rest=restricao->obterTipoRestricao();

    //Para cada tipo de constraint concatena-se o texto referente
    //   a mesma e um separadar de textos de constraints
    if(tipo_rest==TipoRestricao::primary_key)
     str_rest=TEXTO_PRIMARY_KEY + SEPARADOR_REST;
    else if(tipo_rest==TipoRestricao::foreign_key)
     str_rest+=TEXTO_FOREIGN_KEY + SEPARADOR_REST;
    else if(tipo_rest==TipoRestricao::unique)
     str_rest+=TEXTO_UNIQUE + SEPARADOR_REST;
   }
  }

  //Caso a coluna seja não-nula adiciona a QString formatada o texto referente à constraint NOT NULL
  if(coluna->naoNulo()) str_rest+=TEXTO_NOT_NULL + SEPARADOR_REST;

  //Caso a QString formatada tenha sido criada
  if(str_rest!="")
   //Termina sua formatação concatenando a QString formatada aos caracteres delimitadores de restricoes
   str_rest= DELIMITADOR_REST_INI +
              SEPARADOR_REST + str_rest +
              DELIMITADOR_REST_FIM;

  return(str_rest);
 }
 else return("");
}
//***********************************************************
