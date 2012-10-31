#include "espacotabela.h"

EspacoTabela::EspacoTabela(void)
{
 tipo_objeto=OBJETO_ESPACO_TABELA;
 atributos[ParsersAttributes::DIRECTORY]="";
 id_objeto=ObjetoBase::id_esptabela++;
}

void EspacoTabela::definirNome(const QString &nome)
{
 /* Tratando nomes de esquemas iniciados em pg_ pois são reservados para o SGBD e
    não podem ser criados pelo usuário */
 //if(nome.substr(0,3)=="pg_")
 if(nome.mid(0,3)=="pg_")
  throw Exception(Exception::getErrorMessage(ERR_ASG_RESERVED_NAME)
                         .arg(QString::fromUtf8(this->obterNome()))
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_ESPACO_TABELA)),
                ERR_ASG_RESERVED_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 ObjetoBase::definirNome(nome); //Chama o método da classe descendente
}

void EspacoTabela::definirDiretorio(const QString &diretorio)
{
 QString dir_aux=diretorio;
 dir_aux.remove("'");

 /* Caso se tente atribuir um nome de diretório vazio ao espaco de tabelas
    é disparada uma exceção, pois é obrigatório informar o diretório do
    sistema onde o espaço de tabelas será criado */
 if(dir_aux=="")
  throw Exception(ERR_ASG_EMPTY_DIR_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //TODO: Verificar se o diretório está na forma correta em windows ou unix???
 this->diretorio=dir_aux;
}

QString EspacoTabela::obterDiretorio(void)
{
 return(diretorio);
}

QString EspacoTabela::obterDefinicaoObjeto(unsigned tipo_def)
{
 if(diretorio!="")
  atributos[ParsersAttributes::DIRECTORY]="'" + diretorio + "'";

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}

