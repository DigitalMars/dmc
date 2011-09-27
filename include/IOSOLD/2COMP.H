/* $Revision: 1.1.1.1 $ */

#ifndef __2COMP_H
#define __2COMP_H

#pragma pack(1)

struct _2c_translate_info {
	char *cp;
    char digits[20];
    int neg, radix, dcount, bytes;
};

class ostream;
class istream;
class _2Comp {
friend class ostream;
friend class istream;
    static  ostream & __cdecl insert(ostream&, const void *body, int bytes, int issigned);
    static int __cdecl format(unsigned char *body, char *buf, int bytes,
			int radix, int upper, int negative);
    static istream & __cdecl extract(istream &is, void *body, int bytes);
    static int __cdecl _2Comp::xlate(char *s, unsigned char *body,
			int radix, int negative, int bytes);
    static int __cdecl div(unsigned char *body, int d, int bytes);
    static void __cdecl negate(unsigned char *body, int bytes);
};

#pragma pack()
#endif

