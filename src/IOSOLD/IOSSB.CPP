// IOStreams Package
// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1992. All Rights Reserved.

#include <string.h>
#include <iosold\iostream.h>
#include <typeinfo.h>

#if Aiossb1
///////////////////////////////////////////////////////////////////
// This is called by sbumpc when _gptr == _egptr.
// It calls overflow(), increments the get pointer, and then returns
// the value returned by overflow . It's here to avoid inining the
// special-case code.
//
int streambuf::sbumpc_special()
{
    int result;

    if ( (result = underflow()) != EOF)
	_gptr++;

    return result;
}
#endif

#if Aiossb2
// Try to set up a buffer. If one already exists or unbuffered() is non-zero,
// return 0 without doing anything. If something goes wrong, return EOF.
// Otherwise, return 1.
//  * This function is only called by VIRTUAL members of the base class
// streambuf, thus you can override everything that uses it.
int streambuf::allocate()
{
    if (_base || _unbuffered)
	return 0;

    return doallocate();
}
#endif

#if Aiossb3
#include <stdlib.h>

// Most derived classes will override the buffer set-up here.

const int	default_buffer_size = 512;    // arbitrary

// doallocate is called when allocate() determines space is necessary.
// doallocate() must call setb() to set up the buffer, and return a
// non-eof value, or return EOF if it can not get space.
// This is never called if unbuffered() is non-zero or the buffer has
// already been set.
int streambuf::doallocate()
{
    char *buffer = (char *) malloc(default_buffer_size);
//new char[default_buffer_size];
    if ( buffer == 0 )
	return EOF;

    setb(buffer, &buffer[default_buffer_size], 1);
    return 1;
}
#endif

#if Aiossb4
// Setb is called to set up the buffer.
void streambuf::setb(char *base, char *end, int own)
{
    _base = base;
    _ebuf = end;
    _owned = own;
};
#endif

#if Aiossb5
#include <stdlib.h>

streambuf::~streambuf()
{
    if ( _owned && _base )
	free(_base);
}
#endif

#if Aiossb6
streambuf::streambuf()
    :	_unbuffered(0), _owned(0)
{
    setb(0, 0);
    setg(0, 0, 0);
    setp(0, 0);
}
#endif

#if Aiossb7
streambuf::streambuf(char *memory, int length)
    :	_unbuffered(0), _owned(0)
{
    setg(0, 0, 0);
    setp(0, 0);
    if (!memory || length <= 0) {
	_unbuffered = 1;
	setb(unbuf, unbuf+1, 0);
    } else {
	setb(memory, memory+length, 0);
// Using the memory as a put area provides some minimal
// functionality for a plain streambuf
	setp(memory, memory+length);
    }
}
#endif

#if Aiossb8
#include <stdlib.h>

// Request that this streambuf use the argument memory buffer.
// A null memory argument, or a zero or negative length argument,
// is taken as a request that this streambuf be unbuffered.
// If the request to set the buffer is honored, return this, otherwise
// return zero.
streambuf *streambuf::setbuf(char *memory, int length)
{
    this->sync();

    if (_owned && _base)
	free(_base);
    _owned = 0;

// The derived class is responsible for dividing up the buffer area
    setp(0, 0);
    setg(0, 0, 0);

    if ( memory == 0 || length < 1 ) {
	_unbuffered = 1;
// Use a single character buffer area to allow sgetc() work sensibly
	setb(unbuf, unbuf+1, 0);
    } else
	setb(memory, memory+length, 0);

    return this;
}
#endif

#if Aiossb9
// Perform an absolute seek of the get or put pointers, or both, to
// position. Position is an implementation-dependent value which should
// not have arithmetic performed on it. "which" signifies what
// pointer is to be affected: ios::in for the get pointer, ios::out for
// the put pointer, or the two or-ed together for both.
streampos streambuf::seekpos(streampos position, int which)
{
    return seekoff(streamoff(position), ios::beg, which);
}

// Perform a relative seek of the get or put pointers, or both, to
// position. "which" signifies what pointer is to be affected: ios::in
// for the get pointer, ios::out for the put pointer, or the two or-ed
// together for both.
//
// streambuf essentially ignores this request and returns EOF. This
// is virtual and will usually be overidden by derived classes.
streampos streambuf::seekoff(streamoff, ios::relative_to, int)
{
    return streampos(EOF);
}
#endif

#if Aiossb10
// Fetch the "count" characters following the get pointer, and
// stores them in "buffer". If there are less than "count" characters,
// the amount remaining are fetched. The get pointer is repositioned
// after the fetched characters, and the number of characters fetched
// is returned.
int streambuf::sgetn(char *buffer, int count)
{
    int wanted = count;     // Number of chars asked for

    while (count > 0) {
	int transfer = in_avail();  // Number of chars available to move

	if (transfer <= 0) {

// No characters available. Try to fill the buffer.
	    int result = underflow();
	    if (result == EOF)
		break;
	    if (_unbuffered) {	    // skip the call to memmove
		*buffer++ = result;
		++_gptr;
		count--;
		continue;
	    }
	}

// Some characters are available in the get buffer.
// Move as many characters as possible.
	if (count < transfer)
	    transfer = count;
	memmove(buffer, _gptr, transfer);
	buffer += transfer;
	count -= transfer;
	_gptr += transfer;
    }
    return wanted - count;
}
#endif

#if Aiossb11
int streambuf::underflow()
{
// In the simple streambuf there is nowhere to get characters from!
    return EOF;
}

int streambuf::overflow(int c)
{
// There is nothing useful which can be done
    return EOF;
}
#endif

#if Aiossb12
// Store "n" characters on the character stream, advancing the "put"
// pointer past the stored characters. Return the number of characters
// stored, which may be less than "n" if there is an error.
int streambuf::sputn(const char *buffer, int count)
{
    int wanted = count;     // Number of chars asked for

    while ( count > 0 ) {
	int transfer = _epptr - _pptr;

	if ( transfer <= 0 ) {
	    int result = overflow((unsigned char) *buffer);
	    if ( result == EOF )
		break;
	    else {
// This deals with unbuffered case also
		buffer++;
		count--;
		continue;
	    }
	}

	if ( count < transfer )
	    transfer = count;

	memmove(_pptr, buffer, transfer);
	_pptr += transfer;

	buffer += transfer;
	count -= transfer;
    }
    return wanted - count;
}
#endif

#if Aiossb13
// Make the external character stream and the streambuf consistent
// with each other.
int streambuf::sync()
{
// The get area will always be empty in streambuf. Any characters put
// can't be flushed away
    if (_pptr == _pbase)
	return 0;   // nothing put so sync would have nothing to do
    else
	return overflow();	// otherwise fail, flushing the buffers
}
#endif

#if Aiossb14
// This is called by sputbackc() when the get pointer is equal to
// the base of the get area, and there is thus no space to put back
// characters. It may try to re-arrange the buffer so that it is
// possible to put back characters, and then put back c. If it
// succeeds it should return c, otherwise it should return EOF.
int streambuf::pbackfail(int)
{
    return EOF;
}
#endif

