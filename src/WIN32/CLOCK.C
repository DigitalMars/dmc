/*_ clock.c   Modified by: Walter Bright
 * Copyright (C) 1991-1992 by Walter Bright
 * All Rights Reserved
 * Written by Walter Bright
 */

#include <time.h>

#if __OS2__
#include <os2lib.h>
#endif
#if __NT__
#include <windows.h>
#endif

static clock_t starttime;

/****************************
 * Return the time in 1/100ths of a second since program start.
 * Useful for timing various things.
 */

clock_t clock()
{
#if __OS2__
    clock_t c;
    DateTime datetime;

    DosGetDateTime(&datetime);

    c = 10L * (datetime.hundredths +
	100L * (datetime.seconds +
	60L * (datetime.minutes +
	60 * datetime.hours)));

    if (c < starttime)          /* rolled past midnight                 */
	/* Add in one day's worth of CLK_TCKs   */
	c -= starttime + (1000L * 60 * 60 * 24);
    else
	c -= starttime;
    return c;
#endif
#if __NT__
    clock_t c;

    c = (clock_t)(GetTickCount());
#if _NOCTOR
    if (starttime == 0)                 /* if not initialized   */
	starttime = c;
#endif
    return c - starttime;
#endif
}

/****************************
 * Initializer for clock(), it sets starttime to time that the program
 * was started.
 */

#if !_NOCTOR

void _STI_clock()
{
    starttime = clock();
}

#endif
