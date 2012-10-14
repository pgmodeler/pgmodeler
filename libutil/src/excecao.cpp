#include "excecao.h"
//***********************************************************
QString Excecao::mensagens[QTD_ERROS][2]={
{"ERR_NULO", " "},
{"ERR_GLOBAL_OBJBADALOC", QObject::tr("Insufficient memory space to allocate the object!")},
{"ERR_PGMODELER_ATRPSDTIPOCOL", QObject::tr("Assignment of a pseudo-type to the type of the column!")},
{"ERR_PGMODELER_ATRCOMPRIMZERO", QObject::tr("Zero length assignment!")},
{"ERR_PGMODELER_ATRPRECISAOINV", QObject::tr("Assignment of a precision greater than the length of the type!")},
{"ERR_PGMODELER_ATRPRECTIPOTIMEINV", QObject::tr("Assignment of an invalid precision to type time, timestamp or interval. The precision in this case must be equal to or less than 6!")},
{"ERR_PGMODELER_ATRCOLNAOALOC", QObject::tr("Assignment of a not allocated column to object '%1' (%2)!")},
{"ERR_PGMODELER_REFCOLIDXINV", QObject::tr("Reference to a column which index is out of the capacity of the column list!")},
{"ERR_PGMODELER_ATROBJNAOALOC", QObject::tr("Assignment of not allocated object!")},
{"ERR_PGMODELER_ATRESQNAOALOC", QObject::tr("Assignment of a not allocated schema to object '%1' (%2)!")},
{"ERR_PGMODELER_ATROBJDEFSQLINV", QObject::tr("The object '%1' (%2) has inconsistent configuration (invalid SQL/XML definition)!")},
{"ERR_PGMODELER_ATROBJDUPLIC", QObject::tr("The object '%1' (%2) already exists in the list of elements of the same type in the object '%3' (%4)!")},
{"ERR_PGMODELER_ATROBJDUPLICCONT", QObject::tr("The object '%1' (%2) can't be assigned because there is already exists in the container object '%3'!")},
{"ERR_PGMODELER_ATROBJTIPOINV", QObject::tr("Assignment of object of an invalid type!")},
{"ERR_PGMODELER_REMOBJTIPOINV", QObject::tr("Removing an object of an invalid type!")},
{"ERR_PGMODELER_OBTOBJTIPOINV", QObject::tr("Obtaining an object of an invalid type!")},
{"ERR_PGMODELER_ATRNOMEVAZIOTPRET", QObject::tr("Assignment of empty name to table return type!")},
{"ERR_PGMODELER_INSPARAMDUPLIC", QObject::tr("The insertion of the parameter '%1' will not be possible because there is another parameter with same name in the function '%2'!")},
{"ERR_PGMODELER_INSTPRETDUPLIC", QObject::tr("The insertion of the table return type '%1' will not be possible because there is another return type with the same name in the '%2'!")},
{"ERR_PGMODELER_REFPARAMIDXINV", QObject::tr("Reference to a parameter which index is out of the parameter list capacity!")},
{"ERR_PGMODELER_REFEVENTOGATINV", QObject::tr("Reference to an event which does not belongs to trigger!")},
{"ERR_PGMODELER_ATRCOLINVGATILHO",QObject::tr("The column '%1' can't be assigned to the trigger '%2' because does not belongs to the trigger parent table!")},
{"ERR_PGMODELER_ATRFUNCNAOALOC", QObject::tr("Assignment of a not allocated function to object '%1' (%2)!")},
{"ERR_PGMODELER_ATRFUNCGATINV", QObject::tr("Assignment of a function which return type is different from 'trigger'!")},
{"ERR_PGMODELER_ATRFUNCNUMPARAMINV", QObject::tr("Assignment of a function which parameter count is invalid to the object '%1' (%2)!")},
{"ERR_PGMODELER_ATRFUNCLINGINV", QObject::tr("Assignment of a function which language is invalid!")},
{"ERR_PGMODELER_ATRTABNAOALOC", QObject::tr("Assignment of not allocated table to object '%1' (%2)!")},
{"ERR_PGMODELER_REFARGIDXINV", QObject::tr("Reference to an argument which index is out of argument list capacity!")},
{"ERR_PGMODELER_INSCOLDUPLIC", QObject::tr("Insertion of a column which name is already registered to another column of the object!")},
{"ERR_PGMODELER_ATRNOMEOBJVAZIO", QObject::tr("Assignment of empty name to an object!")},
{"ERR_PGMODELER_ATRNOMEOBJINV", QObject::tr("Assignment of invalid name to an object!")},
{"ERR_PGMODELER_ATRESQTIPOINV", QObject::tr("Assignment of schema object which type is invalid!")},
{"ERR_PGMODELER_ATRESPTABTIPOINV", QObject::tr("Assignment of tablespace object with invalid type!")},
{"ERR_PGMODELER_ATRESPTABOBJINV", QObject::tr("Assignment of tablespace to an invalid object!")},
{"ERR_PGMODELER_ATRESPTABRESTTIPOINV", QObject::tr("Assignment of tablespace to a constraint which type is invalid! To belong to a tablespace the constraint must be a primary key or unique!")},
{"ERR_PGMODELER_ATRPAPELTIPOINV", QObject::tr("Assignment of owner object which type is invalid!")},
{"ERR_PGMODELER_ATRDONOOBJINV", QObject::tr("Assignment of owner to an invalid object!")},
{"ERR_PGMODELER_REFFUNCTIPOINV", QObject::tr("Reference to a function with invalid type!")},
{"ERR_PGMODELER_REFARGOPTIPOINV", QObject::tr("Reference to an argument of the operator with invalid type!")},
{"ERR_PGMODELER_REFOPTIPOINV", QObject::tr("Reference to an operator with invalid type!")},
{"ERR_PGMODELER_ATROPPAPELTIPOINV", QObject::tr("Assigment of value to an invalid option type on role!")},
{"ERR_PGMODELER_REFPAPELINV", QObject::tr("Reference to an invalid role type!")},
{"ERR_PGMODELER_INSITEMPAPELDUPLIC", QObject::tr("The insertion of the role '%1' is not possible because this is already being referenced by role '%2'!")},
{"ERR_PGMODELER_REFREDUNDANTEPAPEIS",QObject::tr("Reference redundance detected by having the role '%1' referencing the role '%2'!")},
{"ERR_PGMODELER_ATRMEBROPROPPAPEL", QObject::tr("The role '%1' can not be listed as a member of itself!")},
{"ERR_PGMODELER_REFPAPELIDXINV", QObject::tr("Reference to a paper which index is out of paper list capacity!")},
{"ERR_PGMODELER_INSCMDNULO", QObject::tr("Insertion of null command to the rule!")},
{"ERR_PGMODELER_REFCMDIDXINV", QObject::tr("Reference to a command which index is out of the command list capacity!")},
{"ERR_PGMODELER_HERANCATABINV", QObject::tr("Is not possible to create a self generalization/dependecy relationship! The table can not inherit or copy their own attributes!")},
{"ERR_PGMODELER_ATROBJRELAC", QObject::tr("Assignment of an object that already belongs to another table!")},
{"ERR_PGMODELER_ATRESQDIFSEQ", QObject::tr("Assignment of a schema to the sequence which differs from the schema of the owner table!")},
{"ERR_PGMODELER_ATRESQVALORINV", QObject::tr("Assignment of an invalid value to one of the sequence attributes!")},
{"ERR_PGMODELER_ATRESQVALORMININV", QObject::tr("Assignment of a minimum value to the sequence which is greater than the maximum value!")},
{"ERR_PGMODELER_ATRESQVALORINIINV", QObject::tr("Assignment of a start value to the sequence which is extrapolating the range defined by minimum and maximum values​​!")},
{"ERR_PGMODELER_ATRESQINCNULO", QObject::tr("Assignment of a null increment value to the sequence!")},
{"ERR_PGMODELER_ATRESQCACHENULO", QObject::tr("Assignment of null cache value to the sequence!")},
{"ERR_PGMODELER_ATRESQDIFTAB", QObject::tr("Assignment of owner table which is not in the same schema as the sequence '%1'!")},
{"ERR_PGMODELER_ATRDONODIFTAB", QObject::tr("Assignment of owner table which does not belong to the same owner of the sequence '%1'!")},
{"ERR_PGMODELER_ATRCOLPOSINDEF", QObject::tr("Assignment of a nonexistent owner column to the sequence '%1'!")},
{"ERR_PGMODELER_ATRCOLPOSNAORELAC", QObject::tr("Assignment of an owner column to the sequence '%1' that is not related to any table!")},
{"ERR_PGMODELER_REFROTIDXINV", QObject::tr("Reference to a label which index is out of labels list capacity!")},
{"ERR_PGMODELER_ALOCOBJTIPOINV", QObject::tr("Allocation of object with invalid type!")},
{"ERR_PGMODELER_ATRFUNCRETINV", QObject::tr("Assignment of a function with invalid return type to object '%1' (%2)!")},
{"ERR_PGMODELER_ATRFUNCPARAMINV", QObject::tr("Assignment of a function with invalid parameter(s) type(s) to object '%1' (%2)!")},
{"ERR_PGMODELER_ATRLINGNAOALOC", QObject::tr("Assignment of not allocated language!")},
{"ERR_PGMODELER_ATRLINGTIPOINV", QObject::tr("Assignment of language object which type is invalid!")},
{"ERR_PGMODELER_REFTIPOIDXINV", QObject::tr("Reference to data type with an index outside the capacity of data types list!")},
{"ERR_PGMODELER_ATRTIPONULO", QObject::tr("Assignment of a null type to to object '%1' (%2)!")},
{"ERR_PGMODELER_ATRTIPOINVOBJ", QObject::tr("Assignment of invalid type to the object!")},
{"ERR_PGMODELER_ATRNOMEDIRVAZIO", QObject::tr("Assignment of an empty directory to object '%1' (%2)!")},
{"ERR_PGMODELER_OBTQTDTIPOINV", QObject::tr("Obtaining types with invalid quantity!")},
{"ERR_PGMODELER_INSATRIBTIPODUPLIC", QObject::tr("Insertion of item which already exists in the attributes list of the type!")},
{"ERR_PGMODELER_INSATRIBTIPOINV", QObject::tr("Insertion of invalid item in the attributes list of the type!")},
{"ERR_PGMODELER_INSENUMTIPODUPLIC", QObject::tr("Insertion of item which already exists in the enumarations list of the type!")},
{"ERR_PGMODELER_INSENUMTIPOINV", QObject::tr("Insertion of invalid item in the enumerations list of the type!")},
{"ERR_PGMODELER_REFATRIBTIPOIDXINV", QObject::tr("Reference to an attribute which index is out of the attributes list capacity!")},
{"ERR_PGMODELER_REFENUMTIPOIDXINV", QObject::tr("Reference to an enumeration which index is out of the enumerations list capacity!")},
{"ERR_PGMODELER_ATRCONFTIPOINV", QObject::tr("Assignment of invalid configuration to the type!")},
{"ERR_PGMODELER_INSTIPODUPLIC", QObject::tr("The data type '%1' can not be assigned because it already exists in the types list of the aggregate function '%2'!")},
{"ERR_PGMODELER_ATROPFUNCAGRARGINV", QObject::tr("Assignment of an operator which input  type count is invalid to aggregate function!")},
{"ERR_PGMODELER_ATROPFUNCAGTIPOSINV", QObject::tr("Assigment of an operator which types of arguments is invalid!")},
{"ERR_PGMODELER_ATRNOMERESERV", QObject::tr("Assignment of system reserved name to the object '%1' (%2)!")},
{"ERR_PGMODELER_FUNCCONFIGINV", QObject::tr("One function with invalid configuration is been used by the object '%1' (%2)!")},
{"ERR_PGMODELER_ATRIDUSUARIOINV", QObject::tr("Assignment of invalid id to the user!")},
{"ERR_PGMODELER_ATRVALORINVNUMEST", QObject::tr("Assignment of an invalid strategy/support number to an operator class element!")},
{"ERR_PGMODELER_INSELEMDUPLIC", QObject::tr("Insertion of element which already exists in the element list!")},
{"ERR_PGMODELER_REFELEMIDXINV", QObject::tr("Reference to an element which index is out of element list capacity!")},
{"ERR_PGMODELER_REFOBJIDXINV", QObject::tr("Reference to an object which index is out of object list capacity!")},
{"ERR_PGMODELER_REMOBJNAOALOC", QObject::tr("Removal of an object not allocated!")},
{"ERR_PGMODELER_REMOBJREFERDIR", QObject::tr("The object '%1' (%2) can not be removed because it is being referenced by object '%3' (%4)!")},
{"ERR_PGMODELER_REMOBJREFERIND", QObject::tr("The object '%1' (%2) can not be removed because it is being referenced by object '%3' (%4) that belongs to '%5' (%6)!")},
{"ERR_PGMODELER_OPROBJTIPOINV", QObject::tr("Operation with object(s) which type(s) is invalid!")},
{"ERR_PGMODELER_REFOBJTIPOINV", QObject::tr("Reference to object with invalid type!")},
{"ERR_PGMODELER_OPROBJNAOALOC", QObject::tr("Operation with object not allocated!")},
{"ERR_PGMODELER_RELTABSEMPK", QObject::tr("The creation of the relationship '%1' between the table '%2' and '%3' can not be done because one does not have a primary key. If the relationship is of the type n-n both tables must have primary keys!")},
{"ERR_PGMODELER_TIPORELNAOIMPL", QObject::tr("The relationship of the type 1-1 where both tables are mandatory participation is not implemented because it requires fusion between the tables that breaks the modeling done by the user!")},
{"ERR_PGMODELER_ATREXPRINV", QObject::tr("Assignment of an invalid expression to the object!")},
{"ERR_PGMODELER_ATRPKEXISTETAB", QObject::tr("Assignment of a primary key to a table which already has one!")},
{"ERR_PGMODELER_RELIDENTINV", QObject::tr("Identifier relationship can not be created for a self relationship, relationships of the type n-n, dependency or generalization!")},
{"ERR_PGMODELER_RELINVCOLDUPLIC", QObject::tr("Unable to create a dependency relationship because the column '%1' in table '%2' already exists in table '%3'!")},
{"ERR_PGMODELER_RELINVCOLSINCOMP", QObject::tr("Unable to create the generalization relationship because the column '%1' in table '%2' can not be merged with the column '%3' of table '%4' because they have incompatible types!")},
{"ERR_PGMODELER_ATROBJRELINV", QObject::tr("An attribute can not be added to a dependence or generalization relationship!")},
{"ERR_PGMODELER_ATRFKRELAC", QObject::tr("A foreign key can not be added to a relationship because is created automatically when this is connected!")},
{"ERR_PGMODELER_REFOBJINEXISTE", QObject::tr("The object '%1' (%2) is referencing the object '%3' (%4) which was not found in the list of objects of the same type in the model!")},
{"ERR_PGMODELER_REFTIPOUSRINV", QObject::tr("Reference to an user-defined data type that not exists in the model!")},
{"ERR_PGMODELER_ATRTAMMAXINVLISTA", QObject::tr("Assignment of invalid maximum size to operation list!")},
{"ERR_PGMODELER_ARQNAOGRAVADO", QObject::tr("Unable to write the file %1! Make sure the directory exists, or if the user has access permissions on it!")},
{"ERR_PGMODELER_ARQNAOGRAVADODEFINV", QObject::tr("Unable to write the model in the file %1 due to one or more errors in the definition generation process!")},
{"ERR_PGMODELER_RELEXISTEMODELO", QObject::tr("There is already a relationship between '%1' (%2) and '%3' (%4) in the model!")},
{"ERR_PGMODELER_INSRELGERAREDUNDANCIA", QObject::tr("The configuration of the relationship '%1' generates a redundancy between the relationships '%2'. Redundancy on identifier or generalization/dependency relationships are not accepted since they result in  incorrect column spreading making the model inconsistent!")},
{"ERR_PGMODELER_REFCOLUNAINVTABELA", QObject::tr("One or more objects were invalidated and automatically removed because they were referencing table columns which were included through relationships and which no longer exists due to disconnection of relationships or exclusion of such generated columns!")},
{"ERR_PGMODELER_ALOCPKFORMAINVALIDA", QObject::tr("The primary key '%1' can only be allocated if declared within a block of code that defines a table or relationship!")},
{"ERR_PGMODELER_REFPRIVLEGIOIDXINV",QObject::tr("Reference to an invalid privilege type!")},
{"ERR_PGMODELER_INSPAPELDUPLICPERM", QObject::tr("Insertion of an role which already exists in the role list of the permission!")},
{"ERR_PGMODELER_ATRPRIVILEGIOOBJINV", QObject::tr("Assignment of privilege incompatible with the type of object referenced by permission!")},
{"ERR_PGMODELER_ATRPERMISSAODUPLIC", QObject::tr("There is already a permission on object '%1' (%2) which has one or more equal roles from those present on permission to be assigned to the object!")},
{"ERR_PGMODELER_PERMREFOBJINEXISTE", QObject::tr("A permission is referencing the object '%1' (%2) which was not found in the model!")},
{"ERR_PGMODELER_OBJSEMESQUEMA", QObject::tr("The object '%1' (%2) can not be created by not being assigned to any schema!")},
{"ERR_PGMODELER_ATRESPTABDIRDUPLIC", QObject::tr("The tablespace '%1' can not be inserted into the model because it points to the same directory as the tablespace '%2'!")},
{"ERR_PGMODELER_ATRDIMENSAOINVDOMINIO", QObject::tr("It is not possible to create arrays of domains or sequences (dimension >= 1)! PostgreSQL does not yet implement this feature!")},
{"ERR_PGMODELER_ATRCORPOFUNCLINGC", QObject::tr("The function '%1' can not get a source code as a definition because its language is set to C. Use the attributes symbol and dynamic library instead!")},
{"ERR_PGMODELER_ATRBIBLIOFUNCLINGDIFC", QObject::tr("The function '%1' can have the attributes symbol and dynamic library configured only if the language is set to C. For all other cases you must specify a source code that defines it in the DBMS!")},
{"ERR_PGMODELER_ATROPERCOMUTINV", QObject::tr("The operator '%1' can not be assigned as a comutator of operator '%2' because it has incompatible settings!")},
{"ERR_PGMODELER_ATROPERNEGINV", QObject::tr("The operator '%1' can not be assigned as negator of operator '%2' because it has incompatible settings!")},
{"ERR_PGMODELER_TIPOUSRAUTOREF", QObject::tr("The type '%1' can not self refer in the attributes 'element' or 'copy type' or be used as a data type of an attribute in the configuration of a composite type!")},
{"ERR_PGMODELER_ATRELEMENTOINVTIPO", QObject::tr("Assignment of invalid element to type '%1'!")},
{"ERR_PGMODELER_ATRALINHAMENTOINVTIPO",QObject::tr("Assignment of invalid alignment to type '%1'!")},
{"ERR_PGMODELER_ATRNOMEINVTABRELNN",  QObject::tr("Assignment of invalid name to the table generated from N-N relationship!")},
{"ERR_PGMODELER_PKESPTIPORELINV", QObject::tr("The relationship '%1' can not make use of the special primary key. Only generalization/dependency relationships have access to this resource!")},
{"ERR_PGMODELER_ATRSUFIXORELINV",  QObject::tr("Assignment of invalid suffix to the relationship '%1'!")},
{"ERR_PGMODELERUI_OPROBJINCRELACAO", QObject::tr("The object '%1' (%2) can not be edited or deleted because it was automatically included through a relationship! To manipulate the object is necessary to make it from the edit form of the relationship that holds it.")},
{"ERR_PGMODELERUI_REMOBJPROTEGIDO", QObject::tr("The object '%1' (%2) can not be deleted because it is protected!")},
{"ERR_PGMODELERUI_REDECLGRUPODESTAQUE", QObject::tr("The group '%1' has already been declared earlier!")},
{"ERR_PGMODELERUI_DEFGRUPOLOCALINV", QObject::tr("The group '%1' can not be built in the groups declaration block ('%2')!")},
{"ERR_PGMODELERUI_DEFGRUPONAODECL", QObject::tr("The group '%1' was built but not declared in the groups declaration block ('%2')!")},
{"ERR_PGMODELERUI_DEFGRUPOSEMELEM", QObject::tr("The group '%1' can not be built without possessing child elements!")},
{"ERR_PGMODELERUI_DEFGRUPODUPLICADA", QObject::tr("The group '%1' can not be built once more because this was done in previous blocks!")},
{"ERR_PGMODELERUI_GRUPODECLNAODEF", QObject::tr("The group '%1' has been declared but not built!")},
{"ERR_PGMODELERUI_REFCOLTABOBJIDXINV", QObject::tr("Reference to a column of objects table with invalid index!")},
{"ERR_PGMODELERUI_REFLINTABOBJIDXINV", QObject::tr("Reference to a row of objects table with invalid index!")},
{"ERR_PGMODELERUI_OPROBJRESERVADO", QObject::tr("The scheme 'public' and the languages 'plpgsql', 'c' and 'sql' can not be manipulated because they are reserved to PostgreSQL! They are present in the model database only as a reference!")},
{"ERR_PGMODELERUI_CONFFUNCINVALIDAOBJ", QObject::tr("The new configuration function invalidates the object '%1' (%2)! In this case it is needed to undo the relationship between the affected object and function so that the new configuration of the latter to take effect!")},
{"ERR_PGMODELERUI_TIPOSQLINDEFREFVISAO", QObject::tr("A vision reference must have at least one SQL application : SELECT, FROM, FROM-WHERE or After WHERE!")},
{"ERR_PGMODELERUI_RESTPKFKSEMCOLUNAS", QObject::tr("Constraints like primary key, foreign key or unique must have at least one column related to them! For foreign keys must be selected, in addition, the referenced columns!")},
{"ERR_PGMODELERUI_CONFNAOCARREGADA", QObject::tr("Unable to load one or more configuration files! Please check if files exists in the configuration folder and if they are not corrupted to preventing this error to occur again on the next startup!")},
{"ERR_PGMODELERUI_CONFPADRAONAORESTAURADA", QObject::tr("Could not find the default settings file '%1'! To restore default settings check the existence of the file and try again!")},
{"ERR_PGMODELERUI_FALHAEXPORTACAO", QObject::tr("The export process failed due to an error triggered by the PostgreSQL server in an attempt to execute a SQL command. For more details about the error check the exception stack!\n\n** Executed SQL command: **\n\n%1")},
{"ERR_PGMODELERUI_PLUGINNAOCARREGADO", QObject::tr("Could not load the plugin '%1' from the library '%2'! Message returned by plugin manager: '%3'")},
{"ERR_PGMODELERUI_PLUGINSNAOCARREGADOS", QObject::tr("One or more plugins were not activated due to errors during the loading process! Check the exception stack for more details.")},
{"ERR_PARSERS_SINTAXEINV", QObject::tr("Invalid syntax in file %1, line %2, column %3!")},
{"ERR_PARSERS_CONDICIONALINV", QObject::tr("Invalid conditional instruction '%1' on file %2, line %3, column %4!")},
{"ERR_PARSERS_ATRIBUTODESC", QObject::tr("Unknown attribute '%1' in file %2, line %3, column %4!")},
{"ERR_PARSERS_METACHARINV", QObject::tr("Invalid metacharacter '%1' in file %2, line %3, column %4!")},
{"ERR_PARSERS_ATRIBVALORNULO", QObject::tr("Attribute '%1' with an undefined value in file %2, line %3, column %4!")},
{"ERR_PARSERS_ATRIBBUFXMLVAZIO", QObject::tr("Assignment of empty XML buffer to parser!")},
{"ERR_PARSERS_ARQDIRNAOCARREGADO", QObject::tr("Could not access the file or directory %1! Make sure that it exists or if the user has access permissions on it!")},
{"ERR_PARSERS_ATRIBARQDTDVAZIO", QObject::tr("Assignment of empty DTD file name!")},
{"ERR_PARSERS_ATRIBNOMEDTDVAZIO", QObject::tr("Assignment of empty name to the DTD declaration!")},
{"ERR_PARSERS_LIBXMLERR", QObject::tr("Error while interpreting XML buffer at line %1 column %2.\nMessage generated by the parser: '%3'. %4")},
{"ERR_PARSERS_OPRARVELEMNAOALOC", QObject::tr("Operation on unallocated element tree! It is necessary to load the XML parser buffer and interpret it so that the tree is generated!")},
{"ERR_PARSERS_ARQMODELOINV", QObject::tr("Could not load file %1. The same appears to be inconsistent or one of its dependencies (DTD files) has errors or is missing!")},
{"ERR_PARSERS_OPRELEMNAOALOC", QObject::tr("Operation with unallocated tree element!")},
{"ERR_PARSERS_OPRELEMINVARVDOC", QObject::tr("Operation with element which does not exists in the element tree currently loaded!")},
{"ERR_CONEXBD_ATRPARAMINVCONEX", QObject::tr("Assignment of a value to an invalid connection parameter!")},
{"ERR_CONEXBD_OPRCONEXNAOALOC", QObject::tr("Operation on connection not established!")},
{"ERR_CONEXBD_CONEXSEMPARAMCONFIG", QObject::tr("Attempt to connect without define configuration parameters!")},
{"ERR_CONEXBD_CONEXNAOESTABELECIDA", QObject::tr("Could not connect to the database.\nMessage returned: '%1'")},
{"ERR_CONEXBD_ATRRESSQLNAOALOC", QObject::tr("Assignment of not allocated SQL command result!")},
{"ERR_CONEXBD_SGBDRESPINCOMP", QObject::tr("Unable to allocate the result of the SQL command because the response from the DBMS was not understood by the client!")},
{"ERR_CONEXBD_SGBDERROFATAL", QObject::tr("Unable to allocate command result for the SQL because the server has generated a fatal error!\nMessage returned by the DBMS: %1")},
{"ERR_CONEXBD_COMANDOSQLVAZIO", QObject::tr("Unable to allocate the result of the SQL command because the command passed was an empty SQL!")},
{"ERR_CONEXBD_REFCOLTUPLAIDXINV", QObject::tr("Reference to a column of tuple with invalid index!")},
{"ERR_CONEXBD_REFCOLTUPLANOMEINV", QObject::tr("Reference to a column of tuple with invalid name!")},
{"ERR_CONEXBD_REFTUPLANAOEXISTE", QObject::tr("Reference to a tuple with index invalid or the result is empty (no tuples)!")},
{"ERR_CONEXBD_CMDSQLNAOEXECUTADO", QObject::tr("Could not execute the SQL command.\n Message returned: %1")},
};
//-----------------------------------------------------------
Excecao::Excecao(void)
{
 configurarExcecao("",ERR_NULO,"","",-1,"");
}
//-----------------------------------------------------------
Excecao::Excecao(const QString &msg, const QString &local, const QString &arquivo, int linha, Excecao *excecao, const QString &info_adicional)
{
 configurarExcecao(msg,tipo_erro, local, arquivo, linha, info_adicional);
 if(excecao) adicionarExecao(*excecao);
}
//-----------------------------------------------------------
Excecao::Excecao(TipoErro tipo_erro, const QString &local, const QString &arquivo, int linha, Excecao *excecao, const QString &info_adicional)
{
 configurarExcecao(mensagens[tipo_erro][MENSAGEM_ERRO],tipo_erro, local, arquivo, linha, info_adicional);
 if(excecao) adicionarExecao(*excecao);
}
//-----------------------------------------------------------
Excecao::Excecao(const QString &msg, TipoErro tipo_erro, const QString &local, const QString &arquivo, int linha, Excecao *excecao, const QString &info_adicional)
{
 configurarExcecao(msg,tipo_erro, local, arquivo, linha, info_adicional);
 if(excecao) adicionarExecao(*excecao);
}
//-----------------------------------------------------------
Excecao::Excecao(TipoErro tipo_erro, const QString &local, const QString &arquivo, int linha, vector<Excecao> &excecoes, const QString &info_adicional)
{
 vector<Excecao>::iterator itr, itr_end;

 configurarExcecao(mensagens[tipo_erro][MENSAGEM_ERRO],tipo_erro, local, arquivo, linha, info_adicional);

 itr=excecoes.begin();
 itr_end=excecoes.end();
 while(itr!=itr_end)
 {
  adicionarExecao((*itr));
  itr++;
 }
}
//-----------------------------------------------------------
Excecao::Excecao(const QString &msg, const QString &local, const QString &arquivo, int linha, vector<Excecao> &excecoes, const QString &info_adicional)
{
 vector<Excecao>::iterator itr, itr_end;

 configurarExcecao(msg,tipo_erro, local, arquivo, linha, info_adicional);

 itr=excecoes.begin();
 itr_end=excecoes.end();
 while(itr!=itr_end)
 {
  adicionarExecao((*itr));
  itr++;
 }
}
//-----------------------------------------------------------
void Excecao::configurarExcecao(const QString &msg, TipoErro tipo_erro, const QString &local, const QString &arquivo, int linha, const QString &info_adicional)
{
 this->tipo_erro=tipo_erro;
 this->msg_erro=msg;
 this->local=local;
 this->arquivo=arquivo;
 this->linha=linha;
 this->info_adicional=info_adicional;
}
//-----------------------------------------------------------
QString Excecao::obterMensagemErro(void)
{
 return(msg_erro);
}
//-----------------------------------------------------------
QString Excecao::obterMensagemErro(TipoErro tipo_erro)
{
 if(tipo_erro < QTD_ERROS)
   return(QObject::tr(mensagens[tipo_erro][MENSAGEM_ERRO]));
 else
  return("");
}
//-----------------------------------------------------------
QString Excecao::obterNomeErro(TipoErro tipo_erro)
{
 if(tipo_erro < QTD_ERROS)
   return(mensagens[tipo_erro][CODIGO_ERRO]);
 else
  return("");
}
//-----------------------------------------------------------
QString Excecao::obterLocal(void)
{
 return(local);
}
//-----------------------------------------------------------
QString Excecao::obterArquivo(void)
{
 return(arquivo);
}
//-----------------------------------------------------------
QString Excecao::obterLinha(void)
{
 return(QString("%1").arg(linha));
}
//-----------------------------------------------------------
TipoErro Excecao::obterTipoErro(void)
{
 return(tipo_erro);
}
//-----------------------------------------------------------
QString Excecao::obterInfoAdicional(void)
{
 return(info_adicional);
}
//-----------------------------------------------------------
void Excecao::adicionarExecao(Excecao &excecao)
{
 deque<Excecao>::iterator itr, itr_end;

 itr=excecao.excecoes.begin();
 itr_end=excecao.excecoes.end();

 while(itr!=itr_end)
 {
  this->excecoes.push_front(Excecao(itr->msg_erro,itr->tipo_erro,
                                    itr->local,itr->arquivo,itr->linha,NULL,itr->info_adicional));
  itr++;
 }
 excecao.excecoes.clear();
 this->excecoes.push_front(Excecao(excecao.msg_erro,excecao.tipo_erro,
                                   excecao.local,excecao.arquivo,excecao.linha,NULL,excecao.info_adicional));
}
//-----------------------------------------------------------
void Excecao::obterListaExcecoes(deque<Excecao> &lista)
{
 lista.assign(this->excecoes.begin(), this->excecoes.end());
 lista.push_front(Excecao(this->msg_erro,this->tipo_erro,
                          this->local,this->arquivo,this->linha,NULL,this->info_adicional));
}
//***********************************************************
