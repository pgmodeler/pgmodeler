#include "python_daos_generator.h"
#include "pgmodelerplugin.h"
#include <string>

void PythonDAOsGenerator::generateCode(DatabaseModel &model, BaseLogger &logger, std::map< std::string, std::string > &files_to_generate)
{
    // loop all tables
    std::vector< BaseObject * > * tables = model.getObjectList(OBJ_TABLE);
    std::vector< BaseObject * >::iterator it;
    std::string schema_name;
    std::string table_name;
    std::string table_code;

    std::stringstream text;

    for(it = tables->begin(); it != tables->end(); ++it)
    {
        schema_name = (*it)->getSchema()->getName().toUtf8().constData();
        table_name = (*it)->getName().toUtf8().constData();
        table_code = this->generateCompleteTableCode(*(Table *)(* it));
        files_to_generate[schema_name + "_" + table_name + ".py"] = table_code;
        text << table_code;
    }

    logger.log(text.str());
}

// Starting code generation
std::string PythonDAOsGenerator::generateCompleteTableCode(Table &table)
{
    std::stringstream text;

    // Header:
    this->generateHeader(text, table);

    // Insert
    this->generateInsertAutoPK(text, table);
    this->generateInsertAll(text, table);

    // Update
    this->generateUpdateFromPK(text, table);

    // Delete
    this->generateDeleteFromPK(text, table);

    // Select
    this->generateSelectFromPK(text, table);

    // Select
    this->generateSelectFromUKs(text, table);

    // end newline
    text << '\n';

    return text.str();
}

void PythonDAOsGenerator::generateHeader(std::stringstream &text, Table &table)
{
    // schema and table name
    std::string schema_name = table.getSchema()->getName().toUtf8().constData();
    std::string table_name = table.getName().toUtf8().constData();

    // DAO class name
    std::string dao_class_name = this->getDAONameFromSchemaTableName(schema_name, table_name);

    text << "#!/usr/bin/env python";
    text << "\n# -*- coding: utf-8 -*-";
    text << "\n\"\"\"";
    text << "\n@package " << dao_class_name << " Data Access Object";
    text << "\n         for '" << schema_name << '.' << table_name << "' table";
    text << "\n\"\"\"";
    text << '\n';
    text << '\n';
    text << "\nclass " << dao_class_name << "():";
    text << '\n';
    text << "\n    def __init__(self, database):";
    text << "\n        self.db = database";
}

void PythonDAOsGenerator::generateInsertAutoPK(std::stringstream &text, Table &table)
{
    this->generateInsert(text, table, false);
}

void PythonDAOsGenerator::generateInsertAll(std::stringstream &text, Table &table)
{
    this->generateInsert(text, table, true);
}

void PythonDAOsGenerator::generateInsert(std::stringstream &text, Table &table, bool all)
{
    // schema and table name
    std::string schema_name = table.getSchema()->getName().toUtf8().constData();
    std::string table_name = table.getName().toUtf8().constData();
    std::string schema_table_name = schema_name + '.' + table_name;

    // get table columns and iterator to loop under all table columns
    std::vector< Column * > table_columns = this->getTableColumns(table);
    std::vector< Column * >::iterator table_columns_it;

    // get primary key columns and iterator
    std::vector< Column * > pk_columns = this->getTablePrimaryKeyColumns(table);
    std::vector< Column * >::iterator pk_columns_it;

    std::string method_name = "insert";
    std::vector< Column * > pk_auto_columns;
    std::vector< Column * >::iterator pk_auto_columns_it;

    if(all)
    {
        method_name += "All";
    }
    else
    {
        // get primary key columns that have a sequence
        pk_auto_columns = this->getTablePrimaryKeyAutoColumns(table);
    }

    // in the case that there are no columns to insert
    if(table_columns.size() - pk_auto_columns.size() <= 0)
    {
        return;
    }

    // auxiliary vars
    Column *column;
    std::string col_name;
    bool is_auto_pk;
    bool is_first_column_writed;

    // funcion definition and parameters
    text << '\n';
    text << "\n    def " << method_name << "(";
    text << "\n        self";
    is_first_column_writed = true; // already write self above
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        is_auto_pk = std::find(pk_auto_columns.begin(), pk_auto_columns.end(), *table_columns_it) != pk_auto_columns.end();
        if(! is_auto_pk)
        {
            column = *table_columns_it;
            col_name = column->getName().toUtf8().constData();
            if(!is_first_column_writed)
            {
                text << '\n';
                is_first_column_writed = true;
            }
            else
            {
                text << ",\n";
            }
            text << "        " << col_name;
        }
    }
    text << "\n    ):";

    // commentary
    text << "\n        \"\"\" Insert a '" << schema_table_name << "' record";
    text << "\n            and get back the DB generated ID \"\"\"";
    text << '\n';

    // query arguments
    text << "\n        args = {";
    is_first_column_writed = false;
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        is_auto_pk = std::find(pk_auto_columns.begin(), pk_auto_columns.end(), *table_columns_it) != pk_auto_columns.end();
        if(! is_auto_pk)
        {
            column = *table_columns_it;
            col_name = column->getName().toUtf8().constData();
            if(!is_first_column_writed)
            {
                text << '\n';
                is_first_column_writed = true;
            }
            else
            {
                text << ",\n";
            }
            text << "            \"" << col_name << "\": " << col_name;
        }
    }
    text << "\n        }";
    text << '\n';

    //TODO [CMP] put \n at the start of the line

    // sql query
    text << "\n        sql = (";
    text << "\n            \"INSERT INTO " << schema_table_name << " (";
    is_first_column_writed = false;
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        is_auto_pk = std::find(pk_auto_columns.begin(), pk_auto_columns.end(), *table_columns_it) != pk_auto_columns.end();
        if(! is_auto_pk)
        {
            column = *table_columns_it;
            col_name = column->getName().toUtf8().constData();
            if(!is_first_column_writed)
            {
                text << "\"\n";
                is_first_column_writed = true;
            }
            else
            {
                text << ", \"\n";
            }
            text << "            \"" << col_name;
        }
    }
    text << '\"';
    text << "\n            \") VALUES (";
    is_first_column_writed = false;
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        is_auto_pk = std::find(pk_auto_columns.begin(), pk_auto_columns.end(), *table_columns_it) != pk_auto_columns.end();
        if(! is_auto_pk)
        {
            column = *table_columns_it;
            col_name = column->getName().toUtf8().constData();
            if(!is_first_column_writed)
            {
                text << "\"\n";
                is_first_column_writed = true;
            }
            else
            {
                text << ", \"\n";
            }
            text << "            \"%(" << col_name << ")s";
        }
    }
    text << '\"';
    text << "\n            \")";
    if(!pk_columns.empty())
    {
        text << " RETURNING ";
        for(pk_columns_it = pk_columns.begin(); pk_columns_it != pk_columns.end(); ++pk_columns_it)
        {
            column = *pk_columns_it;
            col_name = column->getName().toUtf8().constData();
            text << col_name;
            if(std::next(pk_columns_it, 1) != pk_columns.end()) text << ", ";
        }
    }
    text << '\"';
    text << "\n        )";
    text << '\n';
    // execution
    text << "\n        try:";
    text << "\n            result = self.db.execute(sql, args)";
    if(!pk_columns.empty())
    {
        text << "\n            return dict(result.fetchone())";
    }
    else
    {
        text << "\n            result.close()";
        text << "\n            return dict()";
    }
    text << "\n        except Exception, error:";
    text << "\n            self.db.logger.error(error)";
    text << "\n            raise";
}

void PythonDAOsGenerator::generateUpdateFromPK(std::stringstream &text, Table &table)
{
    // schema and table name
    std::string schema_name = table.getSchema()->getName().toUtf8().constData();
    std::string table_name = table.getName().toUtf8().constData();
    std::string schema_table_name = schema_name + '.' + table_name;

    // get table columns and iterator to loop under all table columns
    std::vector< Column * > table_columns = this->getTableColumns(table);
    std::vector< Column * >::iterator table_columns_it;

    // get primary key columns and iterator
    std::vector< Column * > pk_columns = this->getTablePrimaryKeyColumns(table);
    std::vector< Column * >::iterator pk_columns_it;

    if(pk_columns.empty())
    {
        return; // will be no update from pk
    }

    std::string pk_names = this->getKeyNamesFromColumns(pk_columns);

    // auxiliary vars
    Column *column;
    std::string col_name;
    bool is_pk;

    // funcion definition and parameters
    text << '\n';
    text << "\n    def update_by_" << pk_names << "(";
    text << "\n        self,";
    // pk first
    for(pk_columns_it = pk_columns.begin(); pk_columns_it != pk_columns.end(); ++pk_columns_it)
    {
        column = *pk_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n        " << col_name;
        if(!table_columns.empty() || std::next(pk_columns_it, 1) != pk_columns.end()) text << ',';
    }
    // then the rest
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        is_pk = std::find(pk_columns.begin(), pk_columns.end(), *table_columns_it) != pk_columns.end();
        if(! is_pk)
        {
            column = *table_columns_it;
            col_name = column->getName().toUtf8().constData();
            text << "\n        " << col_name;
            if(std::next(table_columns_it, 1) != table_columns.end()) text << ',';
        }
    }
    text << "\n    ):";

    // commentary
    text << "\n        \"\"\" Update a '" << schema_table_name << "' record";
    text << "\n            using primary key \"\"\"";
    text << '\n';

    // query arguments
    text << "\n        args = {";
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        column = *table_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n            \"" << col_name << "\": " << col_name;
        if(std::next(table_columns_it, 1) != table_columns.end()) text << ',';
    }
    text << "\n        }";
    text << '\n';

    // sql query
    text << "\n        sql = (";
    text << "\n            \"UPDATE " << schema_table_name << " SET \"";
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        column = *table_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n            \"" << col_name << " = %(" << col_name << ")s";
        if(std::next(table_columns_it, 1) != table_columns.end()) text << ", ";
        text << '\"';
    }
    text << "\n            \" WHERE \"";
    for(pk_columns_it = pk_columns.begin(); pk_columns_it != pk_columns.end(); ++pk_columns_it)
    {
        column = *pk_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n            \"" << col_name << " = %(" << col_name << ")s";
        if(std::next(pk_columns_it, 1) != pk_columns.end()) text << " AND ";
        text << '\"';
    }
    text << "\n        )";
    text << '\n';
    // execution
    text << "\n        try:";
    text << "\n            result = self.db.execute(sql, args)";
    text << "\n            result.close()";
    text << "\n            return result.rowcount";
    text << "\n        except Exception, error:";
    text << "\n            self.db.logger.error(error)";
    text << "\n            raise";
}

void PythonDAOsGenerator::generateDeleteFromPK(std::stringstream &text, Table &table)
{
    // schema and table name
    std::string schema_name = table.getSchema()->getName().toUtf8().constData();
    std::string table_name = table.getName().toUtf8().constData();
    std::string schema_table_name = schema_name + '.' + table_name;

    // get primary key columns and iterator
    std::vector< Column * > pk_columns = this->getTablePrimaryKeyColumns(table);
    std::vector< Column * >::iterator pk_columns_it;

    if(pk_columns.empty())
    {
        return; // will be no delete from pk
    }

    std::string pk_names = this->getKeyNamesFromColumns(pk_columns);

    // auxiliary vars
    Column *column;
    std::string col_name;

    // funcion definition and parameters
    text << '\n';
    text << "\n    def delete_by_" << pk_names << "(";
    text << "\n        self,";
    // pk first
    for(pk_columns_it = pk_columns.begin(); pk_columns_it != pk_columns.end(); ++pk_columns_it)
    {
        column = *pk_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n        " << col_name;
        if(std::next(pk_columns_it, 1) != pk_columns.end()) text << ',';
    }
    text << "\n    ):";

    // commentary
    text << "\n        \"\"\" Delete a '" << schema_table_name << "' record";
    text << "\n            using primary key \"\"\"";
    text << '\n';

    // query arguments
    text << "\n        args = {";
    for(pk_columns_it = pk_columns.begin(); pk_columns_it != pk_columns.end(); ++pk_columns_it)
    {
        column = *pk_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n            \"" << col_name << "\": " << col_name;
        if(std::next(pk_columns_it, 1) != pk_columns.end()) text << ',';
    }
    text << "\n        }";
    text << '\n';

    // sql query
    text << "\n        sql = (";
    text << "\n            \"DELETE FROM " << schema_table_name << " WHERE \"";
    for(pk_columns_it = pk_columns.begin(); pk_columns_it != pk_columns.end(); ++pk_columns_it)
    {
        column = *pk_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n            \"" << col_name << " = %(" << col_name << ")s";
        if(std::next(pk_columns_it, 1) != pk_columns.end()) text << " AND ";
        text << '\"';
    }
    text << "\n        )";
    text << '\n';
    // execution
    text << "\n        try:";
    text << "\n            result = self.db.execute(sql, args)";
    text << "\n            result.close()";
    text << "\n            return result.rowcount";
    text << "\n        except Exception, error:";
    text << "\n            self.db.logger.error(error)";
    text << "\n            raise";
}

void PythonDAOsGenerator::generateSelectFromPK(std::stringstream &text, Table &table)
{
    // schema and table name
    std::string schema_name = table.getSchema()->getName().toUtf8().constData();
    std::string table_name = table.getName().toUtf8().constData();
    std::string schema_table_name = schema_name + '.' + table_name;

    // get table columns and iterator to loop under all table columns
    std::vector< Column * > table_columns = this->getTableColumns(table);

    // get primary key columns and iterator
    std::vector< Column * > pk_columns = this->getTablePrimaryKeyColumns(table);

    // generate for pk
    generateSelectFromConstraintColumns(text, schema_table_name, table_columns, pk_columns);
}

void PythonDAOsGenerator::generateSelectFromUKs(std::stringstream &text, Table &table)
{
    // schema and table name
    std::string schema_name = table.getSchema()->getName().toUtf8().constData();
    std::string table_name = table.getName().toUtf8().constData();
    std::string schema_table_name = schema_name + '.' + table_name;

    // get table columns and iterator to loop under all table columns
    std::vector< Column * > table_columns = this->getTableColumns(table);

    // get all unique keys
    std::vector< std::vector< Column * > > uk_keys = getTableUniqueKeyColumns(table);
    std::vector< std::vector< Column * > >::iterator uk_keys_it;

    // generate for all uk
    for(uk_keys_it = uk_keys.begin(); uk_keys_it != uk_keys.end(); ++uk_keys_it)
    {
        generateSelectFromConstraintColumns(text, schema_table_name, table_columns, *uk_keys_it);
    }
}

void PythonDAOsGenerator::generateSelectFromConstraintColumns(std::stringstream &text, std::string &schema_table_name, std::vector< Column * > &table_columns, std::vector< Column * > &constr_columns)
{
    if(constr_columns.empty())
    {
        return; // will be no select from this primary/unique key
    }

    std::vector< Column * >::iterator table_columns_it;
    std::vector< Column * >::iterator constr_columns_it;
    std::string constr_names = this->getKeyNamesFromColumns(constr_columns);

    // auxiliary vars
    Column *column;
    std::string col_name;

    // funcion definition and parameters
    text << '\n';
    text << "\n    def select_by_" << constr_names << "(";
    text << "\n        self,";
    for(constr_columns_it = constr_columns.begin(); constr_columns_it != constr_columns.end(); ++constr_columns_it)
    {
        column = *constr_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n        " << col_name;
        if(std::next(constr_columns_it, 1) != constr_columns.end()) text << ',';
    }
    text << "\n    ):";

    // commentary
    text << "\n        \"\"\" Retreive a '" << schema_table_name << "' record";
    text << "\n            using primary or unique key \"\"\"";
    text << '\n';

    // query arguments
    text << "\n        args = {";
    for(constr_columns_it = constr_columns.begin(); constr_columns_it != constr_columns.end(); ++constr_columns_it)
    {
        column = *constr_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n            \"" << col_name << "\": " << col_name;
        if(std::next(constr_columns_it, 1) != constr_columns.end()) text << ',';
    }
    text << "\n        }";
    text << '\n';

    // sql query
    text << "\n        sql = (";
    text << "\n            \"SELECT \"";
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        column = *table_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n            \"" << col_name;
        if(std::next(table_columns_it, 1) != table_columns.end()) text << ", ";
        text << '\"';
    }
    text << "\n            \" FROM " << schema_table_name << " WHERE \"";
    for(constr_columns_it = constr_columns.begin(); constr_columns_it != constr_columns.end(); ++constr_columns_it)
    {
        column = *constr_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n            \"" << col_name << " = %(" << col_name << ")s";
        if(std::next(constr_columns_it, 1) != constr_columns.end()) text << " AND ";
        text << '\"';
    }
    text << "\n        )";
    text << '\n';
    // execution
    text << "\n        try:";
    text << "\n            results = self.db.connection.execute(sql, args)";
    text << "\n            if results.rowcount > 1:";
    text << "\n                raise RuntimeError(";
    text << "\n                    \"ERROR: Select by primary/unique key\"";
    text << "\n                    \"returned multiple rows!\")";
    text << "\n            if results.rowcount == 1:";
    text << "\n                row = results.fetchone()";
    text << "\n                result = dict(zip(row.keys(), row.values()))";
    text << "\n            else:";
    text << "\n                result = None";
    text << "\n            results.close()";
    text << "\n            return result";
    text << "\n        except Exception, error:";
    text << "\n            self.db.logger.error(error)";
    text << "\n            raise";
}

std::string PythonDAOsGenerator::getDAONameFromSchemaTableName(std::string &schema_name, std::string &table_name)
{
    stringstream in(schema_name + '_' + table_name);
    stringstream out;

    while(!in.eof())
    {
        string x;
        std::getline(in, x, '_' );

        if(!x.empty())
        {
            x[0] = std::toupper(x[0]);
        }

        out << x;
    }

    return out.str();
}

std::vector< Column * > PythonDAOsGenerator::getTableColumns(Table &table)
{
    std::vector< Column * > table_columns;
    std::vector< TableObject * > *table_objects = table.getObjectList(OBJ_COLUMN);
    std::vector< TableObject * >::iterator table_objects_it;

    for(table_objects_it = table_objects->begin(); table_objects_it != table_objects->end(); ++table_objects_it)
    {
        table_columns.push_back((Column *)(*table_objects_it));
    }

    return table_columns;
}

std::vector< Column * > PythonDAOsGenerator::getTablePrimaryKeyColumns(Table &table)
{
    std::vector< Column * > pk_columns;
    Constraint *primary_key = table.getPrimaryKey();

    if(primary_key)
    {
        unsigned pk_num_columns = primary_key->getColumnCount(Constraint::SOURCE_COLS);

        for(unsigned i = 0; i<pk_num_columns; ++i)
        {
            pk_columns.push_back(primary_key->getColumn(i, Constraint::SOURCE_COLS));
        }
    }

    return pk_columns;
}

std::vector< Column * > PythonDAOsGenerator::getTablePrimaryKeyAutoColumns(Table &table)
{
    std::vector< Column * > pk_columns;
    Constraint *primary_key = table.getPrimaryKey();
    Column *column;

    if(primary_key)
    {
        unsigned pk_num_columns = primary_key->getColumnCount(Constraint::SOURCE_COLS);

        for(unsigned i = 0; i<pk_num_columns; ++i)
        {
            column = primary_key->getColumn(i, Constraint::SOURCE_COLS);

            if(column->getSequence() == nullptr)
            {
                pk_columns.push_back(column);
            }
        }
    }

    return pk_columns;
}

std::vector< std::vector< Column * > > PythonDAOsGenerator::getTableUniqueKeyColumns(Table &table)
{
    std::vector< std::vector< Column * > > uk_columns;

    std::vector< TableObject * > *table_objects = table.getObjectList(OBJ_CONSTRAINT);
    std::vector< TableObject * >::iterator table_objects_it;

    Constraint* unique_key;
    unsigned uk_num_columns;

    for(table_objects_it = table_objects->begin(); table_objects_it != table_objects->end(); ++table_objects_it)
    {
        unique_key = (Constraint *)(*table_objects_it);

        if(unique_key->getConstraintType() == ConstraintType::unique)
        {
            uk_num_columns = unique_key->getColumnCount(Constraint::SOURCE_COLS);

            if(uk_num_columns > 0)
            {
                std::vector< Column * > columns;
                for(unsigned i = 0; i<uk_num_columns; ++i)
                {
                    columns.push_back(unique_key->getColumn(i, Constraint::SOURCE_COLS));
                }
                uk_columns.push_back(columns);
            }
        }
    }

    return uk_columns;
}

std::string PythonDAOsGenerator::getKeyNamesFromColumns(std::vector< Column * > &pk_columns)
{
    std::string pk_names;
    std::vector< Column * >::iterator pk_columns_it;

    for(pk_columns_it = pk_columns.begin(); pk_columns_it != pk_columns.end(); ++pk_columns_it)
    {
        pk_names += (*pk_columns_it)->getName().toUtf8().constData();
        if(std::next(pk_columns_it, 1) != pk_columns.end()) pk_names += "_";
    }

    return pk_names;
}
