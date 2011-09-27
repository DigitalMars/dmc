/* Copyright (C) 1992-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __CERROR_H
#define __CERROR_H 1

#ifndef __NT__

#if __cplusplus
extern "C"
  {
#endif

void __cdecl cerror_open (void);
void __cdecl cerror_close (void);
void __cdecl _cerror_handler (unsigned short *ax, unsigned short *di);

#if __cplusplus
  }
#endif

#endif /* __NT__ */

#endif
