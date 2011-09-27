/*_ iscntrl.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef iscntrl

iscntrl(c)	{ return _ctype[c+1] & _CTL; }
