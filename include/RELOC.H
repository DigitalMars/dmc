/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif


#ifndef __RELOC_H
#define __RELOC_H 1

/* Used when dealing with COFF files. */

#if __cplusplus
extern "C" {
#endif

#pragma pack(2)

struct reloc
{
  long r_vaddr;
  long r_symndx;
  unsigned short r_type;
};

#define	RELOC	struct reloc
#define RELSZ   10


#pragma pack()

#define R_ABS        0
#define R_DIR32      6
#define R_OFF8       7
#define R_OFF16      8
#define R_SEG12      9
#define R_DIR32S  0x0A
#define R_AUX     0x0B
#define R_RELBYTE 0x0F
#define R_RELWORD 0x10
#define R_RELLONG 0x11
#define R_PCRBYTE 0x12
#define R_PCRWORD 0x13
#define R_PCRLONG 0x14

#if __cplusplus
}
#endif

#endif
