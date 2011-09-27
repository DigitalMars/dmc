/*_ sleep.c      Sun Apr 16 1989   Modified by: Walter Bright */
/* Copyright (C) 1986-1994 by Digital Mars  */
/* All Rights Reserved                                  */
/* Written by Walter Bright                             */

#include        <stdlib.h>
#include        <stdio.h>
#include        <time.h>
#include        <errno.h>
#include        <string.h>

#if _MT
#include        "mt.h"
#endif


#if __NT__

#include <windows.h>
//void Sleep(unsigned long);

void sleep(time_t seconds)
{
	Sleep(seconds * 1000L);
}

void msleep(unsigned long milliseconds)
{
	Sleep(milliseconds);
}

void usleep(unsigned long microseconds)
{
	Sleep(microseconds / 1000L);
}

#elif __OS2__

#if __INTSIZE == 4
unsigned _syscall DosSleep(unsigned long);
#else
unsigned __far __pascal DosSleep(unsigned long);
#endif

void sleep(time_t seconds)
{
	DosSleep(seconds * 1000l);
}

void msleep(unsigned long milliseconds)
{
	DosSleep(milliseconds);
}

void usleep(unsigned long microseconds)
{
	DosSleep(microseconds / 1000L);
}

#elif _WINDOWS

#include <windows.h>

void sleep(time_t seconds)
{       time_t endtime;

	if (seconds == 0)
	  {
	  Yield();      /* yield to other Windows programs      */
	  return;
	  }
	endtime = time(0) + seconds;
	while (time(0) < endtime)
		Yield();
}

void msleep(unsigned long milliseconds)
{       clock_t endtime;
	clock_t starttime;
	clock_t present;

	starttime = clock();
	endtime = starttime + milliseconds / (1000 / CLK_TCK);
	while (1)
	 {
		present = clock();
		if (present < starttime)
			present += 86400000;
		if (present >=endtime) break;
	}
}

void usleep(unsigned long microseconds)
{       clock_t endtime;
	clock_t starttime;
	clock_t present;

	starttime = clock();
	endtime = starttime + microseconds / (1000000 / CLK_TCK);
	while (1)
	 {
		present = clock();
		if (present < starttime)
			present += 86400000;
		if (present >=endtime) break;
	}
}

#else

void sleep(time_t seconds)
{       time_t endtime;

	endtime = time(0) + seconds;
	while (time(0) < endtime)
	;
}

void msleep(unsigned long milliseconds)
{       clock_t endtime;
	clock_t starttime;
	clock_t present;

	starttime = clock();
	endtime = starttime + milliseconds / (1000 / CLK_TCK);
	while (1)
	{
		present = clock();
		if (present < starttime)
			present += 86400000;
		if (present>=endtime) break;
	}
}


void usleep(unsigned long microseconds)
{       clock_t endtime;
	clock_t starttime;
	clock_t present;

	starttime = clock();
	endtime = starttime + microseconds / (1000000 / CLK_TCK);
	while (1)
	{
		present = clock();
		if (present < starttime)
			   present += 86400000;
		if (present >=endtime) break;
	}
}
#endif

