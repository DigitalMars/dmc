/*_ _delete.cpp   Tue May 30 1989   Modified by: Walter Bright */

#include <stdlib.h>

/***********************
 * Standard C++ free function.
 * Nothing to it.
 */

void operator delete (void *p)
{
    free(p);
}
