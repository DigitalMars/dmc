/*_ flushwbu.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

#define _UNICODE	1

#include <tchar.h>
#include <sctchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#if MSDOS || __OS2__ || _WINDOWS || __NT__
#include <dos.h>
#endif
#include "mt.h"

#define _NOFILLWBUF	1

#include "flush.c"

#endif

/**/
