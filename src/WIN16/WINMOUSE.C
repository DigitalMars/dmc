/*	winmouse.c
 *	Adds the msm functions that are practical under Windows 3.0
 *
 *	(C) Copyright 1991-1993 Digital Mars.
 *	All Rights Reserved
 *	Written by: G. Eric Engstrom
 */

#ifdef _WINDOWS
#include	<stdio.h>
#include	<string.h>
#include	<msmouse.h>
#include	"windows.h"

typedef unsigned long  ulong;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned char  uchar;

#define TK_LEFT 1
#define TK_RIGHT 2
#define TK_SHIFT 4
#define TK_CONTROL 8
#define TK_MIDDLE 16

#pragma pack(1)

static short msm_reversebuttoncount = 0;

typedef struct TPOINT
  {
  short   x;
  short   y;
  }
Point_t;

typedef union TLONG
  {
    ulong   l;
  Point_t   w;
  }
Long_t;

typedef struct TMOUSE
  {
  ushort   init;
  ushort   signal;
  ushort   mask;
  ushort   buttonstatus;
  ushort   wnd;
  ushort   testMask;
  Long_t   w;
  ushort   pressCount[3];
  ushort   releaseCount[3];
   void    (__cdecl * ISR)(ushort,ushort,short,short);
  }
Mouse_t;

static ushort mask [] =
  {
  TK_LEFT,
  TK_RIGHT,
  TK_MIDDLE,
  };
static ushort bits [] =
  {
  MSM_SIGNALBUTTONDOWNL,
  MSM_SIGNALBUTTONDOWNR,
  MSM_SIGNALBUTTONDOWNM,
  };

static Mouse_t old;
static Mouse_t cur;

short __cdecl msm_init(void)
{
    memset(&cur,0,sizeof(Mouse_t));
    return (cur.init = 1);
}

void __cdecl msm_term(void)
{
    cur.init = 0;
}

ushort __cdecl msm_getstatus(ushort *wX,ushort *wY)
{
    if (cur.init)
      {
      *wX = cur.w.w.x;
      *wY = cur.w.w.y;
      return cur.buttonstatus;
      }
    else
      {
      *wX =
      *wY = -1;
      return 0;
      }
}

/* initializes signal function pointer and tells messagemonitor to check for
 * signal condition
 */
void __cdecl msm_signal(ushort wMask,void (__cdecl *wISR)(ushort,ushort,short,short),void *wStack)
{
    /* ignore wStack, it is not used */
    if (cur.init
      && wMask != 0)
      {
      cur.signal = 1;
      cur.ISR    = wISR;
/*
      if (msm_reversebuttoncount > 0)
        {
        cur.mask   = (wMask - (wMask & 30)) | ((wMask & 24) >> 2) | ((x & 6) << 2);
        }
      else
 */
        {
        cur.mask   = wMask;
        }
      }
    else
      {
      cur.signal = 0;
      cur.ISR    = (void (__cdecl *)(ushort,ushort,short,short))NULL;
      cur.mask   = 0;
      }
}

short __cdecl msm_getpress(ushort *wCount,ushort *wX,ushort *wY)
{
    if (cur.init)
      {
      unsigned short a;

      a = *wCount;
      *wCount = cur.pressCount[a];
      cur.pressCount[a] = 0;
      cur.w.w.x = *wX;
      cur.w.w.y = *wY;
      return cur.buttonstatus;
      }
    else
      return 0;
}

short __cdecl msm_getrelease(ushort *wCount,ushort *wX,ushort *wY)
{
    if (cur.init)
      {
      unsigned short a;

      a = *wCount;
      *wCount = cur.releaseCount[a];
      cur.releaseCount[a] = 0;
      cur.w.w.x = *wX;
      cur.w.w.y = *wY;
      return cur.buttonstatus;
      }
    else
      return 0;
}

ushort __cdecl msm_hwndget(void)
{
    return cur.wnd;
}

void __cdecl msm_reversebuttonon(void)
{
    if (++msm_reversebuttoncount > 0)
      {
      mask [0] = TK_RIGHT;
      mask [1] = TK_LEFT;
      }
}

void __cdecl msm_reversebuttonoff(void)
{
    if (--msm_reversebuttoncount <= 0)
      {
      mask [1] = TK_RIGHT;
      mask [0] = TK_LEFT;
      }
}

short __cdecl msm_reversebuttonis(void)
{
    return msm_reversebuttoncount;
}

short __cdecl msm_reversebutton(short x)
{
    if (msm_reversebuttoncount > 0)
      {
      return (x - (x & 3)) | ((x & 2) >> 1) | ((x & 1) << 1);
      }
    else
      {
      return x;
      }
}

/* this function is placed in the message loop to keep the mouse
 * data structure current
 * the only mouse message it does not process is the MOUSEACTIVE message
 * which is used by windows to activate an inactive window, since the
 * cursor position is not contained in this message there didn't seem to
 * be anything to do with it.
 */
ushort __cdecl msm_messagemonitor(ushort hWnd,ushort wMessage,ushort wParam,ulong lParam,short xBase,short yBase)
{
    if (cur.init)
      {
      switch (wMessage)
        {
        case WM_COMMAND:
          {
          if (wParam == MSM_SCROLLUP
            || wParam == MSM_SCROLLDOWN)
            goto _merge;
          break;
          }
        case WM_MOUSEMOVE:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
          {
          ushort   a;
          ushort   work;

          memcpy(&old,&cur,sizeof(Mouse_t));
          cur.testMask = 0;
          cur.wnd = hWnd;
          cur.buttonstatus = wParam;
          cur.w.w.x = (short)(LOWORD(lParam)) + xBase;
          cur.w.w.y = (short)(HIWORD(lParam)) + yBase;
          work = (old.buttonstatus ^ cur.buttonstatus) & ~(TK_CONTROL|TK_SHIFT); /* have buttons changed */
          for (a = 0;
             a < 3;
               ++a)
            {
            if (work & mask[a])	/* button changed state */
              {
              ushort   b;

              if ((b = (cur.buttonstatus & mask[a])) != 0)
                {
                ++cur.pressCount[a];		/* count down motions for button */
                cur.testMask |= bits[a] << 1;	/* update signal mask */
                }
              else
                {
                ++cur.releaseCount[a];		/* count up motions for button */
                cur.testMask |= bits[a];		/* update signal mask */
                }
              }
            }
          cur.testMask |= (cur.w.l != old.w.l) ? MSM_SIGNALMOVE : 0;
          } /* fall through */
        _merge:
	  {
          if (cur.signal
            && cur.mask & cur.testMask)
            {
            (*cur.ISR)(cur.testMask,cur.buttonstatus,cur.w.w.x,cur.w.w.y);	/* call signal function if mask event triggered */
            }
          return 1;	/* mouse message */
          }
        }
      }
    return 0;	/* non mouse message */
}

#pragma pack()
#endif
