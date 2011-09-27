/*_ timdata.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#include <time.h>
#include <locale.h>
#include <setlocal.h>

/* LC_TIME data for local default "C"
*/
LCTIME		__locale_time_const = {
  {	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
  },
  {	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
  },
  {	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
  },
  {	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
  },
  {	"dddd, MMMM dd, yyyy"
  },
  {	"M/d/yy"
  },
  {	"AM",
	"PM"
  },
  {	"H:mm:ss"
  }
};

/* Points to the current LC_TIME data structure.
*/
LCTIME *	__locale_time = &__locale_time_const;

#endif

