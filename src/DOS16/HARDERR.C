/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#if __INTSIZE == 2 && defined (MSDOS)

#include <dos.h>
#include <setjmp.h>

#define FIRST_TIME             0
#define JUMPED_FROM_HARDRESUME 1
#define JUMPED_FROM_HARDRETN   2

static jmp_buf back;
static user_code;
static void (__far *user_handler) (unsigned, unsigned, unsigned __far *);

static void __interrupt __far our_handler (
   unsigned saved_es, unsigned saved_ds,
   unsigned saved_di, unsigned saved_si, unsigned saved_bp, unsigned saved_sp,
   unsigned saved_bx, unsigned saved_dx, unsigned saved_cx, unsigned saved_ax,
   unsigned saved_ip, unsigned saved_cs, unsigned saved_flags,
   unsigned saved_dos_ax, unsigned saved_dos_bx, unsigned saved_dos_cx, unsigned saved_dos_dx,
   unsigned saved_dos_si, unsigned saved_dos_di, unsigned saved_dos_bp,
   unsigned saved_dos_ds, unsigned saved_dos_es,
   unsigned saved_dos_ip, unsigned saved_dos_cs, unsigned saved_dos_flags)
{
  switch (setjmp (back))
  {
  case FIRST_TIME:
    (*user_handler) (saved_ax, saved_di, _MK_FP (saved_bp, saved_si));
    return;
  case JUMPED_FROM_HARDRESUME:
    saved_ax = user_code; /* DOS will look at this to determine what to do */
    return;
  case JUMPED_FROM_HARDRETN: /* return error directly to caller */
    if (saved_dos_ax >= 0x3800) /* DOS funcs 38 and above set AX and carry flag */
    {
      saved_dos_ax = user_code;
      saved_dos_flags |= 1;
    }
    else
      switch (saved_dos_ax >> 8) /* for these DOS funcs only, set AL to FF on error */
      {
      case 0x0f: /* open */
      case 0x10: /* close */
      case 0x11: /* find first */
      case 0x12: /* find next */
      case 0x13: /* delete */
      case 0x16: /* creat */
      case 0x17: /* rename */
      case 0x1d: /* spec drive allocation info */
      case 0x23: /* file size */
      case 0x29: /* parse fn */
      case 0x2b: /* set dt */
      case 0x2d: /* set tm */
      case 0x33: /* ctrl brk */
      case 0x36: /* disk space */
        *(unsigned char *)&saved_dos_ax = 0xff;
        break;
      }
    asm
    {
        mov    ah,30h
        int    21h       /* calling a function > C puts DOS back into a "stable state" */
        add    sp,28     /* discard saved_es thru saved_flags values */
        pop    ax        /* restore saved_dos_* registers */
        pop    bx
        pop    cx
        pop    dx
        pop    si
        pop    di
        pop    bp
        pop    ds
        pop    es
        iret
    }
  }
}

void _harderr (void (__far *func) (unsigned ax, unsigned di, unsigned __far *bpsi))
{
  user_handler = func;
  _dos_setvect (0x24, (void (__interrupt __far *) ())our_handler);
}

void _hardresume (int code)
{
  user_code = code;
  longjmp (back, JUMPED_FROM_HARDRESUME);
}

void _hardretn (int error)
{
  user_code = error;
  longjmp (back, JUMPED_FROM_HARDRETN);
}

#endif
