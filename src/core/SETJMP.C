/*_ setjmp.c    
 * Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 */
#include <setjmp.h>

#ifdef setjmp
#undef setjmp
#endif

#ifdef _setjmp
#undef _setjmp
#endif

__declspec(naked) int setjmp(jmp_buf enf)
{
   __asm {
          jmp _setjmp
   }
}
