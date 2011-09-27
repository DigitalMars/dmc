/*_ more.c   Mon Dec 10 2001 */
/* $Header$ */
/*
 * Due to my (Bjorn Benson) laziness, the functions will all
 * work with a positive argument, but may or may not with
 * a negative.
 */
#include        <stdio.h>
#include        <ctype.h>
#include        "ed.h"

int	Dnoask_search = FALSE;	/* TRUE for the search again function	*/

/*
 * The multiple delete buffers
 */
#define	DK_CUT	0
#define	DK_LINE	1
#define	DK_WORD	2
#define	DK_CHAR	3

#define	SETMARK { curwp->w_markp = curwp->w_dotp; curwp->w_marko = curwp->w_doto; }

/*
 * Current direction that things happen in
 */
#define	ADVANCE	0
#define	BACKUP	1
static	int	Dcur_direction = ADVANCE;

Dsearch(f, n)
{
	if( Dcur_direction == ADVANCE )
		return( forwsearch(f, n) );
	else	return( backsearch(f, n) );
}

Dsearchagain(f, n)
{
	register int s;
	Dnoask_search = TRUE;
	if( Dcur_direction == ADVANCE )
		s = forwsearch(f, n);
	else	s = backsearch(f, n);
	Dnoask_search = FALSE;
	return( s );
}

Ddelline(f, n)
{
	register int s = TRUE;

	kill_setbuffer(DK_LINE);
	kill_freebuffer();
	while( n-- > 0 && s )
	{   curwp->w_doto = 0;
	    s &= line_delete(llength(curwp->w_dotp) + 1, TRUE);
	}
	kill_setbuffer(DK_CUT);
	return( s );
}

Dundelline(f, n)
{
	register int s = TRUE;

	kill_setbuffer(DK_LINE);
	while( n-- > 0 && s )
	{
		curwp->w_doto = 0;
		s = random_yank(TRUE, 1);
		backline(FALSE, 1);
		curwp->w_doto = 0;
	}
	kill_setbuffer(DK_CUT);
	return( s );
}

Ddelword(f, n)
{
	register int s = TRUE;

	kill_setbuffer(DK_WORD);
	kill_freebuffer();
	while( n-- > 0 && s )
	{
		SETMARK;
		s = word_forw(FALSE, 1);
		if( !s ) break;
		s = region_kill(FALSE, 1);
	}
	kill_setbuffer(DK_CUT);
	return( s );
}

Ddelbword(f, n)
{
	register int s = TRUE;

	kill_setbuffer(DK_WORD);
	kill_freebuffer();
	while( n-- > 0 && s )
	{
		SETMARK;
		s = word_back(FALSE, 1);
		if( !s ) break;
		s = region_kill(FALSE, 1);
	}
	kill_setbuffer(DK_CUT);
	return( s );
}

Dundelword(f, n)
{
	register int s = TRUE;

	kill_setbuffer(DK_WORD);
	while( n-- > 0 && s )
		s &= random_yank(TRUE, 1);
	kill_setbuffer(DK_CUT);
	return( s );
}

Dadvance()
{
	Dcur_direction = ADVANCE;
	return( TRUE );
}

Dbackup()
{
	Dcur_direction = BACKUP;
	return( TRUE );
}

Dignore()
{
	/* Ignore this command. Useful for ^S and ^Q flow control	*/
	/* sent out by some terminals.					*/
	return TRUE;
}

Dpause()
{
#ifdef hpux
	return FALSE;
#else
#if BSDUNIX || LINUX
	extern int sgarbf;
	(*term.t_move)( term.t_nrow - 1, 0 );
	(*term.t_eeop)();
	ttflush();
	ttclose();
	killpg(getpgrp(0), 18);	/* SIGTSTP -- stop the current program */
	ttopen();
	sgarbf = TRUE;
	window_refresh(FALSE, 1);
#endif
	return( TRUE );
#endif
}

/*********************************
 * Decide whether to uppercase a word or a region.
 */

misc_upper(f,n)
int f,n;
{
    if (curwp->w_markp)
	return region_upper(f,n);
    else
	return word_upper(f,n);
}

/*********************************
 * Decide whether to lowercase a word or a region.
 */

misc_lower(f,n)
int f,n;
{
    if (curwp->w_markp)
	return region_lower(f,n);
    else
	return word_lower(f,n);
}

/*********************************
 * Insert file name and date at top of file.
 */

Dinsertdate(f,n)
int f,n;
{	int s,b;
	char buf[100],*p;

	if (!gotobob(FALSE,0))		/* move to beginning of buffer	*/
		goto err;

	/* search for "_ "	*/
	strcpy(pat,"_ ");		/* load search pattern		*/
	b = Dnoask_search;
	Dnoask_search = TRUE;		/* don't prompt for search string */
	s = forwsearch(FALSE,0);	/* search forward for "_ "	*/
	pat[0] = 0;			/* eliminate search pattern	*/
	Dnoask_search = b;		/* restore to original value	*/
	if (!s)				/* if search failed		*/
		goto err;		/* quit				*/

	if (!random_kill(FALSE,0))	/* delete to end of line	*/
		goto err;

	/* load up string to be inserted	*/
	p = curbp->b_fname;
	p += strlen(p);
	while (p > curbp->b_fname && p[-1] != '/'
#if MSDOS || _WIN32
		&& p[-1] != '\\' && p[-1] != ':'
#endif
		) p--;
	strcpy(buf,p);			/* file name			*/
	strcat(buf,"   ");		/* followed by spaces		*/
#if BSDUNIX || LINUX || MSDOS || _WIN32
	{	long t,time();
		char *ctime(),*getlogin(),*getenv();

		t = time((long *) NULL);
		p = ctime(&t);		/* p -> time string		*/
		p[10] = 0;
		p[24] = 0;		/* remove the '\n'		*/
		strcat(buf,p);
		strcat(buf,p + 10 + 9);
		p = getenv("NAME");	/* get user's name		*/
#if BSDUNIX || LINUX
		if (!p)
			p = getlogin();	/* get login name		*/
#endif
		if (p)
		{	strcat(buf,"   Modified by: ");
			strcat(buf,p);
		}
	}
#endif
	strcat(buf," */");		/* end of comment		*/

	/* insert the string	*/
	for (p = buf; *p; p++)
	{	if (line_insert(1,*p) == FALSE)
			goto err;
	}

	return TRUE;			/* all succeeded		*/

err:	return FALSE;
}

/***********************************
 * Remove trailing whitespace from line.
 */

void deblank()
{   int len;
    int n;
    int c;
    int i;

    len = llength(curwp->w_dotp);
    for (i = len - 1; i >= 0; i--)
    {
	c = lgetc(curwp->w_dotp, i);
	if (!isspace(c))
	    break;
    }
    n = (len - 1) - i;
    if (n)
    {
	curwp->w_doto = i + 1;
	line_delete(n,FALSE);
    }
}

/*********************************
 * Convert C comment to C++ comment.
 */

int Dcppcomment(int f,int n)
{
        int    c;
        int    i;
	struct LINE *dotpsave;
	int dotosave;

        if (n < 0)
	    goto err;
	if (window_marking(curwp))
	{   REGION region;
	    int s;

	    if ((s = getregion(&region)) != TRUE)
		return s;
	    dotpsave = curwp->w_dotp;
	    dotosave = curwp->w_doto;
	    curwp->w_dotp = region.r_linep;
	    curwp->w_doto = region.r_offset;
	    n = region.r_nlines;
	}
        while (n--)
	{   int len;

	    deblank();
	    len = llength(curwp->w_dotp);
	    if (len)
	    {
		for (i = 0; i + 3 < len; i++)
		{
		    c = lgetc(curwp->w_dotp, i);
		    if (c == '/' && lgetc(curwp->w_dotp, i + 1) == '*')
		    {
			if (lgetc(curwp->w_dotp, len - 2) == '*' &&
			    lgetc(curwp->w_dotp, len - 1) == '/')
			{
			    curwp->w_doto = i + 1;
			    line_delete(1,FALSE);
			    line_insert(1,'/');
			    curwp->w_doto = len - 2;
			    line_delete(2,FALSE);
			    deblank();
			    break;
			}
		    }
                }
		curwp->w_doto = 0;	/* move to beginning of line	*/
	    }
	    if (forwline(FALSE,1) == FALSE)
		goto err;
        }
	if (window_marking(curwp))
	{
	    if (dotosave > llength(dotpsave))
		dotosave = llength(dotpsave);
	    curwp->w_dotp = dotpsave;
	    curwp->w_doto = dotosave;
	}
        return TRUE;

err:
	return FALSE;
}
