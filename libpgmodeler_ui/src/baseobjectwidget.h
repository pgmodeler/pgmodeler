/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler UI library (libpgmodeler_ui)
# Class: BaseObjectWidget
# Description: Implements the basic operations to create/edit database objects via form.
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/
#ifndef BASE_OBJECT_WIDGET_H
#define BASE_OBJECT_WIDGET_H

#include <QtGui>
#include "databasemodel.h"
#include "operationlist.h"
#include "formbasico.h"
#include "seletorobjetowidget.h"
#include "ui_baseobjectwidget.h"

/* Declarando a classe TipoPgSQL como metatype para que esta
 possa ser usada em conjunto com a classe QVariant (vide documentação
 da classe QVariant e QMetaType). Esta declaração é uma macro específica
 do Qt e está sendo usada para facilitar o uso com classes que necessitam
 armazenar Tipos pgsql em seus containeres (TabelaObjetosWidget).
 Essa declaração não afeta o comportamento das demais classes que de algum
 modo referenciam a classe TipoPgSQL.*/
#include <QMetaType>
Q_DECLARE_METATYPE(PgSQLType)

class BaseObjectWidget: public QDialog, public Ui::BaseObjectWidget {
 Q_OBJECT

 private:
   /* Armazena a altura mínima e máxima da janela pai.
      Estes atributos são usados para controlar a exibição do
      frame de alerta de objeto protegido e redimensionamento
      da janela pai quando este está visível */
   int pf_min_height, pf_max_height;

 protected:
   static const QColor PROT_LINE_BGCOLOR,
                       PROT_LINE_FGCOLOR,
                       RELINC_LINE_BGCOLOR,
                       RELINC_LINE_FGCOLOR;

   //Janela que sustenta todos os widgets do formulário
   FormBasico *parent_form;

   //Modelo de banco de dados de referência
   DatabaseModel *model;

   //Tabela de referência (usado em formulário de objeto de tabela)
   Table *table;

   QString prev_name;

   /* Relacionamento de referência (pode ser usado em formulários
      de coluna e restrição relacionamentos tabela-tabela podem
      ter colunas (atributos) e restriões nos atributos */
   Relationship *relationship;

   //Lista de operações de referência
   OperationList *op_list;

   //Objeto que está sendo editado ou criado
   BaseObject *object;

   /* Armazena a posição do objeto no momento da chamado do
      formulário (apenas para objetos gráficos) */
   float object_px, object_py;

   QGridLayout *baseobject_grid;

   /* Indica se o objeto é novo (que não se trata de um objeto já
      existente e não está sendo editado) */
   bool new_object;

   /* Armazena o destacador de sintaxe que é usado para destacar
      o nome do objeto pai */
   DestaqueSintaxe *hl_parentname_txt;

   //Seletores de esquema, dono e espaço de tabela
   SeletorObjetoWidget *schema_sel,
                       *owner_sel,
                       *tablespace_sel;

   /* Constantes usadas para gerar strings intervalos de versões.
      Estas constantes deve ser aplicadas ao método gerarIntervaloVersoes()
      pois é um metodo que gera chaves para os mapas de campos passados
      para o método gerarFrameAlertaVersao() */
   static const unsigned UNTIL_VERSION=0,
                         VERSIONS_INTERVAL=1,
                         AFTER_VERSION=2;

   /* Gera strings de intervalos de versões para servirem de informativo ao
      usuário no método gerarFrameAlertaVersao().
      Os intervalos gerados têm um dos seguinte formatos:

      * ver_ini <= ver_fim - Intervalo de versões
      * <= ver_ini - Até a versão indicada
      * >= ver_ini - Igual ou após a versão indicada. */
   static QString generateVersionsInterval(unsigned ver_interv_id, const QString &ini_ver, const QString &end_ver="");

   /* Gera o frame de alerta dos campos específicos das versões do pgsql.
      Os mapas de campos manipulados por este método devem ter como chaves
      as versões do pgsql os quais os campos são obrigatório. Recomenda-se
      o uso das constantes de versão da classe ParserEsquema */
   QFrame *generateVersionWarningFrame(map<QString, vector<QWidget *> > &fields, map<QWidget *, vector<QString> > *values=NULL);

   //Gera um frame informativo com icone de informação e a mensagem
   QFrame *generateInformationFrame(const QString &msg);

   /* Faz a mesclagem dos layouts de formulário desta classe base (objetobase_grid)
      com os formulários das classes filhas desta no caso o parâmetro 'grid'.
      Isso é feito para que os campos comuns a todas as classes filhas os quais
      pertencem a esta classe estejam acessíveis. O parâmetro 'tipo_obj' é usado
      para esconder alguns campos do formulário básico quando estes não se aplicam
      ao tipo de objeto informado */
   void configureFormLayout(QGridLayout *grid=NULL, ObjectType obj_type=BASE_OBJECT);

   /* Este método se aplica aos tipos de objetos diferentes do tipo OBJETO_BANCO_DADOS
      e que são passíveis de alocação e e desalocação, pois este método faz a cópia do
      objeto em edição para a lista de operação no caso de edição ou aloca um novo
      objeto no caso de criação de um novo elemento.*/
   template<class Classe>
   void startConfiguration(void);

   /* Finaliza a edição/criação do objeto, inserindo o mesmo na lista de operações
      para possibilitar a reversão da criação do mesmo. Além disso, procede
      com a emissão do sinal de objeto manipulado e fechamento do formulário.*/
   void finishConfiguration(void);

   /* Aborta a configuração do objeto, desalocando o mesmo em caso de novo objeto,
      restaurando o objeto a seu estado original antes da edição
      e removendo a operação de lista de operações referente a este objeto */
   virtual void cancelConfiguration(void);

   /* Aplica as configurações básicas (nome, esquema, comentario, espaço tabela e dono)
      ao objeto que está sendo modificado */
   virtual void applyConfiguration(void);

 public:
   BaseObjectWidget(QWidget * parent = 0, ObjectType obj_type=BASE_OBJECT);
   virtual ~BaseObjectWidget(void);

   void hideEvent(QHideEvent *);
   void showEvent(QShowEvent *);

   virtual void setAttributes(DatabaseModel *model, OperationList *op_list,
                              BaseObject *object, BaseObject *parent_obj=NULL,
                              float obj_px=NAN, float obj_py=NAN);

 protected slots:
   void editPermissions(void);

 public slots:
   void show(void);

 signals:
   void s_objectManipulated(void);
};

template<class Classe>
void BaseObjectWidget::startConfiguration(void)
{
 try
 {
  Classe *novo_obj_tmpl=NULL;

  if(this->object && op_list &&
     this->object->getObjectType()!=OBJ_DATABASE)
  {
   if(this->table)
    op_list->registerObject(this->object, Operation::OBJECT_MODIFIED, -1, this->table);
   else
    op_list->registerObject(this->object, Operation::OBJECT_MODIFIED, -1, this->relationship);
   new_object=false;
  }
  /* Caso o formulário esteja sendo usado para criar um novo
    objeto ou seja o ponteiro this->objeto está nulo */
  else if(!this->object)
  {
   //Aloca um novo esquema e o atribui ao objeto this->objeto
   novo_obj_tmpl=new Classe;
   this->object=novo_obj_tmpl;
   //Marca o flag indicando que um novo objeto foi criado
   new_object=true;
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

#endif
