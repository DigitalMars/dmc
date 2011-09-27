/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __WINC_H
#define __WINC_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>

#ifdef __SC__

typedef unsigned short (* __pascal Hook_t)(char __far *,unsigned short,unsigned short);
extern int winc_startmode;
typedef struct TSTDIO
  {
  unsigned short	X;
  unsigned short	Y;
  HWND		HWnd;
  HWND		PrevHWnd;
  HDC		HDc;
  unsigned short	TextXSize;
  unsigned short	TextYSize;
  POINT		Size;
  POINT		Scroll;
  HBRUSH	SolidBrush;
  HFONT		Font;
  RECT		ClientRect;
  RECT		BottomLineRect;
  Hook_t	HookFunction;
  }
StdIO_t;

#define TAB_FLG 2
#define RET_FLG 1

#define StdOutSet(A)     StdIOSet(&StdOutW,A)
#define StdErrSet(A)     StdIOSet(&StdErrW,A)
#define StdOutGet()      StdIOGet(&StdOutW)
#define StdErrGet()      StdIOGet(&StdErrW)
#define StdOutRelease()  StdIORelease(&StdOutW)
#define StdErrRelease()  StdIORelease(&StdErrW)
#define StdOutHookSet(A) StdIOHook(&StdOutW,A)
#define StdErrHookSet(A) StdIOHook(&StdErrW,A)
#define StdOutHookGet()  StdIOHook(&StdOutW)
#define StdErrHookGet()  StdIOHook(&StdErrW)
#define StdOutClear()    StdIOClear(&StdOutW)
#define StdErrClear()    StdIOClear(&StdErrW)
#define StdOutFontSet(A)  StdIOFontSet(StdOutW,A)
#define StdErrFontSet(A)  StdIOFontSet(StdErrW,A)
#define StdOutFontGet(A)  StdIOFontGet(StdOutW)
#define StdErrFontGet(A)  StdIOFontGet(StdErrW)

#ifndef NOMSG
void   __pascal MessageFatalError(char __far *);
void   __pascal MessageLoop(MSG __far *);
#endif

#ifdef __cplusplus

void   __pascal StdIOSet(class StdIOC *,HWND);
unsigned short __pascal StdIOGet(class StdIOC *);
void   __pascal StdIORelease(class StdIOC *);
void   __pascal StdIOHookSet(class StdIOC *,Hook_t);
Hook_t __pascal StdIOHookGet(class StdIOC *);
void   __pascal StdIOClear(class StdIOC *);
void   __pascal StdIOFontSet(class StdIOC *, HFONT);
HFONT  __pascal StdIOFontGet(class StdIOC *);

class StdIOC
  {
  protected:
 StdIO_t   h;

  public:
    void	   Set(HWND x)        { StdIOSet(this,x); }
  unsigned short   Get(void)          { return StdIOGet(this); }
    void	   Release(void)      { StdIORelease(this); }      
    void	   HookSet(Hook_t x)  { StdIOHookSet(this,x); }
  Hook_t	   HookGet(void)      { return StdIOHookGet(this); }
  unsigned short   LineCountGet(void) { return h.Size.y/h.TextYSize; }
    void	   CursorSet(unsigned short x,unsigned short y) { h.X = x; h.Y = y; }
    void	   FontSet(HFONT f)   { StdIOFontSet(this,f); }
  HBRUSH	   BrushBackGroundGet(void)     { return h.SolidBrush; }
    RECT	  *ClientRectGet(void)          { return &h.ClientRect; }
    RECT	  *BottomLineRectGet(void)      { return &h.BottomLineRect; }
    HWND	   WndGet(void)       { return h.HWnd; }
   HFONT	   FontGet(void)      { return h.Font; }
  unsigned short   TextXGet(void)     { return h.TextXSize; }
  unsigned short   TextYGet(void)     { return h.TextYSize; }
  unsigned short   SizeYGet(void)     { return h.Size.y; }
    void	   Clear(void)        { StdIOClear(this); }
  };

extern StdIOC   StdOutW;
extern StdIOC   StdErrW;

#else

void   __pascal StdIOSet(StdIO_t *,HWND);
unsigned short __pascal StdIOGet(StdIO_t *);
void   __pascal StdIORelease(StdIO_t *);
void   __pascal StdIOHookSet(StdIO_t *,Hook_t);
Hook_t __pascal StdIOHookGet(StdIO_t *);
void   __pascal StdIOClear(StdIO_t *);
void   __pascal StdIOFontSet(StdIO_t *, HFONT);
HFONT  __pascal StdIOFontGet(StdIO_t *);

extern StdIO_t   StdOutW;
extern StdIO_t   StdErrW;

#endif  /* cplusplus */
#endif  /* _SC_ */

#ifdef __cplusplus
}
#endif

#endif
