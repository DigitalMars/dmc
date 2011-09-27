/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SWAP_H
#define __SWAP_H 1

#define SWAP_FREEMEMERROR -2
#define SWAP_NOVMSPACE    -3

#ifdef __cplusplus
extern "C"
  {
#endif
 int __cdecl swap_ison(void);
 int __cdecl swap_istempcheckon(void);
 int __cdecl swap_iswindowon(void);
 int __cdecl swap_ispipeon(void);
 int __cdecl swap_isfreeparagraphson(void);
 int __cdecl swap_istrapcbreakon(void);
 int __cdecl swap_isclearkeyboardon(void);
void __cdecl swap_on(void);
void __cdecl swap_off(void);
void __cdecl swap_tempcheckon(void);
void __cdecl swap_tempcheckoff(void);
void __cdecl swap_window(int col,int lin,int x,int y);
void __cdecl swap_windowon(void);
void __cdecl swap_windowoff(void);
void __cdecl swap_pipe(const char *file);
void __cdecl swap_pipeon(void);
void __cdecl swap_pipeoff(void);
void __cdecl swap_freeparagraphs(unsigned int sz);
void __cdecl swap_freeparagraphson(void);
void __cdecl swap_freeparagraphsoff(void);
void __cdecl swap_trapcbreakon(void);
void __cdecl swap_trapcbreakoff(void);
void __cdecl swap_clearkeyboardon(void);
void __cdecl swap_clearkeyboardoff(void);
#ifdef __cplusplus
  }
#endif

#endif
