/* Copyright (C) 1986-1994 by Digital Mars. */


void __cdecl __faterr(char *);

/*
   _fpurecall is an entry point necessary for
   Microsoft Compatability. This routine simply
   puts up the messate "Pure Virtual Function Call"
   and exits
*/

void _fpurecall(void)
{
    __faterr("pure virtual function call");
}    
