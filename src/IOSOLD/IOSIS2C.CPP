// IOStreams Package
// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1992. All Rights Reserved.


#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <iosold\iostream.h>
#include <iosold\2comp.h>
#include <typeinfo.h>

istream &_2Comp::extract(istream &is, void *body, int bytes)
{
	if (!is.ipfx())
		return is;
	char buf[20], *p = buf;
	int err = 0, gotsign = 0, negative = 0, digits = 0, c, xc, sc;
	int radix, basef = is.flags() & ios::basefield;
	switch (basef) {
	case ios::oct:
		radix = 8;
		break;
	case ios::dec:
		radix = 10;
		break;
	case ios::hex:
		radix = 16;
		break;
	default:			// includes mixed flags
		radix = 0;
	}
	if ((c = sc = is.rdbuf()->sgetc()) == EOF)
		err = ios::failbit | ios::eofbit;
	if (!err) {
		if (sc == '-' || sc == '+') {
			gotsign = 1;
			negative = (sc == '-');
			if ((c = is.rdbuf()->snextc()) == EOF)
// incomplete and no more chars
				err = ios::failbit | ios::eofbit;
		}
		if (!err) {
			if (!radix) {	// infer the radix
				radix = 10;
				if( c == '0') {
					is.rdbuf()->stossc();
					if ((xc = is.rdbuf()->sgetc()) != EOF) {
						if (isdigit(xc)) {
							if (xc < '8') {
								radix = 8;
								c = xc;
							} else {
								*p++ = '0';
								c = 0;
								digits = 1;
							}
						} else if (tolower(xc) == 'x') {
							if ((c = is.rdbuf()->snextc()) != EOF) {
								if (isxdigit(c))
									radix = 16;
								else {
									*p++ = '0';	// isolated zero
									c = 0;
									digits = 1;
									is.rdbuf()->sputbackc(xc);
								}
							}
						} else {
							*p++ = '0';
							c = 0;
							digits = 1;
						}
					 } else
						 err = ios::eofbit;			 // note the eof
				}
			}
			for (; c;) {
				if (!isxdigit(c) || (radix < 16 && c > '9')
									|| (radix == 8 && c > '7'))
					break;
				is.rdbuf()->stossc();
				*p++ = c;
				++digits;
				if ((c = is.rdbuf()->sgetc()) == EOF) {
					err = ios::eofbit;
					break;
				}
			}
			if (!digits) {
				if (gotsign)
					is.rdbuf()->sputbackc(sc);
				err |= ios::failbit;	// no digits
			}
		}
	}
	*p = 0;
	if (!(err & (ios::failbit | ios::badbit))) {
		errno = xlate(buf, (unsigned char *) body,
					radix,negative,bytes)?	ERANGE: 0;
		if (errno)
			err |= ios::failbit;
	}
	is.clear(err);
	return is;
}

#if macintosh
int _2Comp::xlate(char *buf,unsigned char * body, int radix, int negative,int bytes)
{
	int i,overflow = 0;
	char *pin;
	unsigned char *pout,*pend;
	unsigned short digit;

	pend = body + bytes - 1;	// ptr to last digit (lsb)
	for(i=0; i<bytes; i++)		// clear the output number body
		*(body+i) = 0;
		
	for(pin = buf; *pin; pin++)
	{							// loop through input chars
		unsigned val, carry = 0, dval;

		digit = *pin;			// next digit
		for(pout = pend, i=0; i< bytes; i++)
		{						// from lsb to msb multiply previous val by radix
			val = *pout * radix + carry;
			*pout-- = val & 0xff;
			carry = val >> 8;	// pick up carry to add to next msb byte
		}
		
		dval = (digit >= 'a') ? digit - ('a'-10) :
			   (digit >= 'A') ? digit - ('A'-10) : digit - '0';

		pout = pend;			// loop through lsb to msb again
		for (i=0; i < bytes; i++)
		{
			dval += *pout;		// add the new digit value
			*pout-- = dval;		// save lsb
			dval >>= 8;			// propogate carry through msb bytes
		}
		if (dval)				// carry left over, had an overflow
			overflow = 1;
	}
	if (negative)
		_2Comp::negate(body,bytes);
	return overflow;
};
#endif

