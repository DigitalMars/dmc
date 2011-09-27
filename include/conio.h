// Copyright (C) 1986-2001 by Digital Mars.
// All Rights Reserved
// www.digitalmars.com

#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __CONIO_H
#define __CONIO_H 1

#if __cplusplus
extern "C" {
#endif

#if __OS2__ && __INTSIZE == 4
#define __CLIB	__stdcall
#else
#define __CLIB	__cdecl
#endif

/* Define _CRTAPI1 (for compatibility with the NT SDK) */
#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif


/* Define _CRTAPI2 (for compatibility with the NT SDK) */
#ifndef _CRTAPI2
#define _CRTAPI2 __cdecl
#endif

/* Define CRTIMP */
#ifndef _CRTIMP
#if defined(_WIN32) && defined(_DLL)
#define _CRTIMP  __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif




#include <stdio.h>

#define _KB_F1	      59
#define _KB_F2	      60
#define _KB_F3	      61
#define _KB_F4	      62
#define _KB_F5	      63
#define _KB_F6	      64
#define _KB_F7	      65
#define _KB_F8	      66
#define _KB_F9	      67
#define _KB_F10	      68

#define _KB_HOME      71
#define _KB_UP	      72
#define _KB_PGUP      73
#define _KB_LEFT      75
#define _KB_RIGHT     77
#define _KB_END	      79
#define _KB_DOWN      80
#define _KB_PGDN      81
#define _KB_INS	      82
#define _KB_BACK_TAB  15

#define _KB_SF1	      84
#define _KB_SF2	      85
#define _KB_SF3	      86
#define _KB_SF4	      87
#define _KB_SF5	      88
#define _KB_SF6	      89
#define _KB_SF7	      90
#define _KB_SF8	      91
#define _KB_SF9	      92
#define _KB_SF10      93

#define _KB_CF1	      94
#define _KB_CF2	      95
#define _KB_CF3	      96
#define _KB_CF4	      97
#define _KB_CF5	      98
#define _KB_CF6	      99
#define _KB_CF7	      100
#define _KB_CF8	      101
#define _KB_CF9	      102
#define _KB_CF10      103

#define _KB_AF1	      104
#define _KB_AF2	      105
#define _KB_AF3	      106
#define _KB_AF4	      107
#define _KB_AF5	      108
#define _KB_AF6	      109
#define _KB_AF7	      110
#define _KB_AF8	      111
#define _KB_AF9	      112
#define _KB_AF10      113

#define _KB_DEL	      83
#define _KB_CPGUP     132
#define _KB_CLEFT     115
#define _KB_CRIGHT    116
#define _KB_CEND      117
#define _KB_CPGDN     118
#define _KB_CHOME     119
#define _KB_A1	      120
#define _KB_A2	      121
#define _KB_A3	      122
#define _KB_A4	      123
#define _KB_A5	      124
#define _KB_A6	      125
#define _KB_A7	      126
#define _KB_A8	      127
#define _KB_A9	      128
#define _KB_A0	      129
#define _KB_AMINUS    130
#define _KB_APLUS     131

#if M_UNIX || M_XENIX
extern char *ttyname(int file_handle);

typedef char _strmap_t[512];
extern int __cdecl _kb_getmapstr(_strmap_t dest_map);
extern int __cdecl _kb_setmapstr(_strmap_t new_map);

#define MIOC	('k'<<8)

#define GIO_KEYMAP      (MIOC | 6)
#define PIO_KEYMAP      (MIOC | 7)

#define	KBIO_SETMODE	(13|MIOC)
#define	KBIO_GETMODE	(14|MIOC)

/* keyboard mode -- set by KBIO_{S | G}ETMODE, returned by KDGKBTYPE */
#define	KBM_XT		0
#define	KBM_AT		1
#define	KBM_OTHER	2


#define	KIOC		('K' << 8)
#define	KDDISPTYPE	(KIOC | 1)
#define KDMAPDISP	(KIOC | 2)
#define KDUNMAPDISP	(KIOC | 3)
#define KDGKBMODE	(KIOC | 6)
#define KDSKBMODE	(KIOC | 7)
#define KBMKTONE	(KIOC | 8)

/* Used with KD{G | S}ETMODE */
#define K_RAW	0
#define K_XLATE	1

#define KDADDIO		(KIOC | 11)
#define KDDELIO		(KIOC | 12)

#define KDDISPINFO	(KIOC | 18)

#define KIOCSOUND	(KIOC | 63)
#define KDGKBTYPE	(KIOC | 64)
#define KDGETLED	(KIOC | 65)
#define KDSETLED	(KIOC | 66)

struct kd_dispinfo
{	char *vaddr;
	unsigned long physaddr;
	unsigned long size;
};


struct kd_disparam
{	long type;
#define KD_MONO     1
#define KD_HERCULES 2
#define KD_CGA      3
#define KD_EGA      4
#define KD_VGA      5
	char *addr;
};

#define EGAIOC	('E' << 8)
#define EGAMODE (EGAIOC | 1)
#define EGAIO	(EGAIOC | 2)
#define EGA_GET (EGAIOC | 3)
#define EGA_IOPRIVL (EGAIOC | 4)

#define VGAIOC	('E' << 8)
#define VGAMODE (VGAIOC | 65)
#define VGAIO	(VGAIOC | 66)
#define VGA_GET (VGAIOC | 67)
#define VGA_IOPRIVL (VGAIOC | 68)

#define _KB_F1_STR    "\033[M"
#define _KB_F2_STR    "\033[N"
#define _KB_F3_STR    "\033[O"
#define _KB_F4_STR    "\033[P"
#define _KB_F5_STR    "\033[Q"
#define _KB_F6_STR    "\033[R"
#define _KB_F7_STR    "\033[S"
#define _KB_F8_STR    "\033[T"
#define _KB_F9_STR    "\033[U"
#define _KB_F10_STR   "\033[V"
#define _KB_F11_STR   "\033[W"
#define _KB_F12_STR   "\033[X"

#define _KB_SF1_STR   "\033[Y"
#define _KB_SF2_STR   "\033[Z"
#define _KB_SF3_STR   "\033[a"
#define _KB_SF4_STR   "\033[b"
#define _KB_SF5_STR   "\033[c"
#define _KB_SF6_STR   "\033[d"
#define _KB_SF7_STR   "\033[e"
#define _KB_SF8_STR   "\033[f"
#define _KB_SF9_STR   "\033[g"
#define _KB_SF10_STR  "\033[h"
#define _KB_SF11_STR  "\033[i"
#define _KB_SF12_STR  "\033[j"

#define _KB_CF1_STR   "\033[k"
#define _KB_CF2_STR   "\033[l"
#define _KB_CF3_STR   "\033[m"
#define _KB_CF4_STR   "\033[n"
#define _KB_CF5_STR   "\033[o"
#define _KB_CF6_STR   "\033[p"
#define _KB_CF7_STR   "\033[q"
#define _KB_CF8_STR   "\033[r"
#define _KB_CF9_STR   "\033[s"
#define _KB_CF10_STR  "\033[t"
#define _KB_CF11_STR  "\033[u"
#define _KB_CF12_STR  "\033[v"

#define _KB_CSF1_STR  "\033[w"
#define _KB_CSF2_STR  "\033[x"
#define _KB_CSF3_STR  "\033[y"
#define _KB_CSF4_STR  "\033[z"
#define _KB_CSF5_STR  "\033[@"
#define _KB_CSF6_STR  "\033[["
#define _KB_CSF7_STR  "\033[\\"
#define _KB_CSF8_STR  "\033[]"
#define _KB_CSF9_STR  "\033[^"
#define _KB_CSF10_STR "\033[_"
#define _KB_CSF11_STR "\033[`"
#define _KB_CSF12_STR "\033[{"
#define _KB_HOME_STR  "\033[H"

#define _KB_UP_STR    "\033[A"
#define _KB_PGUP_STR  "\033[I"
#define _KB_MINUS_STR "-"
#define _KB_LEFT_STR  "\033[D"
#define _KB_5_STR     "\033[E"
#define _KB_RIGHT_STR "\033[C"
#define _KB_PLUS_STR  "+"
#define _KB_END_STR   "\033[F"
#define _KB_DOWN_STR  "\033[B"
#define _KB_PGDN_STR  "\033[G"
#define _KB_INS_STR   "\033[L"

#define _KB_5	      76
#define _KB_PLUS      78
#define _KB_MINUS     74

#define _KB_CSF1      104
#define _KB_CSF2      105
#define _KB_CSF3      106
#define _KB_CSF4      107
#define _KB_CSF5      108
#define _KB_CSF6      109
#define _KB_CSF7      110
#define _KB_CSF8      111
#define _KB_CSF9      112
#define _KB_CSF10     113

#define _KB_F11	      133
#define _KB_F12	      134
#define _KB_SF11      135
#define _KB_SF12      136
#define _KB_CF11      137
#define _KB_CF12      138
#define _KB_CSF11     139
#define _KB_CSF12     140

#endif  /* M_UNIX || M_XENIX */

int		__cdecl	kbhit(void);
int		__cdecl ungetch(int c);
#define _kbhit kbhit
#define _ungetch ungetch

#if !defined(_WINDOWS) || defined(_WIN32)
int		__cdecl	getch(void);
int		__cdecl getche(void);
int             __cdecl putch(int);
#define _getch getch
#define _getche getche
#define _putch putch

char * __CLIB _cgets(char *);
int __CLIB _cprintf(const char *, ...);
int __CLIB _cputs(const char *);
int __CLIB _cscanf(const char *, ...);

#define cgets _cgets
#define cprintf _cprintf
#define cputs _cputs
#define cscanf _cscanf

#endif /* _WINDOWS */

#if __cplusplus
}
#endif

#endif
