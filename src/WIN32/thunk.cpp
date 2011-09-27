/*_ thunk.cpp
 * Copyright (C) 2005 by Digital Mars, www.digitalmars.com
 * All rights reserved.
 * Written by Walter Bright
 */

/* Code to dynamically adjust thunks for pointer-to-members.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

extern "C"
{
    void _rtl_critical_enter();
    void _rtl_critical_exit();
}

__mptr __cdecl __genthunk(unsigned d, unsigned ty, __mptr m)
{
    //printf("__genthunk(m = %p, ty = x%x, d = %d)\n", m, ty, d);
    char p[13];
    if (ty == 0x33)
    {
	p[0] = 0x81;
	p[1] = 0xC1;
	*(int*)(p + 2) = d;

#if 0
	// jmp m
	p[6] = 0xE9;
	*(unsigned *)(p + 7) = *(unsigned *)&m - ((unsigned)p + 11);
	return reinterpret_cast<__mptr>(p);
#else
	// push m; ret
	p[6] = 0x68;
	*(__mptr*)(p + 7) = *(__mptr *)&m;
	p[11] = 0xC3;
	p[12] = 0;
#endif
    }
    else if (ty == 0x34)
    {
	p[0] = 0x05;
	*(int*)(p + 1) = d;
	p[5] = 0x68;
	*(__mptr*)(p + 6) = *(__mptr *)&m;
	p[10] = 0xC3;
	p[11] = 0;
	p[12] = 0;
    }
    else
    {
	p[0] = 0x81;
	p[1] = 0x04;
	p[2] = 0x24;
	*(int*)(p + 3) = d;
	p[7] = 0x68;
	*(__mptr*)(p + 8) = *(__mptr *)&m;
	p[12] = 0xC3;
    }

    static void **thunklist;
    char *q;

    for (void **pt = thunklist; pt; pt = (void**)*pt)
    {   q = (char *)pt + 4;
	if (memcmp(q, p, 13) == 0)
	    goto Lret;
    }
    q = (char *)malloc(4 + 13);
    memcpy(q + 4, p, 13);

    _rtl_critical_enter();
    *(void**)q = thunklist;
    thunklist = (void**)q;
    _rtl_critical_exit();

    q += 4;

Lret:
    return reinterpret_cast<__mptr>(q);
}
