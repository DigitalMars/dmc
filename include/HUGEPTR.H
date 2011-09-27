/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __HUGEPTR_H
#define __HUGEPTR_H 1

#if __cplusplus
extern "C" {
#endif

#if __INTSIZE == 4

#define hugeptr_cmp(h1,h2) ((h1) - (h2))
#define hugeptr_diff(h1,h2) ((h1) - (h2))
#define hugeptr_add(h1,offset) ((h1) + (offset))

#else

long __pascal hugeptr_diff(void __far *h1,void __far *h2);
void __far * __pascal hugeptr_add(void __far *h1,long offset);

#define hugeptr_cmp(h1,h2) ((long)(h1) - (long)(h2))
#define hugeptr_diff(h1,h2) (hugeptr_diff((h1),(h2)) / sizeof(*h1))
#define hugeptr_add(h,offset) (hugeptr_add(h,(long)offset * sizeof(*h)))

#endif

#if __cplusplus
}
#endif

#endif
