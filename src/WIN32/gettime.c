/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#ifdef __NT__

#include <windows.h>
#include <dos.h>

void gettime (struct time *timePtr)
{
  SYSTEMTIME systime;
  
  //GetSystemTime (&systime);
  GetLocalTime (&systime);
  timePtr->ti_min = (unsigned char)systime.wMinute;
  timePtr->ti_hour = (unsigned char)systime.wHour;
  timePtr->ti_hund = (unsigned char)(systime.wMilliseconds / 10);
  timePtr->ti_sec = (unsigned char)systime.wSecond;
}

#endif /* __NT__ */
