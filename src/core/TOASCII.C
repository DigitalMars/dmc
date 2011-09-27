/*_ toascii.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef toascii
#undef __toascii

toascii(c)	{ return c & 0x7F; }
__toascii(c)	{ return c & 0x7F; }
