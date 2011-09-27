/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SYS_TERMIO_H
#define __SYS_TERMIO_H 1

#if __cplusplus
extern "C"
{
#endif

#ifndef _POSIX_VDISABLE
#define _POSIX_VDISABLE 0
#endif

#if !defined(_POSIX_SOURCE)
#define	CTRL(c)	((c)&037)
#define IBSHIFT 16

#define	NCC	8
#endif /* !defined(_POSIX_SOURCE) */ 

#define	NCCS	19

#include <unistd.h>

typedef unsigned char cc_t;
typedef unsigned long speed_t;
#if defined(M_ELF)
typedef unsigned long tcflag_t;
#else
typedef unsigned short tcflag_t;
#endif

/* POSIX structure */
struct termios
        {
        tcflag_t        c_iflag;
        tcflag_t        c_oflag;
        tcflag_t        c_cflag;
        tcflag_t        c_lflag;
        cc_t            c_cc[NCCS];
        };

/* termios option flags */

#define TCIFLUSH        0
#define TCOFLUSH        1
#define TCIOFLUSH       2

#define TCOOFF          0
#define TCOON           1
#define TCIOFF          2
#define TCION           3

/* System V structure */
#if !defined(_POSIX_SOURCE)
struct termio
{
  unsigned short c_iflag;
  unsigned short c_oflag;
  unsigned short c_cflag;
  unsigned short c_lflag;
  char c_line;
  unsigned char c_cc[NCC];
};

struct winsize
{
	unsigned short ws_row;
	unsigned short ws_col;
	unsigned short ws_xpixel;
	unsigned short ws_ypixel;
};

#endif

/* input modes */
#define IGNBRK 0x1
#define BRKINT 0x2
#define IGNPAR 0x4
#define PARMRK 0x8
#define INPCK  0x10
#define ISTRIP 0x20
#define INLCR  0x40
#define IGNCR  0x80
#define ICRNL  0x100

#if !defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)
#define IUCLC  0x200
#define IXANY  0x800
#define IMAXBEL 0020000
#define DOSMODE 0100000
#endif

#define IXON   0x400
#define IXOFF  0x1000

/* output modes */
#define OPOST  0x1

#if !defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)
#define OLCUC  0x2
#define ONLCR  0x4
#define OCRNL  0x8
#define ONOCR  0x10
#define ONLRET 0x20
#define OFILL  0x40
#define OFDEL  0x80
#define NLDLY  0x100
#define NL0    0
#define NL1    0x100
#define CRDLY  0x600
#define CR0    0
#define CR1    0x200
#define CR2    0x400
#define CR3    0x600
#define TABDLY 0x1800
#define TAB0   0
#define TAB1   0x800
#define TAB2   0x1000
#define TAB3   0x1800
#define XTABS  0x1800
#define BSDLY  0x2000
#define BS0    0
#define BS1    0x2000
#define VTDLY  0x4000
#define VT0    0
#define VT1    0x4000
#define FFDLY  0x8000
#define FF0    0
#define FF1    0x8000
#endif

#if !defined(_POSIX_SOURCE)
#define PAGEOUT 0x10000
#define WRAP    0x20000

/* control modes */
#define CBAUD  0xF
#endif
#define B0     0
#define B50    0x1
#define B75    0x2
#define B110   0x3
#define B134   0x4
#define B150   0x5
#define B200   0x6
#define B300   0x7
#define B600   0x8
#define B1200  0x9
#define B1800  0xA
#define B2400  0xB
#define B4800  0xC
#define B9600  0xD
#define B19200 0xE
#define B38400 0xF
#if !defined(_POSIX_SOURCE)
#define EXTA   0xE
#define EXTB   0xF
#endif
#define CS5    0
#define CS6    0x10
#define CS7    0x20
#define CS8    0x30
#define CSIZE  0x30
#define CSTOPB 0x40
#define CREAD  0x80
#define PARENB 0x100
#define PARODD 0x200
#define HUPCL  0x400
#define CLOCAL 0x800

/* stdin modes */
#define ISIG   0x1
#define ICANON 0x2

#if !defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)
#define XCASE  0x4
#endif

#define ECHO   0x8
#define ECHOE  0x10
#define ECHOK  0x20
#define ECHONL 0x40

#define NOFLSH 0x80
#define TOSTOP 0x100

#if !defined(_POSIX_SOURCE)
#define	ECHOCTL	0001000
#define	ECHOPRT	0002000
#define	ECHOKE	0004000
#define	DEFECHO	0010000
#define	FLUSHO	0020000
#define	PENDIN	0040000

#define RCV1EN  0x8000
#define XMT1EN  0x10000
#define LOBLK   0x20000
#define XCLUDE  0x8000
#define CIBAUD	03600000
#define PAREXT	04000000
#endif

/* control chars indexes */
#define VINTR  0
#define VQUIT  1
#define VERASE 2
#define VKILL  3
#define VEOF   4
#define VMIN   4
#define VEOL   5
#define VTIME  5
#if !defined(_POSIX_SOURCE)
#define VEOL2  6
#define VSWTCH 7
#endif
#define VSTART 8
#define VSTOP  9
#define VSUSP  10

#if !defined(_POSIX_SOURCE) 
#define VDSUSP   11
#define VREPRINT 12
#define VDISCARD 13
#define VWERASE  14
#define VLNEXT   15

#define VCEOF   NCC
#define VCEOL   (NCC + 1)

#define	CNUL	0
#define	CDEL	0377

/* control char values */
#define CNSWTCH 0
#define CEOF	4     /* EOT, cntl D */
#define CERASE	8     /* BS,  cntl H */
#define CKILL	0x15  /* NAK, cntl U */
#define CSTART	0x11  /* DC1, cntl Q */
#define CSTOP	0x13  /* DC3, cntl S */
#define CSWTCH	0x1A  /* SUB, cntl Z */
#define CSUSP	0x1A  /* SUB, cntl Z - POSIX default suspend character */
#define CDSUSP  0x19
#define CQUIT   0x1C  /* FS,  cntl \ */
#define CINTR   0x7F  /* DEL  cntl ? */
#define CESC	0x5C  /* escape char ('\\') */
#define CEOT    04
#define CEOL    0
#define CEOL2   0
#define CRPRNT  CTRL('r')
#define	CFLUSH	CTRL('o')
#define	CWERASE	CTRL('w')
#define CLNEXT  CTRL('v')

/* Line discipline IOCTL's. */
#define LDIOC   ('D'<<8)
#define LDOPEN	(LDIOC|0)
#define LDCLOSE	(LDIOC|1)
#define LDCHG	(LDIOC|2)
#define LDGETT	(LDIOC|8)
#define LDSETT	(LDIOC|9)
#define LDSMAP	(LDIOC|10)
#define LDGMAP	(LDIOC|11)
#define LDNMAP	(LDIOC|12)

#define DIOC		('d'<<8)
#define DIOCGETP	(DIOC|8)
#define DIOCSETP	(DIOC|9)

#define FIORDCHK	(('f'<<8)|3)

/* mouse ioctls. */
#define EVLD_IOC	(LDIOC)
#define LDEV_SETTYPE	(EVLD_IOC|13)
#define LDEV_GETEV	(EVLD_IOC|14)
#define LDEV_ATTACHQ	(EVLD_IOC|15)
#define LDEV_SETRATIO	(EVLD_IOC|16)

/* event driver ioctls */
#define	EQIOC		('Q'<<8)
#define	EQIO_GETQP	(EQIOC|1)
#define	EQIO_SETEMASK	(EQIOC|2)
#define	EQIO_GETEMASK	(EQIOC|3)
#define	EQIO_SUSPEND	(EQIOC|4)
#define	EQIO_RESUME	(EQIOC|5)
#define	EQIO_BLOCK	(EQIOC|6)

#define TIOC	('T'<<8)
#define TCGETA	(1|TIOC)
#define TCSETA	(2|TIOC)
#define TCSETAW	(3|TIOC)
#define TCSETAF	(4|TIOC)
#define TCSBRK	(5|TIOC)
#define TCXONC	(6|TIOC)
#define TCFLSH	(7|TIOC)

#define TIOCKBON (TIOC|8)
#define TIOCKBOF (TIOC|9)
#define KBENABLED (TIOC|10)

#define TCGETS  (13|TIOC)
#define TCSETS  (14|TIOC)

#define TCSETSW (15|TIOC)
#define TCSETSF (16|TIOC)

#if defined(M_ELF)
#define TCSANOW   (14|('T'<<8))
#define TCSADRAIN (15|('T'<<8))
#define TCSAFLUSH (16|('T'<<8))
#else
#define TCSANOW    XCSETA
#define TCSADRAIN  XCSETAW
#define TCSAFLUSH  XCSETAF
#define TCSADFLUSH XCSETAF
#endif

#define IOCTYPE 0xffffff00

#define TCDSET  (32|TIOC)
#define RTS_TOG (33|TIOC)

#define TIOCGWINSZ (104|TIOC)
#define TIOCSWINSZ (103|TIOC)

#endif /* !defined(_POSIX_SOURCE) */

#define IEXTEN  0100000

#if defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)
#define XIOC		(('i'<<24) | ('X'<<16))
#else
#define	XIOC		('x'<<8)
#endif
#define	O_XIOC		('x'<<8)

#define	O_XCGETA	(O_XIOC|1)
#define	O_XCSETA	(O_XIOC|2)
#define	O_XCSETAW	(O_XIOC|3)
#define	O_XCSETAF	(O_XIOC|4)

#define	XCGETA		(XIOC|1)
#define	XCSETA		(XIOC|2)
#define	XCSETAW		(XIOC|3)
#define	XCSETAF		(XIOC|4)

#if !__STDC__

speed_t __cdecl cfgetispeed (const struct termios *);
speed_t __cdecl cfgetospeed (const struct termios *);
int __cdecl cfsetispeed (struct termios *, speed_t);
int __cdecl cfsetospeed (struct termios *, speed_t);

#if !defined(_XOPEN_SOURCE) && !defined(_POSIX_SOURCE)
#ifdef M_ELF
#define tcgetattr(fd, termios_p)        ioctl(fd, TCGETS, termios_p)
#else
#define tcgetattr(fd, termios_p)        ioctl(fd, XCGETA, termios_p)
#endif
#define tcsetattr(fd, opt, termios_p)	ioctl(fd, opt, termios_p)
#define tcflow(fd, action)		ioctl(fd, TCXONC, action)
#define tcflush(fd, queue_select)	ioctl(fd, TCFLSH, queue_select)
#define tcdrain(fd)			ioctl(fd, TCSBRK, 1)
#define tcsendbreak(fd, duration)	ioctl(fd, TCSBRK, 0, duration)
#endif

#endif /* !__STDC__ */

#if __cplusplus
}
#endif

#endif

