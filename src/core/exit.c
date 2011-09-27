/*_ exit.c   Sat Sep    1 1990   Modified by: Walter Bright */
/* Copyright (C) 1985-1994 by Digital Mars */
/* All Rights Reserved                         */
/* $Revision: 1.1.1.3 $ */

#include <process.h>
#include <stdlib.h>
#include <scrtl.h>
#if __INTSIZE == 2 && (__SMALL__ || __COMPACT__)
#include <dos.h>
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#ifdef _WINDLL
void (*_DumpMemLeaksp)(void);
#elif !defined(_DLL)
void __DumpMemLeaks(void);
#endif
#endif

static int full    = 0;
static int partial = 0;
int __terminate_done = 0;  // Set when exit is called.

void (*_fcloseallp)(void);


/**********************
 * Close all open files and exit.
 */

void _cexit (void)
{
    full = 1;
    partial = 0;
    exit(0);
}

void _c_exit(void)
{
    partial = 1;
    full = 0;
    exit(0);
}

void exit (int errstatus)
{
    __terminate_done = 1;  // Ok, termination done.
    if (!partial)
    {
        if (_atexitp)                      /* if any registered functions  */
            while (*_atexitp)                /* while not end of array       */
            {
            #if __INTSIZE == 2 && (__SMALL__ || __COMPACT__)
                if (FP_SEG(*_atexitp) == 0)
                    (*(void(*)(void))*_atexitp)();
                else
                    (**_atexitp)(); /* call registered function        */
            #else
                (**_atexitp)(); /* call registered function   */
            #endif
                _atexitp--;
            }
    }

    if (_hookexitp)                        /* supports exitstat.h  */
        (*_hookexitp)(errstatus);          /* always calls exit_exit in exitstat.h */
    else
    {
        _dodtors();                        /* call static destructors */
#ifdef _DEBUG
#ifdef  _WINDLL
        /* Dump all memory leaks */
       if (_DumpMemLeaksp)
       {
            (*_DumpMemLeaksp)();
       }
#elif !defined(_DLL)
        __DumpMemLeaks();
#endif
#endif  /* _DEBUG */

        if (!(partial))
        {
            if (_fcloseallp)
                (*_fcloseallp)();          /* close any streams  */

            if (!(full))
                _Exit(errstatus);
        }
    }
    partial = 0;
    full = 0;
}
