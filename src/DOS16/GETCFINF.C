/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 */

#include <stdlib.h>
#include <scio.h>

void __get_c_file_info ()
{
  char *stringptr;
  unsigned char *infoptr;
  
  stringptr = getenv ("C_FILE_INFO");
  if (stringptr == NULL)
    return;
  for (infoptr = __fhnd_info; ; infoptr += 1)
  {
    *infoptr = *stringptr - 'A';
    stringptr += 1;
    *infoptr |= (*stringptr - 'A') << 4;
    stringptr += 1;
    if (*stringptr == '\0')
      break;
  }
  putenv ("C_FILE_INFO=");
}
