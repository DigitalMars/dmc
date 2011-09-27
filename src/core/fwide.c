
#include <stdio.h>
#include <wchar.h>

#include "mt.h"
#include "scio.h"

/**************************************
 * C99 7.24.3.5
 */

int fwide(FILE *fp, int mode)
{
    int fd;
    unsigned char *p;
    int result;

    __fp_lock(fp);

    fd = fileno(fp);
    p = &__fhnd_info[fd];

    if (!(*p & (FHND_WCHAR | FHND_BYTE)))
    {
	if (mode < 0)
	{
	    *p |= FHND_BYTE;
	}
	else if (mode > 0)
	{
	    *p |= FHND_WCHAR;
	}
    }

    if (*p & FHND_BYTE)
	result = -1;
    else if (*p & FHND_WCHAR)
	result = 1;
    else
	result = 0;

    __fp_unlock(fp);
    return result;
}
