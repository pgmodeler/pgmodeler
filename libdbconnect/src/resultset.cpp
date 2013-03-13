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

#include "resultset.h"

ResultSet::ResultSet(void)
{
	sql_result=NULL;
	empty_result=false;
	is_res_copied=false;
	current_tuple=-1;
}

ResultSet::ResultSet(PGresult *sql_result)
{
	QString str_aux;
	int res_state;

	if(!sql_result)
		throw Exception(ERR_ASG_SQL_RESULT_NOT_ALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	this->sql_result=sql_result;
	res_state=PQresultStatus(this->sql_result);

	//Handling the status of the result
	switch(res_state)
	{
		//Generating an error in case the server returns an incomprehensible response
		case PGRES_BAD_RESPONSE:
			throw Exception(ERR_INCOMPREHENSIBLE_DBMS_RESP, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		break;

			//Generating an error in case the server returns a fatal error
		case PGRES_FATAL_ERROR:
			str_aux=QString(Exception::getErrorMessage(ERR_DBMS_FATAL_ERROR))
							.arg(PQresultErrorMessage(sql_result));
			throw Exception(str_aux,ERR_DBMS_FATAL_ERROR, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		break;

			//Generating an error in case the user tries to get a result from an empty query
		case PGRES_EMPTY_QUERY:
			throw Exception(ERR_EMPTY_SQL_COMMAND, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		break;

			//In case of sucess states the result will be created
		case PGRES_COMMAND_OK:
		case PGRES_TUPLES_OK:
		case PGRES_COPY_OUT:
		case PGRES_COPY_IN:
		default:
			empty_result=(res_state!=PGRES_TUPLES_OK);
			current_tuple=0;
			is_res_copied=false;
		break;
	}
}

ResultSet::~ResultSet(void)
{
	destroyResultSet();
}

void ResultSet::destroyResultSet(void)
{
	/* Destroy the resultset of the object if it was not copied
		to another class instance (see 'operator =') */
	if(sql_result && !is_res_copied)
		PQclear(sql_result);

	//Reset the other attributes
	sql_result=NULL;
	empty_result=false;
	is_res_copied=false;
	current_tuple=-1;
}

QString ResultSet::getColumnName(int column_idx)
{
	//Throws an error in case the column index is invalid
	if(column_idx < 0 || column_idx >= getColumnCount())
		throw Exception(ERR_REF_TUPLE_COL_INV_INDEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	//Returns the column name on the specified index
	return(QString(PQfname(sql_result, column_idx)));
}

int ResultSet::getColumnIndex(const QString &column_name)
{
	int col_idx=-1;

	//Get the column index using it's name
	col_idx=PQfnumber(sql_result, column_name.toStdString().c_str());

	/* In case the index is negative indicates that the column doesn't exists in the tuple
		thus an error will be raised */
	if(col_idx < 0)
		throw Exception(ERR_REF_TUPLE_COL_INV_NAME, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	return(col_idx);
}

char *ResultSet::getColumnValue(const QString &column_name)
{
	int col_idx=-1;

	try
	{
		/* Raises an error if the user try to get the value of a column in
		 a tuple of an empty result or generated from an INSERT, DELETE, UPDATE,
		 that is, which command do not return lines but only do updates or removal */
		if(getTupleCount()==0 || empty_result)
			throw Exception(ERR_REF_TUPLANAOEXISTE, __PRETTY_FUNCTION__, __FILE__, __LINE__);

		//Get the column index through its name
		col_idx=getColumnIndex(column_name);
	}
	catch(Exception &e)
	{
		//Capture and redirect any generated exception
		throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}

	//Returns the column value on the current tuple
	return(PQgetvalue(sql_result, current_tuple, col_idx));
}

char *ResultSet::getColumnValue(int column_idx)
{
	//Raise an error in case the column index is invalid
	if(column_idx < 0 || column_idx >= getColumnCount())
		throw Exception(ERR_REF_TUPLE_COL_INV_INDEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	/* Raises an error if the user try to get the value of a column in
		a tuple of an empty result or generated from an INSERT, DELETE, UPDATE,
		that is, which command do not return lines but only do updates or removal */
	else if(getTupleCount()==0 || empty_result)
		throw Exception(ERR_REF_TUPLANAOEXISTE, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	//Returns the column value on the current tuple
	return(PQgetvalue(sql_result, current_tuple, column_idx));
}

int ResultSet::getColumnSize(const QString &column_name)
{
	int col_idx=-1;

	try
	{
		//Get the column index wich length must be detected
		col_idx=getColumnIndex(column_name);
	}
	catch(Exception &e)
	{
		//Capture and redirect any generated exception
		throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}

	//Returns the column value length on the current tuple
	return(PQgetlength(sql_result, current_tuple, col_idx));
}

int ResultSet::getColumnSize(int column_idx)
{
	//Raise an error in case the column index is invalid
	if(column_idx < 0 || column_idx >= getColumnCount())
		throw Exception(ERR_REF_TUPLE_COL_INV_INDEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	//Retorns the column value length on the current tuple
	return(PQgetlength(sql_result, current_tuple, column_idx));
}

int ResultSet::getTupleCount(void)
{
	//In case the result has some tuples
	if(!empty_result)
		//Returns the tuple count gathered after the SQL command
		return(PQntuples(sql_result));
	else
		/* Returns the line amount that were affected by the SQL command
		 (only for INSERT, DELETE, UPDATE) */
		return(atoi(PQcmdTuples(sql_result)));
}

int ResultSet::getColumnCount(void)
{
	return(PQnfields(sql_result));
}

int ResultSet::getCurrentTuple(void)
{
	return(current_tuple);
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
		throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}

	/* Returns the column format in the current tuple.
		According to libpq documentation, value = 0, indicates column text format,
		value = 1 the column has binary format, the other values are reserved */
	return(PQfformat(sql_result, col_idx)==1);
}

bool ResultSet::isColumnBinaryFormat(int column_idx)
{
	//Raise an error in case the column index is invalid
	if(column_idx < 0 || column_idx >= getColumnCount())
		throw Exception(ERR_REF_TUPLE_COL_INV_INDEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	/* Returns the column format in the current tuple.
		According to libpq documentation, value = 0, indicates column text format,
		value = 1 the column has binary format, the other values are reserved */
	return(PQfformat(sql_result, column_idx)==1);
}

bool ResultSet::accessTuple(unsigned tuple_type)
{
	int tuple_count=getTupleCount();
	bool accessed=true;

	/* Raises an error if the result does not have any lines or
		is derived from a command which affects only rows or
		The tuple type to be accessed is invalid, out of
		set defined by the class */
	if(tuple_count==0 || empty_result || tuple_type > NEXT_TUPLE)
		throw Exception(ERR_REF_TUPLANAOEXISTE, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	switch(tuple_type)
	{
		case FIRST_TUPLE:
			current_tuple=0;
		break;

		case LAST_TUPLE:
			current_tuple=tuple_count-1;
		break;

		case PREVIOUS_TUPLE:
			accessed=(current_tuple > 0);
			if(accessed) current_tuple--;
		break;

		case NEXT_TUPLE:
			accessed=(current_tuple < (tuple_count-1));
			if(accessed) current_tuple++;
		break;
	}

	return(accessed);
}

void ResultSet::operator = (ResultSet &res)
{
	/* Mark the result parameter as copied, avoiding
		the sql_result attribute to be deallocated */
	res.is_res_copied=true;

	/* If the resultset 'this' is allocated,
		it will be deallocated to avoid memory leaks */
	destroyResultSet();

	//Copy the parameter restulset attributes to 'this' resultset
	this->current_tuple=res.current_tuple;
	this->empty_result=res.empty_result;
	this->sql_result=res.sql_result;
	this->is_res_copied=false;
}

