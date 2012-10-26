#include "destaquesintaxe.h"

DestaqueSintaxe::DestaqueSintaxe(QTextDocument *parent, bool auto_redestaque) : QSyntaxHighlighter(parent)
{
 this->auto_redestaque=auto_redestaque;
 configurarAtributos();
}

DestaqueSintaxe::DestaqueSintaxe(QTextEdit *parent, bool auto_redestaque) : QSyntaxHighlighter(parent)
{
 this->auto_redestaque=auto_redestaque;
 configurarAtributos();
}

void DestaqueSintaxe::configurarAtributos(void)
{
 conf_carregada=false;
 bloco_atual=-1;
 qtd_info_bloco_atual=0;

 if(auto_redestaque)
 {
  connect(document(), SIGNAL(blockCountChanged(int)), this, SLOT(rehighlight(void)));
  connect(document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(validarModificacaoTexto(int,int,int)));
 }
}

void DestaqueSintaxe::validarModificacaoTexto(int, int rem, int ins)
{
 unsigned qtd;

 //Obtém a quantidade de informações de multilinha do bloco atual
 qtd=obterNumInfoMultiLinha(bloco_atual);

 /* Caso a quantidade obtida seja diferente da quantidade capturada
    antes da entrada do texto pelo usuário, ou se não exista nenhuma
    informação de multilinha para o bloco atual porém foram inseridos
    ou removidos alguns caracteres, o documento passará por um redestaque
    completo pois os caracteres removidos ou inseridos podem interferir
    na forma de apresentação do destaque e por isso o documento como
    um todo precisa ser redestacado */
 if(qtd!=qtd_info_bloco_atual ||
   (/*qtd==0 && qtd_info_bloco_atual==qtd &&*/ (ins > 0 || rem > 0)))
  rehighlight();
}

DestaqueSintaxe::InfoMultiLinha *DestaqueSintaxe::obterInfoMultiLinha(int col_ini, int col_fim, int bloco)
{
 unsigned i, qtd;
 bool enc=false;
 InfoMultiLinha *info=NULL;

 /* Varre o vetor de informações de multilinha a fim de verificar se
    os parâmetros passados estão dentro de um bloco (informação) multilinha */
 qtd=info_multi_linha.size();
 for(i=0; i < qtd; i++)
 {
  //Obtém uma informação
  info=info_multi_linha[i];

  /* Primeiramente é preciso verificar se o bloco passado está dentro dos limites
     estabelecidos pelos blocos inicial e final da informação de multilinha.
     É importante salientar que quando um bloco multilinha está em aberto, por exemplo,
     o usuário abriu um multilinha com '/ *' e não fechou com '* /' os atributos
     bloco_fim e col_fim da informação de multilinha possuirão o valor -1 até
     que o usuário feche o multilinha e o destacador identifique em que parte
     do texto se encontra esse fechamento */
  if(bloco >= info->bloco_ini && (info->bloco_fim < 0 || bloco <= info->bloco_fim))
  {
   /* A seguir várias condições são testadas a fim de verificar se os parâmetros
      passados estão dentro de um bloco multilinha.*/

   /* Condição 1: O bloco passado é o mesmo da informação atual e a informação
                  atual trata de um multilinha porém aberto e fechado na mesma linha
                  de texto (info->bloco_ini==info->bloco_fim), será verificado
                  se o parâmetro col_ini e col_fim estão nos limites estabelecidos
                  pelas colunas inicial e final do informação */
   if(bloco==info->bloco_ini && info->bloco_ini==info->bloco_fim)
    enc=(col_ini >= info->col_ini && col_fim <= info->col_fim);

   /* Condição 2: O bloco passado é o mesmo da informação atual e a informação
                  atual trata de um multilinha em aberto. Testa apenas se
                  a coluna inicial do parâmetro está após da coluna inicial da
                  informação. Isso indica que o texto digitado está após a
                  abertura do multilinha e consequentemente dentro do mesmo */
   else if(bloco == info->bloco_ini)
    enc=(col_ini >= info->col_ini);

   /* Condição 3: O bloco passado é o mesmo do bloco final da informação atual
                  e a informação atual trata de um multilinha fechado. Testa
                  apenas se a coluna final do parâmetro está antes da coluna
                  final da informação multilinha atual indicando que o texto
                  inserido está dentro do bloco multilinha */
   else if(info->bloco_fim >=0 && bloco == info->bloco_fim)
    enc=(col_fim <= info->col_fim);

  /* Condição 4: A informação atual trata de um multilinha em aberto. Testa
                  apenas se o bloco passado está no mesmo bloco inicial da informação
                  ou após sem a necessidade de se testar as colunas e o bloco final.
                  Isso é feito pois se o texto é inserido no meio do multilinha
                  após o bloco de abertura, e como o bloco está em aberto, todo texto
                  inserido após o bloco de aberto será considerado um multibloco */
   else if(info->bloco_fim < 0)
    enc=(bloco >= info->bloco_ini);

  /* Condição 5: A informação atual trata de um multilinha fechado. Testa
                 apenas se o bloco passado está no meio do intervalo estabelecido
                 pelos blocos inicial e final da informação. Isso é feito
                 pois se o texto é inserido no meio do multilinha após o bloco de
                 abertura e antes do fechamento a linha do texto como um todo
                 é considerada um multibloco */
   else if(info->bloco_fim >=0 && info->bloco_ini!=info->bloco_fim)
    enc=(bloco >= info->bloco_ini && bloco <= info->bloco_fim);
  }
 }

 if(enc)
  return(info);
 else
  return(NULL);
}

void DestaqueSintaxe::removerInfoMultiLinha(int bloco)
{
 vector<InfoMultiLinha *>::iterator itr, itr_end;

 itr=info_multi_linha.begin();
 itr_end=info_multi_linha.end();

 //Varre a lista de informações de multilinha
 while(itr!=itr_end)
 {
  //Caso a info atual tenha como bloco inicial o mesmo bloco passado
  if((*itr)->bloco_ini==bloco)
  {
   //Remove a informação da memória
   delete(*itr);
   //Remove o elemento da lista e reinicia a varredura
   info_multi_linha.erase(itr);
   itr=info_multi_linha.begin();
   itr_end=info_multi_linha.end();
  }
  else
   itr++;
 }
}

unsigned DestaqueSintaxe::obterNumInfoMultiLinha(int bloco)
{
 vector<InfoMultiLinha *>::iterator itr, itr_end;
 unsigned qtd=0;

 itr=info_multi_linha.begin();
 itr_end=info_multi_linha.end();

 /* Varre a lista de informação de multilinha e contabiliza
    aquelas as quais tem como bloco inicial o bloco passado */
 while(itr!=itr_end)
 {
  if((*itr)->bloco_ini==bloco) qtd++;
  itr++;
 }

 return(qtd);
}

QString DestaqueSintaxe::identificarGrupoPalavra(const QString &palavra, const QChar &chr_lookup, int idx, int &idx_comb, int &comp_combinacao)
{
 QRegExp expr;
 vector<QString>::iterator itr, itr_end;
 vector<QRegExp>::iterator itr_exp, itr_exp_end;
 vector<QRegExp> *vet_expr=NULL;
 QString grupo;
 bool combina=false, comb_parcial=false;
 InfoMultiLinha *info=NULL;

 //Tenta obter uma informação de multilinha do bloco atual
 info=obterInfoMultiLinha(idx, idx, bloco_atual);

 /* Caso o destacador estiver no meio do destaque de um bloco
    multi linha, é executado um procedimento diferente que
    é verificar se a palavra em questão não combina com uma
    das expressões finais do grupo indicando que o detaque
    do grupo deve ser interrompido após a palavra em questão */
 if(info)
 {
  grupo=info->grupo;

  /* Varre as expresões finais exclusivamente do grupo atual
     para verificar se a palavra em questão naõ é a de finalização
     do destaque do grupo */
  itr_exp=exp_finais[grupo].begin();
  itr_exp_end=exp_finais[grupo].end();
  comb_parcial=combinacao_parcial[grupo];

  while(itr_exp!=itr_exp_end && !combina)
  {
   //Obtém a expressão do iterador atual
   expr=(*itr_exp);

    //Caso a expressão esteja configurara para combinação parcial
    if(comb_parcial)
    {
     //Obtém o índice inicial na palavra onde a expressão combina
     idx_comb=palavra.indexOf(expr);
     //Obtém o comprimento da combinação
     comp_combinacao=expr.matchedLength();
     /* Para se saber se a expressão combinou parcialmente com a palavra
        verifica se o indice da combinação é igual ou superior a zero */
     combina=(idx_comb >= 0);
    }
    else
    {
    /* Caso a expressão esteja com o tipo de padrão configurado como
       como FixedString indica que a mesmo não precisa ser tratada como
       uma expressão regular e sim como uma string comum, sendo assim
       a comparação feita é de string com string o que acaba se tornando
       mais rápido */
     if(expr.patternSyntax()==QRegExp::FixedString)
      combina=((expr.pattern().compare(palavra, expr.caseSensitivity())==0));
     else
      //Combina a expressão regular com a palavra
      combina=expr.exactMatch(palavra);

     if(combina)
     {
      idx_comb=0;
      comp_combinacao=palavra.length();
     }
    }

   if(combina && caractere_lookup.count(grupo) > 0 && chr_lookup!=caractere_lookup.at(grupo))
    combina=false;

   itr_exp++;
  }

  /* Caso a palavra combina com uma das expressões finais do grupo
     marca a informação multilinha obtida e configura a coluna final
     e bloco final da informção de bloco multilinha. */
  if(combina)
  {
   info->col_fim=idx + idx_comb + comp_combinacao-1;
   info->bloco_fim=bloco_atual;
  }
  /* Caso o destacador permaneça num bloco de multilinha o índice
     de combinação e o comprimento da combinação serão, respectivamente,
     zero e o comprimento da palavra para forçar o destaque da palavra
     atual no grupo de multi linha atual indicando que o destacador ainda
     se encontra no bloco deste tipo */
  else
  {
   idx_comb=0;
   comp_combinacao=palavra.length();
  }

  return(grupo);
 }
 else
 {
  /* Obtém os iteradores do vetor de ordem de grupos
     para que as expressões dos mesmos sejam aplicadas
       palavra em questão com o intuito de verificar se
     a mesma faz parte do grupo */
  itr=ordem_grupos.begin();
  itr_end=ordem_grupos.end();

  while(itr!=itr_end && !combina)
  {
   //Obtém o grupo a partir do iterador
   grupo=(*itr);
   //Obtém o vetor de expressões iniciais do grupo
   vet_expr=&exp_iniciais[grupo];
   itr++;

   //Varre a lista de expressões comparando com a palavra atual
   itr_exp=vet_expr->begin();
   itr_exp_end=vet_expr->end();
   comb_parcial=combinacao_parcial[grupo];

   while(itr_exp!=itr_exp_end && !combina)
   {
    //Obtém a expressão a partir do iterador atual
    expr=(*itr_exp);

    //Caso a expressão esteja configurara para combinação parcial
    if(comb_parcial)
    {
     //Obtém o índice inicial na palavra onde a expressão combina
     idx_comb=palavra.indexOf(expr);
     //Obtém o comprimento da combinação
     comp_combinacao=expr.matchedLength();
     /* Para se saber se a expressão combinou parcialmente com a palavra
        verifica se o indice da combinação é igual ou superior a zero */
     combina=(idx_comb >= 0);
    }
    else
    {
    /* Caso a expressão esteja com o tipo de padrão configurado como
       como FixedString indica que a mesmo não precisa ser tratada como
       uma expressão regular e sim como uma string comum, sendo assim
       a comparação feita é de string com string o que acaba se tornando
       mais rápido */
     if(expr.patternSyntax()==QRegExp::FixedString)
      combina=((expr.pattern().compare(palavra, expr.caseSensitivity())==0));
     else
      //Combina a expressão regular com a palavra
      combina=expr.exactMatch(palavra);

     if(combina)
     {
      idx_comb=0;
      comp_combinacao=palavra.length();
     }
    }

   if(combina && caractere_lookup.count(grupo) > 0 && chr_lookup!=caractere_lookup.at(grupo))
    combina=false;

    itr_exp++;
   }

   /* Caso a palavra combine com uma das expressões do grupo
      verifica se o mesmo possui expressões finais o que indica
      que o grupo se trata de elementos de multi linha, ou seja,
      que o destaque do grupo se extende além da linha atual até
      um delimitador final do grupo ser encontrado. Desta
      forma aloca uma informção de multilinha com configurações iniciais */
   if(combina && exp_finais.count(grupo))
   {
    if(!info)
    {
     info=new InfoMultiLinha;
     info->grupo=grupo;
     info->col_ini=idx + idx_comb + comp_combinacao;
     info->bloco_ini=bloco_atual;
     info_multi_linha.push_back(info);
    }
   }
  }

  /* Caso a palavra não combine com nenhuma expressão de nenhum
     grupo força o método a retornar um nome de grupo vazio
     indicando que a palavra não combina com grupo algum */
  if(!combina) grupo="";

  return(grupo);
 }
}

void DestaqueSintaxe::rehighlight(void)
{
 InfoMultiLinha *info=NULL;

 /* Remove todas as informações de multilinha
    pois durante o redestaque as mesmas são obtidas
    novamente */
 while(!info_multi_linha.empty())
 {
  info=info_multi_linha.back();
  info_multi_linha.pop_back();
  delete(info);
 }

 QSyntaxHighlighter::rehighlight();
}

void DestaqueSintaxe::highlightBlock(const QString &txt)
{
 /* Caso a linha esteja vazia ou consita apenas de uma
    linha em branco não executa o destacamento para não
    gerar processamento desnecessário. */
 bloco_atual=currentBlock().blockNumber();

 if(!txt.isEmpty())
 {
  QString palavra, grupo, texto;
  unsigned i=0, tam, idx=0, i1;
  int idx_comb, comp_comb, tam_aux, col_ini;
  QChar chr_delim, chr_lookup;

  //Obtém o tamanho total do bloco de texto
  texto=txt + '\n';
  tam=texto.length();

  /* Remove as informações de multilinha do bloco atual
     para forçar a identifição de novas informações
     de multilinha no bloco atual */
  removerInfoMultiLinha(bloco_atual);

  do
  {
   /* Remove os caracteres que constam na lista de caracteres
      ignorandos enquanto estes aparecerem no texto */
   while(i < tam && chr_ignorados.indexOf(texto[i])>=0) i++;

   //Caso o fim do texto não tenha sido alcançado
   if(i < tam)
   {
    /* Armazena a posição atual no texto pois é a partir dela que será
       feito o destaque da palavra extraída nas iterações abaixo */
    idx=i;

    //Caso o caractere atual seja um caractere separador de palavras
    if(sep_palavras.indexOf(texto[i])>=0)
    {
     /* Enquanto o caractere for um separado, o mesmo é concatenada junto
        com os demais separadores */
     while(i < tam && sep_palavras.indexOf(texto[i])>=0)
      palavra+=texto[i++];
    }
    //Caso o caractere atual seja um delimitador de palavras
    else if(delim_palavras.indexOf(texto[i])>=0)
    {
     //Armazena o caractere delimitador
     chr_delim=texto[i++];
     //Adiciona-o na palavra que está sendo extraída
     palavra+=chr_delim;

     /* Extrai todos os próximos caracteres concatenando-o�  palavra,
        idenpendente da categoria destes, enquanto o caractere final
        delimitador de palavra não seja encontrado ou o fim do texto
        seja alcançado. */
     while(i < tam && chr_delim!=texto[i])
      palavra+=texto[i++];

     /* Caso o caractere delimitador final for encontrado concatena-o   palavra
        formando a palavra delimitada como um todo */
     if(i < tam && texto[i]==chr_delim)
     {
      palavra+=chr_delim;
      i++;
     }
    }
    /* Caso o caractere atual não se encaixe em nenhuma das categorias
       acima, será a executada a iteração padrão, que é extrair
       o caractere até alcançar um separadaor ou delimitador de palavra
       ou um caractere a ser ignorado */
    else
    {
     while(i < tam &&
           sep_palavras.indexOf(texto[i]) < 0 &&
           delim_palavras.indexOf(texto[i]) < 0 &&
           chr_ignorados.indexOf(texto[i]) < 0)
     {
      palavra+=texto[i++];
     }
    }
   }

   /* Caso a palavra não esteja vazia, tenta localizar o grupo
      ao qual ela pertence */
   if(!palavra.isEmpty())
   {
    i1=i;
    while(i1 < tam && chr_ignorados.indexOf(texto[i1])>=0) i1++;

    if(i1 < tam)
     chr_lookup=texto[i1];
    else
     chr_lookup='\0';

    //Obtém o grupo ao qual a palavra faz parte
    idx_comb=-1;
    comp_comb=0;
    grupo=identificarGrupoPalavra(palavra,chr_lookup, idx, idx_comb, comp_comb);

    /* Caso o grupo foi identificado faz o destaque da palavra
       usando a posição inicial da palavra com o comprimento
       da mesma */
    if(!grupo.isEmpty())
    {
     col_ini=idx + idx_comb;
     setFormat(col_ini, comp_comb, formatacoes[grupo]);
    }

    tam_aux=(idx_comb + comp_comb);
    if(idx_comb >=0 &&  tam_aux != palavra.length())
     i-=palavra.length() - tam_aux;

    //Limpa a palavra atual para obter uma nova
    palavra="";
   }
  }
  while(i < tam);

  /* Armazena a quantidade de informação de multilinhas no bloco atual,
     pois este atributo é usado para se saber se o documento passará
     por um redestaque ou não */
  qtd_info_bloco_atual=obterNumInfoMultiLinha(bloco_atual);
 }
}

bool DestaqueSintaxe::configuracaoCarregada(void)
{
 return(conf_carregada);
}

void DestaqueSintaxe::limparConfiguracao(void)
{
 exp_iniciais.clear();
 exp_finais.clear();
 formatacoes.clear();
 combinacao_parcial.clear();
 ordem_grupos.clear();
 grupos_sep_palavras.clear();
 sep_palavras.clear();
 delim_palavras.clear();
 chr_ignorados.clear();
 caractere_lookup.clear();

 configurarAtributos();
}

void DestaqueSintaxe::carregarConfiguracao(const QString &nome_arq)
{
 if(nome_arq!="")
 {
  map<QString, QString> atributos;
  QString elem, tipo_exp, grupo;
  bool decl_grupos=false, sensivel_chr=false,
       negrito=false, italico=false,
       sublinhado=false, comb_parcial=false;
  QTextCharFormat formatacao;
  QRegExp exp_regular;
  QColor cor_fundo, cor_fonte;
  vector<QString>::iterator itr, itr_end;

  try
  {
   /* Caso o usuário tente carregar uma nova configuração na mesma instância,
      as configurações anteriores são descartadas */
   limparConfiguracao();

   //Reinicia o parser XML para a leitura do arquivo
   ParserXML::reiniciarParser();

   /* Montando o caminho padrão para localização do arquivo DTD que define a sintaxe
     do arquivo xml de destaque de código fonte. */
   ParserXML::definirArquivoDTD(AtributosGlobais::DIR_CONFIGURACOES +
                                AtributosGlobais::SEP_DIRETORIO +
                                AtributosGlobais::DIR_DTD_OBJETO +
                                AtributosGlobais::SEP_DIRETORIO +
                                AtributosGlobais::CONF_DESTAQUE_CODIGO +
                                AtributosGlobais::EXT_DTD_OBJETO,
                                AtributosGlobais::CONF_DESTAQUE_CODIGO);

   //Carrega o arquivo validando-o de acordo com a DTD informada
   ParserXML::carregarArquivoXML(nome_arq);

   //Acessa o elemento inicial do arquivo de destaque de código fonte
   if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
   {
    do
    {
     if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
     {
      //Obtém o elemento atual
      elem=ParserXML::obterNomeElemento();

      //Obtém os separadores de palavras da linguagem
      if(elem==AtributosParsers::SEP_PALAVRAS)
      {
       //Obtém os atributos do mesmo
       ParserXML::obterAtributosElemento(atributos);
       sep_palavras=atributos[AtributosParsers::VALOR];
      }

      //Obtém os delimitadores de palavras da linguagem
      else if(elem==AtributosParsers::DELIM_PALAVRAS)
      {
       //Obtém os atributos do mesmo
       ParserXML::obterAtributosElemento(atributos);
       delim_palavras=atributos[AtributosParsers::VALOR];
      }
      else if(elem==AtributosParsers::CARACTERES_IGNORADOS)
      {
       //Obtém os atributos do mesmo
       ParserXML::obterAtributosElemento(atributos);
       chr_ignorados=atributos[AtributosParsers::VALOR];
      }

      /* Caso o elemento seja o que define a ordem de aplicação dos grupos
         de destaque (highlight-order). É neste bloco que são declarados
         os grupos usados para destacar o código-fonte. TODOS os grupos
         precisam ser declarados neste bloco antes de serem construídos
         caso contrário será disparado um erro. */
      else if(elem==AtributosParsers::ORDEM_DESTAQUE)
      {
       //Marca a flag indicando que os grupos estão sendo declarados
       decl_grupos=true;
       //Salva a posição atual do parser xml
       ParserXML::salvarPosicao();
       /* Acesso o primeiro elemento filho da tag de ordem de destaque que
          no caso é uma tag de declaração de grupo <group> */
       ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);
       //Obtém o nome do elemento, no caso <group>
       elem=ParserXML::obterNomeElemento();
      }

      //Caso o elemento atual seja de construção de um grupo '<group>'
      if(elem==AtributosParsers::GRUPO)
      {
       //Obtém os atributos do mesmo
       ParserXML::obterAtributosElemento(atributos);
       //Armazena seu nome em uma variável auxiliar
       grupo=atributos[AtributosParsers::NOME];

       /* Caso o parser estiver no bloco de declaração de grupos e não no bloco
          de construção dos mesmos, algumas validações serão executadas. */
       if(decl_grupos)
       {
        /* 1ª Validação: Verifica se o grupo já não foi declarando anteriormente,
                         para isso o mesmo é buscado na lista que armazena a ordem
                         de aplicação dos grupos (ordem_grupos). Caso o mesmo seja
                         encontrado um erro é disparado. Um grupo é dito como localizado
                         na lista quando a chamada a função find() retorna o iterador
                         diferente do iterador final da lista 'ordem_grupos.end()' */
        if(find(ordem_grupos.begin(), ordem_grupos.end(), grupo)!=ordem_grupos.end())
        {
         //Dispara o erro indicado que o grupo está sendo redeclarado
         throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_REDECLGRUPODESTAQUE).arg(grupo),
                       ERR_PGMODELERUI_REDECLGRUPODESTAQUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
        }
        /* 2ª Validação: Verifica se o grupo está sendo declarado e construído ao mesmo tempo no
                         bloco de declaração. Um grupo no bloco de declaração deve aparecer no
                         formato <group name='nome'/>, qualquer construção diferente da apresentada
                         seja com mais atributos ou elementos filhos é considerado que o grupo está
                         sendo construído em local inválido */
        else if(atributos.size() > 1 || ParserXML::possuiElemento(ParserXML::ELEMENTO_FILHO))
        {
         throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_DEFGRUPOLOCALINV)
                       .arg(grupo).arg(AtributosParsers::ORDEM_DESTAQUE),
                       ERR_PGMODELERUI_REDECLGRUPODESTAQUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
        }

        /* Caso nenhum erro for disparado o grupo é adicionad�  lista de
           ordem de aplicação dos grupos */
        ordem_grupos.push_back(grupo);
       }
       /* Caso o parser estiver no bloco de construção de grupos e não no bloco
          de declaração dos mesmos, algumas validações iniciais serão executadas. */
       else
       {
        /* 1ª Validação: Verifica se o grupo está sendo construído pela segunda vez.
                         Para tal, verifica se o mapa de expressões do grupo foi criado
                         em alguma iteração anterior. Caso exista essa ocorrencia indica
                         que o grupo já foi construído anteriormente,
                         desta forma um erro será disparado ao usuário */
        if(exp_iniciais.count(grupo)!=0)
        {
         //Dispara o erro ao usuário indicando construção duplicada
         throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_DEFGRUPODUPLICADA).arg(grupo),
                       ERR_PGMODELERUI_DEFGRUPODUPLICADA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
        }
        /* 2ª Validação: Verifica se o grupo está sendo construído sem ter sido declarado.
                         Para tal, verifica se grupo que está sendo construído não existe
                         na lista de ordem de aplicação de grupos. Um grupo é dito como
                         não localizado na lista quando a chamada a função find() retorna
                         o iterador final da lista 'ordem_grupos.end() */
        else if(find(ordem_grupos.begin(), ordem_grupos.end(), grupo)==ordem_grupos.end())
        {
         //Dispara o erro indicando que o grupo foi construído e não declarado
         throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_DEFGRUPONAODECL)
                       .arg(grupo).arg(AtributosParsers::ORDEM_DESTAQUE),
                       ERR_PGMODELERUI_DEFGRUPONAODECL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
        }
        /* 3ª Validação: Verifica se o grupo possui elementos filhos. No bloco de construção
                         do grupo é necessário que ele possua pelo menos um filho '<element>'.
                         Caso ele não possua elementos deste tipo um erro é retornado ao usuário */
        else if(!ParserXML::possuiElemento(ParserXML::ELEMENTO_FILHO))
        {
         throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_DEFGRUPOSEMELEM).arg(grupo),
                       ERR_PGMODELERUI_DEFGRUPOSEMELEM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
        }

        //Obtém e armazena em variáveis os atributos do grupo que está sendo construído
        sensivel_chr=(atributos[AtributosParsers::SENSIVEL_CARACTERE]==AtributosParsers::VERDADEIRO);
        italico=(atributos[AtributosParsers::ITALICO]==AtributosParsers::VERDADEIRO);
        negrito=(atributos[AtributosParsers::NEGRITO]==AtributosParsers::VERDADEIRO);
        sublinhado=(atributos[AtributosParsers::SUBLINHADO]==AtributosParsers::VERDADEIRO);
        comb_parcial=(atributos[AtributosParsers::COMBINACAO_PARCIAL]==AtributosParsers::VERDADEIRO);
        cor_fonte.setNamedColor(atributos[AtributosParsers::COR_FONTE]);
        cor_fundo.setNamedColor(atributos[AtributosParsers::COR_FUNDO]);

        if(!atributos[AtributosParsers::CARACTERE_LOOKUP].isEmpty())
         caractere_lookup[grupo]=atributos[AtributosParsers::CARACTERE_LOOKUP][0];

        //Configura a formatação do grupo de acordo com os atributos obtidos
        formatacao.setFontItalic(italico);
        formatacao.setFontUnderline(sublinhado);

        if(negrito)
         formatacao.setFontWeight(QFont::Bold);
        else
         formatacao.setFontWeight(QFont::Normal);

        formatacao.setForeground(cor_fonte);
        formatacao.setBackground(cor_fundo);
        formatacoes[grupo]=formatacao;

        //Salva a posição atual do parser e acesso os elementos filhos do grupo
        ParserXML::salvarPosicao();
        ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);

        /* Configura a variável de expressão regular para ser sensível a
           caracteres (case sensitive) de acordo com o mesmo atributo
           obtido do xml */
        if(sensivel_chr)
         exp_regular.setCaseSensitivity(Qt::CaseSensitive);
        else
         exp_regular.setCaseSensitivity(Qt::CaseInsensitive);

        combinacao_parcial[grupo]=comb_parcial;

        do
        {
         if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
         {
          //Obtém os atributos do elemento filho do grupo
          ParserXML::obterAtributosElemento(atributos);
          //Obtém o tipo do elemento
          tipo_exp=atributos[AtributosParsers::TIPO];

          //Configura a expressão regular com o valor presente no atributo 'value' do elemento
          exp_regular.setPattern(atributos[AtributosParsers::VALOR]);

          if(atributos[AtributosParsers::EXP_REGULAR]==AtributosParsers::VERDADEIRO)
           exp_regular.setPatternSyntax(QRegExp::RegExp);
          else
           exp_regular.setPatternSyntax(QRegExp::FixedString);

          /* A expressão regular configura será inserida na lista de expressões
             de acordo com o tipo do elemento */
          if(tipo_exp=="" ||
             tipo_exp==AtributosParsers::EXP_SIMPLES ||
             tipo_exp==AtributosParsers::EXP_INICIAL)
           exp_iniciais[grupo].push_back(exp_regular);
          else
           exp_finais[grupo].push_back(exp_regular);
         }
        }
        while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));

        //Restaura a posição do parser para continuar a leitura dos próximos grupos
        ParserXML::restaurarPosicao();
       }
      }
     }

     /* Após a inserção do grupo, verifica se existem outros grupos a serem
        declarados. Caso não existe, desmarca a flag de declaração de grupos
        e restaura a posição do parser para que o restante do arquivo possa
        ser lido */
     if(decl_grupos && !ParserXML::possuiElemento(ParserXML::ELEMENTO_POSTERIOR))
     {
      decl_grupos=false;
      ParserXML::restaurarPosicao();
     }

    }
    while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
   }

   /* Executa a validação final do carregamento do arquivo que consiste em
      verificar se algum grupo foi declarado porém não construído. Para
      isso, a lista de ordem de grupos é varrida e verifica-se se
      existem expressões para tal grupo. Caso não exista expressões
      para o grupo indica que o mesmo foi declarado e não foi construído */
   itr=ordem_grupos.begin();
   itr_end=ordem_grupos.end();

   while(itr!=itr_end)
   {
    grupo=(*itr);
    itr++;

    //Caso o número de expressões do grupo seja zero
    if(exp_iniciais[grupo].size()==0)
    {
     //Dispara o erro indicando que o grupo foi declarado porém não construído
     throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_GRUPODECLNAODEF).arg(grupo),
                   ERR_PGMODELERUI_GRUPODECLNAODEF,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    }
   }

   //Marca a flag indicando que a configuração foi carregada com sucesso
   conf_carregada=true;
  }
  catch(Excecao &e)
  {
   //Captura e redireciona erros das demais bibliotecas
   throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
 }
}

