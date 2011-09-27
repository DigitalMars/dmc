/*_ termio.c   Wed Dec 18 1985 */
/*
 * The functions in this file negotiate with the operating system for
 * characters, and write characters in a barely buffered fashion on the display.
 * All operating systems.
 */
#include        <stdio.h>
#include        "ed.h"

#if !_WIN32

#if     AMIGA
#define NEW 1006
#define LEN 1

static long terminal;
#endif

#if     VMS
#include        <stsdef.h>
#include        <ssdef.h>
#include        <descrip.h>
#include        <iodef.h>
#include        <ttdef.h>

#define NIBUF   128                     /* Input  buffer size           */
#define NOBUF   1024                    /* MM says bug buffers win!     */
#define EFN     0                       /* Event flag                   */

char    obuf[NOBUF];                    /* Output buffer                */
int     nobuf;                          /* # of bytes in above          */
char    ibuf[NIBUF];                    /* Input buffer                 */
int     nibuf;                          /* # of bytes in above          */
int     ibufi;                          /* Read index                   */
int     oldmode[2];                     /* Old TTY mode bits            */
int     newmode[2];                     /* New TTY mode bits            */
short   iochan;                         /* TTY I/O channel              */
#endif

#if     CPM
#include        <bdos.h>
#endif

#if     MSDOS
#undef  LATTICE
#if     !(__ZTC__ | DLC)
#include        <dos.h>
#endif
#endif

#if RAINBOW
#include "rainbow.h"
#endif

#if BSDUNIX
#undef          CTRL
#include        <sgtty.h>               /* for stty/gtty functions */
struct  sgttyb  ostate;                 /* saved tty state */
struct  sgttyb  nstate;                 /* values for editor mode */
#define NONBLOCK        0

#if NONBLOCK
#include        <fcntl.h>
int oldstatus;                          /* original input channel flags */
int lastchar;                           /* last char read               */
int islastchar;                         /* is there a last char?        */
#else
#include        <sys/ioctl.h>           /* for ttkeysininput()          */
#endif
#endif

#if linux
#include	<termios.h>
#include	<unistd.h>
struct  termios  ostate;                 /* saved tty state */
struct  termios  nstate;                 /* values for editor mode */
#endif

#if NCURSES
#include	<curses.h>
#endif

#if MSDOS && !__OS2__
/********************************
 * Interrupt handler for the int (^C) signal.
 * Set the flag to TRUE so that it can be polled via kbd_int().
 */

static intsignal = FALSE;
static int islastc = 0;			// lastc has a character
static int lastc = 0;			// last character read

static kbd_inthandler()
{
        sgarbf = TRUE;                  /* screen needs repainting      */
        intsignal = TRUE;
        return 1;                       /* don't chain                  */
}

#endif

/*
 * This function is called once to set up the terminal device streams.
 * On VMS, it translates SYS$INPUT until it finds the terminal, then assigns
 * a channel to it and sets it raw. On CPM it is a no-op.
 */
ttopen()
{
#if     AMIGA
        terminal = Open("RAW:1/1/639/199/MicroEmacs", NEW);
#endif
#if     VMS
        struct  dsc$descriptor  idsc;
        struct  dsc$descriptor  odsc;
        char    oname[40];
        int     iosb[2];
        int     status;

        odsc.dsc$a_pointer = "SYS$INPUT";
        odsc.dsc$w_length  = strlen(odsc.dsc$a_pointer);
        odsc.dsc$b_dtype   = DSC$K_DTYPE_T;
        odsc.dsc$b_class   = DSC$K_CLASS_S;
        idsc.dsc$b_dtype   = DSC$K_DTYPE_T;
        idsc.dsc$b_class   = DSC$K_CLASS_S;
        do {
                idsc.dsc$a_pointer = odsc.dsc$a_pointer;
                idsc.dsc$w_length  = odsc.dsc$w_length;
                odsc.dsc$a_pointer = &oname[0];
                odsc.dsc$w_length  = sizeof(oname);
                status = LIB$SYS_TRNLOG(&idsc, &odsc.dsc$w_length, &odsc);
                if (status!=SS$_NORMAL && status!=SS$_NOTRAN)
                        exit(status);
                if (oname[0] == 0x1B) {
                        odsc.dsc$a_pointer += 4;
                        odsc.dsc$w_length  -= 4;
                }
        } while (status == SS$_NORMAL);
        status = SYS$ASSIGN(&odsc, &iochan, 0, 0);
        if (status != SS$_NORMAL)
                exit(status);
        status = SYS$QIOW(EFN, iochan, IO$_SENSEMODE, iosb, 0, 0,
                          oldmode, sizeof(oldmode), 0, 0, 0, 0);
        if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
                exit(status);
        newmode[0] = oldmode[0];
        newmode[1] = oldmode[1] | TT$M_PASSALL | TT$M_NOECHO;
        status = SYS$QIOW(EFN, iochan, IO$_SETMODE, iosb, 0, 0,
                          newmode, sizeof(newmode), 0, 0, 0, 0);
        if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
                exit(status);
#endif
#if     CPM
#endif
#if     MSDOS && !__OS2__
        /* Set control-break handler    */
        int_intercept(0x23,kbd_inthandler,128);
#endif
#if     BSDUNIX
        /* Adjust output channel        */
        gtty(1, &ostate);                       /* save old state */
        gtty(1, &nstate);                       /* get base of new state */
        nstate.sg_flags |= RAW;
        nstate.sg_flags &= ~(ECHO|CRMOD);       /* no echo for now... */
        stty(1, &nstate);                       /* set mode */

#if NONBLOCK
        /* Set input to non-blocking    */
        oldstatus = fcntl(0,F_GETFL);
        fcntl(0,F_SETFL,oldstatus | FNDELAY);   /* don't block on input */
#endif
#endif

#if     linux
        /* Adjust output channel        */
        tcgetattr(1, &ostate);                       /* save old state */
        tcgetattr(1, &nstate);                       /* get base of new state */
	cfmakeraw(&nstate);
        tcsetattr(1, TCSADRAIN, &nstate);      /* set mode */
#endif

#if NCURSES
	initscr();
	cbreak();
	noecho();

	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
#endif
}

/*
 * This function gets called just before we go back home to the command
 * interpreter. On VMS it puts the terminal back in a reasonable state.
 * Another no-operation on CPM.
 */
ttclose()
{
#if     AMIGA
        Close(terminal);
#endif
#if     VMS
        int     status;
        int     iosb[1];

        ttflush();
        status = SYS$QIOW(EFN, iochan, IO$_SETMODE, iosb, 0, 0,
                 oldmode, sizeof(oldmode), 0, 0, 0, 0);
        if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
                exit(status);
        status = SYS$DASSGN(iochan);
        if (status != SS$_NORMAL)
                exit(status);
#endif
#if     CPM
#endif
#if     MSDOS && !__OS2__
        int_restore(0x23);      /* de-install control-break handler     */
#endif
#if     BSDUNIX
        stty(1, &ostate);
#if NONBLOCK
        fcntl(0,F_SETFL,oldstatus);
#endif
#endif

#if     linux
        tcsetattr(1, TCSADRAIN, &ostate);	// return to original mode
#endif

#if NCURSES
	endwin();
#endif
}

/*
 * Write a character to the display. On VMS, terminal output is buffered, and
 * we just put the characters in the big array, after checking for overflow.
 * On CPM terminal I/O unbuffered, so we just write the byte out. Ditto on
 * MS-DOS (use the very very raw console output routine).
 */
ttputc(c)
#if     AMIGA
        char c;
#endif
{
#if     AMIGA
        Write(terminal, &c, LEN);
#endif
#if     VMS
        if (nobuf >= NOBUF)
                ttflush();
        obuf[nobuf++] = c;
#endif

#if     CPM
        bios(BCONOUT, c, 0);
#endif

#if     MSDOS & MWC86
        dosb(CONDIO, c, 0);
#endif

#if     MSDOS && (DLC | __ZTC__)
#if __OS2__
        fputc(c, stdout);
#else
        bdos(2,c & 0x7F);
#endif
#endif

#if RAINBOW
        Put_Char(c);                    /* fast video */
#endif

#if     BSDUNIX || linux
        fputc(c, stdout);
#endif
}

/*
 * Flush terminal buffer. Does real work where the terminal output is buffered
 * up. A no-operation on systems where byte at a time terminal I/O is done.
 */
ttflush()
{
#if     AMIGA
#endif
#if     VMS
        int     status;
        int     iosb[2];

        status = SS$_NORMAL;
        if (nobuf != 0) {
                status = SYS$QIOW(EFN, iochan, IO$_WRITELBLK|IO$M_NOFORMAT,
                         iosb, 0, 0, obuf, nobuf, 0, 0, 0, 0);
                if (status == SS$_NORMAL)
                        status = iosb[0] & 0xFFFF;
                nobuf = 0;
        }
        return (status);
#endif
#if     CPM
#endif
#if     MSDOS
#if __OS2__
        fflush(stdout);
#else
        intsignal = FALSE;
#endif
#endif
#if     BSDUNIX || linux
        fflush(stdout);
#endif
#if NCURSES
	refresh();
#endif
}

/*
 * Read a character from the terminal, performing no editing and doing no echo
 * at all. More complex in VMS that almost anyplace else, which figures. Very
 * simple on CPM, because the system can do exactly what you want.
 */
ttgetc()
{
#if     AMIGA
        char ch;

        Read(terminal, &ch, LEN);
        return (int) ch;
#endif
#if     VMS
        int     status;
        int     iosb[2];
        int     term[2];

        while (ibufi >= nibuf) {
                ibufi = 0;
                term[0] = 0;
                term[1] = 0;
                status = SYS$QIOW(EFN, iochan, IO$_READLBLK|IO$M_TIMED,
                         iosb, 0, 0, ibuf, NIBUF, 0, term, 0, 0);
                if (status != SS$_NORMAL)
                        exit(status);
                status = iosb[0] & 0xFFFF;
                if (status!=SS$_NORMAL && status!=SS$_TIMEOUT)
                        exit(status);
                nibuf = (iosb[0]>>16) + (iosb[1]>>16);
                if (nibuf == 0) {
                        status = SYS$QIOW(EFN, iochan, IO$_READLBLK,
                                 iosb, 0, 0, ibuf, 1, 0, term, 0, 0);
                        if (status != SS$_NORMAL
                        || (status = (iosb[0]&0xFFFF)) != SS$_NORMAL)
                                exit(status);
                        nibuf = (iosb[0]>>16) + (iosb[1]>>16);
                }
        }
        return (ibuf[ibufi++] & 0xFF);          /* Allow multinational  */
#endif

#if     CPM
        return (biosb(BCONIN, 0, 0));
#endif

#if RAINBOW
        int Ch;

        while ((Ch = Read_Keyboard()) < 0);

        if ((Ch & Function_Key) == 0)
                if (!((Ch & 0xFF) == 015 || (Ch & 0xFF) == 0177))
                        Ch &= 0xFF;

        return Ch;
#endif

#if     MSDOS & MWC86
        return (dosb(CONRAW, 0, 0));
#endif

#if     MSDOS && __ZTC__
        int c;

#if __OS2__
        c = getch();
        if (c == 0)
                c = getch() << 8;
#else
        if (intsignal)
        {       intsignal = FALSE;
                return 3;                       /* ^C                   */
        }
	if (islastc)
	{   c = lastc;
	    islastc = 0;
	}
	else
	    c = bdos(7,0) & 0xFF;
        if (c == 0)                             /* if extended code coming */
            c = bdos(7,0) << 8;
#endif
        return c & 0xFFFF;
#endif

#if     BSDUNIX || linux
#if NONBLOCK
        char buf;

        if (islastchar)
        {       islastchar = 0;
                return lastchar;
        }
        while (read(0,&buf,1) != 1)
                usleep(500);
        return buf;
#else
        return(fgetc(stdin));
#endif
#endif

#if NCURSES
        return getch();
#endif
}

/**************************
 * Return TRUE if there are unread chars ready in the input.
 */

int ttkeysininput()
{
#if BSDUNIX
#if NONBLOCK
        char buf;

        if (!islastchar && read(0,&buf,1) == 1)
        {       lastchar = buf;
                islastchar = TRUE;
        }
        return islastchar;
#else
        int num_chars_in_input_buf;

        ioctl( 0, FIONREAD, &num_chars_in_input_buf );
        return num_chars_in_input_buf;
#endif
#endif

#if MSDOS && !__OS2__
#if 0
        return intsignal || (bdos(0x0B,0) & 0xFF) == 0xFF;
#else
	if (intsignal)
	    return TRUE;
	__asm
	{
		mov	AH,6
		mov	DL,0FFh
		int	21h
		jz	L1
		mov	byte ptr lastc,AL
	}
	islastc = 1;
	return TRUE;
L1:
#endif
#endif
        return FALSE;
}

/******************************
 */

int ttyield()
{
}

#endif
