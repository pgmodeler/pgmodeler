#include "modelobd.h"
//***********************************************************
ModeloBD::ModeloBD(void)
{
 id_objeto=ObjetoBase::id_modelobd++;

 tipo_objeto=OBJETO_BANCO_DADOS;
 definirNome(QObject::tr("novo_banco_de_dados"));

 lim_conexao=-1;
 carregando_modelo=false;
 atributos[AtributosParsers::CODIFICACAO]="";
 atributos[AtributosParsers::BD_MODELO]="";
 atributos[AtributosParsers::LIMITE_CONEXAO]="";
 atributos[AtributosParsers::LC_COLLATE_BD]="";
 atributos[AtributosParsers::LC_CTYPE_BD]="";
}
//-----------------------------------------------------------
ModeloBD::~ModeloBD(void)
{
 this->blockSignals(true);
 destruirObjetos();
}
//-----------------------------------------------------------
void ModeloBD::definirCodificacao(TipoCodificacao tipo_cod)
{
 tipo_codif=tipo_cod;
}
//-----------------------------------------------------------
void ModeloBD::definirLocalizacao(int cod_local, const QString &valor)
{
 unsigned idx=0;

 switch(cod_local)
 {
  case LC_CTYPE: idx=0; break;
  case LC_COLLATE:
  default:
   idx=1;
  break;
 }
 localizacoes[idx]=valor;
}
//-----------------------------------------------------------
void ModeloBD::definirLimiteConexao(int lim_conexao)
{
 if(lim_conexao < -1) lim_conexao=-1;
 this->lim_conexao=lim_conexao;
}
//-----------------------------------------------------------
void ModeloBD::definirBDModelo(const QString &bd_modelo)
{
 if(!bd_modelo.isEmpty() && !ObjetoBase::nomeValido(bd_modelo))
  throw Excecao(ERR_PGMODELER_ATRNOMEOBJINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->bd_modelo=bd_modelo;
}
//-----------------------------------------------------------
void ModeloBD::definirAutor(const QString &autor)
{
 this->autor=autor;
}
//-----------------------------------------------------------
vector<ObjetoBase *> *ModeloBD::obterListaObjetos(TipoObjetoBase tipo_obj)
{
 //Retorna a referencia da lista equivalente ao tipo passado
 if(tipo_obj==OBJETO_CAIXA_TEXTO)
  return(&caixas);
 else if(tipo_obj==OBJETO_TABELA)
  return(&tabelas);
 else if(tipo_obj==OBJETO_FUNCAO)
  return(&funcoes);
 else if(tipo_obj==OBJETO_FUNC_AGREGACAO)
  return(&func_agregadas);
 else if(tipo_obj==OBJETO_ESQUEMA)
  return(&esquemas);
 else if(tipo_obj==OBJETO_VISAO)
  return(&visoes);
 else if(tipo_obj==OBJETO_TIPO)
  return(&tipos);
 else if(tipo_obj==OBJETO_PAPEL)
  return(&papeis);
 else if(tipo_obj==OBJETO_ESPACO_TABELA)
  return(&espacos_tabela);
 else if(tipo_obj==OBJETO_LINGUAGEM)
  return(&linguagens);
 else if(tipo_obj==OBJETO_CONV_TIPO)
  return(&conv_tipos);
 else if(tipo_obj==OBJETO_CONV_CODIFICACAO)
  return(&conv_codificacao);
 else if(tipo_obj==OBJETO_OPERADOR)
  return(&operadores);
 else if(tipo_obj==OBJETO_CLASSE_OPER)
  return(&classe_ops);
 else if(tipo_obj==OBJETO_FAMILIA_OPER)
  return(&familia_ops);
 else if(tipo_obj==OBJETO_DOMINIO)
  return(&dominios);
 else if(tipo_obj==OBJETO_SEQUENCIA)
  return(&sequencias);
 else if(tipo_obj==OBJETO_RELACAO_BASE)
  return(&relac_visoes);
 else if(tipo_obj==OBJETO_RELACAO)
  return(&relacionamentos);
 else if(tipo_obj==OBJETO_PERMISSAO)
  return(&permissoes);
 else
  return(NULL);
}
//-----------------------------------------------------------
QString ModeloBD::validarDefinicaoObjeto(ObjetoBase *objeto, unsigned tipo_def)
{
 TipoObjetoBase tipo_obj;
 QString def_obj;

 if(objeto)
 {
  /* Verifica se a definição SQL/XML do objeto é valida por intermédio
     do parser de esquemas, o qual retorna um erro quando existe
     algum problema com a definição sql do objeto */
  try
  {
   tipo_obj=objeto->obterTipoObjeto();

   if(tipo_obj==OBJETO_RELACAO_BASE && tipo_def==ParserEsquema::DEFINICAO_XML)
    def_obj=dynamic_cast<RelacionamentoBase *>(objeto)->obterDefinicaoObjeto();
   else if(tipo_obj==OBJETO_CAIXA_TEXTO && tipo_def==ParserEsquema::DEFINICAO_XML)
    def_obj=dynamic_cast<CaixaTexto *>(objeto)->obterDefinicaoObjeto();
   else
    def_obj=objeto->obterDefinicaoObjeto(tipo_def);
  }
  catch(Excecao &e)
  {
   /* Caso o código do erro seja de atributo obrigatório não preenchido,
      indica que a def. SQL não é válida */
   if(e.obterTipoErro()==ERR_PARSERS_ATRIBVALORNULO)
    throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATROBJDEFSQLINV)
                           .arg(objeto->obterNome(true))
                           .arg(objeto->obterNomeTipoObjeto()),
                  ERR_PGMODELER_ATROBJDEFSQLINV,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
   else
    //Caso o parser dispare os demais erros, apenas redireciona o erro
    throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }

 return(def_obj);
}
//-----------------------------------------------------------
void ModeloBD::adicionarObjeto(ObjetoBase *objeto, int idx_obj)
{
 TipoObjetoBase tipo_obj;

 if(objeto)
 {
  try
  {
   tipo_obj=objeto->obterTipoObjeto();

   if(tipo_obj==OBJETO_RELACAO ||
      tipo_obj==OBJETO_RELACAO_BASE)
    adicionarRelacionamento(dynamic_cast<RelacionamentoBase *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_CAIXA_TEXTO)
    adicionarCaixaTexto(dynamic_cast<CaixaTexto *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_TABELA)
    adicionarTabela(dynamic_cast<Tabela *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_FUNCAO)
    adicionarFuncao(dynamic_cast<Funcao *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_FUNC_AGREGACAO)
    adicionarFuncaoAgregacao(dynamic_cast<FuncaoAgregacao *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_ESQUEMA)
    adicionarEsquema(dynamic_cast<Esquema *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_VISAO)
    adicionarVisao(dynamic_cast<Visao *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_TIPO)
    adicionarTipo(dynamic_cast<Tipo *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_PAPEL)
    adicionarPapel(dynamic_cast<Papel *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_ESPACO_TABELA)
    adicionarEspacoTabela(dynamic_cast<EspacoTabela *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_LINGUAGEM)
    adicionarLinguagem(dynamic_cast<Linguagem *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_CONV_TIPO)
    adicionarConversaoTipo(dynamic_cast<ConversaoTipo *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_CONV_CODIFICACAO)
    adicionarConversaoCodificacao(dynamic_cast<ConversaoCodificacao *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_OPERADOR)
    adicionarOperador(dynamic_cast<Operador *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_CLASSE_OPER)
    adicionarClasseOperadores(dynamic_cast<ClasseOperadores *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_FAMILIA_OPER)
    adicionarFamiliaOperadores(dynamic_cast<FamiliaOperadores *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_DOMINIO)
    adicionarDominio(dynamic_cast<Dominio *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_SEQUENCIA)
    adicionarSequencia(dynamic_cast<Sequencia *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_PERMISSAO)
    adicionarPermissao(dynamic_cast<Permissao *>(objeto));
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}
//-----------------------------------------------------------
void ModeloBD::removerObjeto(ObjetoBase *objeto, int idx_obj)
{
 TipoObjetoBase tipo_obj;


 if(objeto)
 {
  try
  {
   tipo_obj=objeto->obterTipoObjeto();

   if(tipo_obj==OBJETO_RELACAO ||
      tipo_obj==OBJETO_RELACAO_BASE)
    removerRelacionamento(dynamic_cast<RelacionamentoBase *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_CAIXA_TEXTO)
    removerCaixaTexto(dynamic_cast<CaixaTexto *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_TABELA)
    removerTabela(dynamic_cast<Tabela *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_FUNCAO)
    removerFuncao(dynamic_cast<Funcao *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_FUNC_AGREGACAO)
    removerFuncaoAgregacao(dynamic_cast<FuncaoAgregacao *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_ESQUEMA)
    removerEsquema(dynamic_cast<Esquema *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_VISAO)
    removerVisao(dynamic_cast<Visao *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_TIPO)
    removerTipo(dynamic_cast<Tipo *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_PAPEL)
    removerPapel(dynamic_cast<Papel *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_ESPACO_TABELA)
    removerEspacoTabela(dynamic_cast<EspacoTabela *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_LINGUAGEM)
    removerLinguagem(dynamic_cast<Linguagem *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_CONV_TIPO)
    removerConversaoTipo(dynamic_cast<ConversaoTipo *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_CONV_CODIFICACAO)
    removerConversaoCodificacao(dynamic_cast<ConversaoCodificacao *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_OPERADOR)
    removerOperador(dynamic_cast<Operador *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_CLASSE_OPER)
    removerClasseOperadores(dynamic_cast<ClasseOperadores *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_FAMILIA_OPER)
    removerFamiliaOperadores(dynamic_cast<FamiliaOperadores *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_DOMINIO)
    removerDominio(dynamic_cast<Dominio *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_SEQUENCIA)
    removerSequencia(dynamic_cast<Sequencia *>(objeto), idx_obj);
   else if(tipo_obj==OBJETO_PERMISSAO)
    removerPermissao(dynamic_cast<Permissao *>(objeto));
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}
//-----------------------------------------------------------
void ModeloBD::removerObjeto(unsigned idx_obj, TipoObjetoBase tipo_obj)
{
 if(tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_RESTRICAO ||
    tipo_obj==OBJETO_GATILHO || tipo_obj==OBJETO_INDICE ||
    tipo_obj==OBJETO_REGRA ||
    tipo_obj==OBJETO_BASE || tipo_obj==OBJETO_RELACAO_BASE ||
    tipo_obj==OBJETO_BANCO_DADOS)
  //Caso o tipo esteja fora do conjunto, dispara uma exceção
  throw Excecao(ERR_PGMODELER_REMOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso o objeto a ser removido seja uma tabela pai e seu índice seja válido
 else
 {
  vector<ObjetoBase *> *lista_obj=NULL;
  ObjetoBase *objeto=NULL;

  lista_obj=obterListaObjetos(tipo_obj);
  if(idx_obj >= lista_obj->size())
   throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  objeto=(*lista_obj)[idx_obj];
  if(tipo_obj==OBJETO_CAIXA_TEXTO)
   removerCaixaTexto(dynamic_cast<CaixaTexto *>(objeto), idx_obj);
  else if(tipo_obj==OBJETO_TABELA)
   removerTabela(dynamic_cast<Tabela *>(objeto), idx_obj);
  else if(tipo_obj==OBJETO_FUNCAO)
   removerFuncao(dynamic_cast<Funcao *>(objeto), idx_obj);
  else if(tipo_obj==OBJETO_FUNC_AGREGACAO)
   removerFuncaoAgregacao(dynamic_cast<FuncaoAgregacao *>(objeto), idx_obj);
  else if(tipo_obj==OBJETO_ESQUEMA)
   removerEsquema(dynamic_cast<Esquema *>(objeto), idx_obj);
  else if(tipo_obj==OBJETO_VISAO)
   removerVisao(dynamic_cast<Visao *>(objeto), idx_obj);
  else if(tipo_obj==OBJETO_TIPO)
   removerTipo(dynamic_cast<Tipo *>(objeto), idx_obj);
  else if(tipo_obj==OBJETO_PAPEL)
   removerPapel(dynamic_cast<Papel *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_ESPACO_TABELA)
   removerEspacoTabela(dynamic_cast<EspacoTabela *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_LINGUAGEM)
   removerLinguagem(dynamic_cast<Linguagem *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_CONV_TIPO)
   removerConversaoTipo(dynamic_cast<ConversaoTipo *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_CONV_CODIFICACAO)
   removerConversaoCodificacao(dynamic_cast<ConversaoCodificacao *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_OPERADOR)
  removerOperador(dynamic_cast<Operador *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_CLASSE_OPER)
  removerClasseOperadores(dynamic_cast<ClasseOperadores *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_FAMILIA_OPER)
  removerFamiliaOperadores(dynamic_cast<FamiliaOperadores *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_DOMINIO)
  removerDominio(dynamic_cast<Dominio *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_SEQUENCIA)
  removerSequencia(dynamic_cast<Sequencia *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_RELACAO || tipo_obj==OBJETO_RELACAO_BASE)
  removerRelacionamento(dynamic_cast<RelacionamentoBase *>(objeto), idx_obj);
 else if(tipo_obj==OBJETO_PERMISSAO)
  removerPermissao(dynamic_cast<Permissao *>(objeto));
 }
}
//-----------------------------------------------------------
void ModeloBD::__adicionarObjeto(ObjetoBase *objeto, int idx_obj)
{
 int idx;
 TipoObjetoBase tipo_obj;
 vector<ObjetoBase *> *lista_obj=NULL;
 vector<ObjetoBase *>::iterator itr, itr_end;

 if(!objeto)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);


 tipo_obj=objeto->obterTipoObjeto();

 /* Caso o objeto seja um espaço de tabela verifica se não existem
    outro espaço de tabela que esteja apontando para o mesmo diretório
    o que é considerado erro */
 if(tipo_obj==OBJETO_ESPACO_TABELA)
 {
  EspacoTabela *esp_tab=NULL, *esp_tab_aux=NULL;

  lista_obj=obterListaObjetos(tipo_obj);
  itr=lista_obj->begin();
  itr_end=lista_obj->end();
  esp_tab=dynamic_cast<EspacoTabela *>(objeto);

  while(itr!=itr_end)
  {
   esp_tab_aux=dynamic_cast<EspacoTabela *>(*itr);

   //Caso o diretório dos mesmos sejam iguais um erro é disparado
   if(esp_tab->obterDiretorio()==esp_tab_aux->obterDiretorio())
   {
    throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRESPTABDIRDUPLIC)
                         .arg(esp_tab->obterNome())
                         .arg(esp_tab_aux->obterNome()),
                  ERR_PGMODELER_ATRESPTABDIRDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   itr++;
  }
 }

 //Verifica se o objeto a ser inserido já existe no modelo, buscando através do nome.
 if((tipo_obj!=OBJETO_FUNCAO && obterObjeto(objeto->obterNome(true), tipo_obj, idx)) ||
    (tipo_obj==OBJETO_FUNCAO &&
     obterObjeto(dynamic_cast<Funcao *>(objeto)->obterAssinatura(), tipo_obj, idx)))
 {
  QString str_aux;

  str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_ATROBJDUPLIC))
          .arg(objeto->obterNome(true))
          .arg(objeto->obterNomeTipoObjeto())
          .arg(this->obterNome(true))
          .arg(this->obterNomeTipoObjeto());

  throw Excecao(str_aux,ERR_PGMODELER_ATROBJDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 try
 {
  //Valida a definição sql do objeto
  ModeloBD::validarDefinicaoObjeto(objeto, ParserEsquema::DEFINICAO_SQL);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }

 //Obtém a lista de objetos do tipo do novo objeto
 lista_obj=obterListaObjetos(objeto->obterTipoObjeto());

 //Insere o novo elemento na lista
 if(idx_obj < 0 || idx_obj >= static_cast<int>(lista_obj->size()))
  lista_obj->push_back(objeto);
 else
 {
  if(idx_obj >=0 && idx < 0)
   idx=idx_obj;
  else if(idx_obj < 0 && idx < 0)
   idx=0;

  if(lista_obj->size() > 0)
   lista_obj->insert((lista_obj->begin() + idx), objeto);
  else
   lista_obj->push_back(objeto);
 }

 //Emite um sinal indicando a adição do objeto no modelo
 if(!signalsBlocked())
  emit s_objetoAdicionado(objeto);
}
//-----------------------------------------------------------
void ModeloBD::__removerObjeto(ObjetoBase *objeto, int idx_obj)
{
 if(!objeto)
  throw Excecao(ERR_PGMODELER_REMOBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  vector<ObjetoBase *> *lista_obj=NULL;
  TipoObjetoBase tipo_obj;

  tipo_obj=objeto->obterTipoObjeto();

  //Obtém a lista de acordo com o tipo do objeto
  lista_obj=obterListaObjetos(tipo_obj);
  if(!lista_obj)
   throw Excecao(ERR_PGMODELER_OBTOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   //Caso o índice do objeto não foi especificado o método tenta descobrí-lo
   if(idx_obj < 0)
   {
    if(tipo_obj!=OBJETO_FUNCAO && tipo_obj!=OBJETO_OPERADOR)
     obterObjeto(objeto->obterNome(true), tipo_obj, idx_obj);
    else if(tipo_obj==OBJETO_FUNCAO)
     obterObjeto(dynamic_cast<Funcao *>(objeto)->obterAssinatura(), tipo_obj, idx_obj);
    else
     obterObjeto(dynamic_cast<Operador *>(objeto)->obterAssinatura(), tipo_obj, idx_obj);
   }

   if(idx_obj >= 0)
   {
    //Remove as permissões do objeto
    removerPermissoes(objeto);
    lista_obj->erase(lista_obj->begin() + idx_obj);
   }
  }

  //Emite um sinal indicando que o objeto foi removido
  if(!signalsBlocked())
   emit s_objetoRemovido(objeto);
 }
}
//-----------------------------------------------------------
vector<ObjetoBase *> ModeloBD::obterObjetos(TipoObjetoBase tipo_obj, ObjetoBase *esquema)
{
 vector<ObjetoBase *> *lista_obj=NULL, lista_sel;
 vector<ObjetoBase *>::iterator itr, itr_end;

 //Obtém a lista de acordo com o tipo do objeto
 lista_obj=obterListaObjetos(tipo_obj);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
 if(!lista_obj)
  throw Excecao(ERR_PGMODELER_OBTOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=lista_obj->begin();
 itr_end=lista_obj->end();

 while(itr!=itr_end)
 {
  if((*itr)->obterEsquema()==esquema)
   lista_sel.push_back(*itr);
  itr++;
 }

 return(lista_sel);
}
//-----------------------------------------------------------
ObjetoBase *ModeloBD::obterObjeto(const QString &nome, TipoObjetoBase tipo_obj, int &idx_obj)
{
 ObjetoBase *objeto=NULL;
 vector<ObjetoBase *> *lista_obj=NULL;
 vector<ObjetoBase *>::iterator itr, itr_end;
 bool enc=false;
 int qtd;
 QString nome_aux, nome_aux1;

 //Obtém a lista de acordo com o tipo do objeto
 lista_obj=obterListaObjetos(tipo_obj);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
 if(!lista_obj)
  throw Excecao(ERR_PGMODELER_OBTOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Obtém as referências para o inicio e o fim da lista
  itr=lista_obj->begin();
  itr_end=lista_obj->end();
  idx_obj=-1;

  nome_aux1=nome;
  qtd=nome_aux1.count(QChar('\0'));
  if(qtd >=1) nome_aux1.chop(qtd);

  if(tipo_obj!=OBJETO_FUNCAO && tipo_obj!=OBJETO_OPERADOR)
  {
   nome_aux1=ObjetoBase::formatarNome(nome_aux1);

   while(itr!=itr_end && !enc)
   {
    /* Caso o nome do objeto atual seja igual ao nome passado
      o while será quebrado */
    nome_aux=(*itr)->obterNome(true);
    enc=(nome_aux==nome_aux1);
    if(!enc) itr++;
   }
  }
  else
  {
   QString assinatura;

   while(itr!=itr_end && !enc)
   {
    /* Caso especial para função/oeradores: para saber se uma função/operador está duplicada
       deve-se verificar a assinatura da mesma e não apenas o nome, pois
       o PostgreSQL permite a sobrecarga de funções e operadores. Neste caso especial
       o parâmetro informado ao método deve ser uma assinatura de função/operador
       e não seu nome */
    if(tipo_obj==OBJETO_FUNCAO)
     assinatura=dynamic_cast<Funcao *>(*itr)->obterAssinatura();
    else
     assinatura=dynamic_cast<Operador *>(*itr)->obterAssinatura();

    enc=(assinatura==nome_aux1);
    if(!enc) itr++;
   }
  }

  if(enc)
  {
   //Obtém a referência do objeto e calcula seu índice
   objeto=(*itr);
   idx_obj=(itr-lista_obj->begin());
  }
  else idx_obj=-1;
 }

 return(objeto);
}
//-----------------------------------------------------------
ObjetoBase *ModeloBD::obterObjeto(unsigned idx_obj, TipoObjetoBase tipo_obj)
{
 vector<ObjetoBase *> *lista_obj=NULL;

 //Obtém a lista de acordo com o tipo do objeto
 lista_obj=obterListaObjetos(tipo_obj);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
 if(!lista_obj)
  throw Excecao(ERR_PGMODELER_OBTOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Caso o índice do objeto a ser obtido esteja fora do
    intervalo de elementos da lista */
 else if(idx_obj >= lista_obj->size())
  throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  return(lista_obj->at(idx_obj));
}
//-----------------------------------------------------------
unsigned ModeloBD::obterNumObjetos(TipoObjetoBase tipo_obj)
{
 vector<ObjetoBase *> *lista_obj=NULL;

 //Obtém a lista de acordo com o tipo do objeto
 lista_obj=obterListaObjetos(tipo_obj);

 /* Caso a lista não exista indica que foi passado um tipo inválido
    de objeto, dessa forma será retornado um erro */
 if(!lista_obj)
  throw Excecao(ERR_PGMODELER_OBTOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  return(lista_obj->size());
}
//-----------------------------------------------------------
unsigned ModeloBD::obterNumObjetos(void)
{
 TipoObjetoBase tipos[20]={
          OBJETO_RELACAO_BASE,OBJETO_RELACAO, OBJETO_TABELA, OBJETO_VISAO,
          OBJETO_FUNC_AGREGACAO, OBJETO_OPERADOR,
          OBJETO_SEQUENCIA, OBJETO_CONV_CODIFICACAO,
          OBJETO_CONV_TIPO, OBJETO_FAMILIA_OPER, OBJETO_CLASSE_OPER,
          OBJETO_RELACAO_BASE, OBJETO_CAIXA_TEXTO,
          OBJETO_DOMINIO, OBJETO_TIPO, OBJETO_FUNCAO, OBJETO_ESQUEMA,
          OBJETO_LINGUAGEM, OBJETO_ESPACO_TABELA, OBJETO_PAPEL };
 unsigned qtd=0, i;

 for(i=0; i < 20; i++)
  qtd+=obterListaObjetos(tipos[i])->size();

 return(qtd);
}
//-----------------------------------------------------------
QString ModeloBD::obterLocalizacao(int cod_local)
{
 unsigned idx=0;

 switch(cod_local)
 {
  case LC_CTYPE: idx=0; break;
  case LC_COLLATE:
  default:
   idx=1;
  break;
 }
 return(localizacoes[idx]);
}
//-----------------------------------------------------------
int ModeloBD::obterLimiteConexao(void)
{
 return(lim_conexao);
}
//-----------------------------------------------------------
QString ModeloBD::obterBDModelo(void)
{
 return(bd_modelo);
}
//-----------------------------------------------------------
TipoCodificacao ModeloBD::obterCodificacao(void)
{
 return(tipo_codif);
}
//-----------------------------------------------------------
QString ModeloBD::obterAutor(void)
{
 return(autor);
}
//-----------------------------------------------------------
void ModeloBD::definirProtegido(bool protegido)
{
 TipoObjetoBase tipos[19]={
          OBJETO_RELACAO, OBJETO_TABELA, OBJETO_VISAO,
          OBJETO_FUNC_AGREGACAO, OBJETO_OPERADOR,
          OBJETO_SEQUENCIA, OBJETO_CONV_CODIFICACAO,
          OBJETO_CONV_TIPO, OBJETO_FAMILIA_OPER, OBJETO_CLASSE_OPER,
          OBJETO_RELACAO_BASE, OBJETO_CAIXA_TEXTO,
          OBJETO_DOMINIO, OBJETO_TIPO, OBJETO_FUNCAO, OBJETO_ESQUEMA,
          OBJETO_LINGUAGEM, OBJETO_ESPACO_TABELA, OBJETO_PAPEL };
 vector<ObjetoBase *> *lista=NULL;
 vector<ObjetoBase *>::iterator itr, itr_end;
 ObjetoBase *objeto=NULL;
 unsigned i;

 for(i=0; i < 19; i++)
 {
  lista=obterListaObjetos(tipos[i]);
  itr=lista->begin();
  itr_end=lista->end();

  while(itr!=itr_end)
  {
   objeto=(*itr);
   objeto->definirProtegido(protegido);
   itr++;
  }
 }

 ObjetoBase::definirProtegido(protegido);
}
//-----------------------------------------------------------
void ModeloBD::destruirObjetos(void)
{
 /* A ordem de destruição de objetos deve ser seguida de forma que
    os objetos menos dependidos sejam destruídos primeiro para
    evitar falha de segmentação onde, no momento da destruição de algum
    objeto, um dos objetos mais dependendidos sejam referenciados porém
    ja foram destruídos anteriormente. */
 TipoObjetoBase tipos[20]={
          OBJETO_RELACAO_BASE,OBJETO_RELACAO, OBJETO_TABELA, OBJETO_VISAO,
          OBJETO_FUNC_AGREGACAO, OBJETO_OPERADOR,
          OBJETO_SEQUENCIA, OBJETO_CONV_CODIFICACAO,
          OBJETO_CONV_TIPO, OBJETO_FAMILIA_OPER, OBJETO_CLASSE_OPER,
          OBJETO_RELACAO_BASE, OBJETO_CAIXA_TEXTO,
          OBJETO_DOMINIO, OBJETO_TIPO, OBJETO_FUNCAO, OBJETO_ESQUEMA,
          OBJETO_LINGUAGEM, OBJETO_ESPACO_TABELA, OBJETO_PAPEL };
 vector<ObjetoBase *> *lista=NULL;
 ObjetoBase *objeto=NULL;
 unsigned i;

 desconectarRelacionamentos();

 for(i=0; i < 20; i++)
 {
  lista=obterListaObjetos(tipos[i]);

  while(!lista->empty())
  {
   objeto=lista->back();

   if(objeto->obterTipoObjeto()==OBJETO_RELACAO)
    dynamic_cast<Relacionamento *>(objeto)->destruirObjetos();

   delete(objeto);
   lista->pop_back();
  }
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarTabela(Tabela *tabela, int idx_obj)
{
 try
 {
  __adicionarObjeto(tabela, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
Tabela *ModeloBD::obterTabela(unsigned idx_obj)
{
 return(dynamic_cast<Tabela *>(obterObjeto(idx_obj, OBJETO_TABELA)));
}
//-----------------------------------------------------------
void ModeloBD::removerTabela(Tabela *tabela, int idx_obj)
{
 if(tabela)
 {
  vector<ObjetoBase *> vet_refs;
  QString str_aux;

  //Obtém as referências à tabela
  obterReferenciasObjeto(tabela, vet_refs, true);

  //Caso a tabela esteja sendo referenciada, a mesma não pode ser removida
  if(!vet_refs.empty())
  {
   TipoErro tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
       do objeto referenciado */
    if(!dynamic_cast<ObjetoTabela *>(vet_refs[0]))
    {
     tipo_err=ERR_PGMODELER_REMOBJREFERDIR;
     str_aux=QString(Excecao::obterMensagemErro(tipo_err))
             .arg(tabela->obterNome(true))
             .arg(tabela->obterNomeTipoObjeto())
             .arg(vet_refs[0]->obterNome(true))
             .arg(vet_refs[0]->obterNomeTipoObjeto());
    }
    else
    {
     ObjetoBase *obj_ref_pai=dynamic_cast<ObjetoTabela *>(vet_refs[0])->obterTabelaPai();

     //Formata a mensagem caso exista uma referência indireta ao objeto a ser removido
     tipo_err=ERR_PGMODELER_REMOBJREFERIND;
     str_aux=QString(Excecao::obterMensagemErro(tipo_err))
             .arg(tabela->obterNome(true))
             .arg(tabela->obterNomeTipoObjeto())
             .arg(vet_refs[0]->obterNome(true))
             .arg(vet_refs[0]->obterNomeTipoObjeto())
             .arg(obj_ref_pai->obterNome(true))
             .arg(obj_ref_pai->obterNomeTipoObjeto());
    }

    throw Excecao(str_aux, tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

  __removerObjeto(tabela, idx_obj);
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarSequencia(Sequencia *sequencia, int idx_obj)
{
 try
 {
  __adicionarObjeto(sequencia, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
Sequencia *ModeloBD::obterSequencia(unsigned idx_obj)
{
 return(dynamic_cast<Sequencia *>(obterObjeto(idx_obj, OBJETO_SEQUENCIA)));
}
//-----------------------------------------------------------
void ModeloBD::removerSequencia(Sequencia *sequencia, int idx_obj)
{
 if(sequencia)
 {
  __removerObjeto(sequencia, idx_obj);
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarVisao(Visao *visao, int idx_obj)
{
 if(visao)
 {
  try
  {
   __adicionarObjeto(visao, idx_obj);
   atualizarRelTabelaVisao(visao);
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}
//-----------------------------------------------------------
Visao *ModeloBD::obterVisao(unsigned idx_obj)
{
 return(dynamic_cast<Visao *>(obterObjeto(idx_obj, OBJETO_VISAO)));
}
//-----------------------------------------------------------
void ModeloBD::removerVisao(Visao *visao, int idx_obj)
{
 if(visao)
 {
  __removerObjeto(visao, idx_obj);
  atualizarRelTabelaVisao(visao);
 }
}
//-----------------------------------------------------------
void ModeloBD::atualizarRelTabelaVisao(Visao *visao)
{
 Tabela *tab=NULL;
 RelacionamentoBase *rel=NULL;
 Referencia ref;
 unsigned i, qtd_ref, idx;
 vector<ObjetoBase *>::iterator itr, itr_end;

 if(!visao)
  throw Excecao(ERR_PGMODELER_OPROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(obterIndiceObjeto(visao) < 0)
 {
  /* Quando uma visão é excluída, os relacionamentos tabela-visão os quais
     possuem a visão como um dos elementos serão excluídos automaticamente */
  itr=relac_visoes.begin();
  itr_end=relac_visoes.end();

  //Varre a lista de relacionamentos tabela-visão
  idx=0;
  while(itr!=itr_end)
  {
   //Obtém a referência ao relacionamento
   rel=dynamic_cast<RelacionamentoBase *>(*itr);

   //Caso a visão seja um dos elementos do relacionamento
   if(rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM)==visao ||
      rel->obterTabela(RelacionamentoBase::TABELA_DESTINO)==visao)
   {
    //Remove o relacionamento
    removerRelacionamento(rel);
    itr=relac_visoes.begin() + idx;
    itr_end=relac_visoes.end();
   }
   else
   {
    itr++; idx++;
   }
  }
 }
 else
 {
    /* Remove os relacionamentos visão-tabela os quais estão inválidos, ou seja,
     a tabela do relacionametno não está sendo mais referenciada pela visao */
  itr=relac_visoes.begin();
  itr_end=relac_visoes.end();

  //Varre a lista de relacionamentos tabela-visão
  idx=0;
  while(itr!=itr_end)
  {
   //Obtém a referência ao relacionamento
   rel=dynamic_cast<RelacionamentoBase *>(*itr);

   //Caso a visão seja um dos elementos do relacionamento
   if(rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM)==visao ||
      rel->obterTabela(RelacionamentoBase::TABELA_DESTINO)==visao)
   {
    //Obtém a tabela do relacionamento
    if(rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM)->obterTipoObjeto()==OBJETO_TABELA)
     tab=dynamic_cast<Tabela *>(rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM));
    else
     tab=dynamic_cast<Tabela *>(rel->obterTabela(RelacionamentoBase::TABELA_DESTINO));

    //Caso a visão não referencie mais a tabela
    if(!visao->referenciaTabela(tab))
    {
     //Remove o relacionamento
     removerRelacionamento(rel);
     itr=relac_visoes.begin() + idx;
     itr_end=relac_visoes.end();
    }
    else
    {
     itr++; idx++;
    }
   }
   else
   {
    itr++; idx++;
   }
  }

  /* Cria automaticamente os relacionamentos entre as tabelas e a visão.
     As tabelas referenciadas são obtidas das referências na parte SELECT
     da consulta que gera a visão  */
  qtd_ref=visao->obterNumReferencias(Referencia::SQL_REFER_SELECT);

  for(i=0; i < qtd_ref; i++)
  {
   ref=visao->obterReferencia(i, Referencia::SQL_REFER_SELECT);
   tab=ref.obterTabela();

   /* Caso a tabela exista, um relacionamento tabela-visão será automaticamente criado
      (caso este já não existe) e inserido no modelo */
   rel=obterRelacionamento(visao,tab);
   if(tab && !rel)
   {
    rel=new RelacionamentoBase("rel_" + visao->obterNome() + "_" + tab->obterNome(),
                               RelacionamentoBase::RELACIONAMENTO_DEP,visao,tab,false,false);
    adicionarRelacionamento(rel);
   }
  }
 }
}
//-----------------------------------------------------------
void ModeloBD::desconectarRelacionamentos(void)
{
 try
 {
  RelacionamentoBase *rel_base=NULL;
  Relacionamento *rel=NULL;
  vector<ObjetoBase *>::reverse_iterator ritr_rel, ritr_rel_end;

  /* Varre a lista geral de relacionamentos
     do último ao primeiro, desconectando os últimos relacionamentos primeiro
     para evitar falha de segmentação */
  ritr_rel=relacionamentos.rbegin();
  ritr_rel_end=relacionamentos.rend();

  while(ritr_rel!=ritr_rel_end)
  {
   //Converte o ponteiro de relacionamento da conexão para a classe base de relacionametno
   rel_base=dynamic_cast<RelacionamentoBase *>(*ritr_rel);
   ritr_rel++;

   //Caso o relacionamento obtido da lista seja entre tabelas
   if(rel_base->obterTipoObjeto()==OBJETO_RELACAO)
   {
    //Converte o objeto para relacionamento tabela-tabela
    rel=dynamic_cast<Relacionamento *>(rel_base);
    rel->desconectarRelacionamento();
   }
   else
    rel_base->desconectarRelacionamento();
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void ModeloBD::validarRelacionamentos(void)
{
 vector<ObjetoBase *>::iterator itr, itr_end, itr_ant;
 //vector<unsigned> vet_id_objs;
 Relacionamento *rel=NULL;
 RelacionamentoBase *rel_base=NULL;
 vector<ObjetoBase *> vet_rel, vet_rel_inv, rels;
 bool enc_rel_inv;
 vector<Excecao> vet_erros;
 map<unsigned, QString>::iterator itr1, itr1_end;
 map<unsigned, Excecao> mapa_erros;
 map<unsigned, Excecao>::iterator itr2, itr2_end;
 unsigned idx;

 //Obtém os iterador para varredura inicial na lista global de relacionamentos
 itr=relacionamentos.begin();
 itr_end=relacionamentos.end();

 do
 {
  /* Marca como falso a flag indicativa de relacionamento invalidado.
     Este flag sempre será marcado quando 1 ou mais relacionamentos
     forem encontrados durante as interações. */
  enc_rel_inv=false;

  while(itr!=itr_end)
  {
   //Obtém um relacionamento a partir do iterador
   rel_base=dynamic_cast<RelacionamentoBase *>(*itr);
   itr++;

   //Caso o relacionamento obtido seja tabela-tabela
   if(rel_base->obterTipoObjeto()==OBJETO_RELACAO)
   {
    //Converte o relacionamento base para rel. tabela-tabela
    rel=dynamic_cast<Relacionamento *>(rel_base);

    //Caso o relacionamento esteja invalidado
    if(rel->relacionamentoInvalidado())
    {
     //Insere-o na lista de relacionamentos invalidados
     vet_rel_inv.push_back(rel_base);
     /* Marca a flag indicando que pelo menos 1 relacionamento
        invalidado foi encontrado */
     enc_rel_inv=true;
    }
    else
     //Caso não esteja invalidado, insere-o na lista de relacionamentos válidos
     vet_rel.push_back(rel_base);
   }
  }

  //Caso haja algum relacionamento invalidado
  if(enc_rel_inv || !xml_objs_especiais.empty())
  {
   if(!carregando_modelo && xml_objs_especiais.empty())
    obterXMLObjetosEspeciais();

   //Desconecta todos os relacionamentos
   desconectarRelacionamentos();

   //Concatena ambas as lista para reconexão dos relacionamentos
   rels=vet_rel;

   /* Os relacionamentos invalidados vão para o final da lista pois
      estes precisam ser conectados por último para se tornarem válidos */
   rels.insert(rels.end(), vet_rel_inv.begin(), vet_rel_inv.end());

   /* Após criação da lista de relacionamentos temporária (rels) limpa
      as duas listas de relacionamentos usadas na iteração */
   vet_rel.clear();
   vet_rel_inv.clear();

   //Varre a lista temporária de relacionamentos
   itr=rels.begin();
   itr_end=rels.end();
   idx=0;

   while(itr!=itr_end)
   {
    //Obtém o relacionamento
    rel=dynamic_cast<Relacionamento *>(*itr);
    itr_ant=itr;

    //Passa para o próximo relacionamento da lista
    itr++;

    try
    {
     //Executa a conexão do relacionamento
     rel->conectarRelacionamento();
     idx++;
    }
    /* Caso um erro seja disparado durante a conexão do relacionamento o
       mesmo é considerado inválido permanentemente sendo necessário
       sua remoção do modelo */
    catch(Excecao &e)
    {
     /* Remove o relacionamento chamando o método de remoção
        de objetos se verificação */
     __removerObjeto(rel);

     /* Remove o iterador da lista de relacionamentos inválidos para
        evitar que o mesmo seja verificado novamente */
     rels.erase(itr_ant);

     /* Reconfigura a navegação na lista de relacionamentos invalidados.
        Para evitar de que a lista seja varrida novamente desde seu início
        a variável auxiliar 'idx' é usada para apontar o iterador atual
        para o elemento posterior ao que foi removido */
     itr_end=rels.end();
     itr=rels.begin() + idx;

     //Desaloca o relacionamento
     delete(rel);

     //Armazena o erro capturado na lista de erros
     vet_erros.push_back(e);
    }
   }

   /* A nova lista (rels) criada com a nova ordenação de relacionamentos
      passará a ser a lista de relacionamentos a ser varrida nas iterações
      pois é nela que a ordenação de conexão dos relacionamentos é rearranjada. */
   itr=rels.begin();

   /* Caso o flag de modelo em carregamento não esteja marcado será
      feita a recriação dos objetos. Os objetos especiais são recriados
      a cada revalidação dos relacionamentos, sendo assim, tais objetos
      especiais em um dado momento podem ser criados com sucesso e em outros
      não ser criados por quebrarem as referências devido a conexão e
      desconexão constante dos relacionamentos. Quando um erro é encontrado
      na recriação de um objeto especial o mesmo é armazenado em um mapa de
      controle de erros do objeto, onde a chave deste mapa é o identificador
      do próprio objeto gerador do erro. Enquanto tal objeto não for recriado
      com sucesso o erro de geração é armazenado. Ao final, quando todos os
      relacionamentos estiverem sido válidados e o mapa de erros estiver com
      algum elemento de erro, os mesmos são disparados ao usuário indicando
      que alguns objetos especiais não puderam ser criados na validação dos
      relacionamentos. */
   //Obtém os iteradores da lista de xml dos objetos especiais
   itr1=xml_objs_especiais.begin();
   itr1_end=xml_objs_especiais.end();

   if(!carregando_modelo && itr1!=itr1_end)
   {
    do
    {
     try
     {
      //Tenta criar o objeto especial a partir do iterador atual
      criarObjetoEspecial(itr1->second, itr1->first);

      /* Caso algum erro anterior de criação do objeto especial em questão for
         detectado o mesmo é removido do mapa de controle de erros */
      if(mapa_erros.count(itr1->first))
       mapa_erros.erase(mapa_erros.find(itr1->first));

      /* Remove a definição xml do objeto atual da lista indicando
         que o mesmo foi criado com sucesso */
      xml_objs_especiais.erase(itr1);

      //Reinicia a navegação na lista de objetos especiais
      itr1=xml_objs_especiais.begin();
      itr1_end=xml_objs_especiais.end();
     }
     catch(Excecao &e)
     {
      /* Caso uma exceção for caputarada na criação do objeto especial
         armazena o erro no mapa de erros sendo que a chave do mapa
         é o identificador do objeto gerador do erro */
      mapa_erros[itr1->first]=e;
      //Passa para o próximo objeto especial para tentar sua recriação
      itr1++; idx++;
     }
    }
    while(itr1!=itr1_end);
   }
  }
 }
 //Continua a iteração enquanto houver relacionamentos invalidados
 while(enc_rel_inv);

 /* Caso hajam elementos no mapa de controle de erros de criação
    dos objetos especiais, os mesmos são inseridos na lista geral
    de erros de validação para serem disparados ao final do método */
 itr2=mapa_erros.begin();
 itr2_end=mapa_erros.end();
 while(itr2!=itr2_end)
 {
  //Insere o erro na lista geral de erros
  vet_erros.push_back(itr2->second);
  itr2++;
 }

 /* Caso a lista geral de erros não esteja vazia os erros serão
    disparados ao usuário */
 if(!vet_erros.empty())
 {
  /* Limpa a lista de objetos especiais para garantir que eventuais objetos
    não recriados pela quebra das referências não sejam trabalhados pelo
    método de recriação dos objetos especiais */
  xml_objs_especiais.clear();

  throw Excecao(ERR_PGMODELER_REFCOLUNAINVTABELA,__PRETTY_FUNCTION__,__FILE__,__LINE__,vet_erros);
 }
}
//-----------------------------------------------------------
void ModeloBD::verificarRedundanciaRelacoes(Relacionamento *rel)
{
 try
 {
  unsigned tipo_rel;

  /* Retorna um erro caso se tente verificar redundância
     de relacionamentos a partir de um relacionamento
     não alocado */
  if(!rel)
   throw Excecao(ERR_PGMODELER_OPROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  tipo_rel=rel->obterTipoRelacionamento();

  /* Auto relacionamentos são desconsiderados da verificação.
     Já relacionamentos identificadores ou relacionamentos
     os quais possuem atributos identificadores são verificados */
  if((!rel->autoRelacionamento() &&
      (rel->relacionamentoIdentificador() ||
       rel->possuiAtributoIdentificador())) ||

     (tipo_rel==Relacionamento::RELACIONAMENTO_GEN ||
      tipo_rel==Relacionamento::RELACIONAMENTO_DEP))
  {
   TabelaBase *tabela_ref=NULL, *tab_orig=NULL;
   Tabela *tabela_rec=NULL;
   Relacionamento *rel_aux=NULL;
   RelacionamentoBase *rel_base=NULL;
   vector<ObjetoBase *>::iterator itr, itr_end;
   bool ciclo_enc=false;
   unsigned tipo_rel_aux;
   QString str_aux, msg;

   //Obtém tabela referencia e tabela receptora do relacionamento
   tabela_rec=rel->obterTabelaReceptora();
   tabela_ref=rel->obterTabelaReferencia();

   itr=relacionamentos.begin();
   itr_end=relacionamentos.end();

   /* Com base nas tabela obtidas, varre a lista de relacionamento em busca do ciclo.
      Um ciclo será detectado quando a tabela referencia de um relacionamento for
      a tabela recptora do relacionamento usado na validação. */
   while(itr!=itr_end && !ciclo_enc)
   {
    rel_base=dynamic_cast<RelacionamentoBase *>(*itr);
    itr++;

    if(rel_base->obterTipoObjeto()==OBJETO_RELACAO)
    {
     //Obtém um relacionamento da lista
     rel_aux=dynamic_cast<Relacionamento *>(rel_base);
     //Obtém o tipo do relacionamento atual
     tipo_rel_aux=rel_aux->obterTipoRelacionamento();
     //Obtém a tabela referência do relacionamento atual
     tab_orig=rel_aux->obterTabelaReferencia();

     /* Caso a tabela referencia obtida seja igual a tabela receptora
        do relacionamento usado na validação e o tipo do relacionamento
        atual seja o mesmo do relacionamento da validação, isso pode indicar
        um princípio de fechamento de ciclo, para isso a validação prosseguirá
        com a tabela receptora do relacionamento atual até que o a tabela receptora
        seja a própria tabela de referência do relacionamento da validação. */
     if(tabela_rec==tab_orig &&  tipo_rel_aux==tipo_rel &&
        ((!rel_aux->autoRelacionamento() &&
          (rel_aux->relacionamentoIdentificador() ||
           rel_aux->possuiAtributoIdentificador())) ||
         (tipo_rel_aux==Relacionamento::RELACIONAMENTO_GEN ||
          tipo_rel_aux==Relacionamento::RELACIONAMENTO_DEP)))

     {
      //A tabela receptora passará a ser a tabela receptora do relacionamento atual
      tabela_rec=rel_aux->obterTabelaReceptora();
      //Armazena o nome do relacionamento para disparar erro caso um ciclo seja fechado
      str_aux+=rel_aux->obterNome() + ", ";
      /* Um ciclo será detectado caso a tabela receptora seja igual a tabela referência
         do relacionamento usado na validação */
      ciclo_enc=(tabela_rec==tabela_ref);
      //Volta ao início da lista de relacionamento para uma nova validação
      itr=relacionamentos.begin();
     }
    }
   }

   /* Caso um ciclo seja encontrado, dispara um erro contendo os nomes dos
      relacionamentos os quais fecham o ciclo */
   if(ciclo_enc)
   {
    str_aux+=rel->obterNome();
    msg=Excecao::obterMensagemErro(ERR_PGMODELER_INSRELGERAREDUNDANCIA)
        .arg(rel->obterNome())
        .arg(str_aux);
    throw Excecao(msg,ERR_PGMODELER_INSRELGERAREDUNDANCIA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void ModeloBD::obterXMLObjetosEspeciais(void)
{
 unsigned qtd, i, id_tipo;

 vector<ObjetoBase *>::iterator itr, itr_end;
 Sequencia *sequencia=NULL;
 Tabela *tabela=NULL;
 ObjetoTabela *obj_tab=NULL;
 Restricao *restricao=NULL;
 Indice *indice=NULL;
 Gatilho *gatilho=NULL;
 Visao *visao=NULL;
 RelacionamentoBase *rel=NULL;
 Referencia ref;
 TipoObjetoBase tipo_obj_tab[3]={ OBJETO_RESTRICAO, OBJETO_GATILHO, OBJETO_INDICE };
 bool enc=false;

 try
 {
  /* Primeiramente, varre a lista de tabelas em busca de restrições, gatilhos e índices
     os quais possam estar referenciando colunas adicionadas por relacionamentos */
  itr=tabelas.begin();
  itr_end=tabelas.end();

  while(itr!=itr_end)
  {
   //Obtém a tabela a partir do iterador atual
   tabela=dynamic_cast<Tabela *>(*itr);
   itr++;

   //Varre as listas de restrição e índice
   for(id_tipo=0; id_tipo < 3; id_tipo++)
   {
    //Obtém o a quantidade de objetos do tipo de objeto de tabela atual
    qtd=tabela->obterNumObjetos(tipo_obj_tab[id_tipo]);

    //Varre a lista atual de tipo de objetos de tabela
    for(i=0; i < qtd; i++)
    {
     //Obtém um objeto da lista atual na posição atual
     obj_tab=dynamic_cast<ObjetoTabela *>(tabela->obterObjeto(i, tipo_obj_tab[id_tipo]));
     enc=false;

     //Caso seja uma restrição
     if(tipo_obj_tab[id_tipo]==OBJETO_RESTRICAO)
     {
      //Converte o objeto genérico (ObjetoTabela) para restrição
      restricao=dynamic_cast<Restricao *>(obj_tab);

      /* Uma restrição só será considerada como caso especial quando a mesma não foi
         incluída por relacionamento, ou seja, tal restrição a qual referencia colunas
         adicionadas por relacionamentos foi criada pelo usuário. */
      enc=(!restricao->incluidoPorRelacionamento() &&
            restricao->referenciaColunaIncRelacao() &&
            restricao->obterTipoRestricao()!=TipoRestricao::primary_key);

      /* Caso uma restrição seja encontrada obedecendo a condição acima,
         armazena sua definição XML na lista de xml de objetos especiais */
      if(enc)
       xml_objs_especiais[restricao->obterIdObjeto()]=restricao->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_XML, true);
     }
     else if(tipo_obj_tab[id_tipo]==OBJETO_GATILHO)
     {
      //Converte o objeto tabela genérico em gatilho
      gatilho=dynamic_cast<Gatilho *>(obj_tab);

      /* O gatilho só será considerado como especial caso referencie
         colunas adicionadas por relacionamento */
      enc=gatilho->referenciaColunaIncRelacao();

      /* Caso um índice seja encontrado obedecendo a condição acima,
         armazena sua definição XML na lista de xml de objetos especiais */
      if(enc)
       xml_objs_especiais[gatilho->obterIdObjeto()]=gatilho->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_XML);
     }
     //Caso seja um índice
     else
     {
      //Converte o objeto tabela genérico em índice
      indice=dynamic_cast<Indice *>(obj_tab);

      /* O índice só será considerado como especial caso referencie
         colunas adicionadas por relacionamento */
      enc=indice->referenciaColunaIncRelacao();

      /* Caso um índice seja encontrado obedecendo a condição acima,
         armazena sua definição XML na lista de xml de objetos especiais */
      if(enc)
       xml_objs_especiais[indice->obterIdObjeto()]=indice->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_XML);
     }

     //Caso algum objeto especial for encontrado
     if(enc)
     {
      //Remove o mesmo da tabela possuidora
      tabela->removerObjeto(obj_tab->obterNome(), obj_tab->obterTipoObjeto());

      //Remove as permissões existentes para o objeto
      removerPermissoes(obj_tab);

      //Decrementa os controladores da iteração para reiniciar a varredura
      i--; qtd--;
     }
    }
   }
  }

  /* Varre a lista de sequencias para verificar aquelas as quais possam estar
     referenciando colunas adicionadas por relacionamentos. */
  itr=sequencias.begin();
  itr_end=sequencias.end();

  while(itr!=itr_end)
  {
   //Obtém a sequencia atual através do iterador atual
   sequencia=dynamic_cast<Sequencia *>(*itr);
   itr++;

   /* Caso a coluna for incluída por relacionamento considera
      a sequencia como objeto especial */
   if(sequencia->referenciaColunaIncRelacao())
   {
    xml_objs_especiais[sequencia->obterIdObjeto()]=sequencia->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_XML);
    removerSequencia(sequencia);
    delete(sequencia);
   }
  }


  /* Varre a lista de visões para verificar aquelas as quais possam estar
     referenciando colunas adicionadas por relacionamentos. */
  itr=visoes.begin();
  itr_end=visoes.end();

  while(itr!=itr_end)
  {
   //Obtém a visão atual através do iterador atual
   visao=dynamic_cast<Visao *>(*itr);
   itr++;

   /* Caso a visão esteja referenciando uma coluna incluída por
      relacionamento considera a mesma como objeto especial */
   if(visao->referenciaColunaIncRelacao())
   {
    //Armazena a definição XML da visão
    xml_objs_especiais[visao->obterIdObjeto()]=visao->obterDefinicaoObjeto(ParserEsquema::DEFINICAO_XML);

    /* Caso hajam relacionamentos ligando a visão e as tabelas referenciadas
       os mesmo também serão armazenados como objetos especiais para posterior
       recriação */

    //Obtém a quantidade de referências as tabelas
    qtd=visao->obterNumReferencias(Referencia::SQL_REFER_SELECT);

    for(i=0; i < qtd; i++)
    {
     //Obtém uma referência
     ref=visao->obterReferencia(i, Referencia::SQL_REFER_SELECT);
     //Obtém a tabela da referência
     tabela=ref.obterTabela();

     if(tabela)
     {
      /* Caso a tabela exista e um relacionamento tabela-visão entra a visão
         em questão e a tabela obtida, o mesmo será obtido do modelo e
         sua definição XML armazenada */
      rel=obterRelacionamento(visao, tabela);

      if(rel)
      {
       //Armazena a definição xml do relacionamento
       xml_objs_especiais[rel->obterIdObjeto()]=rel->obterDefinicaoObjeto();
       //Remove o mesmo do modelo
       removerRelacionamento(rel);
       //Desaloca o relacionamento obtido
       delete(rel);
      }
     }
    }

    //Remove a visão do modelo e a desaloca
    removerVisao(visao);
    delete(visao);
   }
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void ModeloBD::criarObjetoEspecial(const QString &def_xml_obj, unsigned id_obj)
{
 TipoObjetoBase tipo_obj;
 ObjetoBase *objeto=NULL;

 try
 {
  //Reinicia o parser XML para nova leitura de buffer arquivo
  ParserXML::reiniciarParser();

  //Carrega o buffer XML do parser com a definição XML do objeto especial
  ParserXML::carregarBufferXML(def_xml_obj);

  //Identificando o tipo de objeto de acordo com o elemento obtido anteriormente
  tipo_obj=obterTipoObjeto(ParserXML::obterNomeElemento());

  //Cria o objeto de acordo com o tipo identificado
  objeto=criarObjeto(tipo_obj);

  if(tipo_obj==OBJETO_SEQUENCIA)
   adicionarSequencia(dynamic_cast<Sequencia *>(objeto));
  else if(tipo_obj==OBJETO_VISAO)
   adicionarVisao(dynamic_cast<Visao *>(objeto));

  /* Modifica o id do objeto para o valor do id passado no parâmetro.
     Como um novo id é atribuído quando o objeto é instanciado e levando
     em conta que os objetos especias são realocados e recebem novo id
     isso é necessário para manter a ordem dos objetos da forma como
     os mesmos foram carregados a partir do arquivo evitando a quebra
     de referências. */
  if(objeto && id_obj!=0)
   objeto->id_objeto=id_obj;
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, def_xml_obj);
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarRelacionamento(RelacionamentoBase *relacao, int idx_obj)
{
 try
 {
  TabelaBase *tab1=NULL, *tab2=NULL;
  QString msg;


  if(relacao)
  {
   /* Caso o relacionamento esteja alocado verifica se já não existe um
      relacionamento no modelo entre as tabelas envolvidas */
   //tipo_rel=relacao->obterTipoObjeto();
   tab1=relacao->obterTabela(RelacionamentoBase::TABELA_ORIGEM);
   tab2=relacao->obterTabela(RelacionamentoBase::TABELA_DESTINO);

   //Caso exista um relacionamento entre as tabelas será disparado um erro
   if(obterRelacionamento(tab1,tab2))
   {
    //__removerObjeto(relacao);
    msg=Excecao::obterMensagemErro(ERR_PGMODELER_RELEXISTEMODELO)
        .arg(tab1->obterNomeTipoObjeto())
        .arg(tab1->obterNome(true))
        .arg(tab2->obterNomeTipoObjeto())
        .arg(tab2->obterNome(true));
    throw Excecao(msg,ERR_PGMODELER_RELEXISTEMODELO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  //Adiciona o objeto no modelo
  if(relacao->obterTipoObjeto()==OBJETO_RELACAO)
   verificarRedundanciaRelacoes(dynamic_cast<Relacionamento *>(relacao));

  __adicionarObjeto(relacao, idx_obj);

  if(relacao->obterTipoObjeto()==OBJETO_RELACAO)
  {
   //Conecta o novo relacionamento
   dynamic_cast<Relacionamento *>(relacao)->conectarRelacionamento();

   //Valida os relacionamentos para propagação das colunas
   validarRelacionamentos();
  }
  else
   relacao->conectarRelacionamento();
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void ModeloBD::removerRelacionamento(RelacionamentoBase *relacao, int idx_obj)
{
 try
 {
  if(obterIndiceObjeto(relacao) >= 0)
  {
   if(relacao->obterTipoObjeto()==OBJETO_RELACAO)
   {
    //Desconecta os relacionamentos
    obterXMLObjetosEspeciais();
    desconectarRelacionamentos();
   }
   else if(relacao->obterTipoObjeto()==OBJETO_RELACAO_BASE)
    relacao->desconectarRelacionamento();

   __removerObjeto(relacao, idx_obj);

   if(relacao->obterTipoObjeto()==OBJETO_RELACAO)
   {
    //Valida os relacionamentos após a remoção de o relacionamento atual
    validarRelacionamentos();
   }
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
RelacionamentoBase *ModeloBD::obterRelacionamento(unsigned idx_obj, TipoObjetoBase tipo_rel)
{
 //Caso o tipo de relacionamento seja inválido
 if(tipo_rel!=OBJETO_RELACAO && tipo_rel!=OBJETO_RELACAO_BASE)
  throw Excecao(ERR_PGMODELER_OBTOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(dynamic_cast<RelacionamentoBase *>(obterObjeto(idx_obj, tipo_rel)));
}
//-----------------------------------------------------------
RelacionamentoBase *ModeloBD::obterRelacionamento(TabelaBase *tab_orig, TabelaBase *tab_dest)
{
 vector<ObjetoBase *>::iterator itr, itr_end;
 RelacionamentoBase *rel=NULL;
 bool enc=false, buscar_tab_unica=false;
 TabelaBase *tab1=NULL, *tab2=NULL;

 if(tab_orig)
 {
  /* Caso a tabela de destino não esteja especificada,
     indica que que o método deve procurar por relacionamentos
     onde a tabela de origem (tab_orig)  existe não importando se ela é a
     referência ou a receptora. A execução normal (tab_dest!=NULL)
     o método só retorna um relacionamento onde ambas as tabelas estão
     presentas */
  if(!tab_dest)
  {
   tab_dest=tab_orig;
   buscar_tab_unica=true;
 }

  /* Definindo os iteradores de acordo com os objetos envolvidos
     no relacionamento */

  /* Caso um dos objetos seja uma visão, a lista de relacionamentos
     tabela-visão será varrida, caso contrário a lista de relacionamentos
     tabela-tabela será usada */
  if(tab_orig->obterTipoObjeto()==OBJETO_VISAO ||
     tab_dest->obterTipoObjeto()==OBJETO_VISAO)
  {
   itr=relac_visoes.begin();
   itr_end=relac_visoes.end();
  }
  else
  {
   itr=relacionamentos.begin();
   itr_end=relacionamentos.end();
  }

  while(itr!=itr_end && !enc)
  {
   //Obtém a referência ao relacionamento
   rel=dynamic_cast<RelacionamentoBase *>(*itr);
   tab1=rel->obterTabela(RelacionamentoBase::TABELA_ORIGEM);
   tab2=rel->obterTabela(RelacionamentoBase::TABELA_DESTINO);

   /* Verifica se os elementos do parâmetro coincidem com os elementos
      do relacionamento */
   enc=((tab1==tab_orig && tab2==tab_dest) ||
        (tab2==tab_orig && tab1==tab_dest) ||
        (buscar_tab_unica && (tab1==tab_orig || tab2==tab_orig)));
   if(!enc)
   { rel=NULL; itr++; }
  }
 }

 return(rel);
}
//-----------------------------------------------------------
void ModeloBD::adicionarCaixaTexto(CaixaTexto *caixa, int idx_obj)
{
 try
 {
  __adicionarObjeto(caixa, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void ModeloBD::removerCaixaTexto(CaixaTexto *caixa, int idx_obj)
{
 __removerObjeto(caixa, idx_obj);
}
//-----------------------------------------------------------
CaixaTexto *ModeloBD::obterCaixaTexto(unsigned idx_obj)
{
 return(dynamic_cast<CaixaTexto *>(obterObjeto(idx_obj, OBJETO_CAIXA_TEXTO)));
}
//-----------------------------------------------------------
void ModeloBD::adicionarEsquema(Esquema *esquema, int idx_obj)
{
 try
 {
  __adicionarObjeto(esquema, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
Esquema *ModeloBD::obterEsquema(unsigned idx_obj)
{
 return(dynamic_cast<Esquema *>(obterObjeto(idx_obj, OBJETO_ESQUEMA)));
}
//-----------------------------------------------------------
void ModeloBD::removerEsquema(Esquema *esquema, int idx_obj)
{
 if(esquema)
 {
  vector<ObjetoBase *> vet_refs;

  //Obtém as referências ao esquema
  obterReferenciasObjeto(esquema, vet_refs, true);

 /* Caso o esquema esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */

   throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELER_REMOBJREFERDIR))
                 .arg(esquema->obterNome(true))
                 .arg(esquema->obterNomeTipoObjeto())
                 .arg(vet_refs[0]->obterNome(true))
                 .arg(vet_refs[0]->obterNomeTipoObjeto()),
                 ERR_PGMODELER_REMOBJREFERDIR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(esquema, idx_obj);
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarPapel(Papel *papel, int idx_obj)
{
 try
 {
  __adicionarObjeto(papel, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
Papel *ModeloBD::obterPapel(unsigned idx_obj)
{
 return(dynamic_cast<Papel *>(obterObjeto(idx_obj, OBJETO_PAPEL)));
}
//-----------------------------------------------------------
void ModeloBD::removerPapel(Papel *papel, int idx_obj)
{
 if(papel)
 {
  vector<ObjetoBase *> vet_refs;

  //Obtém as referências ao papel
  obterReferenciasObjeto(papel, vet_refs, true);

 /* Caso o papel esteja sendo referenciado, por algum objeto a
     mesma não pode ser removido */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELER_REMOBJREFERDIR))
                 .arg(papel->obterNome(true))
                 .arg(papel->obterNomeTipoObjeto())
                 .arg(vet_refs[0]->obterNome(true))
                 .arg(vet_refs[0]->obterNomeTipoObjeto()),
                 ERR_PGMODELER_REMOBJREFERDIR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(papel, idx_obj);
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarEspacoTabela(EspacoTabela *espaco_tab, int idx_obj)
{
 try
 {
  __adicionarObjeto(espaco_tab, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
EspacoTabela *ModeloBD::obterEspacoTabela(unsigned idx_obj)
{
 return(dynamic_cast<EspacoTabela *>(obterObjeto(idx_obj, OBJETO_ESPACO_TABELA)));
}
//-----------------------------------------------------------
void ModeloBD::removerEspacoTabela(EspacoTabela *espaco_tab, int idx_obj)
{
 if(espaco_tab)
 {
  vector<ObjetoBase *> vet_refs;
  QString str_aux;

  //Obtém as referências ao espaço de tabela
  obterReferenciasObjeto(espaco_tab, vet_refs, true);

  /* Caso o esquema esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   TipoErro tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   if(!dynamic_cast<ObjetoTabela *>(vet_refs[0]))
   {
    //Formata a mensagem para referencia direta
    tipo_err=ERR_PGMODELER_REMOBJREFERDIR;
    str_aux=QString(Excecao::obterMensagemErro(tipo_err))
            .arg(espaco_tab->obterNome(true))
            .arg(espaco_tab->obterNomeTipoObjeto())
            .arg(vet_refs[0]->obterNome(true))
            .arg(vet_refs[0]->obterNomeTipoObjeto());
   }
   else
   {
    ObjetoBase *obj_ref_pai=dynamic_cast<ObjetoTabela *>(vet_refs[0])->obterTabelaPai();
    //Formata a mensagem para referencia indireta
    tipo_err=ERR_PGMODELER_REMOBJREFERIND;
    str_aux=QString(Excecao::obterMensagemErro(tipo_err))
            .arg(espaco_tab->obterNome(true))
            .arg(espaco_tab->obterNomeTipoObjeto())
            .arg(vet_refs[0]->obterNome(true))
            .arg(vet_refs[0]->obterNomeTipoObjeto())
            .arg(obj_ref_pai->obterNome(true))
            .arg(obj_ref_pai->obterNomeTipoObjeto());

   }

   throw Excecao(str_aux,tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(espaco_tab, idx_obj);
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarConversaoTipo(ConversaoTipo *conv_tipo, int idx_obj)
{
 try
 {
  __adicionarObjeto(conv_tipo, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void ModeloBD::removerConversaoTipo(ConversaoTipo *conv_tipo, int idx_obj)
{
 __removerObjeto(conv_tipo, idx_obj);
}
//-----------------------------------------------------------
ConversaoTipo *ModeloBD::obterConversaoTipo(unsigned idx_obj)
{
 return(dynamic_cast<ConversaoTipo *>(obterObjeto(idx_obj, OBJETO_CONV_TIPO)));
}
//-----------------------------------------------------------
void ModeloBD::adicionarConversaoCodificacao(ConversaoCodificacao *conv_codificacao, int idx_obj)
{
 try
 {
  __adicionarObjeto(conv_codificacao, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void ModeloBD::removerConversaoCodificacao(ConversaoCodificacao *conv_codificacao, int idx_obj)
{
 __removerObjeto(conv_codificacao, idx_obj);
}
//-----------------------------------------------------------
ConversaoCodificacao *ModeloBD::obterConversaoCodificacao(unsigned idx_obj)
{
 return(dynamic_cast<ConversaoCodificacao *>(obterObjeto(idx_obj,
 OBJETO_CONV_CODIFICACAO)));
}
//-----------------------------------------------------------
void ModeloBD::adicionarLinguagem(Linguagem *linguagem, int idx_obj)
{
 try
 {
  __adicionarObjeto(linguagem, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
Linguagem *ModeloBD::obterLinguagem(unsigned idx_obj)
{
 return(dynamic_cast<Linguagem *>(obterObjeto(idx_obj, OBJETO_LINGUAGEM)));
}
//-----------------------------------------------------------
void ModeloBD::removerLinguagem(Linguagem *linguagem, int idx_obj)
{
 if(linguagem)
 {
  vector<ObjetoBase *> vet_refs;

  //Obtém as referências à linguagem
  obterReferenciasObjeto(linguagem, vet_refs, true);

 /* Caso a linguagem esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELER_REMOBJREFERDIR))
                 .arg(linguagem->obterNome(true))
                 .arg(linguagem->obterNomeTipoObjeto())
                 .arg(dynamic_cast<Funcao *>(vet_refs[0])->obterAssinatura())
                 .arg(vet_refs[0]->obterNomeTipoObjeto()),
                 ERR_PGMODELER_REMOBJREFERDIR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(linguagem, idx_obj);
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarFuncao(Funcao *funcao, int idx_obj)
{
 try
 {
  __adicionarObjeto(funcao, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
Funcao *ModeloBD::obterFuncao(unsigned idx_obj)
{
 return(dynamic_cast<Funcao *>(obterObjeto(idx_obj, OBJETO_FUNCAO)));
}
//-----------------------------------------------------------
void ModeloBD::removerFuncao(Funcao *funcao, int idx_obj)
{
 if(funcao)
 {
  vector<ObjetoBase *> vet_refs;
  QString str_aux;

  //Obtém as referências à função
  obterReferenciasObjeto(funcao, vet_refs, true);

 /* Caso a função esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   TipoErro tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   if(!dynamic_cast<ObjetoTabela *>(vet_refs[0]))
   {
    //Formata a mensagem para referência direta
    tipo_err=ERR_PGMODELER_REMOBJREFERDIR;
    str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REMOBJREFERDIR))
            .arg(funcao->obterAssinatura())
            .arg(funcao->obterNomeTipoObjeto())
            .arg(vet_refs[0]->obterNome(true))
            .arg(vet_refs[0]->obterNomeTipoObjeto());

   }
   else
   {
    ObjetoBase *obj_ref_pai=dynamic_cast<ObjetoTabela *>(vet_refs[0])->obterTabelaPai();
    //Formata a mensagem para referência indireta
    tipo_err=ERR_PGMODELER_REMOBJREFERIND;
    str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REMOBJREFERIND))
            .arg(funcao->obterAssinatura())
            .arg(funcao->obterNomeTipoObjeto())
            .arg(vet_refs[0]->obterNome(true))
            .arg(vet_refs[0]->obterNomeTipoObjeto())
            .arg(obj_ref_pai->obterNome(true))
            .arg(obj_ref_pai->obterNomeTipoObjeto());

   }

   throw Excecao(str_aux,tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(funcao, idx_obj);
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarFuncaoAgregacao(FuncaoAgregacao *func_agregacao, int idx_obj)
{
 try
 {
  __adicionarObjeto(func_agregacao, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
FuncaoAgregacao *ModeloBD::obterFuncaoAgregacao(unsigned idx_obj)
{
 return(dynamic_cast<FuncaoAgregacao *>(obterObjeto(idx_obj, OBJETO_FUNC_AGREGACAO)));
}
//-----------------------------------------------------------
void ModeloBD::removerFuncaoAgregacao(FuncaoAgregacao *func_agregacao, int idx_obj)
{
 __removerObjeto(func_agregacao, idx_obj);
}
//-----------------------------------------------------------
void ModeloBD::adicionarDominio(Dominio *dominio, int idx_obj)
{
 if(dominio)
 {
  vector<ObjetoBase *>::iterator itr, itr_end;
  bool enc=false;
  QString str_aux;

  /* Varre a lista de tipos para verificar se existe algum tipo
     com o mesmo nome do domínio */
  itr=tipos.begin();
  itr_end=tipos.end();
  while(itr!=itr_end && !enc)
  {
   enc=((*itr)->obterNome(true)==dominio->obterNome(true));
   itr++;
  }

  //Caso exista um tipo de mesmo nome que o domínio dispara o erro
  if(enc)
  {
   str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_ATROBJDUPLIC))
           .arg(dominio->obterNome(true))
           .arg(dominio->obterNomeTipoObjeto())
           .arg(this->obterNome(true))
           .arg(this->obterNomeTipoObjeto());
   throw Excecao(str_aux, ERR_PGMODELER_ATROBJDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  try
  {
   //Adiciona o domínio
   __adicionarObjeto(dominio, idx_obj);

   /* Ao ser inserido um novo tipo o mesmo tem
    seu nome é adicionado à lista de tipos válidos
    do PostgreSQL */
   TipoPgSQL::adicionarTipoUsuario(dominio->obterNome(true), dominio, this, true);
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}
//-----------------------------------------------------------
void ModeloBD::removerDominio(Dominio *dominio, int idx_obj)
{
 try
 {
  removerTipoUsuario(dominio, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
Dominio *ModeloBD::obterDominio(unsigned idx_obj)
{
 return(dynamic_cast<Dominio *>(obterObjeto(idx_obj, OBJETO_DOMINIO)));
}
//-----------------------------------------------------------
void ModeloBD::adicionarFamiliaOperadores(FamiliaOperadores *familia_op, int idx_obj)
{
 try
 {
  __adicionarObjeto(familia_op, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
FamiliaOperadores *ModeloBD::obterFamiliaOperadores(unsigned idx_obj)
{
 return(dynamic_cast<FamiliaOperadores *>(obterObjeto(idx_obj, OBJETO_FAMILIA_OPER)));
}
//-----------------------------------------------------------
void ModeloBD::removerFamiliaOperadores(FamiliaOperadores *familia_op, int idx_obj)
{
 if(familia_op)
 {
  vector<ObjetoBase *> vet_refs;

  //OBtém as referências ao objeto
  obterReferenciasObjeto(familia_op, vet_refs, true);

   /* Caso a familía esteja sendo referenciada, por algum objeto a
       mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELER_REMOBJREFERDIR))
                 .arg(familia_op->obterNome(true))
                 .arg(familia_op->obterNomeTipoObjeto())
                 .arg(vet_refs[0]->obterNome(true))
                 .arg(vet_refs[0]->obterNomeTipoObjeto()),
                 ERR_PGMODELER_REMOBJREFERDIR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(familia_op, idx_obj);
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarClasseOperadores(ClasseOperadores *classe_op, int idx_obj)
{
 try
 {
  __adicionarObjeto(classe_op, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void ModeloBD::removerClasseOperadores(ClasseOperadores *classe_op, int idx_obj)
{
 __removerObjeto(classe_op, idx_obj);
}
//-----------------------------------------------------------
ClasseOperadores *ModeloBD::obterClasseOperadores(unsigned idx_obj)
{
 return(dynamic_cast<ClasseOperadores *>(obterObjeto(idx_obj, OBJETO_CLASSE_OPER)));
}
//-----------------------------------------------------------
void ModeloBD::adicionarOperador(Operador *operador, int idx_obj)
{
 try
 {
  __adicionarObjeto(operador, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void ModeloBD::removerOperador(Operador *operador, int idx_obj)
{
 if(operador)
 {
  vector<ObjetoBase *> vet_refs;

  //Obtém as referências ao operador
  obterReferenciasObjeto(operador, vet_refs, true);

 /* Caso o operador esteja sendo referenciado, por algum objeto a
     mesma não pode ser removida */
  if(!vet_refs.empty())
  {
   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   //Formata a mensagem para referência direta
   throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELER_REMOBJREFERDIR))
                 .arg(operador->obterAssinatura(true))
                 .arg(operador->obterNomeTipoObjeto())
                 .arg(vet_refs[0]->obterNome(true))
                 .arg(vet_refs[0]->obterNomeTipoObjeto()),
                 ERR_PGMODELER_REMOBJREFERDIR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(operador, idx_obj);
 }
}
//-----------------------------------------------------------
Operador *ModeloBD::obterOperador(unsigned idx_obj)
{
 return(dynamic_cast<Operador *>(obterObjeto(idx_obj, OBJETO_OPERADOR)));
}
//-----------------------------------------------------------
void ModeloBD::adicionarTipo(Tipo *tipo, int idx_obj)
{
 if(tipo)
 {
  vector<ObjetoBase *>::iterator itr, itr_end;
  bool enc=false;
  QString str_aux;

  /* Varre a lista de dominios para verificar se existe algum
     domínio com o mesmo nome do tipo */
  itr=dominios.begin();
  itr_end=dominios.end();
  while(itr!=itr_end && !enc)
  {
   enc=((*itr)->obterNome(true)==tipo->obterNome(true));
   itr++;
  }

  //Caso exista um dominio de mesmo nome que o tipo dispara o erro
  if(enc)
  {
   str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_ATROBJDUPLIC))
           .arg(tipo->obterNome(true))
           .arg(tipo->obterNomeTipoObjeto())
           .arg(this->obterNome(true))
           .arg(this->obterNomeTipoObjeto());
   throw Excecao(str_aux, ERR_PGMODELER_ATROBJDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  try
  {
   //Adiciona o tipo
   __adicionarObjeto(tipo, idx_obj);

   /* Ao ser inserido um novo tipo o mesmo tem
    seu nome é adicionado à lista de tipos válidos
    do PostgreSQL */
   TipoPgSQL::adicionarTipoUsuario(tipo->obterNome(true), tipo, this, false);
  }
  catch(Excecao &e)
  {
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
 }
}
//-----------------------------------------------------------
void ModeloBD::removerTipo(Tipo *tipo, int idx_obj)
{
 try
 {
  removerTipoUsuario(tipo, idx_obj);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
Tipo *ModeloBD::obterTipo(unsigned idx_obj)
{
 return(dynamic_cast<Tipo *>(obterObjeto(idx_obj, OBJETO_TIPO)));
}
//-----------------------------------------------------------
void ModeloBD::removerTipoUsuario(ObjetoBase *objeto, int idx_obj)
{
 if(objeto)
 {
  vector<ObjetoBase *> vet_refs;
  QString str_aux;

  //Obtém as referências ao objeto
  obterReferenciasObjeto(objeto, vet_refs, true);

  /* Caso o tipo esteja sendo referenciado, por algum objeto o
     mesmo não pode ser removida */
  if(!vet_refs.empty())
  {
   TipoErro tipo_err;

   /* Formatando a mensagem de erro com o nome e tipo do objeto que referencia e
      do objeto referenciado */
   if(!dynamic_cast<ObjetoTabela *>(vet_refs[0]))
   {
    //Formata a mensagem para referência direta
    tipo_err=ERR_PGMODELER_REMOBJREFERDIR;
    str_aux=QString(Excecao::obterMensagemErro(tipo_err))
            .arg(objeto->obterNome(true))
            .arg(objeto->obterNomeTipoObjeto())
            .arg(vet_refs[0]->obterNome(true))
            .arg(vet_refs[0]->obterNomeTipoObjeto());
   }
   else
   {
    ObjetoBase *obj_ref_pai=dynamic_cast<ObjetoTabela *>(vet_refs[0])->obterTabelaPai();

    //Formata a mensagem para referência indireta
    tipo_err=ERR_PGMODELER_REMOBJREFERIND;
    str_aux=QString(Excecao::obterMensagemErro(tipo_err))
            .arg(objeto->obterNome(true))
            .arg(objeto->obterNomeTipoObjeto())
            .arg(vet_refs[0]->obterNome(true))
            .arg(vet_refs[0]->obterNomeTipoObjeto())
            .arg(obj_ref_pai->obterNome(true))
            .arg(obj_ref_pai->obterNomeTipoObjeto());
   }

   throw Excecao(str_aux,tipo_err,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  __removerObjeto(objeto, idx_obj);
   /* Ao ser removido do modelo o objeto (tipo ou domínio) classe tem
    seu nome removido da lista de tipos válidos do PostgreSQL */
  TipoPgSQL::removerTipoUsuario(objeto->obterNome(true), objeto);
 }
}
//-----------------------------------------------------------
void ModeloBD::adicionarPermissao(Permissao *permissao)
{
 try
 {
  if(!permissao)
   throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  if(obterIndicePermissao(permissao) >=0)
  {
  /* Caso haja uma permissão semelhante a que está sendo inserida, ou seja,
    o resultado da chamada ao metodo obterIndicePermissao() sejá >= 0,
    um erro será disparado */
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRPERMISSAODUPLIC)
                .arg(permissao->obterObjeto()->obterNome())
                .arg(permissao->obterObjeto()->obterNomeTipoObjeto()),
                ERR_PGMODELER_ATRPERMISSAODUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  permissoes.push_back(permissao);
 }
 catch(Excecao &e)
 {
  if(e.obterTipoErro()==ERR_PGMODELER_ATROBJDUPLIC)
   throw
   Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRPERMISSAODUPLIC)
           .arg(permissao->obterObjeto()->obterNome())
           .arg(permissao->obterObjeto()->obterNomeTipoObjeto()),
           ERR_PGMODELER_ATRPERMISSAODUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);

  else
   throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
 }
}
//-----------------------------------------------------------
void ModeloBD::removerPermissao(Permissao *permissao)
{
 if(permissao)
 {
  __removerObjeto(permissao);
 }
}
//-----------------------------------------------------------
void ModeloBD::removerPermissoes(ObjetoBase *objeto)
{
 vector<Permissao *> vet_perm;
 vector<ObjetoBase *>::iterator itr, itr_end;
 Permissao *permissao=NULL;
 unsigned idx=0;

 /* Caso o objeto o qual terá todas as permissões relacionadas removidas não
    esteja alocado um erro será disparado pois o usuário está
    tentando remover permissões de um objeto inexistente */
 if(!objeto)
  throw Excecao(ERR_PGMODELER_OPROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=permissoes.begin();
 itr_end=permissoes.end();

 /* Varre a lista de permissões removendo as permissões cujo
    objeto relacionado seja o mesmo do objeto do parâmetro */
 while(itr!=itr_end)
 {
  //Obtém a permissão
  permissao=dynamic_cast<Permissao *>(*itr);

  //Verifica se o objeto da permissão é igual ao objeto do parâmetro
  if(permissao->obterObjeto()==objeto)
  {
   //Remove o iterador que possui a permissão
   permissoes.erase(itr);
   //Desaloca a permissão
   delete(permissao);
   //Reinicializa os iteradores para uma nova varredura
   itr=itr_end=permissoes.end();

   /* Caso a lista não esteja vazia após remover a permissão
      desloca o iterador para a posição em que a varredura
      parou ao invés de iniciar uma nova a partir do início
      da lista */
   if(!permissoes.empty())
    itr=permissoes.begin() + idx;
  }
  else
  { itr++; idx++; }
 }
}
//-----------------------------------------------------------
void ModeloBD::obterPermissoes(ObjetoBase *objeto, vector<Permissao *> &vet_perm)
{
 vector<ObjetoBase *>::iterator itr, itr_end;
 Permissao *permissao=NULL;

 /* Caso o objeto o qual terá todas as permissões relacionadas
    obtidas não esteja alocado um erro será disparado pois o
    usuário está tentando obter permissões de um objeto inexistente */
 if(!objeto)
  throw Excecao(ERR_PGMODELER_OPROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 itr=permissoes.begin();
 itr_end=permissoes.end();

 //Limpa a lista receptora de permissões
 vet_perm.clear();

 /* Varre a lista de permissões em busca daquelas
    cujo objeto seja o mesmo do objeto especificado
    no parâmetro */
 while(itr!=itr_end)
 {
  //Obtém a permissão
  permissao=dynamic_cast<Permissao *>(*itr);

  /* Caso a permissão esteja relacionada ao mesmo
     objeto do parâmetro insere tal permissão
     à lista de permissões */
  if(permissao->obterObjeto()==objeto)
   vet_perm.push_back(permissao);

  itr++;
 }
}
//-----------------------------------------------------------
int ModeloBD::obterIndicePermissao(Permissao *permissao)
{
 int idx_perm=-1;

 //Obtém o índice da permissão somente se a mesma estive alocada
 if(permissao)
 {
  Permissao *perm_aux=NULL;
  vector<ObjetoBase *>::iterator itr, itr_end;
  ObjetoBase *objeto=NULL;
  Papel *papel=NULL;
  unsigned qtd, i;
  bool papel_ref=false;

  itr=permissoes.begin();
  itr_end=permissoes.end();

  //Obtém o objeto da permissão
  objeto=permissao->obterObjeto();

  /* Varre a lista de permissões enquanto não chegar ao final
     da lista ou enquanto o índice da permissão não for
     descoberto */
  while(itr!=itr_end && idx_perm < 0)
  {
   //Obtém uma permissão da lista
   perm_aux=dynamic_cast<Permissao *>(*itr);

   /*CAso o objeto do parâmetro seja o mesmo da permissão
     será efetuada uma validação se todos os papeis
     de ambas as permissões são iguais, isso indica
     que ambas possuem o mesmo efeito */
   if(objeto==perm_aux->obterObjeto())
   {
    //Obtém a quantidade de papéis
    qtd=permissao->obterNumPapeis();

    //Varre a lista de papéis das permissões
    for(i=0; i < qtd && !papel_ref; i++)
    {
     //Obtém um papel da permissão do parâmetro
     papel=permissao->obterPapel(i);
     /* Com o papel obtido verifica-se se o mesmo é referenciado
        no papel obtido da lista principal de permissões */
     papel_ref=perm_aux->papelReferenciado(papel);
    }
   }

   /* Caso um papel ambas as permissões seja idênticas
      (mesmo endereço de memória) ou todos os papéis referenciados
      por ambas sejam os mesmos, o índice da permissão na lista
      será calculado */
   if(permissao==perm_aux || papel_ref)
    idx_perm=itr-permissoes.begin();

   itr++;
  }
 }

 return(idx_perm);
}
//-----------------------------------------------------------
ObjetoBase *ModeloBD::obterObjeto(const QString &nome, TipoObjetoBase tipo_obj)
{
 int idx;
 return(obterObjeto(nome, tipo_obj, idx));
}
//-----------------------------------------------------------
int ModeloBD::obterIndiceObjeto(const QString &nome, TipoObjetoBase tipo_obj)
{
 int idx;
 obterObjeto(nome, tipo_obj, idx);
 return(idx);
}
//-----------------------------------------------------------
int ModeloBD::obterIndiceObjeto(ObjetoBase *objeto)
{
 if(!objeto)
  return(-1);
 else
 {
  TipoObjetoBase tipo_obj=objeto->obterTipoObjeto();
  vector<ObjetoBase *> *lista_obj=NULL;
  vector<ObjetoBase *>::iterator itr, itr_end;
  bool enc=false;

  //Obtém a lista de acordo com o tipo do objeto
  lista_obj=obterListaObjetos(tipo_obj);

  /* Caso a lista não exista indica que foi passado um tipo inválido
     de objeto, dessa forma será retornado um erro */
  if(!lista_obj)
   throw Excecao(ERR_PGMODELER_OBTOBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   //Obtém as referências para o inicio e o fim da lista
   itr=lista_obj->begin();
   itr_end=lista_obj->end();

   while(itr!=itr_end && !enc)
   {
    enc=((*itr)==objeto);
    if(!enc) itr++;
   }

   if(enc)
    return(itr-lista_obj->begin());
   else
    return(-1);
  }
 }
}
//-----------------------------------------------------------
bool ModeloBD::carregandoModelo(void)
{
 return(carregando_modelo);
}
//-----------------------------------------------------------
void ModeloBD::carregarModelo(const QString &nome_arq)
{
 if(nome_arq!="")
 {
  QString arq_dtd, str_aux, nome_elem;
  TipoObjetoBase tipo_obj;
  map<QString, QString> atributos;
  ObjetoBase *objeto=NULL;
  deque<const xmlNode *> objetos_incomp;
  bool reaval_objetos=false, modelo_protegido=false;
  const xmlNode *elem_aux=NULL;
  deque<Excecao> vet_erros;
  map<unsigned, QString>::iterator itr, itr_end;

  /* Montando o caminho padrão para localização dos esquemas DTD.
     Por padrão esta pasta se encontra na raiz dentro da pasta esquemas/xml*/
  arq_dtd=AtributosGlobais::DIR_RAIZ_ESQUEMAS +
           AtributosGlobais::SEP_DIRETORIO +
           AtributosGlobais::DIR_ESQUEMA_XML +
           AtributosGlobais::SEP_DIRETORIO +
           AtributosGlobais::DIR_DTD_OBJETO +
           AtributosGlobais::SEP_DIRETORIO;

  try
  {
   carregando_modelo=true;

   //Reinicia o parser XML para a leitura do arquivo
   ParserXML::reiniciarParser();
   //Faz com que o parser carregue a DTD que armazena a sintaxe do arquivo de modelos
   ParserXML::definirArquivoDTD(arq_dtd + AtributosGlobais::DTD_RAIZ +
                                AtributosGlobais::EXT_DTD_OBJETO,
                                AtributosGlobais::DTD_RAIZ);

   //Carrega o arquivo validando-o de acordo com a DTD informada
   ParserXML::carregarArquivoXML(nome_arq);

   //Obter as informações de versão, autor do modelo e versão postgresql
   ParserXML::obterAtributosElemento(atributos);
   this->autor=atributos[AtributosParsers::AUTOR_MODELO];

   modelo_protegido=(atributos[AtributosParsers::PROTEGIDO]==AtributosParsers::VERDADEIRO);

   //Passa para o próximo elemento que provavelmente será um <role> ou <tablespace>
   if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
   {
    do
    {
     if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
     {
      nome_elem=ParserXML::obterNomeElemento();

      /* Caso o nome do elemento atual seja o que define uma permissão
         indica que o parser leu todos os objetos do modelo e o mesmo
         se encontra no trecho final do arquivo onde são armazenadas
         as permissões sobre tais objetos. A obtenção das permissões
         só serão feitas caso não hajam objetos para serem reavaliados,
         caso contrário, os objetos são reavalias e logo apos as permissões
         obtidas. */
      if(nome_elem==AtributosParsers::PERMISSAO && objetos_incomp.size()==0)
      {
       /* Caso a lista de objetos especiais esteja com elementos
          efetua a recriação dos mesmos. Obs: Este processo é executado
          apenas 1 vez caso a lista de objetos especiais contenha algum item
          independente do número de permissões a serem criadas pois esta lista
          é preenchida durante o processo de criação dos objetos e não na criação
          das permissões. */
       if(!xml_objs_especiais.empty())
       {
        /* Efetua a cópia do buffer atual do parser pois
           a restauração de objetos especiais apaga o buffer
           recente do parser para serem usados os códigos xml
           de tais objetos */
        str_aux=ParserXML::obterBufferXML();

        itr=xml_objs_especiais.begin();
        itr_end=xml_objs_especiais.end();

        while(itr!=itr_end)
        {
         /* Cria o objeto especial a partir do iterador atual
            sendo que o elemento 'itr->second' possui a definição xml
            do mesmo e o elemento 'itr->first' possui o id do objeto
            especial a ser recriado */
         criarObjetoEspecial(itr->second, itr->first);
         itr++;
        }

        /* Limpa a lista de objetos especiais para evitar que os mesmos
           tentem ser recriados posteriormente */
        xml_objs_especiais.clear();

        /* Após a restauração dos objetos especiais é necessário recarregar
          o buffer do parser com o buffer usando anteriormente à restauração
          deos objetos especiais para que as permissões possam ser criadas
          a partir do XML */
        ParserXML::reiniciarParser();
        ParserXML::carregarBufferXML(str_aux);
        ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);

        /* Executa a navegação sobre os elementos até que o primeiro elemento
           que define uma permissão seja localizado */
        while(ParserXML::obterNomeElemento()!=AtributosParsers::PERMISSAO &&
              ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
       }

       //Executa acriação da permissão atual a partir do xml
       adicionarPermissao(criarPermissao());
      }
      else
      {
       //Obtém o tipo do elemento de acordo com a tag atual que o parser carregou
       tipo_obj=obterTipoObjeto(nome_elem);

       /* Este bloco faz a reavaliação de objetos. A reavaliação consiste em
          ler um trecho do arquivo XML novamente onde algum objeto foi referenciado
          porém ainda não criado causando o disparo de exceções reclamando de
          objetos com definição SQL incompleta.

          Condição para reavaliação dos objetos:
           * A flag de reavaliação (reaval_objetos) não esteja marcada indicando
             que uma reavaliação não está ocorrendo
           * A lista de objetos incompletos possua pelo menos 1 item, indicando que
             em algum trecho do XML existe um objeto o qual referencia outro que não
             foi encontrado (criado)
       */
       if(!reaval_objetos && objetos_incomp.size() > 0 &&
          tipo_obj!=OBJETO_FUNCAO && tipo_obj!=OBJETO_ESQUEMA &&
          tipo_obj!=OBJETO_ESPACO_TABELA && tipo_obj!=OBJETO_LINGUAGEM &&
          tipo_obj!=OBJETO_TIPO && tipo_obj!=OBJETO_TABELA)
       {
        //Faz o parser XML voltar ao elemento anterior
        ParserXML::acessarElemento(ParserXML::ELEMENTO_ANTERIOR);
        //Armazena a posição atual do parser
        elem_aux=ParserXML::obterElementoAtual();

        reaval_objetos=true;
        //Restaura o parser para a posição onde o objeto incompleto foi lido
        ParserXML::restaurarPosicao(objetos_incomp.front());
        //Obtém o tipo deste objeto
        tipo_obj=obterTipoObjeto(ParserXML::obterNomeElemento());
       }

       //Lendo um objeto banco de dados
       if(tipo_obj==OBJETO_BANCO_DADOS)
       {
        ParserXML::obterAtributosElemento(atributos);
        tipo_codif=atributos[AtributosParsers::CODIFICACAO];
        bd_modelo=atributos[AtributosParsers::BD_MODELO];
        localizacoes[0]=atributos[AtributosParsers::LC_CTYPE_BD];
        localizacoes[1]=atributos[AtributosParsers::LC_COLLATE_BD];

        if(!atributos[AtributosParsers::LIMITE_CONEXAO].isEmpty())
         lim_conexao=atributos[AtributosParsers::LIMITE_CONEXAO].toInt();

        definirAtributosBasicos(this);
       }
       else
       {
        try
        {
         /* Para os demais objetos, sempre a posição do parser XMl é salva
            antes da leitura e criação dos mesmos, para que a reavaliação
            seja possível, quando for necessária */
         ParserXML::salvarPosicao();
         //Cria o objeto de acordo com  o tipo obtido
         objeto=criarObjeto(tipo_obj);

         //Caso o objeto foi criado com sucesso
         if(objeto)
         {
          if(!dynamic_cast<ObjetoTabela *>(objeto) &&
             tipo_obj!=OBJETO_RELACAO && tipo_obj!=OBJETO_RELACAO_BASE)
          {
           //Usa o método de inserção de objetos genéricos
           adicionarObjeto(objeto);
          }

          //Dispara um sinal com o progresso (aproximado) do carregamento de objetos
          if(!signalsBlocked())
          {
           emit s_objetoCarregado(ParserXML::obterLinhaAtualBuffer()/ParserXML::obterNumLinhasBuffer(),
                                  trUtf8("Carregando objeto: %1 (%2)")
                                   .arg(QString::fromUtf8(objeto->obterNome()))
                                   .arg(objeto->obterNomeTipoObjeto()),
                                  tipo_obj);
          }
         }

         //Restaura a posição do parser para o elemento anterior ao atual
         ParserXML::restaurarPosicao();
        }
        catch(Excecao &e)
        {
         if(!reaval_objetos &&
            (((e.obterTipoErro()==ERR_PGMODELER_REFOBJINEXISTE && tipo_obj==OBJETO_TABELA)) ||
             (((e.obterTipoErro()==ERR_PGMODELER_ATROBJDEFSQLINV ||
                e.obterTipoErro()==ERR_PGMODELER_REFOBJINEXISTE ||
                e.obterTipoErro()==ERR_PGMODELER_ATRTIPOINVOBJ) &&
               (tipo_obj==OBJETO_LINGUAGEM || tipo_obj==OBJETO_FUNCAO || tipo_obj==OBJETO_TIPO || tipo_obj==OBJETO_OPERADOR)))))
         {
          /* Adiciona o nó da arvore o qual possui o elemento incompleto
             para que o mesmo tente ser recriado após suas possíveis dependências
             serem carregadas */
          ParserXML::restaurarPosicao();
          objetos_incomp.push_back(ParserXML::obterElementoAtual());
          //Armazena a exceção capturada para verificações ao final do carregamento do modelo
          vet_erros.push_back(e);
         }
         else
         {
          QString info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo()).arg(ParserXML::obterElementoAtual()->line);
          throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
         }
        }
       }
      }

      /* Caso esteja num processo de reavaliação de objetos, o bloco abaixo
         faz a retirada do objeto que acaba de ser reavaliado da lista de
         objetos incompletos */
      if(reaval_objetos && objetos_incomp.size() > 0)
      {
       //Remove o objeto incompleto da lista
       objetos_incomp.pop_front();

       //Remove a exceção relacionada ao objeto incompleto
       vet_erros.pop_front();

       /* Caso a lista ainda possua elementos o parser será voltado para o
          elemento incompleto anterior ao atual */
       if(objetos_incomp.size() > 0)
        ParserXML::restaurarPosicao(objetos_incomp.front());
       else
       {
        //Caso a lista esteja vazia o processo de reavaliação é interrompido
        reaval_objetos=false;
        //O parser é retornado à posição em que se encontrava antes da reavaliação
        ParserXML::restaurarPosicao(elem_aux);
       }
      }
     }
    }
    while((!reaval_objetos && ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR)) ||
          (reaval_objetos));
   }

   /* Verificação final, caso hajam erros pendentes na lista de erros capturados durante o processo
      de leitura do arquivo os mesmo serão redirecionados e o processo de leitura do arquivo
      cancelado */
   if(vet_erros.size() > 0)
    throw Excecao(vet_erros[0].obterMensagemErro(),vet_erros[0].obterTipoErro(), __PRETTY_FUNCTION__,__FILE__,__LINE__);

    //Protege o modelo com base no atributo obtido do xml
   this->ObjetoBase::definirProtegido(modelo_protegido);
   carregando_modelo=false;

   /* Faz uma última validação nos relacionamentos para abranger qualquer modificação de colunas
     não feitas durante o carregamento */
   this->validarRelacionamentos();
  }
  catch(Excecao &e)
  {
   QString info_adicional;
   carregando_modelo=false;
   destruirObjetos();

   if(ParserXML::obterElementoAtual())
    info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo()).arg(ParserXML::obterElementoAtual()->line);

   //Caso o erro seja na biblioteca de parsers
   if(e.obterTipoErro()>=ERR_PARSERS_SINTAXEINV)
   {
    str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_ARQMODELOINV)).arg(nome_arq);
    throw Excecao(str_aux,ERR_PARSERS_ARQMODELOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
   }
   else
    //Captura e redireciona erros das demais bibliotecas
    throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
  }
 }
}
//-----------------------------------------------------------
TipoObjetoBase ModeloBD::obterTipoObjeto(const QString &str_tipo)
{
 TipoObjetoBase tipo_obj=OBJETO_BASE;
 int i;

 for(i=0; i < ObjetoBase::QTD_TIPOS_OBJETO; i++)
 {
  if(esq_objetos[i]==str_tipo)
  {
   tipo_obj=static_cast<TipoObjetoBase>(i);
   break;
  }
 }

 return(tipo_obj);
}
//-----------------------------------------------------------
ObjetoBase *ModeloBD::criarObjeto(TipoObjetoBase tipo_obj)
{
 ObjetoBase *objeto=NULL;

 if(tipo_obj!=OBJETO_BASE)
 {
  if(tipo_obj==OBJETO_PAPEL)
   objeto=criarPapel();
  else if(tipo_obj==OBJETO_ESPACO_TABELA)
   objeto=criarEspacoTabela();
  else if(tipo_obj==OBJETO_ESQUEMA)
   objeto=criarEsquema();
  else if(tipo_obj==OBJETO_LINGUAGEM)
   objeto=criarLinguagem();
  else if(tipo_obj==OBJETO_FUNCAO)
   objeto=criarFuncao();
  else if(tipo_obj==OBJETO_TIPO)
   objeto=criarTipo();
  else if(tipo_obj==OBJETO_DOMINIO)
   objeto=criarDominio();
  else if(tipo_obj==OBJETO_CONV_TIPO)
   objeto=criarConversaoTipo();
  else if(tipo_obj==OBJETO_CONV_CODIFICACAO)
   objeto=criarConversaoCodificacao();
  else if(tipo_obj==OBJETO_OPERADOR)
   objeto=criarOperador();
  else if(tipo_obj==OBJETO_FAMILIA_OPER)
   objeto=criarFamiliaOperadores();
  else if(tipo_obj==OBJETO_CLASSE_OPER)
   objeto=criarClasseOperadores();
  else if(tipo_obj==OBJETO_FUNC_AGREGACAO)
   objeto=criarFuncaoAgregacao();
  else if(tipo_obj==OBJETO_TABELA)
   objeto=criarTabela();
  else if(tipo_obj==OBJETO_SEQUENCIA)
   objeto=criarSequencia();
  else if(tipo_obj==OBJETO_VISAO)
   objeto=criarVisao();
  else if(tipo_obj==OBJETO_CAIXA_TEXTO)
   objeto=criarCaixaTexto();
  else if(tipo_obj==OBJETO_RESTRICAO)
   objeto=criarRestricao(NULL);
  else if(tipo_obj==OBJETO_GATILHO)
   objeto=criarGatilho(NULL);
  else if(tipo_obj==OBJETO_INDICE)
   objeto=criarIndice(NULL);
  else if(tipo_obj==OBJETO_RELACAO ||
          tipo_obj==OBJETO_RELACAO_BASE)
   objeto=criarRelacionamento();
 }

 return(objeto);
}
//-----------------------------------------------------------
void ModeloBD::definirAtributosBasicos(ObjetoBase *objeto)
{
 map<QString, QString> atributos, atribs_aux;
 QString nome_elem;//, str_aux;
 ObjetoBase *esp_tabela=NULL, *dono=NULL;
 Esquema *esquema=NULL;
 TipoObjetoBase tipo_obj=OBJETO_BASE, tipo_obj_aux;
 bool erro=false, protegido=false;

 //Caso o objeto não esteja alocado uma exceção é disparada
 if(!objeto)
  throw Excecao(ERR_PGMODELER_OPROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém os atributos do elemento
 ParserXML::obterAtributosElemento(atributos);

 tipo_obj_aux=objeto->obterTipoObjeto();
 if(tipo_obj_aux!=OBJETO_CONV_TIPO)
  objeto->definirNome(atributos[AtributosParsers::NOME]);

 //Definindo se o objeto está protegido ou não
 protegido=atributos[AtributosParsers::PROTEGIDO]==AtributosParsers::VERDADEIRO;

 ParserXML::salvarPosicao();

 //Passa para os elementos filhos que provavelmente serão <onwer>, <tablespace> e <comment>
 if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
 {
  do
  {
   /* Certificando que só elementos xml serão lidos do parser, qualquer outro
      tipo de objeto xml será ignorado */
   if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
   {
    //Obtém o nome do elemento filho
    nome_elem=ParserXML::obterNomeElemento();

    //Caso o elemento filho seja um comentáio <comment>
    if(nome_elem==AtributosParsers::COMENTARIO)
    {
     /* Para se extraír o comentário, é necessário salvar a posição de navegação
        do parser, pois o conteúdo do comentário é um elemento filho do elemento
        atual do parser XML */
     ParserXML::salvarPosicao();
     //Acessa o elemento filho o qual contém o conteúdo do comentário
     ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
     //Atributo o conteúdo do elemento filho ao atributo cometário do novo papel
     objeto->definirComentario(ParserXML::obterConteudoElemento());
     //Restaura a posição de navegação do parser, ou seja, volta para o elemento <comment>
     ParserXML::restaurarPosicao();
    }
    //Caso o elemento filho seja uma referência a um esquema <schema>
    else if(nome_elem==AtributosParsers::ESQUEMA)
    {
     tipo_obj=OBJETO_ESQUEMA;
     ParserXML::obterAtributosElemento(atribs_aux);
     esquema=dynamic_cast<Esquema *>(obterObjeto(atribs_aux[AtributosParsers::NOME], tipo_obj));
     objeto->definirEsquema(esquema);
     erro=(!esquema && !atribs_aux[AtributosParsers::NOME].isEmpty());
    }

    //Caso o elemento filho seja uma referência a um tablespace <tablespace>
    else if(nome_elem==AtributosParsers::ESPACOTABELA)
    {
     tipo_obj=OBJETO_ESPACO_TABELA;
     ParserXML::obterAtributosElemento(atribs_aux);
     esp_tabela=obterObjeto(atribs_aux[AtributosParsers::NOME], tipo_obj);
     objeto->definirEspacoTabela(esp_tabela);
     erro=(!esp_tabela && !atribs_aux[AtributosParsers::NOME].isEmpty());
    }

    //Caso o elemento filho seja uma referência a um dono (role/papel) <role>
    else if(nome_elem==AtributosParsers::PAPEL)
    {
     tipo_obj=OBJETO_PAPEL;
     ParserXML::obterAtributosElemento(atribs_aux);
     dono=obterObjeto(atribs_aux[AtributosParsers::NOME], tipo_obj);
     objeto->definirDono(dono);
     erro=(!dono && !atribs_aux[AtributosParsers::NOME].isEmpty());
    }
    //Obténdo o atributo a posição do objeto (apenas para objetos gráficos)
    else if(nome_elem==AtributosParsers::POSICAO)
    {
     ParserXML::obterAtributosElemento(atributos);

     if(nome_elem==AtributosParsers::POSICAO &&
        (tipo_obj_aux!=OBJETO_RELACAO &&
         tipo_obj_aux!=OBJETO_RELACAO_BASE))
     {
      dynamic_cast<ObjetoGraficoBase *>(objeto)->definirPosicaoObjeto(QPointF(atributos[AtributosParsers::POSICAO_X].toFloat(),
                                                                       atributos[AtributosParsers::POSICAO_Y].toFloat()));


     }
    }
   }
  }
  /* A extração de elementos continuará até que se chegue no último elemento
     filho do elemento */
  while(!erro && ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
 }

 ParserXML::restaurarPosicao();
 objeto->definirProtegido(protegido);

 if(erro)
 {
  /* Caso o flag de erro esteja marcado indica que o novo objeto
     está referenciando um outro objeto o qual não existe no modelo */

  //Dispara a exceção
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                   .arg(objeto->obterNome())
                   .arg(ObjetoBase::nome_tipo_objetos[objeto->obterTipoObjeto()])
                   .arg(atribs_aux[AtributosParsers::NOME])
                   .arg(ObjetoBase::nome_tipo_objetos[tipo_obj]),
                ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 //Caso o objeto não esteja atribuído a nenhum esquema gera um erro
 else if(!objeto->obterEsquema() &&
         (tipo_obj_aux==OBJETO_FUNCAO || tipo_obj_aux==OBJETO_TABELA ||
          tipo_obj_aux==OBJETO_VISAO  || tipo_obj_aux==OBJETO_DOMINIO ||
          tipo_obj_aux==OBJETO_FUNC_AGREGACAO || tipo_obj_aux==OBJETO_OPERADOR ||
          tipo_obj_aux==OBJETO_SEQUENCIA || tipo_obj_aux==OBJETO_CONV_CODIFICACAO ||
          tipo_obj_aux==OBJETO_TIPO || tipo_obj_aux==OBJETO_FAMILIA_OPER ||
          tipo_obj_aux==OBJETO_CLASSE_OPER))
 {
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_OBJSEMESQUEMA)
                        .arg(objeto->obterNome())
                        .arg(objeto->obterNomeTipoObjeto()),
          ERR_PGMODELER_OBJSEMESQUEMA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}
//-----------------------------------------------------------
Papel *ModeloBD::criarPapel(void)
{
 map<QString, QString> atributos, atribs_aux;
 Papel *papel=NULL, *papel_ref=NULL;
 int i, tam;
 bool marcado;
 QStringList lista;
 QString nome_elem;
 unsigned tipo_papel;

 /* Os elementos dos dois elementos a seguir deve aparecer na
    mesma ordem pois seus valores serão trabalhados em conjunto
    na interação onde ser precisa identificar as opções do papel
    o qual está sendo criado */
 QString atrib_ops[]={ AtributosParsers::SUPER_USUARIO, AtributosParsers::CRIAR_BD,
                       AtributosParsers::CRIAR_PAPEL, AtributosParsers::HERDA_ATRIBUTOS,
                       AtributosParsers::LOGIN, AtributosParsers::ENCRIPTADA };

 unsigned vet_ops[]={ Papel::OP_SUPERUSER, Papel::OP_CREATEDB,
                      Papel::OP_CREATEROLE, Papel::OP_INHERIT,
                      Papel::OP_LOGIN, Papel::OP_ENCRYPTED };

 try
 {
  //Aloca no novo papel
  papel=new Papel;
  definirAtributosBasicos(papel);

  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);

  //Definindo os valores de atributos básicos do papel
  papel->definirSenha(atributos[AtributosParsers::SENHA]);
  papel->definirValidade(atributos[AtributosParsers::VALIDADE]);

  /* Caso o atributo de id de usuário esteja atribuído no xml.
     (atributos[AtributosParsers::UID] != "").
     Atribui ao papel o valor do atributo convertido para inteiro. */
  if(!atributos[AtributosParsers::SYSID].isEmpty())
   papel->definirSysid(atributos[AtributosParsers::SYSID].toInt());

  /* Caso o atributo de limite de conexão esteja atribuído no xml.
     (atributos[AtributosParsers::LIMITE_CONEXAO] != "").
     Atribui ao papel o valor do atributo convertido para inteiro. */
  if(!atributos[AtributosParsers::LIMITE_CONEXAO].isEmpty())
   papel->definirLimiteConexao(atributos[AtributosParsers::LIMITE_CONEXAO].toInt());

  /* Identificando as opções do papel. Caso o atributo referente à uma
     estive com valor "true" no documento XML quer dizer que aquele
     atributo está marcado para o papel */
  for(i=0; i < 6; i++)
  {
   //Verifica se a opção está marcada no XML, valor de atributo = true
   marcado=atributos[atrib_ops[i]]==AtributosParsers::VERDADEIRO;
   papel->definirOpcao(vet_ops[i], marcado);
  }

  //Passa para os elementos filhos que provavelmente serão <roles> e <comment>
  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser, qualquer outro
       tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     //Obtém o nome do elemento filho
     nome_elem=ParserXML::obterNomeElemento();

     //Caso o elemento filho seja uma lista de papeis <roles>
     if(nome_elem==AtributosParsers::PAPEIS)
     {
      //Obtém os atributos do elemento <roles>, neste caso são names e reftype
      ParserXML::obterAtributosElemento(atribs_aux);

      /* O atributo names armazena uma lista de nomes de papéis as quais o novo papel
         referenciará. A lista tem os elementos separados por vírgula, sendo assim a
         string será quebrada usando o delimitador ',') */
      lista=atribs_aux[AtributosParsers::NOMES].split(',');

      //Obtém a quantidade de nomes de papéis na lista
      tam=lista.size();

      /* Identificando o tipo da lista de papéis a qual será inserido os objetos
         cujos nomes foram extraídos acima */
      if(atribs_aux[AtributosParsers::TIPO_PAPEL]==AtributosParsers::REFER)
       tipo_papel=Papel::PAPEL_REF;
      else if(atribs_aux[AtributosParsers::TIPO_PAPEL]==AtributosParsers::MEMBRO)
       tipo_papel=Papel::PAPEL_MEMBRO;
      else
       tipo_papel=Papel::PAPEL_ADMIN;

      //Varre a lista de nomes de papéis
      for(i=0; i < tam; i++)
      {
       //Tenta obter um papel do modelo cujo nome é o elemento atual da lista de nomes (lista[i])
       papel_ref=dynamic_cast<Papel *>(obterObjeto(lista[i].trimmed(),OBJETO_PAPEL));

       /* Caso esse papel não exista um erro será disparado pois um novo papel
          não pode referenciar um outro papel que ainda nem foi criado */
       if(!papel_ref)
       {
        //Dispara a exceção
        throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                               .arg(QString::fromUtf8(papel->obterNome()))
                               .arg(QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(OBJETO_PAPEL)))
                               .arg(QString::fromUtf8(lista[i]))
                               .arg(QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(OBJETO_PAPEL))),
                      ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }
       //Caso o papel exista no modelo, o mesmo será relacionado ao novo papel
       papel->definirPapel(tipo_papel, papel_ref);
      }
     }
    }
   }
   /* A extração de elementos continuará até que se chegue no último elemento
      filho do elemento <role> */
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  //Remove o papel alocado
  if(papel) delete(papel);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 //Retorna o novo papel criado
 return(papel);
}
//-----------------------------------------------------------
EspacoTabela *ModeloBD::criarEspacoTabela(void)
{
 map<QString, QString> atributos;
 EspacoTabela *esp_tabela=NULL;

 try
 {
  //Aloca no novo espaço de tabelas
  esp_tabela=new EspacoTabela;
  definirAtributosBasicos(esp_tabela);

  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);

  //Definindo os valores de atributos básicos do papel
  esp_tabela->definirDiretorio(atributos[AtributosParsers::DIRETORIO]);
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  //Remove o espaco de tabela alocado
  if(esp_tabela) delete(esp_tabela);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(esp_tabela);
}
//-----------------------------------------------------------
Esquema *ModeloBD::criarEsquema(void)
{
 map<QString, QString> atributos;
 Esquema *esquema=NULL;

 try
 {
  //Aloca no novo esquema
  esquema=new Esquema;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(esquema);
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  //Remove o espaco de tabela alocado
  if(esquema) delete(esquema);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(esquema);
}
//-----------------------------------------------------------
Linguagem *ModeloBD::criarLinguagem(void)
{
 map<QString, QString> atributos;
 Linguagem *linguagem=NULL;
 ObjetoBase *funcao=NULL;
 QString assinatura, tipo_ref;
 TipoObjetoBase tipo_obj;

 try
 {
  linguagem=new Linguagem;

  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);
  definirAtributosBasicos(linguagem);

  linguagem->definirConfiavel(atributos[AtributosParsers::CONFIAVEL]==
                               AtributosParsers::VERDADEIRO);

   if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
   {
    do
    {
     /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
     if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
     {
      tipo_obj=obterTipoObjeto(ParserXML::obterNomeElemento());

      if(tipo_obj==OBJETO_FUNCAO)
      {
       ParserXML::obterAtributosElemento(atributos);
       //Obtém o tipo de referência da função
       tipo_ref=atributos[AtributosParsers::TIPO_REFERENCIA];

       //Caso seja uma função handler ou validator
       if(tipo_ref==AtributosParsers::FUNCAO_VALIDATOR ||
          tipo_ref==AtributosParsers::FUNCAO_HANDLER ||
          tipo_ref==AtributosParsers::FUNCAO_INLINE)
       {
        //Obtém a assinatura da função
        assinatura=atributos[AtributosParsers::ASSINATURA];

        //Obtém a função do modelo
        funcao=obterObjeto(assinatura, OBJETO_FUNCAO);

        //Caso a função não seja encontrada
        if(!funcao)
         //Dispara a exceção indicando que o objeto está incompleto
         throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                               .arg(linguagem->obterNome())
                               .arg(linguagem->obterNomeTipoObjeto())
                               .arg(assinatura)
                               .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_FUNCAO)),
                       ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

        if(tipo_ref==AtributosParsers::FUNCAO_VALIDATOR)

         linguagem->definirFuncao(dynamic_cast<Funcao *>(funcao), Linguagem::FUNC_VALIDATOR);
        else if(tipo_ref==AtributosParsers::FUNCAO_HANDLER)
         linguagem->definirFuncao(dynamic_cast<Funcao *>(funcao), Linguagem::FUNC_HANDLER);
        else
         linguagem->definirFuncao(dynamic_cast<Funcao *>(funcao), Linguagem::FUNC_INLINE);

       }
       else
        //Dispara uma exceção caso o tipo de referencia a função seja inválido
        throw Excecao(ERR_PGMODELER_REFFUNCTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }
     }
    }
    while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
   }

 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  //Remove a linguagem alocada
  if(linguagem) delete(linguagem);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(linguagem);
}
//-----------------------------------------------------------
Funcao *ModeloBD::criarFuncao(void)
{
 map<QString, QString> atributos, atrib_aux;
 Funcao *funcao=NULL;
 TipoObjetoBase tipo_obj;
 ObjetoBase *objeto=NULL;
 TipoPgSQL tipo;
 Parametro param;
 QString str_aux, elem;

 try
 {
  funcao=new Funcao;
  //Lê do parser os atributos basicos
  definirAtributosBasicos(funcao);

  //Obtém os atributos da função
  ParserXML::obterAtributosElemento(atributos);

  //Define se a função retorna setof, caso o atributo esteja marcado no XML
  if(!atributos[AtributosParsers::RETORNA_SETOF].isEmpty())
   funcao->definirRetornaSetOf(atributos[AtributosParsers::RETORNA_SETOF]==
                               AtributosParsers::VERDADEIRO);

  //Define se a função é do tipo janela, caso o atributo esteja marcado no XML
  if(!atributos[AtributosParsers::FUNCAO_JANELA].isEmpty())
   funcao->definirFuncaoJanela(atributos[AtributosParsers::FUNCAO_JANELA]==
                               AtributosParsers::VERDADEIRO);

  //Define a configuração de retorno da função, caso o atributo esteja marcado no XML
  if(!atributos[AtributosParsers::TIPO_COMPORTAMENTO].isEmpty())
   funcao->definirTipoComportamento(TipoComportamento(atributos[AtributosParsers::TIPO_COMPORTAMENTO]));

  //Define o tipo da função, caso o atributo esteja marcado no XML
  if(!atributos[AtributosParsers::TIPO_FUNCAO].isEmpty())
   funcao->definirTipoFuncao(TipoFuncao(atributos[AtributosParsers::TIPO_FUNCAO]));

  //Define o tipo de segurança da função, caso o atributo esteja marcado no XML
  if(!atributos[AtributosParsers::TIPO_SEGURANCA].isEmpty())
   funcao->definirTipoSeguranca(TipoSeguranca(atributos[AtributosParsers::TIPO_SEGURANCA]));

  //Define o custo de execução da função, caso o atributo esteja marcado no XML
  if(!atributos[AtributosParsers::CUSTO_EXECUCAO].isEmpty())
   funcao->definirCustoExecucao(atributos[AtributosParsers::CUSTO_EXECUCAO].toInt());

  //Define a quantidade de linhas retornadas pela função, caso o atributo esteja marcado no XML
  if(!atributos[AtributosParsers::QTD_LINHAS].isEmpty())
   funcao->definirQuantidadeLinhas(atributos[AtributosParsers::QTD_LINHAS].toInt());

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();
     tipo_obj=obterTipoObjeto(elem);

     //Caso o parser acesso a tag que determina o tipo de retorno da função
     if(elem==AtributosParsers::TIPO_RETORNO)
     {
      ParserXML::salvarPosicao();

      try
      {
       //Acesso os elementos filhos os quais podem ser um <type> ou vários <parameter>
       ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);

       do
       {
        if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
        {
         /* Caso o elemento atual no parser seja um <type>, indica que
          será extraído o tipo de retorno da função */
         if(ParserXML::obterNomeElemento()==AtributosParsers::TIPO)
         {
          //Cria o tipo
          tipo=criarTipoPgSQL();
          //Atribui ao retorno da função
          funcao->definirTipoRetorno(tipo);
         }
         /* Criação dos tipo de retorno de tabela da função. Os mesmos vem descritos
            dentro da tag <return-type> em forma de parâmetros */
         else if(ParserXML::obterNomeElemento()==AtributosParsers::PARAMETRO)
         {
          param=criarParametro();
          //Adiciona o tipo de retorno à função
          funcao->adicionarTipoRetTabela(param.obterNome(), param.obterTipo());
         }
        }
       }
       while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));

       ParserXML::restaurarPosicao();
      }
      catch(Excecao &e)
      {
       ParserXML::restaurarPosicao();
       throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
      }
     }
     //Obtendo a linguagem da função
     else if(tipo_obj==OBJETO_LINGUAGEM)
     {
      //Obtém seus atributos
      ParserXML::obterAtributosElemento(atributos);

      //Busca a linguagem no modelo
      objeto=obterObjeto(atributos[AtributosParsers::NOME], tipo_obj);

      //Caso a linguagem não existe será disparada uma exceção
      if(!objeto)
       throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                              .arg(funcao->obterNome())
                              .arg(funcao->obterNomeTipoObjeto())
                              .arg(atributos[AtributosParsers::NOME])
                              .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_LINGUAGEM)),
                      ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Define a linguagem da função
      funcao->definirLinguagem(dynamic_cast<Linguagem *>(objeto));
     }
     else if(ParserXML::obterNomeElemento()==AtributosParsers::PARAMETRO)
     {
      param=criarParametro();
      //Adiciona o parâmeto à função
      funcao->adicionarParametro(param);
     }
     //Extraíndo a definição (corpo) da função (tag <definition>)
     else if(ParserXML::obterNomeElemento()==AtributosParsers::DEFINICAO)
     {
      ParserXML::salvarPosicao();

      //Obtém os atributos da biblioteca
      ParserXML::obterAtributosElemento(atrib_aux);

      if(!atrib_aux[AtributosParsers::BIBLIOTECA].isEmpty())
      {
       funcao->definirBiblioteca(atrib_aux[AtributosParsers::BIBLIOTECA]);
       funcao->definirSimbolo(atrib_aux[AtributosParsers::SIMBOLO]);
      }
      /* Para se ter acesso ao código que define a função é preciso acessar
         o filho da tag <definition> e obter seu conteúdo */
      else if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
       funcao->definirCodigoFonte(ParserXML::obterConteudoElemento());

      ParserXML::restaurarPosicao();
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  //Remove o espaco de tabela alocado
  if(funcao)
  {
   str_aux=funcao->obterNome(true);
   delete(funcao);
  }

  //Redireciona qualquer exceção capturada
  if(e.obterTipoErro()==ERR_PGMODELER_REFTIPOUSRINV)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATROBJDEFSQLINV)
                              .arg(str_aux)
                              .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_FUNCAO)),
                 ERR_PGMODELER_ATROBJDEFSQLINV,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, info_adicional);
  else
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(funcao);
}
//-----------------------------------------------------------
Parametro ModeloBD::criarParametro(void)
{
 TipoPgSQL tipo;
 Parametro param;
 map<QString, QString> atributos;
 QString elem;

 try
 {
  //Salva a posição do parser
  ParserXML::salvarPosicao();
  //Obtem os atributos do parâmetro (nome, in e out)
  ParserXML::obterAtributosElemento(atributos);

  param.definirNome(atributos[AtributosParsers::NOME]);
  /* Configurando atributos in e out do parâmetro caso estes estejam
     definidos como true no XML */
  param.definirEntrada(atributos[AtributosParsers::PARAM_ENTRADA]==AtributosParsers::VERDADEIRO);
  param.definirSaida(atributos[AtributosParsers::PARAM_SAIDA]==AtributosParsers::VERDADEIRO);
  param.definirValorPadrao(atributos[AtributosParsers::VALOR_PADRAO]);

  //Acessa os elementos filhos do parâmetro, que no caso será apenas <type> ou <domain>
  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==AtributosParsers::TIPO)
     {
      param.definirTipo(criarTipoPgSQL());
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }

  //Restaura a posição do parser
  ParserXML::restaurarPosicao();
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  //Restaura a posição do parser
  ParserXML::restaurarPosicao();
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(param);
}
//-----------------------------------------------------------
TipoPgSQL ModeloBD::criarTipoPgSQL(void)
{
 map<QString, QString> atributos;
 vector<void *> vet_ptipos;
 vector<void *>::iterator itr, itr_end;
 unsigned comprimento=1, dimensao=0, idx_tipo=0;
 int precisao=-1;
 QString nome;
 Tipo *tipo_usr=NULL;
 void *ptipo=NULL;
 TipoPgSQL tipo;
 bool enc=false, com_timezone;
 TipoIntervalo tipo_interv;


 //Obtém os atributos do tipo
 ParserXML::obterAtributosElemento(atributos);

 if(!atributos[AtributosParsers::COMPRIMENTO].isEmpty())
  comprimento=atributos[AtributosParsers::COMPRIMENTO].toUInt();

 if(!atributos[AtributosParsers::DIMENSAO].isEmpty())
  dimensao=atributos[AtributosParsers::DIMENSAO].toUInt();

 if(!atributos[AtributosParsers::PRECISAO].isEmpty())
  precisao=atributos[AtributosParsers::PRECISAO].toInt();

 com_timezone=(atributos[AtributosParsers::COM_TIMEZONE]==AtributosParsers::VERDADEIRO);
 tipo_interv=atributos[AtributosParsers::TIPO_INTERVALO];

 nome=atributos[AtributosParsers::NOME];

 idx_tipo=TipoPgSQL::obterIndiceTipoBase(nome);
 if(idx_tipo!=TipoPgSQL::nulo)
 {
  return(TipoPgSQL(nome,comprimento,dimensao,precisao,com_timezone,tipo_interv));
 }
 else
 {
  //Obtém a lista de tipos definidios pelo usuario
  TipoPgSQL::obterTiposUsuario(vet_ptipos, this);
  itr=vet_ptipos.begin();
  itr_end=vet_ptipos.end();

  //Faz uma varredura na lista de tipos
  while(itr!=itr_end && !enc)
  {
   //Converte o elemento atual para a classe Tipo
   ptipo=(*itr);
   tipo_usr=reinterpret_cast<Tipo *>(ptipo);

   //Caso o nome passado no XML for igual ao nome do objeto Tipo atual
   enc=(tipo_usr->obterNome(true)==nome);
   itr++;
  }

  /* Caso o tipo não foi encontrado na lista de tipos quer dizer existe
     a tentativa de se referenciar um tipo definido pelo usuário sem que o
     mesmo exista no modelo, sendo assim, um erro será disparado e
     a criação do tipo será abortada */
  if(!enc)
    throw Excecao(ERR_PGMODELER_REFTIPOUSRINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  idx_tipo=TipoPgSQL::obterIndiceTipoUsuario(nome, ptipo);
  return(TipoPgSQL(idx_tipo,comprimento,dimensao,precisao,com_timezone,tipo_interv));
 }
}
//-----------------------------------------------------------
Tipo *ModeloBD::criarTipo(void)
{
 map<QString, QString> atributos;
 map<QString, unsigned> tipo_funcoes;
 Tipo *tipo=NULL;
 int qtd, i;
 QStringList enums;
 QString elem, str_aux;
 Parametro param;
 ObjetoBase *funcao=NULL;
 unsigned tipo_func=0;
 TipoPgSQL tipo_copia;

 try
 {
  tipo=new Tipo;
  //Lê do parser os atributos basicos
  definirAtributosBasicos(tipo);

  //Obtém os atributos do tipo
  ParserXML::obterAtributosElemento(atributos);

  //Define a configuração do tipo
  if(atributos[AtributosParsers::CONFIGURACAO]==AtributosParsers::TIPO_BASE)
  {
   tipo->definirConfiguracao(Tipo::TIPO_BASE);

   //Definindos os atributos específicos para tipo base

   //Definindo se o tipo é passado por valor ou não
   tipo->definirPorValor(atributos[AtributosParsers::POR_VALOR]==AtributosParsers::VERDADEIRO);

   //Definindo o comprimento interno do tipo caso esteja especificado no XML
   if(!atributos[AtributosParsers::COMP_INTERNO].isEmpty())
    tipo->definirCompInterno(atributos[AtributosParsers::COMP_INTERNO].toUInt());

   //Definindo o alinhamento interno do tipo caso esteja especificado no XML
   if(!atributos[AtributosParsers::ALINHAMENTO].isEmpty())
    tipo->definirAlinhamento(atributos[AtributosParsers::ALINHAMENTO]);

   //Definindo o tipo de armazenamento do tipo caso esteja especificado no XML
   if(!atributos[AtributosParsers::ARMAZENAMENTO].isEmpty())
    tipo->definirArmazenamento(atributos[AtributosParsers::ARMAZENAMENTO]);

   //Definindo o elemento do tipo caso esteja especificado no XML
   if(!atributos[AtributosParsers::ELEMENTO].isEmpty())
    tipo->definirElemento(atributos[AtributosParsers::ELEMENTO]);

   //Definindo o delimitador do tipo caso esteja especificado no XML
   if(!atributos[AtributosParsers::DELIMITADOR].isEmpty())
    tipo->definirDelimitador(atributos[AtributosParsers::DELIMITADOR][0].toAscii());

   //Definindo o valor padrão do tipo caso esteja especificado no XML
   if(!atributos[AtributosParsers::VALOR_PADRAO].isEmpty())
    tipo->definirValorPadrao(atributos[AtributosParsers::VALOR_PADRAO]);

   //Definindo a categoria do tipo caso esteja especificado no XML
   if(!atributos[AtributosParsers::CATEGORIA].isEmpty())
    tipo->definirCategoria(atributos[AtributosParsers::CATEGORIA]);

   //Definindo a categoria do tipo caso esteja especificado no XML
   if(!atributos[AtributosParsers::PREFERIDO].isEmpty())
    tipo->definirPreferido(atributos[AtributosParsers::PREFERIDO]==AtributosParsers::VERDADEIRO);


   /* O mapa de tipos de função abaixo é usado para se atribuir de forma
      mas simples, sem comparações, a função que for obtida do XML a qual
      o tipo em construção referencia */
   tipo_funcoes[AtributosParsers::FUNCAO_INPUT]=Tipo::FUNCAO_INPUT;
   tipo_funcoes[AtributosParsers::FUNCAO_OUTPUT]=Tipo::FUNCAO_OUTPUT;
   tipo_funcoes[AtributosParsers::FUNCAO_SEND]=Tipo::FUNCAO_SEND;
   tipo_funcoes[AtributosParsers::FUNCAO_RECV]=Tipo::FUNCAO_RECV;
   tipo_funcoes[AtributosParsers::FUNCAO_TPMOD_IN]=Tipo::FUNCAO_TPMOD_IN;
   tipo_funcoes[AtributosParsers::FUNCAO_TPMOD_OUT]=Tipo::FUNCAO_TPMOD_OUT;
   tipo_funcoes[AtributosParsers::FUNCAO_ANALYZE]=Tipo::FUNCAO_ANALYZE;
  }
  else if(atributos[AtributosParsers::CONFIGURACAO]==AtributosParsers::TIPO_COMPOSTO)
   tipo->definirConfiguracao(Tipo::TIPO_COMPOSTO);
  else
   tipo->definirConfiguracao(Tipo::TIPO_ENUMERACAO);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     //Operação específica para tipo ENUM
     if(elem==AtributosParsers::TIPO_ENUM)
     {
      //Obtém o atributo da tag <enumerations>
      ParserXML::obterAtributosElemento(atributos);
      /* Como se trata de uma lista de enumerações separadas por vírgulas
         a mesma será quebrada e transformada num vetor */
      enums=atributos[AtributosParsers::VALORES].split(",");

      //Adiciona ao tipo todas as enumerações presentes no vetor
      qtd=enums.size();
      for(i=0; i < qtd; i++)
       tipo->adicionarEnumeracao(enums[i]);
     }
     //Operação específica para tipo COMPOSTO
     else if(elem==AtributosParsers::PARAMETRO)
     {
      /* No caso de tipo composto, o mesmo possui indefinida quatidade
         de elementos <parameter> os quais simbolizam os atributos do
         tipo */
      param=criarParametro();
      tipo->adicionarAtributo(param);
     }
     //Operação específica para tipo BASE
     else if(elem==AtributosParsers::TIPO)
     {
      tipo_copia=criarTipoPgSQL();
      tipo->definirTipoCopia(tipo_copia);
     }
     else if(elem==AtributosParsers::FUNCAO)
     {
      /*No caso de tipo base, serão extraídas referência a funções do modelo,
        as quais serão atribuídas às funções que compoem o tipo base. */
      ParserXML::obterAtributosElemento(atributos);

      /* Com a assinatura da função obtida di XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[AtributosParsers::ASSINATURA], OBJETO_FUNCAO);

      /* Dispara uma exceção caso o tipo de referencia a função seja inválido ou
         se a função referenciada não existe */
      if(!funcao && !atributos[AtributosParsers::ASSINATURA].isEmpty())
       throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                              .arg(tipo->obterNome())
                              .arg(tipo->obterNomeTipoObjeto())
                              .arg(atributos[AtributosParsers::ASSINATURA])
                              .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_FUNCAO)),
                     ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      else if(tipo_funcoes.count(atributos[AtributosParsers::TIPO_REFERENCIA])==0)
       throw Excecao(ERR_PGMODELER_REFFUNCTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      tipo_func=tipo_funcoes[atributos[AtributosParsers::TIPO_REFERENCIA]];

      //Atribui a função ao tipo na configuração obtida
      tipo->definirFuncao(tipo_func, dynamic_cast<Funcao *>(funcao));
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  //Remove o tipo alocado
  if(tipo)
  {
   str_aux=tipo->obterNome(true);
   delete(tipo);
  }

  //Redireciona qualquer exceção capturada
  if(e.obterTipoErro()==ERR_PGMODELER_REFTIPOUSRINV)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATROBJDEFSQLINV)
                              .arg(str_aux)
                              .arg(tipo->obterNomeTipoObjeto()),
                 ERR_PGMODELER_ATROBJDEFSQLINV,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, info_adicional);
  else
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(tipo);
}
//-----------------------------------------------------------
Dominio *ModeloBD::criarDominio(void)
{
 map<QString, QString> atributos;
 Dominio *dominio=NULL;
 QString elem;

 try
 {
  dominio=new Dominio;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(dominio);

  //Obtém os atributos do domíno
  ParserXML::obterAtributosElemento(atributos);

  if(!atributos[AtributosParsers::RESTRICAO].isEmpty())
   dominio->definirNomeRestricao(atributos[AtributosParsers::RESTRICAO]);

  if(!atributos[AtributosParsers::VALOR_PADRAO].isEmpty())
   dominio->definirValorPadrao(atributos[AtributosParsers::VALOR_PADRAO]);

  dominio->definirNaoNulo(atributos[AtributosParsers::NAO_NULO]==
                            AtributosParsers::VERDADEIRO);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     /* Caso o elemento seja um <type>, será extraído do XML
        o tipo ao qual o domínio se aplica */
     if(elem==AtributosParsers::TIPO)
     {
      dominio->definirTipo(criarTipoPgSQL());
     }
     //Caso o elemento seja uma expressão
     else if(elem==AtributosParsers::EXPRESSAO)
     {
      /* Para se extraír a expressão, é necessário salvar a posição de navegação
         do parser, pois o conteúdo da mesma é um elemento filho do elemento
         atual do parser XML */
      ParserXML::salvarPosicao();
      //Acessa o elemento filho o qual contém o conteúdo da expressão
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
      //dominio->definirExpressao(QString::fromUtf8(ParserXML::obterConteudoElemento()));
      dominio->definirExpressao(ParserXML::obterConteudoElemento());
      //Restaura a posição de navegação do parser, ou seja, volta para o elemento <expression>
      ParserXML::restaurarPosicao();
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  //Remove o domínio alocado
  if(dominio) delete(dominio);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(dominio);
}
//-----------------------------------------------------------
ConversaoTipo *ModeloBD::criarConversaoTipo(void)
{
 map<QString, QString> atributos;
 ConversaoTipo *conv_tipo=NULL;
 QString elem;
 unsigned idx_tipo=0;
 TipoPgSQL tipo;
 ObjetoBase *funcao=NULL;

 try
 {
  conv_tipo=new ConversaoTipo;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(conv_tipo);

  //Obtém os atributos do domíno
  ParserXML::obterAtributosElemento(atributos);

  if(atributos[AtributosParsers::TIPO_CONVERSAO]==
      AtributosParsers::IMPLICITA)
   conv_tipo->definirTipoConversao(ConversaoTipo::CONV_IMPLICITA);
  else
   conv_tipo->definirTipoConversao(ConversaoTipo::CONV_ATRIBUICAO);

  conv_tipo->definirEntradaSaida(atributos[AtributosParsers::CONV_ENTRADA_SAIDA]==AtributosParsers::VERDADEIRO);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     /* Caso o elemento seja um <type>, será extraído do XML
        o tipo (de origem ou destino) da conversao */
     if(elem==AtributosParsers::TIPO)
     {
      tipo=criarTipoPgSQL();
      if(idx_tipo==0)
       conv_tipo->definirTipoDado(ConversaoTipo::CONV_TIPO_ORIGEM, tipo);
      else
       conv_tipo->definirTipoDado(ConversaoTipo::CONV_TIPO_DESTINO, tipo);
      idx_tipo++;
     }
     //Extraíndo a função de conversão do XML
     else if(elem==AtributosParsers::FUNCAO)
     {
      /*No caso da conversão, será extraída a referência à função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribuída à conversão */
      ParserXML::obterAtributosElemento(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[AtributosParsers::ASSINATURA], OBJETO_FUNCAO);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[AtributosParsers::ASSINATURA].isEmpty())
       throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                             .arg(conv_tipo->obterNome())
                             .arg(conv_tipo->obterNomeTipoObjeto())
                             .arg(atributos[AtributosParsers::ASSINATURA])
                             .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_FUNCAO)),
                     ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Atribui a função à conversão de tipos
      conv_tipo->definirFuncaoConversao(dynamic_cast<Funcao *>(funcao));
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  if(conv_tipo) delete(conv_tipo);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(conv_tipo);
}
//-----------------------------------------------------------
ConversaoCodificacao *ModeloBD::criarConversaoCodificacao(void)
{
 map<QString, QString> atributos;
 ConversaoCodificacao *conv_codif=NULL;
 QString elem;
 ObjetoBase *funcao=NULL;

 try
 {
  conv_codif=new ConversaoCodificacao;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(conv_codif);

  //Obtém os atributos
  ParserXML::obterAtributosElemento(atributos);

  conv_codif->definirCodificacao(ConversaoCodificacao::CONV_COD_ORIGEM,
                                 TipoCodificacao(atributos[AtributosParsers::COD_ORIGEM]));

  conv_codif->definirCodificacao(ConversaoCodificacao::CONV_COD_DESTINO,
                                 TipoCodificacao(atributos[AtributosParsers::COD_DESTINO]));

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==AtributosParsers::FUNCAO)
     {
      /*No caso da conversão, será extraída a referência à função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribuída à conversão */
      ParserXML::obterAtributosElemento(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[AtributosParsers::ASSINATURA], OBJETO_FUNCAO);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[AtributosParsers::ASSINATURA].isEmpty())
       throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                             .arg(conv_codif->obterNome())
                             .arg(conv_codif->obterNomeTipoObjeto())
                             .arg(atributos[AtributosParsers::ASSINATURA])
                             .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_FUNCAO)),
                     ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Atribui a função à conversão de tipos
      conv_codif->definirFuncaoConversao(dynamic_cast<Funcao *>(funcao));
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  if(conv_codif) delete(conv_codif);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(conv_codif);
}
//-----------------------------------------------------------
Operador *ModeloBD::criarOperador(void)
{
 map<QString, QString> atributos;
 map<QString, unsigned> tipo_funcoes;
 map<QString, unsigned> tipo_operadores;
 Operador *operador=NULL;
 QString elem;
 ObjetoBase *funcao=NULL,*oper_aux=NULL;
 unsigned tipo_func, tipo_oper, tipo_arg;
 TipoPgSQL tipo;

 try
 {
  operador=new Operador;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(operador);

  //Obtém os atributos
  ParserXML::obterAtributosElemento(atributos);

  operador->definirMerges(atributos[AtributosParsers::MERGES]==AtributosParsers::VERDADEIRO);
  operador->definirHashes(atributos[AtributosParsers::HASHES]==AtributosParsers::VERDADEIRO);

  /* O mapa de tipos de função abaixo é usado para se atribuir de forma
      mas simples, sem comparações, a função que for obtida do XML a qual
      o tipo em construção referencia */
  tipo_funcoes[AtributosParsers::FUNCAO_OPERADOR]=Operador::FUNC_OPERADOR;
  tipo_funcoes[AtributosParsers::FUNCAO_JUNCAO]=Operador::FUNC_JUNCAO;
  tipo_funcoes[AtributosParsers::FUNCAO_RESTRICAO]=Operador::FUNC_RESTRICAO;

  /* O mapa de tipos de operadores abaixo é usado para se atribuir de forma
      mais simples, sem comparações, o operador que for obtida do XML a qual
      o operador em construção referencia */
  tipo_operadores[AtributosParsers::OP_COMUTACAO]=Operador::OPER_COMUTACAO;
  tipo_operadores[AtributosParsers::OP_MAIOR]=Operador::OPER_MAIOR;
  tipo_operadores[AtributosParsers::OP_MENOR]=Operador::OPER_MENOR;
  tipo_operadores[AtributosParsers::OP_NEGACAO]=Operador::OPER_NEGACAO;
  tipo_operadores[AtributosParsers::OP_ORDENACAO]=Operador::OPER_ORDENACAO1;
  tipo_operadores[AtributosParsers::OP_ORDENACAO2]=Operador::OPER_ORDENACAO2;

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==esq_objetos[OBJETO_OPERADOR])
     {
      //Obtém os atributos do operador referenciado
      ParserXML::obterAtributosElemento(atributos);

      /* Com a assinatura do operador obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      oper_aux=obterObjeto(atributos[AtributosParsers::ASSINATURA], OBJETO_OPERADOR);

      //Dispara uma exceção caso o operador referenciado não exista
      if(!oper_aux && !atributos[AtributosParsers::ASSINATURA].isEmpty())
       throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                             .arg(operador->obterAssinatura(true))
                             .arg(operador->obterNomeTipoObjeto())
                             .arg(atributos[AtributosParsers::ASSINATURA])
                             .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_OPERADOR)),
                     ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      tipo_oper=tipo_operadores[atributos[AtributosParsers::TIPO_REFERENCIA]];
      operador->definirOperador(dynamic_cast<Operador *>(oper_aux),tipo_oper);
     }
     else if(elem==AtributosParsers::TIPO)
     {
      /* Obtém os atributos do tipo para saber se o mesmo é um tipo da
         esquerda ou da direita */
      ParserXML::obterAtributosElemento(atributos);

      //Obtém o tipo de referência do tipo base (esquerda ou direita)
      if(atributos[AtributosParsers::TIPO_REFERENCIA]!=AtributosParsers::TIPO_DIRETA)
       tipo_arg=Operador::ARG_ESQUERDA;
      else
       tipo_arg=Operador::ARG_DIREITA;

      tipo=criarTipoPgSQL();
      operador->definirTipoDadoArgumento(tipo, tipo_arg);
     }
     else if(elem==AtributosParsers::FUNCAO)
     {
      /*No caso do operador, será extraída a referência à função no modelo.
        Será através da assinatura de função vinda do XML que a função no modelo
        será localizada e atribuída ao operador */
      ParserXML::obterAtributosElemento(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[AtributosParsers::ASSINATURA], OBJETO_FUNCAO);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[AtributosParsers::ASSINATURA].isEmpty())
       throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                             .arg(operador->obterNome())
                             .arg(operador->obterNomeTipoObjeto())
                             .arg(atributos[AtributosParsers::ASSINATURA])
                             .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_FUNCAO)),
                     ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      /* Obtém o tipo de configuraçao de função do tipo de acordo com a referência
         da mesma obtida do XML */
      tipo_func=tipo_funcoes[atributos[AtributosParsers::TIPO_REFERENCIA]];

      //Atribui a função ao tipo na configuração obtida
      operador->definirFuncao(dynamic_cast<Funcao *>(funcao), tipo_func);
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);
  if(operador) delete(operador);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(operador);
}
//-----------------------------------------------------------
ClasseOperadores *ModeloBD::criarClasseOperadores(void)
{
 map<QString, QString> atributos;
 map<QString, unsigned> tipos_elem;
 ObjetoBase *objeto=NULL;
 QString elem;
 TipoPgSQL tipo;
 ClasseOperadores *classe_op=NULL;
 ElemClasseOperadores elem_classe;
 bool rechecar;
 unsigned num_estrategia, tp_elem;

 try
 {
  classe_op=new ClasseOperadores;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(classe_op);

  //Obtém os atributos
  ParserXML::obterAtributosElemento(atributos);

  classe_op->definirTipoIndexacao(TipoIndexacao(atributos[AtributosParsers::TIPO_INDEXACAO]));
  classe_op->definirPadrao(atributos[AtributosParsers::PADRAO]==AtributosParsers::VERDADEIRO);

  tipos_elem[AtributosParsers::FUNCAO]=ElemClasseOperadores::ELEM_FUNCAO;
  tipos_elem[AtributosParsers::OPERADOR]=ElemClasseOperadores::ELEM_OPERADOR;
  tipos_elem[AtributosParsers::ARMAZENAMENTO]=ElemClasseOperadores::ELEM_ARMAZENAMENTO;

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==esq_objetos[OBJETO_FAMILIA_OPER])
     {
      //Obtém os atributos do operador referenciado
      ParserXML::obterAtributosElemento(atributos);

      /* Com o nome da família do operador obtida do XML, a mesma será buscada no modelo, para
         saber se existe um objeto correspondente */
      objeto=obterObjeto(atributos[AtributosParsers::NOME], OBJETO_FAMILIA_OPER);

      //Dispara uma exceção caso o operador referenciado não exista
      if(!objeto && !atributos[AtributosParsers::NOME].isEmpty())
       throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                             .arg(classe_op->obterNome())
                             .arg(classe_op->obterNomeTipoObjeto())
                             .arg(atributos[AtributosParsers::NOME])
                             .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_FAMILIA_OPER)),
                     ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      classe_op->definirFamilia(dynamic_cast<FamiliaOperadores *>(objeto));
     }
     else if(elem==AtributosParsers::TIPO)
     {
      //Obtém os atributos do tipo
      ParserXML::obterAtributosElemento(atributos);
      tipo=criarTipoPgSQL();
      classe_op->definirTipoDado(tipo);
     }
     else if(elem==AtributosParsers::ELEMENTO)
     {
      ParserXML::obterAtributosElemento(atributos);

      rechecar=atributos[AtributosParsers::RECHECAR]==AtributosParsers::VERDADEIRO;
      num_estrategia=atributos[AtributosParsers::NUM_ESTRATEGIA].toUInt();
      tp_elem=tipos_elem[atributos[AtributosParsers::TIPO]];

      ParserXML::salvarPosicao();
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
      ParserXML::obterAtributosElemento(atributos);

      if(tp_elem==ElemClasseOperadores::ELEM_ARMAZENAMENTO)
      {
       tipo=criarTipoPgSQL();
       elem_classe.definirArmazenamento(tipo);
      }
      else if(tp_elem==ElemClasseOperadores::ELEM_FUNCAO)
      {
       objeto=obterObjeto(atributos[AtributosParsers::ASSINATURA],OBJETO_FUNCAO);
       elem_classe.definirFuncao(dynamic_cast<Funcao *>(objeto),num_estrategia);
      }
      else if(tp_elem==ElemClasseOperadores::ELEM_OPERADOR)
      {
       objeto=obterObjeto(atributos[AtributosParsers::ASSINATURA],OBJETO_OPERADOR);
       elem_classe.definirOperador(dynamic_cast<Operador *>(objeto),num_estrategia,rechecar);
      }

      classe_op->adicionarElementoClasse(elem_classe);
      ParserXML::restaurarPosicao();
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  if(classe_op) delete(classe_op);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(classe_op);
}
//-----------------------------------------------------------
FamiliaOperadores *ModeloBD::criarFamiliaOperadores(void)
{
 map<QString, QString> atributos;
 FamiliaOperadores *familia_op=NULL;

 try
 {
  familia_op=new FamiliaOperadores;
  definirAtributosBasicos(familia_op);

  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);

  //Definindo os valores de atributos básicos do objeto
  familia_op->definirTipoIndexacao(TipoIndexacao(atributos[AtributosParsers::TIPO_INDEXACAO]));
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  if(familia_op) delete(familia_op);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(familia_op);
}
//-----------------------------------------------------------
FuncaoAgregacao *ModeloBD::criarFuncaoAgregacao(void)
{
 map<QString, QString> atributos;
 ObjetoBase *funcao=NULL;
 QString elem;
 TipoPgSQL tipo;
 FuncaoAgregacao *func_agreg=NULL;

 try
 {
  func_agreg=new FuncaoAgregacao;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(func_agreg);

  //Obtém os atributos
  ParserXML::obterAtributosElemento(atributos);
  func_agreg->definirCondicaoInicial(atributos[AtributosParsers::COND_INICIAL]);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==AtributosParsers::TIPO)
     {
      //Obtém os atributos do tipo
      ParserXML::obterAtributosElemento(atributos);
      tipo=criarTipoPgSQL();

      //Define o tipo à função agregada de acordo com o tipo de referência do mesmo
      if(atributos[AtributosParsers::TIPO_REFERENCIA]==
          AtributosParsers::TIPO_ESTADO)
       func_agreg->definirTipoEstado(tipo);
      else
       func_agreg->adicionarTipoDado(tipo);
     }
     else if(elem==AtributosParsers::FUNCAO)
     {
      ParserXML::obterAtributosElemento(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[AtributosParsers::ASSINATURA], OBJETO_FUNCAO);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[AtributosParsers::ASSINATURA].isEmpty())
       throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                             .arg(func_agreg->obterNome())
                             .arg(func_agreg->obterNomeTipoObjeto())
                             .arg(atributos[AtributosParsers::ASSINATURA])
                             .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_FUNCAO)),
                     ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

      //Define a função de acordo com o tipo de referência da mesma
      if(atributos[AtributosParsers::TIPO_REFERENCIA]==AtributosParsers::FUNCAO_TRANSICAO)
       func_agreg->definirFuncao(FuncaoAgregacao::FUNCAO_TRANSICAO,
                                 dynamic_cast<Funcao *>(funcao));
      else
       func_agreg->definirFuncao(FuncaoAgregacao::FUNCAO_FINAL,
                                 dynamic_cast<Funcao *>(funcao));
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  if(func_agreg) delete(func_agreg);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(func_agreg);
}
//-----------------------------------------------------------
Tabela *ModeloBD::criarTabela(void)
{
 map<QString, QString> atributos;
 QString elem;
 Tabela *tabela=NULL;
 ObjetoTabela *objeto=NULL;

 try
 {
  tabela=new Tabela;

  //Lê do parser os atributos basicos
  definirAtributosBasicos(tabela);

  //Obtém os atributos
  ParserXML::obterAtributosElemento(atributos);
  tabela->definirAceitaOids(atributos[AtributosParsers::OIDS]==AtributosParsers::VERDADEIRO);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();
     ParserXML::salvarPosicao();
     objeto=NULL;

     if(elem==ObjetoBase::esq_objetos[OBJETO_COLUNA])
      objeto=criarColuna();
     else if(elem==ObjetoBase::esq_objetos[OBJETO_RESTRICAO])
      objeto=criarRestricao(tabela);
     else if(elem==ObjetoBase::esq_objetos[OBJETO_INDICE])
      objeto=criarIndice(tabela);
     else if(elem==ObjetoBase::esq_objetos[OBJETO_REGRA])
      objeto=criarRegra();
     else if(elem==ObjetoBase::esq_objetos[OBJETO_GATILHO])
      objeto=criarGatilho(tabela);

     if(objeto)
      tabela->adicionarObjeto(objeto);

     ParserXML::restaurarPosicao();
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }

  tabela->definirProtegido(tabela->objetoProtegido());
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  ParserXML::restaurarPosicao();
  if(tabela) delete(tabela);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(tabela);
}
//-----------------------------------------------------------
Coluna *ModeloBD::criarColuna(void)
{
 map<QString, QString> atributos;
 Coluna *coluna=NULL;
 QString elem;

 try
 {
  coluna=new Coluna;
  definirAtributosBasicos(coluna);

  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);
  coluna->definirNaoNulo(atributos[AtributosParsers::NAO_NULO]==AtributosParsers::VERDADEIRO);
  coluna->definirValorPadrao(atributos[AtributosParsers::VALOR_PADRAO]);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==AtributosParsers::TIPO)
     {
      coluna->definirTipo(criarTipoPgSQL());
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  if(coluna) delete(coluna);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(coluna);
}
//-----------------------------------------------------------
Restricao *ModeloBD::criarRestricao(ObjetoBase *objeto)
{
 map<QString, QString> atributos;
 Restricao *restricao=NULL;
 ObjetoBase *tabela_ref=NULL;
 Tabela *tabela=NULL,*tabela_aux=NULL;
 Coluna *coluna=NULL;
 Relacionamento *relacao=NULL;
 QString elem, str_aux;
 bool postergavel, ins_rest_tabela=false;
 TipoRestricao tipo_rest;
 QStringList lista_cols;
 int qtd, i;
 unsigned tipo_coluna;
 TipoObjetoBase tipo_objeto;

 try
 {
  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);

  //Caso o objeto o qual será possuidor da restrição esteja alocado
  if(objeto)
  {
   tipo_objeto=objeto->obterTipoObjeto();
   if(tipo_objeto==OBJETO_TABELA)
    tabela=dynamic_cast<Tabela *>(objeto);
   else if(tipo_objeto==OBJETO_RELACAO)
    relacao=dynamic_cast<Relacionamento *>(objeto);
   else
    throw Excecao(ERR_PGMODELER_OPROBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  /* Caso o objeto não esteja especificado então o objeto possuidor será considerado
     como sendo sempre uma tabela e com base nisso o atributo "table" no código
     xml da restrição será usado para localizar a tabela possuidora no modelo */
  else
  {
   tipo_objeto=OBJETO_TABELA;
   tabela=dynamic_cast<Tabela *>(obterObjeto(atributos[AtributosParsers::TABELA], OBJETO_TABELA));
   ins_rest_tabela=true;
   /* Caso a tabela a qual possua a restição não for encontrada uma exceção será disparada pois
      não se pode criar uma restrição sem que esta seja atribuida a uma tabela, neste caso. */
   if(!tabela)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE))
          .arg(atributos[AtributosParsers::NOME])
          .arg(ObjetoBase::nome_tipo_objetos[OBJETO_RESTRICAO])
          .arg(atributos[AtributosParsers::TABELA])
          .arg(ObjetoBase::nome_tipo_objetos[OBJETO_TABELA]);
    //Dispara a exceção
    throw Excecao(str_aux,ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  restricao=new Restricao;
  restricao->definirTabelaPai(tabela);

  //Configurando o tipo da restrição
  if(atributos[AtributosParsers::TIPO]==AtributosParsers::REST_CK)
   tipo_rest=TipoRestricao::check;
  else if(atributos[AtributosParsers::TIPO]==AtributosParsers::REST_PK)
   tipo_rest=TipoRestricao::primary_key;
  else if(atributos[AtributosParsers::TIPO]==AtributosParsers::REST_FK)
   tipo_rest=TipoRestricao::foreign_key;
  else
   tipo_rest=TipoRestricao::unique;

  restricao->definirTipo(tipo_rest);
  if(!atributos[AtributosParsers::FATOR].isEmpty())
   restricao->definirFatorPreenchimento(atributos[AtributosParsers::FATOR].toUInt());
  definirAtributosBasicos(restricao);

  /* Caso o tipo de restrição seja uma chave primária uma verificação importante
     é feita. Se a chave primária está sendo criada como filha de uma tag
     <table> ou <relationship> (vide exemplo 1).

     Exemplo 1:

      <table ...>        <relationship ...>
         ...                ...
         <constraint>       <constraint>
      </table>           </relationship>

     Esta restrição está declarada dentro de um bloco de código de uma tabela ou
     de relacionamento a mesma sendo assim a mesma será construída normalmente.

     Caso a mesma esteja sendo construída fora de ambos os blocos (vide exemplo 2)
     e referenciando apenas uma tabela no modelo um erro será gerado pois chaves
     primárias devem sempre ser construídas dentro de um dos dois blocos apresentados.

     Exemplo 2:
     <table name="tabela_X">
       ...
     </table>
     <relationship>
       ...
     </relationship>
     <constraint ... table="tabela_X">
       ...
     </constraint>  */

  /* Na prática, para se fazer a verificação acima, é suficiente
     verificar se o parâmetro 'objeto' está alocado pois o mesmo
     armazena o endereço da tabela ou relacionamento recém criados
     a partir do bloco <table> ou <relationship>, respectivamente.
     Quando tal parâmetro está nulo indica que a restrição será criada
     e atribuída à tabela cujo nome está no atributo 'table' no XML
     significando que a mesma está declarada fora dos dois blocos indicados.
     Adicionalmente é necessário verificar o tipo da restrição para se
     ter certeza que a mesma é uma chave primária. */
  if(!objeto && tipo_rest==TipoRestricao::primary_key)
    throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ALOCPKFORMAINVALIDA).arg(restricao->obterNome()),
                  ERR_PGMODELER_ALOCPKFORMAINVALIDA,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Efetuando configurações específicas para chaves estrangeiras
  if(tipo_rest==TipoRestricao::foreign_key && tipo_objeto==OBJETO_TABELA)
  {
   //Define se a restrição é postergavel (apenas para chaves estrangeiras)
   postergavel=(atributos[AtributosParsers::POSTERGAVEL]==AtributosParsers::VERDADEIRO);
   restricao->definirPostergavel(postergavel);

   if(postergavel && !atributos[AtributosParsers::TIPO_POSTERGACAO].isEmpty())
    restricao->definirTipoPostergacao(atributos[AtributosParsers::TIPO_POSTERGACAO]);

   if(!atributos[AtributosParsers::TIPO_COMPARACAO].isEmpty())
    restricao->definirTipoComparacao(atributos[AtributosParsers::TIPO_COMPARACAO]);

   //Definindo os tipos de ação nos eventos DELETE e UPDATE
   if(!atributos[AtributosParsers::ACAO_DELETE].isEmpty())
    restricao->definirTipoAcao(atributos[AtributosParsers::ACAO_DELETE], false);

   if(!atributos[AtributosParsers::ACAO_UPDATE].isEmpty())
    restricao->definirTipoAcao(atributos[AtributosParsers::ACAO_UPDATE], true);

   //Obtém a tabela referenciada na chave estrangeira
   tabela_ref=obterObjeto(atributos[AtributosParsers::TABELA_REF], OBJETO_TABELA);

   /* Caso a tabela referenciada não seja encontrada verifica se esta não é a própria
      tabela a qual receberá a restrição (usado para auto-relacionamentos) */
   if(!tabela_ref && tabela->obterNome(true)==atributos[AtributosParsers::TABELA_REF])
    tabela_ref=tabela;

   /* Caso a tabela referenciada não foi encontrada uma exceção será disparada pois
      não se pode criar uma chave estrangeira sem referenciar uma tabela de destino */
   if(!tabela_ref)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE))
          .arg(restricao->obterNome())
          .arg(ObjetoBase::nome_tipo_objetos[restricao->obterTipoObjeto()])
          .arg(atributos[AtributosParsers::TABELA_REF])
          .arg(ObjetoBase::nome_tipo_objetos[OBJETO_TABELA]);
    //Dispara a exceção
    throw Excecao(str_aux,ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   //Define a tabela de destino da chave estrangeira
   restricao->definirTabReferenciada(tabela_ref);
  }

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==AtributosParsers::EXPRESSAO)
     {
      ParserXML::salvarPosicao();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
      //Obtém o contéudo do elemento <expression>
      restricao->definirExpChecagem(ParserXML::obterConteudoElemento());
      ParserXML::restaurarPosicao();
     }
     else if(elem==AtributosParsers::COLUNAS)
     {
      //Obtém os atributos da tag <columns>
      ParserXML::obterAtributosElemento(atributos);

      /* Obtém os nomes das colunas participantes da restrição
         colocando seus nomes em um vetor pois os mesmos estão
         unidos por vírgula, neste caso o método split é usado
         para fazer a divisão */
      lista_cols=atributos[AtributosParsers::NOMES].split(',');
      qtd=lista_cols.count();

      /* Obtém o tipo de referência das colunas de acordo com o atributo
         tipo de referência vindo do XML */
      if(atributos[AtributosParsers::TIPO_REFERENCIA]==AtributosParsers::COLUNAS_ORIGEM)
       tipo_coluna=Restricao::COLUNA_ORIGEM;
      else
       tipo_coluna=Restricao::COLUNA_REFER;

      //Varre a lista de nomes de colunas e as obtém da tabela a qual possuirá a restrição
      for(i=0; i < qtd; i++)
      {
       if(tipo_objeto==OBJETO_TABELA)
       {
        if(tipo_coluna==Restricao::COLUNA_ORIGEM)
        {
         coluna=tabela->obterColuna(lista_cols[i]);
         //Caso a coluna não for encontrada, tenta obtê-la referenciando seu nome antigo
         if(!coluna)
          coluna=tabela->obterColuna(lista_cols[i], true);
        }
        else
        {
         tabela_aux=dynamic_cast<Tabela *>(tabela_ref);
         coluna=tabela_aux->obterColuna(lista_cols[i]);
         //Caso a coluna não for encontrada, tenta obtê-la referenciando seu nome antigo
         if(!coluna)
          coluna=tabela_aux->obterColuna(lista_cols[i], true);
        }
       }
       else
        //Para os demais tipos de relacionamento as colunas a serem obtidas são os atributos do relacionamento
         coluna=dynamic_cast<Coluna *>(relacao->obterObjeto(lista_cols[i], OBJETO_COLUNA));

       //Adiciona a coluna à restrição
       restricao->adicionarColuna(coluna, tipo_coluna);
      }
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }

  if(ins_rest_tabela)
  {
   //Caso a restrição criada não seja uma chave primária insere-a normalmente na tabela
   if(restricao->obterTipoRestricao()!=TipoRestricao::primary_key)
    tabela->adicionarRestricao(restricao);
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  if(restricao)
   delete(restricao);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(restricao);
}
//-----------------------------------------------------------
Indice *ModeloBD::criarIndice(Tabela *tabela)
{
 map<QString, QString> atributos;
 Indice *indice=NULL;
 Coluna *coluna=NULL;
 ClasseOperadores *classe_oper=NULL;
 QString elem, str_aux, expr;
 bool inc_ind_tabela=false,
      ordem_asc=false, nulos_primeiro=false;

 try
 {
  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);

  if(!tabela)
  {
   inc_ind_tabela=true;
   tabela=dynamic_cast<Tabela *>(obterObjeto(atributos[AtributosParsers::TABELA], OBJETO_TABELA));
   /* Caso a tabela a qual possua a restição não for encontrada uma exceção será disparada pois
      não se pode criar uma restrição sem que esta seja atribuida a uma tabela, neste caso. */
   if(!tabela)
   {
    //Configura os argumentos da mensagem de erro
    str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE))
          .arg(atributos[AtributosParsers::NOME])
          .arg(ObjetoBase::nome_tipo_objetos[OBJETO_INDICE])
          .arg(atributos[AtributosParsers::TABELA])
          .arg(ObjetoBase::nome_tipo_objetos[OBJETO_TABELA]);
    //Dispara a exceção
    throw Excecao(str_aux,ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  indice=new Indice;
  definirAtributosBasicos(indice);
  indice->definirTabelaPai(tabela);
  indice->definirAtributo(Indice::CONCORRENTE, atributos[AtributosParsers::CONCORRENTE]==AtributosParsers::VERDADEIRO);
  indice->definirAtributo(Indice::UNIQUE, atributos[AtributosParsers::UNIQUE]==AtributosParsers::VERDADEIRO);
  indice->definirAtributo(Indice::ATUAL_RAPIDA, atributos[AtributosParsers::ATUAL_RAPIDA]==AtributosParsers::VERDADEIRO);

  indice->definirTipoIndexacao(atributos[AtributosParsers::TIPO_INDEXACAO]);
  indice->definirFatorPreenchimento(atributos[AtributosParsers::FATOR].toUInt());

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     /* Caso o elemento atual for do tipo <idxelement> indica que
        os elementos filhos que podem ser extraídos são
        <column>, <expression> ou <opclass> */
     if(elem==AtributosParsers::ELEMENTO_INDICE)
     {
      nulos_primeiro=(atributos[AtributosParsers::NULOS_PRIMEIRO]==AtributosParsers::VERDADEIRO);
      ordem_asc=(atributos[AtributosParsers::ORDEM_ASCENDENTE]==AtributosParsers::VERDADEIRO);

      ParserXML::salvarPosicao();
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);

      do
      {
       elem=ParserXML::obterNomeElemento();

       if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
       {
        //Caso o elemento atual seja um  <opclass>
        if(elem==AtributosParsers::CLASSE_OPERADORES)
        {
         ParserXML::obterAtributosElemento(atributos);
         classe_oper=dynamic_cast<ClasseOperadores *>(obterObjeto(atributos[AtributosParsers::NOME], OBJETO_CLASSE_OPER));

         //Caso o índice esteja referenciando uma classe de operadores inexistente
         if(!classe_oper)
         {
          //Configura os argumentos da mensagem de erro
          str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE))
                          .arg(indice->obterNome())
                          .arg(ObjetoBase::nome_tipo_objetos[OBJETO_INDICE])
                          .arg(atributos[AtributosParsers::CLASSE_OPERADORES])
                          .arg(ObjetoBase::nome_tipo_objetos[OBJETO_CLASSE_OPER]);
          //Dispara a exceção
          throw Excecao(str_aux,ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
         }
        }
        //Caso o elemento atual seja um  <column>
        else if(elem==AtributosParsers::COLUNA)
        {
         //Obtém a coluna que o elemento referencia
         ParserXML::obterAtributosElemento(atributos);
         coluna=tabela->obterColuna(atributos[AtributosParsers::NOME]);

         /* Caso a coluna não exista tenta obtê-la novamente porém referenciando
            seu nome antigo */
         if(!coluna)
          coluna=tabela->obterColuna(atributos[AtributosParsers::NOME], true);
        }
        //Caso o elemento atual seja um  <expression>
        else if(elem==AtributosParsers::EXPRESSAO)
        {
         ParserXML::salvarPosicao();
         //Acessa o elemento filho o qual contém o conteúdo da expressão
         ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
         expr=ParserXML::obterConteudoElemento();
         ParserXML::restaurarPosicao();
        }
       }
      }
      while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));

      if(!expr.isEmpty())
       indice->adicionarElemento(expr, classe_oper, ordem_asc, nulos_primeiro);
      else
       indice->adicionarElemento(coluna, classe_oper, ordem_asc, nulos_primeiro);

      ParserXML::restaurarPosicao();
     }
     else if(elem==AtributosParsers::CONDICAO)
     {
      ParserXML::salvarPosicao();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
      str_aux=ParserXML::obterConteudoElemento();
      ParserXML::restaurarPosicao();
      indice->definirExpCondicional(str_aux);
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }

  if(inc_ind_tabela)
  {
   tabela->adicionarIndice(indice);
   tabela->definirModificado(true);
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  if(indice) delete(indice);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(indice);
}
//-----------------------------------------------------------
Regra *ModeloBD::criarRegra(void)
{
 map<QString, QString> atributos;
 QStringList lista_cmd;
 Regra *regra=NULL;
 QString elem, str_aux;
 int qtd, i;

 try
 {
  regra=new Regra;
  definirAtributosBasicos(regra);

  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);
  regra->definirTipoExecucao(atributos[AtributosParsers::TIPO_EXECUCAO]);
  regra->definirTipoEvento(atributos[AtributosParsers::TIPO_EVENTO]);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==AtributosParsers::COMANDOS ||
        elem==AtributosParsers::CONDICAO)
     {
      ParserXML::salvarPosicao();
      //Acessa o elemento filho o qual contém o conteúdo da condição ou comandos
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
      //str_aux=QString::fromUtf8(ParserXML::obterConteudoElemento());
      str_aux=ParserXML::obterConteudoElemento();
      ParserXML::restaurarPosicao();

      if(elem==AtributosParsers::COMANDOS)
      {
       /* A lista de comandos é quebrada por ; e os comandos
          inseridos um a um na regra */
       lista_cmd=str_aux.split(';');
       qtd=lista_cmd.count();
       for(i=0; i < qtd; i++)
       {
        //Caso o comando não esteja vazio
        if(!lista_cmd[i].isEmpty())
         regra->adicionarComando(lista_cmd[i]);
       }
      }
      else
       regra->definirExpCondicional(str_aux);
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);
  if(regra) delete(regra);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(regra);
}
//-----------------------------------------------------------
Gatilho *ModeloBD::criarGatilho(Tabela *tabela)
{
 map<QString, QString> atributos;
 Gatilho *gatilho=NULL;
 QString elem, str_aux;
 QStringList lista_aux;
 int qtd, i;
 ObjetoBase *tabela_ref=NULL, *funcao=NULL;
 Coluna *coluna=NULL;
 bool inc_gat_tabela=false;

 try
 {
  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);

  if(!tabela && atributos[AtributosParsers::TABELA].isEmpty())
   throw Excecao(ERR_PGMODELER_OPROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else if(!tabela && !atributos[AtributosParsers::TABELA].isEmpty())
  {
   inc_gat_tabela=true;
   tabela=dynamic_cast<Tabela *>(obterObjeto(atributos[AtributosParsers::TABELA], OBJETO_TABELA));
   if(!tabela)
    throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE))
                  .arg(atributos[AtributosParsers::NOME])
                  .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_GATILHO))
                  .arg(atributos[AtributosParsers::TABELA])
                  .arg(ObjetoBase::nome_tipo_objetos[OBJETO_TABELA]),
                  ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  gatilho=new Gatilho;
  gatilho->definirTabelaPai(tabela);

  definirAtributosBasicos(gatilho);

  //Marcando os eventos de execução do gatilho
  gatilho->definirEvento(TipoEvento::on_insert,
                        (atributos[AtributosParsers::EVENTO_INS]==
                         AtributosParsers::VERDADEIRO));

  gatilho->definirEvento(TipoEvento::on_delete,
                        (atributos[AtributosParsers::EVENTO_DEL]==
                         AtributosParsers::VERDADEIRO));

  gatilho->definirEvento(TipoEvento::on_update,
                        (atributos[AtributosParsers::EVENTO_UPD]==
                         AtributosParsers::VERDADEIRO));

  gatilho->definirEvento(TipoEvento::on_truncate,
                        (atributos[AtributosParsers::EVENTO_TRUNC]==
                         AtributosParsers::VERDADEIRO));

  //Marcando e o gatilho é executado por linha ou não
  gatilho->executarPorLinha(atributos[AtributosParsers::POR_LINHA]==
                            AtributosParsers::VERDADEIRO);

  //Define o modo de disparo do gatilho
  gatilho->definirTipoDisparo(TipoDisparo(atributos[AtributosParsers::TIPO_DISPARO]));


  /* Atribuindo os argumentos vindo do XML ao gatilho.
     No XML os argumentos estão separados por vírgula,
     sendo assim o método split é usado para quebrar a
     string de argumentos e atribui-los ao objeto */
  lista_aux=atributos[AtributosParsers::ARGUMENTOS].split(',');
  qtd=lista_aux.count();
  for(i=0; i < qtd; i++)
  {
   if(!lista_aux[i].isEmpty())
    gatilho->adicionarArgumento(lista_aux[i]);
  }

  //Caso o objeto alocado seja um gatilho
  gatilho->definirPostergavel(atributos[AtributosParsers::POSTERGAVEL]==
                              AtributosParsers::VERDADEIRO);
  if(gatilho->gatilhoPostergavel())
    gatilho->definirTipoPostergacao(atributos[AtributosParsers::TIPO_POSTERGACAO]);

  //Obtém a tabela referenciada no gatilho
  tabela_ref=obterObjeto(atributos[AtributosParsers::TABELA_REF], OBJETO_TABELA);
  gatilho->definirTabReferenciada(tabela_ref);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==AtributosParsers::FUNCAO)
     {
      ParserXML::obterAtributosElemento(atributos);

      /* Com a assinatura da função obtida do XML, a mesma será buscada no modelo, para
         saber se existe a função correspondente */
      funcao=obterObjeto(atributos[AtributosParsers::ASSINATURA], OBJETO_FUNCAO);

      //Dispara uma exceção caso a função referenciada não exista
      if(!funcao && !atributos[AtributosParsers::ASSINATURA].isEmpty())
      {
       str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE))
               .arg(gatilho->obterNome())
               .arg(ObjetoBase::nome_tipo_objetos[gatilho->obterTipoObjeto()])
               .arg(atributos[AtributosParsers::ASSINATURA])
               .arg(ObjetoBase::nome_tipo_objetos[OBJETO_FUNCAO]);

       //Dispara a exceção
       throw Excecao(str_aux,ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }

      //Define a função executada pelo gatilho
      gatilho->definirFuncao(dynamic_cast<Funcao *>(funcao));
     }
     else if(elem==AtributosParsers::CONDICAO)
     {
      ParserXML::salvarPosicao();
      //Acessa o elemento filho o qual contém o conteúdo da expressão ou condição
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
      str_aux=ParserXML::obterConteudoElemento();
      ParserXML::restaurarPosicao();
      gatilho->definirCondicao(str_aux);
     }
     else if(elem==AtributosParsers::COLUNAS)
     {
      //Obtém os atributos da tag <columns>
      ParserXML::obterAtributosElemento(atributos);

      /* Obtém os nomes das colunas participantes do gatilho
         colocando seus nomes em um vetor pois os mesmos estão
         unidos por vírgula, neste caso o método split é usado
         para fazer a divisão */
      lista_aux=atributos[AtributosParsers::NOMES].split(',');
      qtd=lista_aux.count();

      for(i=0; i < qtd; i++)
      {
       coluna=tabela->obterColuna(lista_aux[i]);

       /* Caso a coluna não exista tenta obtê-la novamente porém referenciando
          seu nome antigo */
       if(!coluna)
        coluna=tabela->obterColuna(lista_aux[i], true);

       gatilho->adicionarColuna(coluna);
      }
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }

  if(inc_gat_tabela)
  {
   tabela->adicionarObjeto(gatilho);
   tabela->definirModificado(true);
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);
  if(gatilho) delete(gatilho);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(gatilho);
}
//-----------------------------------------------------------
Sequencia *ModeloBD::criarSequencia(void)
{
 map<QString, QString> atributos;
 Sequencia *sequencia=NULL;
 ObjetoBase *tabela=NULL;
 Coluna *coluna=NULL;
 QString elem, str_aux, nome_tab, nome_col;
 QStringList lista_elem;
 int qtd;

 try
 {
  sequencia=new Sequencia;
  definirAtributosBasicos(sequencia);

  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);
  sequencia->definirValores(atributos[AtributosParsers::VALOR_MINIMO],
                            atributos[AtributosParsers::VALOR_MAXIMO],
                            atributos[AtributosParsers::INCREMENTO],
                            atributos[AtributosParsers::INICIO],
                            atributos[AtributosParsers::CACHE]);
  sequencia->definirCiclica(atributos[AtributosParsers::CICLICA]==AtributosParsers::VERDADEIRO);

  //Caso o atributo de coluna possuidora da sequencia esteja preenchido
  if(!atributos[AtributosParsers::POSSUIDORA].isEmpty())
  {
   //Quebra o valor do atributo por .
   lista_elem=atributos[AtributosParsers::POSSUIDORA].split('.');
   qtd=lista_elem.count();

   /* Caso a lista de nomes gerada possua 3 elementos indica
      que a coluna possuidora foi está no formato
      [ESQUEMA].[TABELA].[COLUNA] caso contrário
      supõe-se que esteja no formato
      [TABELA].[COLUNA] */
   if(qtd==3)
   {
    nome_tab=lista_elem[0] + "." + lista_elem[1];
    nome_col=lista_elem[2];
   }
   else
   {
    nome_tab=lista_elem[0];
    nome_col=lista_elem[1];
   }

   //Obtém a tabela do modelo
   tabela=obterObjeto(nome_tab, OBJETO_TABELA);

   //Dispara uma exceção caso a tabela referenciada não exista
   if(!tabela)
   {
    str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE))
            .arg(sequencia->obterNome())
            .arg(ObjetoBase::nome_tipo_objetos[OBJETO_SEQUENCIA])
            .arg(nome_tab)
            .arg(ObjetoBase::nome_tipo_objetos[OBJETO_TABELA]);

    //Dispara a exceção
    throw Excecao(str_aux,ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   //Tenta obter a coluna da tabela com o nome vindo do XML
   coluna=dynamic_cast<Tabela *>(tabela)->obterColuna(nome_col);

   //Caso a coluna não for encontrada tenta obtê-la referenciando o antigo nome
   if(!coluna)
    coluna=dynamic_cast<Tabela *>(tabela)->obterColuna(nome_col, true);

   /* Caso a coluna não exista porém a mesma esteja sendo referenciada no xml
      um erro será disparado */
   if(!coluna)
    throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRCOLPOSINDEF).arg(sequencia->obterNome(true)),
                  ERR_PGMODELER_ATRCOLPOSINDEF,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   sequencia->definirPossuidora(coluna);
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);
  if(sequencia) delete(sequencia);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(sequencia);
}
//-----------------------------------------------------------
Visao *ModeloBD::criarVisao(void)
{
 map<QString, QString> atributos;
 Visao *visao=NULL;
 Coluna *coluna=NULL;
 Tabela *tabela=NULL;
 QString elem, str_aux;
 QStringList lista_aux;
 vector<Referencia> vet_refs;
 unsigned tipo;
 int idx_ref, i, qtd;

 try
 {
  visao=new Visao;
  definirAtributosBasicos(visao);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==AtributosParsers::REFERENCIA)
     {
      //Obtém os atributos da referência
      ParserXML::obterAtributosElemento(atributos);

      /* Caso o nome da tabela referenciada esteja preenchido,
         tentar criar uma referência específica a uma tabela/coluna */
      if(!atributos[AtributosParsers::TABELA].isEmpty())
      {
       coluna=NULL;
       tabela=dynamic_cast<Tabela *>(obterObjeto(atributos[AtributosParsers::TABELA], OBJETO_TABELA));

       //Dispara uma exceção caso a tabela referenciada não exista
       if(!tabela)
       {
        str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE))
                        .arg(visao->obterNome())
                        .arg(ObjetoBase::nome_tipo_objetos[OBJETO_VISAO])
                        .arg(atributos[AtributosParsers::TABELA])
                        .arg(ObjetoBase::nome_tipo_objetos[OBJETO_TABELA]);

        //Dispara a exceção
        throw Excecao(str_aux,ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }

       if(!atributos[AtributosParsers::COLUNA].isEmpty())
       {
        //Tenta obter a colna referenciada da tabela
        coluna=tabela->obterColuna(atributos[AtributosParsers::COLUNA]);

        //Caso a coluna não exista tenta obtê-la referenciando o nome antigo da mesma
        if(!coluna)
         coluna=tabela->obterColuna(atributos[AtributosParsers::COLUNA], true);

        /* Caso o atributo coluna da referencia esteja preenchido mas um objeto coluna
           não foi encontrado na tabela, uma exceção será disparada pois a visão está
           referenciando uma coluna inexistente na tabela */
         if(!coluna)
         {
          str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE))
                          .arg(visao->obterNome())
                          .arg(ObjetoBase::nome_tipo_objetos[OBJETO_VISAO])
                          .arg(atributos[AtributosParsers::TABELA] + "." +
                               atributos[AtributosParsers::COLUNA])
                         .arg(ObjetoBase::nome_tipo_objetos[OBJETO_COLUNA]);

          //Dispara a exceção
          throw Excecao(str_aux,ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
         }
        }

       //Adiciona a referência configurada à lista temporária de referências
       vet_refs.push_back(Referencia(tabela, coluna,
                                     atributos[AtributosParsers::ALIAS],
                                     atributos[AtributosParsers::ALIAS_COLUNA]));
      }
      //Extraindo uma referência à uma expressão
      else
      {
       ParserXML::salvarPosicao();
       //Armazena o alias da expressão
       str_aux=atributos[AtributosParsers::ALIAS];

       //Acessa e obtém o conteúdo da expressão
       ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
       ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
       vet_refs.push_back(Referencia(ParserXML::obterConteudoElemento(),str_aux));

       ParserXML::restaurarPosicao();
      }
     }
     /* Extraindo as expressões as quais formam as partes da declaração da visão,
        ou seja, expressões e referências as quais estão entre SELECT-FROM,
        FROM-WHERE */
     else if(elem==AtributosParsers::EXPRESSAO)
     {
      ParserXML::salvarPosicao();
      ParserXML::obterAtributosElemento(atributos);

      //Armazena o alias da expressão
      if(atributos[AtributosParsers::TIPO]==AtributosParsers::EXP_SELECT)
       tipo=Referencia::SQL_REFER_SELECT;
      else if(atributos[AtributosParsers::TIPO]==AtributosParsers::EXP_FROM)
       tipo=Referencia::SQL_REFER_FROM;
      else
       tipo=Referencia::SQL_REFER_WHERE;

      //Acessa e obtém o conteúdo da expressão
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
      lista_aux=ParserXML::obterConteudoElemento().split(',');
      qtd=lista_aux.size();

      //Construindo cada expressão na visão
      for(i=0; i < qtd; i++)
      {
       //Obtém o índice da referência e a adiciona à visão
       idx_ref=lista_aux[i].toInt();
       visao->adicionarReferencia(vet_refs[idx_ref],tipo);
      }

      ParserXML::restaurarPosicao();
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);
  if(visao) delete(visao);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(visao);
}
//-----------------------------------------------------------
CaixaTexto *ModeloBD::criarCaixaTexto(void)
{
 CaixaTexto *caixa_texto=NULL;
 map<QString, QString> atributos;

 try
 {
  caixa_texto=new CaixaTexto;
  definirAtributosBasicos(caixa_texto);

  ParserXML::obterAtributosElemento(atributos);

  if(atributos[AtributosParsers::ITALICO]==AtributosParsers::VERDADEIRO)
   caixa_texto->definirAtributoTexto(CaixaTexto::TEXTO_ITALICO, true);

  if(atributos[AtributosParsers::NEGRITO]==AtributosParsers::VERDADEIRO)
   caixa_texto->definirAtributoTexto(CaixaTexto::TEXTO_NEGRITO, true);

  if(atributos[AtributosParsers::SUBLINHADO]==AtributosParsers::VERDADEIRO)
   caixa_texto->definirAtributoTexto(CaixaTexto::TEXTO_SUBLINHADO, true);

  if(!atributos[AtributosParsers::COR].isEmpty())
   caixa_texto->definirCorTexto(QColor(atributos[AtributosParsers::COR]));
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  if(caixa_texto) delete(caixa_texto);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(caixa_texto);
}
//-----------------------------------------------------------
RelacionamentoBase *ModeloBD::criarRelacionamento(void)
{
 vector<unsigned> cols_pk_especial;
 map<QString, QString> atributos;
 RelacionamentoBase *relacao_base=NULL;
 Relacionamento *relacao=NULL;
 TabelaBase *tabelas[2]={NULL, NULL};
 bool obrig_orig, obrig_dest, identificador, protegido, postergavel;
 TipoPostergacao tipo_postergacao;
 unsigned tipo_relac=0, i;
 TipoObjetoBase tipos_tab[2]={OBJETO_VISAO, OBJETO_TABELA}, tipo_obj_rel;
 QString str_aux, elem,
         atribs[2]={ AtributosParsers::TABELA_ORIGEM,
                     AtributosParsers::TABELA_DESTINO };

 try
 {
  //Obtém os atributos do elemento
  ParserXML::obterAtributosElemento(atributos);
  protegido=(atributos[AtributosParsers::PROTEGIDO]==AtributosParsers::VERDADEIRO);

  if(atributos[TIPO]!=AtributosParsers::RELAC_TAB_VISAO)
  {
   tipos_tab[0]=OBJETO_TABELA;
   tipo_obj_rel=OBJETO_RELACAO;
  }
  else
   tipo_obj_rel=OBJETO_RELACAO_BASE;

  /* Esta iteração obtém as tabelas participantes do relacionamento a
     partir do modelo com base nos nomes das tabelas vindos do XML */
  for(i=0; i < 2; i++)
  {
   //Localiza a tabela
   tabelas[i]=dynamic_cast<TabelaBase *>(obterObjeto(atributos[atribs[i]], tipos_tab[i]));

   //Dispara uma exceção caso a tabela referenciada não exista
   if(!tabelas[i])
   {
    str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE))
                    .arg(atributos[AtributosParsers::NOME])
                    .arg(ObjetoBase::nome_tipo_objetos[tipo_obj_rel])
                    .arg(atributos[atribs[i]])
                    .arg(ObjetoBase::nome_tipo_objetos[tipos_tab[i]]);

    //Dispara a exceção
    throw Excecao(str_aux,ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }

  //Caso o relacionamento entre tabela e visão exista
  relacao_base=obterRelacionamento(tabelas[0], tabelas[1]);
  if(atributos[TIPO]==AtributosParsers::RELAC_TAB_VISAO)
  {
   //Caso o relacionamento tabela-visão nao seja encontrado o erro será disparado
   if(!relacao_base)
    throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFOBJINEXISTE)
                             .arg(this->obterNome())
                             .arg(this->obterNomeTipoObjeto())
                             .arg(atributos[AtributosParsers::NOME])
                             .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_RELACAO_BASE)),
                  ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

   //Desconecta o relacionamento para configurá-lo
   relacao_base->desconectarRelacionamento();
   relacao_base->definirNome(atributos[AtributosParsers::NOME]);
  }
  /* Caso o tipo de relacionamento não seja tabela-visão, isso indica que
     um relacionamento tabela-tabela deverá ser criado */
  else if(atributos[TIPO]!=AtributosParsers::RELAC_TAB_VISAO)
  {
   //Obtém os atributos do relacionamento a partir do XML
   obrig_orig=atributos[AtributosParsers::OBRIG_ORIGEM]==AtributosParsers::VERDADEIRO;
   obrig_dest=atributos[AtributosParsers::OBRIG_DESTINO]==AtributosParsers::VERDADEIRO;
   identificador=atributos[AtributosParsers::IDENTIFICADOR]==AtributosParsers::VERDADEIRO;
   postergavel=atributos[AtributosParsers::POSTERGAVEL]==AtributosParsers::VERDADEIRO;
   tipo_postergacao=TipoPostergacao(atributos[AtributosParsers::TIPO_POSTERGACAO]);

   //Configura o tipo do novo relacionamento
   if(atributos[TIPO]==AtributosParsers::RELAC_11)
    tipo_relac=RelacionamentoBase::RELACIONAMENTO_11;
   else if(atributos[TIPO]==AtributosParsers::RELAC_1N)
    tipo_relac=RelacionamentoBase::RELACIONAMENTO_1N;
   else if(atributos[TIPO]==AtributosParsers::RELAC_NN)
    tipo_relac=RelacionamentoBase::RELACIONAMENTO_NN;
   else if(atributos[TIPO]==AtributosParsers::RELAC_GEN)
    tipo_relac=RelacionamentoBase::RELACIONAMENTO_GEN;
   else if(atributos[TIPO]==AtributosParsers::RELAC_DEP)
    tipo_relac=RelacionamentoBase::RELACIONAMENTO_DEP;

   //Cria o novo relacionamento
   relacao=new Relacionamento(atributos[AtributosParsers::NOME], tipo_relac,
                              dynamic_cast<Tabela *>(tabelas[0]),
                              dynamic_cast<Tabela *>(tabelas[1]),
                              obrig_orig, obrig_dest,
                              atributos[AtributosParsers::SUFIXO_ORIGEM],
                              atributos[AtributosParsers::SUFIXO_DESTINO],
                              identificador, postergavel, tipo_postergacao);

   if(!atributos[AtributosParsers::NOME_TABELA].isEmpty())
    relacao->definirNomeTabelaRelNN(atributos[AtributosParsers::NOME_TABELA]);

   /* Faz com que o ponteiro relacao_base aponte para o novo relacionamento
      para executar as configurações geréricas as quais se aplicam tanto
      para relacionametno tabela-visao quanto para rel. tabela-tabela */
   relacao_base=relacao;
  }

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
       qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     elem=ParserXML::obterNomeElemento();

     if(elem==AtributosParsers::COLUNA && relacao)
     {
      ParserXML::salvarPosicao();
      relacao->adicionarObjeto(criarColuna());
      ParserXML::restaurarPosicao();
     }
     else if(elem==AtributosParsers::RESTRICAO && relacao)
     {
      ParserXML::salvarPosicao();
      relacao->adicionarObjeto(criarRestricao(relacao));
      ParserXML::restaurarPosicao();
     }
     //Configurando a linha do relacionamento
     else if(elem==AtributosParsers::LINHA)
     {
      vector<QPointF> pontos;
      ParserXML::salvarPosicao();
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);

      do
      {
       //Lê o ponto do XML
       ParserXML::obterAtributosElemento(atributos);
       pontos.push_back(QPointF(atributos[AtributosParsers::POSICAO_X].toFloat(),
                                atributos[AtributosParsers::POSICAO_Y].toFloat()));
      }
      while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));

      relacao_base->definirPontos(pontos);
      ParserXML::restaurarPosicao();
     }
     //Configurando a posição dos rótulos
     else if(elem==AtributosParsers::ROTULO)
     {
      ParserXML::obterAtributosElemento(atributos);
      //Obtém o tipo de rótulo a ser configurado
      str_aux=atributos[AtributosParsers::TIPO_REFERENCIA];

      //Acessa o elemento filho da tag <label> o qual armazena a posição do rótulo
      ParserXML::salvarPosicao();
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
      ParserXML::obterAtributosElemento(atributos);
      ParserXML::restaurarPosicao();
     }
     else if(elem==AtributosParsers::COLUNAS_PK_ESPECIAL && relacao)
     {
      QList<QString> lista_cols;

      //Obtém os atributos da tag <special-pk-cols>
      ParserXML::obterAtributosElemento(atributos);
      lista_cols=atributos[AtributosParsers::INDICES].split(',');

      while(!lista_cols.isEmpty())
      {
       cols_pk_especial.push_back(lista_cols.front().toUInt());
       lista_cols.pop_front();
      }

      //Define as colunas que fazem parte da chave primária especila
      relacao->definirColsChavePrimariaEspecial(cols_pk_especial);
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);

  if(relacao_base && relacao_base->obterTipoObjeto()==OBJETO_RELACAO)
   delete(relacao_base);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 //Caso o relacionamento tabela-tabela foi criado o mesmo será adicionado no modelo
 if(relacao)
 {
  obterXMLObjetosEspeciais();
  adicionarRelacionamento(relacao);
 }

 //Define a proteção do relacionamento
 relacao_base->definirProtegido(protegido);

 //Reconecta o relacionamento caso o mesmo seja um rel. tabela-visao
 if(relacao_base && relacao_base->obterTipoObjeto()==OBJETO_RELACAO_BASE)
  relacao_base->conectarRelacionamento();

 return(relacao_base);
}
//-----------------------------------------------------------
Permissao *ModeloBD::criarPermissao(void)
{
 Permissao *permissao=NULL;
 ObjetoBase *objeto=NULL;
 Tabela *tabela_pai=NULL;
 Papel *papel=NULL;
 map<QString, QString> atrib_priv, atributos;
 map<QString, QString>::iterator itr, itr_end;
 TipoObjetoBase tipo_obj;
 QString obj_pai, nome_obj;
 QStringList lista;
 unsigned i, tam, tipo_priv=Permissao::PRIV_SELECT;
 bool valor_priv, op_concessao;

 try
 {
  //Obtém os privilégios configurados para a permissão
  ParserXML::obterAtributosElemento(atrib_priv);

  /* Acessa o elemento filho <object> o qual armazena o objeto
     do modelo relacionado à permissão */
  ParserXML::salvarPosicao();
  ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
  ParserXML::obterAtributosElemento(atributos);

  //Obtém os atributos do objeto que é referenciado pela  permissão
  tipo_obj=obterTipoObjeto(atributos[AtributosParsers::TIPO]);
  nome_obj=atributos[AtributosParsers::NOME];
  obj_pai=atributos[AtributosParsers::OBJETO_PAI];

  //Caso o objeto seja uma coluna a mesma será obtida da tabela pai
  if(tipo_obj==OBJETO_COLUNA)
  {
   //Primeiramente a tabela pai é obtida do modelo
   tabela_pai=dynamic_cast<Tabela *>(obterObjeto(obj_pai, OBJETO_TABELA));

   /* Caso a tabela pai existe obtém o objeto filho da mesma
      o qual é referenciado pela permissão */
   if(tabela_pai)
    objeto=tabela_pai->obterColuna(nome_obj);
  }
  else if(tipo_obj==OBJETO_BANCO_DADOS)
  {
   objeto=this;
  }
  else
   /* Para os demais tipos de objetos, aceitos como referenciados
      por permissões, serão obtidos do modelo em si */
   objeto=obterObjeto(nome_obj, tipo_obj);

  /* Caso o objeto não exista será disparada uma exceção pois uma permissão
     não pode existir sem que referencie um objeto */
  if(!objeto)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_PERMREFOBJINEXISTE)
                          .arg(nome_obj)
                          .arg(ObjetoBase::obterNomeTipoObjeto(tipo_obj)),
                      ERR_PGMODELER_PERMREFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Aloca a permissão relacionando-a com o objeto localizado
  permissao=new Permissao(objeto);

  /* Acessa o elemento que armazena os privilégios dos papéis os quais
     compartilham da mesma permissão sobre o objeto. Informar estes
     papéis não é obrigatório de acordo com a DTD e com a especificação
     SQL para permissões */
  do
  {
   if(ParserXML::obterNomeElemento()==AtributosParsers::PAPEIS)
   {
    //Obtém os atributos do elemento <roles>, neste caso são names e reftype
    ParserXML::obterAtributosElemento(atributos);

    /* O atributo names armazena uma lista de nomes de papéis as quais a permissão
       referenciará. A lista tem os elementos separados por vírgula, sendo assim a
       string será quebrada usando o delimitador ',') */
    lista=atributos[AtributosParsers::NOMES].split(',');

    //Obtém a quantidade de nomes de papéis na lista
    tam=lista.size();

    //Varre a lista de nomes de papéis
    for(i=0; i < tam; i++)
    {
     //Tenta obter um papel do modelo cujo nome é o elemento atual da lista de nomes (lista[i])
     papel=dynamic_cast<Papel *>(obterObjeto(lista[i].trimmed(),OBJETO_PAPEL));

     /* Caso esse papel não exista um erro será disparado pois um novo papel
        não pode referenciar um outro papel que ainda nem foi criado */
     if(!papel)
     {
      //Dispara a exceção
      throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_PERMREFOBJINEXISTE)
                              .arg(objeto->obterNome())
                              .arg(objeto->obterNomeTipoObjeto())
                              .arg(lista[i])
                              .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_PAPEL)),
                     ERR_PGMODELER_REFOBJINEXISTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

     }
     //Adiciona o papel à permissão
     permissao->adicionarPapel(papel);
    }
   }
   else if(ParserXML::obterNomeElemento()==AtributosParsers::PRIVILEGIOS)
   {
    //Obtém os atributos do elemento <privileges>
    ParserXML::obterAtributosElemento(atrib_priv);

    //Atribui os privilégios à permissão recém criada
    itr=atrib_priv.begin();
    itr_end=atrib_priv.end();

    /* Varre o mapa de privilégios configurando-os na permissão caso
       estes estejam definidos no XML */
    while(itr!=itr_end)
    {
     if(itr->first!=AtributosParsers::OP_CONCESSAO)
     {
      //Obtém o valor do privilégio (true/false)
      valor_priv=(itr->second==AtributosParsers::VERDADEIRO);
      op_concessao=(itr->second==AtributosParsers::OP_CONCESSAO);

      //Identifica o tipo de privilégio atual
      if(itr->first==AtributosParsers::PRIV_CONNECT)
       tipo_priv=Permissao::PRIV_CONNECT;
      else if(itr->first==AtributosParsers::PRIV_CREATE)
       tipo_priv=Permissao::PRIV_CREATE;
      else if(itr->first==AtributosParsers::PRIV_DELETE)
       tipo_priv=Permissao::PRIV_DELETE;
      else if(itr->first==AtributosParsers::PRIV_EXECUTE)
       tipo_priv=Permissao::PRIV_EXECUTE;
      else if(itr->first==AtributosParsers::PRIV_INSERT)
       tipo_priv=Permissao::PRIV_INSERT;
      else if(itr->first==AtributosParsers::PRIV_REFERENCES)
       tipo_priv=Permissao::PRIV_REFERENCES;
      else if(itr->first==AtributosParsers::PRIV_SELECT)
       tipo_priv=Permissao::PRIV_SELECT;
      else if(itr->first==AtributosParsers::PRIV_TEMPORARY)
       tipo_priv=Permissao::PRIV_TEMPORARY;
      else if(itr->first==AtributosParsers::PRIV_TRIGGER)
       tipo_priv=Permissao::PRIV_TRIGGER;
      else if(itr->first==AtributosParsers::PRIV_TRUNCATE)
       tipo_priv=Permissao::PRIV_TRUNCATE;
      else if(itr->first==AtributosParsers::PRIV_UPDATE)
       tipo_priv=Permissao::PRIV_UPDATE;
      else if(itr->first==AtributosParsers::PRIV_USAGE)
       tipo_priv=Permissao::PRIV_USAGE;

      //Configura o privilégio na permissão
      permissao->definirPrivilegio(tipo_priv, (valor_priv || op_concessao), op_concessao);
     }
     itr++;
    }
   }
  }
  while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));

  ParserXML::restaurarPosicao();
 }
 catch(Excecao &e)
 {
  QString info_adicional;
  info_adicional=QString(QObject::trUtf8("%1 (linha: %2)")).arg(ParserXML::obterNomeArquivo())
                                   .arg(ParserXML::obterElementoAtual()->line);
  if(permissao) delete(permissao);

  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, info_adicional);
 }

 return(permissao);
}
//-----------------------------------------------------------
void ModeloBD::validarRemocaoColuna(Coluna *coluna)
{
 if(coluna && coluna->obterTabelaPai())
 {
  vector<ObjetoBase *> vet_refs;
  obterReferenciasObjeto(coluna, vet_refs);

  //Caso um objeto seja encontrado o qual referencia a coluna
  if(!vet_refs.empty())
   //Dispara um erro informando que a coluna não pde ser remove e qual objeto a referencia
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REMOBJREFERDIR)
                 .arg(coluna->obterTabelaPai()->obterNome(true) + "." + coluna->obterNome(true))
                 .arg(coluna->obterNomeTipoObjeto())
                 .arg(vet_refs[0]->obterNome(true))
                 .arg(vet_refs[0]->obterNomeTipoObjeto()),
                 ERR_PGMODELER_REMOBJREFERDIR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
}
//-----------------------------------------------------------
void ModeloBD::validarRelacObjetoTabela(ObjetoTabela *objeto, Tabela *tabela_pai)
{
 try
 {
  bool revalidar_rels=false, tab_ref_heranca=false;
  Relacionamento *rel=NULL;
  vector<ObjetoBase *>::iterator itr, itr_end;
  TipoObjetoBase tipo;

  if(objeto && tabela_pai)
  {
   tipo=objeto->obterTipoObjeto();

   /* Condição de revalidação de relacionamentos:
      > Caso seja uma coluna e a mesma é referenciada pela chave primária da tabela pai
      > Caso seja uma restrição e a mesma seja uma chave primária da tabela */
    revalidar_rels=((tipo==OBJETO_COLUNA &&
                     tabela_pai->restricaoReferenciaColuna(dynamic_cast<Coluna *>(objeto), TipoRestricao::primary_key)) ||
                    (tipo==OBJETO_RESTRICAO &&
                     dynamic_cast<Restricao *>(objeto)->obterTipoRestricao()==TipoRestricao::primary_key));

   /* Caso seja uma coluna, verfica se a tabela pai participa de um relacionamento
     de generalização como tabela de destino (aquela que tem suas colunas copiadas
     para a tabela qua a herda) */
   if(tipo==OBJETO_COLUNA)
   {
    itr=relacionamentos.begin();
    itr_end=relacionamentos.end();

    while(itr!=itr_end && !tab_ref_heranca)
    {
     rel=dynamic_cast<Relacionamento *>(*itr);
     itr++;
     tab_ref_heranca=(rel->obterTipoRelacionamento()==Relacionamento::RELACIONAMENTO_GEN &&
                      rel->obterTabelaReferencia()==tabela_pai);
    }
   }

   //Caso as duas condições acima sejam atendidas
   if(revalidar_rels || tab_ref_heranca)
   {
    //(Re)valida os relacionamento e os reconecta
    desconectarRelacionamentos();
    validarRelacionamentos();
   }
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
QString ModeloBD::__obterDefinicaoObjeto(unsigned tipo_def)
{
 if(lim_conexao >= 0)
  atributos[AtributosParsers::LIMITE_CONEXAO]=QString("%1").arg(lim_conexao);

 if(tipo_def==ParserEsquema::DEFINICAO_SQL)
 {
  atributos[AtributosParsers::CODIFICACAO]="'" + (~tipo_codif) + "'";

  if(!localizacoes[1].isEmpty())
   atributos[AtributosParsers::LC_COLLATE_BD]="'" + localizacoes[1] + "'";

  if(!localizacoes[0].isEmpty())
   atributos[AtributosParsers::LC_CTYPE_BD]="'" + localizacoes[0]  + "'";
 }
 else
 {
  atributos[AtributosParsers::CODIFICACAO]=(~tipo_codif);
  atributos[AtributosParsers::LC_COLLATE_BD]=localizacoes[1];
  atributos[AtributosParsers::LC_CTYPE_BD]=localizacoes[0];
 }

 atributos[AtributosParsers::BD_MODELO]=bd_modelo;
 return(this->ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//-----------------------------------------------------------
QString ModeloBD::obterDefinicaoObjeto(unsigned tipo_def)
{
 return(this->obterDefinicaoObjeto(tipo_def, true));
}
//-----------------------------------------------------------
QString ModeloBD::obterDefinicaoObjeto(unsigned tipo_def, bool exportar_arq)
{
 map<QString, QString> atribs_aux;
 unsigned qtd1, i, qtd;
 float qtd_geral_obj, qtd_defs_geradas;
 ObjetoBase *objeto=NULL;
 vector<ObjetoBase *> *lista_obj=NULL;
 vector<ObjetoBase *>::iterator itr, itr_end;
 vector<unsigned>::iterator itr1, itr1_end;
 QString msg=trUtf8("Gerando %1 do objeto: %2 (%3)"),
         atrib=AtributosParsers::OBJETOS,
         tipo_def_str=(tipo_def==ParserEsquema::DEFINICAO_SQL ? "SQL" : "XML");
 Tipo *tipo_usr=NULL;
 map<unsigned, ObjetoBase *> mapa_objetos;
 vector<unsigned> vet_id_objs;
 Tabela *tabela=NULL;
 Indice *indice=NULL;
 Gatilho *gatilho=NULL;
 Restricao *restricao=NULL;
 Relacionamento *relacao=NULL;
 TipoObjetoBase tipo_obj,
                tipos_obj_aux[]={ OBJETO_PAPEL, OBJETO_ESPACO_TABELA, OBJETO_ESQUEMA },
                tipos_obj[]={ OBJETO_LINGUAGEM, OBJETO_FUNCAO, OBJETO_TIPO,
                              OBJETO_CONV_TIPO, OBJETO_CONV_CODIFICACAO,
                              OBJETO_OPERADOR, OBJETO_FAMILIA_OPER, OBJETO_CLASSE_OPER,
                              OBJETO_FUNC_AGREGACAO, OBJETO_DOMINIO, OBJETO_CAIXA_TEXTO, OBJETO_RELACAO_BASE,
                              OBJETO_RELACAO, OBJETO_TABELA, OBJETO_VISAO, OBJETO_SEQUENCIA };
 try
 {
  qtd_geral_obj=this->obterNumObjetos();
  qtd_defs_geradas=0;

  /* Tratandos os objetos os quais tem ids fixos são eles: Papel, Espaço de Tabela,
     e Esquema. Estes precisam ser tratados separadamente no laço abaixo pois não
     entram na ordenação de ids executada para os demais tipos de objetos. */
  for(i=0; i < 3; i++)
  {
   //Obtém a lista de objetos do tipo
   lista_obj=obterListaObjetos(tipos_obj_aux[i]);

   /* Caso o tipo de definição seja SQL obtem o nome do atributo
      do tipo do objeto nos esquema SQL */
   if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   {
    atrib=ObjetoBase::esq_objetos[tipos_obj_aux[i]];
    atribs_aux[atrib]="";
   }

   //Obtém o início e o fim da lista para uma varredura
   itr=lista_obj->begin();
   itr_end=lista_obj->end();

   while(itr!=itr_end)
   {
    //Obtém o objeto atual
    objeto=(*itr);

    if(objeto->obterTipoObjeto()!=OBJETO_ESQUEMA ||
       (objeto->obterTipoObjeto()==OBJETO_ESQUEMA &&
        objeto->obterNome()!="public"))
    {
     //Gera o codigo e o concatena com os demais já gerados
     atribs_aux[atrib]+=validarDefinicaoObjeto(objeto, tipo_def);
     //Dispara um sinal para sinalizar o progresso da geração do códgio
     qtd_defs_geradas++;
     if(!signalsBlocked())
     {
      emit s_objetoCarregado((qtd_defs_geradas/qtd_geral_obj) * 100,
                             msg.arg(tipo_def_str)
                               .arg(QString::fromUtf8(objeto->obterNome()))
                               .arg(objeto->obterNomeTipoObjeto()),
                            objeto->obterTipoObjeto());
     }
    }
    itr++;
   }
  }

  /* Armazena os próprio objeto de modelo de objetos pois o mesmo também precisa estar na
     ordenação de objetos para ser criado na ordem correta quando o parser xml ler
     a definição */
  mapa_objetos[this->obterIdObjeto()]=this;
  vet_id_objs.push_back(this->obterIdObjeto());


  /* Armazenando os demais tipos de objetos no mapa de objetos para
     serem ordenados pelos ids. */
  /* Para definição XML todos os 16 tipos da lista 'tipos_objs' são tratados.
     Para definição SQL são tratados apenas os 12 primeiros (tabelas, relacionamentos, visões e sequências)
     são tratadas separadamente pois existe uma ordem específica em que elas precisam ser criadas e essa
     ordem é definina na interação após a ordenação dos objetos */
  if(tipo_def==ParserEsquema::DEFINICAO_XML)
   qtd=16;
  else
   qtd=12;

  for(i=0; i < qtd; i++)
  {
   /* Seleciona a lista de objetos para obter os identificadores e armazenar seus
      elementos no mapa de objetos. No caso de definição SQL apenas os  objetos
      caixa de texto e relacionamento tabela-visão não são obtidos pois os mesmos
      não possuem código SQL */
   if(tipo_def==ParserEsquema::DEFINICAO_SQL &&
      (tipos_obj[i]==OBJETO_CAIXA_TEXTO || tipos_obj[i]==OBJETO_RELACAO_BASE))
    lista_obj=NULL;
   else
    //Obtém a lista de objeto de acorodo com o tipo
    lista_obj=obterListaObjetos(tipos_obj[i]);

   if(lista_obj)
   {
    //Obtém o início e o fim da lista para uma varredura
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    while(itr!=itr_end)
    {
     //Obtém o objeto atual
     objeto=(*itr);
     //Armazena o objeto em si no mapa de objetos
     mapa_objetos[objeto->obterIdObjeto()]=objeto;
     //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
     vet_id_objs.push_back(objeto->obterIdObjeto());
     itr++;
    }
   }
  }

  /* Caso a definição seja XML executa um trecho especial que é
     a obtenção e armazenamento dos objetos especiais (os quais
     referenciam colunas de tabelas adicionadas por relacionamentos)
     no mapa de objetos. Para isso a lista de tabelas é varrida novamente
     e suas restrições e índices são validados como especiais ou não */
  if(tipo_def==ParserEsquema::DEFINICAO_XML)
  {
   itr=tabelas.begin();
   itr_end=tabelas.end();

   while(itr!=itr_end)
   {
    tabela=dynamic_cast<Tabela *>(*itr);
    itr++;

    //Varre a lista de restrições da tabela
    qtd=tabela->obterNumRestricoes();
    for(i=0; i < qtd; i++)
    {
     restricao=tabela->obterRestricao(i);

     //Caso a restrição seja um objeto especial armazena o mesmo no mapa de objetos
     if(!restricao->incluidoPorLigacao() &&
         restricao->obterTipoRestricao()!=TipoRestricao::primary_key &&
         restricao->referenciaColunaIncRelacao())
     {
      //Armazena o objeto em si no mapa de objetos
      mapa_objetos[restricao->obterIdObjeto()]=restricao;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      vet_id_objs.push_back(restricao->obterIdObjeto());
     }
    }

    //Varre a lista de gatilhos da tabela
    qtd=tabela->obterNumGatilhos();
    for(i=0; i < qtd; i++)
    {
     gatilho=tabela->obterGatilho(i);

     //Caso o gatilho seja um objeto especial armazena-o no mapa de objetos
     if(gatilho->referenciaColunaIncRelacao())
     {
      //Armazena o objeto em si no mapa de objetos
      mapa_objetos[gatilho->obterIdObjeto()]=gatilho;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      vet_id_objs.push_back(gatilho->obterIdObjeto());
     }
    }

    //Varre a lista de índices da tabela
    qtd=tabela->obterNumIndices();
    for(i=0; i < qtd; i++)
    {
     indice=tabela->obterIndice(i);

     //Caso o índice seja um objeto especial armazena-o no mapa de objetos
     if(indice->referenciaColunaIncRelacao())
     {
      //Armazena o objeto em si no mapa de objetos
      mapa_objetos[indice->obterIdObjeto()]=indice;
      //Armazena o id do objeto na lista de ids usada para referenciar os objetos no mapa
      vet_id_objs.push_back(indice->obterIdObjeto());
     }
    }
   }
  }

  //Ordena o vetor de identificadores em ordem crescente
  sort(vet_id_objs.begin(), vet_id_objs.end());

  /* CASO ESPECIAL: Gerando a SQL de tabelas, visões, relacionamentos e sequencias de forma ordenada.

     Esta geração é feita da seguinte forma:
     1) Com a lista de relacionamentos, gera o SQL de ambas as tabelas do relacionametno e logo em seguida a
        SQL do relacionametno em si.
     2) Para as demais tabelas que não participaram de relacionamentos, gerar o SQL de cada uma.
     3) As sequencias devem ter seus SQL gerados após as tabelas restantes
     4) Visões devem ser as últimas a terem o SQL gerado para não terem referências a colunas e tabelas quebradas
  */
  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
  {
   ObjetoBase *objetos[3]={NULL, NULL, NULL};
   vector<ObjetoBase *> vet_aux;
   qtd=16;

   vet_aux=relacionamentos;
   vet_aux.insert(vet_aux.end(), tabelas.begin(),tabelas.end());
   vet_aux.insert(vet_aux.end(), sequencias.begin(),sequencias.end());
   vet_aux.insert(vet_aux.end(), visoes.begin(),visoes.end());;
   itr=vet_aux.begin();
   itr_end=vet_aux.end();

   while(itr!=itr_end)
   {
    objeto=(*itr);
    itr++;

    if(objeto->obterTipoObjeto()==OBJETO_RELACAO)
    {
     relacao=dynamic_cast<Relacionamento *>(objeto);
     objetos[0]=relacao->obterTabela(Relacionamento::TABELA_ORIGEM);
     objetos[1]=relacao->obterTabela(Relacionamento::TABELA_DESTINO);
     objetos[2]=relacao;

     for(i=0; i < 3; i++)
     {
      if(mapa_objetos.count(objetos[i]->obterIdObjeto())==0)
      {
       mapa_objetos[objetos[i]->obterIdObjeto()]=objetos[i];
       vet_id_objs.push_back(objetos[i]->obterIdObjeto());
      }
     }
    }
    else
    {
     if(mapa_objetos.count(objeto->obterIdObjeto())==0)
     {
      mapa_objetos[objeto->obterIdObjeto()]=objeto;
      vet_id_objs.push_back(objeto->obterIdObjeto());
     }
    }
   }
  }

  atribs_aux[AtributosParsers::TIPOS_SHELL]="";

  /* Caso a definição seja SQL e existam tipos definidos pelo usuário
     faz a conversão dos parâmetros das funções usadas internamente
     por estes */
  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
  {
   itr=tipos.begin();
   itr_end=tipos.end();

   while(itr!=itr_end)
   {
    tipo_usr=dynamic_cast<Tipo *>(*itr);
    itr++;

    if(tipo_usr->obterConfiguracao()==Tipo::TIPO_BASE)
     tipo_usr->converterParametrosFuncoes();
   }
  }

  //Varre a lista de ids de objetos para obtê-los do mapa de objetos
  itr1=vet_id_objs.begin();
  itr1_end=vet_id_objs.end();

  atrib=AtributosParsers::OBJETOS;
  while(itr1!=itr1_end)
  {
   /* Obtém o objeto do mapa a partir do seu identificador
      armazenado no iterador atual */
   objeto=mapa_objetos[(*itr1)];

   //Obtém o tipo do objeto
   tipo_obj=objeto->obterTipoObjeto();
   itr1++;

   /* Caso seja um objeto tipo e a definição seja SQL armazena a
      definição shell dos tipos os quais são declarados antes da criação
      dos tipos definidos pelo usuário */
   if(tipo_obj==OBJETO_TIPO && tipo_def==ParserEsquema::DEFINICAO_SQL)
   {
    tipo_usr=dynamic_cast<Tipo *>(objeto);

    //Obtendo a definição do tipo em forma de shell type
    if(tipo_usr->obterConfiguracao()==Tipo::TIPO_BASE)
     atribs_aux[AtributosParsers::TIPOS_SHELL]+=tipo_usr->obterDefinicaoObjeto(tipo_def, true);
    else
     atribs_aux[atrib]+=tipo_usr->obterDefinicaoObjeto(tipo_def);
   }
   else if(tipo_obj==OBJETO_BANCO_DADOS)
   {
    if(tipo_def==ParserEsquema::DEFINICAO_SQL)
     atribs_aux[this->obterNomeEsquemaObjeto()]+=this->__obterDefinicaoObjeto(tipo_def);
    else
     atribs_aux[atrib]+=this->__obterDefinicaoObjeto(tipo_def);
   }
   else
   {
    /* Desprezando as linguagens c e sql
       pois as mesmas não precisam ser declaradas explicitamente poir serem built-in */
    if(//tipo_def==ParserEsquema::DEFINICAO_SQL &&
       (tipo_obj==OBJETO_LINGUAGEM &&
        (objeto->obterNome()==~TipoLinguagem("c") ||
         objeto->obterNome()==~TipoLinguagem("sql") ||
         objeto->obterNome()==~TipoLinguagem("plpgsql"))))
     atribs_aux[atrib]+="";
    else
     atribs_aux[atrib]+=validarDefinicaoObjeto(objeto, tipo_def);
   }

   //Dispara um sinal para sinalizar o progresso da geração do códgio
   qtd_defs_geradas++;
   if(!signalsBlocked())
   {
    emit s_objetoCarregado((qtd_defs_geradas/qtd_geral_obj) * 100,
                           msg.arg(tipo_def_str)
                              .arg(QString::fromUtf8(objeto->obterNome()))
                              .arg(objeto->obterNomeTipoObjeto()),
                           objeto->obterTipoObjeto());
   }
  }

  //Gerando a definição sql/xml das permissões
  itr=permissoes.begin();
  itr_end=permissoes.end();
  atribs_aux[AtributosParsers::PERMISSAO]="";

  while(itr!=itr_end)
  {
   atribs_aux[AtributosParsers::PERMISSAO]+=dynamic_cast<Permissao *>(*itr)->obterDefinicaoObjeto(tipo_def);

   //Dispara um sinal para sinalizar o progresso final da geração de código
   qtd_defs_geradas++;
   if(!signalsBlocked())
   {
    emit s_objetoCarregado((qtd_defs_geradas/qtd_geral_obj) * 100,
                           msg.arg(tipo_def_str)
                              .arg(QString::fromUtf8((*itr)->obterNome()))
                              .arg(objeto->obterNomeTipoObjeto()),
                           objeto->obterTipoObjeto());
   }

   itr++;
  }

  //Configura os atributos específicos do modelo de banco
  atribs_aux[AtributosParsers::AUTOR_MODELO]=autor;

  if(tipo_def==ParserEsquema::DEFINICAO_XML)
  {
   atribs_aux[AtributosParsers::PROTEGIDO]=(this->protegido ? "1" : "");
  }
  else
  {
   /* Ao final da obtenção da definição SQL do modelo, faz a conversão inversa
      dos parâmetros das funções usadas pelos tipos base */
   qtd1=tipos.size();
   for(i=0; i < qtd1; i++)
   {
    tipo_usr=dynamic_cast<Tipo *>(tipos[i]);
    if(tipo_usr->obterConfiguracao()==Tipo::TIPO_BASE)
    {
     atribs_aux[atrib]+=tipo_usr->obterDefinicaoObjeto(tipo_def);
     tipo_usr->converterParametrosFuncoes(true);
    }
   }
  }
 }
 catch(Excecao &e)
 {
  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
  {
   /* Em caso de erro na geração da definição SQL do modelo, faz a conversão inversa
     dos parâmetros das funções usadas pelos tipos base */
   qtd1=tipos.size();
   for(i=0; i < qtd1; i++)
   {
    tipo_usr=dynamic_cast<Tipo *>(tipos[i]);
    if(tipo_usr->obterConfiguracao()==Tipo::TIPO_BASE)
     tipo_usr->converterParametrosFuncoes(true);
   }
  }
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }

 //Armazena o atributo que indica se a exportação é para arquivo ou não
 atribs_aux[AtributosParsers::EXPORTAR_ARQ]=(exportar_arq ? "1" : "");

 //Retorna a definição do modelo completa
 return(ParserEsquema::obterDefinicaoObjeto(AtributosParsers::MODELO_BD, atribs_aux, tipo_def));
}
//-----------------------------------------------------------
void ModeloBD::salvarModelo(const QString &nome_arq, unsigned tipo_def)
{
 QString str_aux;
 QFile saida(nome_arq);

 saida.open(QFile::WriteOnly);

 //Caso não consiga abrir o arquivo para gravação
 if(!saida.isOpen())
 {
  str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_ARQNAOGRAVADO).arg(nome_arq));
  throw Excecao(str_aux,ERR_PGMODELER_ARQNAOGRAVADO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 try
 {
  str_aux=this->obterDefinicaoObjeto(tipo_def);
  saida.write(str_aux.toStdString().c_str(),str_aux.size());
  saida.close();
 }
 catch(Excecao &e)
 {
  if(saida.isOpen()) saida.close();
  str_aux=QString(Excecao::obterMensagemErro(ERR_PGMODELER_ARQNAOGRAVADODEFINV).arg(nome_arq));
  throw Excecao(str_aux,ERR_PGMODELER_ARQNAOGRAVADODEFINV,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void ModeloBD::obterDependenciasObjeto(ObjetoBase *objeto, vector<ObjetoBase *> &vet_deps, bool inc_dep_indiretas)
{
 //Caso o objeto esteja alocado e o mesmo ainda não exista na lista de dependências
 if(objeto && std::find(vet_deps.begin(), vet_deps.end(), objeto)==vet_deps.end())
 {
  //Adiciona o objeto na lista de dependências
  vet_deps.push_back(objeto);
 }

 if((vet_deps.size()==1 && !inc_dep_indiretas) || inc_dep_indiretas)
 {
  TipoObjetoBase tipo_obj=objeto->obterTipoObjeto();

  /* Caso o objeto possua esquema, espaço de tabela e dono,
     busca e inclui se necessário as dependências desses
     objetos na lista */
  if(objeto->obterEsquema() && inc_dep_indiretas)
   obterDependenciasObjeto(objeto->obterEsquema(), vet_deps, inc_dep_indiretas);

  if(objeto->obterEspacoTabela() && inc_dep_indiretas)
   obterDependenciasObjeto(objeto->obterEspacoTabela(), vet_deps, inc_dep_indiretas);

  if(objeto->obterDono()  && inc_dep_indiretas)
   obterDependenciasObjeto(objeto->obterDono(), vet_deps, inc_dep_indiretas);

  //** Obtendo as dependências de Classe de Operadores **
  if(tipo_obj==OBJETO_CLASSE_OPER)
  {
   ClasseOperadores *classe_op=dynamic_cast<ClasseOperadores *>(objeto);

   /* Obtém a referência para o tipo de dado da classe, caso um ponteiro válido seja retornado
      indica que o tipo de dado é um definido pelo usuário (classe Tipo) e que este precisa
      também ter as dependências obtidas */
  ObjetoBase *tipo_usr=obterObjeto(*classe_op->obterTipoDado(), OBJETO_TIPO);

   if(tipo_usr)
    obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);

   //Caso haja uma família de operadores obtém as dependências dela também
   if(classe_op->obterFamilia())
    obterDependenciasObjeto(classe_op->obterFamilia(), vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Domínios **
  else if(tipo_obj==OBJETO_DOMINIO)
  {
   /* Obtém a referência para o tipo de dado do domínio, caso um ponteiro válido seja retornado
      indica que o tipo de dado é um definido pelo usuário (classe Tipo) e que este precisa
      também ter as dependências obtidas */
   ObjetoBase *tipo_usr=obterObjeto(*dynamic_cast<Dominio *>(objeto)->obterTipo(), OBJETO_TIPO);

   if(tipo_usr)
    obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Conversões de Codificação **
  else if(tipo_obj==OBJETO_CONV_CODIFICACAO)
  {
   //Obtém as dependências da função de conversão que define a conversão de codificação
   Funcao *func=dynamic_cast<ConversaoCodificacao *>(objeto)->obterFuncaoConversao();
   obterDependenciasObjeto(func, vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Conversões de Tipo **
  else if(tipo_obj==OBJETO_CONV_TIPO)
  {
   ConversaoTipo *conv=dynamic_cast<ConversaoTipo *>(objeto);
   ObjetoBase *tipo_usr=NULL;

   //Obtém as dependências dos tipos usados na conversão de tipo
   for(unsigned i=ConversaoTipo::CONV_TIPO_ORIGEM; i <= ConversaoTipo::CONV_TIPO_DESTINO; i++)
   {
    tipo_usr=obterObjeto(*conv->obterTipoDado(i), OBJETO_TIPO);
    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências da função de conversão que define a conversão de tipo
   obterDependenciasObjeto(conv->obterFuncaoConversao(), vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Funções **
  else if(tipo_obj==OBJETO_FUNCAO)
  {
   Funcao *func=dynamic_cast<Funcao *>(objeto);
   ObjetoBase *tipo_usr=obterObjeto(*func->obterTipoRetorno(), OBJETO_TIPO);
   unsigned qtd, i;

   //Caso a linguagem da função não seja C ou SQL obtém as dependências da mesma
   if(func->obterLinguagem()->obterNome()!=~TipoLinguagem("c") &&
      func->obterLinguagem()->obterNome()!=~TipoLinguagem("sql"))
    obterDependenciasObjeto(func->obterLinguagem(), vet_deps, inc_dep_indiretas);

   //Obtém as dependências do tipo de retorno caso o mesmo seja um tipo definido pelo usuário
   if(tipo_usr)
    obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);

   //Obtém as dependências dos tipos dos parâmetros
   qtd=func->obterNumParams();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=obterObjeto(*func->obterParametro(i).obterTipo(), OBJETO_TIPO);

    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências dos elementos da tabela de retorno
   qtd=func->obterNumTiposRetTabela();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=obterObjeto(*func->obterTipoRetTabela(i).obterTipo(), OBJETO_TIPO);

    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Funções de Agregação **
  else if(tipo_obj==OBJETO_FUNC_AGREGACAO)
  {
   FuncaoAgregacao *func=dynamic_cast<FuncaoAgregacao *>(objeto);
   ObjetoBase *tipo_usr=NULL;
   unsigned qtd, i;

   //Obtém as dependências das funções que definem a função de agregação
   for(i=FuncaoAgregacao::FUNCAO_FINAL; i <= FuncaoAgregacao::FUNCAO_TRANSICAO; i++)
    obterDependenciasObjeto(func->obterFuncao(i), vet_deps, inc_dep_indiretas);

   //Obtém a dependência do tipo de estado da função de agregação
   tipo_usr=obterObjeto(*func->obterTipoEstado(), OBJETO_TIPO);

   if(tipo_usr)
    obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);

   //Obtém as dependências do operador de ordenação caso este esteja alocado
   if(func->obterOperadorOrdenacao())
    obterDependenciasObjeto(func->obterOperadorOrdenacao(), vet_deps, inc_dep_indiretas);

   //Obtém as dependências dos tipos de dados usados na função de agregação
   qtd=func->obterNumTipoDados();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=obterObjeto(*func->obterTipoDado(i), OBJETO_TIPO);

    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Linguagens **
  else if(tipo_obj==OBJETO_LINGUAGEM)
  {
   Linguagem *ling=dynamic_cast<Linguagem *>(objeto);

   for(unsigned i=Linguagem::FUNC_VALIDATOR; i <= Linguagem::FUNC_INLINE; i++)
   {
    if(ling->obterFuncao(i))
     obterDependenciasObjeto(ling->obterFuncao(i), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Operadores **
  else if(tipo_obj==OBJETO_OPERADOR)
  {
   Operador *oper=dynamic_cast<Operador *>(objeto);
   ObjetoBase *tipo_usr=NULL;
   unsigned i;

   //Obtém as dependências das funções usadas pelo operador
   for(i=Operador::FUNC_OPERADOR; i <= Operador::FUNC_RESTRICAO; i++)
   {
    if(oper->obterFuncao(i))
     obterDependenciasObjeto(oper->obterFuncao(i), vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências dos tipos dos argumentos do operador
   for(i=Operador::ARG_ESQUERDA; i <= Operador::ARG_DIREITA; i++)
   {
    tipo_usr=obterObjeto(*oper->obterTipoDadoArgumento(i), OBJETO_TIPO);
    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências dos operadores auxiliares
   for(i=Operador::OPER_COMUTACAO; i <= Operador::OPER_MAIOR; i++)
   {
    if(oper->obterOperador(i))
     obterDependenciasObjeto(oper->obterOperador(i), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Papéis **
  else if(tipo_obj==OBJETO_PAPEL)
  {
   Papel *papel=dynamic_cast<Papel *>(objeto);
   unsigned i, i1, qtd,
            tipos[3]={ Papel::PAPEL_REF, Papel::PAPEL_MEMBRO, Papel::PAPEL_ADMIN };

   //Obtém as dependências dos papéis membros, papéis admins e papéis referenciados
   for(i=0; i < 3; i++)
   {
    qtd=papel->obterNumPapeis(tipos[i]);
    for(i1=0; i1 < qtd; i1++)
     obterDependenciasObjeto(papel->obterPapel(tipos[i], i1), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Relacionamentos **
  else if(tipo_obj==OBJETO_RELACAO)
  {
   Relacionamento *rel=dynamic_cast<Relacionamento *>(objeto);
   ObjetoBase *tipo_usr=NULL;
   Restricao *rest=NULL;
   unsigned i, qtd;

   //Obtém as dependências das tabelas referenciadas pelo relacionamento
   obterDependenciasObjeto(rel->obterTabela(Relacionamento::TABELA_ORIGEM), vet_deps, inc_dep_indiretas);
   obterDependenciasObjeto(rel->obterTabela(Relacionamento::TABELA_DESTINO), vet_deps, inc_dep_indiretas);

   //Obtém as dependências dos tipos usados pelos atributos do relacionamento
   qtd=rel->obterNumAtributos();
   for(i=0; i < qtd; i++)
   {
    tipo_usr=obterObjeto(*rel->obterAtributo(i)->obterTipo(), OBJETO_TIPO);
    if(tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das restrições (somente chave estrangeira) do relacionamento
   qtd=rel->obterNumRestricoes();
   for(i=0; i < qtd; i++)
   {
    rest=dynamic_cast<Restricao *>(rel->obterRestricao(i));
    if(rest->obterTipoRestricao()==TipoRestricao::foreign_key)
     obterDependenciasObjeto(rest->obterTabReferenciada(), vet_deps, inc_dep_indiretas);

    if(rest->obterEspacoTabela())
     obterDependenciasObjeto(rest->obterEspacoTabela(), vet_deps, inc_dep_indiretas);
   }
  }
  //** Obtendo as dependências de Sequências **
  else if(tipo_obj==OBJETO_SEQUENCIA)
  {
   Sequencia *seq=dynamic_cast<Sequencia *>(objeto);
   if(seq->obterPossuidora())
    obterDependenciasObjeto(seq->obterPossuidora()->obterTabelaPai(), vet_deps, inc_dep_indiretas);
  }
  //** Obtendo as dependências de Tabelas **
  else if(tipo_obj==OBJETO_TABELA)
  {
   Tabela *tab=dynamic_cast<Tabela *>(objeto);
   ObjetoBase *tipo_usr=NULL;
   Restricao *rest=NULL;
   Gatilho *gat=NULL;
   Indice *ind=NULL;
   Coluna *col=NULL;
   unsigned qtd, qtd1, i, i1;

   //Obtém as dependências dos tipos das colunas não incluídas por relacionamento
   qtd=tab->obterNumColunas();
   for(i=0; i < qtd; i++)
   {
    col=tab->obterColuna(i);
    tipo_usr=obterObjeto(*col->obterTipo(), OBJETO_TIPO);
    if(!col->incluidoPorLigacao() && tipo_usr)
     obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das restrições não incluídas por relacionamento
   qtd=tab->obterNumRestricoes();
   for(i=0; i < qtd; i++)
   {
    rest=dynamic_cast<Restricao *>(tab->obterRestricao(i));
    if(!rest->incluidoPorLigacao() &&
        rest->obterTipoRestricao()==TipoRestricao::foreign_key)
     obterDependenciasObjeto(rest->obterTabReferenciada(), vet_deps, inc_dep_indiretas);

    if(!rest->incluidoPorLigacao() && rest->obterEspacoTabela())
     obterDependenciasObjeto(rest->obterEspacoTabela(), vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das tabelas referenciadas nos gatilhos e as funções
   qtd=tab->obterNumGatilhos();
   for(i=0; i < qtd; i++)
   {
    gat=dynamic_cast<Gatilho *>(tab->obterGatilho(i));
    if(gat->obterTabReferenciada())
     obterDependenciasObjeto(gat->obterTabReferenciada(), vet_deps, inc_dep_indiretas);

    if(gat->obterFuncao())
     obterDependenciasObjeto(gat->obterFuncao(), vet_deps, inc_dep_indiretas);

    //qtd1=gat->obterNumColunas();
    //for(i1=0; i1 < qtd1; i1++)
    //obterDependenciasObjeto(gat->obterColuna(i1), vet_deps, inc_dep_indiretas);
   }

   //Obtém as dependências das colunas ou classe de operadores usados nos elementos do índices
   qtd=tab->obterNumIndices();
   for(i=0; i < qtd; i++)
   {
    ind=dynamic_cast<Indice *>(tab->obterIndice(i));
    qtd1=ind->obterNumElementos();

    for(i1=0; i1 < qtd1; i1++)
    {
     if(ind->obterElemento(i1).obterClasseOperadores())
      obterDependenciasObjeto(ind->obterElemento(i1).obterClasseOperadores(), vet_deps, inc_dep_indiretas);
     else if(ind->obterElemento(i1).obterColuna())
     {
      tipo_usr=obterObjeto(*ind->obterElemento(i1).obterColuna()->obterTipo(), OBJETO_TIPO);
      if(tipo_usr)
       obterDependenciasObjeto(tipo_usr, vet_deps, inc_dep_indiretas);
     }
    }
   }
  }
  //** Obtendo as dependências de Tipos definidos pelo usuário **
  else if(tipo_obj==OBJETO_TIPO)
  {
   Tipo *tipo_usr=dynamic_cast<Tipo *>(objeto);
   ObjetoBase *tipo_aux=NULL;
   unsigned qtd, i;

   //Caso o tipo de usuário seja tipo base
   if(tipo_usr->obterConfiguracao()==Tipo::TIPO_BASE)
   {
    //Obtém as dependências do tipo de cópia
    tipo_aux=obterObjeto(*tipo_usr->obterTipoCopia(), OBJETO_TIPO);

    if(tipo_aux)
     obterDependenciasObjeto(tipo_aux, vet_deps, inc_dep_indiretas);

    //Obtém as dependências das funções usadas pelo tipo
    for(i=Tipo::FUNCAO_INPUT; i <= Tipo::FUNCAO_ANALYZE; i++)
     obterDependenciasObjeto(tipo_usr->obterFuncao(i), vet_deps, inc_dep_indiretas);
   }
   //Caso seja um tipo composto
   else if(tipo_usr->obterConfiguracao()==Tipo::TIPO_COMPOSTO)
   {
    //Obtém as dependências dos tipos dos atributos
    qtd=tipo_usr->obterNumAtributos();
    for(i=0; i < qtd; i++)
    {
     tipo_aux=obterObjeto(*tipo_usr->obterAtributo(i).obterTipo(), OBJETO_TIPO);

     if(tipo_aux)
      obterDependenciasObjeto(tipo_aux, vet_deps, inc_dep_indiretas);
    }
   }
  }
  //** Obtendo as dependências de Visões **
  else if(tipo_obj==OBJETO_VISAO)
  {
   Visao *visao=dynamic_cast<Visao *>(objeto);
   unsigned i, qtd;

   //Obtém as dependências das tabelas referenciadas pela visão
   qtd=visao->obterNumReferencias();
   for(i=0; i < qtd; i++)
   {
    if(visao->obterReferencia(i).obterTabela())
     obterDependenciasObjeto(visao->obterReferencia(i).obterTabela(), vet_deps, inc_dep_indiretas);
   }
  }
 }
}
//-----------------------------------------------------------
void ModeloBD::obterReferenciasObjeto(ObjetoBase *objeto, vector<ObjetoBase *> &vet_refs, bool modo_exclusao)
{
 vet_refs.clear();

 if(objeto)
 {
  TipoObjetoBase tipo_obj=objeto->obterTipoObjeto();
  bool refer=false;

  if(tipo_obj==OBJETO_TABELA)
  {
   Tabela *tabela=dynamic_cast<Tabela *>(objeto);
   Sequencia *seq=NULL;
   Restricao *rest=NULL;
   Tabela *tab=NULL;
   Gatilho *gat=NULL;
   RelacionamentoBase *rel_base=NULL;
   vector<ObjetoBase *>::iterator itr, itr_end;
   unsigned i, qtd;

   /* Vericando se existe algum relacionamento (tabela-tabela)
      o qual um dos objetos participantes é a tabela
      do parâmetro */
   itr=relacionamentos.begin();
   itr_end=relacionamentos.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    rel_base=dynamic_cast<RelacionamentoBase *>(*itr);
    if(rel_base->obterTabela(RelacionamentoBase::TABELA_ORIGEM)==tabela ||
       rel_base->obterTabela(RelacionamentoBase::TABELA_DESTINO)==tabela)
    {
     refer=true;
     vet_refs.push_back(rel_base);
    }
    itr++;
   }

   /* Vericando se existe alguma sequência a qual referencia
      a tabela do parâmetro */
   itr=sequencias.begin();
   itr_end=sequencias.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    seq=dynamic_cast<Sequencia *>(*itr);
    if(seq->obterPossuidora() &&
       seq->obterPossuidora()->obterTabelaPai()==tabela)
    {
     refer=true;
     vet_refs.push_back(seq);
    }

    itr++;
   }

   /* Vericando se existe alguma tabela cuja uma tabela cópia é
      a própria tabela a ser excluída, verifica também se as
      restrições das tabelas, especificamente, as chaves estrangeiras
      estão referenciado a tabela */
   itr=tabelas.begin();
   itr_end=tabelas.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    tab=dynamic_cast<Tabela *>(*itr);

    //Verificando as restrições
    qtd=tab->obterNumRestricoes();
    for(i=0; i < qtd&& (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     rest=tab->obterRestricao(i);
     if(rest->obterTipoRestricao()==TipoRestricao::foreign_key &&
        rest->obterTabReferenciada()==tabela)
     {
      refer=true;
      vet_refs.push_back(rest);
     }
    }

    //Verificando os gatilhos
    qtd=tab->obterNumGatilhos();
    for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     gat=tab->obterGatilho(i);
     if(gat->obterTabReferenciada()==tabela)
     {
      refer=true;
      vet_refs.push_back(gat);
     }
    }

    itr++;
   }

   /* Vericando se existe algum relacionamento (tabela-visao)
      o qual um dos objetos participantes é a tabela */
   itr=relac_visoes.begin();
   itr_end=relac_visoes.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    rel_base=dynamic_cast<RelacionamentoBase *>(*itr);
    if(rel_base->obterTabela(RelacionamentoBase::TABELA_ORIGEM)==tabela)
    {
     refer=true;
     vet_refs.push_back(rel_base->obterTabela(RelacionamentoBase::TABELA_DESTINO));
    }
    else if(rel_base->obterTabela(RelacionamentoBase::TABELA_DESTINO)==tabela)
    {
     refer=true;
     vet_refs.push_back(rel_base->obterTabela(RelacionamentoBase::TABELA_ORIGEM));
    }
    itr++;
   }
  }
  else if(tipo_obj==OBJETO_FUNCAO)
  {
   Funcao *funcao=dynamic_cast<Funcao *>(objeto);
   vector<ObjetoBase *> *lista_obj=NULL;
   vector<ObjetoBase *>::iterator itr, itr_end;
   TipoObjetoBase tipos[7]={OBJETO_CONV_TIPO, OBJETO_CONV_CODIFICACAO,
                            OBJETO_FUNC_AGREGACAO, OBJETO_OPERADOR,
                            OBJETO_TABELA, OBJETO_TIPO, OBJETO_LINGUAGEM };
   unsigned i, i1, qtd;
   Tabela *tab=NULL;
   FuncaoAgregacao *func_ag=NULL;
   Operador *oper=NULL;
   Gatilho *gat=NULL;
   Tipo *tipo=NULL;
   Linguagem *ling=NULL;

   /* Varre todas as listas de objetos os quais podem
      referenciar direta ou indiretamente uma função */
   for(i=0; i < 7 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    if(tipos[i]==OBJETO_CONV_TIPO)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Verifica se o objeto não referencia o papel
      if(dynamic_cast<ConversaoTipo *>(*itr)->obterFuncaoConversao()==funcao)
      {
       refer=true;
       vet_refs.push_back(*itr);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJETO_CONV_CODIFICACAO)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Verifica se o objeto não referencia o papel
      if(dynamic_cast<ConversaoCodificacao *>(*itr)->obterFuncaoConversao()==funcao)
      {
       refer=true;
       vet_refs.push_back(*itr);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJETO_FUNC_AGREGACAO)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      func_ag=dynamic_cast<FuncaoAgregacao *>(*itr);
      //Verifica se o objeto não referencia o papel
      if(func_ag->obterFuncao(FuncaoAgregacao::FUNCAO_FINAL)==funcao ||
         func_ag->obterFuncao(FuncaoAgregacao::FUNCAO_TRANSICAO)==funcao)
      {
       refer=true;
       vet_refs.push_back(func_ag);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJETO_OPERADOR)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operador *>(*itr);

      //Verifica se o objeto não referencia o papel
      if(oper->obterFuncao(Operador::FUNC_OPERADOR)==funcao ||
         oper->obterFuncao(Operador::FUNC_JUNCAO)==funcao  ||
         oper->obterFuncao(Operador::FUNC_RESTRICAO)==funcao)
      {
       refer=true;
       vet_refs.push_back(oper);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJETO_TABELA)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      tab=dynamic_cast<Tabela *>(*itr);
      itr++;
      qtd=tab->obterNumGatilhos();
      for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       gat=tab->obterGatilho(i1);
       //Verifica se o gatilho não referencia a função
       if(gat->obterFuncao()==funcao)
       {
        refer=true;
        vet_refs.push_back(gat);
       }
      }
     }
    }
    else if(tipos[i]==OBJETO_TIPO)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      tipo=dynamic_cast<Tipo *>(*itr);
      itr++;

      for(i1=Tipo::FUNCAO_INPUT; i1 <= Tipo::FUNCAO_ANALYZE && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       //Verifica se o tipo não referencia a função
       if(tipo->obterFuncao(i1)==funcao)
       {
        refer=true;
        vet_refs.push_back(tipo);
       }
      }
     }
    }
    else if(tipos[i]==OBJETO_LINGUAGEM)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      ling=dynamic_cast<Linguagem *>(*itr);
      itr++;
      //Verifica se a função a ser removida é uma função handler ou validator da linguagem
      if(ling->obterFuncao(Linguagem::FUNC_HANDLER)==funcao ||
         ling->obterFuncao(Linguagem::FUNC_VALIDATOR)==funcao ||
         ling->obterFuncao(Linguagem::FUNC_INLINE)==funcao)
      {
       refer=true;
       vet_refs.push_back(ling);
      }
     }
    }
   }
  }
  else if(tipo_obj==OBJETO_ESQUEMA)
  {
   vector<ObjetoBase *> *lista_obj=NULL;
   vector<ObjetoBase *>::iterator itr, itr_end;
   TipoObjetoBase tipos[11]={OBJETO_FUNCAO, OBJETO_TABELA, OBJETO_VISAO,
                             OBJETO_DOMINIO, OBJETO_FUNC_AGREGACAO, OBJETO_OPERADOR,
                             OBJETO_SEQUENCIA, OBJETO_CONV_CODIFICACAO,
                             OBJETO_TIPO, OBJETO_FAMILIA_OPER, OBJETO_CLASSE_OPER};
   unsigned i;

   /* Varre todas as listas de objetos os quais pode pertencer a
      um esquema e verifica se o mesmos não estão referenciando
      o esquema a ser removido */
   for(i=0; i < 11 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
    {
     //Verifica se o objeto não referencia o esquema
     if((*itr)->obterEsquema()==objeto)
     {
      refer=true;
      vet_refs.push_back(*itr);
     }
     itr++;
    }
   }
  }
  else if(tipo_obj==OBJETO_TIPO || tipo_obj==OBJETO_DOMINIO)
  {
   vector<ObjetoBase *> *lista_obj=NULL;
   vector<ObjetoBase *>::iterator itr, itr_end;
   TipoObjetoBase tipos[]={OBJETO_TABELA, OBJETO_CLASSE_OPER, OBJETO_CONV_TIPO,
                           OBJETO_DOMINIO, OBJETO_FUNCAO, OBJETO_FUNC_AGREGACAO,
                           OBJETO_OPERADOR, OBJETO_TIPO };
   unsigned i, i1, qtd;
   ClasseOperadores *classe_op=NULL;
   Tabela *tab=NULL;
   Coluna *col=NULL;
   ConversaoTipo *conv_tipo=NULL;
   Dominio *dom=NULL;
   Funcao *func=NULL;
   FuncaoAgregacao *func_ag=NULL;
   Operador *oper=NULL;
   Tipo *tipo=NULL;

   /* Varre todas as listas de objetos os quais podem
     referenciar direta ou indiretamente um tipo definido
     pelo usuário */
   for(i=0; i < 8 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    if(tipos[i]==OBJETO_TABELA)
    {
     //Verifica se as colunas das tabelas não referenciam o tipo
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a tabela
      tab=dynamic_cast<Tabela *>(*itr);
      itr++;

      //Varre a lista de colunas da tabela
      qtd=tab->obterNumColunas();
      for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       col=tab->obterColuna(i1);
       //Verifica se o tipo da coluna é o próprio tipo a ser excluído
       if(col->obterTipo()==objeto)
       {
        refer=true;
        vet_refs.push_back(col);
       }
      }
     }
    }
    else if(tipos[i]==OBJETO_CLASSE_OPER)
    {
     /* Varre a lista de classe operadores e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      classe_op=dynamic_cast<ClasseOperadores *>(*itr);
      itr++;

      //Verifica se o tipo de dado da classe é o próprio tipo a ser removido
      if(classe_op->obterTipoDado()==objeto)
      {
       refer=true;
       vet_refs.push_back(classe_op);
      }
     }
    }
    else if(tipos[i]==OBJETO_DOMINIO)
    {
     /* Varre a lista de domínios e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      dom=dynamic_cast<Dominio *>(*itr);
      itr++;

      //Verifica se o tipo de dado do dominio é o próprio tipo a ser removido
      if(dom->obterTipo()==objeto)
      {
       refer=true;
       vet_refs.push_back(dom);
      }
     }
    }
    else if(tipos[i]==OBJETO_TIPO)
    {
     /* Varre a lista de tipo definidos pelo usuário e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      tipo=dynamic_cast<Tipo *>(*itr);
      itr++;

      if(tipo->obterAlinhamento()==objeto ||
         tipo->obterElemento()==objeto ||
         tipo->obterTipoCopia()==objeto)
      {
       refer=true;
       vet_refs.push_back(tipo);
      }
     }
    }
    else if(tipos[i]==OBJETO_FUNC_AGREGACAO)
    {
     /* Varre a lista de funções agregadas e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      func_ag=dynamic_cast<FuncaoAgregacao *>(*itr);
      itr++;

      /* Verifica se os tipos de dados da função agregada referenciam
         tipo a ser removido */
      qtd=func_ag->obterNumTipoDados();
      for(i1=0; i1 < qtd  && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       if(func_ag->obterTipoDado(i1)==objeto)
       {
        refer=true;
        vet_refs.push_back(func_ag);
       }
      }
     }
    }
    else if(tipos[i]==OBJETO_FUNCAO)
    {
     /* Varre a lista de funções e verifica se algum
        objeto (parâmetros e tipo de retorno) referencia
        o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      func=dynamic_cast<Funcao *>(*itr);
      itr++;

      //Verifica se o tipo de retorno é o próprio tipo a ser removido
      if(func->obterTipoRetorno()==objeto)
      {
       refer=true;
       vet_refs.push_back(func);
      }
      else
      {
       /* Verifica se os tipos de dados dos parâmetros da função
          referenciam tipo a ser removido */
       qtd=func->obterNumParams();
       for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
       {
        if(func->obterParametro(i1).obterTipo()==objeto)
        {
         refer=true;
         vet_refs.push_back(func);
        }
       }
      }
     }
    }
    else if(tipos[i]==OBJETO_OPERADOR)
    {
     /* Varre a lista de operadores e verifica se algum
        objeto referencia o tipo a ser excluído */
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operador *>(*itr);
      itr++;

      //Verifica se um dos argumentos do operador é o próprio tipo a ser removido
      if(oper->obterTipoDadoArgumento(Operador::ARG_ESQUERDA)==objeto ||
         oper->obterTipoDadoArgumento(Operador::ARG_DIREITA)==objeto)
      {
       refer=true;
       vet_refs.push_back(oper);
      }
     }
    }
    else if(tipos[i]==OBJETO_CONV_TIPO)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      conv_tipo=dynamic_cast<ConversaoTipo *>(*itr);
      itr++;

      //Verifica se o objeto não referencia o tipo
      if(conv_tipo->obterTipoDado(ConversaoTipo::CONV_TIPO_ORIGEM)==objeto)
      {
       refer=true;
       vet_refs.push_back(conv_tipo);
      }
     }
    }
   }
  }
  else if(tipo_obj==OBJETO_PAPEL)
  {
   vector<ObjetoBase *> *lista_obj=NULL;
   vector<ObjetoBase *>::iterator itr, itr_end;
   TipoObjetoBase tipos[13]={OBJETO_FUNCAO, OBJETO_TABELA, OBJETO_DOMINIO,
                             OBJETO_FUNC_AGREGACAO, OBJETO_ESQUEMA, OBJETO_OPERADOR,
                             OBJETO_SEQUENCIA, OBJETO_CONV_CODIFICACAO,
                             OBJETO_LINGUAGEM, OBJETO_ESPACO_TABELA,
                             OBJETO_TIPO, OBJETO_FAMILIA_OPER, OBJETO_CLASSE_OPER};
   unsigned i,i1, qtd;
   Papel *papel_aux=NULL;
   Papel *papel=dynamic_cast<Papel *>(objeto);
   unsigned tipo_papel[3]={Papel::PAPEL_REF, Papel::PAPEL_MEMBRO, Papel::PAPEL_ADMIN};

   /* Caso especial: Varre a lista de papéis e verifica se o papel a ser
      removido se encontra em uma das três listas de papeis da classe
      Papel (ref, membro ou admin) */
   itr=papeis.begin();
   itr_end=papeis.end();
   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    //Obtém um papel
    papel_aux=dynamic_cast<Papel *>(*itr);
    itr++;

    for(i1=0; i1 < 3 && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
    {
     //Obtém a quantidade de papeis presentes na lista atual (tipo_papel[i1])
     qtd=papel_aux->obterNumPapeis(tipo_papel[i1]);
     for(i=0; i < qtd && !refer; i++)
     {
      /* Caso o papel a ser excluído seja igual ao elemento atual
         da lista de papéis do papel aux */
      if(papel_aux->obterPapel(tipo_papel[i1], i)==papel)
      {
       refer=true;
       vet_refs.push_back(papel_aux);
      }
     }
    }
   }

   /* Varre todas as listas de objetos os quais pode pertencer a
      um dono e verifica se o mesmos não estão referenciando
      o dono a ser removido */
   for(i=0; i < 13 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
    {
     //Verifica se o objeto não referencia o papel
     if((*itr)->obterDono()==papel)
     {
      refer=true;
      vet_refs.push_back(*itr);
     }
     itr++;
    }
   }

   /*Caso especial: Verifica se o papel é a ser removido é dono
     do próprio banco de dados */
   if((!modo_exclusao || (modo_exclusao && !refer)) && this->obterDono()==papel)
   {
    refer=true;
    vet_refs.push_back(this);
   }
  }
  else if(tipo_obj==OBJETO_ESPACO_TABELA)
  {
   vector<ObjetoBase *>::iterator itr, itr_end;
   unsigned i, qtd;
   Tabela *tab=NULL;
   Indice *ind=NULL;
   Restricao *rest=NULL;

   /* Varre a lista de tabelas e verifica se estas e seus índices
      não estão referenciando o espaço de tabelas a ser removido */
   itr=tabelas.begin();
   itr_end=tabelas.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    //Obtém a tabela
    tab=dynamic_cast<Tabela *>(*itr);

    //Verifica se referencia o espaço de tabela
    if(tab->obterEspacoTabela()==objeto)
    {
     refer=true;
     vet_refs.push_back(tab);
    }


    /* Verifica se algum dos índices da tabela referecia
       o espaço de tabela */
    qtd=tab->obterNumIndices();
    for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     ind=tab->obterIndice(i);
     if(ind->obterEspacoTabela()==objeto)
     {
      refer=true;
      vet_refs.push_back(ind);
     }
    }

    /* Verifica se alguma restrição da tabela referecia
       o espaço de tabela */
    qtd=tab->obterNumRestricoes();
    for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
    {
     rest=tab->obterRestricao(i);
     if(rest->obterEspacoTabela()==objeto)
     {
      refer=true;
      vet_refs.push_back(rest);
     }
    }

    itr++;
   }

   if((!modo_exclusao || (modo_exclusao && !refer)) && this->ObjetoBase::obterEspacoTabela()==objeto)
   {
    refer=true;
    vet_refs.push_back(this);
   }
  }
  else if(tipo_obj==OBJETO_LINGUAGEM)
  {
   vector<ObjetoBase *>::iterator itr, itr_end;
   Funcao *func=NULL;

   /* Varre a lista de funções e verifica se estas
      não estão referenciando a linguage a ser removida */
   itr=funcoes.begin();
   itr_end=funcoes.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    //Verifica se referencia o espaço de tabela
    func=dynamic_cast<Funcao *>(*itr);
    if(func->obterLinguagem()==objeto)
    {
     refer=true;
     vet_refs.push_back(func);
    }
    itr++;
   }
  }
  else if(tipo_obj==OBJETO_OPERADOR)
  {
   vector<ObjetoBase *> *lista_obj=NULL;
   vector<ObjetoBase *>::iterator itr, itr_end;
   TipoObjetoBase tipos[3]={OBJETO_CLASSE_OPER,
                            OBJETO_FUNC_AGREGACAO,
                            OBJETO_OPERADOR};
   unsigned i, i1, qtd;
   ClasseOperadores *classe_op=NULL;
   Operador *oper=NULL, *operador=dynamic_cast<Operador *>(objeto);

   /* Varre todas as listas de objetos os quais podem
      referenciar direta ou indiretamente um operador */
   for(i=0; i < 3 && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém uma lista
    lista_obj=obterListaObjetos(tipos[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    if(tipos[i]==OBJETO_CLASSE_OPER)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      //Obtém a referência ao objeto
      classe_op=dynamic_cast<ClasseOperadores *>(*itr);
      itr++;

      //Varre a lista de elementos da classe de operadores
      qtd=classe_op->obterNumElemClasseOperadores();
      for(i1=0; i1 < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       //Verifica se o objeto não referencia o operador
       if(classe_op->obterElementoClasse(i1).obterOperador()==operador)
       {
        refer=true;
        vet_refs.push_back(classe_op);
       }
      }
     }
    }
    else if(tipos[i]==OBJETO_FUNC_AGREGACAO)
    {
     while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
     {
      if(dynamic_cast<FuncaoAgregacao *>(*itr)->obterOperadorOrdenacao()==operador)
      {
       refer=true;
       vet_refs.push_back(*itr);
      }
      itr++;
     }
    }
    else if(tipos[i]==OBJETO_OPERADOR)
    {
     while(itr!=itr_end && !refer)
     {
      //Obtém a referência ao objeto
      oper=dynamic_cast<Operador *>(*itr);
      itr++;

      /* Verifica se um dos operadores agregados ao operador atual não referencia
         o objeto não referencia o operador */
      for(i1=Operador::OPER_COMUTACAO; i1 <= Operador::OPER_MAIOR &&
            (!modo_exclusao || (modo_exclusao && !refer)); i1++)
      {
       if(oper->obterOperador(i1)==operador)
       {
        refer=true;
        vet_refs.push_back(oper);
       }
      }
     }
    }
   }
  }
  else if(tipo_obj==OBJETO_FAMILIA_OPER)
  {
   vector<ObjetoBase *>::iterator itr, itr_end;
   FamiliaOperadores *familia_op=dynamic_cast<FamiliaOperadores *>(objeto);

   /* Varre a lista de classe de operadores e verifica se um
      de seus elementos está referenciando a família de operadores */
   itr=classe_ops.begin();
   itr_end=classe_ops.end();

   while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
   {
    if(dynamic_cast<ClasseOperadores *>(*itr)->obterFamilia()==familia_op)
    {
     refer=true;
     vet_refs.push_back(*itr);
    }
    itr++;
   }
  }
  else if(tipo_obj==OBJETO_COLUNA)
  {
   Coluna *coluna=dynamic_cast<Coluna *>(objeto);
   vector<ObjetoBase *> *lista_obj=NULL;
   vector<ObjetoBase *>::iterator itr, itr_end;
   TipoObjetoBase  tipos_obj[]={ OBJETO_SEQUENCIA, OBJETO_VISAO, OBJETO_TABELA };
   unsigned i, qtd=3;

   for(i=0; i < qtd && (!modo_exclusao || (modo_exclusao && !refer)); i++)
   {
    //Obtém a lista do tipo atual
    lista_obj=obterListaObjetos(tipos_obj[i]);
    itr=lista_obj->begin();
    itr_end=lista_obj->end();

    //Varre a lista selecionada verificado se a coluna passada é referenciada
    while(itr!=itr_end && (!modo_exclusao || (modo_exclusao && !refer)))
    {
     /* Caso 1: o tipo atual seja sequência, faz o cast para a classe
        e chama do método da sequencia que retorna a coluna possuidora
        da mesma.

        Caso 2: o tipo atual seja visão, faz o cast para a classe
        e chama do método da visão o qual retorna se a coluna
        é referenciada pelo elementos da visão. */
     if((tipos_obj[i]==OBJETO_SEQUENCIA && dynamic_cast<Sequencia *>(*itr)->obterPossuidora()==coluna) ||
        (tipos_obj[i]==OBJETO_VISAO && dynamic_cast<Visao *>(*itr)->referenciaColuna(coluna)))
     {
      refer=true;
      vet_refs.push_back(*itr);
     }
     else if(tipos_obj[i]==OBJETO_TABELA)
     {
      Tabela *tab=dynamic_cast<Tabela *>(*itr);
      unsigned qtd_gat, idx, qtd1, i1;
      Gatilho *gat=NULL;

      qtd_gat=tab->obterNumGatilhos();
      for(idx=0; idx < qtd_gat && (!modo_exclusao || (modo_exclusao && !refer)); idx++)
      {
       gat=tab->obterGatilho(idx);
       qtd1=gat->obterNumColunas();

       for(i1=0; i1 < qtd1 && (!modo_exclusao || (modo_exclusao && !refer)); i1++)
       {
        if(gat->obterColuna(i1)==coluna)
        {
         refer=true;
         vet_refs.push_back(gat);
        }
       }
      }
     }
     itr++;
    }
   }
  }
 }
}
//-----------------------------------------------------------
void ModeloBD::definirObjetosModificados(void)
{
 TipoObjetoBase tipos[]={OBJETO_TABELA, OBJETO_VISAO,
                         OBJETO_RELACAO, OBJETO_RELACAO_BASE,
                         OBJETO_CAIXA_TEXTO};
 vector<ObjetoBase *>::iterator itr, itr_end;
 vector<ObjetoBase *> *lista_obj=NULL;
 CaixaTexto *rot=NULL;
 RelacionamentoBase *rel=NULL;
 unsigned i, i1;

 for(i=0; i < 5; i++)
 {
  lista_obj=obterListaObjetos(tipos[i]);
  itr=lista_obj->begin();
  itr_end=lista_obj->end();

  while(itr!=itr_end)
  {
   dynamic_cast<ObjetoGraficoBase *>(*itr)->definirModificado(true);

   /* Caso especial: Caso o objeto seja um relacionamento, os rótulos
      do mesmo que são caixas de texto, devem também ser marcados
      como modificado */
   if(tipos[i]==OBJETO_RELACAO || tipos[i]==OBJETO_RELACAO_BASE)
   {
    rel=dynamic_cast<RelacionamentoBase *>(*itr);
    for(i1=0; i1 < 3; i1++)
    {
     rot=rel->obterRotulo(i1);
     if(rot) rot->definirModificado(true);
    }
   }

   itr++;
  }
 }
}
//***********************************************************
