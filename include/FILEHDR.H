/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __FILEHDR_H
#define __FILEHDR_H 1
/* Used when dealing with COFF files. */

#if __cplusplus
extern "C" {
#endif

#pragma pack(2)

#define FILHDR struct filehdr
#define FILHSZ sizeof(FILHDR)

struct filehdr
{
  unsigned short f_magic;

#define IAPX16          0x144
#define IAPX16TV        0x145
#define IAPX20          0x146
#define IAPX20TV	0x147
#define X86MAGIC	0x148
#define XTVMAGIC	0x149
#define I286SMAGIC      0x14A
#define I286LMAGIC	0x152
#define I386MAGIC       0x14C

  unsigned short f_nscns;
  long f_timdat;
  long f_symptr;
  long f_nsyms;
  unsigned short f_opthdr;
  unsigned short f_flags;

#define F_RELFLG 1
#define F_EXEC   2
#define F_LNNO   4
#define F_LSYMS  8
#define F_MINMAL 0x10
#define F_UPDATE 0x20
#define F_SWABD  0x40
#define F_AR16WR 0x80
#define F_AR32WR 0x100
#define F_AR32W  0x200
#define F_PATCH  0x400
#define F_NODF   0x400

#define F_80186  0x1000
#define F_80286  0x2000

};

#define ISCOFF(x) (((x)==I386MAGIC) || ((x)==X86MAGIC) || ((x)==XTVMAGIC) ||\
		   ((x)==I286SMAGIC)|| ((x)==I286LMAGIC))

#pragma pack()

#if __cplusplus
}
#endif

#endif
