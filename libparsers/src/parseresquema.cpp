#include "parseresquema.h"
//**********************************************************
const char ParserEsquema::CHR_COMENTARIO='#';
const char ParserEsquema::CHR_FIM_LINHA='\n';
const char ParserEsquema::CHR_TABULACAO='\t';
const char ParserEsquema::CHR_ESPACO=' ';
const char ParserEsquema::CHR_INI_ATRIBUTO='@';
const char ParserEsquema::CHR_MEIO_ATRIBUTO='{';
const char ParserEsquema::CHR_FIM_ATRIBUTO='}';
const char ParserEsquema::CHR_INI_CONDICIONAL='%';
const char ParserEsquema::CHR_INI_METACHAR='$';
const char ParserEsquema::CHR_INI_TEXTOPURO='[';
const char ParserEsquema::CHR_FIM_TEXTOPURO=']';

const QString ParserEsquema::TOKEN_IF="if";
const QString ParserEsquema::TOKEN_THEN="then";
const QString ParserEsquema::TOKEN_ELSE="else";
const QString ParserEsquema::TOKEN_END="end";

const QString ParserEsquema::TOKEN_META_SP="sp";
const QString ParserEsquema::TOKEN_META_BR="br";
const QString ParserEsquema::TOKEN_META_TB="tb";

const QString ParserEsquema::VERSAO_PGSQL_80="8.0";
const QString ParserEsquema::VERSAO_PGSQL_81="8.1";
const QString ParserEsquema::VERSAO_PGSQL_82="8.2";
const QString ParserEsquema::VERSAO_PGSQL_83="8.3";
const QString ParserEsquema::VERSAO_PGSQL_84="8.4";
const QString ParserEsquema::VERSAO_PGSQL_90="9.0";
const QString ParserEsquema::VERSAO_PGSQL_91="9.1";

vector<QString> ParserEsquema::buffer;
QString ParserEsquema::arquivo="";
unsigned ParserEsquema::linha=0;
unsigned ParserEsquema::coluna=0;
unsigned ParserEsquema::qtd_coment=0;
bool ParserEsquema::ignorar_atribs_desc=false;

QString ParserEsquema::versao_pgsql=ParserEsquema::VERSAO_PGSQL_91;

void ParserEsquema::definirVersaoPgSQL(const QString &versao_pgsql)
{
 ParserEsquema::versao_pgsql=versao_pgsql;
}

QString ParserEsquema::obterVersaoPgSQL(void)
{
 return(ParserEsquema::versao_pgsql);
}

void ParserEsquema::obterVersoesPgSQL(vector<QString> &vet_versoes)
{
 QDir diretorio;
 QString caminho, str_aux;
 QStringList lista_dirs;
 /* Expressão regular que define o formato aceito para nomes de diretórios os quais são
    considerados diretórios contendo arquivos de equemas de objetos */
 QRegExp exp_reg("([0-9]+)(\\.)([0-9]+)((\\.)([0-9]+))*");
 int qtd, i;

 //Configura o caminho para o diretório convertendo o mesmo para o formato nativo do sistema operacional
 caminho=AtributosGlobais::DIR_RAIZ_ESQUEMAS + AtributosGlobais::SEP_DIRETORIO +
         AtributosGlobais::DIR_ESQUEMA_SQL + AtributosGlobais::SEP_DIRETORIO;

 //Abre o diretório
 diretorio=QDir(caminho);
 //Configura o filtro de listagem para listar apenas os sub-diretórios
 diretorio.setFilter(QDir::Dirs);

 //Caso o diretorio não exista um erro será disparado
 if(!diretorio.exists())
 {
  str_aux=Excecao::obterMensagemErro(ERR_PARSERS_ARQDIRNAOCARREGADO).arg(caminho);
  throw Excecao(str_aux, ERR_PARSERS_ARQDIRNAOCARREGADO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 //Limpa a lista de versões passada
 vet_versoes.clear();
 //Obtém a listagem de sub-diretorios filtrando de acordo com a expressão regular
 lista_dirs=diretorio.entryList().filter(exp_reg);

 //Insere todos os elementos da lista de subdiretorios no vetor de versões
 qtd=lista_dirs.count();
 for(i=qtd-1; i >=0; i--)
  vet_versoes.push_back(lista_dirs[i]);
}

void ParserEsquema::reiniciarParser(void)
{
/* Limpa o buffer e reinicia os contadores de linha,
   coluna e quantidade de comentários */
 buffer.clear();
 linha=coluna=qtd_coment=0;
}

void ParserEsquema::carregarArquivo(const QString &arquivo)
{
 if(arquivo!="")
 {
  ifstream entrada;
  char buf_aux[1025];
  QString str_aux;
  int pos;

  //Abre o arquivo para leitura
  entrada.open(arquivo.toStdString().c_str());

  if(!entrada.is_open())
  {
   str_aux=Excecao::obterMensagemErro(ERR_PARSERS_ARQDIRNAOCARREGADO).arg(arquivo);
   throw Excecao(str_aux, ERR_PARSERS_ARQDIRNAOCARREGADO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  //Caso o arquivo foi aberto com sucesso
  reiniciarParser();

  //Enquanto o fim do arquivo não for alcançado
  while(!entrada.eof())
  {
   //Obtém uma linha do arquivo (até o caractere antes do \n)
   entrada.getline(buf_aux,1024);
   //str_aux=QString::fromUtf8(buf_aux);
   str_aux=buf_aux;

   /* Como o método getline descarta o \n quando a linha era apenas uma quebra
      é necessário tratá-la para que essa linha não seja perdida */
   if(str_aux=="") str_aux+=CHR_FIM_LINHA;

   /* Caso a linha inteira esteja comentada (iniciada por um caractere
      de comentário), incrementa o contadore de linhas de comentário. */
   if(str_aux[0]==CHR_COMENTARIO) qtd_coment++;

   //Procurando a posição dos demais caractres de comentário para exclusão
   pos=str_aux.find(CHR_COMENTARIO);

   //Remove os caractres a partir da posição encontrada
   if(pos >= 0)
    str_aux.remove(pos, str_aux.size());

   if(str_aux!="")
   {
    //Adiciona uma quebra de linha casa o último caractere não seja
    if(str_aux[str_aux.size()-1]!=CHR_FIM_LINHA)
     str_aux+=CHR_FIM_LINHA;

    //Adiciona a linha tratada no buffer
    buffer.push_back(str_aux);
   }
  }

  //Fecha o arquivo
  entrada.close();
 }
}

QString ParserEsquema::obterAtributo(void)
{
 QString atrib, linha_atual, str_aux;
 bool ini_atrib, fim_atrib, erro=false;

 //Obtém a linha atual do buffer
 linha_atual=buffer[linha];

 /* Só iniciará a extração de um atributo caso comece com uma @
    mesmo se o caractere atual for um delimitador de atributo */
 if(linha_atual[coluna]==CHR_INI_ATRIBUTO)
 {
  //Passa para o próximo caractere
  coluna++;

  /* O próximo caractere deve ser uma { pois este inicia a delimitação
     do nome do atributo. Caso não seja isso indica um erro */
  if(linha_atual[coluna]!=CHR_MEIO_ATRIBUTO)
   erro=true;
  else
  {
   //Marca o flag de início de atributo
   ini_atrib=true;
   //Desmarca o flag de fim de atributo
   fim_atrib=false;
   /* Passa para a próxima coluna que contém a primeira
      letra do nome do atributo */
   coluna++;

   /* Tentará extrair um atributo até que um espaço, fim de linha
      ou de atributo seja encontrado */
   while(linha_atual[coluna]!=CHR_FIM_LINHA &&
         linha_atual[coluna]!=CHR_ESPACO &&
         !fim_atrib && !erro)
   {
    if(linha_atual[coluna]!=CHR_FIM_ATRIBUTO)
     atrib+=linha_atual[coluna];
    else if(linha_atual[coluna]==CHR_FIM_ATRIBUTO && atrib!="")
     fim_atrib=true;
    else
     erro=true;
    coluna++;
   }
   /* Caso o atributo foi iniciado porém não finalizado
      ou seja, ausencia da } na sua declaração ex. @{atrib,
      gera um erro. */
   if(ini_atrib && !fim_atrib) erro=true;
  }
 }
 else erro=true;

 if(erro)
 {
  //Dispara um erro de sintaxe
  str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV))
          .arg(arquivo).arg((linha + qtd_coment + 1)).arg((coluna+1));
  throw Excecao(str_aux,ERR_PARSERS_SINTAXEINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 return(atrib);
}

QString ParserEsquema::obterPalavra(void)
{
 QString palavra, linha_atual;

 //Obtém a linha atual do buffer
 linha_atual=buffer[linha];

 /* Tentará extrair uma palavra caso o primeiro caractere não seja
    um caractere especial. */
 if(!caractereEspecial(linha_atual[coluna].toAscii()))
 {
  /* Extrái a palavra enquanto não for final de linha, espaço ou
     caractere especial */
  while(linha_atual[coluna]!=CHR_FIM_LINHA &&
        !caractereEspecial(linha_atual[coluna].toAscii()) &&
        linha_atual[coluna]!=CHR_ESPACO)
  {
   palavra+=linha_atual[coluna];
   coluna++;
  }
 }

 return(palavra);
}

QString ParserEsquema::obterTextoPuro(void)
{
 QString texto, linha_atual, str_aux;
 bool erro=false;

 //Obtém a linha atual do buffer
 linha_atual=buffer[linha];

 //Tentará extrair um texto puro caso o primeiro caractere seja um [
 if(linha_atual[coluna]==CHR_INI_TEXTOPURO)
 {
  //Passa para o próximo caractere que contém o início do texto
  coluna++;

  /* Extrái o texto enquanto o fim de texto puro (]), fim do buffer ou
     início de texto puro ([) for alcançado */
  while(linha_atual[coluna]!=CHR_FIM_TEXTOPURO &&
        linha < buffer.size() &&
        linha_atual[coluna]!=CHR_INI_TEXTOPURO)
  {
   texto+=linha_atual[coluna];

   /* Caso especial para fim de linha. Diferente dos outros elementos da
      linguagem, um texto puro pode ser extraído até o fim do buffer,
      sendo assim, este método controla as transições de linhas também */
   if(linha_atual[coluna]==CHR_FIM_LINHA)
   {
    //Passa para a próxima linha
    linha++;
    coluna=0;
    //Obtém a linha do buffer
    linha_atual=buffer[linha];
   }
   else coluna++;
  }

  if(linha_atual[coluna]==CHR_FIM_TEXTOPURO)
   coluna++;
  else
   erro=true;
 }
 else erro=true;

 if(erro)
 {
  str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV))
          .arg(arquivo).arg((linha + qtd_coment + 1)).arg((coluna+1));

  throw Excecao(str_aux,ERR_PARSERS_SINTAXEINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 return(texto);
}

QString ParserEsquema::obterCondicional(void)
{
 QString condicional, linha_atual, str_aux;
 bool erro=false;

 //Obté a linha atual do buffer
 linha_atual=buffer[linha];

 //Iniciará a extração caso um % seja encontrado
 if(linha_atual[coluna]==CHR_INI_CONDICIONAL)
 {
  /* Passa para o próximo caractere que é o início do
     do nome da palavra condicional */
  coluna++;

  /* Extrái o nome do condicional enquanto não encontrar
     espaço ou fim de linha */
  while(linha_atual[coluna]!=CHR_FIM_LINHA &&
        linha_atual[coluna]!=CHR_ESPACO)
  {
   condicional+=linha_atual[coluna];
   coluna++;
  }

  //Caso nenhuma palavra foi extraído um erro será disparado
  if(condicional=="") erro=true;
 }
 else erro=true;

 if(erro)
 {
  str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV))
          .arg(arquivo).arg(linha + qtd_coment + 1).arg(coluna+1);
  throw Excecao(str_aux,ERR_PARSERS_SINTAXEINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 return(condicional);
}

QString ParserEsquema::obterMetaCaractere(void)
{
 QString meta, linha_atual, str_aux;
 bool erro=false;

 //Obtém a linha atual do buffer
 linha_atual=buffer[linha];

 //Iniciará a extração caso um $ seja encontrado
 if(linha_atual[coluna]==CHR_INI_METACHAR)
 {
  /* Passa para o próximo caractere que é o início do
     do nome do metacaractere */
  coluna++;

  /* Extrái o nome do metacaractere enquanto não encontrar
     espaço ou fim de linha */
  while(linha_atual[coluna]!=CHR_FIM_LINHA &&
        linha_atual[coluna]!=CHR_ESPACO)
  {
   meta+=linha_atual[coluna];
   coluna++;
  }

  //Caso nenhum metacaractere foi extraído um erro será disparado
  if(meta=="") erro=true;
 }
 else erro=true;

 if(erro)
 {
  str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV))
          .arg(arquivo).arg(linha + qtd_coment + 1).arg(coluna+1);
  throw Excecao(str_aux,ERR_PARSERS_SINTAXEINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 return(meta);
}

bool ParserEsquema::caractereEspecial(char chr)
{
 return(chr==CHR_INI_ATRIBUTO || chr==CHR_MEIO_ATRIBUTO ||
        chr==CHR_FIM_ATRIBUTO || chr==CHR_INI_CONDICIONAL ||
        chr==CHR_INI_METACHAR || chr==CHR_INI_TEXTOPURO ||
        chr==CHR_FIM_TEXTOPURO);
}

QString ParserEsquema::obterDefinicaoObjeto(const QString & nome_obj, map<QString, QString> &atributos, unsigned tipo_def)
{
 if(nome_obj!="")
 {
  QString nome_arq;

  if(tipo_def==DEFINICAO_SQL)
  {
   try
   {
    //Formata o nome do arquivo
    nome_arq=AtributosGlobais::DIR_RAIZ_ESQUEMAS + AtributosGlobais::SEP_DIRETORIO +
             AtributosGlobais::DIR_ESQUEMA_SQL + AtributosGlobais::SEP_DIRETORIO + versao_pgsql +
             AtributosGlobais::SEP_DIRETORIO + nome_obj + AtributosGlobais::EXT_ESQUEMA;

    //Tenta obter a definição do objeto no caminho especificado
    return(obterDefinicaoObjeto(nome_arq, atributos));
   }
   catch(Excecao &e)
   {
    /* Caso seja detectado o erro de que o arquivo não foi encontrado, o parser
       tentará buscar o arquivo no diretório comum de esquemas de todas as versões
       do postgresql */

    //Caso a exceção seja de outro tipo senão de arquivo não carregado, redireciona o erro
    if(e.obterTipoErro()!=ERR_PARSERS_ARQDIRNAOCARREGADO)
     throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),
                   __PRETTY_FUNCTION__,__FILE__,__LINE__,&e);

    //Reconfigura o nome do arquivo para apontar para o diretório padrão
    nome_arq=AtributosGlobais::DIR_RAIZ_ESQUEMAS + AtributosGlobais::SEP_DIRETORIO +
             AtributosGlobais::DIR_ESQUEMA_SQL + AtributosGlobais::SEP_DIRETORIO +
             AtributosGlobais::DIR_ESQUEMA_COMUM + AtributosGlobais::SEP_DIRETORIO +
             nome_obj + AtributosGlobais::EXT_ESQUEMA;

    return(obterDefinicaoObjeto(nome_arq, atributos));
   }
  }
  else
  {
   //Formata o nome do arquivo
   nome_arq=AtributosGlobais::DIR_RAIZ_ESQUEMAS + AtributosGlobais::SEP_DIRETORIO +
            AtributosGlobais::DIR_ESQUEMA_XML + AtributosGlobais::SEP_DIRETORIO + nome_obj +
            AtributosGlobais::EXT_ESQUEMA;
   return(obterDefinicaoObjeto(nome_arq, atributos));
  }
 }
 else return("");
}

void ParserEsquema::ignorarAtributosDesc(bool ignorar)
{
 ignorar_atribs_desc=ignorar;
}

QString ParserEsquema::obterDefinicaoObjeto(const QString &nome_arq, map<QString,QString> &atributos)
{
 QString def_sql;

 if(nome_arq!="")
 {
  unsigned qtd_end, qtd_if;
  int nivel_if, nivel_if_ant;
  QString atrib, cond, cond_ant, palavra, meta, str_aux;
  bool erro, atrib_if;
  char chr;
  vector<bool> vet_expif, vet_tk_if, vet_tk_then, vet_tk_else;
  map<int, vector<QString> > mapa_if, mapa_else;
  vector<QString>::iterator itr, itr_end;
  vector<int> vet_nivel_ant;
  vector<QString> *vet_aux;

  arquivo=nome_arq;
  carregarArquivo(arquivo);

  //Caso o arquivo foi carregado com sucesso
  if(buffer.size() > 0)
  {
   //Inicia as variáveis
   erro=atrib_if=false;
   nivel_if=-1;
   qtd_end=qtd_if=0;

   while(linha < buffer.size())
   {
    chr=buffer[linha][coluna].toAscii();
    switch(chr)
    {
     /* Incrementa o número de linhas fazendo com que o parser
        obtenha a próxima linha do buffer para análise */
     case CHR_FIM_LINHA:
      linha++;
      coluna=0;
     break;

     case CHR_ESPACO:
      // O parser o ignorará espaços que não estejam dentro de textos puros
      while(buffer[linha][coluna]==CHR_ESPACO) coluna++;
     break;

     //Extração de um meta caractere
     case CHR_INI_METACHAR:
      meta=obterMetaCaractere();

      //Verifica se o token metacaractere extraído é válido
      if(meta!=TOKEN_META_SP && meta!=TOKEN_META_TB &&
         meta!=TOKEN_META_BR)
      {
       str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_METACHARINV))
               .arg(meta).arg(arquivo).arg(linha + qtd_coment +1).arg(coluna+1);


       throw Excecao(str_aux,ERR_PARSERS_METACHARINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }
      //Verifica se o metacaractere está na parte da expressão if (erro)
      else if(nivel_if>=0 && vet_tk_if[nivel_if] && !vet_tk_then[nivel_if])
      {
/*       sprintf(str_aux, (Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV)).c_str(),
               arquivo.c_str(), (linha + qtd_coment +1), (coluna+1));*/
       str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV))
               .arg(arquivo).arg(linha + qtd_coment +1).arg(coluna+1);

       throw Excecao(str_aux,ERR_PARSERS_SINTAXEINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }
      else
      {
       /* Convertando o metacaractere extraído para o caractere
          que este representa */
       if(meta==TOKEN_META_SP) chr=CHR_ESPACO;
       else if(meta==TOKEN_META_TB) chr=CHR_TABULACAO;
       else chr=CHR_FIM_LINHA;

       meta="";
       meta+=chr;

       //Caso o parser esteja dentro de um if/else extraíndo os tokens
       if(nivel_if>=0)
       {
        /* Caso o parser esteja na parte if,
           coloca o metacaractere no mapa de palavras
           do if atual */
        if(vet_tk_if[nivel_if] &&
           vet_tk_then[nivel_if] &&
           !vet_tk_else[nivel_if])
         mapa_if[nivel_if].push_back(meta);

        /* Caso o parser esteja na parte else,
           colocar o metacaractere no mapa de palavras
           do else atual */
        else if(vet_tk_else[nivel_if])
         mapa_else[nivel_if].push_back(meta);
       }
       else
        /* Caso não esteja num if/else, já embuto o metacaractere
           na definição sql*/
        def_sql+=meta;
      }

     break;

     //Extração de um atributo
     case CHR_INI_ATRIBUTO:
     case CHR_MEIO_ATRIBUTO:
     case CHR_FIM_ATRIBUTO:
      atrib=obterAtributo();

      //Verifica se o atributo extraido pertence �  lista de atributos passada
      if(atributos.count(atrib)==0)
      {
       if(!ignorar_atribs_desc)
       {
        str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_ATRIBUTODESC))
                .arg(atrib).arg(arquivo).arg((linha + qtd_coment +1)).arg((coluna+1));
        throw Excecao(str_aux,ERR_PARSERS_ATRIBUTODESC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }
       else
        atributos[atrib]="";
      }


      //Caso o parser esteja dentro de um if/else extraíndo os tokens
      if(nivel_if>=0)
      {
       /* Caso o parser esteja na parte da expressão do if porém ainda não
          extraiu o atributo */
       if(!atrib_if && vet_tk_if[nivel_if] && !vet_tk_then[nivel_if])
       {
        //Marca o flag indicando que um atributo do if/then já foi extraído
        atrib_if=true;
        /* Faz a comparação do valor do atributo e coloca o resultado no
           vetor de expressões if */
        vet_expif.push_back((atributos[atrib]!=""));
       }
       /* Caso o parser esteja na parte da expressão do if e já
          extraiu o atributo, retornará um erro, pois apenas um atributo
          pode aparecer na expressão if */
       else if(atrib_if && vet_tk_if[nivel_if] && !vet_tk_then[nivel_if])
       {
/*        sprintf(str_aux, (Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV)).c_str(),
                arquivo.c_str(), (linha + qtd_coment +1), (coluna+1));*/
        str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV))
                .arg(arquivo).arg(linha + qtd_coment +1).arg(coluna+1);

        throw Excecao(str_aux,ERR_PARSERS_SINTAXEINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }
       else
       {
        palavra=atrib;
        atrib="";
        atrib+=CHR_INI_ATRIBUTO;
        atrib+=CHR_MEIO_ATRIBUTO;
        atrib+=palavra;
        atrib+=CHR_FIM_ATRIBUTO;

        //Caso o parser esteja na parte if
        if(vet_tk_if[nivel_if] &&
           vet_tk_then[nivel_if] &&
           !vet_tk_else[nivel_if])
         //Insere o valor do atributo no mapa de palavras da seção if atual
         //mapa_if[nivel_if].push_back(atributos[atrib]);
         mapa_if[nivel_if].push_back(atrib);
        else if(vet_tk_else[nivel_if])
         //Insere o valor do atributo no mapa de palavras da seção else atual
         //mapa_else[nivel_if].push_back(atributos[atrib]);
         mapa_else[nivel_if].push_back(atrib);
       }
      }
      else
      {
       if(atributos[atrib]=="")
       {
        str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_ATRIBVALORNULO))
                .arg(atrib).arg(arquivo).arg(linha + qtd_coment +1).arg(coluna+1);

        throw Excecao(str_aux,ERR_PARSERS_ATRIBVALORNULO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }

       /* Caso o parser não esteja num if/else, concatena o valor do
          atributo diretamente na definição sql */
       def_sql+=atributos[atrib];
      }
     break;

     //Extração de instrução condicional
     case CHR_INI_CONDICIONAL:
      cond_ant=cond;
      cond=obterCondicional();

      //Verifica se o token condicional extraído é válido
      if(cond!=TOKEN_IF && cond!=TOKEN_ELSE &&
         cond!=TOKEN_THEN && cond!=TOKEN_END)
      {
       str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_CONDICIONALINV))
               .arg(cond).arg(arquivo).arg(linha + qtd_coment +1).arg(coluna+1);
       throw Excecao(str_aux,ERR_PARSERS_CONDICIONALINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }
      else
      {
       //Caso o token seja de um if
       if(cond==TOKEN_IF)
       {
        /* Insere o valor do nivel do if atual no vetor de níveis
           de ifs anteriores. Esse vetor é usado para saber em qual if
           o parser se encontrava antes de entrar no if atual */
        vet_nivel_ant.push_back(nivel_if);

        /* Marca os flags indicando que um if foi encontrado e que um
           then e um else ainda não foram */
        vet_tk_if.push_back(true);
        vet_tk_then.push_back(false);
        vet_tk_else.push_back(false);

        /* Define o nivel do if atual como sendo o tamanho da lista de
           tokens if encontrados  -1 */
        nivel_if=(vet_tk_if.size()-1);

        //Incrementa o número de ifs encontrados
        qtd_if++;
       }
       //Caso o parser esteja num if/else e um token then foi encontrado
       else if(cond==TOKEN_THEN && nivel_if>=0)
       {
        //Marca o flag do token then do if atual
        vet_tk_then[nivel_if]=true;

        /* Desmarca o flag de atributo extraído da parte if - then,
           para que o parser não gere um erro quando encontrar outro
           if - then com um atributo ainda não extraído */
        atrib_if=false;
       }
       //Caso o parser esteja num if/else e um token else foi encontrado
       else if(cond==TOKEN_ELSE && nivel_if>=0)
        //Marca o flag do token else do if atual
        vet_tk_else[nivel_if]=true;
       //Caso o parser esteja num if/else e um token end foi encontrado
       else if(cond==TOKEN_END && nivel_if>=0)
       {
        //Incrementa o número de tokes end encontrados
        qtd_end++;

        //Obtém o nível do if anterior em que o parser se encontrava
        nivel_if_ant=vet_nivel_ant[nivel_if];

        //Caso o if atual seja interno, > 0
        if(nivel_if > 0)
        {
         //Caso o if atual não esteja na parte else do if superior (nivel anterior)
         if(!vet_tk_else[nivel_if_ant])
          //Obtém o vetor de palavras extraídas no if do if superior
          vet_aux=&mapa_if[nivel_if_ant];
         else
          //Obtém o vetor de palavras extraídas no else do if superior
          vet_aux=&mapa_else[nivel_if_ant];
        }
        else
         vet_aux=NULL;

        /* Inicializa os iteradores para varredura do
           vetor auxiliar se necessário */
        itr=itr_end=mapa_if[0].end();

        /* Caso a expressão do if atual tenha valor verdadeiro
           então o parser varrerá a lista de palavras da parte if
           do if atual */
        if(vet_expif[nivel_if])
        {
         itr=mapa_if[nivel_if].begin();
         itr_end=mapa_if[nivel_if].end();
        }
        /* Caso a parte else do if atual exista
           então o parser varrerá a lista de palavras da parte else
           do if atual */
        else if(mapa_else.count(nivel_if)>0)
        {
         itr=mapa_else[nivel_if].begin();
         itr_end=mapa_else[nivel_if].end();
        }

        /* Este while varre a lista de palavras selecionada acima
           inserindo-as na parte if ou else do if superior ao atual.
           Isso é feito para que apenas as palavras extraídas com base nas
           expressões dos ifs do buffer sejam embutidas na definição sql */
        while(itr!=itr_end)
        {
         //Caso o vetor auxiliar esteja alocado, insere a palavra no if/else superior
         if(vet_aux)
          vet_aux->push_back((*itr));
         else
         {
          palavra=(*itr);

          //Verifica se a palavra não é um atributo
          if(palavra[0]==CHR_INI_ATRIBUTO)
          {
           /* Caso seja um atributo, extrái seu nome e verifica se o mesmo
              possui valor nulo*/
           //atrib=palavra.substr(2,palavra.size()-3);
           atrib=palavra.mid(2,palavra.size()-3);
           palavra=atributos[atrib];

           //Caso o atributo não possua valor definido, dispara uma exceção
           if(palavra=="")
           {
            str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_ATRIBVALORNULO))
                    .arg(atrib).arg(arquivo).arg(linha + qtd_coment +1).arg(coluna+1);
            throw Excecao(str_aux,ERR_PARSERS_ATRIBVALORNULO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
           }
          }

          //Caso contrário, insere a palavra diretamente na definição sql
          def_sql+=palavra;
         }
         itr++;
        }

        //Caso o if atual seja interno
        if(nivel_if > 0)
         //Faz com que o parser volte para o if anterior
         nivel_if=nivel_if_ant;

        /* Caso o if seja o mais superior (nivel 0), indica que todos
           os ifs já foram verificados, sendo assim o parser limpará as
           estruturas auxiliares utilizadas */
        else
        {
         mapa_if.clear();
         mapa_else.clear();
         vet_tk_if.clear();
         vet_tk_then.clear();
         vet_tk_else.clear();
         vet_expif.clear();
         vet_nivel_ant.clear();

         //Reiniciando o nível dos ifs
         nivel_if=nivel_if_ant=-1;
        }
       }
       else
        erro=true;

       if(!erro)
       {
        /* Verificando se as palavras condicionais aparecem em uma ordem válida, caso
           contrário o parser gera um erro. */
        if((cond_ant==TOKEN_IF && cond!=TOKEN_THEN) ||
           (cond_ant==TOKEN_ELSE && cond!=TOKEN_IF && cond!=TOKEN_END) ||
           (cond_ant==TOKEN_THEN && cond==TOKEN_THEN))
         erro=true;
       }

       if(erro)
       {
        str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV))
                .arg(arquivo).arg(linha + qtd_coment +1).arg(coluna+1);
        throw Excecao(str_aux,ERR_PARSERS_SINTAXEINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
       }
      }
     break;

     //Extração de texto puro ou palavra simples
     default:
      if(chr==CHR_INI_TEXTOPURO ||
         chr==CHR_FIM_TEXTOPURO)
       palavra=obterTextoPuro();
      else
       palavra=obterPalavra();

       //Caso o parser esteja em um if/else
       if(nivel_if>=0)
       {
        /* Caso a palavra/texto puro apareça na expressão if, o parser retorna um erro
           pois apenas um atributo pode aparecer na expressão if */
        if(vet_tk_if[nivel_if] && !vet_tk_then[nivel_if])
        {
         str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV))
                 .arg(arquivo).arg(linha + qtd_coment +1).arg(coluna+1);
         throw Excecao(str_aux,ERR_PARSERS_SINTAXEINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
        }
        //Caso o parser esteja na parte if
        else if(vet_tk_if[nivel_if] &&
                vet_tk_then[nivel_if] &&
                !vet_tk_else[nivel_if])
         //Insere a palavra no mapa de palavras extraída na parte if
         mapa_if[nivel_if].push_back(palavra);
        else if(vet_tk_else[nivel_if])
         //Insere a palavra no mapa de palavras extraída na parte else
         mapa_else[nivel_if].push_back(palavra);
       }
       else
       /* Caso o parser não esteja num if/else, concatena a palavra/texto puro
          diretamente na definição sql */
        def_sql+=palavra;
     break;
    }
   }

   /* Caso existam mais tokes if do que end, isso indica que algum if no código não
      foi fechado. Desta forma o parser retornará um erro */
   if(qtd_if!=qtd_end)
   {
    /*sprintf(str_aux, (Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV)).c_str(),
            arquivo.c_str(), (linha + qtd_coment +1), (coluna+1));*/
    str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_SINTAXEINV))
            .arg(arquivo).arg(linha + qtd_coment +1).arg(coluna+1);
    throw Excecao(str_aux,ERR_PARSERS_SINTAXEINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }
  }
 }

 reiniciarParser();
 ignorar_atribs_desc=false;
 return(def_sql);
}
//**********************************************************
