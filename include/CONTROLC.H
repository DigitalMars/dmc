/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __CONTROLC_H
#define __CONTROLC_H 1

#if __cplusplus
extern "C"
  {
#endif
void __cdecl controlc_open(void);
void __cdecl controlc_close(void);
typedef void (__cdecl * _controlc_handler_t)(void);
_controlc_handler_t __cdecl _controlc_handler;
#if __cplusplus
  }
#endif

#endif
