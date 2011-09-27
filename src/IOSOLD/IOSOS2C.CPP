#include <string.h>
#include <iosold\iostream.h>
#include <iosold\2comp.h>
#include <typeinfo.h>

#ifdef THINK_CPLUS
	/* STRREV
	 *	Reverse the characters in the string p.
	 *	Returns: p
	 */
	static char *strrev(char* p) {
		char *pend,*pstart = p;
	
		pend = p + strlen(p) - 1;
		while (p < pend) {	*p ^= *pend;
			*pend ^= *p;
			*p++ ^= *pend--;				// swap *p and *pend
		}
		return pstart;
	}
#endif

ostream &_2Comp::insert(ostream &os, const void *bp,  int bytes, int issigned)
{
    if (!os.opfx())
        return os;

    char buf[20];
    unsigned char ws[8];
    memmove(ws,bp,bytes);

    int radix = 10;
    if (os.flags() & ios::hex)
        radix = 16;
    else if (os.flags() & ios::oct)
        radix = 8;
    issigned = (radix == 10)? issigned: 0;

    int isneg = 0;
#if macintosh
    if (issigned && (ws[0] & 0x80))
#else
    if (issigned && (ws[bytes-1] & 0x80))
#endif
        ++isneg;

    int length = format(ws, buf, bytes, radix,
                            os.flags() & ios::uppercase, isneg);
    char sign = isneg? '-': ((os.flags() & ios::showpos)? '+': 0);
    int octal = 0, fulllength = length+(sign != 0);
	if (os.flags() & ios::showbase) {
	    if (radix == 16)
	        fulllength += 2;
	    else if (radix == 8) {
			if (*buf != '0' ||
					(os.flags() & ios::internal) && os.width() > fulllength) {
				octal++;
				fulllength++;
			}
	    }
	}

    int err = os.pad(ios::right,fulllength);
    if (!err) {
        if (sign) {
            if (os.rdbuf()->sputc(sign) == EOF)
                err = ios::failbit | ios::eofbit;
        }
        if (!err) {
			if (os.flags() & ios::showbase) {
	            if (radix == 16) {
	                if (os.rdbuf()->sputn((os.flags() & ios::uppercase)?
                                                        "0X": "0x",2) != 2)
	                    err = ios::failbit | ios::eofbit;
	            } else if (radix == 8 && octal) {
	                if (os.rdbuf()->sputc('0') == EOF)
	                    err = ios::failbit | ios::eofbit;
	            }
			}
            if (!err) {
                err = os.pad(ios::internal,fulllength);
                if (!err) {
                    if (os.rdbuf()->sputn(buf,length) != length)
                        err = ios::failbit | ios::eofbit;
                    if (!err)
                        err = os.pad(ios::left,fulllength);
                }
            }
        }
    }
    os.clear(err);
    os.osfx();
    return os;
}

int _2Comp::format(unsigned char *body, char *buf,
                            int bytes, int radix, int upper, int negative)
{
    if (negative)
        negate(body,bytes);	// if it is negative make the corresponding positive
    int digits = 0;
    char *p = buf;
    for (;;) {
        int d, q = div(body,radix,bytes); // get successive digits
		d = (q & 0xff);
        if (radix == 16) {
            if (d < 10)
                *p++ = '0'+d;
            else if (upper)
                *p++ = 'A'+d-10;
            else
                *p++ = 'a'+d-10;
        } else
            *p++ = '0'+d;
        digits++;
        if (!(q & 0xff00)) break;
    }
    *p = 0;
    strrev(buf);                                    // reverse in-situ
    return digits;
}

#if macintosh
//  'body' points to a series of bytes of 'bytes' length
//  There can be no leading 0 bytes
//  'divisor' is the radix the digits are to be divided into
//  dual return value - (more bytes left) << 8 | msb digit
//  'body' reflects the division

int _2Comp::div(unsigned char *body, int divisor, int bytes)
{
    unsigned char *pbytes;
    int q,rem,val,moreflag = 0;
    pbytes = body;						// start at msb byte
    rem = 0;							// no previous remainder
    while(bytes--)
    	{
	val = (rem << 8) | *pbytes;			// next lsb byte and whats left over
	q = val/divisor;					// get quotient for this byte
	rem = val - (q*divisor);			// get remainder that is left over
	if(q)
	    moreflag++;						// will still have more digits
	*pbytes = q;						// save last quotient
	pbytes++;
	}
    return moreflag << 8 | rem;			// return last remainder and if still more
}

// negate the value that is 'bytes' long pointed to by 'body'
void _2Comp::negate(unsigned char *body,int bytes)
{
unsigned short val,carry = 1;
unsigned char *pn = body + bytes -1;

    while(bytes--)						// from lst to msb
        {
	val = *pn;							// get previous value
	val = ((~val) & 0xff) + carry;  	// take ~ of byte only + previous carry
	*pn-- = val;						// save byte
	carry = val >> 8;					// next carry will be in msb of addition
	}
}
#endif
