/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "doublenan.h"
#include <exception>
#include <signal.h>
#include <vector>
#include <deque>
#include <type_traits>

using namespace std;

//! \brief This function causes the provided enum to be converted to its underlying datatype
template<typename Enum>
constexpr std::underlying_type_t<Enum> enum_cast (Enum obj_type) noexcept
{
	return static_cast<typename std::underlying_type_t<Enum>>(obj_type);
}

//! \brief This enum defines the global error codes used throughout the application
enum class ErrorCode: unsigned {
	Custom,
	AsgPseudoTypeColumn,
	AsgInvalidPrecision,
	AsgInvalidPrecisionTimestamp,
	AsgNotAllocatedColumn,
	RefColumnInvalidIndex,
	AsgNotAllocattedObject,
	AsgNotAllocatedSchema,
	AsgObjectInvalidDefinition,
	AsgDuplicatedObject,
	AsgDuplicatedObjectContainer,
	AsgObjectInvalidType,
	RemObjectInvalidType,
	ObtObjectInvalidType,
	AsgEmptyNameTableReturnType,
	AsgDuplicatedParameterFunction,
	InsDuplicatedTableReturnType,
	RefParameterInvalidIndex,
	RefInvalidTriggerEvent,
	AsgInvalidColumnTrigger,
	AsgNotAllocatedFunction,
	AsgInvalidTriggerFunction,
	AsgFunctionInvalidParamCount,
	AsgFunctionInvalidLanguage,
	AsgEventTriggerFuncInvalidLang,
	AsgNotAllocatedTable,
	RefArgumentInvalidIndex,
	AsgEmptyNameObject,
	AsgInvalidNameObject,
	AsgLongNameObject,
	AsgInvalidSchemaObject,
	AsgInvalidTablespaceObject,
	AsgTablespaceInvalidObject,
	AsgTablespaceInvalidConstraintType,
	AsgInvalidRoleObject,
	AsgRoleObjectInvalidType,
	AsgCustomSQLObjectInvalidType,
	RefFunctionInvalidType,
	RefOperatorArgumentInvalidType,
	RefOperatorInvalidType,
	AsgValueInvalidRoleOptionType,
	RefInvalidRoleType,
	InsDuplicatedRole,
	AsgRoleReferenceRedundancy,
	AsgRoleMemberItself,
	RefRoleInvalidIndex,
	InsEmptyRuleCommand,
	RefRuleCommandInvalidIndex,
	InvInheritCopyPartRelationship,
	AsgObjectBelongsAnotherTable,
	AsgSchemaSequenceDiffersTableSchema,
	AsgInvalidValueSequenceAttributes,
	AsgInvalidSequenceMinValue,
	AsgInvalidSequenceStartValue,
	AsgInvalidSequenceIncrementValue,
	AsgInvalidSequenceCacheValue,
	AsgSeqOwnerTableDifferentSchema,
	AsgSeqOwnerTableDifferentRole,
	AsgInexistentSeqOwnerColumn,
	AsgInvalidSeqOwnerColumn,
	RefLabelInvalidIndex,
	AllocationObjectInvalidType,
	AsgFunctionInvalidReturnType,
	AsgFunctionInvalidParameters,
	AsgNotAllocatedLanguage,
	AsgInvalidLanguageObject,
	RefTypeInvalidIndex,
	AsgNullTypeObject,
	AsgInvalidTypeObject,
	AsgEmptyDirectoryName,
	ObtTypesInvalidQuantity,
	InsDuplicatedItems,
	InsInvalidTypeAttribute,
	InsDuplicatedEnumerationItem,
	InsInvalidEnumerationItem,
	RefAttributeInvalidIndex,
	RefEnumerationInvalidIndex,
	AsgInvalidTypeConfiguration,
	AsgInvalidOperatorArguments,
	AsgInvalidOperatorTypes,
	AsgReservedName,
	AsgFunctionInvalidConfiguration,
	AsgInvalidSupportStrategyNumber,
	InsDuplicatedElement,
	RefElementInvalidIndex,
	RefObjectInvalidIndex,
	RemNotAllocatedObject,
	RemDirectReference,
	RemInderectReference,
	OprObjectInvalidType,
	RefObjectInvalidType,
	OprNotAllocatedObject,
	InvLinkTablesNoPrimaryKey,
	NotImplementedRelationshipType,
	AsgInvalidExpressionObject,
	AsgExistingPrimaryKeyTable,
	InvIdentifierRelationship,
	InvCopyRelationshipDuplicCols,
	InvInheritRelationshipIncompCols,
	InvInheritRelationshipIncompConstrs,
	AsgObjectInvalidRelationshipType,
	AsgForeignKeyRelationship,
	RefObjectInexistsModel,
	RefUserTypeInexistsModel,
	AsgInvalidMaxSizeOpList,
	FileDirectoryNotWritten,
	FileNotWrittenInvalidDefinition,
	InsDuplicatedRelationship,
	InsRelationshipRedundancy,
	RemInvalidatedObjects,
	InvPrimaryKeyAllocation,
	RefInvalidPrivilegeType,
	InsDuplicatedRolePermission,
	AsgInvalidPrivilegeObject,
	AsgDuplicatedPermission,
	PermissionRefInexistObject,
	InvObjectAllocationNoSchema,
	AsgTablespaceDuplicatedDirectory,
	AsgInvalidSequenceTypeArray,
	AsgSourceCodeFuncCLanguage,
	AsgRefLibraryFuncLanguageNotC,
	AsgInvalidCommutatorOperator,
	AsgInvalidNegatorOperator,
	InvUserTypeSelfReference,
	AsgInvalidElementType,
	AsgInvalidAlignmentType,
	AsgInvalidNameTableRelNN,
	InvUseSpecialPrimaryKey,
	OprRelationshipAddedObject,
	RemProtectedObject,
	InvRedeclarationGroup,
	InvGroupDeclaration,
	DefNotDeclaredGroup,
	DefEmptyGroup,
	DefDuplicatedGroup,
	InvGroupDeclarationNotDefined,
	RefColObjectTabInvalidIndex,
	RefRowObjectTabInvalidIndex,
	OprReservedObject,
	InvFuncConfigInvalidatesObject,
	InvSQLScopeViewReference,
	InvConstratintNoColumns,
	ConfigurationNotLoaded,
	DefaultConfigNotRestored,
	ExportFailure,
	PluginNotLoaded,
	PluginsNotLoaded,
	InvalidSyntax,
	InvalidInstruction,
	UnkownAttribute,
	InvalidMetacharacter,
	InvalidOperatorInExpression,
	UndefinedAttributeValue,
	InvalidAttribute,
	AsgEmptyXMLBuffer,
	FileDirectoryNotAccessed,
	AsgEmptyDTDFile,
	AsgEmptyDTDName,
	LibXMLError,
	OprNotAllocatedElementTree,
	InvModelFileNotLoaded,
	OprNotAllocatedElement,
	OprInexistentElement,
	AsgInvalidConnParameter,
	OprNotAllocatedConnection,
	ConnectionNotConfigured,
	ConnectionAlreadyStablished,
	ConnectionNotStablished,
	AsgNotAllocatedSQLResult,
	IncomprehensibleDBMSResponse,
	DBMSFatalError,
	RefTupleColumnInvalidIndex,
	RefTupleColumnInvalidName,
	RefInvalidTuple,
	RefInvalidTupleColumn,
	SQLCommandNotExecuted,
	AsgInvalidViewDefExpression,
	AsgSecondViewDefExpression,
	MixingViewDefExprsReferences,
	AsgInvalidCollationObject,
	UnsupportedPKColsAddedByRel,
	EmptyLCCollationAttributes,
	ObjectReferencingItself,
	AsgInvalidOpFamilyOpClassElem,
	RefInvalidLikeOptionType,
	InvCopyRelTableDefined,
	InvPartRelPartitionedDefined,
	InvRelTypeForPatitionTables,
	InvTableTriggerInsteadOfFiring,
	InvUsageTruncateOnTrigger,
	InvUsageInsteadOfOnTrigger,
	InvConstrTriggerNotAfterRow,
	InvUsageAfterBeforeViewTrigger,
	InvUsageInsteadOfUpdateTrigger,
	AsgColumnNoParent,
	InvUseConstraintTriggerAttribs,
	RefInvalidFunctionIdTypeConfig,
	AsgInvalidOpClassObject,
	InvPostgreSQLVersion,
	ValidationFailure,
	ExtensionHandlingTypeImmutable,
	InvAllocationFKRelationship,
	AsgInvalidNamePattern,
	RefInvalidNamePatternId,
	InvUsageVariadicParamMode,
	MixingIncompExportOptions,
	MixingIncompDropOptions,
	InvIdSwapSameObject,
	InvIdSwapInvalidObjectType,
	AsgWidgetAlreadyHasParent,
	ObjectNotImported,
	ModelFileNotLoaded,
	InvColumnTableType,
	OprInvalidElementId,
	RefInvalidElementColorId,
	AsgInvalidObjectType,
	IncompColumnTypeForSequence,
	InvUsageTempNamesExportOption,
	InvConversionIntegerToSerial,
	AsgInvalidEventTriggerVariable,
	RowDataNotManipulated,
	MalformedUnescapedValue,
	UndoRedoOperationInvalidObject,
	RequiredFieldsNotFilled,
	InvRelationshipIdSwap,
	InvInheritParentTableNotFound,
	AsgEnumInvalidChars,
	AsgEnumLongName,
	ConnectionTimeout,
	ConnectionBroken,
	DropCurrentDBDefault,
	NullPrimaryKeyColumn,
	InvalidIdentityColumn,
	RefInvalidAffectedCommand,
	RefInvalidSpecialRole,
	InvColumnCountPartRel,
	InvPartitioningTypePartRel,
	InvPartitionKeyCount,
	PartKeyObjectInexistsModel,
	AsgInvalidColumnPartitionKey,
	RemColumnRefByPartitionKey,
	AsgOptionInvalidName,
	AsgInvalidNameObjReference,
	AsgNotAllocatedObjectReference,
	InsDuplicatedObjectReference,
	ModelFileInvalidSize,
	AsgInvalidObjectForeignTable,
	InvRelTypeForeignTable,
	InvCopyRelForeignTable,
	InvDataDictDirectory,
	InitialUserConfigNotCreated,
	InvalidObjectFilter,
	InvChildObjectTypeFilter
};

class Exception {
	private:
		static constexpr unsigned ErrorCount=253;

		/*! \brief Stores other exceptions before raise the 'this' exception.
		 This structure can be used to simulate a stack trace to improve the debug */
		vector<Exception> exceptions;

		//! \brief Stores the error messages and codes (names of errors) in string format
		static QString messages[ErrorCount][2];

		//! \brief Constants used to access the error details
		static constexpr unsigned ErrorCodeId=0, ErrorMessage=1;

		//! \brief Error type related to the exception
		ErrorCode error_code;

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
		void configureException(const QString &msg, ErrorCode error_code, const QString &method, const QString &file, int line, const QString &extra_info);

		//! \brief Adds a exception to the list of exceptions
		void addException(Exception &exception);

	public:
		static unsigned constexpr MaximumStackSize = 50;

		Exception();
		Exception(const QString &msg, const QString &method, const QString &file, int line, Exception *exception=nullptr, const QString &extra_info=QString());
		Exception(const QString &msg, const QString &method, const QString &file, int line, vector<Exception> &exceptions, const QString &extra_info=QString());
		Exception(const QString &msg, ErrorCode error_code, const QString &method, const QString &file, int line, Exception *exception=nullptr, const QString &extra_info=QString());
		Exception(const QString &msg, ErrorCode error_code, const QString &method, const QString &file, int line, vector<Exception> &exceptions, const QString &extra_info=QString());
		Exception(ErrorCode error_code, const QString &method, const QString &file, int line, Exception *exception=nullptr, const QString &extra_info=QString());
		Exception(ErrorCode error_code, const QString &method, const QString &file, int line, vector<Exception> &exceptions, const QString &extra_info=QString());

		~Exception(void){}
		QString getErrorMessage();
		static QString getErrorMessage(ErrorCode error_code);
		static QString getErrorCode(ErrorCode error_code);
		QString getMethod();
		QString getFile();
		QString getLine();
		ErrorCode getErrorCode();
		QString getExtraInfo();

		//! \brief Gets the full exception stack
		void getExceptionsList(vector<Exception> &list);

		//! \brief Gets the exception stack in a formatted text
		QString getExceptionsText();
};

#endif
