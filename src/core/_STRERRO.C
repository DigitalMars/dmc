#include <string.h>
#include <errno.h>

char *_strerror (const char *string)
{
  static char buffer[128];

  *buffer = '\0';
  if (string != NULL && *string != '\0')
    strcat (strcat (buffer, string), ": ");
  return strcat (strcat (buffer, strerror (errno)), "\n");
}