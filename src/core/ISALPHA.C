/*_ isalpha.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef isalpha

isalpha(c)	{ return _ctype[c+1] & (_UC|_LC); }
