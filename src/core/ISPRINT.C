/*_ isprint.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef isprint

isprint(c)	{ return _ctype[c+1] & (_UC|_LC|_DIG|_PNC|_BLK); }
