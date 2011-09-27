// IOStreams Package
// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1992. All Rights Reserved.

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <dos.h>
#include <io.h>
#include <iosold\strstrea.h>
#include <typeinfo.h>

void strstreambuf::buffer_setup(char *memory, int length,
				    char *start_of_put_area)
{
    char *end;

    if ( length > 0 )
	end = memory + length;
    if ( length < 0 )
#if __SMALL__ || __MEDIUM__ || macintosh
#if DOS386 || macintosh || M_UNIX || M_XENIX
		end = (char *) 0xFFFFFFFF;
#else
		end = (char *) 0xFFFF;
#endif
#else
	end = MK_FP(getDS(),0xFFFF);
#endif
    else if ( length == 0 )
	end = memory + strlen(memory)+1;
// take account of terminating null byte

    setb(memory, end, 0);

    if (start_of_put_area) {
	if ( start_of_put_area < memory
	|| start_of_put_area > end ) {
	    errno = EINVAL;
	    setg(0,0,0);
	    setp(0,0);
	    return;
	}

	if (sflags&statmem && start_of_put_area > memory)
// Assume there is something useful in the get area.
	    setg(memory, memory, start_of_put_area);
	else
// there is nothing to get.
	    setg(memory,memory,memory);
	setp(start_of_put_area, end);
    } else {
	sflags |= rdonly;
	setg(memory, memory, end);
	setp(0,0);
    }
}

strstreambuf::strstreambuf(char *memory, int length,
				char *start_of_put_area)
    :	sflags(statmem)
{
    buffer_setup(memory, length, start_of_put_area);
}

strstreambuf::strstreambuf(unsigned char *memory, int length,
			    unsigned char *start_of_put_area)
    :	sflags(statmem)
{
    buffer_setup((char *) memory, length, (char *) start_of_put_area);
}

strstreambuf::strstreambuf(int chunksize)
    :	sflags(0),
	chunk(chunksize),
	allocate_function(operator new),
	free_function(operator delete)
{}

strstreambuf::strstreambuf(void *(*allocate_f)(size_t),
		void (*free_f)(void *), int chunksize)
    :	sflags(0),
	chunk(chunksize),
	allocate_function(allocate_f),
	free_function(free_f)
{}

strstreambuf::~strstreambuf()
{
    if (!(sflags & (statmem|frozen)) && base() )
	free_function(base());
}

void strstreambuf::freeze(int on)
{
    if (on)
	sflags |= frozen;
    else
	sflags &= ~frozen;
}

char *strstreambuf::str()
{
    freeze();
    return base();
}

streambuf *strstreambuf::setbuf(char *, int length)
{
    chunk = length;
    return this;
}

int strstreambuf::doallocate()
{
    if (sflags & statmem)
	return EOF;

    int length = chunk;
    char * memory;

    if ( length <= 0 )
	length = default_allocation;
    memory = (char *) (*allocate_function)(length);
    if (!memory)
	return EOF;

    buffer_setup(memory, length, memory);
    return 0;
}

int strstreambuf::overflow(int c)
{
    if (sflags & (statmem|frozen|rdonly))
	return EOF;
    if (!base()) {
	if (allocate() == EOF)
	    return EOF;
	if (c != EOF) {
	    *pptr() = c;
	    pbump(1);
	}
	return 0;
    } else {

	char *memory, *old = base();
    	unsigned new_length;
    
    	if (blen() >= 0xFFF0) return EOF;
    
    	if (blen() >= 0x5550)
       	   new_length = 0xFFF0;
    	else
       	   new_length = 3 * blen();   

	memory = (char *)(*allocate_function)(new_length);
	if (!memory)
	    return EOF;

	memmove(memory, base(), blen());

// Relax the get restriction while we are at it
	int t1 = gptr()-base(), t2 = pptr()-base();
	setg(memory, memory+t1, memory+t2);
	t1 = pptr()-pbase(); t2 = pbase()-base();
	setp(memory+t2, memory+new_length);
// Position pptr relative to new buffer
	pbump(t1);
	setb(memory, memory+new_length);

	if ( c != EOF ) {
	    *pptr() = c;
	    pbump(1);
	}

	free_function(old);

	return 0;
    }
}

int strstreambuf::underflow()
{
// Try to extend the get area to overlap characters which have been put.
    if (gptr() && pptr() && (egptr() < pptr())) {
	setg(base(),gptr(),pptr());
	return (unsigned char) *gptr();
    } else
	return EOF;
}

streampos strstreambuf::seekoff(streamoff offset,
			ios::relative_to from, int which)
{
    if (!(which & (ios::in | ios::out)))
	return EOF;
    char *pointer;
    if (offset.stepsize() != 1)
	return EOF;
    long diff = offset.steps();
    if (diff > INT_MAX || diff < INT_MIN)
	return EOF;
    int d = int(diff);

// Relax any unneccessary gptr restriction
    underflow();

    if (which & ios::in) {
	if (!gptr())
	    return EOF;
	switch (from) {
	case ios::beg:
	    pointer = base() + d;
	    break;
	case ios::cur:
	    pointer = gptr() + d;
	    break;
	case ios::end:
	    pointer = egptr() + d;
	    break;
	default:
	    return EOF;
	}
	if (pointer < base() || pointer >= egptr())
	    return EOF;
	setg(base(), pointer, egptr());
    }
    if (which & ios::out) {
	if (!pptr())
	    return EOF;
	switch (from) {
	case ios::beg:
	    pointer = base() + d;
	    break;
	case ios::cur:
	    if (which & ios::in)
		pointer = gptr();
	    else
		pointer = pptr() + d;
	    break;
	case ios::end:
	    pointer = ebuf() + d;
	    break;
	default:
	    return EOF;
	}
	if (pointer < base() || pointer >= ebuf())
	    return EOF;
	int t = pointer-base();
	setp(base(),ebuf());
	pbump(t);
    }
    return pointer-base();
}

int strstreambuf::sync()
{
    return 0;	// always in sync by definition
}

