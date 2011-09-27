/* Copyright (C) 1994 by Digital Mars		*/
/* All Rights Reserved				*/
/* March 10, 1994  */

#ifdef MSDOS

#include <dos.h>
#include <process.h>

extern void(*_fcloseallp)(void);

void _dos_keep(unsigned retcode, unsigned memsize)
{
  union _REGS inregs, outregs;
  
  _cexit();
  if (_fcloseallp)
    (*_fcloseallp)();
  inregs.h.ah = 0x31;
  inregs.h.al = (unsigned char)retcode;
  inregs.x.dx = memsize;
  _intdos(&inregs, &outregs);
}

#endif
