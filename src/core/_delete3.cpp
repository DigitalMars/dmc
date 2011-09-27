/*_ _delete3.cpp */

#include <stdlib.h>
#include <new.h>

/***********************
 * Standard C++ free function.
 */

void operator delete (void *p, const std::nothrow_t& t) throw()
{
    free(p);
}
