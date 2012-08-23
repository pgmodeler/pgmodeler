/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
# Sub-projeto: Biblioteca núcleo libpgsqldbm
# Descrição:  Definição da classe EspacoTabela que é usada para
#             gerar os códigos SQL pertinentes aos tablespaces
#             em banco de dados.
#
# Data de Criação: 05/06/2008
#
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
#ifndef ESPACOTABELA_H
#define ESPACOTABELA_H

#include "objetobase.h"
//***********************************************************
class EspacoTabela: public ObjetoBase{
 private:
  //Diretório em que se encontra o espaco de tabela
  QString diretorio;

 public:
  EspacoTabela(void);

  //Define o nome do espaço de tabela
  void definirNome(const QString &nome);

  //Define o diretório do tablespace
  void definirDiretorio(const QString &diretorio);

  //Retorna o diretório configurado para o espaço de tabela
  QString obterDiretorio(void);

  //Retorna a definição SQL ou XML do objeto
  QString obterDefinicaoObjeto(unsigned tipo_def);
};
//***********************************************************
#endif
