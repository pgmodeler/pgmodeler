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

/**
\ingroup libutils
\class Exception
\brief Catalogs all the errors raised by pgModeler
\note <strong>Creation date:</strong> 10/07/2006
*/

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QObject>
#include <exception>
#include <signal.h>
#include <vector>
#include <deque>
#include "utf8string.h"

using namespace std;

const int ERROR_COUNT=198;

/*
 ErrorType enum format: ERR_[LIBRARY]_[[OPERATION_CODE][ERROR_CODE]] where:

 OPERATION_CODE is composed as:
	 INS=Insertion
	 ASG=Assignment
	 REF=Reference
	 REM=Removing
	 RES=Restoration
	 OBT=Obtaining
*/

enum ErrorType {
	ERR_CUSTOM,
	ERR_ASG_PSDTYPE_COLUMN,
	ERR_ASG_ZERO_LENGTH,
	ERR_ASG_INV_PRECISION,
	ERR_ASG_INV_PREC_TIMESTAMP,
	ERR_ASG_NOT_ALOC_COLUMN,
	ERR_REF_COLUMN_INV_INDEX,
	ERR_ASG_NOT_ALOC_OBJECT,
	ERR_ASG_NOT_ALOC_SCHEMA,
	ERR_ASG_OBJ_INV_DEFINITION,
	ERR_ASG_DUPLIC_OBJECT,
	ERR_ASG_DUPL_OBJ_CONTAINER,
	ERR_ASG_OBJECT_INV_TYPE,
	ERR_REM_OBJ_INVALID_TYPE,
	ERR_OBT_OBJ_INVALID_TYPE,
	ERR_ASG_EMPTY_NAME_RET_TABLE,
	ERR_ASG_DUPLIC_PARAM_FUNCTION,
	ERR_INS_DUPLIC_RET_TAB_TYPE,
	ERR_REF_PARAM_INV_INDEX,
	ERR_REF_INV_TRIGGER_EVENT,
	ERR_ASG_INV_COLUMN_TRIGGER,
	ERR_ASG_NOT_ALOC_FUNCTION,
	ERR_ASG_INV_TRIGGER_FUNCTION,
	ERR_ASG_FUNC_INV_PARAM_COUNT,
	ERR_ASG_FUNC_INV_LANGUAGE,
	ERR_ASG_NOT_ALOC_TABLE,
	ERR_REF_ARG_INV_INDEX,
	ERR_ASG_EMPTY_NAME_OBJECT,
	ERR_ASG_INV_NAME_OBJECT,
	ERR_ASG_LONG_NAME_OBJECT,
	ERR_ASG_INV_SCHEMA_OBJECT,
	ERR_ASG_INV_TABLESPACE_OBJECT,
	ERR_ASG_TABSPC_INV_OBJECT,
	ERR_ASG_TABSPC_INV_CONSTR_TYPE,
	ERR_ASG_INV_ROLE_OBJECT,
	ERR_ASG_ROLE_OBJECT_INV_TYPE,
	ERR_REF_FUNCTION_INV_TYPE,
	ERR_REF_OPER_ARG_INV_TYPE,
	ERR_REF_OPER_INV_TYPE,
	ERR_ASG_VAL_INV_ROLE_OPT_TYPE,
	ERR_REF_INV_ROLE_TYPE,
	ERR_INS_DUPLIC_ROLE,
	ERR_ROLE_REF_REDUNDANCY,
	ERR_ROLE_MEMBER_ITSELF,
	ERR_REF_ROLE_INV_INDEX,
	ERR_INS_EMPTY_RULE_COMMAND,
	ERR_REF_RULE_CMD_INV_INDEX,
	ERR_INV_INH_COPY_RELATIONSHIP,
	ERR_ASG_OBJ_BELONGS_OTHER_TABLE,
	ERR_ASG_SEQ_DIF_TABLE_SCHEMA,
	ERR_ASG_INV_VALUE_SEQ_ATTRIBS,
	ERR_ASG_INV_SEQ_MIN_VALUE,
	ERR_ASG_INV_SEQ_START_VALUE,
	ERR_ASG_INV_SEQ_INCR_VALUE,
	ERR_ASG_INV_SEQ_CACHE_VALUE,
	ERR_ASG_TAB_DIF_SEQ_SCHEMA,
	ERR_ASG_SEQ_OWNER_DIF_TABLE,
	ERR_ASG_INEXIST_OWNER_COL_SEQ,
	ERR_ASG_INV_OWNER_COL_SEQ,
	ERR_REF_LABEL_INV_INDEX,
	ERR_ALOC_OBJECT_INV_TYPE,
	ERR_ASG_FUNCTION_INV_RET_TYPE,
	ERR_ASG_FUNCTION_INV_PARAMS,
	ERR_ASG_NOT_ALOC_LANGUAGE,
	ERR_ASG_INV_LANGUAGE_OBJECT,
	ERR_REF_TYPE_INV_INDEX,
	ERR_ASG_nullptr_TYPE_OBJECT,
	ERR_ASG_INV_TYPE_OBJECT,
	ERR_ASG_EMPTY_DIR_NAME,
	ERR_OBT_TYPES_INV_QUANTITY,
	ERR_INS_DUPLIC_ITEMS,
	ERR_INS_INV_TYPE_ATTRIB,
	ERR_INS_DUPLIC_ENUM_ITEM,
	ERR_INS_INV_TYPE_ENUM_ITEM,
	ERR_REF_ATTRIB_INV_INDEX,
	ERR_REF_ENUM_INV_INDEX,
	ERR_ASG_INV_TYPE_CONFIG,
	ERR_INS_DUPLIC_TYPE,
	ERR_ASG_INV_OPER_ARGS,
	ERR_ASG_INV_OPERATOR_TYPES,
	ERR_ASG_RESERVED_NAME,
	ERR_USING_INV_FUNC_CONFIG,
	ERR_ASG_INV_SUPSTG_NUMBER,
	ERR_INS_DUPLIC_ELEMENT,
	ERR_REF_ELEM_INV_INDEX,
	ERR_REF_OBJ_INV_INDEX,
	ERR_REM_NOT_ALOC_OBJECT,
	ERR_REM_DIRECT_REFERENCE,
	ERR_REM_INDIRECT_REFERENCE,
	ERR_OPR_OBJ_INV_TYPE,
	ERR_REF_OBJ_INV_TYPE,
	ERR_OPR_NOT_ALOC_OBJECT,
	ERR_LINK_TABLES_NO_PK,
	ERR_NOT_IMPL_REL_TYPE,
	ERR_ASG_INV_EXPR_OBJECT,
	ERR_ASG_EXISTING_PK_TABLE,
	ERR_INV_IDENT_RELATIOSHIP,
	ERR_DUPLIC_COLS_COPY_REL,
	ERR_INCOMP_COLS_INHERIT_REL,
	ERR_ASG_OBJ_INV_REL_TYPE,
	ERR_ASG_FOREIGN_KEY_REL,
	ERR_REF_OBJ_INEXISTS_MODEL,
	ERR_REF_INEXIST_USER_TYPE,
	ERR_ASG_INV_MAX_SIZE_OP_LIST,
	ERR_FILE_NOT_WRITTEN,
	ERR_FILE_NOT_WRITTER_INV_DEF,
	ERR_DUPLIC_RELATIONSHIP,
	ERR_INS_REL_GENS_REDUNDACY,
	ERR_INVALIDATED_OBJECTS,
	ERR_INV_PRIM_KEY_ALOCATION,
	ERR_REF_INV_PRIVILEGE_TYPE,
	ERR_INS_DUP_ROLE_PERMISSION,
	ERR_ASG_INCOMP_PRIV_OBJECT,
	ERR_ASG_DUPLIC_PERMISSION,
	ERR_PERM_REF_INEXIST_OBJECT,
	ERR_ALOC_OBJECT_NO_SCHEMA,
	ERR_ASG_DUP_TABLESPACE_DIR,
	ERR_ASG_INV_DOMAIN_ARRAY,
	ERR_ASG_CODE_FUNC_C_LANGUAGE,
	ERR_ASG_FUNC_REFLIB_LANG_NOT_C,
	ERR_ASG_INV_COM_OPEERATOR,
	ERR_ASG_INV_NEG_OPERATOR,
	ERR_USER_TYPE_SELF_REFERENCE,
	ERR_ASG_INV_ELEMENT_TYPE,
	ERR_ASG_INV_ALIGNMENT_TYPE,
	ERR_ASG_INV_NAME_TABLE_RELNN,
	ERR_INV_USE_ESPECIAL_PK,
	ERR_OPR_REL_INCL_OBJECT,
	ERR_REM_PROTECTED_OBJECT,
	ERR_REDECL_HL_GROUP,
	ERR_DEF_INV_GROUP_DECL,
	ERR_DEF_NOT_DECL_GROUP,
	ERR_DEF_EMPTY_GROUP,
	ERR_DEF_DUPLIC_GROUP,
	ERR_GROUP_DECL_NOT_DEFINED,
	ERR_REF_COL_OBJTAB_INV_INDEX,
	ERR_REF_LIN_OBJTAB_INV_INDEX,
	ERR_OPR_RESERVED_OBJECT,
	ERR_FUNC_CONFIG_INV_OBJECT,
	ERR_SQL_SCOPE_INV_VIEW_REF,
	ERR_CONSTR_NO_COLUMNS,
	ERR_CONFIG_NOT_LOADED,
	ERR_DEFAULT_CONFIG_NOT_REST,
	ERR_EXPORT_FAILURE,
	ERR_PLUGIN_NOT_LOADED,
	ERR_PLUGINS_NOT_LOADED,
	ERR_INVALID_SYNTAX,
	ERR_INV_CONDITIONAL,
	ERR_UNK_ATTRIBUTE,
	ERR_INV_METACHARACTER,
	ERR_UNDEF_ATTRIB_VALUE,
	ERR_ASG_EMPTY_XML_BUFFER,
	ERR_FILE_DIR_NOT_ACCESSED,
	ERR_ASG_EMPTY_DTD_FILE,
	ERR_ASG_EMPTY_DTD_NAME,
	ERR_LIBXMLERR,
	ERR_OPR_NOT_ALOC_ELEM_TREE,
	ERR_LOAD_INV_MODEL_FILE,
	ERR_OPR_NOT_ALOC_ELEMENT,
	ERR_OPR_INEXIST_ELEMENT,
	ERR_ASG_INV_CONN_PARAM,
	ERR_OPR_NOT_ALOC_CONN,
	ERR_CONNECTION_NOT_CONFIGURED,
	ERR_CONNECTION_NOT_STABLISHED,
	ERR_ASG_SQL_RESULT_NOT_ALOC,
	ERR_INCOMPREHENSIBLE_DBMS_RESP,
	ERR_DBMS_FATAL_ERROR,
	ERR_EMPTY_SQL_COMMAND,
	ERR_REF_TUPLE_COL_INV_INDEX,
	ERR_REF_TUPLE_COL_INV_NAME,
	ERR_REF_TUPLE_INEXISTENT,
	ERR_CMD_SQL_NOT_EXECUTED,
	ERR_INV_VIEW_DEF_EXPRESSION,
	ERR_ASG_SEC_VIEW_DEF_EXPRESSION,
	ERR_MIX_VIEW_DEF_EXPR_REFS,
	ERR_ASG_INV_COLLATION_OBJECT,
	ERR_PK_USING_COLS_ADDED_BY_REL,
	ERR_EMPTY_LOCAL_ATTRIB_COLLATION,
	ERR_OBJECT_REFERENCING_ITSELF,
	ERR_ASG_INV_OPFAM_OPCLSELEM,
	ERR_REF_INV_LIKE_OP_TYPE,
	ERR_COPY_REL_TAB_DEFINED,
	ERR_TABLE_TRIG_INSTEADOF_FIRING,
	ERR_TRIGGER_INV_TRUNCATE_USAGE,
	ERR_TRIGGER_INV_INSTEADOF_USAGE,
	ERR_CONST_TRIG_NOT_AFTER_ROW,
	ERR_VIEW_TRIG_INV_AFTBFR_USAGE,
	ERR_TRIGGER_INV_INSTEADOF_UPDATE,
	ERR_ASG_COLUMN_NO_PARENT,
	ERR_TRIG_USING_CONSTRIG_ATRIBS,
	ERR_REF_FUNCTION_INV_TYPE_CONF,
	ERR_ASG_INV_OPCLASS_OBJ,
	ERR_INV_POSTGRESQL_VERSION,
	ERR_VALIDATION_FAILURE,
	ERR_REG_EXT_NOT_HANDLING_TYPE,
	ERR_ALOC_INV_FK_RELATIONSHIP,
	ERR_ASG_INV_NAME_PATTERN,
	ERR_REF_INV_NAME_PATTERN_ID
};

class Exception {
	private:
		/*! \brief Stores other exceptions before raise the 'this' exception.
		 This structure can be used to simulate a stack trace to improve the debug */
		vector<Exception> exceptions;

		//! \brief Stores the error messages and codes (names of errors) in string format
		static QString messages[ERROR_COUNT][2];

		//! \brief Constants used to access the error details
		static constexpr unsigned ERROR_CODE=0,
															ERROR_MESSAGE=1;

		//! \brief Error type related to the exception
		ErrorType error_type;

		//! \brief Formated error message
		QString error_msg,
						/*! \brief Holds the class name and method which was
						triggered the exception. For this to be possible, at the time
						instantiation of this class the  G++ macro __ PRETTY_FUNCTION__
						must be passed. This macro contains the format [RETURN][CLASS]::[METHOD][PARAMS] */
						method,

						//! \brief File where the exception was generated (Macro __ FILE__)
						file,

						/*! \brief Additional information (optional) may store any other
						type of information that is interesting on attempt to resolve the error */
						extra_info;

		//! \brief Line of file where the exception were generated (Macro __LINE__)
		int line;

		//! \brief Configures the basic attributes of exception
		void configureException(const QString &msg, ErrorType error_type, const QString &method, const QString &file, int line, const QString &extra_info);

		//! \brief Adds a exception to the list of exceptions
		void addException(Exception &exception);

	public:
		Exception(void);
		Exception(const QString &msg, const QString &method, const QString &file, int line, Exception *exception=nullptr, const QString &extra_info="");
		Exception(const QString &msg, ErrorType error_type, const QString &method, const QString &file, int line, Exception *exception=nullptr, const QString &extra_info="");
		Exception(ErrorType error_type, const QString &method, const QString &file, int line, Exception *exception=nullptr, const QString &extra_info="");
		Exception(ErrorType error_type, const QString &method, const QString &file, int line, vector<Exception> &exceptions, const QString &extra_info="");
		Exception(const QString &msg, const QString &method, const QString &file, int line, vector<Exception> &exceptions, const QString &extra_info="");
		~Exception(void){}
		QString getErrorMessage(void);
		static QString getErrorMessage(ErrorType error_type);
		static QString getErrorCode(ErrorType error_type);
		QString getMethod(void);
		QString getFile(void);
		QString getLine(void);
		ErrorType getErrorType(void);
		QString getExtraInfo(void);

		//! \brief Gets the full exception stack
		void getExceptionsList(vector<Exception> &list);

		//! \brief Gets the exception stack in a formatted text
		QString getExceptionsText(void);
};

#endif
