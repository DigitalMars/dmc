/*_ atof.c   Thu May 25 1989   Modified by: Walter Bright */
/* Copyright (C) 1985-1993 by Digital Mars	*/
/* All Rights Reserved					*/

#include	<stdlib.h>

/*************************
 * Convert string to double.
 * Terminates on first unrecognized character.
 */

double atof(const char *p)
{   int errnosave = errno;
    double d;

#if _WIN32
    d = strtold(p,(char **)NULL);
#else
    d = strtod(p,(char **)NULL);
#endif
    errno = errnosave;
    return d;
}
