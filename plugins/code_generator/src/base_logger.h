#ifndef BASE_LOGGER_H
#define BASE_LOGGER_H

#include <string>

class BaseLogger {

    public:
        virtual void log(std::string text) = 0;
};

#endif
