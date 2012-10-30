#include "xmlparser.h"

QString XMLParser::xml_doc_name="";
QString XMLParser::xml_buffer="";
QString XMLParser::dtd_decl="";
QString XMLParser::xml_decl="";
xmlNode *XMLParser::root_elem=NULL;
xmlNode *XMLParser::curr_elem=NULL;
xmlDoc *XMLParser::xml_doc=NULL;
stack<xmlNode*> XMLParser::elems_stack;

const QString XMLParser::CHAR_AMP="&amp;";
const QString XMLParser::CHAR_LT="&lt;";
const QString XMLParser::CHAR_GT="&gt;";
const QString XMLParser::CHAR_QUOT="&quot;";
const QString XMLParser::CHAR_APOS="&apos;";

XMLParser::~XMLParser(void)
{
 restartParser();
}

void XMLParser::removeDTD(void)
{
 int pos1=-1, pos2=-1, pos3=-1, tam;

 if(!xml_buffer.isEmpty())
 {
  /* Retira a DTD atual do documento.
     Caso o usuário tente manipular a estrutura do
     documento prejudicando a integridade do mesmo. */
  pos1=xml_buffer.find("<!DOCTYPE");
  pos2=xml_buffer.find("]>\n");
  pos3=xml_buffer.find("\">\n");
  if(pos1 >=0 && (pos2 >=0 || pos3 >= 0))
  {
   tam=((pos2 > pos3) ? (pos2-pos1)+3 :  (pos3-pos2)+3);
   xml_buffer.replace(pos1,tam,"");
  }
 }
}

void XMLParser::loadXMLFile(const QString &nome_arq)
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
    str_aux=QString(Exception::getErrorMessage(ERR_PARSERS_ARQDIRNAOCARREGADO)).arg(nome_arq);
    throw Exception(str_aux,ERR_PARSERS_ARQDIRNAOCARREGADO,__PRETTY_FUNCTION__,__FILE__,__LINE__);
   }

   buffer="";

   //Lê cada linha e armazena em um buffer
   while(!entrada.eof())
   {
    getline(entrada, lin);
    buffer+=QString::fromStdString(lin) + "\n";
   }
   entrada.close();

   xml_doc_name=nome_arq;
   loadXMLBuffer(buffer);
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void XMLParser::loadXMLBuffer(const QString &buf_xml)
{
 try
 {
  int pos1=-1, pos2=-1, tam=0;

  if(buf_xml.isEmpty())
   throw Exception(ERR_PARSERS_ATRIBBUFXMLVAZIO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  pos1=buf_xml.find("<?xml");
  pos2=buf_xml.find("?>");
  xml_buffer=buf_xml;

  if(pos1 >= 0 && pos2 >= 0)
  {
   tam=(pos2-pos1)+3;
   xml_decl=xml_buffer.mid(pos1, tam);
   xml_buffer.replace(pos1,tam,"");
  }
  else
   xml_decl="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

  removeDTD();
  readBuffer();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void XMLParser::setDTDFile(const QString &arq_dtd, const QString &nome_dtd)
{
 if(arq_dtd.isEmpty())
  throw Exception(ERR_PARSERS_ATRIBARQDTDVAZIO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(nome_dtd.isEmpty())
  throw Exception(ERR_PARSERS_ATRIBNOMEDTDVAZIO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 dtd_decl="<!DOCTYPE " + nome_dtd + " SYSTEM " + "\"" +  arq_dtd + "\">\n";
}

void XMLParser::readBuffer(void)
{
 QString buffer, msg, arquivo;
 xmlError *erro_xml=NULL;
 int parser_opt;

 if(!xml_buffer.isEmpty())
 {
  //Insere o cabeçalho do arquivo XML
  buffer+=xml_decl;

  //Configura o parser, inicialmente, para não validar o documento
  parser_opt=( XML_PARSE_NOBLANKS | XML_PARSE_NONET | XML_PARSE_NOENT );
  //XML_PARSE_NOERROR | XML_PARSE_NOWARNING

  //Caso a declaração DTD não esteja vazia
  if(!dtd_decl.isEmpty())
  {
   //Insere a declaração DTD do software ao buffer do documento XML
   buffer+=dtd_decl;

   //Configurando o parser para validar o documento com a DTD
   parser_opt=(parser_opt | XML_PARSE_DTDLOAD | XML_PARSE_DTDVALID);
  }

  buffer+=xml_buffer;

  //Lê o documento XML armazenado no buffer
  xml_doc=xmlReadMemory(buffer.toStdString().c_str(), buffer.size(),
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
   if(xml_doc) restartParser();

   throw Exception(QString(Exception::getErrorMessage(ERR_PARSERS_LIBXMLERR))
                 .arg(erro_xml->line).arg(erro_xml->int2).arg(msg).arg(arquivo),
                 ERR_PARSERS_LIBXMLERR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }

  //Obtém a raiz da árvore de elementos gerada
  root_elem=curr_elem=xmlDocGetRootElement(xml_doc);

  //Limpa as variáveis alocadas pelo parser
  if(xml_doc) xmlCleanupParser();
 }
}

void XMLParser::savePosition(void)
{
 if(!root_elem)
  throw Exception(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 elems_stack.push(curr_elem);
}

void XMLParser::restorePosition(void)
{
 if(!root_elem)
  throw Exception(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(elems_stack.empty())
  curr_elem=root_elem;
 else
 {
  curr_elem=elems_stack.top();
  elems_stack.pop();
 }
}

void XMLParser::restorePosition(const xmlNode *elem)
{
 if(!elem)
  throw Exception(ERR_PARSERS_OPRELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(elem->doc!=xml_doc)
  throw Exception(ERR_PARSERS_OPRELEMINVARVDOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 restartNavigation();
 curr_elem=const_cast<xmlNode *>(elem);
}

void XMLParser::restartNavigation(void)
{
 if(!root_elem)
  throw Exception(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 curr_elem=root_elem;

 while(!elems_stack.empty())
  elems_stack.pop();
}

void XMLParser::restartParser(void)
{
 root_elem=curr_elem=NULL;
 if(xml_doc)
 {
  xmlFreeDoc(xml_doc);
  xml_doc=NULL;
 }
 dtd_decl=xml_buffer=xml_decl="";

 while(!elems_stack.empty())
  elems_stack.pop();

 xmlResetLastError();
 xml_doc_name="";
}

bool XMLParser::accessElement(unsigned tipo_elem)
{
 bool possui_elem;
 xmlNode *vet_elems[4];

 if(!root_elem)
  throw Exception(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 vet_elems[ROOT_ELEMENT]=curr_elem->parent;
 vet_elems[CHILD_ELEMENT]=curr_elem->children;
 vet_elems[NEXT_ELEMENT]=curr_elem->next;
 vet_elems[PREVIOUS_ELEMENT]=curr_elem->prev;

 /* Verifica se o elemento atual possui o elemento que
    se deseja acessar. O flag possui_elem também é usado
    no retorno do método para indicar se o elemento foi
    movido ou não. */
 possui_elem=hasElement(tipo_elem);

 //Caso possua o elemento
 if(possui_elem)
  //Move o elemento atual para o elemento escolhido
  curr_elem=vet_elems[tipo_elem];

 return(possui_elem);
}

bool XMLParser::hasElement(unsigned tipo_elem)
{
 if(!root_elem)
  throw Exception(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 if(tipo_elem==ROOT_ELEMENT)
  /* Retorna a verificação se o elemento atual possui um pai.
     O elemento deve ser diferente da raiz, pois o elemento raiz
     nao está ligado a um pai */
  return(curr_elem!=root_elem && curr_elem->parent!=NULL);
 else if(tipo_elem==CHILD_ELEMENT)
  //Retorna a verificação se o elemento atual possui filhos
  return(curr_elem->children!=NULL);
 else if(tipo_elem==NEXT_ELEMENT)
  return(curr_elem->next!=NULL);
 else
  /* A segunda comparação na expressão é feita para o elemento raiz.
     pois a libxml2 coloca como elemento anterior da raiz ela mesma */
  return(curr_elem->prev!=NULL && curr_elem->prev!=root_elem);
}

bool XMLParser::hasAttributes(void)
{
 if(!root_elem)
  throw Exception(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(curr_elem->properties!=NULL);
}

QString XMLParser::getElementContent(void)
{
 if(!root_elem)
  throw Exception(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Caso esteja alocado, converte para char * e retorna em forma de std::QString
 return(QString(reinterpret_cast<char *>(curr_elem->content)));
}

QString XMLParser::getElementName(void)
{
 if(!root_elem)
  throw Exception(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(QString(reinterpret_cast<const char *>(curr_elem->name)));
}

xmlElementType XMLParser::getElementType(void)
{
 if(!root_elem)
  throw Exception(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(curr_elem->type);
}

const xmlNode *XMLParser::getCurrentElement(void)
{
 return(curr_elem);
}

void XMLParser::getElementAttributes(map<QString, QString> &atributos)
{
 xmlAttr *atribs_elem=NULL;
 QString atrib, valor;

 if(!root_elem)
  throw Exception(ERR_PARSERS_OPRARVELEMNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Limpa a lista de atributos passada
 atributos.clear();

 //Obtém a referência � s propriedades do elemento
 atribs_elem=curr_elem->properties;

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

QString XMLParser::getLoadedFilename(void)
{
 return(xml_doc_name);
}

QString XMLParser::getXMLBuffer(void)
{
 return(xml_buffer);
}

int XMLParser::getCurrentBufferLine(void)
{
 if(curr_elem)
  return(curr_elem->line);
 else
  return(0);
}

int XMLParser::getBufferLineCount(void)
{
 if(xml_doc)
  return(xml_doc->last->line);
 else
  return(0);
}

