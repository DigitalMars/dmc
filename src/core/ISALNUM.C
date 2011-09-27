/*_ isalnum.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef isalnum

isalnum(c)	{ return _ctype[c+1] & (_UC|_LC|_DIG); }
