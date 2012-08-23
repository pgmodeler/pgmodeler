#include "linguagem.h"
//***********************************************************
Linguagem::Linguagem(void)
{
 nome="";
 tipo_objeto=OBJETO_LINGUAGEM;
 confiavel=false;

 for(unsigned i=FUNC_VALIDATOR; i <= FUNC_INLINE; i++)
  funcoes[i]=NULL;

 atributos[AtributosParsers::CONFIAVEL]="";
 atributos[AtributosParsers::FUNCAO_HANDLER]="";
 atributos[AtributosParsers::FUNCAO_VALIDATOR]="";
 atributos[AtributosParsers::FUNCAO_INLINE]="";
}
//-----------------------------------------------------------
void Linguagem::definirNome(const QString &nome)
{
 /* Tratando nome de linguagem. Nomes como SQL, C são reservados
    para o SGBD e não podem ser criados pelo usuário */
 if(nome.toLower()==~TipoLinguagem("c") || nome.toLower()==~TipoLinguagem("sql"))
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRNOMERESERV)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_LINGUAGEM)),
                ERR_PGMODELER_ATRNOMERESERV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 ObjetoBase::definirNome(nome); //Chama o método da classe descendente
}
//-----------------------------------------------------------
void Linguagem::definirConfiavel(bool valor)
{
 confiavel=valor;
}
//-----------------------------------------------------------
void  Linguagem::definirFuncao(Funcao *funcao, unsigned tipo_func)
{
 TipoLinguagem ling;
 ling=TipoLinguagem::c;

 if(!funcao ||
     (funcao &&
      /* Obrigatóriamente uma função de manipulação deve ser escrita em C,
         e possuir tipo de retorno "language_handler" */
      ((tipo_func==FUNC_HANDLER &&
        funcao->obterTipoRetorno()=="language_handler" &&
        funcao->obterNumParams()==0 &&
        funcao->obterLinguagem()->obterNome()==(~ling)) ||
       /* Obrigatoriamente a função de validator de ser escrita em C,
          retornar "void", possuir apenas 1 parâmetro e que o mesmo
          seja do tipo "oid" */
       (tipo_func==FUNC_VALIDATOR &&
        funcao->obterTipoRetorno()=="void" &&
        funcao->obterNumParams()==1 &&
        funcao->obterParametro(0).obterTipo() == "oid" &&
        funcao->obterLinguagem()->obterNome()==(~ling)) ||
      /* Obrigatoriamente a função inline de ser escrita em C,
         retornar "void", possuir apenas 1 parâmetro e que o mesmo
         seja do tipo "internal" */
       (tipo_func==FUNC_INLINE &&
        funcao->obterTipoRetorno()=="void" &&
        funcao->obterNumParams()==1 &&
        funcao->obterParametro(0).obterTipo() == "internal" &&
        funcao->obterLinguagem()->obterNome()==(~ling)) )))
 {
  this->funcoes[tipo_func]=funcao;
 }
 //Disparando uma exceção caso o tipo de retorno da função não coincida com o aceito para cada tipo de função
 else if((tipo_func==FUNC_HANDLER && funcao->obterTipoRetorno()!="language_handler") ||
         ((tipo_func==FUNC_VALIDATOR || tipo_func==FUNC_INLINE) && funcao->obterTipoRetorno()!="void"))
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCRETINV)
                         .arg(this->obterNome(true))
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_LINGUAGEM)),
                ERR_PGMODELER_ATRFUNCRETINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Dispara um erro se a função possuir parâmetros inválidos
  throw Excecao(ERR_PGMODELER_ATRFUNCPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
//-----------------------------------------------------------
Funcao * Linguagem::obterFuncao(unsigned tipo_func)
{
 if(tipo_func > FUNC_INLINE)
  throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(funcoes[tipo_func]);
}
//-----------------------------------------------------------
bool Linguagem::linguagemConfiavel(void)
{
 return(confiavel);
}
//-----------------------------------------------------------
QString Linguagem::obterDefinicaoObjeto(unsigned tipo_def)
{
 return(this->obterDefinicaoObjeto(tipo_def, false));
}
//-----------------------------------------------------------
QString Linguagem::obterDefinicaoObjeto(unsigned tipo_def, bool forma_reduzida)
{
 unsigned i;
 QString atrib_func[3]={AtributosParsers::FUNCAO_VALIDATOR,
                        AtributosParsers::FUNCAO_HANDLER,
                        AtributosParsers::FUNCAO_INLINE};

 atributos[AtributosParsers::CONFIAVEL]=(confiavel ? "1" : "");

 if(!forma_reduzida && tipo_def==ParserEsquema::DEFINICAO_XML)
  forma_reduzida=(!funcoes[FUNC_VALIDATOR] && !funcoes[FUNC_HANDLER] && !funcoes[FUNC_INLINE] && !this->obterDono());

 for(i=0; i < 3; i++)
 {
  if(funcoes[i])
  {
   if(tipo_def==ParserEsquema::DEFINICAO_SQL)
    atributos[atrib_func[i]]=funcoes[i]->obterNome(true);
   else
   {
    funcoes[i]->definirAtributoEsquema(AtributosParsers::TIPO_REFERENCIA, atrib_func[i]);
    atributos[atrib_func[i]]=funcoes[i]->obterDefinicaoObjeto(tipo_def, true);
   }
  }
 }

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def, forma_reduzida));
}
//***********************************************************
