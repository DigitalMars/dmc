/*_ isascii.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef isascii
#undef __isascii

isascii(c)	{ return (unsigned) c < 0x80; }
__isascii(c)	{ return (unsigned) c < 0x80; }
