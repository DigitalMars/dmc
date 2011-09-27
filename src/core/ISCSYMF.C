/*_ iscsymf.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef iscsymf
#undef __iscsymf

iscsymf(c)	{ return (_ctype[c+1] & (_UC|_LC)) || (c == '_'); }
__iscsymf(c)	{ return (_ctype[c+1] & (_UC|_LC)) || (c == '_'); }
