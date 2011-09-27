/*_ stream.cpp   Sun Jan 17 1988   Modified by: Walter Bright */

#include	<oldstr\stream.h>

int streambuf::underflow()
{
	return EOF;
}

streambuf::streambuf()
{
    fp = 0;
    base = gptr = eptr = pptr = 0;
    alloc = 0;
}

streambuf::streambuf(char* buf, int buflen)
{
    setbuf(buf,buflen);
    alloc = 0;
    fp = 0;
}

streambuf::~streambuf()
{
    if (alloc)
	delete base;
}

streambuf *streambuf::setbuf(char *buf, int buflen, int written, int wasalloc)
{
    base = buf;
    gptr = base;
    pptr = base + written;
    eptr = base + buflen;
    alloc = wasalloc;
    return this;
}

int streambuf::overflow(int c)
{
    if (allocate() == EOF || c == EOF)
	return EOF;
    *pptr++ = c;
    return c & 0xFF;
}

int streambuf::doallocate()
{
    base = new char[BUFSIZE];
    if (base == 0)
	return EOF;
    gptr = base;
    pptr = base;
    eptr = base + BUFSIZE;
    alloc = 1;
    return 0;
}
