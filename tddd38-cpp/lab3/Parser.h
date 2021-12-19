#ifndef PARSER_H_
#define PARSER_H_

#include "Expression.h"

#include <stdexcept>
#include <string>

/* used to report parser errors */
class parser_error : public std::logic_error
{
public:
    using std::logic_error::logic_error;
};

Expression parse(std::string const& expression);

#endif//PARSER_H_
