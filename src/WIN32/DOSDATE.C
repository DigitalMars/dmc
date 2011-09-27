/*_ dosdate.c	 Modified by: Walter Bright */
/*
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include	<dos.h>


#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
#endif


/****************************
 * Set dos date information.
 */

unsigned dos_setdate(struct dos_date_t *date)
{
#ifdef	__OS2__
    DateTime datetime;

    DosGetDateTime(&datetime);
    datetime.day = date->day;
    datetime.month = date->month;
    datetime.year = date->year;
    datetime.weekday = date->dayofweek;
    return DosSetDateTime(&datetime);
#endif
#ifdef	__NT__
    SYSTEMTIME datetime;

    GetLocalTime(&datetime);
    datetime.wDay = date->day;
    datetime.wMonth = date->month;
    datetime.wYear = date->year;
    datetime.wDayOfWeek = date->dayofweek;
    return SetLocalTime(&datetime);
#endif
}

/****************************
 * Get dos date information
 */

void dos_getdate(struct dos_date_t *date)
{
#ifdef __OS2__	
    DateTime datetime;

    DosGetDateTime(&datetime);

    date->day = datetime.day;
    date->month = datetime.month;
    date->year = datetime.year;
    date->dayofweek = datetime.weekday;
#endif
#ifdef __NT__
    SYSTEMTIME datetime;

    GetLocalTime(&datetime);

    date->day = (char)datetime.wDay;
    date->month = (char)datetime.wMonth;
    date->year = datetime.wYear;
    date->dayofweek = (char)datetime.wDayOfWeek;
#endif
}

/****************************
 * Set dos time information
 */

unsigned dos_settime(struct dos_time_t *time)
{
#ifdef	__OS2__
    DateTime datetime;

    DosGetDateTime(&datetime);
    datetime.hours = time->hour;
    datetime.minutes = time->minute;
    datetime.seconds = time->second;
    datetime.hundredths = time->hsecond;
    return DosSetDateTime(&datetime);
#endif
#ifdef	__NT__
    SYSTEMTIME datetime;

    GetLocalTime(&datetime);
    datetime.wHour = time->hour;
    datetime.wMinute = time->minute;
    datetime.wSecond = time->second;
    datetime.wMilliseconds = (WORD)time->hsecond * 10;
    return SetLocalTime(&datetime);
#endif
}

/****************************
 * Get dos time information
 */

void dos_gettime(struct dos_time_t *time)
{
#ifdef __OS2__
    DateTime datetime;

    DosGetDateTime(&datetime);

    time->hour = datetime.hours;
    time->minute = datetime.minutes;
    time->second = datetime.seconds;
    time->hsecond = datetime.hundredths;
#endif
#ifdef __NT__
    SYSTEMTIME datetime;

    GetLocalTime(&datetime);

    time->hour = (char)datetime.wHour;
    time->minute = (char)datetime.wMinute;
    time->second = (char)datetime.wSecond;
    time->hsecond = (char)(datetime.wMilliseconds / 10);
#endif
}

