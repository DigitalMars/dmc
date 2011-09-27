/*_ _new2.cpp  Thurs Jan 12, 1995 */
/* Copyright (C) 1995 by Digital Mars	*/
/* All Rights Reserved					*/

#include <new.h>

/************
 * Overload of the operator new to take 2 arguments.
 * This overload is provided per the ANSI C++ Standard.
 */
void *operator new(unsigned, void *p) throw()
{
	return (p);
}


