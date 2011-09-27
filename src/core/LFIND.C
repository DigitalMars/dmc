#include <stdlib.h>
#include <search.h>

void *_lfind (const void *key, const void *base, unsigned int *num, unsigned int width,
  int (__cdecl *compare)(const void *elem1, const void *elem2))
{
  char *end;
  
  for (end = (char *)base + *num * width; (char *)base < end; (char *)base += width)
    if ((*compare) (key, base) == 0)
      return (void *)base;
  return NULL;
}
