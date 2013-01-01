#include "ogtabelabase.h"

OGTabelaBase::OGTabelaBase(BaseTable *tab_base) : BaseObjectView(tab_base)
{
 if(!tab_base)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 corpo=new QGraphicsPolygonItem;
 titulo=new OGTituloObjeto;
 this->addToGroup(corpo);
 this->addToGroup(titulo);
}

OGTabelaBase::~OGTabelaBase(void)
{
 this->removeFromGroup(corpo);
 this->removeFromGroup(titulo);
 delete(corpo);
 delete(titulo);
}

QVariant OGTabelaBase::itemChange(GraphicsItemChange change, const QVariant &value)
{
 //Executa o método itemChange() da classe superior
 BaseObjectView::itemChange(change, value);

 if(change==ItemPositionHasChanged)
  emit s_objetoMovido();

 return(value);
}

