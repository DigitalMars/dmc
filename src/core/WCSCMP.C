//_ wcscmp.c
// Copyright (C) 2001 by Digital Mars
// All Rights Reserved
// Written by Walter Bright
// www.digitalmars.com

#include <wchar.h>
#include <string.h>

int __CLIB wcscmp(const wchar_t *ws1, const wchar_t *ws2)
{   int c;
    int i;

    i = 0;
    while (1)
    {	int s1;

	s1 = (short)ws1[i];
	c = s1 - (short)ws2[i];
	if (c || !s1)
	    break;
	i++;
    }
    return c;
}

