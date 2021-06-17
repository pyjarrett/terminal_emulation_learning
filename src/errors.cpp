#include "errors.h"

#include <iostream>

namespace errors {

void Die(const char *Message)
{
    // use '\n' to force a flush.
    std::cerr << Message << '\n';
    std::exit(EXIT_FAILURE);
}

} // namespace errors