/*_ time.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Walter Bright
 * All Rights Reserved
 * Written by Walter Bright
 */

#include <stddef.h>
#include <time.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <scrtl.h>

#if defined(__OS2__)
#include "os2lib.h"
#elif defined( _WIN32)
#include <windows.h>
#else
#include <dos.h>
#endif



unsigned short __time_milliseconds;

/****************************************
 * Return the number of seconds that have elapsed since the start
 * of 1970.
 * Input:
 *	timer	pointer to where to store result (or NULL)
 * Output:
 *	*timer = result (unless timer == NULL)
 * Returns:
 *	time
 */

time_t time(time_t *timer)
{	
	
	time_t t;

#if defined(_WIN32)
	SYSTEMTIME datetime;
    struct tm  converttm;
   
	GetLocalTime (&datetime);

   converttm.tm_sec = datetime.wSecond;
   converttm.tm_min = datetime.wMinute;
   converttm.tm_hour = datetime.wHour;
   converttm.tm_year = datetime.wYear;
   converttm.tm_mon = datetime.wMonth; 
   converttm.tm_mday = datetime.wDay;  
  	__time_milliseconds = datetime.wMilliseconds;
  	t = __localstructtmtotime_t(&converttm);

#elif defined(__OS2__)
	DateTime datetime;
	unsigned long secs;
	int day;		// must be signed
	unsigned month;
	unsigned year;
    int is_leap = 0;

	DosGetDateTime(&datetime);

	/* Compute # of seconds since midnight	*/
	secs = (unsigned long)((datetime.hours * 60) + datetime.minutes) * 60 +
	datetime.seconds;

	year = datetime.year - 1970;	/* years since 1970		*/
	month = datetime.month - 1;		/* make it 0..11		*/
	day = datetime.day - 1;
	__time_milliseconds = some value we get from OS/2 if we ever support it again.

	day += _mdays[month];	/* add days prior to current month in year */
  
   /* If after february in a leap year, add in leap day */
   if (is_leap && month > 1)
     day++;
     
	t = SECSPERDAY *
	    (
	       (year * 365L) +      /* days exclusive of leap years */
	       ((year + 1) >> 2) +    /* one day per leap year since 1970 */
	       day		    /* days since beginning of year */
	    ) + 
	    secs;		    /* seconds since midnight */

#else  /* WINDOWS, DOS, and DOSX */
   struct dos_date_t dosdate;
   struct dos_time_t dostime;
   unsigned char day;
   
   struct tm  converttm;
    
   dos_getdate(&dosdate); /* get original date */
   
   while (1)
   {
        day = dosdate.day;  /* for checking if we straddled midnight */
   
        dos_gettime(&dostime);
        dos_getdate(&dosdate);
        if (day == dosdate.day)
           break;
   }
     
   converttm.tm_sec = dostime.second;
   converttm.tm_min = dostime.minute;
   converttm.tm_hour = dostime.hour;
   converttm.tm_year = dosdate.year;
   converttm.tm_mon = dosdate.month; 
   converttm.tm_mday = dosdate.day;  
  	__time_milliseconds = dostime.hsecond * 10; 
   t = __localstructtmtotime_t(&converttm);
#endif


	if (timer)
	*timer = t;

	return t;
}
