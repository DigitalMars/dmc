/*_ loctime.c    Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1986-1994 by Digital Mars  */
/* All Rights Reserved                                  */
/* Written by Walter Bright                             */

#include        <stdlib.h>
#include        <stdio.h>
#include        <time.h>
#include        <errno.h>
#include        <string.h>
#include 		 <scrtl.h>

#if __NT__
#include        <windows.h>
#endif

#if _MT
#include        "mt.h"
#endif

#if M_UNIX || M_XENIX
#define DefaultTimeZone 0L
#else
#define DefaultTimeZone 8L
#endif

/**************************
 * Convert from time_t to broken-down time.
 * Returns:
 *      pointer to broken-down time struct, which is static and is
 *      overwritten by each call to localtime().
 */

/***********
For UNIX we have to parse the environment variable TZ for daylight savings
time.

It appears that TIMEOFFSET is based on PDT from GMT.  PDT is GMT - 7 hours.
Keep this in mind when looking at the following code.  /etc/TIMEZONE and
TIMEZONE(F) are also useful references.

Format of TZ environment variable could be as simple as:
TZ='PST8'       Which means name of the zone is 'PST' which is 8 hours west of
GMT with no daylight savings time.

More complicated values are:
TZ='PST8PDT' Means PST is 8 hours west, PDT is name for daylight time, which
follows current US law for the date and time of change and is 1 hour less
than standard time.

TZ='PST8:00:00PDT7:00:00' Means the same as the previous example.

TZ='PST8:00:00PDT7:00:00;117/2:00:00,299/2:00:00' Means same as previous
except change over to daylight time occurs on 117th day (0 based -- Jan 1 is
day 0) at 2:00:00 AM.  Change over to standard time occurs on 299th day at
2:00:00 AM.  Note that for the southern hemisphere start of daylight time
will be at a date greater than end of daylight time.  Also note that leap
year is never counted.  That is March 1 is always day 59 (0 based) or J60
even on leap years.

Start and end dates for daylight savings time have other variations:
J117/2:00:00 would be a 1 based calender -- Jan 1 is day 1.

W15.6  Means week 15 (1 <= n <= 53) day of week 6 (Sunday == 0 -- 0 <= d <= 6).
Week 53 always means the last week containing day 'd' even if there are
actually only 52 weeks containing day d.

M4.5.6 Means day 6 (Sunday) of week 5 (1 <= n <= 5) month 4 (April).  Week
5 of a month is always the last week containing day 'd', whether there are
actually 4 or 5 weeks containing day d.
*********************/

/* First Sunday in April, last Sunday in October.*/
/* #define US_RULE ";M4.1.0/2,M10.5.0" */
#define US_RULE ";M4.5.0/2,M10.5.0"

/* Pacific daylight time */
#define DEFAULTTIMEZONE "PST8PDT7"

static int dayyear[2] = { 365, 366 };
static char daytab[2][13] = {
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

static make_gtime;

#include <ctype.h>
#include <stdlib.h>

static char * __near __pascal next_number(char *p)
{
	/* Advance past this number (if it exists). */
	while(*p && (*p == '+' || *p == '-' || *p == ':' || isdigit(*p)))
	  p++;
	/* Find the next number in the string, but not past a ';' or ','
	   or a letter */
	while(*p && *p != ';' && *p != ',' && *p != '+' && *p != '-' && !isalnum(*p))
	  p++;
	return p;
}

/*****************
Return the number of seconds different from GMT that we are.
*/
static time_t __near __pascal gmt_offset(char *tz_ptr)
{
	time_t offset = 0;

	if(tz_ptr && *tz_ptr)
	{
	while(isalpha(*tz_ptr)) /* Skip alpha part of the st. time */
		tz_ptr++;
	if(*tz_ptr)
	{
		int sign = 1;

		offset = atol(tz_ptr) * SECSPERHOUR;
		if(offset < 0)
		sign = -1;

		tz_ptr = next_number(tz_ptr);   /* Get minutes offset. */
		if(*tz_ptr && *tz_ptr != ';' && *tz_ptr != ',')
		{
		offset += sign * atol(tz_ptr) * 60;
		tz_ptr = next_number(tz_ptr);
		if(*tz_ptr && *tz_ptr != ';' && *tz_ptr != ',')
			offset += sign * atol(tz_ptr);
		}
	tz_ptr = next_number(tz_ptr);
	}
	}
	return offset;
}

/********************
Return the number of seconds since the beginning of the year that this
daylight savings time expression represents.  The first character pointed to
by p should be either a ';' or a ',' and is to be ignored. d_offset is the
day of the week that Jan 1 of this year falls on.  This should have been
'fudged' before reaching here if it is leap year and it is after Feb 28.
********************/
static long __near __pascal dst_time(char *p, int d_offset)
{
	long int month, ret_val;
	if(isdigit(*++p))       /* expression in 0 based days? */
	{
	ret_val = 0;
	goto j_case;
	}
	else
	ret_val = -1;

	switch(*p++)
	{
	case 'J':
j_case:
	ret_val += atol(p); /* Number of days into the year. */
	ret_val *= SECSPERDAY;
	break;
	case 'W':
	ret_val = atol(p) * 7 * SECSPERDAY;
	while(isdigit(*p))
		p++;
	if(*p == '.')
		ret_val += atol(p) * SECSPERDAY;
	break;
	case 'M':
	month = atoi(p) - 1;

	/* Get day of week of 1st day of this month. */
	d_offset = (d_offset + _mdays[month]) % 7;
	while(isdigit(*p))
		p++;
	if(*p++ == '.')
	{
		int week = atoi(p) - 1;
		while(isdigit(*p))
		p++;
		if(*p++ == '.')
		{
		static char days_in_month[12]=
			/*      J  F  M  A      M  J  J  A      S  O  N  D */
			  {31,28,31,30,31,30,31,31,30,31,30,31};
		int temp, day = atoi(p);
		temp = week * 7 + (7 + day - d_offset) % 7;
		if(temp >= days_in_month[month])
			temp -= 7;
		ret_val = (_mdays[month] + temp) * SECSPERDAY;
		}
	}
	break;
	}

	if((p = strchr(p,'/')) != 0)
	{
	ret_val += atol(++p) * SECSPERHOUR;
	while(isdigit(*p))
		p++;
	if(*p == ':')
	{
		ret_val += atol(++p) * 60;
		while(isdigit(*p))
		p++;
		if(*p == ':')
		ret_val += atol(++p);
	}
	}
	return ret_val;
}
/********************
Adjust this struct tm for daylight savings time.
********************/
static void __near __pascal dst_adjust(struct tm *t, char *tz_ptr)
{
	char *ptr;
	int leap;

	if(tz_ptr)
	{
	time_t dst_offset, std_offset = gmt_offset(tz_ptr);
	while(*tz_ptr && isalpha(*tz_ptr))
		tz_ptr++;
	tz_ptr = next_number(tz_ptr);   /* Advance to the first number. */

	/* Advance to the daylight time zone name. */
	while(*tz_ptr && !isalpha(*tz_ptr))
		tz_ptr++;

	if(*tz_ptr == 0)/* If no daylight time zone name then no dst. */
		 t->tm_isdst = 0;
	else
	{       /* Number of seconds from start of year for start/end of dst. */
		long now, dst_start, dst_end;
		/* Intentional ignore the possibility of leap year. */
		int jan1_wday = (t->tm_wday - (t->tm_yday % 7) + 7) % 7;

		now = t->tm_yday;
#if 0
		if(now > 58 && ISLEAP(t->tm_year))/* If a leap year -- day is */
		now--;                          /* to ignore leap year. */
#endif
		now *= 24;
		now += t->tm_hour;      now *= 60;
		now += t->tm_min;       now *= 60;
		now += t->tm_sec;

		while(isalpha(*tz_ptr))
		tz_ptr++;

		/* Get the offset from GMT for daylight time. */
#if 0
		dst_offset = isdigit(*tz_ptr)?
			 gmt_offset(tz_ptr - 1): std_offset - SECSPERHOUR;
#else
		dst_offset = std_offset - SECSPERHOUR;
#endif

		ptr = strchr(tz_ptr,';');/* If no rule for when to switch */
		if (!ptr)
			tz_ptr = strchr(tz_ptr,',');
		else
			tz_ptr = ptr;
		if(!tz_ptr)                     /* then use US rule. */
			tz_ptr = US_RULE;

		dst_start = dst_time(tz_ptr,jan1_wday);
		if (*tz_ptr == ',')
			tz_ptr++;
		tz_ptr = strchr(tz_ptr, ',');
		if(!tz_ptr)
		{
		t->tm_isdst = -1;       /* Error -- should be an end time. */
		return;
		}
		dst_end = dst_time(tz_ptr,jan1_wday);
		if(now >= dst_start && now <= dst_end ||
		(dst_start > dst_end && (now <= dst_end || now >= dst_start)))
		{
		long delta = std_offset - dst_offset;    

		t->tm_isdst = 1;
		t->tm_sec  += delta % 60;       delta /= 60;
		if (t->tm_sec >= 60)
		{
		   t->tm_sec -= 60;
		   t->tm_min ++;
		}
		t->tm_min  += delta % 60;       delta /= 60;
		if (t->tm_min >= 60)
		{
		  t->tm_min -= 60;
		  t->tm_hour ++;
		}
		t->tm_hour += delta;
		if (t->tm_hour >= 24)
		{
		  t->tm_hour -= 24;
		  t->tm_yday ++;
		  t->tm_mday ++;
		  t->tm_wday++;
		}
		t->tm_wday %= 7;
		leap = ISLEAP(t->tm_year);
		if ( t->tm_yday >= dayyear[leap] )
		{
		  t->tm_yday = 0;
		  t->tm_year ++;
		  t->tm_mon = 0;
		  t->tm_mday = 1;
		}
		else if ( t->tm_mday > daytab[leap][t->tm_mon+1] )
		{
		  t->tm_mon ++;
		  t->tm_mday=1;
		}

		}
		else
		t->tm_isdst = 0;
	}
	}
	else
	t->tm_isdst = -1;       /* Don't know if it is dst or not. */
}

/********************
Adjust (back) this struct tm for daylight savings time.
********************/
static void __near __pascal dst_adjust_back(struct tm *t, char *tz_ptr)
{
	char *ptr;
	int leap;

	if(tz_ptr)
	{
	time_t dst_offset, std_offset = gmt_offset(tz_ptr);
	while(*tz_ptr && isalpha(*tz_ptr))
		tz_ptr++;
	tz_ptr = next_number(tz_ptr);   /* Advance to the first number. */

	/* Advance to the daylight time zone name. */
	while(*tz_ptr && !isalpha(*tz_ptr))
		tz_ptr++;

	if(*tz_ptr == 0)/* If no daylight time zone name then no dst. */
		 /* t->tm_isdst = 0 */;
	else
	{       /* Number of seconds from start of year for start/end of dst. */
		long now, dst_start, dst_end;
		/* Intentional ignore the possibility of leap year. */
		int jan1_wday = (t->tm_wday - (t->tm_yday % 7) + 7) % 7;

		now = t->tm_yday;
#if 0
		if(now > 58 && ISLEAP(t->tm_year))/* If a leap year -- day is */
		now--;                          /* to ignore leap year. */
#endif
		now *= 24;
		now += t->tm_hour;      now *= 60;
		now += t->tm_min;       now *= 60;
		now += t->tm_sec;

		while(isalpha(*tz_ptr))
		tz_ptr++;

		/* Get the offset from GMT for daylight time. */
#if 0
		dst_offset = isdigit(*tz_ptr)?
			 gmt_offset(tz_ptr - 1): std_offset - SECSPERHOUR;
#else
		dst_offset = std_offset - SECSPERHOUR;
#endif

		ptr = strchr(tz_ptr,';');/* If no rule for when to switch */
		if (!ptr)
			tz_ptr = strchr(tz_ptr,',');
		else
			tz_ptr = ptr;
		if(!tz_ptr)                     /* then use US rule. */
			tz_ptr = US_RULE;

		dst_start = dst_time(tz_ptr,jan1_wday);
		if (*tz_ptr == ',')
			tz_ptr++;
		tz_ptr = strchr(tz_ptr, ',');
		if(!tz_ptr)
		{
		t->tm_isdst = -1;       /* Error -- should be an end time. */
		return;
		}
		dst_end = dst_time(tz_ptr,jan1_wday);
		if ( t->tm_isdst == 1 )
		  now -= std_offset - dst_offset;
		if(now >= dst_start && now <= dst_end ||
		(dst_start >= dst_end && (now <= dst_end || now >= dst_start)))
		{
		long delta = - (std_offset - dst_offset);

		/* t->tm_isdst = 1; */
		t->tm_sec  += delta % 60;       delta /= 60;
		if (t->tm_sec < 0)
		{
		   t->tm_sec += 60;
		   t->tm_min --;
		}
		t->tm_min  += delta % 60;       delta /= 60;
		if (t->tm_min < 0)
		{
		  t->tm_min += 60;
		  t->tm_hour --;
		}
		t->tm_hour += delta;
		if (t->tm_hour < 0)
		{
		  t->tm_hour += 24;
		  t->tm_yday --;
		  t->tm_mday --;
		  t->tm_wday --;
		}
		t->tm_wday = (t->tm_wday + 7) % 7;
		if ( t->tm_yday < 0 )
		{
		  t->tm_year --;
		  leap = ISLEAP(t->tm_year);
		  t->tm_yday = dayyear[leap]-1;
		  t->tm_mon = 11;
		  t->tm_mday = 31;
		}
		else if ( t->tm_mday < 1 )
		{
		  leap = ISLEAP(t->tm_year);
		  t->tm_mon --;
		  t->tm_mday = daytab[leap][t->tm_mon+1] ;
		}

		}
		else
		/* t->tm_isdst = 0 */;
	}
	}
	else
	t->tm_isdst = -1;       /* Don't know if it is dst or not. */
}


long timezone = DefaultTimeZone * 60L * 60L;
long altzone = 0;
int daylight = 0;
char __mainzone[10] = "";
char __altzone[10] = "";
char * const tzname[2] = {
			__mainzone,
			__altzone
			};

/*
 *  Call tzset and from globals, return what TZ points to 
 *  return NULL on failure
 */
static void make_tz(char *s)
{
   int  offset;

   __internal_tzset();
   /* Calculate offset off of GMT. */
   offset = timezone / (60L * 60L);  /* 60 secs per min. * 60 min. per hour. */
   sprintf (s,"%s%d%s",__mainzone,offset,__altzone);
}



void tzset(void)
{
	int i;
	char *p;
	char *tz_p = getenv("TZ");      /* TZ will give us hours from GMT */
	if (tz_p != NULL && *tz_p && toupper(*tz_p) >= 'A' && toupper(*tz_p) <= 'Z')
		{
		p = tz_p;
		i = 0;
		while (*p && !isdigit(*p) && *p != '-' && *p != '+' && i<9)
			__mainzone[i++] = *p++; // timzone can be 1 to 9 chars 
		__mainzone[i] = '\0';
		while(*p && (isdigit(*p) || *p == ':' || *p == '-' || *p == '+' || *p == ';'))
			p++;    // skip offset from GMT
		daylight = *p != '\0';
		if (daylight)
			{
			i = 0;
			while (*p && !isdigit(*p) && *p != '-' && *p != '+' && i < 9)
				__altzone[i++] = *p++;
			__altzone[i] = '\0';
			}
		else
		        __altzone[0] = '\0'; /* according to spec, tzname[1] is empty string when no daylight */
		timezone = gmt_offset(tz_p);
		}
	else
		{
#               if __NT__
			TIME_ZONE_INFORMATION tzinfo;
			DWORD retcode;
			
			retcode = GetTimeZoneInformation (&tzinfo);
			if (retcode != -1 && retcode != TIME_ZONE_ID_UNKNOWN)
			{
				timezone = tzinfo.Bias * 60;
				if (tzinfo.StandardDate.wMonth != 0)
					timezone += tzinfo.StandardBias * 60;
				daylight = tzinfo.DaylightDate.wMonth != 0
					&& tzinfo.DaylightBias != 0;
				*__mainzone = '\0';
				*__altzone = '\0';
			}
			else
			{
				strcpy(__mainzone, "PST");
				strcpy(__altzone, "PDT");
				daylight = 1;
				timezone = DefaultTimeZone * 60L * 60L;
			}
#               else
#                       if M_UNIX || M_XENIX
				strcpy(__mainzone, "GMT");
				strcpy(__altzone, "   ");
				daylight = 0;
#                       else
				strcpy(__mainzone, "PST");
				strcpy(__altzone, "PDT");
				daylight = 1;
#                       endif
			timezone = DefaultTimeZone * 60L * 60L;
#               endif
		}
	if (daylight)
		altzone = timezone - 60L * 60L;
}



struct tm *localtime(const time_t *pt)
{
	time_t t = *pt;
	struct tm *retval = NULL;
#if !(M_UNIX || M_XENIX)
    time_t adjustval;  /* value returned for dst adjustment */
#endif

#if M_UNIX || M_XENIX
	char tz_p[sizeof(__mainzone)+sizeof(__altzone)+sizeof(int)+1]; 
#endif
	
   if (t < 0)
     return NULL;

#if M_UNIX || M_XENIX
    make_tz(tz_p);
    if (tz_p != NULL)
		t += SECSPERHOUR * 7 - gmt_offset(tz_p);
	else
		t += SECSPERHOUR * 7;
    t -= TIMEOFFSET;                /* convert to DOS time          */
#else
    __internal_tzset();
    t -=  timezone;  /* Fixup from GMTIME to localtime */
#endif

   /* call gmtiime to do work */
   retval = gmtime(&t);
   if (!retval) goto ret;
   
#if M_UNIX || M_XENIX
   dst_adjust(retval,tz_p);
#else
   
   if (daylight)
   {
      if ((adjustval =  __adjustfordst(retval)) != 0)
      {
         t += adjustval;
         retval = gmtime(&t);
         retval->tm_isdst = 1;
      }
   }
#endif
      
ret:
    return retval;
            
}


/***********************************
 * Convert from broken-down time into a time_t.
 */

time_t mktime(struct tm *tmp)
{  

   time_t t;
	unsigned date;
	int n_leap;
	int is_leap;
	int i;
	int cur_year;
	struct tm *loctm; /* struct tm returned from localtime */
#if M_UNIX || M_XENIX
	char tz_p[sizeof(__mainzone)+sizeof(__altzone)+sizeof(int)+1]; 
#else
    int adjustval = 0;  
#endif
	

#if M_UNIX || M_XENIX
     make_tz(tz_p);  /* get time from GMT */
#else
     __internal_tzset();
#endif
   
   /* scale month to be between 0 and 11 */
  
      
        while (tmp->tm_mon < 0)
        {
           tmp->tm_mon += 12;
           tmp->tm_year --;
        }
        while (tmp->tm_mon > 11)
        {
           tmp->tm_mon -= 12;
           tmp->tm_year ++;
        }

	cur_year = tmp->tm_year;
	if ( cur_year < 1900 ) cur_year += 1900;
	if ( cur_year < YEAR0 )
		goto err;

#if M_UNIX || M_XENIX		
	if ( tmp->tm_isdst < 0 )
	  dst_adjust_back(tmp,tz_p);
	else 
	  tmp->tm_isdst = 0;
#endif	  

   n_leap = NLEAP_YEARS((cur_year - YEAR0));
	is_leap = ISLEAP(cur_year);

	if ( tmp->tm_mday > daytab[is_leap][tmp->tm_mon+1] )
		goto err;
	
   date = (cur_year - YEAR0) * 365 + n_leap;
   
   /* at this point date is number of days passed */
	date += _mdays[tmp->tm_mon];
   if (tmp->tm_mon > 1 && is_leap)
     date++;

	date += tmp->tm_mday-1;
	t = (time_t) date * (time_t) (60*60*24L) +
		((tmp->tm_hour * 60L) + tmp->tm_min) * 60L + tmp->tm_sec ;
#if M_UNIX || M_XENIX
	tzset();
	t += TIMEOFFSET + timezone - 7 * SECSPERHOUR;
#else
	  t += timezone;            
#endif

#if !(M_UNIX || M_XENIX)
    /* properly adjust for Daylight savings time */
    
    /* if tm_isdst is > 0, then we have to adjust */
	if (tmp->tm_isdst > 0)
	{
	    adjustval = __adjustfordst(tmp);
	    t -= adjustval;
	}
	/* if it is less than 0, we are not sure, have localtime inform us */
	else if (tmp->tm_isdst < 0)
	{
	    loctm = localtime(&t);
	    if (!loctm)
	      goto err;
	    if (loctm->tm_isdst > 0)
	    {
	       adjustval = __adjustfordst(loctm);
	       t -= adjustval;
	    }
	}
	
   /* adjust structure */
	loctm = localtime(&t);
	if (!loctm)
	   goto err;
	*tmp = *loctm;
	
#else
    *tmp = *localtime(&t);
#endif	       

    if ( tmp->tm_year < YEAR0 - 1900 )
		goto err;


	return t;

	err:
	return (time_t)-1;                      /* time cannot be represented   */
}

struct tm *gmtime(const time_t *timer)
{
#if _MT
	struct tm bdtime;               /* on stack so we have local copy */
#else
	static struct tm bdtime;
#endif
	int leap_period;
	int is_leap = 0;
	time_t t = *timer;
	int i;
	
   if (t < 0)
     return NULL;

   bdtime.tm_sec = t % 60;                 /* seconds 0..59        */
   bdtime.tm_min = (t / 60) % 60;          /* minutes 0..59        */
   bdtime.tm_hour = (t / SECSPERHOUR) % 24;/* hour of day 0..23    */
	
   t /= SECSPERDAY;                /* t = days since start of 1970 */
   bdtime.tm_wday = (WDAY0 + t) % 7;

   /*
      The approach here is to break up years into 4 year periods. Any given 4
      year period is made up of DAYS_PER_LEAP_PERIOD days.  Given the number 
      of days, divide by this value and it will give you how many 4 day periods
      have gone by. Multiply 4 will give you the number of years that have elapsed covering 
      those leap periods. The remainder will tell you how many days into the current 
      leap period you are. Using ifs, you can determine what year, day of the year
      and also dependant upon YEAR0, you will know if the year is a leap year or not.
   */
   bdtime.tm_year = (t / DAYS_PER_LEAP_PERIOD) * 4L + (YEAR0 - 1900);
   leap_period = t % DAYS_PER_LEAP_PERIOD;
   
   if (leap_period < DAYS_IN_YEAR1)
   {
      bdtime.tm_yday = leap_period;
   }
   else if (leap_period >= DAYS_IN_YEAR1 && leap_period < DAYS_IN_YEAR2)
   {
      bdtime.tm_year += 1;
      bdtime.tm_yday = leap_period - DAYS_IN_YEAR1;
   }
   else if (leap_period >= DAYS_IN_YEAR2 && leap_period < DAYS_IN_YEAR3)
   {
      /* for a YEAR0 of 1970 the the leap year is the 3rd year of the 4 year period, 
         when YEAR0 changes, this will have to change. It is faster than using ISLEAP.
       */
      is_leap = 1; 
      bdtime.tm_year += 2;
      bdtime.tm_yday = leap_period - DAYS_IN_YEAR2;
   }
   else 
   {
      bdtime.tm_year += 3;
      bdtime.tm_yday = leap_period - DAYS_IN_YEAR3;
   }
   
  
    
   t = bdtime.tm_yday;
 
   for ( i =1; i<13; t -= daytab[is_leap][i], i++ )
	{
	   if ( daytab[is_leap][i] > t )
		break;
	}


   bdtime.tm_mon =  i - 1;
   bdtime.tm_mday = t + 1;

   bdtime.tm_isdst = 0;
   
    if ( bdtime.tm_year < YEAR0 - 1900 )
 	return (struct tm*) 0;

#if defined(_MT)
	{       /* Copy result to this thread's version of bdtime   */
		struct tm *pbdtime = &_getthreaddata()->t_tm;
		*pbdtime = bdtime;
	  	return pbdtime;
	}
#else

	return &bdtime;
#endif
}


