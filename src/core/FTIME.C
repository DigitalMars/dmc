/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#include <sys/timeb.h>
#include <time.h>
#include <scrtl.h>

void _ftime (struct _timeb *timeptr)
{
  struct tm *tmp;
  
  __internal_tzset ();
  timeptr->timezone = _timezone / 60;
  timeptr->time = time (NULL);
  timeptr->millitm = __time_milliseconds;
  tmp = localtime (&timeptr->time);
  timeptr->dstflag = tmp->tm_isdst;
}
