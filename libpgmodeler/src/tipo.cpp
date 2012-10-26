#include "tipo.h"

Tipo::Tipo(void)
{
 id_objeto=ObjetoBase::id_tipo++;
 tipo_objeto=OBJETO_TIPO;
 config=TIPO_ENUMERACAO;
 //nome="novo_tipo";
 alinhamento="integer";
 delimitador='\0';
 armazenamento=TipoArmazenamento::plain;
 elemento="any";
 comp_interno=0;

 categoria=TipoCategoria::userdefined;
 preferido=false;
 tipo_copia="any";

 ObjetoBase::atributos[AtributosParsers::TIPO_BASE]="";
 ObjetoBase::atributos[AtributosParsers::TIPO_COMPOSTO]="";
 ObjetoBase::atributos[AtributosParsers::ELEMENTOS]="";
 ObjetoBase::atributos[AtributosParsers::TIPO_ENUM]="";
 ObjetoBase::atributos[AtributosParsers::ENUMERACOES]="";
 ObjetoBase::atributos[AtributosParsers::FUNCAO_INPUT]="";
 ObjetoBase::atributos[AtributosParsers::FUNCAO_OUTPUT]="";
 ObjetoBase::atributos[AtributosParsers::FUNCAO_RECV]="";
 ObjetoBase::atributos[AtributosParsers::FUNCAO_SEND]="";
 ObjetoBase::atributos[AtributosParsers::FUNCAO_TPMOD_IN]="";
 ObjetoBase::atributos[AtributosParsers::FUNCAO_TPMOD_OUT]="";
 ObjetoBase::atributos[AtributosParsers::FUNCAO_ANALYZE]="";
 ObjetoBase::atributos[AtributosParsers::COMP_INTERNO]="";
 ObjetoBase::atributos[AtributosParsers::POR_VALOR]="";
 ObjetoBase::atributos[AtributosParsers::ALINHAMENTO]="";
 ObjetoBase::atributos[AtributosParsers::ARMAZENAMENTO]="";
 ObjetoBase::atributos[AtributosParsers::VALOR_PADRAO]="";
 ObjetoBase::atributos[AtributosParsers::ELEMENTO]="";
 ObjetoBase::atributos[AtributosParsers::DELIMITADOR]="";
 ObjetoBase::atributos[AtributosParsers::FORMA_REDUZIDA]="";
 ObjetoBase::atributos[AtributosParsers::CATEGORIA]="";
 ObjetoBase::atributos[AtributosParsers::PREFERIDO]="";
 ObjetoBase::atributos[AtributosParsers::TIPO_COPIA]="";
}

Tipo::~Tipo(void)
{
 /* Ao ser destruído um objeto desta classe tem
    seu nome removido da lista de tipos válidos
    do PostgreSQL */
 TipoPgSQL::removerTipoUsuario(this->obterNome(true), this);
}

void Tipo::definirNome(const QString &nome)
{
 QString nome_ant;

 nome_ant=this->obterNome(true);//this->nome;
 ObjetoBase::definirNome(nome);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->obterNome(true));
}

void Tipo::definirEsquema(ObjetoBase *esquema)
{
 QString nome_ant;

 nome_ant=this->obterNome(true);
 ObjetoBase::definirEsquema(esquema);

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->obterNome(true));
}

bool Tipo::atributoExiste(const QString &nome_atrib)
{
 vector<Parametro>::iterator itr, itr_end;
 bool enc=false;

 itr=atributos.begin();
 itr_end=atributos.end();

 while(itr!=itr_end && !enc)
 {
  enc=(itr->obterNome()==nome_atrib);
  itr++;
 }

 return(enc);
}

void Tipo::adicionarAtributo(Parametro atrib)
{
 //O atributo não pode ter o nome vazio nem tipo nulo
 if(atrib.obterNome()=="" || atrib.obterTipo()==TipoPgSQL::nulo)
  throw Excecao(ERR_PGMODELER_INSATRIBTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(TipoPgSQL::obterIndiceTipoUsuario(this->obterNome(true), this) == !atrib.obterTipo())
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_TIPOUSRAUTOREF).arg(QString::fromUtf8(this->obterNome(true))),
                ERR_PGMODELER_TIPOUSRAUTOREF,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //Verifica se o atributo com mesmo nome já não foi inserido no tipo
 else if(atributoExiste(atrib.obterNome()))
  throw Excecao(ERR_PGMODELER_INSATRIBTIPODUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 atributos.push_back(atrib);
}

void Tipo::removerAtributo(unsigned idx_atrib)
{
 //Verifica se o índice do atributo é valido
 if(idx_atrib >= atributos.size())
  throw Excecao(ERR_PGMODELER_REFATRIBTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Remove atributo no índice especificado
 atributos.erase(atributos.begin() + idx_atrib);
}

void Tipo::removerAtributos(void)
{
 atributos.clear();
}

bool Tipo::enumeracaoExiste(const QString &nome_enum)
{
 vector<QString>::iterator itr, itr_end;
 bool enc=false;

 itr=enumeracoes.begin();
 itr_end=enumeracoes.end();

 while(itr!=itr_end && !enc)
 {
  enc=((*itr)==nome_enum);
  itr++;
 }

 return(enc);
}

void Tipo::adicionarEnumeracao(const QString &enumer)
{
 //Verifica se a enumeração é vazia
 if(enumer=="")
  throw Excecao(ERR_PGMODELER_INSENUMTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Verifica se o nome da enumeração é válida de acordo com
    com a regra de nomenclatura de identificadores no PostgreSQL */
 else if(!ObjetoBase::nomeValido(enumer))
  throw Excecao(ERR_PGMODELER_ATRNOMEOBJINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //Verifica se uma enumeração com mesmo nome já não foi inserido no tipo
 else if(enumeracaoExiste(enumer))
  throw Excecao(ERR_PGMODELER_INSENUMTIPODUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 enumeracoes.push_back(enumer);
}

void Tipo::removerEnumeracao(unsigned idx_enum)
{
 if(idx_enum >= enumeracoes.size())
  throw Excecao(ERR_PGMODELER_REFENUMTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 enumeracoes.erase(enumeracoes.begin() + idx_enum);
}

void Tipo::removerEnumeracoes(void)
{
 enumeracoes.clear();
}

void Tipo::definirConfiguracao(unsigned conf)
{
 unsigned idx;

 /* Ao se definir uma configuração de tipo os atributos não
    pertinentes a esta serão limpos. */

 //Verifica se a configuração a ser atribuída ao tipo é válida
 if(conf < TIPO_BASE || conf > TIPO_COMPOSTO)
  throw Excecao(ERR_PGMODELER_ATRCONFTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(conf!=TIPO_BASE)
 {
  if(conf==TIPO_ENUMERACAO)
   atributos.clear();
  else
   enumeracoes.clear();

  for(idx=0; idx < 7; idx++)
   funcoes[idx]=NULL;

  valor_padrao="";
  elemento="any";
  delimitador='\0';
  por_valor=false;
  comp_interno=0;
 }
 else
 {
  atributos.clear();
  enumeracoes.clear();
 }

 this->config=conf;
}

void Tipo::definirFuncao(unsigned conf_func, Funcao *funcao)
{
 unsigned qtd_params;
 TipoLinguagem ling;
 ling=TipoLinguagem::c;

 //Verifica se o tipo da função é válido
 if(conf_func > FUNCAO_ANALYZE)
  throw Excecao(ERR_PGMODELER_REFFUNCTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém a quantidade de parâmetros da função
 if(funcao)
  qtd_params=funcao->obterNumParams();

 /* Verifica se a função está alocada quando o tipo de função é INPUT ou OUTPUT,
    pois estas duas são obrigatórias para um tipo base */
 if(!funcao && (conf_func==FUNCAO_INPUT || conf_func==FUNCAO_OUTPUT))
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCNAOALOC)
                         .arg(QString::fromUtf8(this->obterNome(true)))
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_TIPO)),
                ERR_PGMODELER_ATRFUNCNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 else if(funcao)
 {
  /* Verifica se a função está escrita em C. Para a criação de um tipo base
   apenas funções nesta linguagem podem ser atribuídas */
  if(funcao->obterLinguagem()->obterNome()!=(~ling))
   throw Excecao(ERR_PGMODELER_ATRFUNCLINGINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Verificando a quantidade de parâmetros da função em relação ao tipo.
     Funções INPUT e RECV devem possuir 1 ou 3 parâmetros, já as demais funções
     devem ter somente 1 parâmetro. */
  else if((qtd_params!=1 && qtd_params!=3 &&
          (conf_func==FUNCAO_INPUT || conf_func==FUNCAO_RECV)) ||
          (qtd_params!=1 &&
          (conf_func==FUNCAO_OUTPUT   || conf_func==FUNCAO_SEND ||
           conf_func==FUNCAO_TPMOD_IN || conf_func==FUNCAO_TPMOD_OUT ||
           conf_func==FUNCAO_ANALYZE)))
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCNUMPARAMINV)
                          .arg(QString::fromUtf8(this->obterNome()))
                          .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_TIPO)),
                 ERR_PGMODELER_ATRFUNCNUMPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Verificando os tipos de returno da função em relação ao tipo.
     Funções do tipo INPUT e RECV devem retornar dados do próprio tipo que está sendo
     definido de acordo com a documentação, porém para facilitar a implementação esta função
     precisa retornar dados do tipo 'any' o qual será substituído pelo nome do tipo no momento
     da geração do SQL da função. Funções do tipo OUTPUT e TPMOD_OUT devem retornar cstring.
     As demais funções SEND, TPMOD_IN e ANALYZE devem retornar bytea, integer e boolean,
     respectivamente. */
  else if((conf_func==FUNCAO_INPUT && funcao->obterTipoRetorno()!="any") ||
          (conf_func==FUNCAO_OUTPUT && funcao->obterTipoRetorno()!="cstring") ||
          (conf_func==FUNCAO_RECV && funcao->obterTipoRetorno()!="any") ||
          (conf_func==FUNCAO_SEND && funcao->obterTipoRetorno()!="bytea") ||
          (conf_func==FUNCAO_TPMOD_IN && funcao->obterTipoRetorno()!="integer") ||
          (conf_func==FUNCAO_TPMOD_OUT && funcao->obterTipoRetorno()!="cstring") ||
          (conf_func==FUNCAO_ANALYZE && funcao->obterTipoRetorno()!="boolean"))
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCRETINV)
                          .arg(QString::fromUtf8(this->obterNome()))
                          .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_TIPO)),
                 ERR_PGMODELER_ATRFUNCRETINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Validando os tipos do parâmetro da função em ra��o �  configuração do tipo.
     A função INPUT deve possuir os parâmetros com tipo (cstring, oid, integer).
     As funções OUTPUT e SEND devem possuir um parâmetro do mesmo tipo sendo definido,
     neste caso, para facilitar a implementação basta usar um parâmetro de tipo 'any'.
     A função RECV deve possuir os parâmetros (internal, oid, integer).
     A função TPMOD_IN deve possuir um parâmetro do tipo (ctring[]).
     A função TPMOD_OUT deve possuir um parâmetro do tipo (integer).
     A função ANALYZE deve possuir um parâmetro do tipo (internal). */
  else if((conf_func==FUNCAO_INPUT &&
          (funcao->obterParametro(0).obterTipo()!="cstring" ||
           (qtd_params==3 &&
            (funcao->obterParametro(1).obterTipo()!="oid" ||
             funcao->obterParametro(2).obterTipo()!="integer")))) ||
          (conf_func==FUNCAO_OUTPUT && funcao->obterParametro(0).obterTipo()!="any") ||
          (conf_func==FUNCAO_RECV &&
           (funcao->obterParametro(0).obterTipo()!="internal" ||
            (qtd_params==3 &&
             (funcao->obterParametro(1).obterTipo()!="oid" ||
              funcao->obterParametro(2).obterTipo()!="integer")))) ||
           (conf_func==FUNCAO_SEND && funcao->obterParametro(0).obterTipo()!="any") ||
          (conf_func==FUNCAO_TPMOD_IN && *(funcao->obterParametro(0).obterTipo())!="cstring[]") ||
          (conf_func==FUNCAO_TPMOD_OUT && funcao->obterParametro(0).obterTipo()!="integer") ||
          (conf_func==FUNCAO_ANALYZE && funcao->obterParametro(0).obterTipo()!="internal"))
   throw Excecao(ERR_PGMODELER_ATRFUNCPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  funcao->definirProtegido(false);
 }

 funcoes[conf_func]=funcao;
}

void Tipo::converterParametrosFuncoes(bool conv_inversa)
{
 unsigned i, conf_funcs[]={ FUNCAO_INPUT, FUNCAO_RECV,
                            FUNCAO_OUTPUT, FUNCAO_SEND };
 TipoFuncao conf_func;
 Parametro param;
 Funcao *funcao=NULL;

 for(i=0; i < 4; i++)
 {
  funcao=funcoes[conf_funcs[i]];

  if(funcao)
  {
   /* Para as funções OUTPUT e SEND a conversão normal é mudar o tipo do primeiro
      parâmetro de 'any' para o tipo 'this' já a conversão inversa faz o processo contrário */
   if(conf_funcs[i]==FUNCAO_OUTPUT || conf_funcs[i]==FUNCAO_SEND)
   {
    param=funcao->obterParametro(0);
    funcao->removerParametro(0);

    if(!conv_inversa)
    {
     param.definirTipo(TipoPgSQL(this));
     funcao->adicionarParametro(param);
    }
    else
    {
     param.definirTipo(TipoPgSQL("any"));
     funcao->adicionarParametro(param);
    }
   }

   /* Para as funções INPUT e RECV a conversão normal é mudar o tipo de retorno
      de 'any' para o tipo 'this' já a conversão inversa faz o processo contrário */
   else if(conf_funcs[i]==FUNCAO_INPUT || conf_funcs[i]==FUNCAO_RECV)
   {
    if(!conv_inversa)
     funcao->definirTipoRetorno(TipoPgSQL(this));
    else
     funcao->definirTipoRetorno(TipoPgSQL("any"));
   }
  }
 }
}

void Tipo::definirCompInterno(unsigned comp)
{
 comp_interno=comp;
}

void Tipo::definirPorValor(bool valor)
{
 por_valor=valor;
}

void Tipo::definirAlinhamento(TipoPgSQL tipo)
{
 QString tp;

 tp=(*tipo);
 /* Verifica se o tipo a ser atribuído ao alinhamento é
    diferente de char, smallint, integer e double (os únicos aceitos) */
 if(tp!="char" && tp!="smallint" && tp!="integer" && tp!="double precision")
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRALINHAMENTOINVTIPO).arg(QString::fromUtf8(this->obterNome(true))),
                ERR_PGMODELER_ATRALINHAMENTOINVTIPO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Tipo::definirArmazenamento(TipoArmazenamento armaz)
{
 armazenamento=armaz;
}

void Tipo::definirValorPadrao(const QString &valor_padrao)
{
 this->valor_padrao=valor_padrao;
}

void Tipo::definirElemento(TipoPgSQL elemento)
{
 if(TipoPgSQL::obterIndiceTipoUsuario(this->obterNome(true), this) == !elemento)
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_TIPOUSRAUTOREF).arg(QString::fromUtf8(this->obterNome(true))),
                ERR_PGMODELER_TIPOUSRAUTOREF,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(elemento!="any" &&
   (elemento.tipoOID() || elemento.pseudoTipo() ||
    elemento.tipoUsuario() || elemento.tipoArray()))
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRELEMENTOINVTIPO).arg(QString::fromUtf8(this->obterNome(true))),
                ERR_PGMODELER_ATRELEMENTOINVTIPO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->elemento=elemento;
}

void Tipo::definirDelimitador(char delim)
{
 delimitador=delim;
}

void Tipo::definirAtributoElementos(unsigned tipo_def)
{
 Parametro param;
 QString str_elem;
 unsigned i, qtd;

 qtd=Tipo::atributos.size();
 for(i=0; i < qtd; i++)
 {
  param=Tipo::atributos[i];

  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
  {
   str_elem+=param.obterNome() + " " + (*param.obterTipo());
   if(i < (qtd-1)) str_elem+=",";
  }
  else
  {
   str_elem+=param.obterDefinicaoObjeto(tipo_def);
  }
 }

 ObjetoBase::atributos[AtributosParsers::ELEMENTOS]=str_elem;
}

void Tipo::definirAtributoEnumeracoes(unsigned tipo_def)
{
 QString str_enum;
 unsigned i, qtd;

 qtd=enumeracoes.size();
 for(i=0; i < qtd; i++)
 {
  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   str_enum+="'" + enumeracoes[i] + "'";
  else
   str_enum+=enumeracoes[i];

  if(i < (qtd-1)) str_enum+=",";
 }

 ObjetoBase::atributos[AtributosParsers::ENUMERACOES]=str_enum;
}

void Tipo::definirCategoria(TipoCategoria categoria)
{
 this->categoria=categoria;
}

void Tipo::definirPreferido(bool preferido)
{
 this->preferido=preferido;
}

void Tipo::definirTipoCopia(TipoPgSQL tipo_copia)
{
 if(TipoPgSQL::obterIndiceTipoUsuario(this->obterNome(true), this) == !tipo_copia)
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_TIPOUSRAUTOREF).arg(QString::fromUtf8(this->obterNome(true))),
                ERR_PGMODELER_TIPOUSRAUTOREF,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->tipo_copia=tipo_copia;
}

Parametro Tipo::obterAtributo(unsigned idx_atrib)
{
 if(idx_atrib >= atributos.size())
  throw Excecao(ERR_PGMODELER_REFATRIBTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(atributos[idx_atrib]);
}

unsigned Tipo::obterNumAtributos(void)
{
 return(atributos.size());
}

QString Tipo::obterEnumeracao(unsigned idx_enum)
{
 if(idx_enum >= enumeracoes.size())
  throw Excecao(ERR_PGMODELER_REFENUMTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(enumeracoes[idx_enum]);
}

unsigned Tipo::obterNumEnumeracoes(void)
{
 return(enumeracoes.size());
}

Funcao *Tipo::obterFuncao(unsigned conf_func)
{
 if(conf_func > FUNCAO_ANALYZE)
  throw Excecao(ERR_PGMODELER_REFFUNCTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(funcoes[conf_func]);
}

unsigned Tipo::obterCompInterno(void)
{
 return(comp_interno);
}

bool Tipo::passadoPorValor(void)
{
 return(por_valor);
}

TipoPgSQL Tipo::obterAlinhamento(void)
{
 return(alinhamento);
}

TipoArmazenamento Tipo::obterArmazenamento(void)
{
 return(armazenamento);
}

QString Tipo::obterValorPadrao(void)
{
 return(valor_padrao);
}

TipoPgSQL Tipo::obterElemento(void)
{
 return(elemento);
}

char Tipo::obterDelimitador(void)
{
 return(delimitador);
}

unsigned Tipo::obterConfiguracao(void)
{
 return(config);
}

TipoCategoria Tipo::obterCategoria(void)
{
 return(categoria);
}

bool Tipo::tipoPreferido(void)
{
 return(preferido);
}

TipoPgSQL Tipo::obterTipoCopia(void)
{
 return(tipo_copia);
}

QString Tipo::obterDefinicaoObjeto(unsigned tipo_def)
{
 return(this->obterDefinicaoObjeto(tipo_def, false));
}

QString Tipo::obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida)
{
 if(config==TIPO_ENUMERACAO)
 {
  ObjetoBase::atributos[AtributosParsers::TIPO_ENUM]="1";
  definirAtributoEnumeracoes(tipo_def);
 }
 else if(config==TIPO_COMPOSTO)
 {
  ObjetoBase::atributos[AtributosParsers::TIPO_COMPOSTO]="1";
  definirAtributoElementos(tipo_def);
 }
 else
 {
  unsigned i;
  QString atrib_func[7]={AtributosParsers::FUNCAO_INPUT,
                         AtributosParsers::FUNCAO_OUTPUT,
                         AtributosParsers::FUNCAO_RECV,
                         AtributosParsers::FUNCAO_SEND,
                         AtributosParsers::FUNCAO_TPMOD_IN,
                         AtributosParsers::FUNCAO_TPMOD_OUT,
                         AtributosParsers::FUNCAO_ANALYZE};

  ObjetoBase::atributos[AtributosParsers::TIPO_BASE]="1";

  for(i=0; i < 7; i++)
  {
   if(funcoes[i])
   {
    if(tipo_def==ParserEsquema::DEFINICAO_SQL)
     ObjetoBase::atributos[atrib_func[i]]=funcoes[i]->obterNome();
    else
    {
     funcoes[i]->definirAtributoEsquema(AtributosParsers::TIPO_REFERENCIA, atrib_func[i]);
     ObjetoBase::atributos[atrib_func[i]]=funcoes[i]->obterDefinicaoObjeto(tipo_def, true);
    }
   }
  }

  if(comp_interno==0 && tipo_def==ParserEsquema::DEFINICAO_SQL)
   ObjetoBase::atributos[AtributosParsers::COMP_INTERNO]="VARIABLE";
  else
   ObjetoBase::atributos[AtributosParsers::COMP_INTERNO]=QString("%1").arg(comp_interno);

  ObjetoBase::atributos[AtributosParsers::POR_VALOR]=(por_valor ? "1" : "");
  ObjetoBase::atributos[AtributosParsers::ALINHAMENTO]=(*alinhamento);
  ObjetoBase::atributos[AtributosParsers::ARMAZENAMENTO]=(~armazenamento);
  ObjetoBase::atributos[AtributosParsers::VALOR_PADRAO]=valor_padrao;

  if(elemento!="any")
   ObjetoBase::atributos[AtributosParsers::ELEMENTO]=(*elemento);

  if(delimitador!='\0')
   ObjetoBase::atributos[AtributosParsers::DELIMITADOR]=delimitador;

  ObjetoBase::atributos[AtributosParsers::CATEGORIA]=~(categoria);

  ObjetoBase::atributos[AtributosParsers::PREFERIDO]=(preferido ? "1" : "");

  if(tipo_copia!="any")
   ObjetoBase::atributos[AtributosParsers::TIPO_COPIA]=(*tipo_copia);
 }

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def, forma_reduzida));
}

void Tipo::operator = (Tipo &tipo)
{
 QString nome_ant;
 unsigned i=0;

 nome_ant=this->obterNome(true);
 *(dynamic_cast<ObjetoBase *>(this))=dynamic_cast<ObjetoBase &>(tipo);

 this->config=tipo.config;
 this->atributos=tipo.atributos;
 this->enumeracoes=tipo.enumeracoes;
 this->comp_interno=tipo.comp_interno;
 this->por_valor=tipo.por_valor;
 this->alinhamento=tipo.alinhamento;
 this->elemento=tipo.elemento;
 this->armazenamento=tipo.armazenamento;
 this->valor_padrao=tipo.valor_padrao;
 this->categoria=tipo.categoria;
 this->preferido=tipo.preferido;
 this->tipo_copia=tipo.tipo_copia;
 this->delimitador=tipo.delimitador;

 while(i < 7)
 {
  this->funcoes[i]=tipo.funcoes[i];
  i++;
 }

 /* Renomeia o tipo já definido anteriormente na
    lista de tipos do PostgreSQL */
 TipoPgSQL::renomearTipoUsuario(nome_ant, this, this->obterNome(true));
}

