#include "parserxml.h"
//**********************************************************
QString ParserXML::nome_doc_xml="";
QString ParserXML::buffer_xml="";
QString ParserXML::decl_dtd="";
QString ParserXML::decl_xml="";
xmlNode *ParserXML::elem_raiz=NULL;
xmlNode *ParserXML::elem_atual=NULL;
xmlDoc *ParserXML::doc_xml=NULL;
stack<xmlNode*> ParserXML::pilha_elems;

const QString ParserXML::CHR_ECOMERCIAL="&amp;";
const QString ParserXML::CHR_MENORQUE="&lt;";
const QString ParserXML::CHR_MAIORQUE="&gt;";
const QString ParserXML::CHR_ASPAS="&quot;";
const QString ParserXML::CHR_APOSTROFO="&apos;";
//----------------------------------------------------------
ParserXML::~ParserXML(void)
{
 reiniciarParser();
}
//----------------------------------------------------------
void ParserXML::removerDTD(void)
{
 int pos1=-1, pos2=-1, pos3=-1, tam;

 if(!buffer_xml.isEmpty())
 {
  /* Retira a DTD atual do documento.
     Caso o usuário tente manipular a estrutura do
     documento prejudicando a integridade do mesmo. */
  pos1=buffer_xml.find("<!DOCTYPE");
  pos2=buffer_xml.find("]>\n");
  pos3=buffer_xml.find("\">\n");
  if(pos1 >=0 && (pos2 >=0 || pos3 >= 0))
  {
   tam=((pos2 > pos3) ? (pos2-pos1)+3 :  (pos3-pos2)+3);
   buffer_xml.replace(pos1,tam,"");
  }
 }
}
//----------------------------------------------------------
void ParserXML::carregarArquivoXML(const QString &nome_arq)
{
 try
 {
  ifstream entrada;
  QString buffer, str_aux;
  string lin;

  //Caso o nome do arquivo seja especificado
  if(nome_arq!="")
  {
   //Abre o arquivo para leitura
   entrada.open(nome_arq.toAscii(),ios_base::in);

   //Caso o arquivo foi aberto com sucesso
   if(!entrada.is_open())
   {
    str_aux=QString(Excecao::obterMensagemErro(ERR_PARSERS_ARQDIRNAOCARREGADO)).arg(nome_arq);
    throw Excecao(str_aux,ERR_PARSERS_ARQDIRNAOCARREGADO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   buffer="";

   //Lê cada linha e armazena em um buffer
   while(!entrada.eof())
   {
    getline(entrada, lin);
    buffer+=QString::fromStdString(lin) + "\n";
   }
   entrada.close();

   nome_doc_xml=nome_arq;
   carregarBufferXML(buffer);
  }
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void ParserXML::carregarBufferXML(const QString &buf_xml)
{
 try
 {
  int pos1=-1, pos2=-1, tam=0;

  if(buf_xml.isEmpty())
   throw Excecao(ERR_PARSERS_ATRIBBUFXMLVAZIO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  pos1=buf_xml.find("<?xml");
  pos2=buf_xml.find("?>");
  buffer_xml=buf_xml;

  if(pos1 >= 0 && pos2 >= 0)
  {
   tam=(pos2-pos1)+3;
   decl_xml=buffer_xml.mid(pos1, tam);
   buffer_xml.replace(pos1,tam,"");
  }
  else
   decl_xml="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

  removerDTD();
  interpretarBuffer();
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void ParserXML::definirArquivoDTD(const QString &arq_dtd, const QString &nome_dtd)
{
 if(arq_dtd.isEmpty())
  throw Excecao(ERR_PARSERS_ATRIBARQDTDVAZIO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(nome_dtd.isEmpty())
  throw Excecao(ERR_PARSERS_ATRIBNOMEDTDVAZIO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 decl_dtd="<!DOCTYPE " + nome_dtd + " SYSTEM " + "\"" +  arq_dtd + "\">\n";
}
//----------------------------------------------------------
void ParserXML::interpretarBuffer(void)
{
 QString buffer, msg, arquivo;
 xmlError *erro_xml=NULL;
 int parser_opt;

 if(!buffer_xml.isEmpty())
 {
  //Insere o cabeçalho do arquivo XML
  buffer+=decl_xml;

  //Configura o parser, inicialmente, para não validar o documento
  parser_opt=( XML_PARSE_NOBLANKS | XML_PARSE_NONET | XML_PARSE_NOENT );
  //XML_PARSE_NOERROR | XML_PARSE_NOWARNING

  //Caso a declaração DTD não esteja vazia
  if(!decl_dtd.isEmpty())
  {
   //Insere a declaração DTD do software ao buffer do documento XML
   buffer+=decl_dtd;

   //Configurando o parser para validar o documento com a DTD
   parser_opt=(parser_opt | XML_PARSE_DTDLOAD | XML_PARSE_DTDVALID);
  }

  buffer+=buffer_xml;

  //Lê o documento XML armazenado no buffer
  doc_xml=xmlReadMemory(buffer.toStdString().c_str(), buffer.size(),
                        NULL, NULL, parser_opt);

  //Obtém o último erro XML
  erro_xml=xmlGetLastError();

  //Caso haja algum erro dispara uma exceção
  if(erro_xml)
  {
   //Configurando a mensagem de erro
   msg=erro_xml->message;
   arquivo=erro_xml->file;
   if(!arquivo.isEmpty()) arquivo="("+arquivo+")";

   //Remove o \n final da mensagem original
   msg.replace("\n"," ");

   //Liberando a memória ocupada pelo documento
   if(doc_xml) reiniciarParser();

   throw Excecao(QString(Excecao::obterMensagemErro(ERR_PARSERS_LIBXMLERR))
                 .arg(erro_xml->line).arg(erro_xml->int2).arg(msg).arg(arquivo),
                 ERR_PARSERS_LIBXMLERR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  //Obtém a raiz da árvore de elementos gerada
  elem_raiz=elem_atual=xmlDocGetRootElement(doc_xml);

  //Limpa as variáveis alocadas pelo parser
  if(doc_xml) xmlCleanupParser();
 }
}
//----------------------------------------------------------
void ParserXML::salvarPosicao(void)
{
 if(!elem_raiz)
  throw Excecao(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 pilha_elems.push(elem_atual);
}
//----------------------------------------------------------
void ParserXML::restaurarPosicao(void)
{
 if(!elem_raiz)
  throw Excecao(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(pilha_elems.empty())
  elem_atual=elem_raiz;
 else
 {
  elem_atual=pilha_elems.top();
  pilha_elems.pop();
 }
}
//----------------------------------------------------------
void ParserXML::restaurarPosicao(const xmlNode *elem)
{
 if(!elem)
  throw Excecao(ERR_PARSERS_OPRELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(elem->doc!=doc_xml)
  throw Excecao(ERR_PARSERS_OPRELEMINVARVDOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 reiniciarNavegacao();
 elem_atual=const_cast<xmlNode *>(elem);
}
//----------------------------------------------------------
void ParserXML::reiniciarNavegacao(void)
{
 if(!elem_raiz)
  throw Excecao(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 elem_atual=elem_raiz;

 while(!pilha_elems.empty())
  pilha_elems.pop();
}
//----------------------------------------------------------
void ParserXML::reiniciarParser(void)
{
 elem_raiz=elem_atual=NULL;
 if(doc_xml)
 {
  xmlFreeDoc(doc_xml);
  doc_xml=NULL;
 }
 decl_dtd=buffer_xml=decl_xml="";

 while(!pilha_elems.empty())
  pilha_elems.pop();

 xmlResetLastError();
 nome_doc_xml="";
}
//----------------------------------------------------------
bool ParserXML::acessarElemento(unsigned tipo_elem)
{
 bool possui_elem;
 xmlNode *vet_elems[4];

 if(!elem_raiz)
  throw Excecao(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 vet_elems[ELEMENTO_RAIZ]=elem_atual->parent;
 vet_elems[ELEMENTO_FILHO]=elem_atual->children;
 vet_elems[ELEMENTO_POSTERIOR]=elem_atual->next;
 vet_elems[ELEMENTO_ANTERIOR]=elem_atual->prev;

 /* Verifica se o elemento atual possui o elemento que
    se deseja acessar. O flag possui_elem também é usado
    no retorno do método para indicar se o elemento foi
    movido ou não. */
 possui_elem=possuiElemento(tipo_elem);

 //Caso possua o elemento
 if(possui_elem)
  //Move o elemento atual para o elemento escolhido
  elem_atual=vet_elems[tipo_elem];

 return(possui_elem);
}
//----------------------------------------------------------
bool ParserXML::possuiElemento(unsigned tipo_elem)
{
 if(!elem_raiz)
  throw Excecao(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(tipo_elem==ELEMENTO_RAIZ)
  /* Retorna a verificação se o elemento atual possui um pai.
     O elemento deve ser diferente da raiz, pois o elemento raiz
     nao está ligado a um pai */
  return(elem_atual!=elem_raiz && elem_atual->parent!=NULL);
 else if(tipo_elem==ELEMENTO_FILHO)
  //Retorna a verificação se o elemento atual possui filhos
  return(elem_atual->children!=NULL);
 else if(tipo_elem==ELEMENTO_POSTERIOR)
  return(elem_atual->next!=NULL);
 else
  /* A segunda comparação na expressão é feita para o elemento raiz.
     pois a libxml2 coloca como elemento anterior da raiz ela mesma */
  return(elem_atual->prev!=NULL && elem_atual->prev!=elem_raiz);
}
//----------------------------------------------------------
bool ParserXML::possuiAtributos(void)
{
 if(!elem_raiz)
  throw Excecao(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(elem_atual->properties!=NULL);
}
//----------------------------------------------------------
QString ParserXML::obterConteudoElemento(void)
{
 if(!elem_raiz)
  throw Excecao(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso esteja alocado, converte para char * e retorna em forma de std::QString
 return(QString(reinterpret_cast<char *>(elem_atual->content)));
}
//----------------------------------------------------------
QString ParserXML::obterNomeElemento(void)
{
 if(!elem_raiz)
  throw Excecao(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(QString(reinterpret_cast<const char *>(elem_atual->name)));
}
//----------------------------------------------------------
xmlElementType ParserXML::obterTipoElemento(void)
{
 if(!elem_raiz)
  throw Excecao(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(elem_atual->type);
}
//----------------------------------------------------------
const xmlNode *ParserXML::obterElementoAtual(void)
{
 return(elem_atual);
}
//----------------------------------------------------------
void ParserXML::obterAtributosElemento(map<QString, QString> &atributos)
{
 xmlAttr *atribs_elem=NULL;
 QString atrib, valor;

 if(!elem_raiz)
  throw Excecao(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Limpa a lista de atributos passada
 atributos.clear();

 //Obtém a referência às propriedades do elemento
 atribs_elem=elem_atual->properties;

 //Varre a lista de propriedades até o seu final
 while(atribs_elem)
 {
  //Obtém o nome do atributo, covertendo para utf8
  atrib=QString(reinterpret_cast<const char *>(atribs_elem->name));
  //Obtém o valor atributo, covertendo para utf8
  valor=QString(reinterpret_cast<char *>(atribs_elem->children->content));

  /* Atribui ao mapa de atributos, no índice especificado pelo próprio
     nome do atributo o valor obtido */
  atributos[atrib]=valor;

  //Passa para o próximo atributo
  atribs_elem=atribs_elem->next;
 }
}
//----------------------------------------------------------
QString ParserXML::obterNomeArquivo(void)
{
 return(nome_doc_xml);
}
//----------------------------------------------------------
QString ParserXML::obterBufferXML(void)
{
 return(buffer_xml);
}
//----------------------------------------------------------
int ParserXML::obterLinhaAtualBuffer(void)
{
 if(elem_atual)
  return(elem_atual->line);
 else
  return(0);
}
//----------------------------------------------------------
int ParserXML::obterNumLinhasBuffer(void)
{
 if(doc_xml)
  return(doc_xml->last->line);
 else
  return(0);
}
//**********************************************************
