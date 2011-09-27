//_ ldclass.c
// $Header$
// Copyright (C) 2001-2004 by Digital Mars
// All Rights Reserved
// www.digitalmars.com
// Written by Walter Bright

#include	<fltpnt.h>

__declspec(naked) unsigned __cdecl __fpxam_ld(long double d)
{
    _asm
    {
	fld	tbyte ptr 4[ESP]
	fxam
	fstsw	AX
	fstp	ST(0)
	ret
    }
}

/************************
 * classify long double float
 * Input:
 *	d
 * Returns:
 *	FP_xxxxxx
 */

unsigned __cdecl __fpclassify_ld(long double d)
{
    unsigned result;

    _asm
    {
	fld	tbyte ptr d
	fxam
	fstsw	AX
	fstp	ST(0)
    }

#define C0	0x100
#define C1	0x200		// sign of ST
#define C2	0x400
#define C3	0x4000

/*
 *	C3,C2,C0
 *	000	unsupported
 *	001	nan
 *	010	normal
 *	011	infinity
 *	100	zero
 *	101	empty
 *	110	denormal
 */

    switch (_EAX & (C3 | C2 | C0))
    {
	case C0:
	#if 0
	    // The difference between a QNAN and an SNAN is that a signalling
	    // NAN has 0 as the most significant bit of its significand (bit 62).
	    if (((unsigned *)d)[1] & 0x40000000) == 0)
		result = FP_NANS;
	#endif
	    result = FP_NAN;
	    break;
	case C2:
	    result = FP_NORMAL;
	    break;
	case C2|C0:
	    result = FP_INFINITE;
	    break;
	case C3:
	    result = FP_ZERO;
	    break;
	case C3|C2:
	    result = FP_SUBNORMAL;
	    break;
	case C3|C0:
	    result = FP_EMPTY;
	    break;
	default:	
	    result = FP_UNSUPPORTED;
	    break;
    }
    return result;
}
