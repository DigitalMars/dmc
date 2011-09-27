
#ifndef __8087_H
#define __8087_H

/* Condition code positions when transferred to AX */

#define C0	0x0100		// CF
#define C1	0x0200		// (none)
#define C2	0x0400		// PF
#define C3	0x4000		// ZF

/* Results of FXAM:
 *	C1 = sign
 *
 *	C3,C2,C0
 *	 0  0  0	unsupported
 *	 0  0  1	nan
 *	 0  1  0	normal
 *	 0  1  1	infinity
 *	 1  0  0	zero
 *	 1  0  1	empty
 *	 1  1  0	denormal
 */

#define _CCC_UNSUPPORTED	0
#define _CCC_NAN		C0
#define _CCC_NORMAL		C2
#define _CCC_INFINITY		(C2 | C0)
#define _CCC_ZERO		C3
#define _CCC_EMPTY		(C3 | C0)
#define _CCC_DENORMAL		(C3 | C2)
#define _CCC_MASK		0x4500

#define _CCC_NEGATIVE		C1

/* Results of FTST:
 *	C3,C2,C0
 *	 0  0  0	ST > operand
 *	 0  0  1	ST < operand
 *	 1  0  0	ST == operand
 *	 1  1  1	unordered
 *
 * See Table 6-4, page 6-11 in Pentium Processor's User Manual Volume 3
 */

/**********************
 * Is quiet nan?
 */

inline int isqnanl(long double x)
{
    // The difference between a QNAN and an SNAN is that a signalling
    // NAN has 0 as the most significant bit of its significand (bit 62).
    return ((unsigned long *)&x)[1] & 0x40000000;
}

/************************
 * Convert to qnan.
 */

inline long double toqnanl(long double x)
{
    ((unsigned long *)&x)[1] |= 0x40000000;
    return x;
}

#endif
