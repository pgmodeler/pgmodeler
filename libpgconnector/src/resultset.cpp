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

#include "resultset.h"

ResultSet::ResultSet()
{
	sql_result=nullptr;
	empty_result=false;
	is_res_copied=false;
	current_tuple=-1;
}

ResultSet::ResultSet(PGresult *sql_result)
{
	QString str_aux;
	int res_state;

	if(!sql_result)
		throw Exception(ErrorCode::AsgNotAllocatedSQLResult, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	this->sql_result=sql_result;
	res_state=PQresultStatus(this->sql_result);

	//Handling the status of the result
	switch(res_state)
	{
		//Generating an error in case the server returns an incomprehensible response
		case PGRES_BAD_RESPONSE:
			throw Exception(ErrorCode::IncomprehensibleDBMSResponse, __PRETTY_FUNCTION__, __FILE__, __LINE__);

			//Generating an error in case the server returns a fatal error
		case PGRES_FATAL_ERROR:
			str_aux=Exception::getErrorMessage(ErrorCode::DBMSFatalError)
					.arg(PQresultErrorMessage(sql_result));
			throw Exception(str_aux,ErrorCode::DBMSFatalError, __PRETTY_FUNCTION__, __FILE__, __LINE__);

			//In case of sucess states the result will be created
		case PGRES_COMMAND_OK:
		case PGRES_TUPLES_OK:
		case PGRES_SINGLE_TUPLE:
		case PGRES_COPY_OUT:
		case PGRES_COPY_IN:
		default:
			empty_result=(res_state!=PGRES_TUPLES_OK && res_state!=PGRES_SINGLE_TUPLE && res_state!=PGRES_EMPTY_QUERY);
			current_tuple=-1;
			is_res_copied=false;
		break;
	}
}

ResultSet::~ResultSet()
{
	clearResultSet();
}

void ResultSet::clearResultSet()
{
	/* Destroy the resultset of the object if it was not copied
		to another class instance (see 'operator =') */
	if(sql_result && !is_res_copied)
		PQclear(sql_result);

	//Reset the other attributes
	sql_result=nullptr;
	empty_result=false;
	is_res_copied=false;
	current_tuple=-1;
}

QString ResultSet::getColumnName(int column_idx)
{
	//Throws an error in case the column index is invalid
	if(column_idx < 0 || column_idx >= getColumnCount())
		throw Exception(ErrorCode::RefTupleColumnInvalidIndex, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	//Returns the column name on the specified index
	return QString(PQfname(sql_result, column_idx));
}

unsigned ResultSet::getColumnTypeId(int column_idx)
{
	//Throws an error in case the column index is invalid
	if(column_idx < 0 || column_idx >= getColumnCount())
		throw Exception(ErrorCode::RefTupleColumnInvalidIndex, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	//Returns the column type id on the specified index
	return static_cast<unsigned>(PQftype(sql_result, column_idx));
}

int ResultSet::getColumnIndex(const QString &column_name)
{
	int col_idx=-1;

	//Get the column index using it's name
	col_idx=PQfnumber(sql_result, column_name.toStdString().c_str());

	/* In case the index is negative indicates that the column doesn't exists in the tuple
		thus an error will be raised */
	if(col_idx < 0)
		throw Exception(ErrorCode::RefTupleColumnInvalidName, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	return col_idx;
}

int ResultSet::validateColumnName(const QString &column_name)
{
	try
	{
		/* Raises an error if the user try to get the value of a column in
		 a tuple of an empty result or generated from an INSERT, DELETE, UPDATE,
		 that is, which command do not return lines but only do updates or removal */
		if(getTupleCount()==0 || empty_result)
			throw Exception(ErrorCode::RefInvalidTuple, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		else if(current_tuple < 0 || current_tuple >= getTupleCount())
			throw Exception(ErrorCode::RefInvalidTupleColumn, __PRETTY_FUNCTION__, __FILE__, __LINE__);

		//Get the column index through its name
		return getColumnIndex(column_name);
	}
	catch(Exception &e)
	{
		//Capture and redirect any generated exception
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

char *ResultSet::getColumnValue(const QString &column_name)
{
	//Returns the column value on the current tuple
	return PQgetvalue(sql_result, current_tuple, validateColumnName(column_name));
}

void ResultSet::validateColumnIndex(int column_idx)
{
	//Raise an error in case the column index is invalid
	if(column_idx < 0 || column_idx >= getColumnCount())
		throw Exception(ErrorCode::RefTupleColumnInvalidIndex, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	/* Raises an error if the user try to get the value of a column in
		a tuple of an empty result or generated from an INSERT, DELETE, UPDATE,
		that is, which command do not return lines but only do updates or removal */
	else if(getTupleCount()==0 || empty_result)
		throw Exception(ErrorCode::RefInvalidTuple, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	else if(current_tuple < 0 || current_tuple >= getTupleCount())
		throw Exception(ErrorCode::RefInvalidTupleColumn, __PRETTY_FUNCTION__, __FILE__, __LINE__);
}

char *ResultSet::getColumnValue(int column_idx)
{
	validateColumnIndex(column_idx);

	//Returns the column value on the current tuple
	return PQgetvalue(sql_result, current_tuple, column_idx);
}

bool ResultSet::isColumnValueNull(int column_idx)
{
	validateColumnIndex(column_idx);

	//Returns the null state of the column on the current tuple
	return PQgetisnull(sql_result, current_tuple, column_idx);
}

bool ResultSet::isColumnValueNull(const QString &column_name)
{
	//Returns the null state of the column on the current tuple
	return PQgetisnull(sql_result, current_tuple, validateColumnName(column_name));
}

int ResultSet::getColumnSize(const QString &column_name)
{
	//Returns the column value length on the current tuple
	return PQgetlength(sql_result, current_tuple, validateColumnName(column_name));
}

int ResultSet::getColumnSize(int column_idx)
{
	//Raise an error in case the column index is invalid
	if(column_idx < 0 || column_idx >= getColumnCount())
		throw Exception(ErrorCode::RefTupleColumnInvalidIndex, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	else if(current_tuple < 0 || current_tuple >= getTupleCount())
		throw Exception(ErrorCode::RefInvalidTupleColumn, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	//Retorns the column value length on the current tuple
	return PQgetlength(sql_result, current_tuple, column_idx);
}

attribs_map ResultSet::getTupleValues()
{
	attribs_map tup_vals;

	if(current_tuple < 0 || current_tuple >= getTupleCount())
		throw Exception(ErrorCode::RefInvalidTuple, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	for(int col=0; col < getColumnCount(); col++)
		tup_vals[getColumnName(col)]=getColumnValue(col);

	return tup_vals;
}

int ResultSet::getTupleCount()
{
	//In case the result has some tuples
	if(!empty_result)
		//Returns the tuple count gathered after the SQL command
		return PQntuples(sql_result);
	else
		/* Returns the line amount that were affected by the SQL command
		 (only for INSERT, DELETE, UPDATE) */
		return atoi(PQcmdTuples(sql_result));
}

int ResultSet::getColumnCount()
{
	return PQnfields(sql_result);
}

int ResultSet::getCurrentTuple()
{
	return current_tuple;
}

bool ResultSet::isColumnBinaryFormat(const QString &column_name)
{
	int col_idx=-1;

	try
	{
		col_idx=getColumnIndex(column_name);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}

	/* Returns the column format in the current tuple.
		According to libpq documentation, value = 0, indicates column text format,
		value = 1 the column has binary format, the other values are reserved */
	return (PQfformat(sql_result, col_idx) == 1);
}

bool ResultSet::isColumnBinaryFormat(int column_idx)
{
	//Raise an error in case the column index is invalid
	if(column_idx < 0 || column_idx >= getColumnCount())
		throw Exception(ErrorCode::RefTupleColumnInvalidIndex, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	/* Returns the column format in the current tuple.
		According to libpq documentation, value = 0, indicates column text format,
	value = 1 the column has binary format, the other values are reserved.

	One additional check is made, if the type of the column is bytea. */
	return (PQfformat(sql_result, column_idx)==1 || PQftype(sql_result, column_idx)==BYTEAOID);
}

bool ResultSet::accessTuple(unsigned tuple_type)
{
	int tuple_count=getTupleCount();

	/* Raises an error if the result
		is derived from a command which affects only rows or
		The tuple type to be accessed is invalid, out of
		set defined by the class */
	if(empty_result || tuple_type > NextTuple)
		throw Exception(ErrorCode::RefInvalidTuple, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	if(tuple_count==0)
		return false;
	else
	{
		bool accessed=true;

		switch(tuple_type)
		{
			case FirstTuple:
				current_tuple=0;
			break;

			case LastTuple:
				current_tuple=tuple_count-1;
			break;

			case PreviousTuple:
				accessed=(current_tuple > 0);
				if(accessed) current_tuple--;
			break;

			case NextTuple:
				accessed=(current_tuple < (tuple_count-1));
				if(accessed) current_tuple++;
			break;
		}

		return accessed;
	}
}

bool ResultSet::isEmpty()
{
	return empty_result;
}

bool ResultSet::isValid()
{
	return (sql_result != nullptr);
}

void ResultSet::operator = (ResultSet &res)
{
	/* Mark the result parameter as copied, avoiding
		the sql_result attribute to be deallocated */
	res.is_res_copied=true;

	/* If the resultset 'this' is allocated,
		it will be deallocated to avoid memory leaks */
	clearResultSet();

	//Copy the parameter restulset attributes to 'this' resultset
	this->current_tuple=res.current_tuple;
	this->empty_result=res.empty_result;
	this->sql_result=PQcopyResult(res.sql_result, PG_COPYRES_TUPLES | PG_COPYRES_ATTRS | PG_COPYRES_EVENTS);
	this->is_res_copied=false;
}

