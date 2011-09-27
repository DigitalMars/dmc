/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __INT_H
#define __INT_H 1
#if  !defined(__NT__) 
 
#if __cplusplus
extern "C" {
#endif

#include	<dos.h>

#pragma pack(1)
struct INT_DATA
{
#if __INTSIZE == 4
    char		align;
#endif
    unsigned		prevvec_off;
    unsigned short	prevvec_seg;
#if __INTSIZE == 4
    unsigned short	prevvecr_off;
    unsigned short	prevvecr_seg;
#endif
    unsigned		stacksize;
    unsigned		newstack_off;
    unsigned short	newstack_seg;
    unsigned		oldstack_off;
    unsigned short	oldstack_seg;
#if __COMPACT__ || __LARGE__
    unsigned short	staticseg;
#endif
    int		(__cdecl *funcptr)();
    union REGS regs;
    struct SREGS sregs;
};

#pragma pack()

void __cdecl int_getvector(unsigned,unsigned *,unsigned *);
void __cdecl int_setvector(unsigned,unsigned,unsigned);
int __cdecl int_intercept(unsigned,int (__cdecl *funcptr)(struct INT_DATA *),unsigned);
void __cdecl int_restore(unsigned);
void __cdecl int_off(void);
void __cdecl int_on(void);
long __cdecl int_prev(struct INT_DATA *);

#define int_on()	__emit__(0xFB)
#define int_off()	__emit__(0xFA)
#define int_gen(i)	__emit__(0xCD,i)

#if __cplusplus
}
#endif
#endif /* ifndef __NT__ */

#endif
