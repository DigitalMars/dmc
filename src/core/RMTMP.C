/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */
 
#include <scio.h>

int _rmtmp (void)
{
  FILE *fp;
  int count;

  count = 0;
  for (fp = _iob; fp < _iob + _NFILE; fp += 1)
  {
    __fp_lock (fp);
    if (_tmpnum (fp) != 0)
    {
      fclose(fp);
      count += 1;
    }
    __fp_unlock(fp);
  }
  return count;
}
