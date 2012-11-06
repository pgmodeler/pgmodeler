#include "relacionamentobase.h"
#include <QApplication>

RelacionamentoBase::RelacionamentoBase(RelacionamentoBase *relacao)
{
 if(!relacao)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Inicializa com NULL os rótulos pois os mesmos são copiados no operador =
 for(unsigned i=0; i < 3; i++)
  rotulos[i]=NULL;

 (*(this))=(*relacao);
}

RelacionamentoBase::RelacionamentoBase(/*const QString &nome,*/ unsigned tipo_rel,
                                       TabelaBase *tab_orig, TabelaBase *tab_dest,
                                        bool obrig_orig, bool obrig_dest)

{
 try
 {
  QString str_aux;

  this->conectado=false;
  this->obrig_orig=obrig_orig;
  this->obrig_dest=obrig_dest;
  this->tabela_orig=tab_orig;
  this->tabela_dest=tab_dest;
  this->tipo_relac=tipo_rel;

  for(unsigned i=0; i < 3; i++)
  {
   rotulos[i]=NULL;
   dist_rotulos[i]=QPointF(NAN, NAN);
  }

  configurarRelacionamento();

  str_aux=QApplication::translate("RelacionamentoBase","rel_%1_%2","",QApplication::UnicodeUTF8);
  setName(str_aux.arg(tab_orig->getName()).arg(tab_dest->getName()));
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void RelacionamentoBase::configurarRelacionamento(void)
{
 obj_type=BASE_RELATIONSHIP;

 attributes[ParsersAttributes::TYPE]="";
 attributes[ParsersAttributes::SRC_REQUIRED]="";
 attributes[ParsersAttributes::DST_REQUIRED]="";
 attributes[ParsersAttributes::SRC_TABLE]="";
 attributes[ParsersAttributes::DST_TABLE]="";
 attributes[ParsersAttributes::POINTS]="";
 attributes[ParsersAttributes::COLUMNS]="";
 attributes[ParsersAttributes::CONSTRAINTS]="";
 attributes[ParsersAttributes::ELEMENTS]="";
 attributes[ParsersAttributes::SRC_SUFFIX]="";
 attributes[ParsersAttributes::DST_SUFFIX]="";
 attributes[ParsersAttributes::AUTO_SUFFIX]="";
 attributes[ParsersAttributes::IDENTIFIER]="";
 attributes[ParsersAttributes::REDUCED_FORM]="";
 attributes[ParsersAttributes::DEFERRABLE]="";
 attributes[ParsersAttributes::DEFER_TYPE]="";
 attributes[ParsersAttributes::TABLE_NAME]="";
 attributes[ParsersAttributes::SPECIAL_PK_COLS]="";


 //Verifica se o tipo de relacionamento é valido
 if(tipo_relac <= RELACIONAMENTO_DEP)
 {
  try
  {
   /* Verifica se uma das tabelas envolvidas no relacionamentos
      não estão alocadas, caso isso ocorra, dispara uma exceção */
   if(!tabela_orig || !tabela_dest)
    throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_TABLE)
                         .arg(QString::fromUtf8(this->getName()))
                         .arg(BaseObject::getTypeName(BASE_RELATIONSHIP)),
                  ERR_ASG_NOT_ALOC_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   /* Caso o tipo de relacionamento seja de generalização ou de dependência
      e as tabelas de origem e destino forem iguais, será disparada uma exceção
      pois uma tabela não pode herdar/copiar atributos dela mesma */
   if((tipo_relac==RELACIONAMENTO_GEN ||
       tipo_relac==RELACIONAMENTO_DEP) && tabela_orig==tabela_dest)
    throw Exception(ERR_INV_INH_COPY_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   /* Aloca o rótulo de nome do relacionamento, todos os tipos de relacionamento
      possui esse rótulo */
   rotulos[ROTULO_NOME_RELAC]=new CaixaTexto;
   rotulos[ROTULO_NOME_RELAC]->definirAtributoTexto(CaixaTexto::TEXTO_ITALICO, true);

   /* Alocando os rótulos de cardinalidade, exceto para os relacionamentos
      de generalização e dependência, pois esses não trabalham com cardinalidade */
   if(tipo_relac!=RELACIONAMENTO_GEN &&
      tipo_relac!=RELACIONAMENTO_DEP)
   {
    //Aloca os rótulos
    rotulos[ROTULO_CARD_ORIGEM]=new CaixaTexto;
    rotulos[ROTULO_CARD_DESTINO]=new CaixaTexto;
    rotulos[ROTULO_CARD_ORIGEM]->definirAtributoTexto(CaixaTexto::TEXTO_ITALICO, true);
    rotulos[ROTULO_CARD_DESTINO]->definirAtributoTexto(CaixaTexto::TEXTO_ITALICO, true);

    /* Define a obrigatoriedade das tabelas e já cria o texto
       dos rótulos de cardinalidade */
    definirTabelaObrigatoria(TABELA_ORIGEM,obrig_orig);
    definirTabelaObrigatoria(TABELA_DESTINO,obrig_dest);
   }
  }
  catch(bad_alloc &e)
  {
   throw Exception(ERR_GLOBAL_OBJBADALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
 }
 else
  throw Exception(ERR_ALOC_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

RelacionamentoBase::~RelacionamentoBase(void)
{
 //Desconecta o relacionamento
 desconectarRelacionamento();

 //Exclui os rótulos
 for(unsigned i=0; i<3; i++)
  if(rotulos[i]) delete(rotulos[i]);
}

void RelacionamentoBase::setName(const QString &nome)
{
 try
 {
  BaseObject::setName(nome);

  if(rotulos[ROTULO_NOME_RELAC])
   rotulos[ROTULO_NOME_RELAC]->setComment(nome);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void RelacionamentoBase::definirTabelaObrigatoria(unsigned id_tabela, bool valor)
{
 QString cmin, aux;
 unsigned tipo_rot;

 /* Retona um erro ao usuário indicando que a combinação de cardinalidade
    e tipo de relacionamento não é implementado. O unico caso onde isso
    acontece é no relacionamento do tipo (1,1)-<>-(1,1) */
 if(tipo_relac==RELACIONAMENTO_11 &&
    ((id_tabela==TABELA_ORIGEM && valor && obrig_dest) ||
     (id_tabela==TABELA_DESTINO && valor && obrig_orig)))
  throw Exception(ERR_NOT_IMPL_REL_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso a tabela de origem seja obrigatório
 if(id_tabela==TABELA_ORIGEM)
 {
  obrig_orig=valor;
  /* Indica que o rótulo de cardinalidade
     de origem será configurado */
  tipo_rot=ROTULO_CARD_ORIGEM;
 }
 else
 {
  if(tipo_relac!=RELACIONAMENTO_1N)
   obrig_dest=valor;
  else
   /* Relacionamento do tipo 1-n,  a entidade do lado
      n sempre será no formato (0,n) */
   obrig_dest=false;

  /* Indica que o rótulo de cardinalidade
     de destino será configurado */
  tipo_rot=ROTULO_CARD_DESTINO;
 }

 /* Caso o valor seja false, indica que a participação
    da tabela em questão é opcional */
 if(!valor) cmin="0";
 else cmin="1";

 if(rotulos[tipo_rot])
 {
  //Configurando o rótulo para o relacionamento 1-1
  if(tipo_relac==RELACIONAMENTO_11)
   rotulos[tipo_rot]->setComment("(" + cmin + ",1)");
  //Configurando o rótulo para o relacionamento 1-n
  else if(tipo_relac==RELACIONAMENTO_1N)
  {
   aux=(id_tabela==TABELA_ORIGEM ? "1" : "n");
   rotulos[tipo_rot]->setComment("(" + cmin + "," + aux + ")");
  }
  else if(tipo_relac==RELACIONAMENTO_NN)
  {
   /* A cardinalidade de relacionamentos n-n sempre será 0-n pois
      será gerada uma tabela própria, portanto a cardinalidade mínima
      é ignorada */
   //rotulos[tipo_rot]->definirComentario("(" + cmin + ",n)");
   rotulos[tipo_rot]->setComment("(n)");
  }
  rotulos[tipo_rot]->setModefied(true);
 }
}

TabelaBase *RelacionamentoBase::obterTabela(unsigned id_tabela)
{
 //Retorna a tabela de acordo o índice passado
 if(id_tabela==TABELA_ORIGEM)
  return(tabela_orig);
 else if(id_tabela==TABELA_DESTINO)
  return(tabela_dest);
 else
  return(NULL);
}

bool RelacionamentoBase::tabelaObrigatoria(unsigned id_tabela)
{
 /* Retorna o flag de obrigatoriedade de acordo o índice
    da tabela passado */
 if(id_tabela==TABELA_ORIGEM)
  return(obrig_orig);
 else
  return(obrig_dest);
}

void RelacionamentoBase::desconectarRelacionamento(void)
{
 if(conectado)
 {
  conectado=false;
  tabela_orig->setModefied(true);
  tabela_dest->setModefied(true);
 }
}

void RelacionamentoBase::conectarRelacionamento(void)
{
 //Caso o relacionamento não esteja conectado
 if(!conectado)
 {
  //Indica que o relacionamento está conectado
  conectado=true;

  /* Marca que as tabelas do relacionamentos estão modificadas para forçar
     o redimensionamento das mesmas */
  tabela_orig->setModefied(true);
  tabela_dest->setModefied(true);
 }
}

CaixaTexto *RelacionamentoBase::obterRotulo(unsigned idx_rot)
{
 // Retorna o rótulo na posição especificada, caso o índice
 //   do mesmo seja válido
 if(idx_rot<=ROTULO_NOME_RELAC)
  return(rotulos[idx_rot]);
 else
  //Dispara uma exceção caso o índice usado seja inválido
  throw Exception(ERR_REF_LABEL_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
 
unsigned RelacionamentoBase::obterTipoRelacionamento(void)
{
 return(tipo_relac);
}

bool RelacionamentoBase::relacionamentoConectado(void)
{
 return(conectado);
}

bool RelacionamentoBase::autoRelacionamento(void)
{
 return(tabela_dest==tabela_orig);
}

void RelacionamentoBase::definirAtributosRelacionamento(void)
{
 unsigned qtd, i;
 QString str_aux,
         atribs_rot[3]={ ParsersAttributes::SRC_LABEL,
                         ParsersAttributes::DST_LABEL,
                         ParsersAttributes::NAME_LABEL};

 //Definindo o atributo de tipo de relacionamento
 switch(tipo_relac)
 {
  case RELACIONAMENTO_11: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_11; break;
  case RELACIONAMENTO_1N: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_1N; break;
  case RELACIONAMENTO_NN: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_NN; break;
  case RELACIONAMENTO_GEN: attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_GEN; break;
  default:
    if(tabela_orig->getType()==OBJ_VIEW)
     attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATION_TAB_VIEW;
    else
     attributes[ParsersAttributes::TYPE]=ParsersAttributes::RELATIONSHIP_DEP;
  break;
 }

 attributes[ParsersAttributes::SRC_REQUIRED]=(obrig_orig ? "1" : "");
 attributes[ParsersAttributes::DST_REQUIRED]=(obrig_dest ? "1" : "");

 if(tabela_orig)
  attributes[ParsersAttributes::SRC_TABLE]=tabela_orig->getName(true);

 if(tabela_dest)
  attributes[ParsersAttributes::DST_TABLE]=tabela_dest->getName(true);


 //Criando a definição XML da linha do relacionamento
 qtd=pontos.size();
 for(i=0; i < qtd; i++)
 {
  attributes[ParsersAttributes::X_POS]=QString("%1").arg(pontos[i].x());
  attributes[ParsersAttributes::Y_POS]=QString("%1").arg(pontos[i].y());
  str_aux+=SchemaParser::getObjectDefinition(ParsersAttributes::POSITION,
                                                attributes, SchemaParser::XML_DEFINITION);
 }
 attributes[ParsersAttributes::POINTS]=str_aux;

 //Obtendo a posição dos rótulos
 str_aux="";
 for(i=0; i < 3; i++)
 {
  if(!isnan(dist_rotulos[i].x()))
  {
   attributes[ParsersAttributes::X_POS]=QString("%1").arg(dist_rotulos[i].x());
   attributes[ParsersAttributes::Y_POS]=QString("%1").arg(dist_rotulos[i].y());
   attributes[ParsersAttributes::POSITION]=SchemaParser::getObjectDefinition(ParsersAttributes::POSITION,
                                                 attributes, SchemaParser::XML_DEFINITION);
   attributes[ParsersAttributes::REF_TYPE]=atribs_rot[i];
   str_aux+=SchemaParser::getObjectDefinition(ParsersAttributes::LABEL,
                                                attributes, SchemaParser::XML_DEFINITION);
  }
 }
 attributes[ParsersAttributes::LABELS_POS]=str_aux;
}

QString RelacionamentoBase::getCodeDefinition(void)
{
 bool forma_reduzida;

 definirAtributosRelacionamento();
 forma_reduzida=attributes[ParsersAttributes::POINTS].isEmpty();
 return(BaseObject::getCodeDefinition(SchemaParser::XML_DEFINITION,forma_reduzida));
}

void RelacionamentoBase::definirPontos(const vector<QPointF> &pontos)
{
 this->pontos=pontos;
}

void RelacionamentoBase::definirDistanciaRotulo(unsigned idx_rot, QPointF dist_rotulo)
{
 if(idx_rot > ROTULO_NOME_RELAC)
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->dist_rotulos[idx_rot]=dist_rotulo;
}

QPointF RelacionamentoBase::obterDistanciaRotulo(unsigned idx_rot)
{
 if(idx_rot > ROTULO_NOME_RELAC)
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(this->dist_rotulos[idx_rot]);
}

vector<QPointF> RelacionamentoBase::obterPontos(void)
{
 return(pontos);
}

void RelacionamentoBase::operator = (RelacionamentoBase &rel)
{
 (*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(rel);
 //this->conectado=rel.conectado;
 this->conectado=false;
 this->tabela_orig=rel.tabela_orig;
 this->tabela_dest=rel.tabela_dest;
 this->tipo_relac=rel.tipo_relac;
 this->pontos=rel.pontos;

 /* Inicializa as distâncias dos rótulos com NAN.
    Quando este valor está presente em uma das coordenadas
    de uma distância de rótulo, o mesmo sempre será
    ajustado automaticamente */
 for(int i=0; i < 3; i++)
 {
  if(rel.rotulos[i])
  {
   if(!this->rotulos[i])
    this->rotulos[i]=new CaixaTexto;

   (*this->rotulos[i])=(*rel.rotulos[i]);
  }
  this->dist_rotulos[i]=rel.dist_rotulos[i];
 }


 /* Define ambas as tabelas como não obrigatórias temporariamente a fim de evitar
    um disparo de erro prematuro, vide definirTabelaObrigatoria() */
 this->definirTabelaObrigatoria(TABELA_ORIGEM, false);
 this->definirTabelaObrigatoria(TABELA_DESTINO, false);

 this->definirTabelaObrigatoria(TABELA_ORIGEM, rel.obrig_orig);
 this->definirTabelaObrigatoria(TABELA_DESTINO, rel.obrig_dest);
}

