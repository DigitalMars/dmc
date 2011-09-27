#include <stdlib.h>
#include <memory.h>
#include <search.h>

void *_lsearch (const void *key, const void *base, unsigned int *num,unsigned int width,
  int (__cdecl *compare) (const void *elem1,const void *elem2))
{
  void *result;
  
  result = _lfind (key, base, num, width, compare);
  if (result != NULL)
    return result;
  return memcpy ((char *)base + (*num)++ * width, key, width);
}
