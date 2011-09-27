/*_ filelen.c   Modified by: Walter Bright
 * Copyright (C) 1991 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#include <io.h>

long filelength(int fd)
{
    long curpos;
    long length;

    /* Determine file size using lseek() */
    curpos = lseek(fd,0L,SEEK_CUR);
    if (curpos == -1L)
	length = -1L;			/* errno was set by lseek()	*/
    else
    {	length = lseek(fd,0L,SEEK_END);
	lseek(fd,curpos,SEEK_SET);
    }
    return length;
}
