/*_ strftime.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <tchar.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <locale.h>
#include <setlocal.h>
#include <sctchar.h>

# ifndef _NOSTRFTIME

static char *	fmampm[] = {	/* kind of AM/PM format */
  "am/pm",
  "a/p",
  NULL
};

/********************************************
 * Compute ISO 8601 year and week from tm.
 */

static int Jan1Day(int year)
{   int d;

    // Return number of days of Jan 1 since Jan 1, 1970
    d = (int) (365 * (year - 1970) +
              (int)((year - 1969) / 4.0) -
              (int)((year - 1901) / 100.0) +
              (int)((year - 1601) / 400.0));
    return d;
}

static void week_based_year(const struct tm *t, int *pyear, int *pweek)
{   int year = t->tm_year + 1900;
    int yday = t->tm_yday;
    int d;
    int w;
    int ydaybeg;

    /* Determine day of week Jan 4 falls on.
     * Weeks begin on a Monday.
     */

    d = Jan1Day(year);
    w = (d + 3/*Jan4*/ + 3) % 7;
    if (w < 0)
	w += 7;

    /* Find yday of beginning of ISO 8601 year
     */
    ydaybeg = 3/*Jan4*/ - w;

    /* Check if yday is actually the last week of the previous year
     */
    if (yday < ydaybeg)
    {
	*pyear = year - 1;
	*pweek = 53;
	return;
    }

    /* Check if yday is actually the first week of the next year
     */
    if (yday >= 362)				// possible
    {	int d2;
	int ydaybeg2;

	d2 = Jan1Day(year + 1);
	w = (d2 + 3/*Jan4*/ + 3) % 7;
	if (w < 0)
	    w += 7;
	//printf("w = %d\n", w);
	ydaybeg2 = 3/*Jan4*/ - w;
	if (d + yday >= d2 + ydaybeg2)
	{
	    *pyear = year + 1;
	    *pweek = 1;
	    return;
	}
    }

    *pyear = year;
    *pweek = (yday - ydaybeg) / 7 + 1;
}

typedef struct {
  char *	buf;
  size_t	cnt;
  int		err;
  char		sup;
  char		E,O;
} TallyCount;

static void ConvTime(TallyCount *tc, char key, const struct tm *t);

static void TallyError(TallyCount *tc)
{
    tc->err = 1;
    tc->cnt = 0;
}

static void PutChar(TallyCount *tc, char c)
{
    if (tc->cnt)
    {
	*tc->buf = c;
	tc->buf++;
	tc->cnt--;
    }
}

static void PutInt(TallyCount *tc, int value, int digits)
{
    int i;
    int n;
    char buf[32];

    if (tc->err == 0)
    {
	i = 32;

	do
	{
	    buf[--i] = (char) ('0' + value % 10);
	    value /= 10;
	} while (value != 0);

	if (!tc->sup)
	{   /* padding leading zero */
	    while (32 - i < digits)
	    {
		buf[--i] = '0';
	    }
	}
	n = 32 - i;
	if (tc->cnt <= n)	// allow room for terminating 0
	    TallyError(tc);
	else
	{
	    memcpy(tc->buf, &buf[i], n);
	    tc->buf += n;
	    tc->cnt -= n;
	}
    }
}

static void PutStr (TallyCount *tc, char *s)
{
    if (tc->err == 0)
    {
	while (tc->cnt > 0 && *s != '\0')
	{
	    *tc->buf = *s++;
	    tc->buf++;
	    tc->cnt--;
	}
    }
}

static void PutMem (TallyCount *tc, const char *s, size_t sz)
{
    if (tc->err == 0)
    {
	while (tc->cnt > 0 && sz-- > 0)
	{
	    *tc->buf = *s++;
	    tc->buf++;
	    tc->cnt--;
	}
    }
}

static void PutDateTime (TallyCount *tc, const char *f, const struct tm *t)
{
    char		key;
    const char *	p;
    int			step;
    int			i;
    size_t		sz;

    while (tc->cnt > 0 && *f != '\0')
    {
	key = 0;
	p = f;
	for (step = 0; *p == *f; step++)
	{
	    p++;
	}
	switch (*f)
	{
	    case 'A':
	    case 'a':
		i = 0;
		while (fmampm[i] != NULL)
		{
		    sz = strlen (fmampm[i]);
		    if (_strnicmp (f, fmampm[i], sz) == 0)
		    {
			p = f + sz;
			key = 'p';		/* AM or PM */
			break;
		    }
		    i++;
		}
		break;
	    case 'd':
		switch (step)
		{
		    case 1:
		    case 2:
			key = 'd';		/* day of month as 01..31 */
			break;
		    case 3:
			key = 'a';		/* abbreviated weekday name */
			break;
		    case 4:
		    default:
			key = 'A';		/* full weekday name */
			break;
		}
		break;
	    case 'H':
		switch (step)
		{
		    case 1:
		    case 2:
		    default:
			key = 'H';		/* hour as 00..23 */
			break;
		}
		break;
	    case 'h':
		switch (step)
		{
		    case 1:
		    case 2:
		    default:
			key = 'I';		/* hour as 01..12 */
			break;
		}
		break;
	    case 'M':
		switch (step)
		{
		    case 1:
		    case 2:
			key = 'm';		/* month as 01..12 */
			break;
		    case 3:
			key = 'b';		/* abbreviated month name */
			break;
		    case 4:
		    default:
			key = 'B';		/* full month name */
			break;
		}
		break;
	    case 'm':
		switch (step)
		{
		    case 1:
		    case 2:
		    default:
			key = 'M';		/* minute as 00..59 */
			break;
		}
		break;
	    case 's':
		switch (step)
		{
		    case 1:
		    case 2:
		    default:
			key = 'S';		/* second as 00..59 */
			break;
		}
		break;
	    case 'y':
		switch (step)
		{
		    case 2:
			key = 'y';		/* year of century (00..99) */
			break;
		    case 4:
		    default:
			key = 'Y';		/* year with century */
			break;
		}
		break;
	    case '\'':
		if (step & 1)
		{
		    f += step;
		    while (tc->cnt > 0 && *f != '\0')
		    {
			if (*f == '\'')
			{
			    f++;
			    break;
			}
			if (isleadbyte ((int)*f))
			{
			    if (tc->cnt < MB_LEN_MAX)
			    {
				TallyError (tc);
				break;
			    }
			    PutMem (tc, f++, 1);
			}
			PutMem (tc, f++, 1);
		    }
		}
		else
		{
		    f += step;
		}
		continue;
	    default:
		break;
	}

	if (key != 0)
	{
	    ConvTime (tc, key, t);
	    f = p;
	}
	else
	{
	    if (isleadbyte ((int)*f))
	    {
		if (tc->cnt < MB_LEN_MAX)
		{
		    TallyError (tc);
		    break;
		}
		PutMem (tc, f++, 1);
	    }
	    PutMem (tc, f++, 1);
	}
    }
}

static void ConvTime (TallyCount *tc, char key, const struct tm *t)
{
    switch (key)
    {
	case 'A':	/* full weekday name */
	    PutStr (tc, __locale_time->lgweek[t->tm_wday]);
	    break;
	case 'a':	/* abbreviated weekday name */
	    PutStr (tc, __locale_time->shweek[t->tm_wday]);
	    break;
	case 'B':	/* full month name */
	    PutStr (tc, __locale_time->lgmont[t->tm_mon]);
	    break;
	case 'h':	// equivalent to %b
	case 'b':	/* abbreviated month name */
	    PutStr (tc, __locale_time->shmont[t->tm_mon]);
	    break;
	case 'C':	// year divided by 100, 00..99
	    PutInt (tc, (t->tm_year / 100) % 100, 2);
	    break;
	case 'c':	/* date and time representation */
	    if (tc->sup)
	    {
		tc->sup = FALSE;
		PutDateTime (tc, __locale_time->lgdate, t);
	    }
	    else
	    {
		PutDateTime (tc, __locale_time->shdate, t);
	    }
	    PutStr (tc, " ");
	    PutDateTime (tc, __locale_time->lytime, t);
	    break;
	case 'D':	// "%m/%d/%y"
	    PutInt (tc, t->tm_mon + 1, 2);
	    PutChar(tc, '/');
	    PutInt (tc, t->tm_mday, 2);
	    PutChar(tc, '/');
	    PutInt (tc, t->tm_year % 100, 2);
	    break;
	case 'd':	/* day of month as 01..31 */
	    PutInt (tc, t->tm_mday, 2);
	    break;
	case 'e':	/* day of month as ' 1'..'31' */
	    if (t->tm_mday < 10)
		PutChar(tc, ' ');
	    PutInt (tc, t->tm_mday, 1);
	    break;
	case 'F':	// "%Y-%m-%d"
	    PutInt (tc, t->tm_year + 1900, 4);
	    PutChar(tc, '-');
	    PutInt (tc, t->tm_mon + 1, 2);
	    PutChar(tc, '-');
	    PutInt (tc, t->tm_mday, 2);
	    break;

	case 'G':	// ISO 8601 week based year
	case 'g':	// ISO 8601 week based year 00..99
	case 'V':	// ISO 8601 week number 01..53
	{   int week;
	    int year;

	    week_based_year(t, &year, &week);
	    switch (key)
	    {	case 'g':
		    PutInt(tc, year % 100, 2);
		    break;
		case 'G':
		    PutInt(tc, year, 1);
		    break;
		case 'V':
		    PutInt (tc, week, 2);
		    break;
	    }
	    break;
	}

	case 'H':	/* hour as 00..23 */
	    PutInt (tc, t->tm_hour, 2);
	    break;
	case 'I':	/* hour as 01..12 */
	    PutInt (tc, (t->tm_hour + 11) % 12 + 1, 2);
	    break;
	case 'j':	/* day of year as 001..366 */
	    PutInt (tc, t->tm_yday + 1, 3);
	    break;
	case 'M':	/* minute as 00..59 */
	    PutInt (tc, t->tm_min, 2);
	    break;
	case 'm':	/* month as 01..12 */
	    PutInt (tc, t->tm_mon + 1, 2);
	    break;
	case 'n':	// new-line character
	    PutChar(tc, '\n');
	    break;
	case 'p':	/* AM or PM */
	    PutStr (tc, __locale_time->lyampm[t->tm_hour / 12]);
	    break;
	case 'R':	// "%H:%M"
	    PutInt (tc, t->tm_hour, 2);
	    PutChar(tc, ':');
	    PutInt (tc, t->tm_min, 2);
	    break;

	case 'r':	// locale's 12 hour clock time
	    // BUG: use "%I:%M:%S%p", should use LOCALE_STIMEFORMAT
	    PutInt (tc, (t->tm_hour + 11) % 12 + 1, 2);
	    PutChar(tc, ':');
	    PutInt (tc, t->tm_min, 2);
	    PutChar(tc, ':');
	    PutInt (tc, t->tm_sec, 2);
	    PutStr (tc, __locale_time->lyampm[t->tm_hour / 12]);
	    break;

	case 'S':	/* second as 00..59 */
	    PutInt (tc, t->tm_sec, 2);
	    break;
	case 'T':	// "%H:%M:%S"
	    PutInt (tc, t->tm_hour, 2);
	    PutChar(tc, ':');
	    PutInt (tc, t->tm_min, 2);
	    PutChar(tc, ':');
	    PutInt (tc, t->tm_sec, 2);
	    break;
	case 't':	// tab character
	    PutChar(tc, '\t');
	    break;
	case 'u':	// weekday as 1..7, where Monday is 1
	{   int w = t->tm_wday;
	    if (w == 0)
		w = 7;
	    PutInt (tc, w, 1);
	    break;
	}
	case 'U':	/* week as 00..53 (1st Sunday is 1st day of week 1) */
	    PutInt (tc, (t->tm_yday - t->tm_wday + 7) / 7, 2);
	    break;
	case 'W':	/* week as 00..53 (1st Monday is 1st day of week 1) */
	    PutInt (tc, (t->tm_yday - ((t->tm_wday + 6) % 7) + 7) / 7, 2);
	    break;
	case 'w':	/* weekday as 0(Sunday)..6 */
	    PutInt (tc, t->tm_wday, 1);
	    break;
	case 'X':	/* locale time representation */
	    tc->sup = FALSE;
	    PutDateTime (tc, __locale_time->lytime, t);
	    break;
	case 'x':	/* locale date representation */
	    if (tc->sup)
	    {
		tc->sup = FALSE;
		PutDateTime (tc, __locale_time->lgdate, t);
	    }
	    else
	    {
		PutDateTime (tc, __locale_time->shdate, t);
	    }
	    break;
	case 'y':	/* year of century (00..99) */
	    PutInt (tc, t->tm_year % 100, 2);
	    break;
	case 'Y':	/* year with century */
	    PutInt (tc, t->tm_year + 1900, 4);
	    break;
	case 'Z':	/* time zone name or abbrev */
	case 'z':
	    _tzset();
	    PutStr (tc, _tzname[t->tm_isdst ? 1 : 0]);
	    break;
	case '%':	/* character '%' */
	    PutChar(tc, '%');
	    break;
	default:
	    TallyError (tc);
	    break;
    }
}

size_t __CLIB strftime(char *s, size_t lsz, const char *f, const struct tm *t)
{
    TallyCount tc;
    memset(&tc, 0, sizeof(tc));
    tc.buf = s;
    tc.cnt = lsz;

    while (tc.cnt > 0 && *f != '\0')
    {
	if (*f == '%')
	{
	    f++;
	    tc.sup = FALSE;
	    if (*f == '#')
	    {
		f++;
		tc.sup = TRUE;
	    }
	    if (*f == 'E')
	    {
		tc.E = 1;
		f++;
	    }
	    else if (*f == 'O')
	    {
		tc.O = 1;
		f++;
	    }
	    ConvTime(&tc, *f, t);
	    f++;
	}
	else
	{
	    if (isleadbyte((int)(*f)))
	    {
		if (tc.cnt < MB_LEN_MAX)
		{
		    TallyError(&tc);
		    break;
		}
		PutMem(&tc, f++, 1);
	    }
	    PutMem(&tc, f++, 1);
	}
    }
    if (tc.cnt > 0)
    {
	*tc.buf = '\0';
	return lsz - tc.cnt;	// return count not including terminating 0
    }
    return 0;
}

# endif

#else

/*_ strftime.c	Sun Apr 16 1989 Modified by: Walter Bright */
/* Copyright (C) 1986-1994 by Digital Mars	*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/

#include	<tchar.h>
#include	<sctchar.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<time.h>
#include	<errno.h>
#include	<string.h>

#if _MT
#include	"mt.h"
#endif

#define PUT(c)	{ if (nwritten >= maxsize)	\
			goto error;		\
		  s[nwritten++] = (c);		\
		}

#define PUTA(s,l) p = (s); length = (l); goto puta
#define PUTS(s) p = (s); goto puts
#define PUT1(v) n = (v); goto put1
#define PUT2(v) n = (v); goto put2
#define PUT3(v) n = (v); goto put3
#define PUT4(v) n = (v); goto put4

size_t strftime(char *s, size_t maxsize, const char *format,
	const struct tm *timeptr)
{	char c;
	char *p;
	int n;
	size_t length;
	char buf[25];
	size_t nwritten = 0;
	static char fulldays[7][10] =
	{ "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday" };
	static char months[12][10] =
	{ "January","February","March","April","May","June","July","August",
	  "September","October","November","December" };
	while ((c = *format++) != 0)
	{
	if (c == '%')
	{
		switch (*format++)
		{
		case 'a':		/* abbreviated weekday name */
			PUTA(fulldays[timeptr->tm_wday],3);
			break;
		case 'A':		/* full weekday name		*/
			PUTS(fulldays[timeptr->tm_wday]);
			break;
		case 'b':		/* abbreviated month name	*/
			PUTA(months[timeptr->tm_mon],3);
			break;
		case 'B':		/* full month name		*/
			PUTS(months[timeptr->tm_mon]);
			break;
		case 'c':		/* date and time representation */
			p = "%a %b %d %X %Y";
			goto strf;
		case 'd':		/* day of month as 01..31	*/
			PUT2(timeptr->tm_mday);
			break;
		case 'H':		/* hour as 00..23		*/
			PUT2(timeptr->tm_hour);
			break;
		case 'I':		/* hour as 01..12		*/
			n = timeptr->tm_hour - 1;
			if (n < 0)
			n = 24;
			PUT2((n % 12) + 1);
			break;
		case 'j':		/* day of year as 001..366	*/
			PUT3(timeptr->tm_yday + 1);
			break;
		case 'm':		/* month as 01..12		*/
			PUT2(timeptr->tm_mon + 1);
			break;
		case 'M':		/* minute as 00..59		*/
			PUT2(timeptr->tm_min);
			break;
		case 'p':		/* AM or PM			*/
			PUTA((timeptr->tm_hour < 12 ? "AM" : "PM"),2);
			break;
		case 'S':		/* second as 00..59		*/
			PUT2(timeptr->tm_sec);
			break;
		case 'U':		/* week as 00..53 (1st Sunday is 1st day of week 1) */
			PUT2((timeptr->tm_yday - timeptr->tm_wday + 7) / 7);
			break;
		case 'w':		/* weekday as 0(Sunday)..6	*/
			PUT1(timeptr->tm_wday);
			break;
		case 'W':		/* week as 00..53 (1st Monday is 1st day of week 1) */
			PUT2((timeptr->tm_yday - ((timeptr->tm_wday + 6) % 7) + 7) / 7);
			break;
		case 'x':		/* appropriate date representation */
			p = "%d-%b-%y";
			goto strf;
		case 'X':		/* appropriate time representation */
			p = "%H:%M:%S";
		strf:
			if (strftime(buf,sizeof(buf),p,timeptr) == 0)
			goto error;
			PUTS(buf);
			break;
		case 'y':		/* year of century (00..99) */
			PUT2(timeptr->tm_year % 100);
			break;
		case 'Y':		/* year with century		*/
			PUT4(timeptr->tm_year + 1900);
			break;
		case 'Z':		/* time zone name or abbrev */
			PUTS((timeptr ->tm_isdst) ? tzname[1] : tzname[0]);
			break;
		case '%':
			PUT('%');
			break;
		}
	}
	else
		PUT(c);
	continue;
put1:	length = 1;
	goto putn;
put2:	length = 2;
	goto putn;
put3:	length = 3;
	goto putn;
put4:	length = 4;
putn:	buf[0] = (n / 1000) + '0';
	buf[1] = ((n / 100) % 10) + '0';
	buf[2] = ((n / 10) % 10) + '0';
	buf[3] = (n % 10) + '0';
	p = buf + 4 - length;
	goto puta;
puts:	length = strlen(p);
puta:	if (nwritten + length >= maxsize)
		goto error;
	memcpy(s + nwritten,p,length);
	nwritten += length;
	}
	if (nwritten >= maxsize)
	goto error;
	s[nwritten] = 0;
	return nwritten;		/* exclude terminating 0 from count */
error:
	return 0;
}

#endif

#ifndef _NOSTRDATE

_TCHAR *_tstrdate(_TCHAR *datestr)
{
	if (datestr)
	{
	time_t t;
	struct tm* ptm;
	time(&t);
	ptm = localtime(&t);
	_stprintf(datestr, _T("%02d/%02d/%02d"), ptm->tm_mon+1,
					 ptm->tm_mday,
					 (ptm->tm_year % 100));
	}
	return datestr;
}

#endif

#ifndef _NOSTRTIME

_TCHAR *_tstrtime(_TCHAR *timestr)
{
	if (timestr)
	{
	time_t t;
	struct tm* ptm;
	time(&t);
	ptm = localtime(&t);
	_stprintf(timestr, _T("%02d:%02d:%02d"), ptm->tm_hour,
					 ptm->tm_min,
					 ptm->tm_sec);
	}
	return timestr;
}

#endif

/**/

#if 0
#include <stdio.h>
#include <assert.h>

void main()
{
    struct tm tm;
    int year, week;
    int i;

    struct Vectors
    {	int tm_year;
	int tm_yday;
	int year;
	int week;
    };

    static struct Vectors vectors[] =
    {
	{ 1998, 364,	1998,53	},	// Thu Dec 31
	{ 1999, 0,	1998,53	},	// Fri Jan 1
	{ 1999, 1,	1998,53	},	// Sat Jan 2
	{ 1999, 2,	1998,53	},	// Sun Jan 3
	{ 1999, 3,	1999,1	},	// Mon Jan 4
	{ 1999, 4,	1999,1	},	// Tue Jan 5
	{ 1999, 5,	1999,1	},	// Wed Jan 6
	{ 1999, 6,	1999,1	},	// Thu Jan 7

	{ 1997, 361,	1997,52	},	// Sun Dec 28
	{ 1997, 362,	1998,1	},	// Mon Dec 29
	{ 1997, 363,	1998,1	},	// Tue Dec 30
	{ 1997, 364,	1998,1	},	// Wed Dec 31
	{ 1998, 0,	1998,1	},	// Thu Jan 1
	{ 1998, 1,	1998,1	},	// Fri Jan 2
	{ 1998, 2,	1998,1	},	// Sat Jan 3
	{ 1998, 3,	1998,1	},	// Sun Jan 4
	{ 1998, 4,	1998,2	},	// Mon Jan 5
    };


    for (i = 0; i < sizeof(vectors)/sizeof(vectors[0]); i++)
    {
	tm.tm_year = vectors[i].tm_year;
	tm.tm_yday = vectors[i].tm_yday;
	week_based_year(&tm, &year, &week);
	printf("year = %d, week = %d\n", year, week);
	assert(year == vectors[i].year && week == vectors[i].week);
    }
}
#endif
