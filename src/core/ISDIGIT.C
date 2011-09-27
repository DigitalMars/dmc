/*_ isdigit.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef isdigit

isdigit(c)	{ return _ctype[c+1] & _DIG; }
