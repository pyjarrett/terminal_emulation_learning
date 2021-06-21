#include "errors.h"

#include <cstdlib>
#include <iostream>

namespace errors {

void die(const char* Message)
{
    std::cerr << Message << std::endl;
    std::exit(EXIT_FAILURE);
}

} // namespace errors