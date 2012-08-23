/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca pgsqldbm_ui
# Classe: ObjetoBaseWidget
# Descrição: Definição da classe que implementa operações básicas
#            dos formulários de criação e edição de objetos no modelo
#            de banco de dados.
#
# Copyleft 2006-2012 - Raphael Araujo e Silva <rkhaotix@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License

#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef OBJETO_BASE_WIDGET_H
#define OBJETO_BASE_WIDGET_H

#include <QtGui>
#include "modelobd.h"
#include "listaoperacoes.h"
#include "formbasico.h"
#include "seletorobjetowidget.h"
#include "ui_objetobasewidget.h"

/* Declarando a classe TipoPgSQL como metatype para que esta
 possa ser usada em conjunto com a classe QVariant (vide documentação
 da classe QVariant e QMetaType). Esta declaração é uma macro específica
 do Qt e está sendo usada para facilitar o uso com classes que necessitam
 armazenar Tipos pgsql em seus containeres (TabelaObjetosWidget).
 Essa declaração não afeta o comportamento das demais classes que de algum
 modo referenciam a classe TipoPgSQL.*/
#include <QMetaType>
Q_DECLARE_METATYPE(TipoPgSQL)
//***********************************************************
class ObjetoBaseWidget: public QDialog, public Ui::ObjetoBaseWidget {
 Q_OBJECT

 private:
   /* Armazena a altura mínima e máxima da janela pai.
      Estes atributos são usados para controlar a exibição do
      frame de alerta de objeto protegido e redimensionamento
      da janela pai quando este está visível */
   int alt_min_jp, alt_max_jp;

 protected:
   static const QColor COR_FUNDO_LIN_PROT,
                       COR_TEXTO_LIN_PROT,
                 COR_FUNDO_LIN_INCREL,
                 COR_TEXTO_LIN_INCREL;

   //Janela que sustenta todos os widgets do formulário
   FormBasico *janela_pai;

   //Modelo de banco de dados de referência
   ModeloBD *modelo;

   //Tabela de referência (usado em formulário de objeto de tabela)
   Tabela *tabela;

   /* Relacionamento de referência (pode ser usado em formulários
      de coluna e restrição relacionamentos tabela-tabela podem
      ter colunas (atributos) e restriões nos atributos */
   Relacionamento *relacionamento;

   //Lista de operações de referência
   ListaOperacoes *lista_op;

   //Objeto que está sendo editado ou criado
   ObjetoBase *objeto;

   /* Armazena a posição do objeto no momento da chamado do
      formulário (apenas para objetos gráficos) */
   float px_objeto, py_objeto;

   QGridLayout *objetobase_grid;

   /* Indica se o objeto é novo (que não se trata de um objeto já
      existente e não está sendo editado) */
   bool novo_obj;

   /* Armazena o destacador de sintaxe que é usado para destacar
      o nome do objeto pai */
   DestaqueSintaxe *dest_nomepai_txt;

   //Seletores de esquema, dono e espaço de tabela
   SeletorObjetoWidget *sel_esquema,
                       *sel_dono,
                       *sel_esptabela;

   /* Constantes usadas para gerar strings intervalos de versões.
      Estas constantes deve ser aplicadas ao método gerarIntervaloVersoes()
      pois é um metodo que gera chaves para os mapas de campos passados
      para o método gerarFrameAlertaVersao() */
   static const unsigned ATE_VERSAO=0,
                         INTER_VERSOES=1,
                         APOS_VERSAO=2;

   /* Gera strings de intervalos de versões para servirem de informativo ao
      usuário no método gerarFrameAlertaVersao().
      Os intervalos gerados têm um dos seguinte formatos:

      * ver_ini <= ver_fim - Intervalo de versões
      * <= ver_ini - Até a versão indicada
      * >= ver_ini - Igual ou após a versão indicada. */
   static QString gerarIntervaloVersoes(unsigned tipo_intervalo, const QString &ver_ini, const QString &ver_fim="");

   /* Gera o frame de alerta dos campos específicos das versões do pgsql.
      Os mapas de campos manipulados por este método devem ter como chaves
      as versões do pgsql os quais os campos são obrigatório. Recomenda-se
      o uso das constantes de versão da classe ParserEsquema */
   QFrame *gerarFrameAlertaVersao(map<QString, vector<QWidget *> > &campos, map<QWidget *, vector<QString> > *valores=NULL);

   //Gera um frame informativo com icone de informação e a mensagem
   QFrame *gerarFrameInformacao(const QString &mensagem);

   /* Faz a mesclagem dos layouts de formulário desta classe base (objetobase_grid)
      com os formulários das classes filhas desta no caso o parâmetro 'grid'.
      Isso é feito para que os campos comuns a todas as classes filhas os quais
      pertencem a esta classe estejam acessíveis. O parâmetro 'tipo_obj' é usado
      para esconder alguns campos do formulário básico quando estes não se aplicam
      ao tipo de objeto informado */
   void configurarLayouFormulario(QGridLayout *grid=NULL, TipoObjetoBase tipo_obj=OBJETO_BASE);

   /* Este método se aplica aos tipos de objetos diferentes do tipo OBJETO_BANCO_DADOS
      e que são passíveis de alocação e e desalocação, pois este método faz a cópia do
      objeto em edição para a lista de operação no caso de edição ou aloca um novo
      objeto no caso de criação de um novo elemento.*/
   template<class Classe>
   void iniciarConfiguracao(void);

   /* Finaliza a edição/criação do objeto, inserindo o mesmo na lista de operações
      para possibilitar a reversão da criação do mesmo. Além disso, procede
      com a emissão do sinal de objeto manipulado e fechamento do formulário.*/
   void finalizarConfiguracao(void);

   /* Aborta a configuração do objeto, desalocando o mesmo em caso de novo objeto,
      restaurando o objeto a seu estado original antes da edição
      e removendo a operação de lista de operações referente a este objeto */
   void cancelarConfiguracao(void);

   /* Aplica as configurações básicas (nome, esquema, comentario, espaço tabela e dono)
      ao objeto que está sendo modificado */
   void aplicarConfiguracao(void);

 public:
   ObjetoBaseWidget(QWidget * parent = 0, TipoObjetoBase tipo_obj=OBJETO_BASE);
  ~ObjetoBaseWidget(void);
   void hideEvent(QHideEvent *);
   void showEvent(QShowEvent *);
   void definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op,
                         ObjetoBase *objeto, ObjetoBase *objeto_pai=NULL,
                         float px_objeto=NAN, float py_objeto=NAN);

 protected slots:
   void editarPermissoes(void);

 public slots:
   void show(void);

 signals:
   void s_objetoManipulado(void);
};
//----------------------------------------------------------
template<class Classe>
void ObjetoBaseWidget::iniciarConfiguracao(void)
{
 try
 {
  Classe *novo_obj_tmpl=NULL;

  if(this->objeto && lista_op &&
     this->objeto->obterTipoObjeto()!=OBJETO_BANCO_DADOS)
  {
   if(this->tabela)
    lista_op->adicionarObjeto(this->objeto, Operacao::OBJETO_MODIFICADO, -1, this->tabela);
   else
    lista_op->adicionarObjeto(this->objeto, Operacao::OBJETO_MODIFICADO, -1, this->relacionamento);
   novo_obj=false;
  }
  /* Caso o formulário esteja sendo usado para criar um novo
    objeto ou seja o ponteiro this->objeto está nulo */
  else if(!this->objeto)
  {
   //Aloca um novo esquema e o atribui ao objeto this->objeto
   novo_obj_tmpl=new Classe;
   this->objeto=novo_obj_tmpl;
   //Marca o flag indicando que um novo objeto foi criado
   novo_obj=true;
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//***********************************************************
#endif
