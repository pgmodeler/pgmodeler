#ifndef PYTHON_DAOS_GENERATOR_H
#define PYTHON_DAOS_GENERATOR_H

#include "base_code_generator.h"
#include <sstream>
#include <string>
#include <vector>

class PythonDAOsGenerator: public BaseCodeGenerator {

    public:
        void generateCode(DatabaseModel &model, BaseLogger &logger, std::map< std::string, std::string > &files_to_generate);

    private:
        std::string generateCompleteTableCode(Table &table);

        void generateHeader(std::stringstream &text, Table &table);
        void generateInsert(std::stringstream &text, Table &table);
        void generateUpdateFromPK(std::stringstream &text, Table &table);
        void generateDeleteFromPK(std::stringstream &text, Table &table);
        void generateSelectFromPK(std::stringstream &text, Table &table);
        void generateSelectFromUKs(std::stringstream &text, Table &table);

        void generateSelectFromConstraintColumns(std::stringstream &text, std::string &table_name, std::vector< Column * > &table_columns, std::vector< Column * > &constr_columns);
        std::string getDAONameFromTableName(std::string &table_name);
        std::vector< Column * > getTableColumns(Table &table);
        std::vector< Column * > getTablePrimaryKeyColumns(Table &table);
        std::vector< Column * > getTablePrimaryKeyAutoColumns(Table &table);
        std::vector< std::vector< Column * > > getTableUniqueKeyColumns(Table &table);
        std::string getKeyNamesFromColumns(std::vector< Column * > &pk_columns);
};

#endif
