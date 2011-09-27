/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __BIOS_H
#define __BIOS_H 1

#ifndef __NT__
#if __cplusplus
extern "C" {
#endif

#define _KEYBRD_READ        0
#define _KEYBRD_READY       1
#define _KEYBRD_SHIFTSTATUS 2
#define _NKEYBRD_READ       0x10
#define _NKEYBRD_READY      0x11
#define _NKEYBRD_SHIFTSTATUS 0x12

#ifndef _WINDOWS
unsigned short __cdecl _bios_keybrd(unsigned);
unsigned short __cdecl bioskey(int);
#endif

#if M_UNIX || M_XENIX

void __cdecl _bios_keybrd_close(void);
int __cdecl _bios_keybrd_open(void);

#else /* M_UNIX || M_XENIX */

#pragma pack(1)
struct diskinfo_t {
    unsigned drive;
    unsigned head;
    unsigned track;
    unsigned sector;
    unsigned nsectors;
    void __far *buffer;
    };
#define _diskinfo_t diskinfo_t

#pragma pack()

#define _COM_INIT       0
#define _COM_SEND       1
#define _COM_RECEIVE    2
#define _COM_STATUS     3
#define _COM_CHR7       2
#define _COM_CHR8       3
#define _COM_STOP1      0
#define _COM_STOP2      4
#define _COM_NOPARITY   0
#define _COM_ODDPARITY  8
#define _COM_EVENPARITY 24
#define _COM_110        0
#define _COM_150        32
#define _COM_300        64
#define _COM_600        96
#define _COM_1200       128
#define _COM_2400       160
#define _COM_4800       192
#define _COM_9600       224
#define _DISK_RESET     0
#define _DISK_STATUS    1
#define _DISK_READ      2
#define _DISK_WRITE     3
#define _DISK_VERIFY    4
#define _DISK_FORMAT    5
#define _PRINTER_WRITE  0
#define _PRINTER_INIT   1
#define _PRINTER_STATUS 2
#define _TIME_GETCLOCK  0
#define _TIME_SETCLOCK  1

unsigned __cdecl _bios_equiplist(void);
unsigned __cdecl _bios_memsize(void);
#ifndef _WINDOWS
unsigned __cdecl _bios_printer(unsigned, unsigned, unsigned);
#define biosprint(cmd,abyte,port) _bios_printer(cmd,port,abyte)
unsigned __cdecl _bios_serialcom(unsigned, unsigned, unsigned);
#define bioscom _bios_serialcom
#endif
unsigned __cdecl _bios_timeofday(unsigned, long *);
long __cdecl biostime(int , long);
#ifndef DOS386
unsigned __cdecl _bios_disk(unsigned, struct diskinfo_t *);
int __cdecl biosdisk(int, int, int, int, int, int, void *);
#endif /* DOS386 */

#define biosmemory() _bios_memsize()
#define biosequip() _bios_equiplist()
#endif /* M_UNIX || M_XENIX */

#ifndef __REGS_DEFINED
#define __REGS_DEFINED
#ifndef __NT__

#if __INTSIZE == 4

#pragma pack(1)
struct _DWORDREGS
{       unsigned eax,ebx,ecx,edx,esi,edi,cflag,flags;
};
#define DWORDREGS _DWORDREGS

struct _WORDREGS
{       unsigned short  ax,eaxmsw,bx,ebxmsw,cx,ecxmsw,dx,edxmsw,
			si,esimsw,di,edimsw;
	unsigned cflag,flags;
};
#define WORDREGS _WORDREGS

struct _BYTEREGS
{       unsigned char al,ah,ax2,ax3,bl,bh,bx2,bx3,
			  cl,ch,cx2,cx3,dl,dh,dx2,dx3;
};
#define BYTEREGS _BYTEREGS

union _REGS { struct _DWORDREGS e; struct _WORDREGS x; struct _BYTEREGS h; };
#define REGS _REGS

struct _SREGS { unsigned short es,cs,ss,ds,fs,gs; };
#define SREGS _SREGS

#pragma pack()
#else /* __INTSIZE == 4 */

#pragma pack(1)
struct WORDREGS { unsigned ax,bx,cx,dx,si,di,cflag,flags; };
struct BYTEREGS {unsigned char al,ah,bl,bh,cl,ch,dl,dh; };
union REGS { struct WORDREGS x; struct BYTEREGS h; };
struct SREGS { unsigned es,cs,ss,ds; };
#pragma pack()
#define _REGS REGS
#define _SREGS SREGS
#define _WORDREGS WORDREGS
#define _BYTEREGS BYTEREGS

#endif /* __INTSIZE == 4 */
#endif /* __NT__ */
#endif /* __REGS_DEFINED */

int __cdecl int86(int,union REGS *,union REGS *);
int __cdecl int86x(int,union REGS *,union REGS *,struct SREGS *);
#define _int86 int86
#define _int86x int86x

#if __cplusplus
}
#endif

#endif /* __NT__ */

#endif
