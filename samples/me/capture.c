/*_ capture.c   Mon Oct 31 1988   */
/* Written by Walter Bright				*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<dos.h>
#include	<int.h>

#include	"capture.h"

static unsigned buffersize;
static unsigned used;
static char *buffer;
static capture_inited = 0;

/********************************
 * Look for writes to the screen, and copy them to our
 * own buffer before passing them on to MS-DOS.
 */

#if MSDOS

static int handler(struct INT_DATA *pd)
{   unsigned count;

    if (pd->regs.h.ah == 0x40 &&		/* write to handle	*/
	(pd->regs.x.bx == 1 || pd->regs.x.bx == 2))	/* stdout or stderr */
    {
	sound_click();
	count = pd->regs.x.cx;
	if (used + count > buffersize)
	    count = buffersize - used;
	if (count)
	{
#if __INTSIZE == 2
	    peekbytes(pd->sregs.ds,pd->regs.x.dx,buffer + used,count);
#endif
	    used += count;
	}
    }

    return 0;		/* chain to previous interrupt handler	*/
}

#endif

/*******************************
 * Setup for capturing MS-DOS output.
 * If successful, all MS-DOS output is copied into an internal buffer.
 * Before the program terminates, capture_term() must be called to
 * finish up things and unhook the vectors.
 * Input:
 *	size	Buffer size to use (if 0, use default size of 10000)
 * Returns:
 *	!=0	succeeded
 */

int capture_init(unsigned size)
{
#if MSDOS
    int status;

    buffersize = size ? size : 10000;
    buffer = malloc(buffersize);
    if (buffer)
    {	if (int_intercept(0x21,handler,1024) == 0)
	{   capture_inited = 1;
	    used = 0;
	    return 1;
	}
	free(buffer);
    }
#endif
    return 0;			/* failed		*/
}

/******************************
 * Terminate capturing MS-DOS output.
 * Return a buffer to the buffer of data.
 * Return in *pcount the number of bytes in the buffer.
 * The caller must free the buffer.
 */

char *capture_term(unsigned *pcount)
{
#if MSDOS
    if (capture_inited)
    {
	int_restore(0x21);
	capture_inited = 0;
    }
    else
    {	buffer = NULL;
	used = 0;
    }
    *pcount = used;
    return buffer;
#else
    return NULL;
#endif
}
