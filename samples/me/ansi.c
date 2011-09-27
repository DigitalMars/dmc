/*_ ansi.c   Fri Feb  7 1986   Modified by: Bjorn Benson */
/*
 * The routines in this file provide support for ANSI style terminals
 * over a serial line. The serial I/O services are provided by routines in
 * "termio.c". It compiles into nothing if not an ANSI device.
 */

#include        <stdio.h>
#include        "ed.h"

#if     ANSI || ANSISYS

#define NROW    23                      /* Screen size.                 */
#define NCOL    77                      /* Edit if you want to.         */
#define BEL     0x07                    /* BEL character.               */
#define ESC     0x1B                    /* ESC character.               */

extern  int     ttopen();               /* Forward references.          */
extern  int     ttgetc();
extern  int     ttputc();
extern  int     ttflush();
extern  int     ttclose();
extern  int     ansimove();
extern  int     ansieeol();
extern  int     ansieeop();
extern  int     ansibeep();
extern  int     ansiopen();
extern	int	ansinull();
extern	int	ansistartstand();
extern	int	ansiendstand();
extern	int	ansiscrup();
extern	int	ansiscrdn();

/*
 * Standard terminal interface dispatch table. Most of the fields point into
 * "termio" code.
 */
TERM    term    = {
        NROW-1,
        NCOL,
        ansiopen,
        ttclose,
        ttgetc,
        ttputc,
        ttflush,
        ansimove,
        ansieeol,
        ansieeop,
        ansibeep,
	ansistartstand,
	ansiendstand,
#if ANSISYS
	NULL,NULL		/* scrolling regions don't work right	*/
#else
	ansiscrup,
	ansiscrdn
#endif
};

ansinull()
{
	/* null function - do nothing */
}

ansimove(row, col)
{
        ttputc(ESC);
        ttputc('[');
        ansiparm(row+1);
        ttputc(';');
        ansiparm(col+1);
        ttputc('H');
}

ansieeol()
{
        ttputc(ESC);
        ttputc('[');
        ttputc('K');
}

ansieeop()
{
        ttputc(ESC);
        ttputc('[');
        ttputc('J');
}

ansibeep()
{
        ttputc(BEL);
        ttflush();
}

ansistartstand()
{
	ttputc(ESC); ttputc('['); ttputc('7'); ttputc('m');
}

ansiendstand()
{
	ttputc(ESC); ttputc('['); ttputc('0'); ttputc('m');
}

ansiscrup( first, last )
int first,last;
{
        ttputc(ESC);
        ttputc('[');
        ansiparm(first+1);
	ttputc(';');
	ansiparm(last+1);
	ttputc('r');
	ttputc(ESC);
	ttputc('[');
	ansiparm(last+1);
	ttputc(';');
	ttputc('1');
	ttputc('f');
	ttputc('\n');
        ttputc(ESC);
        ttputc('[');
	ttputc('1');
	ttputc(';');
	ansiparm( NROW+1 );
	ttputc('r');
}

ansiscrdn( first, last )
int first,last;
{
        ttputc(ESC);
        ttputc('[');
        ansiparm(first+1);
	ttputc(';');
	ansiparm(last+1);
	ttputc('r');
	ttputc(ESC);
	ttputc('[');
	ansiparm(first+1);
	ttputc(';');
	ttputc('1');
	ttputc('f');
	ttputc(ESC);
	ttputc('M');
        ttputc(ESC);
        ttputc('[');
	ttputc('1');
	ttputc(';');
	ansiparm( NROW+1 );
	ttputc('r');
}

ansiparm(n)
register int    n;
{
        register int    q;

        q = n/10;
        if (q != 0)
                ansiparm(q);
        ttputc((n%10) + '0');
}

ansiopen()
{
#if     BSDUNIX || LINUX
        register char *cp;
        char *getenv();

        if ((cp = getenv("TERM")) == NULL) {
                puts("Shell variable TERM not defined!");
                exit(1);
        }
        if (strcmp(cp, "vt100") != 0) {
                puts("Terminal type not 'vt100'!");
                exit(1);
        }
#endif
        ttopen();
}

#endif
