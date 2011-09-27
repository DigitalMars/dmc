/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __EMM_H
#define __EMM_H 1

/* not supported in 32-bit models */
#if !defined(_WIN32) && !defined(DOS386)

#if __cplusplus
extern "C" {
#endif

#define EMM_PAGESIZE    0x4000

extern int __cdecl emm_inited;
int __cdecl emm_init(void);
unsigned __cdecl emm_getunalloc(void);
unsigned __cdecl emm_gettotal(void);
int __cdecl emm_allocpages(unsigned);
void __cdecl emm_maphandle(int handle,unsigned logical,unsigned physical);
void __cdecl emm_savepagemap(int handle);
void __cdecl emm_restorepagemap(int handle);
void far * __cdecl emm_physpage(int);
void __cdecl emm_term(void);

#pragma pack(__DEFALIGN)
struct emm_handle_s
{   int handle;
    int pages;
};
#pragma pack()

int __cdecl emm_gethandlespages(struct emm_handle_s *p);
int __cdecl emm_gethandlecount(void);
void __cdecl emm_deallocpages(int handle);
int __cdecl emm_getversion(void);
unsigned __cdecl emm_getpagemapsize(void);
void __cdecl emm_getpagemap(void *dst);
void __cdecl emm_setpagemap(void *src);
void __cdecl emm_getsetpagemap(void *dst,void *src);

#if __cplusplus
}
#endif

#endif /* __NT__  && DOS386 */

#endif
