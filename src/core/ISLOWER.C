/*_ islower.c   Sat Sep 10 1988   Modified by: Walter Bright */

#include	<ctype.h>

#undef islower

islower(c)	{ return _ctype[c+1] & _LC; }
