/*_ filebuf.cpp   Fri Mar  2 1990   Modified by: Walter Bright */
/* Implementation of member functions for class filebuf	*/

#include	<oldstr\stream.h>
#include	<io.h>
#include	<fcntl.h>
#include	<string.h>
#if M_UNIX || M_XENIX
#include        <sys/stat.h>
#else
#include        <sys\stat.h>
#endif

/******************************
 * Open a file.
 * Returns:
 *	this	success
 *	NULL	failure
 */

filebuf *filebuf::open(char *name,open_mode om)
{
    switch (om)
    {
	case input:
#if M_UNIX || M_XENIX
            fd = ::open(name,O_RDONLY); // call C standard open()
#else
            fd = ::open(name,O_RDONLY|O_BINARY); // call C standard open()
#endif
	    break;
	case append:
#if M_UNIX || M_XENIX
		fd = ::open(name,O_RDWR);
#else
		fd = ::open(name,O_RDWR|O_BINARY);
#endif
	    if (fd == -1)
		goto L1;		// try creating it
	    lseek(fd,0L,SEEK_END);	// to end of file
	    break;
	case output:
	L1:
	    fd = creat(name,S_IREAD | S_IWRITE);
	    break;
    }
    if (fd == -1)
	return NULL;
    opened = 1;			// non-zero indicating file is open
    return this;
}

/////////////////////////////////////////

int filebuf::close()
{   int result = 0;

    if (opened)
    {
        overflow();
        opened = 0;
	if (!fp)
	    result = ::close(fd);
    }
    return result;
}

/*************************
 * Empty buffer.
 * Returns:
 *	0	success
 *	EOF	failure
 */

int filebuf::overflow(int c)
{
    if (opened == 0)			/* if file is not open		*/
	goto fail;
    if (fp)
	return fflush(fp);
    if (base == 0)			/* if no buffer			*/
    {	if (doallocate() == EOF)	/* try to allocate a buffer	*/
	    goto fail;
    }
    if (base == eptr)			// if allocate failed
    {	if (c != EOF)
	{
	    if (write(fd,&c,1) != 1)
		goto fail;
	}
    }
    else
    {
	if (pptr > base)
	{
	    if (write(fd,base,pptr - base) != pptr - base)
		goto fail;
	}
	gptr = base;
	pptr = base;
	if (c == EOF)
	    goto fail;
	pptr++;
	*base = c;
    }
    return (unsigned char) c;

fail:
    return EOF;
}

/*************************
 * Fill buffer.
 * Returns:
 *	EOF	failure
 *	next char on success
 */

int filebuf::underflow()
{
    int len;

    if (opened == 0)
	goto fail;
    if (allocate() == EOF)
	goto fail;
    len = eptr - base - 1;
    if (fp)
    {	if (fgets(base + 1,len,fp) == 0)
	    goto fail;
	len = strlen(base + 1);
    }
    else
    {	len = read(fd,base + 1,len);
	if (len < 1)
	    goto fail;
    }
    gptr = base + 1;		// leave room for a 'putback'
    pptr = gptr + len;
    return (unsigned char) *gptr;

fail:
    return EOF;
}

/////////////////////////////////////////
// Constructors

filebuf::filebuf()		{ fp = 0; opened = 0; }
filebuf::filebuf(FILE* p)	{ fp = p; opened = 1; }
filebuf::filebuf(int _fd)	{ fp = 0; fd = _fd; opened = 1; }
filebuf::filebuf(int _fd, char *buf, int buflen) 
	: (buf,buflen)
{
    fp = 0;
    fd = _fd;
    opened = 1;
}

////////////////////////////////
// Destructor

filebuf::~filebuf() { close(); }



