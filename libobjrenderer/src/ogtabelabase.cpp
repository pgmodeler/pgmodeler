#include "ogtabelabase.h"
//***********************************************************
OGTabelaBase::OGTabelaBase(TabelaBase *tab_base) : ObjetoGrafico(tab_base)
{
 if(!tab_base)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 corpo=new QGraphicsPolygonItem;
 titulo=new OGTituloObjeto;
 this->addToGroup(corpo);
 this->addToGroup(titulo);
}
//-----------------------------------------------------------
OGTabelaBase::~OGTabelaBase(void)
{
 this->removeFromGroup(corpo);
 this->removeFromGroup(titulo);
 delete(corpo);
 delete(titulo);
}
//-----------------------------------------------------------
QVariant OGTabelaBase::itemChange(GraphicsItemChange change, const QVariant &value)
{
 //Executa o método itemChange() da classe superior
 ObjetoGrafico::itemChange(change, value);

 if(change==ItemPositionHasChanged)
  emit s_objetoMovido();

 return(value);
}
//***********************************************************
