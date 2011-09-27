/*_ isspace.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef isspace

isspace(c)	{ return _ctype[c+1] & _SPC; }
