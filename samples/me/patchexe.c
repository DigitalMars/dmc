/*_ patchexe.c   Wed Jun 15 1988 */
/* Module to patch .COM and .EXE files			*/

#if M_I86SM
#include	<string.h>
#endif

#include	<stdlib.h>
#include	<dos.h>
#include	<io.h>
#include	<fcntl.h>

/*****************************
 * Patch .EXE or .COM file.
 * Currently only works for 16 bit DOS executables.
 * Doesn't work if data to be patched contains relocatable values.
 * Input:
 *	progname	Name of program to patch
 *	data		Pointer to data (for T,S,M models, this is the
 *			offset from DS)
 *	size		Number of bytes to patch
 * Returns:
 *	0	Success
 *	!=0	Failure
 */

typedef unsigned short unsigned16;	/* unsigned 16 bit integer	*/

int patchexe(char *progname,void *data,unsigned size)
{
#if MSDOS && __INTSIZE == 2
    int fd;
    unsigned seg;
    unsigned long offset;
#if M_I86SM
    int len;
    int comfile;

    len = strlen(progname);
    if (len < 4)
	goto fail;
    comfile = strcmpl(progname + len - 3,"exe");
#endif
    fd = open(progname,_O_RDWR);
    if (fd == -1)
	goto fail;
    seg = getDS() - _psp - 0x10;
#if M_I86SM
    if (comfile)
    {
	if (seg >= 0x1000)
	    goto failclose;
    }
    else
#endif
    {   unsigned16 header[13];		/* buffer for EXE file header	*/

	if (read(fd,header,sizeof(header)) != sizeof(header))
	    goto failclose;
	if (header[0] != 0x5A4D)	/* check magic number		*/
	    goto failclose;

	seg += header[4];		/* offset to start of load module */
    }
    offset = ((unsigned long) seg << 4) + (unsigned) data;
    if (lseek(fd,offset,SEEK_SET) == -1L)
	goto failclose;
    if (write(fd,data,size) != size)
	goto failclose;
    if (close(fd) == -1)
	goto fail;

    return 0;			/* success		*/

failclose:
    close(fd);
fail:
#endif
    return -1;
}

/******************* TEST PROGRAM ****************/

#if TEST

#include	<stdio.h>
main(int argc,char *argv[])
{
    static int val = 0;

    printf("%s\n",argv[0]);
    printf("val = %d\n",val);
    val++;
    if (patchexe(argv[0],&val,sizeof(val)))
	printf("Failed\n");
    else
	printf("Succeeded\n");
}

#endif
