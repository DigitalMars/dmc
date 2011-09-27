/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#ifdef __NT__

#include <windows.h>
#include <dos.h>

void getdate (struct date *datePtr)
{
  SYSTEMTIME systime;
  
  GetSystemTime (&systime);
  datePtr->da_year = systime.wYear;
  datePtr->da_day = systime.wDay;
  datePtr->da_mon = systime.wMonth;
}

#endif /* __NT__ */
