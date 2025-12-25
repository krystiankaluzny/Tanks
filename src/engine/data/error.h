#ifndef ENGINE_DATA_ERROR_H
#define ENGINE_DATA_ERROR_H

#include <stdexcept>
#include <string>

class Error : public std::runtime_error
{
public:
    explicit Error(const std::string &msg) : std::runtime_error(msg) {};
    Error(const std::string &msg, const std::string &details) : std::runtime_error(msg + " - Details: " + details) {};

    virtual ~Error() = default;
};

#endif // ENGINE_DATA_ERROR_H