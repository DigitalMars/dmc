/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __FLOAT_H
#define __FLOAT_H 1

#if __cplusplus
extern "C" {
#endif

/* Define _CRTAPI1 (for compatibility with the NT SDK) */
#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif

/* Define _CRTAPI2 (for compatibility with the NT SDK) */
#ifndef _CRTAPI2
#define _CRTAPI2 __cdecl
#endif

/* Define CRTIMP */
#ifndef _CRTIMP
#if defined(_WIN32) && defined(_DLL)
#define _CRTIMP  __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif

#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

#define	DBL_DIG		15
#define DBL_EPSILON	0x1p-52
#define DBL_MANT_DIG	53
#define DBL_MAX_10_EXP	308
#define	DBL_MAX_EXP	1024
#define DBL_MIN_10_EXP	-307
#define	DBL_MIN_EXP	-1021

#define DBL_MAX 	0x1.FFFFFFFFFFFFFp1023
#define DBL_MIN 	0x1p-1022
#define FLT_MAX 	0x1.FFFFFEp+127
#define FLT_MIN 	0x1p-126

#define	FLT_DIG		6
#define FLT_EPSILON	0x1p-23	/* 1.19209290e-7f */
#define FLT_MANT_DIG	24
#define FLT_MAX_10_EXP	38
#define	FLT_MAX_EXP	128
#define FLT_MIN_10_EXP	-37
#define	FLT_MIN_EXP	-125
#define	FLT_RADIX	2

#define FLT_EVAL_METHOD 2

extern int __CLIB __flt_rounds(void);
#define	FLT_ROUNDS	(__flt_rounds())

#if _WIN32
#define	LDBL_DIG	18
#define LDBL_EPSILON	0x1p-63L
#define LDBL_MANT_DIG	64
#define LDBL_MAX	0x1.FFFFFFFFFFFFFFFEp+16383L
#define LDBL_MAX_10_EXP	4932
#define	LDBL_MAX_EXP	16384
#define LDBL_MIN	0x1p-16382L
#define LDBL_MIN_10_EXP	-4932
#define	LDBL_MIN_EXP	-16381
#else
#define	LDBL_DIG	DBL_DIG
#define LDBL_EPSILON	DBL_EPSILON
#define LDBL_MANT_DIG	DBL_MANT_DIG
#define LDBL_MAX	DBL_MAX
#define LDBL_MAX_10_EXP	DBL_MAX_10_EXP
#define	LDBL_MAX_EXP	DBL_MAX_EXP
#define LDBL_MIN	DBL_MIN
#define LDBL_MIN_10_EXP	DBL_MIN_10_EXP
#define	LDBL_MIN_EXP	DBL_MIN_EXP
#endif

#ifndef __STDC__

extern int __cdecl _8087;
unsigned	__CLIB _status87(void);
unsigned	__CLIB _clear87(void);
unsigned	__CLIB _control87(unsigned, unsigned);
void		__CLIB _fpreset(void);

#define _MCW_EM 	0x003f
#define _EM_INVALID	0x0001		
#define _EM_DENORMAL	0x0002		
#define _EM_ZERODIVIDE	0x0004		
#define _EM_OVERFLOW	0x0008		
#define _EM_UNDERFLOW	0x0010		
#define _EM_INEXACT	0x0020	

#define _MCW_IC 	0x1000
#define _IC_AFFINE	0x1000		
#define _IC_PROJECTIVE	0x0000

#define _MCW_RC 	0x0c00
#define _RC_CHOP	0x0c00	
#define _RC_UP		0x0800		
#define _RC_DOWN	0x0400
#define _RC_NEAR	0x0000		

#define _MCW_PC 	0x0300
#define _PC_24		0x0000		
#define _PC_53		0x0200		
#define _PC_64		0x0300		

#define _CW_DEFAULT (_RC_NEAR | _IC_AFFINE | _EM_DENORMAL | _EM_UNDERFLOW | _EM_INEXACT | _PC_64)

#endif

#if _WIN32
// 53 bits => 17, 64 bits => 21, 113 bits => 36
#define DECIMAL_DIG	21
#else
#define DECIMAL_DIG	17
#endif

/*  Floating point error signals and return codes */
#define _FPE_DENORMAL 0x82
#define _FPE_ZERODIVIDE 0x83
#define _FPE_UNEMULATED 0x87
#define _FPE_UNDERFLOW  0x85
#define _FPE_STACKUNDERFLOW 0x8b
#define _FPE_STACKOVERFLOW  0x8a
#define _FPE_SQRTNEG 0x88
#define _FPE_OVERFLOW  0x84
#define _FPE_INVALID  0x81
#define _FPE_INEXACT 0x86
#define _FPE_EXPLICITGEN	0x8c	/* occurs on an explicit raise */

/* Non-ANSI Names */
#if !__STDC__

#define MCW_EM _MCW_EM
#define EM_INVALID _EM_INVALID
#define EM_DENORMAL _EM_DENORMAL
#define EM_ZERODIVIDE _EM_ZERODIVIDE
#define EM_OVERFLOW _EM_OVERFLOW
#define EM_UNDERFLOW _EM_UNDERFLOW
#define EM_INEXACT _EM_INEXACT

#define MCW_IC _MCW_IC
#define IC_AFFINE _IC_AFFINE
#define IC_PROJECTIVE _IC_PROJECTIVE

#define MCW_RC _MCW_RC
#define RC_CHOP _RC_CHOP
#define RC_UP _RC_UP
#define RC_DOWN _RC_DOWN
#define RC_NEAR _RC_NEAR

#define MCW_PC		_MCW_PC
#define PC_24 _PC_24
#define PC_53 _PC_53
#define PC_64 _PC_64

#define CW_DEFAULT	_CW_DEFAULT

#define FPE_DENORMAL _FPE_DENORMAL
#define FPE_ZERODIVIDE _FPE_ZERODIVIDE 
#define FPE_UNEMULATED _FPE_UNEMULATED
#define FPE_UNDERFLOW _FPE_UNDERFLOW
#define FPE_STACKUNDERFLOW _FPE_STACKUNDERFLOW
#define FPE_STACKOVERFLOW  _FPE_STACKOVERFLOW
#define FPE_SQRTNEG  _FPE_SQRTNEG
#define FPE_OVERFLOW _FPE_OVERFLOW
#define FPE_INVALID _FPE_INVALID
#define FPE_INEXACT _FPE_INEXACT
#define FPE_EXPLICITGEN 	_FPE_EXPLICITGEN

#endif

#if __cplusplus
}
#endif

#endif
