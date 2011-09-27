/* Copyright (C) 2001 by Digital Mars */
/* www.digitalmars.com */

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __INTTYPES_H
#define __INTTYPES_H	1

#ifndef __STDINT_H
#include <stdint.h>
#endif

#pragma pack(__DEFALIGN)
typedef struct _IMAXDIV { intmax_t  quot,rem; } imaxdiv_t;
#pragma pack()

#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

#define PRId8		"hhd"
#define PRId16		"hd"
#define PRId32		"ld"

#define PRIdLEAST8	"hhd"
#define PRIdLEAST16	"hd"
#define PRIdLEAST32	"ld"

#define PRIdFAST8	"hhd"
#define PRIdFAST16	"d"
#define PRIdFAST32	"ld"

#define PRIi8		"hhi"
#define PRIi16		"hi"
#define PRIi32		"li"

#define PRIiLEAST8	"hhi"
#define PRIiLEAST16	"hi"
#define PRIiLEAST32	"li"

#define PRIiFAST8	"hhi"
#define PRIiFAST16	"i"
#define PRIiFAST32	"li"

#define PRIo8		"hho"
#define PRIo16		"ho"
#define PRIo32		"lo"
#define PRIoLEAST8	"hho"
#define PRIoLEAST16	"ho"
#define PRIoLEAST32	"lo"
#define PRIoFAST8	"hho"
#define PRIoFAST16	"o"
#define PRIoFAST32	"lo"

#define PRIu8		"hhu"
#define PRIu16		"hu"
#define PRIu32		"lu"
#define PRIuLEAST8	"hhu"
#define PRIuLEAST16	"hu"
#define PRIuLEAST32	"lu"
#define PRIuFAST8	"hhu"
#define PRIuFAST16	"u"
#define PRIuFAST32	"lu"

#define PRIx8		"hhx"
#define PRIx16		"hx"
#define PRIx32		"lx"
#define PRIxLEAST8	"hhx"
#define PRIxLEAST16	"hx"
#define PRIxLEAST32	"lx"
#define PRIxFAST8	"hhx"
#define PRIxFAST16	"x"
#define PRIxFAST32	"lx"

#define PRIX8		"hhX"
#define PRIX16		"hX"
#define PRIX32		"lX"
#define PRIXLEAST8	"hhX"
#define PRIXLEAST16	"hX"
#define PRIXLEAST32	"lX"
#define PRIXFAST8	"hhX"
#define PRIXFAST16	"X"
#define PRIXFAST32	"lX"

#define SCNd8		"hhd"
#define SCNd16		"hd"
#define SCNd32		"ld"
#define SCNdLEAST8	"hhd"
#define SCNdLEAST16	"hd"
#define SCNdLEAST32	"ld"
#define SCNdFAST8	"hhd"
#define SCNdFAST16	"d"
#define SCNdFAST32	"ld"

#define SCNi8		"hhd"
#define SCNi16		"hi"
#define SCNi32		"li"
#define SCNiLEAST8	"hhd"
#define SCNiLEAST16	"hi"
#define SCNiLEAST32	"li"
#define SCNiFAST8	"hhd"
#define SCNiFAST16	"i"
#define SCNiFAST32	"li"

#define SCNo8		"hhd"
#define SCNo16		"ho"
#define SCNo32		"lo"
#define SCNoLEAST8	"hhd"
#define SCNoLEAST16	"ho"
#define SCNoLEAST32	"lo"
#define SCNoFAST8	"hhd"
#define SCNoFAST16	"o"
#define SCNoFAST32	"lo"

#define SCNu8		"hhd"
#define SCNu16		"hu"
#define SCNu32		"lu"
#define SCNuLEAST8	"hhd"
#define SCNuLEAST16	"hu"
#define SCNuLEAST32	"lu"
#define SCNuFAST8	"hhd"
#define SCNuFAST16	"u"
#define SCNuFAST32	"lu"

#define SCNx8		"hhd"
#define SCNx16		"hx"
#define SCNx32		"lx"
#define SCNxLEAST8	"hhd"
#define SCNxLEAST16	"hx"
#define SCNxLEAST32	"lx"
#define SCNxFAST8	"hhd"
#define SCNxFAST16	"x"
#define SCNxFAST32	"lx"

#if __LONGLONG
#define PRId64		"lld"
#define PRIdLEAST64	"lld"
#define PRIdFAST64	"lld"
#define PRIi64		"lli"
#define PRIiLEAST64	"lli"
#define PRIiFAST64	"lli"
#define PRIo64		"llo"
#define PRIoLEAST64	"llo"
#define PRIoFAST64	"llo"
#define PRIu64		"llu"
#define PRIuLEAST64	"llu"
#define PRIuFAST64	"llu"
#define PRIx64		"llx"
#define PRIxLEAST64	"llx"
#define PRIxFAST64	"llx"
#define PRIX64		"llX"
#define PRIXLEAST64	"llX"
#define PRIXFAST64	"llX"

#define SCNd64		"lld"
#define SCNdLEAST64	"lld"
#define SCNdFAST64	"lld"
#define SCNi64		"lli"
#define SCNiLEAST64	"lli"
#define SCNiFAST64	"lli"
#define SCNo64		"llo"
#define SCNoLEAST64	"llo"
#define SCNoFAST64	"llo"
#define SCNu64		"llu"
#define SCNuLEAST64	"llu"
#define SCNuFAST64	"llu"
#define SCNx64		"llx"
#define SCNxLEAST64	"llx"
#define SCNxFAST64	"llx"
#endif

#if __LONGLONG
#define PRIdMAX		PRId64
#define PRIiMAX		PRIi64
#define PRIoMAX		PRIo64
#define PRIuMAX		PRIu64
#define PRIxMAX 	PRIx64
#define PRIXMAX		PRIX64
#define SCNdMAX		SCNd64
#define SCNiMAX		SCNi64
#define SCNoMAX		SCNo64
#define SCNuMAX		SCNu64
#define SCNxMAX		SCNx64
#else
#define PRIdMAX		PRId32
#define PRIiMAX		PRIi32
#define PRIoMAX		PRIo32
#define PRIuMAX		PRIu32
#define PRIxMAX		PRIx32
#define PRIXMAX		PRIX32
#define SCNdMAX		SCNd32
#define SCNiMAX		SCNi32
#define SCNoMAX		SCNo32
#define SCNuMAX		SCNu32
#define SCNxMAX		SCNx32
#endif

#if defined(__COMPACT__) || defined(__LARGE__) || defined(__VCM__) || __INTSIZE == 4
#define PRIdPTR		PRId32
#define PRIiPTR		PRIi32
#define PRIoPTR		PRIo32
#define PRIuPTR		PRIu32
#define PRIxPTR		PRIx32
#define PRIXPTR		PRIX32
#define SCNdPTR		SCNd32
#define SCNiPTR		SCNi32
#define SCNoPTR		SCNo32
#define SCNuPTR		SCNu32
#define SCNxPTR		SCNx32
#else
#define PRIdPTR		PRId16
#define PRIiPTR		PRIi16
#define PRIoPTR		PRIo16
#define PRIuPTR		PRIu16
#define PRIxPTR		PRIx16
#define PRIXPTR		PRIX16
#define SCNdPTR		SCNd16
#define SCNiPTR		SCNi16
#define SCNoPTR		SCNo16
#define SCNuPTR		SCNu16
#define SCNxPTR		SCNx16
#endif


#endif

#if __cplusplus
extern "C" {
#endif

#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

intmax_t __CLIB imaxabs(intmax_t);
imaxdiv_t  __CLIB imaxdiv(intmax_t, intmax_t);
intmax_t  __CLIB strtoimax(const char * __restrict nptr,
		char ** __restrict endptr, int base);
uintmax_t  __CLIB strtoumax(const char * __restrict nptr,
		char ** __restrict endptr, int base);

#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif

intmax_t  __CLIB wcstoimax(const wchar_t * __restrict nptr,
		wchar_t ** __restrict endptr, int base);
uintmax_t  __CLIB wcstoumax(const wchar_t * __restrict nptr,
		wchar_t ** __restrict endptr, int base);

#if __cplusplus
}
#endif

#endif
