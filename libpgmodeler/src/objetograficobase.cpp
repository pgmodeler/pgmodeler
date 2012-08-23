#include "objetograficobase.h"
//***********************************************************
ObjetoGraficoBase::ObjetoGraficoBase(void)
{
 /* Por padrão o objeto é definido como modificado
    para forçar o seu redesenho  */
 modificado=true;

 //Atributos de posição do objeto, usado apenas na definição XML
 atributos[AtributosParsers::POSICAO_X]="";
 atributos[AtributosParsers::POSICAO_Y]="";
 atributos[AtributosParsers::POSICAO]="";

 objeto_receptor=NULL;
}
//-----------------------------------------------------------
ObjetoGraficoBase::~ObjetoGraficoBase(void)
{
 this->blockSignals(true);
}
//-----------------------------------------------------------
void ObjetoGraficoBase::definirProtegido(bool valor)
{
 protegido=valor;
 if(!this->signalsBlocked())
  emit s_objetoProtegido(valor);
}
//-----------------------------------------------------------
void ObjetoGraficoBase::definirModificado(bool valor)
{
 modificado=valor;

 if(!this->signalsBlocked() && modificado)
  emit s_objetoModificado();
}
//-----------------------------------------------------------
bool ObjetoGraficoBase::objetoModificado(void)
{
 return(modificado);
}
//-----------------------------------------------------------
void ObjetoGraficoBase::definirAtributoPosicao(void)
{
 atributos[AtributosParsers::POSICAO_X]=QString("%1").arg(posicao.x());
 atributos[AtributosParsers::POSICAO_Y]=QString("%1").arg(posicao.y());
 atributos[AtributosParsers::POSICAO]=ParserEsquema::obterDefinicaoObjeto(AtributosParsers::POSICAO,
                                     atributos, ParserEsquema::DEFINICAO_XML);
}
//-----------------------------------------------------------
void  ObjetoGraficoBase::definirPosicaoObjeto(QPointF p)
{
 posicao=p;
}
//-----------------------------------------------------------
QPointF ObjetoGraficoBase::obterPosicaoObjeto(void)
{
 return(posicao);
}
//-----------------------------------------------------------
void ObjetoGraficoBase::operator = (ObjetoGraficoBase &obj)
{
 *(dynamic_cast<ObjetoBase *>(this))=dynamic_cast<ObjetoBase &>(obj);
 this->posicao=obj.posicao;
}
//-----------------------------------------------------------
void ObjetoGraficoBase::definirObjetoReceptor(QObject *objeto)
{
 if(!objeto && objeto_receptor)
  disconnect(this, NULL, objeto_receptor, NULL);

 objeto_receptor=objeto;
}
//-----------------------------------------------------------
QObject *ObjetoGraficoBase::obterObjetoReceptor(void)
{
 return(objeto_receptor);
}
//***********************************************************
