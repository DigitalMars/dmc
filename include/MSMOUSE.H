/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __MSMOUSE_H
#define __MSMOUSE_H 1

#ifndef __NT__

#ifdef _WINDOWS
#include <windows.h>
#endif

#if __cplusplus
extern "C" {
#endif

#define MSM_SIGNALMOVE 1
#define MSM_SIGNALBUTTONDOWNL 2
#define MSM_SIGNALBUTTONUPL 4
#define MSM_SIGNALBUTTONDOWNR 8
#define MSM_SIGNALBUTTONUPR 16
#define MSM_SIGNALBUTTONDOWNM 32
#define MSM_SIGNALBUTTONUPM 64
#define MSM_BUTTONL 1
#define MSM_BUTTONR 2
#define MSM_BUTTONM 4
#define LEFT_BUTTON     1
#define RIGHT_BUTTON    2
#define MIDDLE_BUTTON   0
#ifndef _WINDOWS
int     __cdecl msm_init(void);
void    __cdecl msm_term(void);
void    __cdecl msm_showcursor(void);
void    __cdecl msm_hidecursor(void);
int     __cdecl msm_getstatus(unsigned *,unsigned *);
void    __cdecl msm_setcurpos(unsigned,unsigned);
int     __cdecl msm_getpress(unsigned *,unsigned *,unsigned *);
int     __cdecl msm_getrelease(unsigned *,unsigned *,unsigned *);
void    __cdecl msm_setareax(unsigned,unsigned);
void    __cdecl msm_setareay(unsigned,unsigned);
void    __cdecl msm_setgraphcur(int,int,int *);
void    __cdecl msm_settextcur(int,int,int);
void    __cdecl msm_readcounters(int *,int *);
void    __cdecl msm_signal(unsigned,
		void (__cdecl *)(unsigned,unsigned,unsigned,unsigned),void *);
void    __cdecl msm_lightpenon(void);
void    __cdecl msm_lightpenoff(void);
void    __cdecl msm_setratio(unsigned,unsigned);
void    __cdecl msm_condoff(unsigned,unsigned,unsigned,unsigned);
void    __cdecl msm_setthreshhold(unsigned);
void    __cdecl msm_reversebuttonon(void);
void    __cdecl msm_reversebuttonoff(void);
int     __cdecl msm_reversebuttonis(void);
int     __cdecl msm_reversebutton(int);
#else

#define MSM_SCROLLUP    120
#define MSM_SCROLLDOWN  121

#define msm_setcurpos(A,B)
#define msm_setareax(A,B)
#define msm_setareay(A,B)
#define msm_setgraphcur(A,B,C)
#define msm_settextcur(A,B,C)
#define msm_setratio(A,B)
#define msm_condoff(A,B,C,D)
#define msm_setthreshhold(A)
#define msm_lightpenon()
#define msm_lightpenoff()
#define msm_showcursor()        ShowCursor(1)
#define msm_hidecursor()        ShowCursor(0)
short           __cdecl msm_init(void);
void            __cdecl msm_term(void);
unsigned short  __cdecl msm_hwndget(void);
unsigned short  __cdecl msm_messagemonitor(unsigned short,unsigned short,unsigned short,unsigned long,short,short);
unsigned short  __cdecl msm_getstatus(unsigned short *,unsigned short *);
short           __cdecl msm_getpress(unsigned short *,unsigned short *,unsigned short *);
short           __cdecl msm_getrelease(unsigned short *,unsigned short *,unsigned short *);
void            __cdecl msm_signal(unsigned short,
		void (__cdecl *)(unsigned short,unsigned short,short,short),void *);
void            __cdecl msm_reversebuttonon(void);
void            __cdecl msm_reversebuttonoff(void);
short           __cdecl msm_reversebuttonis(void);
short           __cdecl msm_reversebutton(short);
#endif

#if __cplusplus
}
#endif

#endif /* __NT__ */

#endif
