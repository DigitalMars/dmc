/*_ tcap.c   Fri Mar 28 1986 */
#include <stdio.h>
#include "ed.h"

#include <time.h>

#if TERMCAP

#define NROW    24
#define NCOL    80
#define BEL     0x07
#define ESC     0x1B

extern int      ttopen();
extern int      ttgetc();
extern int      ttputc();
extern int      ttflush();
extern int      ttclose();
extern int      tcapmove();
extern int      tcapeeol();
extern int      tcapeeop();
extern int      tcapbeep();
extern int      tcapopen();
extern int	tcapclose();
extern int	tcapgetc();
extern int      tput();
extern char     *tgoto();
extern int	tcapstartstand();
extern int	tcapendstand();
extern int	tcapscrup();
extern int	tcapscrdn();

#define TCAPSLEN 2048

char tcapbuf[TCAPSLEN];
static char *p;		/* roving pointer into tcapbuf[]	*/
char    PC,
        *CM,
        *CL,
        *CE,
        *UP,
        *CD,
	*KU,	/* up arrow key */
	*KD,	/* down arrow key */
	*KL,	/* left arrow key */
	*KR,	/* right arrow key */
	*K1,	/* second meta key */
	*SO,	/* start standout mode */
	*SE,	/* end standout mode */
	*VB,	/* visible bell */
	*CS,	/* scrolling region */
	*SR,	/* scroll reverse */
	*SF,	/* scroll forward */
	*DO,	/* move down (alt for scroll forward) */
	*AL,	/* add a line */
	*DL;	/* delete a line */

/*
 * The LONGKEY structures are used to map the keys that produce
 * escape sequences to single return values.  Each LONGKEY element
 * is allocated as an array of LONGKEY structures.  lk[0].key is
 * the number of structure-1, lk[0].ptr is a backpointer.  lk[1] is
 * the first entry, etc.  These structures are dynamcially allocated
 * and linked at initialize time.  See tcapgetc() to see how they
 * are used.
 */
#define	LONGFINAL(C)	((C)&0x80)
#define	LONGCHAR(C)	((C)&0x7F)
struct LONGKEY {
	unsigned char key;
	union {
	  struct LONGKEY *Uptr;
	  int Ukeyv;
	} U;
	};
#define	ptr	U.Uptr
#define	keyv	U.Ukeyv
struct LONGKEY *lkroot = NULL;

TERM term = {
        NROW-1,
        NCOL,
        &tcapopen,
        &tcapclose,
	&tcapgetc,
        &ttputc,
        &ttflush,
        &tcapmove,
        &tcapeeol,
        &tcapeeop,
        &tcapbeep,
	&tcapstartstand,
	&tcapendstand,
	&tcapscrup,
	&tcapscrdn
};

static int scroll_type;	/* type of scrolling region (used in tcapscr{up|dn}) */

tcapopen()

{
        char *getenv();
        char *t, *tgetstr();
        char tcbuf[1024];
        char *tv_stype;
        char err_str[72];

        if ((tv_stype = getenv("TERM")) == NULL)
        {
                puts("Environment variable TERM not defined!");
                exit(1);
        }

        if((tgetent(tcbuf, tv_stype)) != 1)
        {
                sprintf(err_str, "Unknown terminal type %s!", tv_stype);
                puts(err_str);
                exit(1);
        }

        p = tcapbuf;
        t = tgetstr("pc", &p);
        if(t)
                PC = *t;

	term.t_ncol = tgetnum("co");
	term.t_nrow = tgetnum("li");
        CD = tgetstr("cd", &p);
        CM = tgetstr("cm", &p);
        CE = tgetstr("ce", &p);
        UP = tgetstr("up", &p);
	//printf("UP=%p,%02x,%02x,%02x '%s'\n\n\n\n",UP,*UP,UP[1],UP[2],UP); fflush(stdout); sleep(1);
	VB = tgetstr("vb", &p);
	KU = tgetstr("ku", &p);
	//printf("KU=%p,%02x,%02x,%02x '%s'\n\n\n\n",KU,*KU,KU[1],KU[2],KU); fflush(stdout); sleep(3);
	KD = tgetstr("kd", &p);
	KL = tgetstr("kl", &p);
	KR = tgetstr("kr", &p);
	K1 = tgetstr("k1", &p);
	SO = tgetstr("so", &p);
	SE = tgetstr("se", &p);
	CS = tgetstr("cs", &p);
	SR = tgetstr("sr", &p);
	SF = tgetstr("sf", &p);
	DO = tgetstr("do", &p);
	AL = tgetstr("al", &p);
	DL = tgetstr("dl", &p);
	build_long_keys( tv_stype );

        if(CD == NULL || CM == NULL || CE == NULL || UP == NULL
	 || term.t_ncol < 1 || term.t_nrow < 1 )
        {
                puts("Incomplete termcap entry\n");
                exit(1);
        }

        if (p >= &tcapbuf[TCAPSLEN])
        {
                puts("Terminal description too big!\n");
                exit(1);
        }

	if(AL && DL)
	{
		scroll_type = 0;
	}
	else if (CS && SR && (SF || DO))
	{
		scroll_type = 1;
		if( !SF ) SF = DO;
	}
	else
	{
		term.t_scrollup = NULL;
		term.t_scrolldn = NULL;
	}

        ttopen();
	if( strcmp(tv_stype,"vt100") == 0 )
		fputs("\033=",stdout);	/* turn on the keypad	*/
}

/*****************************
 * Close use of termcap.
 */

tcapclose()
{
	if( strcmp(getenv("TERM"),"vt100") == 0 )
		fputs("\033[?7h",stdout);	/* turn on autowrap	*/
	ttclose();
}

tcapmove(row, col)
register int row, col;
{
        putpad(tgoto(CM, col, row));
}

tcapeeol()
{
        putpad(CE);
}

tcapeeop()
{
        putpad(CD);
}

tcapbeep()
{
	if( VB )
		putpad(VB);
	else
	        ttputc(BEL);
}

putpad(str)
char    *str;
{
        tputs(str, 1, ttputc);
}

putnpad(str, n)
char    *str;
{
        tputs(str, n, ttputc);
}

int tcapgetc()
{
	register int c,i;
	struct LONGKEY *lkp,*tmpp;

	static int backlen;
	static char backc[16];

	/*
	 * If there was a previously almost complete LONGKEY sequence
	 * that failed, then we have to send the characters as if they
	 * were pressed individually.
	 */
    start:
	if( backlen )
		return( backc[--backlen] );

	/*
	 * Continue following the LONGKEY structure sequence until
	 * either there is a complete match, or there is a complete
	 * failure to match.
	 */
	lkp = lkroot;
    loop:
	c = ttgetc();
	for(i=1; i<=lkp[0].key; i++)
	{
	    if( LONGCHAR(lkp[i].key) == c )
	    {
//static int x;
//printf("\nmatch %d\n",++x); fflush(stdout); sleep(2);
		if( LONGFINAL(lkp[i].key) )
			return( lkp[i].keyv );
		lkp = lkp[i].ptr;
		goto loop;
	    }
	}

#if 0
for(i=1; i<=lkp[0].key; i++)
{
  printf("\nLONGCHAR[%d] = x%02x\n",i,lkp[i].key);
}
printf("no match for x%02x of %d entries\n",c,i); fflush(stdout); sleep(10);
#endif
	/*
	 * Upon a complete failure to match, we fill up the backc[]
	 * array with the ASCII characters that have been entered
	 * so that future calls to tcapgetc() can return these
	 * characters is the correct order.
	 */
	backc[backlen++] = c;
	while( lkp[0].ptr != NULL )
	{
		tmpp = lkp;
		lkp = lkp[0].ptr;
		for(i=1; i<=lkp[0].key; i++)
			if( lkp[i].ptr == tmpp ) break;
		backc[backlen++] = lkp[i].key;
	}
	goto start;
}

static build_long_keys( term )
char *term;
{
	extern char *malloc();

	lkroot = (struct LONGKEY *)malloc( sizeof(struct LONGKEY) );
	lkroot->key = 0;	lkroot->ptr = NULL;

#if linux
        build_one_long("\033[A", UPKEY);
	build_one_long("\033[B", DNKEY);
	build_one_long("\033[C", RTKEY);
	build_one_long("\033[D", LTKEY);
	build_one_long( "\033\x4F\x50", F1KEY );
	build_one_long( "\033\x4F\x51", F2KEY );
	build_one_long( "\033\x4F\x52", F3KEY );
	build_one_long( "\033\x4F\x53", F4KEY );
	build_one_long( "\033\x5B\x31\x35\x7E", F5KEY );
	build_one_long( "\033\x5B\x31\x37\x7E", F6KEY );
	build_one_long( "\033\x5B\x31\x38\x7E", F7KEY );
	build_one_long( "\033\x5B\x31\x39\x7E", F8KEY );
	build_one_long( "\033\x5B\x32\x30\x7E", F9KEY );
	build_one_long( "\033\x5B\x32\x31\x7E", F10KEY );


	build_one_long("\033\x62", ALTB );
	build_one_long("\033\x63", ALTC );
	build_one_long("\033\x64", ALTD );
	build_one_long("\033\x64", ALTE );
	build_one_long("\033\x66", ALTF );
	build_one_long("\033\x68", ALTH );
	build_one_long("\033\x6D", ALTM );
	build_one_long("\033\x78", ALTX );
	build_one_long("\033\x7A", ALTZ );

	build_one_long("\033\x5B\x32\x7E", InsKEY );
	build_one_long("\033\x5B\x33\x7E", DelKEY );
	build_one_long("\033\x5B\x31\x7E", HOMEKEY );
	build_one_long("\033\x5B\x34\x7E", ENDKEY );
	build_one_long("\033\x5B\x35\x7E", PgUpKEY );
	build_one_long("\033\x5B\x36\x7E", PgDnKEY );
#else
	build_one_long( KU, UPKEY );
	build_one_long( KD, DNKEY );
	build_one_long( KR, RTKEY );
	build_one_long( KL, LTKEY );
	build_one_long( K1, GOLDKEY );
	build_one_long( tgetstr("kR",&p), SCROLLUPKEY ); /* scroll back */
	build_one_long( tgetstr("kF",&p), SCROLLDNKEY ); /* scroll forw */
	build_one_long( tgetstr("kP",&p), PAGEUPKEY ); /* prev page */
	build_one_long( tgetstr("kN",&p), PAGEDNKEY ); /* next page */
	if( strcmp(term,"vt100") == 0 )
	{
		build_one_long( "\033OQ", PF2KEY );
		build_one_long( "\033OR", PF3KEY );
		build_one_long( "\033OS", PF4KEY );
		build_one_long( "\033Op", F0KEY );
		build_one_long( "\033Oq", F1KEY );
		build_one_long( "\033Or", F2KEY );
		build_one_long( "\033Os", F3KEY );
		build_one_long( "\033Ot", F4KEY );
		build_one_long( "\033Ou", F5KEY );
		build_one_long( "\033Ov", F6KEY );
		build_one_long( "\033Ow", F7KEY );
		build_one_long( "\033Ox", F8KEY );
		build_one_long( "\033Oy", F9KEY );
		build_one_long( "\033Om", FMINUSKEY );
		build_one_long( "\033Ol", FCOMMAKEY );
		build_one_long( "\033On", FDOTKEY );
		build_one_long( "\033OM", FENTERKEY );
	}
	else
	{
		build_one_long( tgetstr("k2",&p), PF2KEY );
		build_one_long( tgetstr("k3",&p), PF3KEY );
		build_one_long( tgetstr("k4",&p), PF4KEY );
		build_one_long( tgetstr("k5",&p), F5KEY );
		build_one_long( tgetstr("k6",&p), F6KEY );
		build_one_long( tgetstr("k7",&p), F7KEY );
		build_one_long( tgetstr("k8",&p), F8KEY );
		build_one_long( tgetstr("k9",&p), F9KEY );
		build_one_long( "\033Op", F0KEY );
		build_one_long( "\033Oq", F1KEY );
		build_one_long( "\033Or", F2KEY );
		build_one_long( "\033Os", F3KEY );
		build_one_long( "\033Ot", F4KEY );
		build_one_long( "\033Om", FMINUSKEY );
		build_one_long( "\033[229z", FCOMMAKEY );
		build_one_long( "\033Ol", FCOMMAKEY );
		build_one_long( "\033OM", FENTERKEY );
	}
#endif
}

static build_one_long( s, keyval )
char *s;
int keyval;
{
	register int i;
	struct LONGKEY *lkp,*tmpp,*tmpq;

	if (!s)
		return;
	lkp = lkroot;
	while( *s )
	{
		for(i=1; i<=lkp[0].key; i++)
			if( lkp[i].key == *s ) break;
		if( i != lkp[0].key+1 )
		{
			lkp = lkp[i].ptr;
			s++;
		}
		else
		{
			tmpp = (struct LONGKEY *)malloc( 
				sizeof(struct LONGKEY) * (++i) );
			while( i-- )
			{
				tmpp[i].key = lkp[i].key;
				tmpp[i].ptr = lkp[i].ptr;
			}
			if( lkp[0].ptr )
			{
				tmpq = lkp[0].ptr;
				for(i=1; i<=tmpq[0].key; i++)
					if( tmpq[i].ptr == lkp )
						tmpq[i].ptr = tmpp;
			}
			else
				lkroot = tmpp;
			free( lkp );
			lkp = tmpp;
			for(i=1; i<=lkp[0].key; i++)
				if( !LONGFINAL(lkp[i].key) )
					lkp[i].ptr->ptr = lkp;
			lkp[0].key++;
			lkp[lkp[0].key].key = *s++;
			if( !*s )
			{
				lkp[lkp[0].key].keyv = keyval;
				lkp[lkp[0].key].key |= 0x80;
			}
			else
			{
				tmpp = lkp;
				lkp = lkp[lkp[0].key].ptr = (struct LONGKEY *)
					malloc( sizeof(struct LONGKEY) );
				lkp[0].key = 0;
				lkp[0].ptr = tmpp;
			}
		}
	}
}

tcapstartstand()
{
	if( SO )
		putpad( SO );
}

tcapendstand()
{
	if( SE )
		putpad( SE );
}

tcapscrup( first, last )
int first,last;
{
	if( scroll_type )
	{
		tpstr( CS, first, last );
		tcapmove( last, 0 );
		putpad( SF );
		tpstr( CS, 0, term.t_nrow - 1 );
		tcapmove( last, 0 );
	}
	else
	{
		tcapmove( first, 0 );
		putpad( DL );
		tcapmove( last, 0 );
		putpad( AL );
	}
}

tcapscrdn( first, last )
int first,last;
{
	if( scroll_type )
	{
		tpstr( CS, first, last );
		tcapmove( first, 0 );
		putpad( SR );
		tpstr( CS, 0, term.t_nrow - 1 );
		tcapmove( first, 0 );
	}
	else
	{
		tcapmove( last, 0 );
		putpad( DL );
		tcapmove( first, 0 );
		putpad( AL );
	}
}

tpstr( str, p1, p2 )
char *str;
int p1,p2;
{
	char buf[128];
	char *pt = buf;
	int pi;

	for(pi=0; pi<128;) buf[pi++] = '\0';
	pi = p1;
	while( *str >= '0' && *str <= '9' ) *pt++ = *str++;
	if( *str == '*' ) *pt++ = *str++;
	while( *str )
	{
		if( *str == '%' )
		{	str++;
			switch( *str++ )
			{
			  case 'd':
				sprintf(pt,"%d",pi);
				pi = p2; break;
			  case '2':
				sprintf(pt,"%2d",pi);
				pi = p2; break;
			  case '3':
				sprintf(pt,"%3d",pi);
				pi = p2; break;
			  case '.':
				sprintf(pt,"%c",pi);
				pi = p2; break;
			  case 'r':
				pi = p2;
				p2 = p1;
				p1 = pi;
				break;
			  case 'i':
				p1++; p2++; pi++; break;
			  case '%':
				*pt++ = '%'; break;
			  default:
				strcpy(pt,"\nTERMCAP Error\n");
				break;
			}
			pt = buf + strlen(buf);
		}
		else
			*pt++ = *str++;
	}
	*pt = '\0';
	putpad( buf );
}

#ifdef DEBUG
dump_longs()
{
	printf("__dump__\n");
	dump_one_long( 0, lkroot, NULL );
	printf("________\n");
}

int backcheck = 0;

dump_one_long( offset, lkp, lkprev )
struct LONGKEY *lkp,*lkprev;
{
	int i,j;
	if( backcheck )
	{
		for(j=0; j<offset; j++) printf(" ");
		printf( (lkprev == lkp[0].ptr) ? "TRUE\n" : "FALSE\n" );
	}
	for(i=1; i<=lkp[0].key; i++)
	{
		for(j=0; j<offset; j++) printf(" ");
		printf("%d (%c)      ",
			LONGCHAR(lkp[i].key), LONGCHAR(lkp[i].key));
		if( LONGFINAL(lkp[i].key) )
			printf("return( %d )\n", lkp[i].keyv );
		else
		{	printf("\n");
			dump_one_long( offset+1, lkp[i].ptr, lkp );
		}
	}
}
#endif

#endif TERMCAP
