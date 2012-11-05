#include "objetograficobase.h"

ObjetoGraficoBase::ObjetoGraficoBase(void)
{
 /* Por padrão o objeto é definido como modificado
    para forçar o seu redesenho  */
 modificado=true;

 //Atributos de posição do objeto, usado apenas na definição XML
 attributes[ParsersAttributes::X_POS]="";
 attributes[ParsersAttributes::Y_POS]="";
 attributes[ParsersAttributes::POSITION]="";

 objeto_receptor=NULL;
}

ObjetoGraficoBase::~ObjetoGraficoBase(void)
{
 this->blockSignals(true);
}

void ObjetoGraficoBase::setProtected(bool valor)
{
 protected_obj=valor;
 if(!this->signalsBlocked())
  emit s_isProtected(valor);
}

void ObjetoGraficoBase::definirModificado(bool valor)
{
 modificado=valor;

 if(!this->signalsBlocked() && modificado)
  emit s_objetoModificado();
}

bool ObjetoGraficoBase::objetoModificado(void)
{
 return(modificado);
}

void ObjetoGraficoBase::definirAtributoPosicao(void)
{
 attributes[ParsersAttributes::X_POS]=QString("%1").arg(posicao.x());
 attributes[ParsersAttributes::Y_POS]=QString("%1").arg(posicao.y());
 attributes[ParsersAttributes::POSITION]=SchemaParser::getObjectDefinition(ParsersAttributes::POSITION,
                                     attributes, SchemaParser::XML_DEFINITION);
}

void  ObjetoGraficoBase::definirPosicaoObjeto(QPointF p)
{
 posicao=p;
}

QPointF ObjetoGraficoBase::obterPosicaoObjeto(void)
{
 return(posicao);
}

void ObjetoGraficoBase::operator = (ObjetoGraficoBase &obj)
{
 *(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(obj);
 this->posicao=obj.posicao;
}

void ObjetoGraficoBase::definirObjetoReceptor(QObject *objeto)
{
 if(!objeto && objeto_receptor)
  disconnect(this, NULL, objeto_receptor, NULL);

 objeto_receptor=objeto;
}

QObject *ObjetoGraficoBase::obterObjetoReceptor(void)
{
 return(objeto_receptor);
}

