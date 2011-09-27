/*_ corenew.cpp   */
/* Copyright (C) 1995 by Digital Mars */
/* All Rights Reserved     */
/* Written by Walter Bright    */

#include <stdlib.h>
#include <new.h>

/********************
 * _new_handler can be modified to point to a function in case
 * the allocation fails. _new_handler can attempt to repair things.
 */

extern _PNH _new_handler;
extern int __new_handler_type;

#define ANSI_COMPAT 1
#define MS_COMPAT 2

/*****************************
 * The standard C++ storage allocator.
 * Calls (*_new_handler)() if fails. If still fails, it returns NULL.
 */

extern "C" int MallocSem;

void *__do_newalloc(size_t m_size)
{
    void *p;
    int keep_trying = 1;

    while (keep_trying)
    {
        if (m_size == 0)
            m_size++;
        p = malloc(m_size);
        if (p != NULL || _new_handler == NULL)
            break;
        switch (__new_handler_type)
        {
            case ANSI_COMPAT:
                (*(PFVV) _new_handler) ();
                keep_trying = 1;
                break;
            case MS_COMPAT:
                keep_trying = (*_new_handler) (m_size);
                break;
        }
    }
Done:
    return p;
}
