/**
 * fatexit.c
 * Copyright (C) 1985-2017 by Digital Mars
 * All Rights Reserved
 * Author: Walter Bright
 * License: Boost
 * Source: https://github.com/DigitalMars/dmc/blob/master/src/core/fatexit.c
 */

#include <stdlib.h>
#include <scrtl.h>

typedef void (_32BIT *fp_t)(void);

struct AtexitOverflow
{
    fp_t *tbl;
    int i;
    int length;
};

#if _WIN32
static struct AtexitOverflow atexit_overflow;

static void _32BIT atexit_overflow_func()
{
    struct AtexitOverflow* a = &atexit_overflow;
    for (int i = a->i; i; --i)
    {
	fp_t fp = a->tbl[i];
	if (fp)
	    fp();
    }
}
#endif

/********************************
 * Register function to be called by exit().
 * The functions are called in the reverse order that they are registered.
 * A maximum of _atexit_max - 1 functions can be registered.
 * Returns:
 *     0       succeeds
 *     !=0     failed to register the function
 */

int _32BIT _fatexit(fp_t func)
{
    if (!_atexitp)			// initialize
       _atexitp = _atexit_tbl;

#if _WIN32
    /* Allow unlimited atexit() registrations by allocating
     * the table with malloc()
     */
    if (_atexitp < _atexit_tbl + _atexit_max - 2)  // if more than one entry left
	*++_atexitp = func;		// [0] is left NULL as a sentinal for the end
    else
    {
	_atexitp[1] = &atexit_overflow_func;
        struct AtexitOverflow* a = &atexit_overflow;
	if (a->i == a->length)
	{
	    int length = 8 + a->length * 2;
	    fp_t *tbl = (fp_t *)realloc(a->tbl, length * sizeof(a->tbl[0]));
	    if (!tbl)
		return 1;	// failed
	    a->length = length;
	    a->tbl = tbl;
	}
	a->tbl[a->i] = func;
	++a->i;
    }
#else
    if (_atexitp >= _atexit_tbl + _atexit_max - 1)
       return 1;
/* Position 0 is left NULL as a sentinal for the end            */
    *++_atexitp = func;
#endif
    return 0;
}
