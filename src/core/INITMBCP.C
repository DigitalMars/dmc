/* Copyright (C) 1995 by Digital Mars
 * All Rights Reserved
 */
 


void (*__init_mbctype_ptr)(void);

void   __initmbctype(void)
{
    if (__init_mbctype_ptr)
      (*__init_mbctype_ptr)();
}


