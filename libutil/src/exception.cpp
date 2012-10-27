#include "exception.h"
#include <QApplication>

QString Exception::messages[ERROR_COUNT][2]={
{"ERR_NULO", " "},
{"ERR_GLOBAL_OBJBADALOC", QT_TR_NOOP("Insufficient memory space to allocate the object!")},
{"ERR_PGMODELER_ATRPSDTIPOCOL", QT_TR_NOOP("Assignment of a pseudo-type to the type of the column!")},
{"ERR_PGMODELER_ATRCOMPRIMZERO", QT_TR_NOOP("Zero length assignment!")},
{"ERR_PGMODELER_ATRPRECISAOINV", QT_TR_NOOP("Assignment of a precision greater than the length of the type!")},
{"ERR_PGMODELER_ATRPRECTIPOTIMEINV", QT_TR_NOOP("Assignment of an invalid precision to type time, timestamp or interval. The precision in this case must be equal to or less than 6!")},
{"ERR_PGMODELER_ATRCOLNAOALOC", QT_TR_NOOP("Assignment of a not allocated column to object '%1' (%2)!")},
{"ERR_PGMODELER_REFCOLIDXINV", QT_TR_NOOP("Reference to a column which index is out of the capacity of the column list!")},
{"ERR_PGMODELER_ATROBJNAOALOC", QT_TR_NOOP("Assignment of not allocated object!")},
{"ERR_PGMODELER_ATRESQNAOALOC", QT_TR_NOOP("Assignment of a not allocated schema to object '%1' (%2)!")},
{"ERR_PGMODELER_ATROBJDEFSQLINV", QT_TR_NOOP("The object '%1' (%2) has inconsistent configuration (invalid SQL/XML definition)!")},
{"ERR_PGMODELER_ATROBJDUPLIC", QT_TR_NOOP("The object '%1' (%2) already exists in the list of elements of the same type in the object '%3' (%4)!")},
{"ERR_PGMODELER_ATROBJDUPLICCONT", QT_TR_NOOP("The object '%1' (%2) can't be assigned because there is already exists in the container object '%3'!")},
{"ERR_PGMODELER_ATROBJTIPOINV", QT_TR_NOOP("Assignment of object of an invalid type!")},
{"ERR_PGMODELER_REMOBJTIPOINV", QT_TR_NOOP("Removing an object of an invalid type!")},
{"ERR_PGMODELER_OBTOBJTIPOINV", QT_TR_NOOP("Obtaining an object of an invalid type!")},
{"ERR_PGMODELER_ATRNOMEVAZIOTPRET", QT_TR_NOOP("Assignment of empty name to table return type!")},
{"ERR_PGMODELER_INSPARAMDUPLIC", QT_TR_NOOP("The insertion of the parameter '%1' will not be possible because there is another parameter with same name in the function '%2'!")},
{"ERR_PGMODELER_INSTPRETDUPLIC", QT_TR_NOOP("The insertion of the table return type '%1' will not be possible because there is another return type with the same name in the '%2'!")},
{"ERR_PGMODELER_REFPARAMIDXINV", QT_TR_NOOP("Reference to a parameter which index is out of the parameter list capacity!")},
{"ERR_PGMODELER_REFEVENTOGATINV", QT_TR_NOOP("Reference to an event which does not belongs to trigger!")},
{"ERR_PGMODELER_ATRCOLINVGATILHO",QT_TR_NOOP("The column '%1' can't be assigned to the trigger '%2' because does not belongs to the trigger parent table!")},
{"ERR_PGMODELER_ATRFUNCNAOALOC", QT_TR_NOOP("Assignment of a not allocated function to object '%1' (%2)!")},
{"ERR_PGMODELER_ATRFUNCGATINV", QT_TR_NOOP("Assignment of a function which return type is different from 'trigger'!")},
{"ERR_PGMODELER_ATRFUNCNUMPARAMINV", QT_TR_NOOP("Assignment of a function which parameter count is invalid to the object '%1' (%2)!")},
{"ERR_PGMODELER_ATRFUNCLINGINV", QT_TR_NOOP("Assignment of a function which language is invalid!")},
{"ERR_PGMODELER_ATRTABNAOALOC", QT_TR_NOOP("Assignment of not allocated table to object '%1' (%2)!")},
{"ERR_PGMODELER_REFARGIDXINV", QT_TR_NOOP("Reference to an argument which index is out of argument list capacity!")},
{"ERR_PGMODELER_INSCOLDUPLIC", QT_TR_NOOP("Insertion of a column which name is already registered to another column of the object!")},
{"ERR_PGMODELER_ATRNOMEOBJVAZIO", QT_TR_NOOP("Assignment of empty name to an object!")},
{"ERR_PGMODELER_ATRNOMEOBJINV", QT_TR_NOOP("Assignment of invalid name to an object!")},
{"ERR_PGMODELER_ATRESQTIPOINV", QT_TR_NOOP("Assignment of schema object which type is invalid!")},
{"ERR_PGMODELER_ATRESPTABTIPOINV", QT_TR_NOOP("Assignment of tablespace object with invalid type!")},
{"ERR_PGMODELER_ATRESPTABOBJINV", QT_TR_NOOP("Assignment of tablespace to an invalid object!")},
{"ERR_PGMODELER_ATRESPTABRESTTIPOINV", QT_TR_NOOP("Assignment of tablespace to a constraint which type is invalid! To belong to a tablespace the constraint must be a primary key or unique!")},
{"ERR_PGMODELER_ATRPAPELTIPOINV", QT_TR_NOOP("Assignment of owner object which type is invalid!")},
{"ERR_PGMODELER_ATRDONOOBJINV", QT_TR_NOOP("Assignment of owner to an invalid object!")},
{"ERR_PGMODELER_REFFUNCTIPOINV", QT_TR_NOOP("Reference to a function with invalid type!")},
{"ERR_PGMODELER_REFARGOPTIPOINV", QT_TR_NOOP("Reference to an argument of the operator with invalid type!")},
{"ERR_PGMODELER_REFOPTIPOINV", QT_TR_NOOP("Reference to an operator with invalid type!")},
{"ERR_PGMODELER_ATROPPAPELTIPOINV", QT_TR_NOOP("Assignment of value to an invalid option type on role!")},
{"ERR_PGMODELER_REFPAPELINV", QT_TR_NOOP("Reference to an invalid role type!")},
{"ERR_PGMODELER_INSITEMPAPELDUPLIC", QT_TR_NOOP("The insertion of the role '%1' is not possible because this is already being referenced by role '%2'!")},
{"ERR_PGMODELER_REFREDUNDANTEPAPEIS",QT_TR_NOOP("Reference redundancy detected by having the role '%1' referencing the role '%2'!")},
{"ERR_PGMODELER_ATRMEBROPROPPAPEL", QT_TR_NOOP("The role '%1' can not be listed as a member of itself!")},
{"ERR_PGMODELER_REFPAPELIDXINV", QT_TR_NOOP("Reference to a paper which index is out of paper list capacity!")},
{"ERR_PGMODELER_INSCMDNULO", QT_TR_NOOP("Insertion of null command to the rule!")},
{"ERR_PGMODELER_REFCMDIDXINV", QT_TR_NOOP("Reference to a command which index is out of the command list capacity!")},
{"ERR_PGMODELER_HERANCATABINV", QT_TR_NOOP("Is not possible to create a self generalization/dependency relationship! The table can not inherit or copy their own attributes!")},
{"ERR_PGMODELER_ATROBJRELAC", QT_TR_NOOP("Assignment of an object that already belongs to another table!")},
{"ERR_PGMODELER_ATRESQDIFSEQ", QT_TR_NOOP("Assignment of a schema to the sequence which differs from the schema of the owner table!")},
{"ERR_PGMODELER_ATRESQVALORINV", QT_TR_NOOP("Assignment of an invalid value to one of the sequence attributes!")},
{"ERR_PGMODELER_ATRESQVALORMININV", QT_TR_NOOP("Assignment of a minimum value to the sequence which is greater than the maximum value!")},
{"ERR_PGMODELER_ATRESQVALORINIINV", QT_TR_NOOP("Assignment of a start value to the sequence which is extrapolating the range defined by minimum and maximum values​​!")},
{"ERR_PGMODELER_ATRESQINCNULO", QT_TR_NOOP("Assignment of a null increment value to the sequence!")},
{"ERR_PGMODELER_ATRESQCACHENULO", QT_TR_NOOP("Assignment of null cache value to the sequence!")},
{"ERR_PGMODELER_ATRESQDIFTAB", QT_TR_NOOP("Assignment of owner table which is not in the same schema as the sequence '%1'!")},
{"ERR_PGMODELER_ATRDONODIFTAB", QT_TR_NOOP("Assignment of owner table which does not belong to the same owner of the sequence '%1'!")},
{"ERR_PGMODELER_ATRCOLPOSINDEF", QT_TR_NOOP("Assignment of a nonexistent owner column to the sequence '%1'!")},
{"ERR_PGMODELER_ATRCOLPOSNAORELAC", QT_TR_NOOP("Assignment of an owner column to the sequence '%1' that is not related to any table!")},
{"ERR_PGMODELER_REFROTIDXINV", QT_TR_NOOP("Reference to a label which index is out of labels list capacity!")},
{"ERR_PGMODELER_ALOCOBJTIPOINV", QT_TR_NOOP("Allocation of object with invalid type!")},
{"ERR_PGMODELER_ATRFUNCRETINV", QT_TR_NOOP("Assignment of a function with invalid return type to object '%1' (%2)!")},
{"ERR_PGMODELER_ATRFUNCPARAMINV", QT_TR_NOOP("Assignment of a function with invalid parameter(s) type(s) to object '%1' (%2)!")},
{"ERR_PGMODELER_ATRLINGNAOALOC", QT_TR_NOOP("Assignment of not allocated language!")},
{"ERR_PGMODELER_ATRLINGTIPOINV", QT_TR_NOOP("Assignment of language object which type is invalid!")},
{"ERR_PGMODELER_REFTIPOIDXINV", QT_TR_NOOP("Reference to data type with an index outside the capacity of data types list!")},
{"ERR_PGMODELER_ATRTIPONULO", QT_TR_NOOP("Assignment of a null type to to object '%1' (%2)!")},
{"ERR_PGMODELER_ATRTIPOINVOBJ", QT_TR_NOOP("Assignment of invalid type to the object!")},
{"ERR_PGMODELER_ATRNOMEDIRVAZIO", QT_TR_NOOP("Assignment of an empty directory to object '%1' (%2)!")},
{"ERR_PGMODELER_OBTQTDTIPOINV", QT_TR_NOOP("Obtaining types with invalid quantity!")},
{"ERR_PGMODELER_INSATRIBTIPODUPLIC", QT_TR_NOOP("Insertion of item which already exists in the attributes list of the type!")},
{"ERR_PGMODELER_INSATRIBTIPOINV", QT_TR_NOOP("Insertion of invalid item in the attributes list of the type!")},
{"ERR_PGMODELER_INSENUMTIPODUPLIC", QT_TR_NOOP("Insertion of item which already exists in the enumarations list of the type!")},
{"ERR_PGMODELER_INSENUMTIPOINV", QT_TR_NOOP("Insertion of invalid item in the enumerations list of the type!")},
{"ERR_PGMODELER_REFATRIBTIPOIDXINV", QT_TR_NOOP("Reference to an attribute which index is out of the attributes list capacity!")},
{"ERR_PGMODELER_REFENUMTIPOIDXINV", QT_TR_NOOP("Reference to an enumeration which index is out of the enumerations list capacity!")},
{"ERR_PGMODELER_ATRCONFTIPOINV", QT_TR_NOOP("Assignment of invalid configuration to the type!")},
{"ERR_PGMODELER_INSTIPODUPLIC", QT_TR_NOOP("The data type '%1' can not be assigned because it already exists in the types list of the aggregate function '%2'!")},
{"ERR_PGMODELER_ATROPFUNCAGRARGINV", QT_TR_NOOP("Assignment of an operator which input  type count is invalid to aggregate function!")},
{"ERR_PGMODELER_ATROPFUNCAGTIPOSINV", QT_TR_NOOP("Assignment of an operator which types of arguments is invalid!")},
{"ERR_PGMODELER_ATRNOMERESERV", QT_TR_NOOP("Assignment of system reserved name to the object '%1' (%2)!")},
{"ERR_PGMODELER_FUNCCONFIGINV", QT_TR_NOOP("One function with invalid configuration is been used by the object '%1' (%2)!")},
{"ERR_PGMODELER_ATRIDUSUARIOINV", QT_TR_NOOP("Assignment of invalid id to the user!")},
{"ERR_PGMODELER_ATRVALORINVNUMEST", QT_TR_NOOP("Assignment of an invalid strategy/support number to an operator class element!")},
{"ERR_PGMODELER_INSELEMDUPLIC", QT_TR_NOOP("Insertion of element which already exists in the element list!")},
{"ERR_PGMODELER_REFELEMIDXINV", QT_TR_NOOP("Reference to an element which index is out of element list capacity!")},
{"ERR_PGMODELER_REFOBJIDXINV", QT_TR_NOOP("Reference to an object which index is out of object list capacity!")},
{"ERR_PGMODELER_REMOBJNAOALOC", QT_TR_NOOP("Removal of an object not allocated!")},
{"ERR_PGMODELER_REMOBJREFERDIR", QT_TR_NOOP("The object '%1' (%2) can not be removed because it is being referenced by object '%3' (%4)!")},
{"ERR_PGMODELER_REMOBJREFERIND", QT_TR_NOOP("The object '%1' (%2) can not be removed because it is being referenced by object '%3' (%4) that belongs to '%5' (%6)!")},
{"ERR_PGMODELER_OPROBJTIPOINV", QT_TR_NOOP("Operation with object(s) which type(s) is invalid!")},
{"ERR_PGMODELER_REFOBJTIPOINV", QT_TR_NOOP("Reference to object with invalid type!")},
{"ERR_PGMODELER_OPROBJNAOALOC", QT_TR_NOOP("Operation with object not allocated!")},
{"ERR_PGMODELER_RELTABSEMPK", QT_TR_NOOP("The creation of the relationship '%1' between the table '%2' and '%3' can not be done because one does not have a primary key. If the relationship is of the type n-n both tables must have primary keys!")},
{"ERR_PGMODELER_TIPORELNAOIMPL", QT_TR_NOOP("The relationship of the type 1-1 where both tables are mandatory participation is not implemented because it requires fusion between the tables that breaks the modeling done by the user!")},
{"ERR_PGMODELER_ATREXPRINV", QT_TR_NOOP("Assignment of an invalid expression to the object!")},
{"ERR_PGMODELER_ATRPKEXISTETAB", QT_TR_NOOP("Assignment of a primary key to a table which already has one!")},
{"ERR_PGMODELER_RELIDENTINV", QT_TR_NOOP("Identifier relationship can not be created for a self relationship, relationships of the type n-n, dependency or generalization!")},
{"ERR_PGMODELER_RELINVCOLDUPLIC", QT_TR_NOOP("Unable to create a dependency relationship because the column '%1' in table '%2' already exists in table '%3'!")},
{"ERR_PGMODELER_RELINVCOLSINCOMP", QT_TR_NOOP("Unable to create the generalization relationship because the column '%1' in table '%2' can not be merged with the column '%3' of table '%4' because they have incompatible types!")},
{"ERR_PGMODELER_ATROBJRELINV", QT_TR_NOOP("An attribute can not be added to a dependence or generalization relationship!")},
{"ERR_PGMODELER_ATRFKRELAC", QT_TR_NOOP("A foreign key can not be added to a relationship because is created automatically when this is connected!")},
{"ERR_PGMODELER_REFOBJINEXISTE", QT_TR_NOOP("The object '%1' (%2) is referencing the object '%3' (%4) which was not found in the list of objects of the same type in the model!")},
{"ERR_PGMODELER_REFTIPOUSRINV", QT_TR_NOOP("Reference to an user-defined data type that not exists in the model!")},
{"ERR_PGMODELER_ATRTAMMAXINVLISTA", QT_TR_NOOP("Assignment of invalid maximum size to operation list!")},
{"ERR_PGMODELER_ARQNAOGRAVADO", QT_TR_NOOP("Unable to write the file %1! Make sure the directory exists, or if the user has access permissions on it!")},
{"ERR_PGMODELER_ARQNAOGRAVADODEFINV", QT_TR_NOOP("Unable to write the model in the file %1 due to one or more errors in the definition generation process!")},
{"ERR_PGMODELER_RELEXISTEMODELO", QT_TR_NOOP("There is already a relationship between '%1' (%2) and '%3' (%4) in the model!")},
{"ERR_PGMODELER_INSRELGERAREDUNDANCIA", QT_TR_NOOP("The configuration of the relationship '%1' generates a redundancy between the relationships '%2'. Redundancy on identifier or generalization/dependency relationships are not accepted since they result in  incorrect column spreading making the model inconsistent!")},
{"ERR_PGMODELER_REFCOLUNAINVTABELA", QT_TR_NOOP("One or more objects were invalidated and automatically removed because they were referencing table columns which were included through relationships and which no longer exists due to disconnection of relationships or exclusion of such generated columns!")},
{"ERR_PGMODELER_ALOCPKFORMAINVALIDA", QT_TR_NOOP("The primary key '%1' can only be allocated if declared within a block of code that defines a table or relationship!")},
{"ERR_PGMODELER_REFPRIVLEGIOIDXINV",QT_TR_NOOP("Reference to an invalid privilege type!")},
{"ERR_PGMODELER_INSPAPELDUPLICPERM", QT_TR_NOOP("Insertion of an role which already exists in the role list of the permission!")},
{"ERR_PGMODELER_ATRPRIVILEGIOOBJINV", QT_TR_NOOP("Assignment of privilege incompatible with the type of object referenced by permission!")},
{"ERR_PGMODELER_ATRPERMISSAODUPLIC", QT_TR_NOOP("There is already a permission on object '%1' (%2) which has one or more equal roles from those present on permission to be assigned to the object!")},
{"ERR_PGMODELER_PERMREFOBJINEXISTE", QT_TR_NOOP("A permission is referencing the object '%1' (%2) which was not found in the model!")},
{"ERR_PGMODELER_OBJSEMESQUEMA", QT_TR_NOOP("The object '%1' (%2) can not be created by not being assigned to any schema!")},
{"ERR_PGMODELER_ATRESPTABDIRDUPLIC", QT_TR_NOOP("The tablespace '%1' can not be inserted into the model because it points to the same directory as the tablespace '%2'!")},
{"ERR_PGMODELER_ATRDIMENSAOINVDOMINIO", QT_TR_NOOP("It is not possible to create arrays of domains or sequences (dimension >= 1)! PostgreSQL does not yet implement this feature!")},
{"ERR_PGMODELER_ATRCORPOFUNCLINGC", QT_TR_NOOP("The function '%1' can not get a source code as a definition because its language is set to C. Use the attributes symbol and dynamic library instead!")},
{"ERR_PGMODELER_ATRBIBLIOFUNCLINGDIFC", QT_TR_NOOP("The function '%1' can have the attributes symbol and dynamic library configured only if the language is set to C. For all other cases you must specify a source code that defines it in the DBMS!")},
{"ERR_PGMODELER_ATROPERCOMUTINV", QT_TR_NOOP("The operator '%1' can not be assigned as a comutator of operator '%2' because it has incompatible settings!")},
{"ERR_PGMODELER_ATROPERNEGINV", QT_TR_NOOP("The operator '%1' can not be assigned as negator of operator '%2' because it has incompatible settings!")},
{"ERR_PGMODELER_TIPOUSRAUTOREF", QT_TR_NOOP("The type '%1' can not self refer in the attributes 'element' or 'copy type' or be used as a data type of an attribute in the configuration of a composite type!")},
{"ERR_PGMODELER_ATRELEMENTOINVTIPO", QT_TR_NOOP("Assignment of invalid element to type '%1'!")},
{"ERR_PGMODELER_ATRALINHAMENTOINVTIPO",QT_TR_NOOP("Assignment of invalid alignment to type '%1'!")},
{"ERR_PGMODELER_ATRNOMEINVTABRELNN",  QT_TR_NOOP("Assignment of invalid name to the table generated from N-N relationship!")},
{"ERR_PGMODELER_PKESPTIPORELINV", QT_TR_NOOP("The relationship '%1' can not make use of the special primary key. Only generalization/dependency relationships have access to this resource!")},
{"ERR_PGMODELER_ATRSUFIXORELINV",  QT_TR_NOOP("Assignment of invalid suffix to the relationship '%1'!")},
{"ERR_PGMODELERUI_OPROBJINCRELACAO", QT_TR_NOOP("The object '%1' (%2) can not be edited or deleted because it was automatically included through a relationship! To manipulate the object is necessary to make it from the edit form of the relationship that holds it.")},
{"ERR_PGMODELERUI_REMOBJPROTEGIDO", QT_TR_NOOP("The object '%1' (%2) can not be deleted because it is protected!")},
{"ERR_PGMODELERUI_REDECLGRUPODESTAQUE", QT_TR_NOOP("The group '%1' has already been declared earlier!")},
{"ERR_PGMODELERUI_DEFGRUPOLOCALINV", QT_TR_NOOP("The group '%1' can not be built in the groups declaration block ('%2')!")},
{"ERR_PGMODELERUI_DEFGRUPONAODECL", QT_TR_NOOP("The group '%1' was built but not declared in the groups declaration block ('%2')!")},
{"ERR_PGMODELERUI_DEFGRUPOSEMELEM", QT_TR_NOOP("The group '%1' can not be built without possessing child elements!")},
{"ERR_PGMODELERUI_DEFGRUPODUPLICADA", QT_TR_NOOP("The group '%1' can not be built once more because this was done in previous blocks!")},
{"ERR_PGMODELERUI_GRUPODECLNAODEF", QT_TR_NOOP("The group '%1' has been declared but not built!")},
{"ERR_PGMODELERUI_REFCOLTABOBJIDXINV", QT_TR_NOOP("Reference to a column of objects table with invalid index!")},
{"ERR_PGMODELERUI_REFLINTABOBJIDXINV", QT_TR_NOOP("Reference to a row of objects table with invalid index!")},
{"ERR_PGMODELERUI_OPROBJRESERVADO", QT_TR_NOOP("The schema public and the languages plpgsql, c and sql can not be manipulated because they are reserved to PostgreSQL! They are present in the model database only as a reference!")},
{"ERR_PGMODELERUI_CONFFUNCINVALIDAOBJ", QT_TR_NOOP("The new configuration function invalidates the object '%1' (%2)! In this case it is needed to undo the relationship between the affected object and function so that the new configuration of the latter to take effect!")},
{"ERR_PGMODELERUI_TIPOSQLINDEFREFVISAO", QT_TR_NOOP("A vision reference must have at least one SQL application : SELECT, FROM, FROM-WHERE or After WHERE!")},
{"ERR_PGMODELERUI_RESTPKFKSEMCOLUNAS", QT_TR_NOOP("Constraints like primary key, foreign key or unique must have at least one column related to them! For foreign keys must be selected, in addition, the referenced columns!")},
{"ERR_PGMODELERUI_CONFNAOCARREGADA", QT_TR_NOOP("Unable to load one or more configuration files! Please check if files exists in the configuration folder and if they are not corrupted to preventing this error to occur again on the next startup!")},
{"ERR_PGMODELERUI_CONFPADRAONAORESTAURADA", QT_TR_NOOP("Could not find the default settings file '%1'! To restore default settings check the existence of the file and try again!")},
{"ERR_PGMODELERUI_FALHAEXPORTACAO", QT_TR_NOOP("The export process failed due to an error triggered by the PostgreSQL server in an attempt to execute a SQL command. For more details about the error check the exception stack!\n\n** Executed SQL command: **\n\n%1")},
{"ERR_PGMODELERUI_PLUGINNAOCARREGADO", QT_TR_NOOP("Could not load the plugin '%1' from the library '%2'! Message returned by plugin manager: '%3'")},
{"ERR_PGMODELERUI_PLUGINSNAOCARREGADOS", QT_TR_NOOP("One or more plugins were not activated due to errors during the loading process! Check the exception stack for more details.")},
{"ERR_PARSERS_SINTAXEINV", QT_TR_NOOP("Invalid syntax in file %1, line %2, column %3!")},
{"ERR_PARSERS_CONDICIONALINV", QT_TR_NOOP("Invalid conditional instruction '%1' on file %2, line %3, column %4!")},
{"ERR_PARSERS_ATRIBUTODESC", QT_TR_NOOP("Unknown attribute '%1' in file %2, line %3, column %4!")},
{"ERR_PARSERS_METACHARINV", QT_TR_NOOP("Invalid metacharacter '%1' in file %2, line %3, column %4!")},
{"ERR_PARSERS_ATRIBVALORNULO", QT_TR_NOOP("Attribute '%1' with an undefined value in file %2, line %3, column %4!")},
{"ERR_PARSERS_ATRIBBUFXMLVAZIO", QT_TR_NOOP("Assignment of empty XML buffer to parser!")},
{"ERR_PARSERS_ARQDIRNAOCARREGADO", QT_TR_NOOP("Could not access the file or directory %1! Make sure that it exists or if the user has access permissions on it!")},
{"ERR_PARSERS_ATRIBARQDTDVAZIO", QT_TR_NOOP("Assignment of empty DTD file name!")},
{"ERR_PARSERS_ATRIBNOMEDTDVAZIO", QT_TR_NOOP("Assignment of empty name to the DTD declaration!")},
{"ERR_PARSERS_LIBXMLERR", QT_TR_NOOP("Error while interpreting XML buffer at line %1 column %2.\nMessage generated by the parser: '%3'. %4")},
{"ERR_PARSERS_OPRARVELEMNAOALOC", QT_TR_NOOP("Operation on unallocated element tree! It is necessary to load the XML parser buffer and interpret it so that the tree is generated!")},
{"ERR_PARSERS_ARQMODELOINV", QT_TR_NOOP("Could not load file %1. The same appears to be inconsistent or one of its dependencies (DTD files) has errors or is missing!")},
{"ERR_PARSERS_OPRELEMNAOALOC", QT_TR_NOOP("Operation with unallocated tree element!")},
{"ERR_PARSERS_OPRELEMINVARVDOC", QT_TR_NOOP("Operation with element which does not exists in the element tree currently loaded!")},
{"ERR_CONEXBD_ATRPARAMINVCONEX", QT_TR_NOOP("Assignment of a value to an invalid connection parameter!")},
{"ERR_CONEXBD_OPRCONEXNAOALOC", QT_TR_NOOP("Operation on connection not established!")},
{"ERR_CONEXBD_CONEXSEMPARAMCONFIG", QT_TR_NOOP("Attempt to connect without define configuration parameters!")},
{"ERR_CONEXBD_CONEXNAOESTABELECIDA", QT_TR_NOOP("Could not connect to the database.\nMessage returned: '%1'")},
{"ERR_CONEXBD_ATRRESSQLNAOALOC", QT_TR_NOOP("Assignment of not allocated SQL command result!")},
{"ERR_CONEXBD_SGBDRESPINCOMP", QT_TR_NOOP("Unable to allocate the result of the SQL command because the response from the DBMS was not understood by the client!")},
{"ERR_CONEXBD_SGBDERROFATAL", QT_TR_NOOP("Unable to allocate command result for the SQL because the server has generated a fatal error!\nMessage returned by the DBMS: %1")},
{"ERR_CONEXBD_COMANDOSQLVAZIO", QT_TR_NOOP("Unable to allocate the result of the SQL command because the command passed was an empty SQL!")},
{"ERR_CONEXBD_REFCOLTUPLAIDXINV", QT_TR_NOOP("Reference to a column of tuple with invalid index!")},
{"ERR_CONEXBD_REFCOLTUPLANOMEINV", QT_TR_NOOP("Reference to a column of tuple with invalid name!")},
{"ERR_CONEXBD_REFTUPLANAOEXISTE", QT_TR_NOOP("Reference to a tuple with index invalid or the result is empty (no tuples)!")},
{"ERR_CONEXBD_CMDSQLNAOEXECUTADO", QT_TR_NOOP("Could not execute the SQL command.\n Message returned: %1")},
};

Exception::Exception(void)
{
 configureException("",ERR_NULO,"","",-1,"");
}

Exception::Exception(const QString &msg, const QString &method, const QString &file, int line, Exception *exception, const QString &extra_info)
{
 configureException(msg,error_type, method, file, line, extra_info);
 if(exception) addException(*exception);
}

Exception::Exception(ErrorType error_type, const QString &method, const QString &file, int line, Exception *exception, const QString &extra_info)
{
 /* Because the Exception class is not derived from QObject the function tr() is inefficient to translate messages
    so the translation method is called  directly from the application specifying the
    context (Exception) in the ts file and the text to be translated */
 configureException(QApplication::translate("Exception",messages[error_type][ERROR_MESSAGE],"",QApplication::UnicodeUTF8),
                   error_type, method, file, line, extra_info);

 if(exception) addException(*exception);
}

Exception::Exception(const QString &msg, ErrorType error_type, const QString &method, const QString &file, int line, Exception *exception, const QString &extra_info)
{
 configureException(msg,error_type, method, file, line, extra_info);
 if(exception) addException(*exception);
}

Exception::Exception(ErrorType error_type, const QString &method, const QString &file, int line, vector<Exception> &exceptions, const QString &extra_info)
{
 vector<Exception>::iterator itr, itr_end;

 /* Because the Exception class is not derived from QObject the function tr() is inefficient to translate messages
    so the translation method is called  directly from the application specifying the
    context (Exception) in the ts file and the text to be translated */
 configureException(QApplication::translate("Exception",messages[error_type][ERROR_MESSAGE],"",QApplication::UnicodeUTF8),
                   error_type, method, file, line, extra_info);

 itr=exceptions.begin();
 itr_end=exceptions.end();
 while(itr!=itr_end)
 {
  addException((*itr));
  itr++;
 }
}

Exception::Exception(const QString &msg, const QString &method, const QString &file, int line, vector<Exception> &exceptions, const QString &extra_info)
{
 vector<Exception>::iterator itr, itr_end;

 configureException(msg,error_type, method, file, line, extra_info);

 itr=exceptions.begin();
 itr_end=exceptions.end();
 while(itr!=itr_end)
 {
  addException((*itr));
  itr++;
 }
}

void Exception::configureException(const QString &msg, ErrorType error_type, const QString &method, const QString &file, int line, const QString &extra_info)
{
 this->error_type=error_type;
 this->error_msg=msg;
 this->method=method;
 this->file=file;
 this->line=line;
 this->extra_info=extra_info;
}

QString Exception::getErrorMessage(void)
{
 return(error_msg);
}

QString Exception::getErrorMessage(ErrorType error_type)
{
 if(error_type < ERROR_COUNT)
  /* Because the Exception class is not derived from QObject the function tr() is inefficient to translate messages
     so the translation method is called  directly from the application specifying the
     context (Exception) in the ts file and the text to be translated */
  return(QApplication::translate("Exception",messages[error_type][ERROR_MESSAGE],"", QApplication::UnicodeUTF8));
 else
  return("");
}

QString Exception::getErrorCode(ErrorType error_type)
{
 if(error_type < ERROR_COUNT)
   return(messages[error_type][ERROR_CODE]);
 else
  return("");
}

QString Exception::getMethod(void)
{
 return(method);
}

QString Exception::getFile(void)
{
 return(file);
}

QString Exception::getLine(void)
{
 return(QString("%1").arg(line));
}

ErrorType Exception::getErrorType(void)
{
 return(error_type);
}

QString Exception::getExtraInfo(void)
{
 return(extra_info);
}

void Exception::addException(Exception &exception)
{
 deque<Exception>::iterator itr, itr_end;

 itr=exception.exceptions.begin();
 itr_end=exception.exceptions.end();

 while(itr!=itr_end)
 {
  this->exceptions.push_front(Exception(itr->error_msg,itr->error_type,
                                    itr->method,itr->file,itr->line,NULL,itr->extra_info));
  itr++;
 }
 exception.exceptions.clear();
 this->exceptions.push_front(Exception(exception.error_msg,exception.error_type,
                                   exception.method,exception.file,exception.line,NULL,exception.extra_info));
}

void Exception::getExceptionsList(deque<Exception> &list)
{
 list.assign(this->exceptions.begin(), this->exceptions.end());
 list.push_front(Exception(this->error_msg,this->error_type,
                          this->method,this->file,this->line,NULL,this->extra_info));
}

QString Exception::getExceptionsText(void)
{
 deque<Exception> exceptions;
 deque<Exception>::iterator itr, itr_end;
 unsigned idx=0;
 QString exceptions_txt;

 //Get the generated exceptions list
 this->getExceptionsList(exceptions);
 itr=exceptions.begin();
 itr_end=exceptions.end();
 idx=exceptions.size()-1;

 //Append all usefull information about the exceptions on the string
 while(itr!=itr_end)
 {
  exceptions_txt+=QString("[%1] %2 (%3)\n").arg(idx).arg(itr->getFile()).arg(itr->getLine());
  exceptions_txt+=QString("  %1\n").arg(itr->getMethod());
  exceptions_txt+=QString("    [%1] %2\n\n").arg(Exception::getErrorCode(itr->getErrorType())).arg(itr->getErrorMessage());
  itr++; idx--;
 }

 return(exceptions_txt);
}

