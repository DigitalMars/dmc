/*_ mbcpinfo.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

/* This module is system definitions.			*/

#include <mbctype.h>
#include <mbstring.h>
#include <string.h>
#ifdef _WIN32
#include <setlocal.h>
#endif
#include <setmbcp.h>

#define mLCID(m,s)	MAKELCID(MAKELANGID(m,s),SORT_DEFAULT)

/* 932 Shift-JIS Japanese.
*/
static BYTERANG		__br_jpn[] = {
  { _MS, 0xA6, 0xDF },
  { _MP, 0xA1, 0xA5 },
  { _M1, 0x81, 0x9F }, { _M1, 0xE0, 0xFC },
  { _M2, 0x40, 0x7E }, { _M2, 0x80, 0xFC }  };
static MBCASERANG	__cr_jpn[] = {
  { 0x8260, 0x8279, 0x8281, 0x829A }  };
static MBDIGITRANG	__dr_jpn[] = {
  { 0x824F, 0x8258 }  };

/* 936 GB 2312-80 Chinese Simplified.
*/
static BYTERANG		__br_chs[] = {
  { _M1, 0xA1, 0xFE },
  { _M2, 0xA1, 0xFE }  };
static MBCASERANG	__cr_chs[] = {
  { 0xA3C1, 0xA3DA, 0xA3E1, 0xA3FA }  };
static MBDIGITRANG	__dr_chs[] = {
  { 0xA3B0, 0xA3B9 }  };

/* 949 KS C-5601-1987 Korean Wangsung.
*/
static BYTERANG		__br_kor[] = {
  { _M1, 0x81, 0xFE },
  { _M2, 0x41, 0x5A }, { _M2, 0x61, 0x7A }, { _M2, 0x81, 0xFE }  };
static MBCASERANG	__cr_kor[] = {
  { 0xA3C1, 0xA3DA, 0xA3E1, 0xA3FA }  };
static MBDIGITRANG	__dr_kor[] = {
  { 0xA3B0, 0xA3B9 }  };

/* 950 Big-5 Chinese Taiwan.
*/
static BYTERANG		__br_cht[] = {
  { _M1, 0x81, 0xFE },
  { _M2, 0x40, 0x7E }, { _M2, 0xA1, 0xFE }  };
static MBCASERANG	__cr_cht[] = {
  { 0xA2CF, 0xA2E4, 0xA2E9, 0xA2FE },
  { 0xA2E5, 0xA2E8, 0xA340, 0xA343 }  };
static MBDIGITRANG	__dr_cht[] = {
  { 0xA2AF, 0xA2B8 }  };

/* 1361 KS C-5601-1992 Korean Johab.
*/
static BYTERANG		__br_jhb[] = {
  { _M1, 0x84, 0xD3 }, { _M1, 0xD8, 0xDE }, { _M1, 0xE0, 0xF9 },
  { _M2, 0x31, 0x7E }, { _M2, 0x81, 0xFE }  };
static MBCASERANG	__cr_jhb[] = {
  { 0xDA51, 0xDA5E, 0xDA71, 0xDA7E },
  { 0xDA5F, 0xDA6A, 0xDA91, 0xDA9C }  };
static MBDIGITRANG	__dr_jhb[] = {
  { 0xDA40, 0xDA49 }  };

/* CodePage Control table.
*/
MBCPINFO	__mb_cpinfo[] = {
  {  932,
#ifdef _WIN32
     mLCID (LANG_JAPANESE, SUBLANG_DEFAULT),
#endif
     _BCountof  (__br_jpn), __br_jpn, _MCCountof (__cr_jpn), __cr_jpn,
     _MDCountof (__dr_jpn), __dr_jpn },
  {  936,
#ifdef _WIN32
     mLCID (LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED),
#endif
     _BCountof  (__br_chs), __br_chs, _MCCountof (__cr_chs), __cr_chs,
     _MDCountof (__dr_chs), __dr_chs },
  {  949,
#ifdef _WIN32
     mLCID (LANG_KOREAN, SUBLANG_DEFAULT),
#endif
     _BCountof  (__br_kor), __br_kor, _MCCountof (__cr_kor), __cr_kor,
     _MDCountof (__dr_kor), __dr_kor },
  {  950,
#ifdef _WIN32
     mLCID (LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL),
#endif
     _BCountof  (__br_cht), __br_cht, _MCCountof (__cr_cht), __cr_cht,
     _MDCountof (__dr_cht), __dr_cht },
  { 1361,
#ifdef _WIN32
     0,
#endif
     _BCountof  (__br_jhb), __br_jhb, _MCCountof (__cr_jhb), __cr_jhb,
     _MDCountof (__dr_jhb), __dr_jhb }  };

/* CodePage Control table item number.
*/
int		__mb_cpinfoN = _MICountof(__mb_cpinfo);

/**/
