#include "formrestauracaomodelo.h"

FormRestauracaoModelo::FormRestauracaoModelo(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
 setupUi(this);
 connect(restaurar_btn, SIGNAL(clicked(void)), this, SLOT(accept(void)));
 connect(cancelar_btn, SIGNAL(clicked(void)), this, SLOT(reject(void)));
 connect(arqs_tmps_lst, SIGNAL(itemSelectionChanged()), this, SLOT(habilitarRestauracao(void)));
}

QStringList FormRestauracaoModelo::obterModelosTemporarios(void)
{
 //Retorna se na pasta tmp existem arquivos *.dbm
 return(QDir(GlobalAttributes::TEMPORARY_DIR, "*.dbm", QDir::Name, QDir::Files | QDir::NoDotAndDotDot).entryList());
}

void FormRestauracaoModelo::exec(void)
{
 QStringList lista_arqs=this->obterModelosTemporarios();
 QFileInfo info;
 QListWidgetItem *item=NULL;

 //Caso haja arquivos na pasta tmp
 while(!lista_arqs.isEmpty())
 {
  //Obtém as informações do arquivo
  info.setFile(GlobalAttributes::TEMPORARY_DIR, lista_arqs.front());

  //Cria um item da lista no formato [NOME] - [DATA CRIAÇÃO] - [TAMANHO]
  item=new QListWidgetItem;
  item->setText(info.fileName() +
                QString(" - ") +
                trUtf8("Modified: %1").arg(info.lastModified().toString(tr("yyyy-MM-dd hh:mm:ss"))) +
                QString(" - ") +
                QString("%1 bytes").arg(info.size()));
  //Armazena o caminho completo para o arquivo como dado o item
  item->setData(Qt::UserRole, QVariant::fromValue<QString>(GlobalAttributes::TEMPORARY_DIR +
                                                           GlobalAttributes::DIR_SEPARATOR + lista_arqs.front()));
  arqs_tmps_lst->addItem(item);
  item->setSelected(true);
  lista_arqs.pop_front();
 }

 QDialog::exec();
}

bool FormRestauracaoModelo::existeModelosTemporarios(void)
{
 return(!this->obterModelosTemporarios().isEmpty());
}

void FormRestauracaoModelo::excluirModelosTemporarios(void)
{
 QStringList lista_arqs=this->obterModelosTemporarios();
 QDir arq_tmp;

 //Varre a lista de arquivos temporários excluindo cada um
 while(!lista_arqs.isEmpty())
 {
  arq_tmp.remove(GlobalAttributes::TEMPORARY_DIR + GlobalAttributes::DIR_SEPARATOR + lista_arqs.front());
  lista_arqs.pop_front();
 }
}

void FormRestauracaoModelo::habilitarRestauracao(void)
{
 restaurar_btn->setEnabled(!arqs_tmps_lst->selectedItems().isEmpty());
}

QStringList FormRestauracaoModelo::obterModelosSelecionados(void)
{
 QStringList lista;
 QList<QListWidgetItem *> itens;

 itens=arqs_tmps_lst->selectedItems();

 /* Adiciona a uma lista o dado de cada item selecionado o qual armazena
    o caminho completo para o arquivo temporário */
 while(!itens.isEmpty())
 {
  lista.push_back(itens.front()->data(Qt::UserRole).asString());
  itens.pop_front();
 }

 return(lista);
}

