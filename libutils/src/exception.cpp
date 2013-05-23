/*
# PostgreSQL Database Modeler (pgModeler)
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
#include "exception.h"
#include <QApplication>

QString Exception::messages[ERROR_COUNT][2]={
	{"ERR_CUSTOM", " "},
	{"ERR_ASG_PSDTYPE_COLUMN", QT_TR_NOOP("Assignment of a pseudo-type to the type of the column!")},
	{"ERR_ASG_ZERO_LENGTH", QT_TR_NOOP("Zero length assignment!")},
	{"ERR_ASG_INV_PRECISION", QT_TR_NOOP("Assignment of a precision greater than the length of the type!")},
	{"ERR_ASG_INV_PREC_TIMESTAMP", QT_TR_NOOP("Assignment of an invalid precision to type time, timestamp or interval. The precision in this case must be equal to or less than 6!")},
	{"ERR_ASG_NOT_ALOC_COLUMN", QT_TR_NOOP("Assignment of a not allocated column to object '%1' (%2)!")},
	{"ERR_REF_COLUMN_INV_INDEX", QT_TR_NOOP("Reference to a column which index is out of the capacity of the column list!")},
	{"ERR_ASG_NOT_ALOC_OBJECT", QT_TR_NOOP("Assignment of not allocated object!")},
	{"ERR_ASG_NOT_ALOC_SCHEMA", QT_TR_NOOP("Assignment of a not allocated schema to object '%1' (%2)!")},
	{"ERR_ASG_OBJ_INV_DEFINITION", QT_TR_NOOP("The object '%1' (%2) has inconsistent configuration (invalid SQL/XML definition)!")},
	{"ERR_ASG_DUPLIC_OBJECT", QT_TR_NOOP("The object '%1' (%2) already exists on '%3' (%4)!")},
	{"ERR_ASG_DUPL_OBJ_CONTAINER", QT_TR_NOOP("The object '%1' (%2) can't be assigned because there is already exists in the container object '%3'!")},
	{"ERR_ASG_OBJECT_INV_TYPE", QT_TR_NOOP("Assigning object of an invalid type!")},
	{"ERR_REM_OBJ_INVALID_TYPE", QT_TR_NOOP("Removing an object of an invalid type!")},
	{"ERR_OBT_OBJ_INVALID_TYPE", QT_TR_NOOP("Obtaining an object of an invalid type!")},
	{"ERR_ASG_EMPTY_NAME_RET_TABLE", QT_TR_NOOP("Assignment of empty name to table return type!")},
	{"ERR_ASG_DUPLIC_PARAM_FUNCTION", QT_TR_NOOP("The insertion of the parameter '%1' will not be possible because there is another parameter with same name in the function '%2'!")},
	{"ERR_INS_DUPLIC_RET_TAB_TYPE", QT_TR_NOOP("The insertion of the table return type '%1' will not be possible because there is another return type with the same name in the '%2'!")},
	{"ERR_REF_PARAM_INV_INDEX", QT_TR_NOOP("Reference to a parameter which index is out of the parameter list bounds!")},
	{"ERR_REF_INV_TRIGGER_EVENT", QT_TR_NOOP("Reference to an event which does not belongs to trigger!")},
	{"ERR_ASG_INV_COLUMN_TRIGGER",QT_TR_NOOP("The column '%1' can't be assigned to the trigger '%2' because does not belongs to the trigger's parent table!")},
	{"ERR_ASG_NOT_ALOC_FUNCTION", QT_TR_NOOP("Assignment of a not allocated function to object '%1' (%2)!")},
	{"ERR_ASG_INV_TRIGGER_FUNCTION", QT_TR_NOOP("Assignment of a function which return type is different from 'trigger'!")},
	{"ERR_ASG_FUNC_INV_PARAM_COUNT", QT_TR_NOOP("Assignment of a function which parameter count is invalid to the object '%1' (%2)!")},
	{"ERR_ASG_FUNC_INV_LANGUAGE", QT_TR_NOOP("Assignment of a function which language is invalid!")},
	{"ERR_ASG_NOT_ALOC_TABLE", QT_TR_NOOP("Assignment of not allocated table to object '%1' (%2)!")},
	{"ERR_REF_ARG_INV_INDEX", QT_TR_NOOP("Reference to an argument which index is out of argument list bounds!")},
	{"ERR_ASG_EMPTY_NAME_OBJECT", QT_TR_NOOP("Assignment of empty name to an object!")},
	{"ERR_ASG_INV_NAME_OBJECT", QT_TR_NOOP("Assignment of a name which contains invalid characters!")},
	{"ERR_ASG_LONG_NAME_OBJECT", QT_TR_NOOP("Assignment of a name which length exceeds the maximum of 63 characters!")},
	{"ERR_ASG_INV_SCHEMA_OBJECT", QT_TR_NOOP("Assignment of schema object which type is invalid!")},
	{"ERR_ASG_INV_TABLESPACE_OBJECT", QT_TR_NOOP("Assignment of tablespace object with invalid type!")},
	{"ERR_ASG_TABSPC_INV_OBJECT", QT_TR_NOOP("Assignment of tablespace to an invalid object!")},
	{"ERR_ASG_TABSPC_INV_CONSTR_TYPE", QT_TR_NOOP("Assignment of tablespace to a constraint which type is invalid! To belong to a tablespace the constraint must be a primary key or unique!")},
	{"ERR_ASG_INV_ROLE_OBJECT", QT_TR_NOOP("Assignment of owner object which type is invalid!")},
	{"ERR_ASG_ROLE_OBJECT_INV_TYPE", QT_TR_NOOP("Assignment of owner to an invalid object!")},
	{"ERR_REF_FUNCTION_INV_TYPE", QT_TR_NOOP("Reference to a function with invalid type!")},
	{"ERR_REF_OPER_ARG_INV_TYPE", QT_TR_NOOP("Reference to an argument of the operator with invalid type!")},
	{"ERR_REF_OPER_INV_TYPE", QT_TR_NOOP("Reference to an operator with invalid type!")},
	{"ERR_ASG_VAL_INV_ROLE_OPT_TYPE", QT_TR_NOOP("Assignment of value to an invalid option type on role!")},
	{"ERR_REF_INV_ROLE_TYPE", QT_TR_NOOP("Reference to an invalid role type!")},
	{"ERR_INS_DUPLIC_ROLE", QT_TR_NOOP("The insertion of the role '%1' is not possible because this is already being referenced by role '%2'!")},
	{"ERR_ROLE_REF_REDUNDANCY",QT_TR_NOOP("Reference redundancy detected by having the role '%1' referencing the role '%2'!")},
	{"ERR_ROLE_MEMBER_ITSELF", QT_TR_NOOP("The role '%1' can not be listed as a member of itself!")},
	{"ERR_REF_ROLE_INV_INDEX", QT_TR_NOOP("Reference to a role which index is out of role list bounds!")},
	{"ERR_INS_EMPTY_RULE_COMMAND", QT_TR_NOOP("Insertion of empty command to the rule!")},
	{"ERR_REF_RULE_CMD_INV_INDEX", QT_TR_NOOP("Reference to a command which index is out of the command list bounds!")},
	{"ERR_INV_INH_COPY_RELATIONSHIP", QT_TR_NOOP("Is not possible to create a self generalization/copy relationship! The table can not inherit or copy their own attributes!")},
	{"ERR_ASG_OBJ_BELONGS_OTHER_TABLE", QT_TR_NOOP("Assignment of an object that already belongs to another table!")},
	{"ERR_ASG_SEQ_DIF_TABLE_SCHEMA", QT_TR_NOOP("Assignment of a schema to the sequence which differs from the schema of the owner table!")},
	{"ERR_ASG_INV_VALUE_SEQ_ATTRIBS", QT_TR_NOOP("Assignment of an invalid value to one of the sequence attributes!")},
	{"ERR_ASG_INV_SEQ_MIN_VALUE", QT_TR_NOOP("Assignment of a minimum value to the sequence which is greater than the maximum value!")},
	{"ERR_ASG_INV_SEQ_START_VALUE", QT_TR_NOOP("Assignment of a start value to the sequence which is extrapolating the range defined by minimum and maximum values!")},
	{"ERR_ASG_INV_SEQ_INCR_VALUE", QT_TR_NOOP("Assignment of a null increment value to the sequence!")},
	{"ERR_ASG_INV_SEQ_CACHE_VALUE", QT_TR_NOOP("Assignment of null cache value to the sequence!")},
	{"ERR_ASG_TAB_DIF_SEQ_SCHEMA", QT_TR_NOOP("Assignment of owner table which is not in the same schema as the sequence '%1'!")},
	{"ERR_ASG_SEQ_OWNER_DIF_TABLE", QT_TR_NOOP("Assignment of owner table which does not belong to the same owner of the sequence '%1'!")},
	{"ERR_ASG_INEXIST_OWNER_COL_SEQ", QT_TR_NOOP("Assignment of a nonexistent owner column to the sequence '%1'!")},
	{"ERR_ASG_INV_OWNER_COL_SEQ", QT_TR_NOOP("Assignment of an owner column to the sequence '%1' that is not related to any table!")},
	{"ERR_REF_LABEL_INV_INDEX", QT_TR_NOOP("Reference to a label which index is out of labels list bounds!")},
	{"ERR_ALOC_OBJECT_INV_TYPE", QT_TR_NOOP("Allocation of object with invalid type!")},
	{"ERR_ASG_FUNCTION_INV_RET_TYPE", QT_TR_NOOP("Assignment of a function with invalid return type to object '%1' (%2)!")},
	{"ERR_ASG_FUNCTION_INV_PARAMS", QT_TR_NOOP("Assignment of a function with invalid parameter(s) type(s) to object '%1' (%2)!")},
	{"ERR_ASG_NOT_ALOC_LANGUAGE", QT_TR_NOOP("Assignment of not allocated language!")},
	{"ERR_ASG_INV_LANGUAGE_OBJECT", QT_TR_NOOP("Assignment of language object which type is invalid!")},
	{"ERR_REF_TYPE_INV_INDEX", QT_TR_NOOP("Reference to data type with an index outside the capacity of data types list!")},
	{"ERR_ASG_nullptr_TYPE_OBJECT", QT_TR_NOOP("Assignment of a null type to to object '%1' (%2)!")},
	{"ERR_ASG_INV_TYPE_OBJECT", QT_TR_NOOP("Assignment of invalid type to the object!")},
	{"ERR_ASG_EMPTY_DIR_NAME", QT_TR_NOOP("Assignment of an empty directory to object '%1' (%2)!")},
	{"ERR_OBT_TYPES_INV_QUANTITY", QT_TR_NOOP("Obtaining types with invalid quantity!")},
	{"ERR_INS_DUPLIC_ITEMS", QT_TR_NOOP("Insertion of item which already exists in the attributes list of the type!")},
	{"ERR_INS_INV_TYPE_ATTRIB", QT_TR_NOOP("Insertion of invalid item in the attributes list of the type!")},
	{"ERR_INS_DUPLIC_ENUM_ITEM", QT_TR_NOOP("Insertion of item which already exists in the enumarations list of the type!")},
	{"ERR_INS_INV_TYPE_ENUM_ITEM", QT_TR_NOOP("Insertion of invalid item in the enumerations list of the type!")},
	{"ERR_REF_ATTRIB_INV_INDEX", QT_TR_NOOP("Reference to an attribute which index is out of the attributes list bounds!")},
	{"ERR_REF_ENUM_INV_INDEX", QT_TR_NOOP("Reference to an enumeration which index is out of the enumerations list bounds!")},
	{"ERR_ASG_INV_TYPE_CONFIG", QT_TR_NOOP("Assignment of invalid configuration to the type!")},
	{"ERR_INS_DUPLIC_TYPE", QT_TR_NOOP("The data type '%1' can not be assigned because it already exists in the types list of the aggregate function '%2'!")},
	{"ERR_ASG_INV_OPER_ARGS", QT_TR_NOOP("Assignment of an operator which input type count is invalid to aggregate function!")},
	{"ERR_ASG_INV_OPERATOR_TYPES", QT_TR_NOOP("Assignment of an operator which types of arguments is invalid!")},
	{"ERR_ASG_RESERVED_NAME", QT_TR_NOOP("Assignment of system reserved name to the object '%1' (%2)!")},
	{"ERR_USING_INV_FUNC_CONFIG", QT_TR_NOOP("One function with invalid configuration is been used by the object '%1' (%2)!")},
	{"ERR_ASG_INV_SUPSTG_NUMBER", QT_TR_NOOP("Assignment of an invalid strategy/support number to an operator class element!")},
	{"ERR_INS_DUPLIC_ELEMENT", QT_TR_NOOP("Insertion of element which already exists in the element list!")},
	{"ERR_REF_ELEM_INV_INDEX", QT_TR_NOOP("Reference to an element which index is out of element list bounds!")},
	{"ERR_REF_OBJ_INV_INDEX", QT_TR_NOOP("Reference to an object which index is out of object list bounds!")},
	{"ERR_REM_NOT_ALOC_OBJECT", QT_TR_NOOP("Removal of an object not allocated!")},
	{"ERR_REM_DIRECT_REFERENCE", QT_TR_NOOP("The object '%1' (%2) can not be removed because it is being referenced by object '%3' (%4)!")},
	{"ERR_REM_INDIRECT_REFERENCE", QT_TR_NOOP("The object '%1' (%2) can not be removed because it is being referenced by object '%3' (%4) that belongs to '%5' (%6)!")},
	{"ERR_OPR_OBJ_INV_TYPE", QT_TR_NOOP("Operation with object(s) which type(s) is invalid!")},
	{"ERR_REF_OBJ_INV_TYPE", QT_TR_NOOP("Reference to object with invalid type!")},
	{"ERR_OPR_NOT_ALOC_OBJECT", QT_TR_NOOP("Operation with object not allocated!")},
	{"ERR_LINK_TABLES_NO_PK", QT_TR_NOOP("The creation of the relationship '%1' between the table '%2' and '%3' can not be done because one does not have a primary key. If the relationship is of the type n-n both tables must have primary keys!")},
	{"ERR_NOT_IMPL_REL_TYPE", QT_TR_NOOP("The relationship of the type 1-1 where both tables are mandatory participation is not implemented because it requires fusion between the tables that breaks the modeling done by the user!")},
	{"ERR_ASG_INV_EXPR_OBJECT", QT_TR_NOOP("Assignment of an invalid expression to the object!")},
	{"ERR_ASG_EXISTING_PK_TABLE", QT_TR_NOOP("Assignment of a primary key to a table which already has one!")},
	{"ERR_INV_IDENT_RELATIOSHIP", QT_TR_NOOP("Identifier relationship can not be created for a self relationship, relationships of the type n-n, copy or generalization!")},
	{"ERR_DUPLIC_COLS_COPY_REL", QT_TR_NOOP("Unable to create a copy relationship because the column '%1' in table '%2' already exists in table '%3'!")},
	{"ERR_INCOMP_COLS_INHERIT_REL", QT_TR_NOOP("Unable to create the generalization relationship because the column '%1' in table '%2' can not be merged with the column '%3' of table '%4' because they have incompatible types!")},
	{"ERR_ASG_OBJ_INV_REL_TYPE", QT_TR_NOOP("An attribute can not be added to a copy or generalization relationship!")},
	{"ERR_ASG_FOREIGN_KEY_REL", QT_TR_NOOP("A foreign key can not be added to a relationship because is created automatically when this is connected!")},
	{"ERR_REF_OBJ_INEXISTS_MODEL", QT_TR_NOOP("The object '%1' (%2) is referencing the object '%3' (%4) which was not found in the model!")},
	{"ERR_REF_INEXIST_USER_TYPE", QT_TR_NOOP("Reference to an user-defined data type that not exists in the model!")},
	{"ERR_ASG_INV_MAX_SIZE_OP_LIST", QT_TR_NOOP("Assignment of invalid maximum size to operation list!")},
	{"ERR_FILE_NOT_WRITTEN", QT_TR_NOOP("Unable to write the file %1! Make sure the directory exists, or if the user has access permissions on it!")},
	{"ERR_FILE_NOT_WRITTER_INV_DEF", QT_TR_NOOP("Unable to write the model in the file %1 due to one or more errors in the definition generation process!")},
	{"ERR_DUPLIC_RELATIONSHIP", QT_TR_NOOP("There is already a relationship between '%1' (%2) and '%3' (%4) in the model!")},
	{"ERR_INS_REL_GENS_REDUNDACY", QT_TR_NOOP("The configuration of the relationship '%1' generates a redundancy between the relationships '%2'. Redundancy on identifier or generalization/copy relationships are not accepted since they result in  incorrect column spreading making the model inconsistent!")},
	{"ERR_INVALIDATED_OBJECTS", QT_TR_NOOP("One or more objects were invalidated and automatically removed because they were referencing table columns which were included through relationships and which no longer exists due to disconnection of relationships or exclusion of such generated columns!")},
	{"ERR_INV_PRIM_KEY_ALOCATION", QT_TR_NOOP("The primary key '%1' can only be allocated if declared within a block of code that defines a table or relationship!")},
	{"ERR_REF_INV_PRIVILEGE_TYPE",QT_TR_NOOP("Reference to an invalid privilege type!")},
	{"ERR_INS_DUP_ROLE_PERMISSION", QT_TR_NOOP("Insertion of a role which already exists in the role list of the permission!")},
	{"ERR_ASG_INCOMP_PRIV_OBJECT", QT_TR_NOOP("Assignment of privilege incompatible with the type of object referenced by permission!")},
	{"ERR_ASG_DUPLIC_PERMISSION", QT_TR_NOOP("There is already a permission on object '%1' (%2) which has one or more equal roles from those present on permission to be assigned to the object!")},
	{"ERR_PERM_REF_INEXIST_OBJECT", QT_TR_NOOP("A permission is referencing the object '%1' (%2) which was not found in the model!")},
	{"ERR_ALOC_OBJECT_NO_SCHEMA", QT_TR_NOOP("The object '%1' (%2) can not be created because its not being assigned to any schema!")},
	{"ERR_ASG_DUP_TABLESPACE_DIR", QT_TR_NOOP("The tablespace '%1' can not be inserted into the model because it points to the same directory as the tablespace '%2'!")},
	{"ERR_ASG_INV_DOMAIN_ARRAY", QT_TR_NOOP("It is not possible to create arrays of domains or sequences (dimension >= 1)! PostgreSQL does not yet implement this feature!")},
	{"ERR_ASG_CODE_FUNC_C_LANGUAGE", QT_TR_NOOP("The function '%1' can not get a source code as a definition because its language is set to C. Use the attributes symbol and dynamic library instead!")},
	{"ERR_ASG_FUNC_REFLIB_LANG_NOT_C", QT_TR_NOOP("The function '%1' can have the attributes symbol and dynamic library configured only if the language is set to C. For all other cases you must specify a source code that defines it in the DBMS!")},
	{"ERR_ASG_INV_COM_OPEERATOR", QT_TR_NOOP("The operator '%1' can not be assigned as a comutator of operator '%2' because it has incompatible settings!")},
	{"ERR_ASG_INV_NEG_OPERATOR", QT_TR_NOOP("The operator '%1' can not be assigned as negator of operator '%2' because it has incompatible settings!")},
	{"ERR_USER_TYPE_SELF_REFERENCE", QT_TR_NOOP("The type '%1' can not self refer in the attributes 'element' or 'copy type' or be used as a data type of an attribute in the configuration of a composite type!")},
	{"ERR_ASG_INV_ELEMENT_TYPE", QT_TR_NOOP("Assignment of invalid element to type '%1'!")},
	{"ERR_ASG_INV_ALIGNMENT_TYPE",QT_TR_NOOP("Assignment of invalid alignment to type '%1'!")},
	{"ERR_ASG_INV_NAME_TABLE_RELNN",  QT_TR_NOOP("Assignment of invalid name to the table generated from N-N relationship!")},
	{"ERR_INV_USE_ESPECIAL_PK", QT_TR_NOOP("The relationship '%1' can not make use of the special primary key because it is marked as identifier or it is a many-to-many relationship!")},
	{"ERR_OPR_REL_INCL_OBJECT", QT_TR_NOOP("The object '%1' (%2) can not be edited or deleted because it was automatically included through a relationship! If the object is an attribute or constraint the modifications must be done on the relationship editing form.")},
	{"ERR_REM_PROTECTED_OBJECT", QT_TR_NOOP("The object '%1' (%2) can not be deleted because it is protected!")},
	{"ERR_REDECL_HL_GROUP", QT_TR_NOOP("The group '%1' has already been declared earlier!")},
	{"ERR_DEF_INV_GROUP_DECL", QT_TR_NOOP("The group '%1' can not be built in the groups declaration block ('%2')!")},
	{"ERR_DEF_NOT_DECL_GROUP", QT_TR_NOOP("The group '%1' was built but not declared in the groups declaration block ('%2')!")},
	{"ERR_DEF_EMPTY_GROUP", QT_TR_NOOP("The group '%1' can not be built without possessing child elements!")},
	{"ERR_DEF_DUPLIC_GROUP", QT_TR_NOOP("The group '%1' can not be built once more because this was done in previous blocks!")},
	{"ERR_GROUP_DECL_NOT_DEFINED", QT_TR_NOOP("The group '%1' has been declared but not built!")},
	{"ERR_REF_COL_OBJTAB_INV_INDEX", QT_TR_NOOP("Reference to a column of the objects table with invalid index!")},
	{"ERR_REF_LIN_OBJTAB_INV_INDEX", QT_TR_NOOP("Reference to a row of the objects table with invalid index!")},
	{"ERR_OPR_RESERVED_OBJECT", QT_TR_NOOP("The schema public and the languages plpgsql, c and sql can not be manipulated because they are reserved to PostgreSQL! They are present in the model database only as a reference!")},
	{"ERR_FUNC_CONFIG_INV_OBJECT", QT_TR_NOOP("The new configuration function invalidates the object '%1' (%2)! In this case it is needed to undo the relationship between the affected object and function so that the new configuration of the latter to take effect!")},
	{"ERR_SQL_SCOPE_INV_VIEW_REF", QT_TR_NOOP("A view reference must be used in at least one these SQL scopes: View Definition, SELECT-FROM, FROM-WHERE or After WHERE!")},
	{"ERR_CONSTR_NO_COLUMNS", QT_TR_NOOP("Constraints like primary key, foreign key or unique must have at least one column related to them! For foreign keys must be selected, in addition, the referenced columns!")},
	{"ERR_CONFIG_NOT_LOADED", QT_TR_NOOP("Unable to load one or more configuration files! Please check if files exists in the configuration folder and if they are not corrupted to preventing this error to occur again on the next startup!")},
	{"ERR_DEFAULT_CONFIG_NOT_REST", QT_TR_NOOP("Could not find the default settings file '%1'! To restore default settings check the existence of the file and try again!")},
	{"ERR_EXPORT_FAILURE", QT_TR_NOOP("The export process failed due to an error triggered by the PostgreSQL server in an attempt to execute a SQL command. For more details about the error check the exception stack!\n\n** Executed SQL command: **\n\n%1")},
	{"ERR_PLUGIN_NOT_LOADED", QT_TR_NOOP("Could not load the plugin '%1' from the library '%2'! Message returned by plugin manager: '%3'")},
	{"ERR_PLUGINS_NOT_LOADED", QT_TR_NOOP("One or more plugins were not activated due to errors during the loading process! Check the exception stack for more details.")},
	{"ERR_INVALID_SYNTAX", QT_TR_NOOP("Invalid syntax in file %1, line %2, column %3!")},
	{"ERR_INV_CONDITIONAL", QT_TR_NOOP("Invalid conditional instruction '%1' on file %2, line %3, column %4!")},
	{"ERR_UNK_ATTRIBUTE", QT_TR_NOOP("Unknown attribute '%1' in file %2, line %3, column %4!")},
	{"ERR_INV_METACHARACTER", QT_TR_NOOP("Invalid metacharacter '%1' in file %2, line %3, column %4!")},
	{"ERR_UNDEF_ATTRIB_VALUE", QT_TR_NOOP("Attribute '%1' with an undefined value in file %2, line %3, column %4!")},
	{"ERR_ASG_EMPTY_XML_BUFFER", QT_TR_NOOP("Assignment of empty XML buffer to parser!")},
	{"ERR_FILE_DIR_NOT_ACCESSED", QT_TR_NOOP("Could not access the file or directory %1! Make sure that it exists or if the user has access permissions on it!")},
	{"ERR_ASG_EMPTY_DTD_FILE", QT_TR_NOOP("Assignment of empty DTD file name!")},
	{"ERR_ASG_EMPTY_DTD_NAME", QT_TR_NOOP("Assignment of empty name to the DTD declaration!")},
	{"ERR_LIBXMLERR", QT_TR_NOOP("Error while interpreting XML buffer at line %1 column %2.\nMessage generated by the parser: '%3'. %4")},
	{"ERR_OPR_NOT_ALOC_ELEM_TREE", QT_TR_NOOP("Operation on unallocated element tree! It is necessary to load the XML parser buffer and interpret it so that the tree is generated!")},
	{"ERR_LOAD_INV_MODEL_FILE", QT_TR_NOOP("Could not load file %1. The same appears to be inconsistent or one of its dependencies (DTD files) has errors or is missing!")},
	{"ERR_OPR_NOT_ALOC_ELEMENT", QT_TR_NOOP("Operation with unallocated tree element!")},
	{"ERR_OPR_INEXIST_ELEMENT", QT_TR_NOOP("Operation with element which does not exists in the element tree currently loaded!")},
	{"ERR_ASG_INV_CONN_PARAM", QT_TR_NOOP("Assignment of a value to an invalid connection parameter!")},
	{"ERR_OPR_NOT_ALOC_CONN", QT_TR_NOOP("Operation on connection not established!")},
	{"ERR_CONNECTION_NOT_CONFIGURED", QT_TR_NOOP("Attempt to connect without define configuration parameters!")},
	{"ERR_CONNECTION_NOT_STABLISHED", QT_TR_NOOP("Could not connect to the database.\nMessage returned: '%1'")},
	{"ERR_ASG_SQL_RESULT_NOT_ALOC", QT_TR_NOOP("Assignment of not allocated SQL command result!")},
	{"ERR_INCOMPREHENSIBLE_DBMS_RESP", QT_TR_NOOP("Unable to allocate the result of the SQL command because the response from the DBMS was not understood by the client!")},
	{"ERR_DBMS_FATAL_ERROR", QT_TR_NOOP("Unable to allocate command result for the SQL because the server has generated a fatal error!\nMessage returned by the DBMS: %1")},
	{"ERR_EMPTY_SQL_COMMAND", QT_TR_NOOP("Unable to allocate the result of the SQL command because the command passed was an empty SQL!")},
	{"ERR_REF_TUPLE_COL_INV_INDEX", QT_TR_NOOP("Reference to a column of tuple with invalid index!")},
	{"ERR_REF_TUPLE_COL_INV_NAME", QT_TR_NOOP("Reference to a column of tuple with invalid name!")},
	{"ERR_REF_TUPLE_INEXISTENT", QT_TR_NOOP("Reference to a tuple with index invalid or the result is empty (no tuples)!")},
	{"ERR_CMD_SQL_NOT_EXECUTED", QT_TR_NOOP("Could not execute the SQL command.\n Message returned: %1")},
	{"ERR_INV_VIEW_DEF_EXPRESSION", QT_TR_NOOP("Invalid use of a view referece as whole SQL definition! The assigned reference must be an expression!")},
	{"ERR_ASG_SEC_VIEW_DEF_EXPRESSION", QT_TR_NOOP("Assignment of a second definition expression to the view!")},
	{"ERR_MIX_VIEW_DEF_EXPR_REFS", QT_TR_NOOP("It's not possible mix ordinary references (SELECT-FROM, FROM-WHERE, After WHERE) with references used as view SQL definition!")},
	{"ERR_ASG_INV_COLLATION_OBJECT", QT_TR_NOOP("Assignment of collation object which type is invalid!")},
	{"ERR_PK_USING_COLS_ADDED_BY_REL", QT_TR_NOOP("At the moment pgModeler doesn't support the creation of primary keys which some columns were generated by relationship connection. To create primary keys with this feature you can use the field 'Indentifier' or the tab 'Primary key' on relationship editing form!")},
	{"ERR_EMPTY_LOCAL_ATTRIB_COLLATION", QT_TR_NOOP("Collations must be created at least with attributes LC_COLLATE and LC_CTYPE defined!")},
	{"ERR_OBJECT_REFERENCING_ITSELF", QT_TR_NOOP("The object '%1' (%2) cannot reference itself! This operation is not permitted for this kind of object!")},
	{"ERR_ASG_INV_OPFAM_OPCLSELEM", QT_TR_NOOP("Only operator families which uses 'btree' as indexing method is accepted by operator class elements!")},
	{"ERR_REF_INV_LIKE_OP_TYPE", QT_TR_NOOP("Reference to an invalid copy table option!")},
	{"ERR_COPY_REL_TAB_DEFINED", QT_TR_NOOP("Copy relationship between tables '%1' and '%2' cannot be done because the first one already copies attributes from '%3'! Tables can have only one copy table!")},
	{"ERR_TABLE_TRIG_INSTEADOF_FIRING",QT_TR_NOOP("The INSTEAD OF mode cannot be used on triggers that belongs to tables! This is available only for view triggers!")},
	{"ERR_TRIGGER_INV_TRUNCATE_USAGE",QT_TR_NOOP("The TRUNCATE event can only be used when the trigger executes for each statement and belongs to a table!")},
	{"ERR_TRIGGER_INV_INSTEADOF_USAGE",QT_TR_NOOP("The INSTEAD OF mode cannot be used on view triggers that executes for each statement!")},
	{"ERR_CONST_TRIG_NOT_AFTER_ROW",QT_TR_NOOP("Constraint triggers can only be executed on AFTER events and for each row!")},
	{"ERR_VIEW_TRIG_INV_AFTBFR_USAGE",QT_TR_NOOP("A view trigger cannot be AFTER/BEFORE when it executes for each row!")},
	{"ERR_TRIGGER_INV_INSTEADOF_UPDATE",QT_TR_NOOP("A trigger cannot make reference to columns when using INSTEAD OF mode and UPDATE event!")},
	{"ERR_ASG_COLUMN_NO_PARENT",QT_TR_NOOP("Assignment of a column which has no parent table to the object '%1' (%2)!")},
	{"ERR_TRIG_USING_CONSTRIG_ATRIBS",QT_TR_NOOP("Only constraint triggers can be deferrable or reference another table!")},
	{"ERR_REF_FUNCTION_INV_TYPE_CONF", QT_TR_NOOP("Reference to a function id which is incompatible whit the user define type configuration!")},
	{"ERR_ASG_INV_OPCLASS_OBJ", QT_TR_NOOP("The operator class assigned to the object '%1' (%2) must use 'btree' as indexing method!")},
	{"ERR_INV_POSTGRESQL_VERSION", QT_TR_NOOP("Invalid PostgreSQL version '%1' assigned to the schema parser! Valid versions are: %2")},
	{"ERR_VALIDATION_FAILURE", QT_TR_NOOP("The validation process failed due to an error triggered by the validation helper. For more details about the error check the exception stack!")},
	{"ERR_REG_EXT_NOT_HANDLING_TYPE", QT_TR_NOOP("The extension '%1' is registered as a data type and cannot have the attribute 'handles datatype' modified!")},
	{"ERR_ALOC_INV_FK_RELATIONSHIP", QT_TR_NOOP("The fk relationship '%1' cannot be created because the foreign-key that represents it wasn't created on table '%2'!")},
	{"ERR_ASG_INV_NAME_PATTERN", QT_TR_NOOP("Assignement of an invalid object name pattern to the relationship '%1'!")},
	{"ERR_REF_INV_NAME_PATTERN_ID", QT_TR_NOOP("Reference to an invalid object name pattern id on the relationship '%1'!")}
};

Exception::Exception(void)
{
	configureException("",ERR_CUSTOM,"","",-1,"");
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
	configureException(QApplication::translate("Exception",messages[error_type][ERROR_MESSAGE].toStdString().c_str(),"", -1),
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
	configureException(QApplication::translate("Exception",messages[error_type][ERROR_MESSAGE].toStdString().c_str(),"",-1),
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
		return(QApplication::translate("Exception",messages[error_type][ERROR_MESSAGE].toStdString().c_str(),"", -1));
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
	vector<Exception>::iterator itr, itr_end;

	itr=exception.exceptions.begin();
	itr_end=exception.exceptions.end();

	while(itr!=itr_end)
	{
		this->exceptions.push_back(Exception(itr->error_msg,itr->error_type,
																					itr->method,itr->file,itr->line,nullptr,itr->extra_info));
		itr++;
	}
	exception.exceptions.clear();
	this->exceptions.push_back(Exception(exception.error_msg,exception.error_type,
																			 exception.method,exception.file,exception.line,nullptr,exception.extra_info));
}

void Exception::getExceptionsList(vector<Exception> &list)
{
	list.assign(this->exceptions.begin(), this->exceptions.end());
	list.push_back(Exception(this->error_msg,this->error_type,
													 this->method,this->file,this->line,nullptr,this->extra_info));
}

QString Exception::getExceptionsText(void)
{
	vector<Exception> exceptions;
	vector<Exception>::iterator itr, itr_end;
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

