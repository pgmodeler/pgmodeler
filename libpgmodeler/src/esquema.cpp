#include "esquema.h"
//***********************************************************
Esquema::Esquema(void)
{
 tipo_objeto=OBJETO_ESQUEMA;
 id_objeto=ObjetoBase::id_esquema++;
}
//-----------------------------------------------------------
void Esquema::definirNome(const QString &nome)
{
 /* Tratando nomes de esquemas iniciados em pg_ pois são reservados para o SGBD e
    não podem ser criados pelo usuário */
 if(nome.mid(0,3)=="pg_")
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRNOMERESERV)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_ESQUEMA)),
                ERR_PGMODELER_ATRNOMERESERV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 ObjetoBase::definirNome(nome); //Chama o método da classe descendente
}
//***********************************************************
