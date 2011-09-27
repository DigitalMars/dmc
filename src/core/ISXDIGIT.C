/*_ isxdigit.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef isxdigit

isxdigit(c)	{ return _ctype[c+1] & _HEX; }
