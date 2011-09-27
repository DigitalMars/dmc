/*_ purevirt.c  
 * Copyright (C) 1995 by Digital Mars
 * All Rights Reserved
 */


void __cdecl __faterr(char*);

/*
   _purecall is an entry point necessary for
   Microsoft Compatability. This routine simply
   puts up the message "Pure Virtual Function Call"
   and exits
*/

void _purecall(void)
{
    __faterr("pure virtual function called");
}    

