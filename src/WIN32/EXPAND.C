/* Temporary solution to having an _expand entry point in the library
 * for MFC to use.
 */

#include <stdlib.h>

#undef _expand

void *_expand (void *block, size_t newsize)
{
  return realloc (block, newsize);
}
