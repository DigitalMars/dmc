/*
	stockutl.h

	Miscellaneous utility functions for the Simulated Stock Watcher OLE2
	sample program

	(c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved
*/

#ifndef __STOCKUTL_H
#define __STOCKUTL_H

#include <windows.h>    //to get LPSTR definition--this maybe overkill ;)
long Str2Stock(LPSTR szStock);
LPSTR Stock2Str( long n );

#endif
