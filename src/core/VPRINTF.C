/*_ vprintf.c   Mon Jun 6, 1994 */
/* Copyright (C) 1994 by Digital Mars		*/
/* All Rights Reserved				*/

#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<scio.h>




/***********************
 */

int vprintf(const char *format,va_list args)
{   int status;

    __fp_lock(stdout);
    status = _pformat((fp_t)_fputc_nlock,stdout,format,args);
    __fp_unlock(stdout);
    if (stdout->_flag & _IOLBF)
	    fflush(stdout);
    return status;
}
