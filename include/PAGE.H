/* Copyright (C) 1992-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __PAGE_H
#define __PAGE_H 1

#if __cplusplus
extern "C" {
#endif

#if __INTSIZE == 4
#define __BF
#else
#define __BF far
#endif

#define __PGOFF		sizeof(unsigned short)

#pragma pack(1)

struct Pageheader
{
    unsigned short pagesize;
    unsigned short maxsize;
    unsigned short allocp;
    unsigned short bassize;
    unsigned short baslnk;
};

#pragma pack()

#define PAGEOVERHEAD	sizeof(struct Pageheader)

unsigned __cdecl page_calloc(void __BF *baseptr,unsigned size);
unsigned __cdecl page_malloc(void __BF *baseptr,unsigned size);
unsigned __cdecl page_realloc(void __BF *baseptr,unsigned p, unsigned nbytes);
int __cdecl page_free(void __BF *baseptr,unsigned p);
unsigned __cdecl page_maxfree(void __BF *baseptr);
unsigned __cdecl page_initialize(void __BF *baseptr,unsigned pagesize);
#define page_size(baseptr,p) \
	(*(unsigned short __BF *)((char __BF *)(baseptr) + (p) - __PGOFF) - __PGOFF)
#define page_toptr(baseptr,p)	(void __BF *)((char __BF *)(baseptr) + (p))

#if __cplusplus
}
#endif

#endif
