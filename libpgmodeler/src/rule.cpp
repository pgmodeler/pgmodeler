#include "rule.h"

Rule::Rule(void)
{
 tipo_exec=TipoBase::nulo;
 obj_type=OBJ_RULE;
 attributes[ParsersAttributes::EVENT_TYPE]="";
 attributes[ParsersAttributes::TABLE]="";
 attributes[ParsersAttributes::CONDITION]="";
 attributes[ParsersAttributes::EXEC_TYPE]="";
 attributes[ParsersAttributes::COMMANDS]="";
}

void Rule::definirAtributoComandos(void)
{
 QString str_cmds;
 unsigned i, qtd;

 qtd=comandos.size();
 for(i=0; i < qtd; i++)
 {
  str_cmds+=comandos[i];
  if(i < (qtd-1)) str_cmds+=";";
 }

 //if(str_cmds!="") str_cmds="(" + str_cmds + ")";
 attributes[ParsersAttributes::COMMANDS]=str_cmds;
}

void Rule::definirTipoEvento(TipoEvento tipo)
{
 tipo_evento=tipo;
}

void Rule::definirTipoExecucao(TipoExecucao tipo)
{
 tipo_exec=tipo;
}

void Rule::definirExpCondicional(const QString &exp)
{
 exp_condicional=exp;
}

void Rule::adicionarComando(const QString &comando)
{
 //Caso o comando a ser atribuido estaja vazio
 if(comando=="")
  //Dispara a exceção avisando tal fato
  throw Exception(ERR_INS_EMPTY_RULE_COMMAND,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  QString cmd_aux=comando;

  //Adiciona o comando a lista de comandos da regra
  cmd_aux.remove(";");
  comandos.push_back(cmd_aux);
 }
}

TipoEvento Rule::obterTipoEvento(void)
{
 return(tipo_evento);
}

TipoExecucao Rule::obterTipoExecucao(void)
{
 return(tipo_exec);
}

QString Rule::obterExpCondicional(void)
{
 return(exp_condicional);
}

QString Rule::obterComando(unsigned idx_cmd)
{
 /* Verifica se o índice condiz com o tamanho das listas de comandos,
    caso não conincida, dispara exceção */
 if(idx_cmd >= comandos.size())
  throw Exception(ERR_REF_RULE_CMD_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Retorna o comando no índice passado
  return(comandos[idx_cmd]);
}

unsigned Rule::obterNumComandos(void)
{
 return(comandos.size());
}

void Rule::removerComando(unsigned idx_cmd)
{
 /* Verifica se o índice condiz com o tamanho das listas de comandos,
    caso não conincida, dispara exceção */
 if(idx_cmd>=comandos.size())
  throw Exception(ERR_REF_RULE_CMD_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Remove o comando no índice específico
  comandos.erase(comandos.begin() + idx_cmd);
}

void Rule::removerComandos(void)
{
 comandos.clear();
}

QString Rule::getCodeDefinition(unsigned tipo_def)
{
 definirAtributoComandos();
 attributes[ParsersAttributes::CONDITION]=exp_condicional;
 attributes[ParsersAttributes::EXEC_TYPE]=(~tipo_exec);
 attributes[ParsersAttributes::EVENT_TYPE]=(~tipo_evento);

 if(this->parent_table)
  attributes[ParsersAttributes::TABLE]=this->parent_table->getName(true);


 return(BaseObject::__getCodeDefinition(tipo_def));
}

