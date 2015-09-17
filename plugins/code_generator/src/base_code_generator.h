#ifndef BASE_CODE_GENERATOR_H
#define BASE_CODE_GENERATOR_H

#include "pgmodelerplugin.h"
#include "base_logger.h"

class BaseCodeGenerator {

    public:
        virtual void generateCode(DatabaseModel *model, BaseLogger *logger) = 0;
};

#endif
