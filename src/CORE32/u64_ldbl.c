

// Copyright (c) 2002-2009 by Digital Mars
// All Rights Reserved
// written by Walter Bright
// http://www.digitalmars.com

// Convert unsigned long long to long double

static long double adjust = (long double)0x800000000000000 * 0x10;
static short roundTo0 = 0xFBF;

__declspec(naked) long double _U64_LDBL()
{
    _asm
    {
	push	EDX
	push	EAX
	and	dword ptr 4[ESP], 0x7FFFFFFF
	fild	qword ptr [ESP]
	test	EDX,EDX
	jns	L1
	fld	tbyte ptr adjust
	faddp	ST(1), ST
    L1:
	add	ESP, 8
	ret
    }
}

// Convert long double in ST0 to unsigned long long

__declspec(naked) unsigned long long __LDBLULLNG()
{
    _asm
    {
	    sub		ESP,16
	    fld		tbyte ptr adjust
	    fcomp
	    fstsw	AX
	    fstcw	8[ESP]
	    fldcw	roundTo0
	    sahf
	    jae		L1
	    fld		tbyte ptr adjust
	    fsubp	ST(1), ST
	    fistp	qword ptr [ESP]
	    pop		EAX
	    pop		EDX
	    fldcw	[ESP]
	    add		ESP,8
	    add		EDX,0x80000000
	    ret
	L1:
	    fistp	qword ptr [ESP]
	    pop		EAX
	    pop		EDX
	    fldcw	[ESP]
	    add		ESP,8
	    ret
    }
}


