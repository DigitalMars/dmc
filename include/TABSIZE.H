/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __TABSIZE_H
#define __TABSIZE_H 1   

#ifdef __cplusplus
extern "C"
  {
#endif

          void __cdecl tab_sizeset(unsigned short newtabsize);
          void __cdecl tab_sizeputenv(unsigned short newtabsize);
unsigned short __cdecl tab_sizeget(void);
unsigned short __cdecl tab_sizegetenv(void);

extern unsigned short _tab_size;

#ifdef __cplusplus
  }
#endif

#endif
