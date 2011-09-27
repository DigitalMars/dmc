/*_ iscsym.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef iscsym
#undef __iscsym

iscsym(c)	{ return (_ctype[c+1] & (_UC|_LC|_DIG)) || (c == '_'); }
__iscsym(c)	{ return (_ctype[c+1] & (_UC|_LC|_DIG)) || (c == '_'); }
