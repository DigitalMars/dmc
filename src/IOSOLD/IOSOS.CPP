// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1992. All Rights Reserved.

#include <iosold\iostream.h>
#ifdef THINK_CPLUS
#include <string.h>
#endif
#include <typeinfo.h>

#if Aiosos1
ostream::ostream(streambuf * buffer) : ios(buffer) {}
#endif

#if Aiosos2
ostream::ostream() : ios(0) {}
#endif

#if Aiosos3
ostream::~ostream()
{
// There is a default constructor which sets a zero
// buffer pointer, must check for this.
    if (rdbuf())
        rdbuf()->sync();
}
#endif

#if Aiosos5
// Perform output-prefix operations.
//
// If the error state is non-zero, return zero immediately.
// If there is a tied ostream (see ios::tie()), flush it.
// Return non-zero. If you define your own inserter that directly
// manipulates the streambuf instead of calling other inserters,
// it should call this when it starts.
int ostream::opfx()
{
    if (good()) {
        if (tie()) 
            tie()->flush();
        return 1;
    }
    return 0;
}
#endif

#if Aiosos6
// output-suffix operations.
//
// If ios::unitbuf is set in the format state, flush output.
// If ios::stdio is set, flush stdio and stderr. If you define
// your own inserter that directly manipulates the streambuf
// it should call this just before returning. All of the inserters
// in ostream call this. Binary put() and write() functions do
// not call this.
void ostream::osfx()
{
    if (flags() & ios::spacing) {
        if (rdbuf()->sputc(fill()) == EOF)
            eof_fail();
    }
    if (!(flags() & ios::stickywidth))
        width(0);
    if ( flags() & ios::unitbuf )
        flush();
    if (flags() & stdio)
        flush_stdio();
}
#endif

#if Aiosos7
int ostream::pad(int position, int n)
{
	long fv = flags() & ios::adjustfield;
	if (!fv) fv = ios::right;
    if (!width())
        return 0;
    if (position & fv) {
        for (int i = width()-n; i > 0; --i) {
            if (rdbuf()->sputc(fill()) == EOF)
                return ios::badbit | ios::eofbit;
        }
    }
    return 0;
}
#endif

#if Aiosos8
// Insert a single character.
ostream &ostream::put(char c)
{
    if ( opfx() ) {
        if ( rdbuf()->sputc(c & 0xff) == EOF )
            eof_fail();
    }
    return *this;
}
#endif

#if Aiosos9
// Perform an absolute seek.
ostream &ostream::seekp(streampos position)
{
    if ( rdbuf()->seekpos(position, ios::out) == EOF )
        eof_fail();
    return *this;
}
#endif

#if Aiosos10
// Perform a relative seek.
ostream &ostream::seekp(streamoff offset, ios::relative_to startpos)
{
    if ( rdbuf()->seekoff(offset, startpos, ios::out) == EOF )
        eof_fail();
    return *this;
}
#endif

#if Aiosos11
// Return the current file position.
streampos ostream::tellp()
{
    streampos result = rdbuf()->seekoff(0, cur, ios::out);

    if ( result == EOF )
        eof_fail();
    return result;
}
#endif

#if Aiosos12
// Write a string of n characters without formatting.
ostream &ostream::write(const void *data, size_t size)
{
    if ( (write_count = rdbuf()->sputn((const char *) data, size))
                                != size )
        eof_fail();
    return *this;
}
#endif

#if Aiosos13
#include <string.h>
// Insert a null-terminated character string.
ostream &ostream::operator<<(const char *s)
{
    if (!opfx())
		return *this;
	int count = strlen(s);
    int err = pad(ios::right | ios::internal, count);
	if (!err) {
        err = (rdbuf()->sputn(s, count) != count);
		if (!err)
	        err = pad(ios::left, count);
    }
	if (err)
		clear(ios::failbit | ios::eofbit | ios::badbit);
    osfx();

    return *this;
}
#endif

#ifdef THINK_CPLUS
	/* Convert integer i to ascii, and store in string pointed to by a.
	 * Input:
	 *	i		input
	 *	a		pointer to string of sufficient length, 17 is always sufficient
	 *	base	radix, between 2 and 36 inclusive
	 * Returns: a
	 */
	static char *itoa(int i,char* a,int base)
	{	unsigned u;
		char *p,buf[sizeof(i) * 8 + 1];
		int sign;
	
		sign = 0;
		u = i;							// assume positive
		buf[sizeof(buf) - 1] = 0;
		p = &buf[sizeof(buf) - 2];		// last character position
		if (base == 10 && i < 0)
		{   sign++;
		    u = -i;
		}
		if (u)
		{	while (1)
			{	*p = (u % base) + '0';
				if (*p > '9')
				    *p += 'A'-'0'-10;
				if ((u /= base) == 0)
					break;
				p--;
			}
		}
		else
			*p = '0';
		*a = '-';
		memcpy(a + sign,p,&buf[sizeof(buf)] - p);
		return a;
	}
#endif

#if Aiosos14
// Insert a void *
#ifndef macintosh
#include <disp.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <dos.h>
ostream &ostream::operator<<(void *vp)
{
	if (!opfx())
		return *this;
	int i;
	char *p, buf1[20], buf2[20];
#if DOS386 || M_UNIX || M_XENIX || macintosh
	itoa((unsigned) vp,buf1,16);
	i = 8 - strlen(buf1);
	p = buf2;
	*p++ = '0';			// bug #4690
	*p++ = 'x';
	for (; i--;)
		*p++ = '0';
	strcpy(p,buf1);
#elif M_I86SM || M_I86MM
	itoa((unsigned) vp,buf1,16);
	i = 4 - strlen(buf1);
	p = buf2;
	*p++ = '0';
	*p++ = 'x';
	for (; i--;)
		*p++ = '0';
	strcpy(p,buf1);
#else
	itoa(FP_SEG(vp),buf2,16);
	p = buf2+strlen(buf2);
	*p++ = '0';
	*p++ = 'x';
	itoa(FP_OFF(vp),p,16);
#endif
	int count = strlen(buf2);
	int err = pad(ios::right | ios::internal, count);
	if (!err) {
		err = (rdbuf()->sputn(buf2, count) != count);
		if (!err)
			err = pad(ios::left, count);
	}
	osfx();
	return *this;
}
#endif

#if Aiosos16
#include <string.h>
#include <stdlib.h>
#include <dos.h>

// Insert a streambuf
ostream &ostream::operator<<(streambuf *s)
{
    int c, rv;

    while ((c = s->sbumpc()) != EOF && (rv = rdbuf()->sputc(c)) != EOF ) ;
    if (rv == EOF)
        eof_fail();
    return *this;
}
#endif

#if Aiosos17
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <iosold\iostream.h>
ostream &flush(ostream &stream) // DJB
{
    stream.flush();
    return stream;
}
#endif

#if Aiosos18
#include <string.h>
#include <stdlib.h>
#include <dos.h>

ostream &endl(ostream &stream)
{
    stream.put('\n');
    stream.flush();
    return stream;
}
#endif

#if Aiosos19
ostream &stickywidth(ostream &stream)
{
    stream.setf(ios::stickywidth);
    return stream;
}
#endif

#if Aiosos20
ostream &spacing(ostream &stream)
{
    stream.setf(ios::spacing);
    return stream;
}
#endif

#if Aiosos21
ostream &nospacing(ostream &stream)
{
    stream.unsetf(ios::spacing);
    return stream;
}
#endif

#if Aiosos22
ostream &fixed(ostream &stream)
{
    stream.setf(ios::fixed, ios::floatfield);
    return stream;
}
#endif

#if Aiosos23
ostream &scientific(ostream &stream)
{
    stream.setf(ios::scientific, ios::floatfield);
    return stream;
}
#endif

#if Aiosos24
ostream &showpoint(ostream &stream)
{
    stream.setf(ios::showpoint);
    return stream;
}
#endif

#if Aiosos25
ostream &floating(ostream &stream)
{
    stream.unsetf(ios::floatfield | ios::showpoint | ios::showpos);
    return stream;
}
#endif

#if Aiosos26
ostream &leftjust(ostream &stream)
{
    stream.setf(ios::left,ios::adjustfield);
    return stream;
}
#endif

#if Aiosos27
ostream &rightjust(ostream &stream)
{
    stream.setf(ios::right,ios::adjustfield);
    return stream;
}
#endif

#if Aiosos28
ostream &internal(ostream &stream)
{
    stream.setf(ios::internal,ios::adjustfield);
    return stream;
}
#endif

#if Aiosos29
ostream &showbase(ostream &stream)
{
    stream.setf(ios::showbase,0);
    return stream;
}
#endif

#if Aiosos32
ostream &uppercase(ostream &stream)
{
	stream.setf(ios::uppercase);
	return stream;
}
#endif

#if Aiosos30
ostream &ostream::operator<<(char c)
{
	if (!opfx())		// Protocol, check ostream state, if not good
		return *this;	// do nothing

	int err = pad(ios::right | ios::internal,1);

// if the stream is still ok, send the character
	if (!err) {
		if (rdbuf()->sputc(c) == EOF)
			err = ios::badbit | ios::eofbit;
		if (!err)
			err = pad(ios::left,1);
	}

// If there was an error mark the stream
	clear(err);
	osfx();						// Protocol
	return *this;
}
#endif

#if Aiosos31
// Flush output
ostream &ostream::flush()
{
    if (rdbuf())
        rdbuf()->sync();
    return *this;
}
#endif

#if Aiosos33
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <iosold\iostream.h>
// this is from iosos16.cpp in old Mac builds, it disappeard in the latest PC release
ostream &ends(ostream &stream)
{
	stream.put('\0');
	return stream;
}
#endif

#if Aiososfp
#include <stdlib.h>
#include <string.h>
#include <math.h>
#if !THINK_CPLUS
	#include <fltpnt.h>
#else
	#include "IOSane.h"
	// remove dependencies on SANE library
	#ifndef FP_NANQ
	#define FP_NANQ		QNAN
	#endif
	#ifndef FP_INFINITE
	#define	FP_INFINITE	INFINITE
	#endif
	// these are from fltpnt.h
	inline int fpclassify(float fe)		{ return IOclassfloat(fe); }
	inline int fpclassify(double fe)	{ return IOclassdouble(fe); }
	inline int fpclassify(long double fe)	{ return IOclassextended(fe); }
	inline int fpclassify(comp fe)  	{ return IOclasscomp(fe); }
	inline int signbit(float fe)		{ return IOsignnum(fe); }
	inline int signbit(double fe)		{ return IOsignnum(fe); }
	inline int signbit(long double fe)	{ return IOsignnum(fe); }
#endif

#if THINK_CPLUS
	#include <ecvt.h>
#endif

const int widest_real = 50;

#if macintosh
ostream &ostream::operator<<(long double value)
#else
ostream &ostream::operator<<(double value)
#endif
{
    const char *end;
    int field_width;
    int number_width;
    int negative = signbit(value);
    char buffer[widest_real+1];
    int exp;
 
    if (!opfx())
        return *this;

// If ios::fixed is set, convert using fcvt() otherwise use ecvt(), then
// check if the representation can be displayed as fixed
	int fpt = fpclassify(value);
	if (fpt <= FP_NANQ) {
		strcpy(buffer,"NAN");
		number_width = 3;
	} else if (fpt == FP_INFINITE) {
		strcpy(buffer,"INFINITY");
		number_width = 8;
	} else {
	    if (flags() & ios::fixed) {
fixed:
	        number_width = fixed_point(value, buffer);
	    } else {
	        number_width = sci_notation(value, buffer, exp);
	        if (!(flags() & ios::scientific) && exp > -4 && exp <= precision())
	            goto fixed;
	    }
	}
    field_width = number_width;
    if (negative || (flags() & ios::showpos))
        field_width++;

    if (pad(ios::right, field_width) == EOF)
		goto eoffail;

    if (negative) {
        if (rdbuf()->sputc('-') == EOF)
			goto eoffail;
    } else if (flags() & ios::showpos) {
        if (rdbuf()->sputc('+') == EOF)
			goto eoffail;
	}
    if (pad(ios::internal, field_width) == EOF)
        goto eoffail;

    if (rdbuf()->sputn(buffer, number_width) == EOF)
		goto eoffail;
    if (pad(ios::left, field_width) == EOF)
		goto eoffail;

    osfx();
    return *this;
eoffail:
	clear(ios::eofbit | ios::failbit | ios::badbit);
	return *this;
}

// Fixed point conversion using fcvt()
#if macintosh
int ostream::fixed_point(long double value, char *buf)
#else
int ostream::fixed_point(double value, char *buf)
#endif
{
    int point, dummy;
    char *s = buf;
    const char *string = fcvt(value, precision(), &point, &dummy);
    if ( string == 0 || *string == '\0' ) {
        *s++ = '0';
        if (precision() && (flags() & ios::showpoint)) {
            *s++ = '.';
            for (int i = precision(); i--;)
                *s++ = '0';
        }
        *s = '\0';
        return s-buf;
    }

    int count;
    if ( point > 0 ) {
        count = point;
        while ( count-- > 0 && *string )
            *s++ = *string++;
    } else
        *s++ = '0';
// Put a leading zero before the decimal point

    char *decimal_point = s;
    *s++ = '.';
    if (point >= 0) {
        while (*string)
            *s++ = *string++;
    } else {
        count = -point;
        int zeroes = count > precision()? precision(): count;
        while (count--)
            *s++ = '0';
        if (zeroes < precision())
            while (*string)
                *s++ = *string++;
    }
    *s-- = '\0';
            
// If there is no fractional part and ios::showpoint is not set,
// do not show the decimal point or trailing zeroes.
    if (!(flags() & ios::showpoint)) {
        while ( s > decimal_point && *s == '0' )
            *s-- = '\0';
        if ( s == decimal_point )
            *s-- = '\0';
    }

    return s-buf+1;
}

#if macintosh
int ostream::sci_notation(long double value, char *buf, int &exp)
#else
int ostream::sci_notation(double value, char *buf, int &exp)
#endif
{
// There should be a decimal point and at least one digit,
// unless the user has set precision to 0. Trailing zeroes,
// other than a zero immediately to the right of the decimal
// point, are truncated unless ios::showpoint is set.
    char *s = buf;
	int dummy;
    const char *string =  ecvt(value, precision()+1, &exp, &dummy);
    int point = exp;
    if ( string == 0 || *string == '\0' ) {
        *s++ = '0';
        if (precision() && (flags() & ios::showpoint)) {
            *s++ = '.';
            for (int i = precision(); i--;)
                *s++ = '0';
        }
        strcpy(s,(flags() & ios::uppercase)? "E+00": "e+00");
        return s-buf+4;
    }

    *s++ = *string++;

    char *decpoint = s;
    if (precision())
        *s++ = '.';
    if (*string)
        while (*string)
            *s++ = *string++;
    else
        if (precision() && (flags() & ios::showpoint))
            *s++ = '0';
    if (!(flags() & ios::showpoint)) {
        s--;
        while (s >= decpoint && (*s == '0' || *s == '.'))
            *s-- = '\0';
        s++;
    }


// Introduce the exponent. Always print a sign for the exponent.
    *s++ = (flags() & ios::uppercase)? 'E': 'e';

    if (value != 0.0)
        point--;

    *s++ = point < 0? (point = -point, '-'): '+';

    if (point >= 100) {
        *s++ = '0' + (point / 100);
        point %= 100;
    }
    *s++ = '0' + (point / 10);
    point %= 10;
    *s++ = '0' + point;

    *s = '\0';

    return s-buf;
}
#endif


