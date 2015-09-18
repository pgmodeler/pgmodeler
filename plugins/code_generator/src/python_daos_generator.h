#ifndef PYTHON_DAOS_GENERATOR_H
#define PYTHON_DAOS_GENERATOR_H

#include "base_code_generator.h"
#include <sstream>

class PythonDAOsGenerator: public BaseCodeGenerator {

    public:
        void generateCode(DatabaseModel *model, BaseLogger *logger);

    private:
        void generateTable(std::stringstream &text, Table *table);
        void generateHeader(std::stringstream &text, Table *table);
        void generateInsert(std::stringstream &text, Table *table);
        void generateUpdate(std::stringstream &text, Table *table);
        void generateDelete(std::stringstream &text, Table *table);
        void generateSelect(std::stringstream &text, Table *table);

        std::string getDAONameFromTableName(std::string &table_name);
        std::vector< Column * > getTableColumns(Table *table);
        std::vector< Column * > getTablePrimaryKeysColumns(Table *table);
};

#endif
