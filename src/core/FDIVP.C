
#include <float.h>

#if __INTSIZE == 4
#define __far
#endif

#if __INTSIZE == 2
extern void __pascal fdiv_r();
#else
extern void __cdecl fdiv_r();
#endif
int __cdecl fdiv_chk_flag;

/***********************************
 * Emulate the instruction:
 *	FDIVP ST(1),ST
 */

void __cdecl _FDIVP()
{
    if (fdiv_chk_flag == 1)		// if no bug
    {
	     __asm
	    {
	        fdivp	ST(1),ST
	    }
    }
    else
    {

#if __INTSIZE == 4
        _EAX = 0x0D;			// FDIVP ST(1),ST
#else
        _AX = 0x0D;				// FDIVP ST(1),ST
#endif
        fdiv_r();				// call Intel replacement function
    }
}

/***********************************
 * Set fdiv_chk_flag according to if the FDIV instruction is broken
 * or not.
 */

int FDIV_DETECT()
{
    // determine if there is a bug
    static volatile double x = 4195835.0;
    static volatile double y = 3145727.0;
    static volatile double one = 1.0;

    if (_8087 <= 2)			// if not a 387 or better
	fdiv_chk_flag = 1;		// no bug
    else
    {
	__asm
	{
	fld    x
	fdiv   y			// no_error = ((x - (x / y)*y) < 1.0
	fmul   y
	fsubr  x
	fcomp  one
	fstsw  AX
#if __INTSIZE == 4
	and    EAX, 0100h
	shr    EAX, 7
	dec    EAX
	mov    fdiv_chk_flag, EAX
#else
	and    AX, 0100h
	shr    AX, 7
	dec    AX
	mov    fdiv_chk_flag, AX
#endif
	}
    }
}

