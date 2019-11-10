#ifndef BASE_CODE_GENERATOR_H
#define BASE_CODE_GENERATOR_H

#include "base_logger.h"
#include "pgmodelerplugin.h"
#include <map>
#include <string>

class BaseCodeGenerator {

    public:
        virtual void generateCode(DatabaseModel &model, BaseLogger &logger, std::map< std::string, std::string > &files_to_generate) = 0;
};

#endif
