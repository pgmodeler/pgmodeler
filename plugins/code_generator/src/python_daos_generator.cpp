#include "python_daos_generator.h"
#include "pgmodelerplugin.h"
#include <string>

void PythonDAOsGenerator::generateCode(DatabaseModel *model, BaseLogger *logger)
{
    std::stringstream text;

    // loop all tables
    std::vector< BaseObject * > * tables = model->getObjectList(OBJ_TABLE);
    std::vector< BaseObject * >::iterator it;

    for(it = tables->begin(); it != tables->end(); ++it)
    {
        this->generateTable(text, (Table *)(* it));
    }

    logger->log(text.str());
}

// Starting code generation
void PythonDAOsGenerator::generateTable(std::stringstream &text, Table *table)
{
    // Header:
    this->generateHeader(text, table);
    text << '\n';

    // Insert
    this->generateInsert(text, table);
    text << '\n';

    // Update
    this->generateUpdate(text, table);
    text << '\n';

    // Delete
    this->generateDelete(text, table);
    text << '\n';

    // Select
    this->generateSelectFromPK(text, table);
    text << '\n';
}

void PythonDAOsGenerator::generateHeader(std::stringstream &text, Table *table)
{
    std::string table_name = table->getName().toUtf8().constData();
    std::string dao_name = this->getDAONameFromTableName(table_name);

    text << "#!/usr/bin/env python";
    text << "\n# -*- coding: utf-8 -*-";
    text << "\n\"\"\"";
    text << "\n@package " << dao_name << " Data Access Object";
    text << "\n         for '" << table_name << "' table";
    text << "\n\"\"\"";
    text << '\n';
    text << '\n';
    text << "\nclass " << dao_name << "():";
    text << '\n';
    text << "\n    def __init__(self, database):";
    text << "\n        self.db = database";
}

void PythonDAOsGenerator::generateInsert(std::stringstream &text, Table *table)
{
    // table name
    std::string table_name = table->getName().toUtf8().constData();

    // get table columns and iterator to loop under all table columns
    std::vector< Column * > table_columns = this->getTableColumns(table);
    std::vector< Column * >::iterator table_columns_it;

    // get primary key columns and iterator
    std::vector< Column * > pk_columns = this->getTablePrimaryKeyColumns(table);
    std::vector< Column * >::iterator pk_columns_it;

    // get primary key columns that have a sequence
    std::vector< Column * > pk_auto_columns = this->getTablePrimaryKeyAutoColumns(table);
    std::vector< Column * >::iterator pk_auto_columns_it;

    // auxiliary vars
    Column *column;
    std::string col_name;
    bool is_auto_pk;
    bool is_first_column_writed;

    // funcion definition and parameters
    text << "\n    def insert(";
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
    text << "\n        \"\"\" Insert a '" << table_name << "' record";
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
    text << "\n        }\n";

    // sql query
    text << "\n        sql = (";
    text << "\n            \"INSERT INTO " << table_name << " (";
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

void PythonDAOsGenerator::generateUpdate(std::stringstream &text, Table *table)
{

}

void PythonDAOsGenerator::generateDelete(std::stringstream &text, Table *table)
{

}

void PythonDAOsGenerator::generateSelectFromPK(std::stringstream &text, Table *table)
{
    // table name
    std::string table_name = table->getName().toUtf8().constData();

    // get table columns and iterator to loop under all table columns
    std::vector< Column * > table_columns = this->getTableColumns(table);
    std::vector< Column * >::iterator table_columns_it;

    // get primary key columns and iterator
    std::vector< Column * > pk_columns = this->getTablePrimaryKeyColumns(table);
    std::vector< Column * >::iterator pk_columns_it;

    if(pk_columns.empty())
    {
        return; // will be no select from pk
    }

    std::string pk_names = this->getTablePrimaryKeyNames(pk_columns);

    // auxiliary vars
    Column *column;
    std::string col_name;

    // funcion definition and parameters
    text << "\n    def select_by_" << pk_names << "(";
    text << "\n        self,";
    for(pk_columns_it = pk_columns.begin(); pk_columns_it != pk_columns.end(); ++pk_columns_it)
    {
        column = *pk_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n        " << col_name;
        if(std::next(pk_columns_it, 1) != pk_columns.end()) text << ',';
    }
    text << "\n    ):";

    // commentary
    text << "\n        \"\"\" Retreive a '" << table_name << "' record";
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
    text << "\n            \"SELECT \"";
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        column = *table_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "\n            \"" << col_name;
        if(std::next(table_columns_it, 1) != table_columns.end()) text << ", ";
        text << '\"';
    }
    text << "\n            \" FROM " << table_name << " WHERE \"";
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
    text << "\n            results = []";
    text << "\n            for row in result:";
    text << "\n                results.append(dict(zip(row.keys(), row.values())))";
    text << "\n            return results";
    text << "\n        except Exception, error:";
    text << "\n            self.db.logger.error(error)";
    text << "\n            raise";
}

std::string PythonDAOsGenerator::getDAONameFromTableName(std::string &table_name)
{
    stringstream in(table_name);
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

std::vector< Column * > PythonDAOsGenerator::getTableColumns(Table *table)
{
    std::vector< Column * > table_columns;
    std::vector< TableObject * > *table_objects = table->getObjectList(OBJ_COLUMN);
    std::vector< TableObject * >::iterator table_objects_it;

    for(table_objects_it = table_objects->begin(); table_objects_it != table_objects->end(); ++table_objects_it)
    {
        table_columns.push_back((Column *)(*table_objects_it));
    }

    return table_columns;
}

std::vector< Column * > PythonDAOsGenerator::getTablePrimaryKeyColumns(Table *table)
{
    std::vector< Column * > pk_columns;
    Constraint *primary_key = table->getPrimaryKey();

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

std::vector< Column * > PythonDAOsGenerator::getTablePrimaryKeyAutoColumns(Table *table)
{
    std::vector< Column * > pk_columns;
    Constraint *primary_key = table->getPrimaryKey();
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

std::string PythonDAOsGenerator::getTablePrimaryKeyNames(std::vector< Column * > &pk_columns)
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
