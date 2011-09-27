/*_ timesup.c */
/* Copyright (C) 1995 by Digital Mars  */
/* All Rights Reserved     */

#include <time.h>
#include <scrtl.h>


/* these static routines aid in calculating DST */
static int near pascal _calc_yday(struct tm *);
static time_t near pascal _calc_dststartday(struct tm *, int);
static time_t near pascal _calc_dstendday(struct tm *, int);

/**************************
 * # of days in year at start of month
 */
const unsigned short _mdays[12+1] =
{	0,
	31,
	31+28,
	31+28+31,
	31+28+31+30,
	31+28+31+30+31,
	31+28+31+30+31+30,
	31+28+31+30+31+30+31,
	31+28+31+30+31+30+31+31,
	31+28+31+30+31+30+31+31+30,
	31+28+31+30+31+30+31+31+30+31,
	31+28+31+30+31+30+31+31+30+31+30,
	365
};

/*
 * Given a struct tm built from local time information
 * obtained from the operating system, return a time_t
 * in UTC based on the tz settings. This routine is called
 * from the following functions:
 *	time();
 *	stat();
 * 	fstat();
 */
time_t __localstructtmtotime_t(struct tm *t)
{
	time_t years,days;
	time_t secs,retval=-1;
        int month;
        struct tm dst_tm;  /* copy of t passed to __adjustfordst */

        /* get number of years that have elapsed, checking for err */
        if (t->tm_year < YEAR0) goto ret;

        years = t->tm_year - YEAR0;

        /* get number of days that have passed this year */
        month = t->tm_mon - 1;  /* get month 0..11 */
        days = _mdays[month] + t->tm_mday - 1; /* Remember, this day has not finished */

        /* If after february in a leap year, add in leap day */
        if (ISLEAP(t->tm_year) && month > 1)
                days++;

	/* Compute # of seconds since midnight	*/
	secs = (unsigned long)((t->tm_hour * 60) + t->tm_min) * 60 + t->tm_sec;

	retval = SECSPERDAY *
	    (
	       (years * 365L) +      /* days exclusive of leap years */
                NLEAP_YEARS(years)+	 /* one day per leap year since 1970 */
                days		    /* days since beginning of year */
	    ) +
	    secs;		    /* seconds since midnight */

        /* We need to return UTC, so call tzset and adjust by timezone and
           account for dst */
        __internal_tzset();
        retval += timezone;
        if (daylight)
        {
                dst_tm = *t;
                dst_tm.tm_mon -= 1;  /* __adjustfordst expects month 0..11 */
                dst_tm.tm_year -= 1900;  /* __adjustfordst expects years since 1900 */
                retval -= __adjustfordst(&dst_tm);
        }

ret:
        return retval;

}


/*
 *  Given a struct tm pointer, determine if the callee must adjust for DST
 *  or not.
 *  The struct tm passed in has to have the  month in the range 0..11
 *  Returns:
 *      0 - Do not adjust for DST.
 *      Non-zero - Number of seconds to adjust for DST, this is so that
 *      it will be easy to add support for Non-US rules.
 */
 int __adjustfordst (struct tm *t)
 {

        int retval = 0;


        /* if before april and after October, then no need to worry about adjusting */
        /* BUG: should use env settings to figure out what month DST is done on */
        if (t->tm_mon < 3 || t->tm_mon > 9)
                retval = 0; /* Do not adjust */

        /* if May - September, then yes, adjust for DST */
        else if (t->tm_mon > 3 && t->tm_mon < 9)
                retval = SECSPERHOUR;  /* US Rule, adjust by 1 hour */
        else
        {
                time_t now,dst_start,dst_end;

                /* Was either in October or April, deal with it */
                now = (_calc_yday(t) * SECSPERDAY) + (t->tm_hour * 60L * 60L) + (t->tm_min * 60L) + t->tm_sec;
                dst_start = (_calc_dststartday(t,3) * SECSPERDAY) + (2 * SECSPERHOUR);
                dst_end = (_calc_dstendday(t,9) * SECSPERDAY) + (2 * SECSPERHOUR);
               	if(now >= dst_start && now <= dst_end)
                  retval = SECSPERHOUR;
                else
                  retval = 0;

        }

        return retval;
 }

 /***
  *  Given a struct tm and a specific month, calculate the number of days to
  *  first sunday of that month, this can be used to calculate the number
  *  of seconds to the start of DST in any given year.  This should be
  *  written to actually information in the tz settings to return this value.
  *  For example, the environment may actually specify the number of days,
  *  so this routine could simply return that and have no work to do.
  *  For now it is written to the US Rule.
  */
 static time_t near pascal _calc_dststartday(struct tm *t, int mon)
 {
        time_t retval;
		time_t totaldays;   /* totaldays elapsed between Jan 1, 1970 and April 1 of current year */
        time_t cur_days = _mdays[mon]; /* days elapsed in current year to start of DST */

        int temp_year = (t->tm_year + 1900) - YEAR0;

        if (ISLEAP(t->tm_year+1900) && mon > 1)
                cur_days++;

        totaldays = cur_days + (temp_year * 365L) + NLEAP_YEARS(temp_year);

        retval =  ((WDAY0 + totaldays) % 7);

        /* retval is now day 1 on the month in question, now figure out how many more days until
           the first sunday */
        if (retval != 0)
          retval = 7 - retval;

        return (cur_days + retval);
 }

  /***
  *  Given a struct tm and a specific month, calculate the number of days to
  *  last sunday of that month, this can be used to calculate the number
  *  of seconds to the end of DST in any given year.  This should be
  *  written to actually use information in the tz settings to return this value.
  *  For example, the environment may actually specify the number of days,
  *  so this routine could simply return that and have no work to do.
  *  For now, it is written to the US Rule.
  */
 static time_t near pascal _calc_dstendday(struct tm *t, int mon)
 {
        time_t retval;
		time_t totaldays;  /* total days elapsed from Jan 1, 1970 to day of mon in question */
        time_t cur_days = _mdays[mon+1] - 1;  /* get to last day of month in question */

        int temp_year = (t->tm_year + 1900) - YEAR0;

        if (ISLEAP(t->tm_year+1900) && mon > 1)
                cur_days++;

        totaldays = cur_days + (temp_year * 365L) + NLEAP_YEARS(temp_year);

        retval =  ((WDAY0 + totaldays) % 7);

        return (cur_days - retval);
 }



/***
 * Given a struct tm, calculate which day of the year it falls on.
 */
static int near pascal _calc_yday(struct tm *t)
{
   int retval = _mdays[t->tm_mon] + t->tm_mday -1;

   if (ISLEAP(t->tm_year+1900) && t->tm_mon > 1)
      retval++;

   return retval;
}


/*
 * internal tzset used to call tzset.
 * Internally in the RTL, we want to only call
 * the system once for each time call.  This will
 * check for this.
 */
void __internal_tzset()
{
        static unsigned long __tzset_called = 0;

        if (__tzset_called == 0)
        {
                tzset();
                __tzset_called++;
        }
}




