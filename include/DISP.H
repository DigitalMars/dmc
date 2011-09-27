/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif


#ifndef __DISP_H
#define __DISP_H 1

#if __cplusplus
extern "C" {
#endif

extern  int
	__cdecl disp_numrows,
	__cdecl disp_numcols,
	__cdecl disp_cursorrow,
	__cdecl disp_cursorcol;
extern short disp_nowrap;  /* does display wrap or not */	

extern  int
	__cdecl disp_printf(char *,...),
	__cdecl disp_getmode(void),
	__cdecl disp_getattr(void),
	__cdecl disp_putc(int);
extern  void
	__cdecl disp_levelblockpoke(int,int,int,int,unsigned,unsigned *,unsigned,unsigned *,unsigned),
	__cdecl disp_open(void),
	__cdecl disp_puts(const char *),
	__cdecl disp_box(int,int,unsigned,unsigned,unsigned,unsigned),
	__cdecl disp_close(void),
#ifndef _WINDOWS
	__cdecl disp_usebios(void),
#endif
	__cdecl disp_move(int,int),
	__cdecl disp_flush(void),
	__cdecl disp_eeol(void),
	__cdecl disp_eeop(void),
	__cdecl disp_startstand(void),
	__cdecl disp_endstand(void),
	__cdecl disp_setattr(int),
	__cdecl disp_setcursortype(int),
	__cdecl disp_pokew(int,int,unsigned short),
	__cdecl disp_scroll(int,unsigned,unsigned,unsigned,unsigned,unsigned),
	__cdecl disp_setmode(unsigned char),
	__cdecl disp_peekbox(unsigned short *,unsigned,unsigned,unsigned,unsigned),
	__cdecl disp_pokebox(unsigned short *,unsigned,unsigned,unsigned,unsigned),
#ifndef _WINDOWS
	__cdecl disp_set43(void),
	__cdecl disp_reset43(void),
#endif
	__cdecl disp_fillbox(unsigned,unsigned,unsigned,unsigned,unsigned),
	__cdecl disp_hidecursor(void),
	__cdecl disp_showcursor(void);
extern  unsigned short __cdecl disp_peekw(int,int);

/* Globals for IBM PC displays (read only):     */
extern  unsigned char
	__cdecl disp_mono,
	__cdecl disp_mode,
	__cdecl disp_inited,
	__cdecl disp_ega;
extern  unsigned __cdecl disp_base;

#if M_UNIX || M_XENIX
extern unsigned char __cdecl disp_scomode, __cdecl disp_captured;
extern int __cdecl disp_vt_number;
extern char __cdecl *disp_offset;
#else
extern  unsigned char __cdecl disp_snowycga;
#endif

#define DISP_REVERSEVIDEO       0x70
#define DISP_NORMAL             0x07
#define DISP_UNDERLINE          0x01
#define DISP_NONDISPLAY         0x00

#define DISP_INTENSITY          0x08
#define DISP_BLINK              0x80

#if _WIN32
#define DISP_CURSORBLOCK	100
#define DISP_CURSORHALF		 50
#define DISP_CURSORUL		 20
#else
#define DISP_CURSORBLOCK (disp_mono ? 0x000C : 0x0007)
#define DISP_CURSORHALF  (disp_mono ? 0x060C : 0x0307)
#define DISP_CURSORUL    (disp_mono ? 0x0B0C : 0x0707)
#endif

#if !(M_UNIX || M_XENIX)
#pragma pack(1)

typedef struct disp_t
{
    unsigned numrows;
    unsigned numcols;
    unsigned cursorrow;
    unsigned cursorcol;
    unsigned char mono;
    unsigned char snowycga;
    unsigned char mode;
    unsigned char inited;
    unsigned char ega;
    unsigned char reserved[3];
    short nowrap;

    union _disp
    {
	unsigned short __far *base;
	struct _disp1
	{   unsigned offset;
	    unsigned short base;
	} _disp1x;
    } _dispx;
#if _WIN32
   void *handle;
#endif
} disp_t;

#pragma pack()

extern disp_t __cdecl disp_state;


#define disp_numrows    disp_state.numrows
#define disp_numcols    disp_state.numcols
#define disp_cursorrow  disp_state.cursorrow
#define disp_cursorcol  disp_state.cursorcol
#define disp_mono       disp_state.mono
#define disp_snowycga   disp_state.snowycga
#define disp_mode       disp_state.mode
#define disp_inited     disp_state.inited
#define disp_ega        disp_state.ega
#define disp_nowrap     disp_state.nowrap
#define disp_base       disp_state._dispx._disp1x.base

#endif

#if __cplusplus
}
#endif

#endif
