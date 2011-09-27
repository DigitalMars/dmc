/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __LINENUM_H
#define __LINENUM_H 1
/* Used when dealing with COFF files. */

#if __cplusplus
extern "C" {
#endif

#define	LINENO	struct lineno
#define LINESZ  6

#pragma pack(2)
struct lineno
{
  union
  {
    long l_symndx;
    long l_paddr;
  }l_addr;
  unsigned short l_lnno;
};
#pragma pack()

#if __cplusplus
}
#endif

#endif
