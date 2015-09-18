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

    // Insert
    this->generateInsert(text, table);

    // Update
    this->generateUpdate(text, table);

    // Delete
    this->generateDelete(text, table);

    // Select
    this->generateSelect(text, table);
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
    text << '\n';
}

void PythonDAOsGenerator::generateInsert(std::stringstream &text, Table *table)
{
    std::string table_name = table->getName().toUtf8().constData();

    // get primary key columns iterator to loop for all ref columns
    Constraint *primary_key = table->getPrimaryKey();
    unsigned pk_num_columns = 0;

    if(primary_key)
    {
        pk_num_columns = primary_key->getColumnCount(Constraint::SOURCE_COLS);
    }

    // get columns iterator to loop under all table columns
    std::vector< TableObject * > *columns = table->getObjectList(OBJ_COLUMN);
    std::vector< TableObject * >::iterator cols_it;

    // auxiliary vars
    Column *column;
    std::string col_name;

    // funcion definition and parameters
    text << "    def insert(\n";
    text << "        self,\n";
    for(cols_it = columns->begin(); cols_it != columns->end(); ++cols_it)
    {
        column = ((Column *)(* cols_it));
        col_name = column->getName().toUtf8().constData();
        text << "        " << col_name;
        if(std::next(cols_it, 1) != columns->end()) text << ", ";
        text << "\n";
    }
    text << "    ):\n";
    // commentary
    text << "        \"\"\" Insert a '" << table_name << "' record\n";
    text << "            and get back the DB generated ID \"\"\"\n";
    text << '\n';
    // query arguments
    text << "        args = {\n";
    for(cols_it = columns->begin(); cols_it != columns->end(); ++cols_it)
    {
        column = ((Column *)(* cols_it));
        col_name = column->getName().toUtf8().constData();
        text << "            \"" << col_name << "\": " << col_name;
        if(std::next(cols_it, 1) != columns->end()) text << ",";
        text << "\n";
    }
    text << "        }\n";
    text << '\n';
    // sql query
    text << "        sql = (\n";
    text << "            \"INSERT INTO " << table_name << " (\"\n";
    for(cols_it = columns->begin(); cols_it != columns->end(); ++cols_it)
    {
        column = ((Column *)(* cols_it));
        col_name = column->getName().toUtf8().constData();
        text << "            \"" << col_name;
        if(std::next(cols_it, 1) != columns->end()) text << ", ";
        text << "\"\n";
    }
    text << "            \") VALUES (\"\n";
    for(cols_it = columns->begin(); cols_it != columns->end(); ++cols_it)
    {
        column = ((Column *)(* cols_it));
        col_name = column->getName().toUtf8().constData();
        text << "            \"%(" << col_name << ")s";
        if(std::next(cols_it, 1) != columns->end()) text << ", ";
        text << "\"\n";
    }
    text << "            \")";
    if(primary_key)
    {
        text << " RETURNING ";
        for(unsigned i = 0; i<pk_num_columns; ++i)
        {
            column = primary_key->getColumn(i, Constraint::SOURCE_COLS);
            col_name = column->getName().toUtf8().constData();
            text << col_name;
            if(i+1 != pk_num_columns) text << ", ";
        }
    }
    text << "\"\n";
    text << "        )\n";
    text << '\n';
    // execution
    text << "        try:\n";
    text << "            result = self.db.execute(sql, args)\n";
    if(pk_num_columns > 0)
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
    text << '\n';
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
