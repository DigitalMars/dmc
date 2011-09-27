/*_ jmp.c    
 * Copyright (C) 1995 by Digital Mars
 * All Rights Reserved
 */


#include <setjmp.h>


#ifdef _setjmp
#undef _setjmp
#endif

int _setjmp(jmp_buf env)

{
   Catch(env);
}

#ifdef longjmp
#undef longjmp
#endif

void longjmp(jmp_buf env, int retval)
{

	 Throw(env,retval);
}
