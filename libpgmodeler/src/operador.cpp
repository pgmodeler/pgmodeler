#include "operador.h"
//***********************************************************
Operador::Operador(void)
{
 unsigned i;

 tipo_objeto=OBJETO_OPERADOR;

 for(i=0; i < 3; i++)
  funcoes[i]=NULL;

 for(i=0; i < 6; i++)
  operadores[i]=NULL;

 hashes=merges=false;
 tipo_args[ARG_ESQUERDA]=TipoPgSQL("any");
 tipo_args[ARG_DIREITA]=TipoPgSQL("any");

 atributos[AtributosParsers::TIPO_ESQUERDA]="";
 atributos[AtributosParsers::TIPO_DIRETA]="";
 atributos[AtributosParsers::OP_COMUTACAO]="";
 atributos[AtributosParsers::OP_NEGACAO]="";
 atributos[AtributosParsers::OP_ORDENACAO]="";
 atributos[AtributosParsers::OP_ORDENACAO2]="";
 atributos[AtributosParsers::OP_MENOR]="";
 atributos[AtributosParsers::OP_MAIOR]="";
 atributos[AtributosParsers::FUNCAO_RESTRICAO]="";
 atributos[AtributosParsers::FUNCAO_JUNCAO]="";
 atributos[AtributosParsers::FUNCAO_OPERADOR]="";
 atributos[AtributosParsers::HASHES]="";
 atributos[AtributosParsers::MERGES]="";
 atributos[AtributosParsers::ASSINATURA]="";
 atributos[AtributosParsers::TIPO_REFERENCIA]="";
}
//-----------------------------------------------------------
bool Operador::nomeValido(const QString &nome)
{
 //ATENÇÃO: Não alterar a seqüência em que aparecem os caracteres.
 QString chr_validos="+-*/<>=~!@#%^&|'?";
 int pos, tam;
 bool valido=true;

 //Verifica se o tamanho do nome é válido
 valido=(nome.size() <= static_cast<int>(TAM_MAX_NOME_OBJETO));

 //Verificando se o nome é válido de acordo com as condições:
 /* 1) O nome possui apenas caracteres do conjunto
       definido por chr_validos */
 tam=nome.size();
 for(pos=0; pos < tam && valido; pos++)
  valido=!(chr_validos.find(nome[pos]) < 0);

 /* 2) O nome não possui as sequencias -- ou / * as quais
       definem início de comentário SQL */
 if(valido) valido=(nome.indexOf("--") < 0);
 if(valido) valido=(nome.indexOf("/*") < 0);

 /* 3) Se o nome terminar com um caractere - ou + o mesmo deve
       possuir um ou mais caracteres do conjunto ~!@#%^&|'? */
 if(nome[nome.size()-1]=='-' || nome[nome.size()-1]=='+')
 {
  /* O conjunto ~!@#%^&|'? é definido a partir da
     posição 7 do conjunto chr_validos */
  tam=chr_validos.size();
  for(pos=7; pos < tam && valido; pos++)
   valido=(nome.indexOf(chr_validos[pos]) < 0);
 }

 return(valido);
}
//-----------------------------------------------------------
void Operador::definirNome(const QString &nome)
{
 if(nome=="")
  throw Excecao(ERR_PGMODELER_ATRNOMEOBJVAZIO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  if(!nomeValido(nome))
   throw Excecao(ERR_PGMODELER_ATRNOMEOBJINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
   this->nome=nome;
 }
}
//-----------------------------------------------------------
void Operador::definirFuncao(Funcao *funcao, unsigned tipo_funcao)
{
 //Caso o tipo de função seja inválido
 if(tipo_funcao > FUNC_RESTRICAO)
  throw Excecao(ERR_PGMODELER_REFFUNCTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(tipo_funcao==FUNC_OPERADOR)
 {
  /* Caso a função não esteja alocada e se tente atribuí-la
     como função executada pelo operador */
  if(!funcao)
    throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCNAOALOC)
                         .arg(this->obterNome(true))
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_OPERADOR)),
                  ERR_PGMODELER_ATRFUNCNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  /* Caso o número de parâmetros da função seja inválido. Para operadores
     a mesma deve possuir 1 ou 2 parâmetros */
  else if(funcao->obterNumParams()==0 || funcao->obterNumParams() > 2)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCNUMPARAMINV)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_OPERADOR)),
                 ERR_PGMODELER_ATRFUNCNUMPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {
   unsigned qtd_params=funcao->obterNumParams();
   TipoPgSQL tipo_param1=TipoPgSQL("any"), tipo_param2=TipoPgSQL("any");

   //Obtém os prâmetros da função
   tipo_param1=funcao->obterParametro(0).obterTipo();
   if(qtd_params==2) tipo_param2=funcao->obterParametro(1).obterTipo();

   /* Verificando os parâmetros da função de acordo com o tipo dos
      argumentos dos operadores */

   //A função não pode possuir parametros do tipo any
   if((tipo_param1=="any" || (qtd_params==2 && tipo_param2=="any")) ||

      /* Caso a quantidade de parametros é 1 e ambos os argumentos do
         operador esteja com tipos configurados (operador binário) */
      (qtd_params==1 && tipo_args[0]!="any" && tipo_args[1]!="any") ||

      /* Caso a quantidade de parâmetros seja 2 verifica se ambos os argumentos
         do operador estão com tipos configurados */
      (qtd_params==2 && ((tipo_args[0]=="any" && tipo_args[1]!="any") ||
                         (tipo_args[0]!="any" && tipo_args[1]=="any"))) ||

    /* Quando a função possui 2 parametros os mesmo deve ser do mesmo tipo
      que os argumentos do operador */
      (qtd_params==2 &&
      ((tipo_args[0]=="any" || tipo_args[1]=="any") ||
       (tipo_args[0]!=tipo_param1 || tipo_args[1]!=tipo_param2))) ||

       /* Quando possui apenas 1 parâmetro o tipo do mesmo deve coincidir
          com um dos argumentos do operador, sendo que um deles deve
          ser do tipo "any" indicando que apenas um argumento foi setado para o operador */
      (qtd_params==1 &&
       ((tipo_args[0]!="any" && tipo_args[0]!=tipo_param1) ||
        (tipo_args[1]!="any" && tipo_args[1]!=tipo_param1))))
     throw Excecao(ERR_PGMODELER_ATRFUNCPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
 }

 funcoes[tipo_funcao]=funcao;
}
//-----------------------------------------------------------
void Operador::definirTipoDadoArgumento(TipoPgSQL tipo_dado, unsigned tipo_arg)
{
 //Caso o tipo de argumento seja inválido
 if(tipo_arg > ARG_DIREITA)
  throw Excecao( ERR_PGMODELER_REFARGOPTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  tipo_args[tipo_arg]=tipo_dado;
}
//-----------------------------------------------------------
void Operador::definirOperador(Operador *op, unsigned tipo_op)
{
 //Caso o tipo de operador seja inválido
 if(tipo_op > OPER_MAIOR)
   throw Excecao(ERR_PGMODELER_REFOPTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  /* Validando OP Comutação: De acordo com a documentação do PostgreSQL o operador
     de comutação deve possuir seu argumento à direita do mesmo tipo do argumento
     à esquerda do operador comutado. Ou seja, se o operador ++ (tipoA, tipoB)
     está sendo definido e seu operador de comutação é +*+ então a assinatura
     deste último deve ser +*+(tipoB, tipoA). A condição testa abaixo é a situação
     contrária, ou seja quando o operador de comutação não atende aos requisitos
     da documentação  */
  if(op && tipo_op==OPER_COMUTACAO && tipo_args[ARG_ESQUERDA]!=op->tipo_args[ARG_DIREITA])
  {
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATROPERCOMUTINV)
                         .arg(op->obterAssinatura(true))
                         .arg(this->obterAssinatura(true)),
                 ERR_PGMODELER_ATRFUNCNUMPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  /* Validando OP Negação: De acordo com a documentação do PostgreSQL o operador
     de negação deve possuir seus argumentos do mesmo tipo dos argumentos
     do operador a ser definido. Ou seja, se o operador !! (tipoA) está sendo
     definido e seu operador de negação é !*! então a assinatura deste
     último deve ser !*!(tipoA). A condição testa abaixo é a situação contrária,
     ou seja quando o operador de negação não atende aos requisitos da documentação */
  else if(op && tipo_op==OPER_NEGACAO &&
          (tipo_args[ARG_ESQUERDA]!=op->tipo_args[ARG_ESQUERDA] &&
           tipo_args[ARG_DIREITA]!=op->tipo_args[ARG_DIREITA]))
  {
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATROPERNEGINV)
                         .arg(op->obterAssinatura(true))
                         .arg(this->obterAssinatura(true)),
                 ERR_PGMODELER_ATRFUNCNUMPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
  else
   operadores[tipo_op]=op;
 }
}
//-----------------------------------------------------------
void Operador::definirHashes(bool valor)
{
 hashes=valor;
}
//-----------------------------------------------------------
void Operador::definirMerges(bool valor)
{
 merges=valor;
}
//-----------------------------------------------------------
Funcao *Operador::obterFuncao(unsigned tipo_funcao)
{
 //Caso o tipo de função seja inválido
 if(tipo_funcao > FUNC_RESTRICAO)
  throw Excecao(ERR_PGMODELER_REFOPTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(funcoes[tipo_funcao]);
}
//-----------------------------------------------------------
TipoPgSQL Operador::obterTipoDadoArgumento(unsigned tipo_arg)
{
 //Caso o tipo de argumento seja inválido
 if(tipo_arg > ARG_DIREITA)
  throw Excecao( ERR_PGMODELER_REFARGOPTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 return(tipo_args[tipo_arg]);
}
//-----------------------------------------------------------
Operador *Operador::obterOperador(unsigned tipo_op)
{
 //Caso o tipo de operador seja inválido
 if(tipo_op > OPER_MAIOR)
  throw Excecao(ERR_PGMODELER_REFFUNCTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 return(operadores[tipo_op]);
}
//-----------------------------------------------------------
bool Operador::aceitaHashes(void)
{
 return(hashes);
}
//-----------------------------------------------------------
bool Operador::aceitaMerges(void)
{
 return(merges);
}
//-----------------------------------------------------------
QString Operador::obterAssinatura(bool formatar_nome)
{
 QString assinatura, str_aux;
 unsigned i;

 assinatura=this->obterNome(formatar_nome);

 for(i=0; i < 2; i++)
 {
  if(tipo_args[i]!="any")
  {
   if(tipo_args[i]=="any") str_aux+="NONE";
   else str_aux+=(*tipo_args[i]);
   if(i==0) str_aux+=",";
  }
 }
 assinatura+="(" + str_aux + ")";
 return(assinatura);
}
//-----------------------------------------------------------
QString Operador::obterDefinicaoObjeto(unsigned tipo_def)
{
 return(this->obterDefinicaoObjeto(tipo_def, false));
}
//-----------------------------------------------------------
QString Operador::obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida)
{
 unsigned i;
 QString atribs_tipos[]={AtributosParsers::TIPO_ESQUERDA, AtributosParsers::TIPO_DIRETA},
        atribs_ops[]={ AtributosParsers::OP_COMUTACAO,
                      AtributosParsers::OP_NEGACAO,
                      AtributosParsers::OP_ORDENACAO,
                      AtributosParsers::OP_ORDENACAO2,
                      AtributosParsers::OP_MENOR,
                      AtributosParsers::OP_MAIOR},
        atribs_funcoes[]={AtributosParsers::FUNCAO_OPERADOR,
                          AtributosParsers::FUNCAO_JUNCAO,
                          AtributosParsers::FUNCAO_RESTRICAO};

 for(i=Operador::ARG_ESQUERDA; i <= Operador::ARG_DIREITA; i++)
 {
  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
  {
   if(tipo_args[i]!="any")
    atributos[atribs_tipos[i]]=(*tipo_args[i]);
  }
  else
  {
   atributos[atribs_tipos[i]]=tipo_args[i].
                obterDefinicaoObjeto(ParserEsquema::DEFINICAO_XML,atribs_tipos[i]);
  }
 }

 for(i=Operador::OPER_COMUTACAO; i <= Operador::OPER_MAIOR; i++)
 {
  if(operadores[i])
  {
   if(tipo_def==ParserEsquema::DEFINICAO_SQL)
    atributos[atribs_ops[i]]=operadores[i]->obterNome(true);
   else
   {
    operadores[i]->atributos[AtributosParsers::TIPO_REFERENCIA]=atribs_ops[i];
    atributos[atribs_ops[i]]=operadores[i]->obterDefinicaoObjeto(tipo_def, true);
   }
  }
 }

 for(i=Operador::FUNC_OPERADOR; i <= Operador::FUNC_RESTRICAO; i++)
 {
  if(funcoes[i])
  {
   if(tipo_def==ParserEsquema::DEFINICAO_SQL)
    atributos[atribs_funcoes[i]]=funcoes[i]->obterAssinatura();
   else
   {
    funcoes[i]->definirAtributoEsquema(AtributosParsers::TIPO_REFERENCIA, atribs_funcoes[i]);
    atributos[atribs_funcoes[i]]=funcoes[i]->obterDefinicaoObjeto(tipo_def, true);
   }
  }
 }

 atributos[AtributosParsers::HASHES]=(hashes ? "1" : "");
 atributos[AtributosParsers::MERGES]=(merges ? "1" : "");
 atributos[AtributosParsers::ASSINATURA]=obterAssinatura();

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def, forma_reduzida));
}
//***********************************************************
