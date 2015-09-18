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
    this->generateSelect(text, table);
}

void PythonDAOsGenerator::generateHeader(std::stringstream &text, Table *table)
{
    std::string table_name = table->getName().toUtf8().constData();
    std::string dao_name = this->getDAONameFromTableName(table_name);

    text << "#!/usr/bin/env python\n";
    text << "# -*- coding: utf-8 -*-\n";
    text << "\"\"\"\n";
    text << "@package " << dao_name << " Data Access Object\n";
    text << "         for '" << table_name << "' table\n";
    text << "\"\"\"\n";
    text << '\n';
    text << '\n';
    text << "class " << dao_name << "():\n";
    text << '\n';
    text << "    def __init__(self, database):\n";
    text << "        self.db = database\n";
}

void PythonDAOsGenerator::generateInsert(std::stringstream &text, Table *table)
{
    // table name
    std::string table_name = table->getName().toUtf8().constData();

    // get primary key columns iterator to loop for all ref columns
    std::vector< Column * > pk_columns = this->getTablePrimaryKeysColumns(table);
    std::vector< Column * >::iterator pk_columns_it;

    // get columns iterator to loop under all table columns
    std::vector< Column * > table_columns = this->getTableColumns(table);
    std::vector< Column * >::iterator table_columns_it;

    // auxiliary vars
    Column *column;
    std::string col_name;

    // funcion definition and parameters
    text << "    def insert(\n";
    text << "        self,\n";
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        column = *table_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "        " << col_name;
        if(std::next(table_columns_it, 1) != table_columns.end()) text << ", ";
        text << "\n";
    }
    text << "    ):\n";
    // commentary
    text << "        \"\"\" Insert a '" << table_name << "' record\n";
    text << "            and get back the DB generated ID \"\"\"\n";
    text << '\n';
    // query arguments
    text << "        args = {\n";
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        column = *table_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "            \"" << col_name << "\": " << col_name;
        if(std::next(table_columns_it, 1) != table_columns.end()) text << ",";
        text << "\n";
    }
    text << "        }\n";
    text << '\n';
    // sql query
    text << "        sql = (\n";
    text << "            \"INSERT INTO " << table_name << " (\"\n";
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        column = *table_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "            \"" << col_name;
        if(std::next(table_columns_it, 1) != table_columns.end()) text << ", ";
        text << "\"\n";
    }
    text << "            \") VALUES (\"\n";
    for(table_columns_it = table_columns.begin(); table_columns_it != table_columns.end(); ++table_columns_it)
    {
        column = *table_columns_it;
        col_name = column->getName().toUtf8().constData();
        text << "            \"%(" << col_name << ")s";
        if(std::next(table_columns_it, 1) != table_columns.end()) text << ", ";
        text << "\"\n";
    }
    text << "            \")";
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
    text << "\"\n";
    text << "        )\n";
    text << '\n';
    // execution
    text << "        try:\n";
    text << "            result = self.db.execute(sql, args)\n";
    if(!pk_columns.empty())
    {
        text << "            return dict(result.fetchone())\n";
    }
    else
    {
        text << "            result.close()\n";
        text << "            return dict()\n";
    }
    text << "        except Exception, error:\n";
    text << "            self.db.logger.error(error)\n";
    text << "            raise\n";
}

void PythonDAOsGenerator::generateUpdate(std::stringstream &text, Table *table)
{

}

void PythonDAOsGenerator::generateDelete(std::stringstream &text, Table *table)
{

}

void PythonDAOsGenerator::generateSelect(std::stringstream &text, Table *table)
{

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

std::vector< Column * > PythonDAOsGenerator::getTablePrimaryKeysColumns(Table *table)
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
