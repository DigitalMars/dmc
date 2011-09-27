/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SYMS_H
#define __SYMS_H

/* Used when dealing with COFF files. */

#if __cplusplus
extern "C" {
#endif

#pragma pack(2)

struct syment
{
#define SYMNMLEN 8
  union
  {
    char  _n_name[SYMNMLEN];
    struct
    {
      long _n_zeroes;
      long _n_offset;
    } _n_n;
    char *_n_nptr[2];
  } _n;
  long n_value;
  short n_scnum;
  unsigned short n_type;
  char n_sclass;
  char n_numaux;
};

#define n_name _n._n_name
#define n_nptr _n._n_nptr[1]
#define n_zeroes _n._n_n._n_zeroes
#define n_offset _n._n_n._n_offset

#define N_UNDEF 0
#define N_ABS   -1
#define N_DEBUG -2
#define N_TV    -3
#define P_TV    -4

#define _EF    ".ef"

#define T_NULL   0
#define T_ARG    1
#define T_CHAR   2
#define T_SHORT  3
#define T_INT    4
#define T_LONG   5
#define T_FLOAT  6
#define T_DOUBLE 7
#define T_STRUCT 8
#define T_UNION  9
#define T_ENUM   10
#define T_MOE    11
#define T_UCHAR  12
#define T_USHORT 13
#define T_UINT   14
#define T_ULONG  15

#define DT_NON 0
#define DT_PTR 1
#define DT_FCN 2
#define DT_ARY 3

#define N_BTMASK 017
#define N_TMASK  060
#define N_TMASK1 0300
#define N_TMASK2 0360
#define N_BTSHFT 4
#define N_TSHIFT 2

#define BTYPE(A) ((A)&N_BTMASK)
#define ISPTR(A) (((A)&N_TMASK)==(DT_PTR<<N_BTSHFT))
#define ISFCN(A) (((A)&N_TMASK)==(DT_FCN<<N_BTSHFT))
#define ISARY(A) (((A)&N_TMASK)==(DT_ARY<<N_BTSHFT))
#define ISTAG(A) ((A)==C_STRTAG||(A)==C_UNTAG||(A)==C_ENTAG)
#define INCREF(A) ((((A)&~N_BTMASK)<<N_TSHIFT)|(DT_PTR<<N_BTSHFT)|(A&N_BTMASK))
#define DECREF(A) ((((A)>>N_TSHIFT)&~N_BTMASK)|((A)&N_BTMASK))

union auxent
{
  struct
  {
    long x_tagndx;
    union
    {
      struct
      {
        unsigned short x_lnno;
        unsigned short x_size;
      } x_lnsz;
      long x_fsize;
    } x_misc;
    union
    {
      struct
      {
        long x_lnnoptr;
        long x_endndx;
      }x_fcn;
      struct
      {
#define DIMNUM 4
        unsigned short  x_dimen[DIMNUM];
      } x_ary;
    }x_fcnary;
    unsigned short x_tvndx;
  } x_sym;
  struct
  {
#define FILNMLEN 14
    char x_fname[FILNMLEN];
  } x_file;
  struct
  {
    long x_scnlen;
    unsigned short x_nreloc;
    unsigned short x_nlinno;
  } x_scn;
  struct
  {
    long x_tvfill;
    unsigned short x_tvlen;
    unsigned short x_tvran[2];
  }x_tv;
};

#define SYMENT struct syment
#define SYMESZ sizeof(SYMENT)
#define AUXENT union auxent
#define AUXESZ sizeof(AUXENT)

#define _ETEXT   "etext"
#define _EDATA   "edata"
#define _END     "end"
#define _START   "_start"
#define _TVORIG  "_tvorig"
#define _TORIGIN "_torigin"
#define _DORIGIN "_dorigin"
#define _SORIGIN "_sorigin"

#pragma pack()

#if __cplusplus
}
#endif

#endif
