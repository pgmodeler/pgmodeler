#include "objetobase.h"
//***********************************************************
/* Atenção: Se a ordem e quantidade das enumerações forem modificados
   então a ordem e quantidade dos elementos deste vetor
   também devem ser modificados */
QString ObjetoBase::esq_objetos[QTD_TIPOS_OBJETO]={
  "column",  "constraint", "function", "trigger",
  "index", "rule", "table", "view",
  "domain", "schema", "aggregate", "operator",
  "sequence", "role", "conversion", "cast",
  "language", "usertype", "tablespace",
  "opfamily", "opclass", "database","relationship","textbox",
   "grant","parameter","relationship"
};


QString ObjetoBase::nome_tipo_objetos[QTD_TIPOS_OBJETO]={
  QObject::tr("Coluna"), QObject::tr("Restrição"), QObject::tr("Função"),
  QObject::tr("Gatilho"), QObject::tr("Índice"), QObject::tr("Regra"),
  QObject::tr("Tabela"), QObject::tr("Visão"),  QObject::tr("Domínio"),
  QObject::tr("Esquema"), QObject::tr("Função de Agregação"), QObject::tr("Operador"),
  QObject::tr("Seqüência"), QObject::tr("Papel"), QObject::tr("Conversão de Codificação"),
  QObject::tr("Conversão de Tipo"),
  QObject::tr("Linguagem"), QObject::tr("Tipo"), QObject::tr("Espaço de Tabela"),
  QObject::tr("Família de Operadores"), QObject::tr("Classe de Operadores"),
  QObject::tr("Banco de Dados"), QObject::tr("Relacionamento Tabela-Tabela"),
  QObject::tr("Caixa de Texto"), QObject::tr("Permissão"), QObject::tr("Parâmetro"),
  QObject::tr("Relacionamento Tabela-Visão")

};

QString ObjetoBase::sql_objetos[QTD_TIPOS_OBJETO]={
  "COLUMN",  "CONSTRAINT", "FUNCTION", "TRIGGER",
  "INDEX", "RULE", "TABLE", "VIEW",
  "DOMAIN", "SCHEMA", "AGGREGATE", "OPERATOR",
  "SEQUENCE", "ROLE", "CONVERSION", "CAST",
  "LANGUAGE", "TYPE", "TABLESPACE",
  "OPERATOR FAMILY", "OPERATOR CLASS", "DATABASE"
};

/* Inicializa o id global o qual é compartilhado entre as instâncias
   de classes derivadas da classe ObjetoBase. O valor do id_global
   inicia-se em 60k pois as faixas de ids 0,10k,20k,30k,40k e 50k
   estão atribuídos respectivamente aos objetos das classes Papel, EspacoTabela,
   ModeloBD, Esquema, Função e Tipo */
unsigned ObjetoBase::id_global=60000;
unsigned ObjetoBase::id_tipo=50000;
unsigned ObjetoBase::id_funcao=40000;
unsigned ObjetoBase::id_esquema=30000;
unsigned ObjetoBase::id_modelobd=20000;
unsigned ObjetoBase::id_esptabela=10000;
unsigned ObjetoBase::id_papel=0;
//-----------------------------------------------------------
ObjetoBase::ObjetoBase(void)
{
 id_objeto=ObjetoBase::id_global++;
 protegido=false;
 nome="novo_objeto";
 tipo_objeto=OBJETO_BASE;
 esquema=NULL;
 dono=NULL;
 espacotabela=NULL;
 atributos[AtributosParsers::NOME]="";
 atributos[AtributosParsers::COMENTARIO]="";
 atributos[AtributosParsers::DONO]="";
 atributos[AtributosParsers::ESPACOTABELA]="";
 atributos[AtributosParsers::ESQUEMA]="";
 atributos[AtributosParsers::PROTEGIDO]="";
}
//-----------------------------------------------------------
unsigned ObjetoBase::obterIdGlobal(void)
{
 return(id_global);
}
//-----------------------------------------------------------
QString ObjetoBase::obterNomeTipoObjeto(TipoObjetoBase tipo_objeto)
{
 if(tipo_objeto!=OBJETO_BASE)
  return(QObject::tr(nome_tipo_objetos[tipo_objeto]));
 else
  return("");
}
//-----------------------------------------------------------
QString ObjetoBase::obterNomeEsquemaObjeto(TipoObjetoBase tipo_objeto)
{
 return(esq_objetos[tipo_objeto]);
}
//-----------------------------------------------------------
QString ObjetoBase::obterNomeSQLObjeto(TipoObjetoBase tipo_objeto)
{
 return(sql_objetos[tipo_objeto]);
}
//-----------------------------------------------------------
QString ObjetoBase::formatarNome(const QString &nome_obj, bool obj_operador)
{
 int i;
 bool formatado=false;
 QString nome_form;
 QChar chr, chr1, chr2;
 QRegExp vet_regexp[]={
                        QRegExp("(\")(.)+(\")"),
                        QRegExp("(\")(.)+(\")(\\.)(\")(.)+(\")"),
                        QRegExp("(\")(.)+(\")(\\.)(.)+"),
                        QRegExp("(.)+(\\.)(\")(.)+(\")"),
                        QRegExp("(.)+(\\.)(.)+")
                      };

 /* Verifica através de expressões regulares
    se o nome passado para formatação já não se encontra
    formatado. As formas prováveis de formatação são:
    1) "NOME_OBJETO"
    2) "NOME_ESQUEMA"."NOME_OBJETO"
    3) "NOME_ESQUEMA".NOME_OBJETO
    4) NOME_ESQUEMA."NOME_OBJETO"
    5) NOME_ESQUEMA.NOME_OBJETO */
 for(i=0; i < 5 && !formatado; i++)
  formatado=vet_regexp[i].exactMatch(nome_obj);

 /* Caso o nome não esteja formatado ou
    o mesmo simboliza o nome de um operador (o qual possui caracteres
    inválidos de acordo com a regra e é a única exceção a qual seu
    nome é formatado mesmo sendo inválido) ou se nome é valido de acordo
    com as regras do SGBD para os demais tipos de objetos */
 if(!formatado && (obj_operador || nomeValido(nome_obj)))
 {
  bool maiusc=false;
  unsigned i, qtd;

  /* Verifica se o nome passado possui alguma
     letra maíuscula, caso possui, o nome será
     colocado entre aspas */
  qtd=nome_obj.size();
  //for(i=0; i < qtd && !maiusc; i++)
  i=0;
  while(i < qtd && !maiusc)
  {
   chr=nome_obj[i];

   if(((i + 1) < (qtd-1)) &&
       ((chr >= 0xC2 && chr <= 0xDF) ||
        (chr >= 0xE0 && chr <= 0xEF)))
    chr1=nome_obj[i+1];
   else
    chr1=0;

   if((i + 2) < (qtd-1) &&
      chr >= 0xE0 && chr <= 0xEF)
    chr2=nome_obj[i+2];
   else
    chr2=0;

   if(chr1!=0 && chr2!=0)
    i+=3;
   else if(chr1!=0 && chr2==0)
    i+=2;
   else
    i++;

   //Caractere UTF-8 com 2 bytes
   if((chr  >= 0xC2 && chr <= 0xDF &&
       chr1 >= 0x80 && chr1 <= 0xBF) ||

       //Caractere UTF-8 com 3 bytes
       (chr  >= 0xE0 && chr <= 0xEF &&
        chr1 >= 0x80 && chr1 <= 0xBF &&
        chr2 >= 0x80 && chr2 <= 0xBF) ||

      chr.isUpper())
   {
    maiusc=true;
   }

  }

  //Caso o nome possua letras maiúsculas
  if(maiusc)
   nome_form="\"" + nome_obj + "\"";
  else
   nome_form=nome_obj;
 }
 else if(formatado)
  nome_form=nome_obj;

 return(nome_form);
}
//-----------------------------------------------------------
bool ObjetoBase::nomeValido(const QString &nome_obj)
{
 int tam;

 /* No cálculo do tamanho de uma QString o '\0' é considerado,
    sendo assim, para evitar comparações desnecessárias com
    o caracterece \0, o tamanho original é decrementado em 1 */
 tam=nome_obj.size();

 /* Caso o nome seja maior do que o tamanho máximo
    aceito pelo SGBD (atualmente 63 bytes) */
 if(nome_obj.isEmpty() || tam > TAM_MAX_NOME_OBJETO)
  return(false);
 else
 {
  int i=0;
  bool valido=true;
  QChar chr='\0', chr1='\0', chr2='\0';

  chr=nome_obj[0];
  if(tam > 1)
   chr1=nome_obj[tam-1];

  /* Verifica se o nome do objeto está entre aspas ou não.
     Situações do tipo "nome ou nome" não são aceitas */
  if(chr=='\"' && chr1=='\"')
  {
   /* Valida o nome mas a contagem de caracteres se iniciará
      após a primeira aspa e finaliza antes de última */
   valido=true; i++; tam--;
  }

  while(valido && i < tam)
  {
   chr=nome_obj[i];

   //Validação de caracteres ascii simples
   /* Verifica se o nome possui apenas os caracteres
     a-z A-Z 0-9 _ */
   if((chr >= 'a' && chr <='z') ||
      (chr >= 'A' && chr <='Z') ||
      (chr >= '0' && chr <='9') ||
      chr == '_')
   {
    valido=true;
    i++;
   }
   else valido=false;

   /* Validação de caracteres utf8. Caracteres UTF8 possuem 2 bytes.
      Sendo que o primeiro byte deve estar entre 0xC2 e 0xDF e o segundo/terceiro
      deve estar entre 0x80 e 0xBF.
      Ref.: http://www.fileformat.info/info/unicode/utf8.htm

      Trecho extraído da url:

      The value of each individual byte indicates its UTF-8 function, as follows:
      00 to 7F hex (0 to 127): first and only byte of a sequence.
      80 to BF hex (128 to 191): continuing byte in a multi-byte sequence.
      C2 to DF hex (194 to 223): first byte of a two-byte sequence.
      E0 to EF hex (224 to 239): first byte of a three-byte sequence.  */
   if(!valido && (i < tam-1))
   {
    chr1=nome_obj[i+1];

    if((i + 2) < (tam-1))
     chr2=nome_obj[i+2];
    else
     chr2=0;

       //Caractere UTF-8 com 2 bytes
    if((chr  >= 0xC2 && chr <= 0xDF &&
        chr1 >= 0x80 && chr1 <= 0xBF) ||

       //Caractere UTF-8 com 3 bytes
       (chr  >= 0xE0 && chr <= 0xEF &&
        chr1 >= 0x80 && chr1 <= 0xBF &&
        chr2 >= 0x80 && chr2 <= 0xBF))
    valido=true;

    //Caso o caractere validado seja de 2 bytes
    if(chr >= 0xC2 && chr <= 0xDF)
     //Incrementa em 2 a posição na string
     i+=2;
    else
     /* Incrementa em 3 a posição na string pois o
        caractere validado é de 3 bytes */
     i+=3;
   }
  }

  return(valido);
 }
}
//-----------------------------------------------------------
void ObjetoBase::definirProtegido(bool valor)
{
 protegido=valor;
}
//-----------------------------------------------------------
void ObjetoBase::definirNome(const QString &nome)
{
 /* Caso se tente atribuir um nome vazio ao objeto
    é disparada uma exceção */
 if(nome.isEmpty())
  throw Excecao(ERR_PGMODELER_ATRNOMEOBJVAZIO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  int qtd;
  QString nome_aux=nome;

  qtd=nome_aux.count(QChar('\0'));
  if(qtd >=1) nome_aux.chop(qtd);

  //Caso o nome não seja válido dispara uma exceção
  if(!nomeValido(nome_aux))
   throw Excecao(ERR_PGMODELER_ATRNOMEOBJINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   nome_aux.remove('\"');
   this->nome=nome_aux;
  }
 }
}
//-----------------------------------------------------------
void ObjetoBase::definirComentario(const QString &comentario)
{
 this->comentario=comentario;
}
//-----------------------------------------------------------
void ObjetoBase::definirEsquema(ObjetoBase *esquema)
{
 if(!esquema)
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRESQNAOALOC)
                .arg(this->nome).arg(this->obterNomeTipoObjeto()),
                ERR_PGMODELER_ATRESQNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(esquema && esquema->obterTipoObjeto()!=OBJETO_ESQUEMA)
  throw Excecao(ERR_PGMODELER_ATRESQTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  if(tipo_objeto==OBJETO_FUNCAO || tipo_objeto==OBJETO_TABELA ||
     tipo_objeto==OBJETO_VISAO  || tipo_objeto==OBJETO_DOMINIO ||
     tipo_objeto==OBJETO_FUNC_AGREGACAO || tipo_objeto==OBJETO_OPERADOR ||
     tipo_objeto==OBJETO_SEQUENCIA || tipo_objeto==OBJETO_CONV_CODIFICACAO ||
     tipo_objeto==OBJETO_TIPO || tipo_objeto==OBJETO_CLASSE_OPER ||
     tipo_objeto==OBJETO_FAMILIA_OPER)
  {
   this->esquema=esquema;
  }
  else
   throw Excecao(ERR_PGMODELER_ATRESQTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}
//-----------------------------------------------------------
void ObjetoBase::definirDono(ObjetoBase *dono)
{
 if(dono && dono->obterTipoObjeto()!=OBJETO_PAPEL)
  throw Excecao(ERR_PGMODELER_ATRPAPELTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  if(tipo_objeto==OBJETO_FUNCAO || tipo_objeto==OBJETO_TABELA ||
     tipo_objeto==OBJETO_DOMINIO || tipo_objeto==OBJETO_ESQUEMA ||
     tipo_objeto==OBJETO_FUNC_AGREGACAO || tipo_objeto==OBJETO_OPERADOR ||
     tipo_objeto==OBJETO_CONV_CODIFICACAO ||
     tipo_objeto==OBJETO_LINGUAGEM || tipo_objeto==OBJETO_TIPO ||
     tipo_objeto==OBJETO_ESPACO_TABELA || tipo_objeto==OBJETO_BANCO_DADOS ||
     tipo_objeto==OBJETO_CLASSE_OPER || tipo_objeto==OBJETO_FAMILIA_OPER)
  {
   this->dono=dono;
  }
  else
   throw Excecao(ERR_PGMODELER_ATRDONOOBJINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}
//-----------------------------------------------------------
void ObjetoBase::definirEspacoTabela(ObjetoBase *espacotabela)
{
 if(espacotabela && espacotabela->obterTipoObjeto()!=OBJETO_ESPACO_TABELA)
  throw Excecao(ERR_PGMODELER_ATRESPTABTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  if(tipo_objeto==OBJETO_INDICE ||
     tipo_objeto==OBJETO_TABELA ||
     tipo_objeto==OBJETO_RESTRICAO ||
     tipo_objeto==OBJETO_BANCO_DADOS)
  {
   this->espacotabela=espacotabela;
  }
  else
   throw Excecao(ERR_PGMODELER_ATRESPTABOBJINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}
//-----------------------------------------------------------
QString ObjetoBase::obterNome(bool formatar)
{
 if(formatar)
 {
  QString nome_aux;

  /* Formata o nome do objeto e marca o flag que indica se o objeto
     é um operador ou não */
  nome_aux=formatarNome(this->nome, (tipo_objeto==OBJETO_OPERADOR));

  if(this->esquema)
   nome_aux=formatarNome(this->esquema->obterNome()) + "." + nome_aux;

  return(nome_aux);
 }
 else return(this->nome);
}
//-----------------------------------------------------------
QString ObjetoBase::obterComentario(void)
{
 return(comentario);
}
//-----------------------------------------------------------
ObjetoBase *ObjetoBase::obterEsquema(void)
{
 return(esquema);
}
//-----------------------------------------------------------
ObjetoBase *ObjetoBase::obterDono(void)
{
 return(dono);
}
//-----------------------------------------------------------
ObjetoBase *ObjetoBase::obterEspacoTabela(void)
{
 return(espacotabela);
}
//-----------------------------------------------------------
TipoObjetoBase ObjetoBase::obterTipoObjeto(void)
{
 return(tipo_objeto);
}
//-----------------------------------------------------------
QString ObjetoBase::obterNomeTipoObjeto(void)
{
 return(ObjetoBase::obterNomeTipoObjeto(this->tipo_objeto));
}
//-----------------------------------------------------------
QString ObjetoBase::obterNomeEsquemaObjeto(void)
{
 return(ObjetoBase::obterNomeEsquemaObjeto(this->tipo_objeto));
}
//-----------------------------------------------------------
QString ObjetoBase::obterNomeSQLObjeto(void)
{
 return(ObjetoBase::obterNomeSQLObjeto(this->tipo_objeto));
}
//-----------------------------------------------------------
bool ObjetoBase::objetoProtegido(void)
{
 return(protegido);
}
//-----------------------------------------------------------
unsigned ObjetoBase::obterIdObjeto(void)
{
 return(id_objeto);
}
//-----------------------------------------------------------
bool ObjetoBase::operator == (const QString &nome)
{
 return(this->nome==nome);
}
//-----------------------------------------------------------
bool ObjetoBase::operator != (const QString &nome)
{
 return(this->nome!=nome);
}
//-----------------------------------------------------------
QString ObjetoBase::obterDefinicaoObjeto(unsigned tipo_def)
{
 return(ObjetoBase::obterDefinicaoObjeto(tipo_def, false));
}
//-----------------------------------------------------------
QString ObjetoBase::obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida)
{
 QString def;

 if((tipo_def==ParserEsquema::DEFINICAO_SQL &&
     tipo_objeto!=OBJETO_BASE && tipo_objeto!=OBJETO_RELACAO_BASE &&
     tipo_objeto!=OBJETO_TABELA_BASE && tipo_objeto!=OBJETO_CAIXA_TEXTO) ||

    (tipo_def==ParserEsquema::DEFINICAO_XML &&
     tipo_objeto!=OBJETO_BASE && tipo_objeto!=OBJETO_TABELA_BASE))
 {
  bool formatar;
  QString str_aux;

  //Formata o nome dos objetos caso uma definição SQL está sendo gerada
  formatar=(tipo_def==ParserEsquema::DEFINICAO_SQL ||
            (tipo_def==ParserEsquema::DEFINICAO_XML && forma_reduzida &&
             tipo_objeto!=OBJETO_CAIXA_TEXTO && tipo_objeto!=OBJETO_RELACAO &&
             tipo_objeto!=OBJETO_RELACAO_BASE));
  atributos[esq_objetos[tipo_objeto]]="1";

  /* Marcando a flag que indica que o a forma de comentário a ser gerada
     para o objeto é específica para o mesmo, fugindo da regra padrão.
     (Ver arquivo de esquema SQL para comentários) */
  switch(tipo_objeto)
  {
   case OBJETO_COLUNA:
   case OBJETO_FUNC_AGREGACAO:
   case OBJETO_FUNCAO:
   case OBJETO_CONV_TIPO:
   case OBJETO_RESTRICAO:
   case OBJETO_REGRA:
   case OBJETO_GATILHO:
   case OBJETO_OPERADOR:
   case OBJETO_CLASSE_OPER:
   case OBJETO_FAMILIA_OPER:
    atributos[AtributosParsers::SQL_DIF]="1";
   break;
   default:
    atributos[AtributosParsers::SQL_DIF]="";
   break;
  }

  atributos[AtributosParsers::NOME]=this->obterNome(formatar);
  atributos[AtributosParsers::SQL_OBJETO]=sql_objetos[this->tipo_objeto];

  if(tipo_def==ParserEsquema::DEFINICAO_XML && esquema)
  {
   atributos[AtributosParsers::ESQUEMA]=esquema->obterDefinicaoObjeto(tipo_def, true);
  }

  if(tipo_def==ParserEsquema::DEFINICAO_XML)
    atributos[AtributosParsers::PROTEGIDO]=(protegido ? "1" : "");

  if(comentario!="")
  {
   atributos[AtributosParsers::COMENTARIO]=comentario;

   if((tipo_def==ParserEsquema::DEFINICAO_SQL &&
       tipo_objeto!=OBJETO_ESPACO_TABELA &&
       tipo_objeto!=OBJETO_BANCO_DADOS) ||
      tipo_def==ParserEsquema::DEFINICAO_XML)
   {
    ParserEsquema::ignorarAtributosDesc(true);
    atributos[AtributosParsers::COMENTARIO]=
    ParserEsquema::obterDefinicaoObjeto(AtributosParsers::COMENTARIO, atributos, tipo_def);
   }
  }

  if(espacotabela)
  {
   if(tipo_def==ParserEsquema::DEFINICAO_SQL)
    atributos[AtributosParsers::ESPACOTABELA]=espacotabela->obterNome(formatar);
   else
    atributos[AtributosParsers::ESPACOTABELA]=espacotabela->obterDefinicaoObjeto(tipo_def, true);
  }

  if(dono)
  {
   if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   {
    atributos[AtributosParsers::DONO]=dono->obterNome(formatar);

    /** Apenas espaços de tabelas e banco de dados não têm um comando ALTER SET OWNER
        pois por regra do PostgreSQL, espaços de tabelas e banco de dados devem ser criados
        com apenas por linha de comando isolada das demais **/
    if((tipo_def==ParserEsquema::DEFINICAO_SQL &&
        tipo_objeto!=OBJETO_ESPACO_TABELA &&
        tipo_objeto!=OBJETO_BANCO_DADOS) ||
       tipo_def==ParserEsquema::DEFINICAO_XML)
    {
     ParserEsquema::ignorarAtributosDesc(true);
     atributos[AtributosParsers::DONO]=
     ParserEsquema::obterDefinicaoObjeto(AtributosParsers::DONO, atributos, tipo_def);
    }
   }
   else
    atributos[AtributosParsers::DONO]=dono->obterDefinicaoObjeto(tipo_def, true);
  }

  if(forma_reduzida)
   atributos[AtributosParsers::FORMA_REDUZIDA]="1";
  else
   atributos[AtributosParsers::FORMA_REDUZIDA]="";

  try
  {
   /* Caso seja a definição XML a ser obtida, os atributos
      terão os caracteres <, >, ', " e & substituídos pelos
      equivalentes no XML, evitando assim a construção de
      código XMl com erros de sintaxe */
   def=ParserEsquema::obterDefinicaoObjeto(esq_objetos[tipo_objeto], atributos, tipo_def);
   if(tipo_def==ParserEsquema::DEFINICAO_XML)
   {
    QRegExp vet_regexp[]={
                           QRegExp("(=\")+"),
                           QRegExp("(\")(([\r\n\t])+|(\\ )+|(/>)+|(>)+)"),
                           QRegExp("(<)+([a-z])+(>)+"),
                           QRegExp("(</)+([a-z])+(>)+")
                         };

    int pos=0, pos1=0, pos_ant=0, qtd=0;
    QString str_aux;

    do
    {
     pos_ant=pos1;
     //Tenta extrair os valores dos atributos
     pos=vet_regexp[0].indexIn(def, pos);
     pos+=vet_regexp[0].matchedLength();
     pos1=vet_regexp[1].indexIn(def, pos);

     /* CAso não consigua extrair os valores dos atributos
        usa expressões regulares para retirada de conteúdo
        de tags vazias (sem atributos) ex.: <comment>,<condition>,<expression> */
     if(pos < 0)
     {
      pos=vet_regexp[2].indexIn(def, pos_ant);
      pos+=vet_regexp[2].matchedLength();
      pos1=vet_regexp[3].indexIn(def, pos);
     }

     //Calcula a quantidade de caracteres extraidos
     qtd=(pos > 0 ? (pos1-pos) : 0);
     if(pos >= 0)
     {
      //Obtém a substring extraída com as expressões regulares
      str_aux=def.mid(pos, qtd);
      //Substitui os caracteres especiais pelas entidades XML
      str_aux.replace('\"',ParserXML::CHR_ASPAS);
      str_aux.replace('<',ParserXML::CHR_MENORQUE);
      str_aux.replace('>',ParserXML::CHR_MAIORQUE);
      //Substitui na definição XML original pela string modificada
      def.replace(pos,qtd,str_aux);
      pos+=qtd;
     }
    }
    /* Enquanto as posições das expressões encontradas sejam válidas
       Posições menores que 0 indicam que nenhuma das expressões regulares
       conseguiram encontrar valores */
    while(pos >=0 && pos1 >=0);
   }

   limparAtributos();
  }
  catch(Excecao &e)
  {
   ParserEsquema::reiniciarParser();
   limparAtributos();
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }

 return(def);
}
//-----------------------------------------------------------
void ObjetoBase::definirAtributoEsquema(const QString &atrib, const QString &valor)
{
 atributos[atrib]=valor;
}
//-----------------------------------------------------------
void ObjetoBase::limparAtributos(void)
{
 map<QString, QString>::iterator itr, itr_end;

 itr=atributos.begin();
 itr_end=atributos.end();

 while(itr!=itr_end)
 {
  itr->second="";
  itr++;
 }
}
//-----------------------------------------------------------
void ObjetoBase::operator = (ObjetoBase &obj)
{
 this->dono=obj.dono;
 this->esquema=obj.esquema;
 this->espacotabela=obj.espacotabela;
 this->comentario=obj.comentario;
 this->nome=obj.nome;
 this->tipo_objeto=obj.tipo_objeto;
 this->protegido=obj.protegido;
}
//***********************************************************
