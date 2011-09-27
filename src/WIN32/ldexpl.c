
#include <math.h>

/*********************************
 * value*(2**exp)
 */

#undef ldexpl
#undef ldexp

__declspec(naked)
long double ldexpl(long double value, int exp)
{
    __asm
    {
	fild	dword ptr 16[ESP]	// load exp
	fld	tbyte ptr 4[ESP]	// load value
	fscale				// ST(0) = ST(0) * (2**ST(1))
	fstp	ST(1)
	ret
    }
}
