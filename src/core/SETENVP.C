/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 *
 * setenvp.c
 * Entry:
 *   _setenvp
 * Notes:
 *   This entry is intended for use by the C initialization routines.
 *   They are not part of the published RTL interface.
 */

int (* __init_environ_ptr)(void);

void _setenvp()
{

   if (__init_environ_ptr)
     (*__init_environ_ptr)();
}


