/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 */

#include <stdlib.h>

#ifdef DOS386
extern long _x386_break; 
#endif

__declspec(naked) size_t stackavail()
{
	__asm {
#ifdef DOS386
          mov     eax,_x386_break  /* In DOSX, _x386_break contains stack bottom */
#else
          mov     eax,FS:8  /* in NT, FS:8 is bottom of stack for this thread */
#endif
          neg     eax
          add     eax,esp         
          ret
      }
}
