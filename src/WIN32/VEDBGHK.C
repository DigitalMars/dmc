/*_ vedgbhk.c  
 * Copyright (C) 1995 by Digital Mars
 * All rights reserved.
 * $Revision:     1.0
 * 
 * Win32 Debug Hook code
 */



typedef void (*DEBUGHOOK_PTR)(void);

DEBUGHOOK_PTR __ve_debughook;


void __callve()
{
    if(__ve_debughook)
        __ve_debughook();
}





