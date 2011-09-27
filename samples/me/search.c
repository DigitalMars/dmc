/*_ search.c   Tue Nov 17 1987 */
/* $Header$ */
/*
 * The functions in this file implement commands that search in the forward
 * and backward directions. There are no special characters in the search
 * strings. Probably should have a regular expression search, or something
 * like that.
 *
 * REVISION HISTORY:
 *
 * ?    Steve Wilhite, 1-Dec-85
 *      - massive cleanup on code.
 */

#include        <stdio.h>
#include	<ctype.h>
#include        "ed.h"

#define CASESENSITIVE TRUE	/* TRUE means case sensitive		*/

#ifdef MYIO
extern	int	Dnoask_search;
#endif

#if CASESENSITIVE
#define eq(a,b)	((a) == (b))
#endif

/*
 * Search forward. Get a search string from the user, and search, beginning at
 * ".", for the string. If found, reset the "." to be just after the match
 * string, and [perhaps] repaint the display. Bound to "C-S".
 */
forwsearch(f, n)
    {
    register LINE *clp;
    register int cbo;
    register int len;
    register LINE*tlp;
    register int tbo;
    register int c;
    register char *pp;
    register int s;

    if ((s = readpattern("Search: ",pat)) != TRUE)
        return (s);

    clp = curwp->w_dotp;		/* get pointer to current line	*/
    cbo = curwp->w_doto;		/* and offset into that line	*/

    len = llength(clp);
    while (clp != curbp->b_linep)	/* while not end of buffer	*/
        {
	while (cbo < len)
		if (eq(lgetc(clp,cbo++),pat[0]))
			goto match1;
	cbo = 0;
	clp = lforw(clp);
	len = llength(clp);
	if (!eq('\n',pat[0]))
		continue;

    match1:
            {
            tlp = clp;
            tbo = cbo;			/* remember where start of pattern */

	    for (pp = &pat[1]; *pp; pp++)
                {
                if (tlp == curbp->b_linep)	/* if reached end of buffer */
                    goto fail;

                if (tbo == llength(tlp))
                    {
                    tlp = lforw(tlp);
                    tbo = 0;
                    c = '\n';
                    }
                else
                    c = lgetc(tlp, tbo++);

                if (!eq(c, *pp))
                    goto fail;
                }

	    /* We've found it. It starts at clp,cbo and ends before tlp,tbo */
            curwp->w_dotp  = tlp;
            curwp->w_doto  = tbo;
            curwp->w_flag |= WFMOVE;
            return (TRUE);
            }
fail:;
        }

    mlwrite("Not found");
    return (FALSE);
}

/*
 * Reverse search. Get a search string from the user, and search, starting at
 * "." and proceeding toward the front of the buffer. If found "." is left
 * pointing at the first character of the pattern [the last character that was
 * matched]. Bound to "C-R".
 */
backsearch(f, n)
{
    register LINE *clp;
    register int cbo;
    register LINE *tlp;
    register int tbo;
    register int c;
    register char *epp;
    register char *pp;
    register int s;

    if ((s = readpattern("Reverse search: ",pat)) != TRUE)
        return (s);

    for (epp = &pat[0]; epp[1] != 0; ++epp)
        ;

    clp = curwp->w_dotp;
    cbo = curwp->w_doto;

    for (;;)
        {
        if (cbo == 0)
            {
            clp = lback(clp);

            if (clp == curbp->b_linep)
                {
                mlwrite("Not found");
                return (FALSE);
                }

            cbo = llength(clp)+1;
            }

        if (--cbo == llength(clp))
            c = '\n';
        else
            c = lgetc(clp, cbo);

        if (eq(c, *epp))
            {
            tlp = clp;
            tbo = cbo;
            pp  = epp;

            while (pp != &pat[0])
                {
                if (tbo == 0)
                    {
                    tlp = lback(tlp);
                    if (tlp == curbp->b_linep)
                        goto fail;

                    tbo = llength(tlp)+1;
                    }

                if (--tbo == llength(tlp))
                    c = '\n';
                else
                    c = lgetc(tlp, tbo);

                if (!eq(c, *--pp))
                    goto fail;
                }

            curwp->w_dotp  = tlp;
            curwp->w_doto  = tbo;
            curwp->w_flag |= WFMOVE;
            return (TRUE);
            }
fail:;
        }
}

/*
 * Compare two characters. The "bc" comes from the buffer. It has it's case
 * folded out. The "pc" is from the pattern.
 */

#ifndef eq
eq(bc, pc)
    int bc;
    int pc;
    {
    if (bc>='a' && bc<='z')
        bc -= 0x20;

    if (pc>='a' && pc<='z')
        pc -= 0x20;

    return (bc == pc);
    }
#endif

/*********************************
 * Replace occurrences of pat with withpat.
 */

int replacestring(f,n)
{
	return replace(FALSE);
}

/********************************
 */

int queryreplacestring(f,n)
{
	return replace(TRUE);
}

/*************************
 * Do the replacements.
 * Input:
 *	query	if TRUE then it's a query-search-replace
 */

static replace(query)
{
    register    LINE * clp;
    register int    cbo;
    register    LINE * tlp;
    register int    tbo;
    register int    c;
    register char  *pp;
    register int    s;
    register int    numreplacements;
    register int    retval;
    struct LINE *dotpsave;
    int dotosave;
    static char withpat[NPAT];
    int stop;

#if DLC || __ZTC__
    _chkstack();			/* check for stack overflow	*/
#endif
    if ((s = readpattern("Replace: ", pat)) != TRUE)
	return (s);			/* must have search pattern	*/
    /*withpat[0] = 0;*/			/* no default replacement pattern */
    readpattern ("With: ", withpat);	/* replacement pattern can be null */

    stop = FALSE;
    retval = TRUE;
    numreplacements = 0;
    dotpsave = curwp->w_dotp;
    dotosave = curwp->w_doto;		/* save original position	*/
    clp = curwp -> w_dotp;		/* get pointer to current line	 */
    cbo = curwp -> w_doto;		/* and offset into that line	 */

    while (clp != curbp -> b_linep)	/* while not end of buffer	 */
    {
	/* Compute c, the character at the current position		 */
	if (cbo >= llength (clp))	/* if at end of line		 */
	{
	    clp = lforw (clp);
	    cbo = 0;
	    c = '\n';			/* then current char is a newline */
	}
	else
	    c = lgetc (clp, cbo++);	/* else get char from line	 */

	if (eq (c, pat[0]))		/* if char matches start of pattern */
	{
	    tlp = clp;
	    tbo = cbo;			/* remember where start of pattern */

	    for (pp = &pat[1]; *pp; pp++)
	    {
		if (tlp == curbp -> b_linep)/* if reached end of buffer */
		    goto fail;

		if (tbo == llength (tlp))
		{
		    tlp = lforw (tlp);
		    tbo = 0;
		    c = '\n';
		}
		else
		    c = lgetc (tlp, tbo++);

		if (!eq (c, *pp))
		    goto fail;
	    }

	    /* We've found it. It starts before clp,cbo and ends	*/
	    /* before tlp,tbo						*/

	    /* If query, get user input about this			*/
	    if (query)
	    {
		curwp->w_dotp= clp;
		curwp->w_doto = cbo;
		backchar(FALSE,1);
		mlwrite("' ' change 'n' continue '!' change rest '.' change and stop ^G abort");
		curwp->w_flag |= WFMOVE;
	      tryagain:
		update();
		switch (getkey())
		{
		    case 'n':		/* don't change, but continue	*/
			goto fail;
		    /*case 'R':*/	/* enter recursive edit		*/
		    case '!':		/* change rest w/o asking	*/
			query = FALSE;
			/* FALL-THROUGH */
		    case ' ':		/* change and continue to next	*/
			break;
		    case '.':		/* change and stop		*/
			stop = TRUE;
			break;
		    case CTRL('G'):	/* abort			*/
			goto abortreplace;
		    default:		/* illegal command		*/
			(*term.t_beep)();
			goto tryagain;
		}
	    }

	    /* Delete the pattern by setting the current position to	*/
	    /* the start of the pattern, and deleting 'n' characters	*/
	    curwp -> w_flag |= WFHARD;
	    curwp->w_dotp= clp;
	    curwp->w_doto = cbo;
	    if (backchar(FALSE,1) == FALSE || line_delete(pp - pat,FALSE) == FALSE)
		goto L1;

	    /* 'Yank' the replacement pattern back in at dot (also	*/
	    /* moving cursor past end of replacement pattern to prevent	*/
	    /* recursive replaces).					*/
	    for (pp = withpat; *pp; pp++)
		if (line_insert(1,*pp) == FALSE)
		{
		L1:
			if (dotpsave == clp)
				dotpsave = curwp->w_dotp;
			retval = FALSE;
			goto abortreplace;
		}

	    /* Take care of case where line_insert() reallocated the line	*/
	    if (dotpsave == clp)
		dotpsave = curwp->w_dotp;
	    clp = curwp -> w_dotp;
	    cbo = curwp -> w_doto;	/* continue from end of with text */
	    numreplacements++;
	    if (stop)
		break;
	}
fail:	;
    }

abortreplace:
    curwp->w_dotp = dotpsave;
    curwp->w_doto = dotosave;		/* back to original position	*/
    curwp->w_flag |= WFMOVE;
    mlwrite ("%d replacements done",numreplacements);
    return retval;
}

/*
 * Read a pattern. Stash it in the external variable "pat". The "pat" is not
 * updated if the user types in an empty line. If the user typed an empty line,
 * and there is no old pattern, it is an error. Display the old pattern, in the
 * style of Jeff Lomicka. There is some do-it-yourself control expansion.
 */
static int readpattern(prompt,pat)
    char *prompt,*pat;
    {
    register char *cp1;
    register char *cp2;
    register int c;
    register int s;
    char tpat[NPAT+20];

#ifdef MYIO
    if( Dnoask_search )
	return( pat[0] != '\0' );
#endif
#if 0
    cp1 = &tpat[0];                     /* Copy prompt */
    cp2 = prompt;

    while ((c = *cp2++) != '\0')
        *cp1++ = c;

    if (pat[0] != '\0')                 /* Old pattern */
        {
        *cp1++ = ' ';
        *cp1++ = '[';
        cp2 = &pat[0];

        while ((c = *cp2++) != 0)
            {
            if (cp1 < &tpat[NPAT+20-6]) /* "??]: \0" */
                {
                if (c<0x20 || c==0x7F) {
                    *cp1++ = '^';
                    c ^= 0x40;
                    }
                else if (c == '%')      /* Map "%" to */
                    *cp1++ = c;         /* "%%". */

                *cp1++ = c;
                }
            }

        *cp1++ = ']';
        }

    *cp1++ = ':';                       /* Finish prompt */
    *cp1++ = ' ';
    *cp1++ = '\0';
    s = mlreply(tpat, tpat, NPAT);      /* Read pattern */
#else
    strcpy(tpat,pat);
    s = mlreply(prompt,tpat,NPAT);
#endif

    if (s == TRUE)                      /* Specified */
        strcpy(pat, tpat);
    else if (s == FALSE && pat[0] != 0)         /* CR, but old one */
        s = TRUE;

    return (s);
    }

/*********************************
 * Examine line at '.'.
 * Returns:
 *	HASH_xxx
 *	0	anything else
 */

#define HASH_IF		1
#define HASH_ELIF	2
#define HASH_ELSE	3
#define HASH_ENDIF	4

static int ifhash(clp)
LINE *clp;
{
    register int len;
    register int i,h;
    static char *hash[] = {"if","elif","else","endif"};

    len = llength(clp);
    if (len < 3 || lgetc(clp,0) != '#')
	goto ret0;
    for (i = 1; ; i++)
    {
	if (i >= len)
	    goto ret0;
	if (!isspace(lgetc(clp,i)))
	    break;
    }
    for (h = 0; h < arraysize(hash); h++)
	if (len - i >= strlen(hash[h]) &&
	    memcmp(&clp->l_text[i],hash[h],strlen(hash[h])) == 0)
	    return h + 1;
ret0:
    return 0;
}

/*********************************
 * Search for the next occurence of the character at '.'.
 * If character is a (){}[]<>, search for matching bracket.
 * If '.' is on #if, #elif, or #else search for next #elif, #else or #endif.
 * If '.' is on #endif, search backwards for corresponding #if.
 */

int search_paren(f, n)
    {
    register LINE *clp;
    register int cbo;
    register int len;
    register int i;
    char chinc,chdec,ch;
    int count;
    int forward;
    int h;
    static char bracket[][2] = {{'(',')'},{'<','>'},{'[',']'},{'{','}'}};

    clp = curwp->w_dotp;		/* get pointer to current line	*/
    cbo = curwp->w_doto;		/* and offset into that line	*/
    count = 0;

    len = llength(clp);
    if (cbo >= len)
	chinc = '\n';
    else
	chinc = lgetc(clp,cbo);

    if (cbo == 0 && (h = ifhash(clp)) != 0)
    {	forward = h != HASH_ENDIF;
    }
    else
    {
	forward = TRUE;			/* forward			*/
	h = 0;
	chdec = chinc;
	for (i = 0; i < 4; i++)
	    if (bracket[i][0] == chinc)
	    {	chdec = bracket[i][1];
		break;
	    }
	for (i = 0; i < 4; i++)
	    if (bracket[i][1] == chinc)
	    {	chdec = bracket[i][0];
		forward = FALSE;	/* search backwards		*/
		break;
	    }
    }

    while (1)				/* while not end of buffer	*/
    {
	if (forward)
	{
	    if (h || cbo >= len)
	    {
		clp = lforw(clp);
		if (clp == curbp->b_linep)	/* if end of buffer	*/
		    break;
		len = llength(clp);
		cbo = 0;
	    }
	    else
		cbo++;
	}
	else /* backward */
	{
	    if (h || cbo == 0)
            {
		clp = lback(clp);
		if (clp == curbp->b_linep)
		    break;
		len = llength(clp);
		cbo = len;
            }
	    else
		--cbo;
	}

	if (h)
	{   int h2;

	    cbo = 0;
	    h2 = ifhash(clp);
	    if (h2)
	    {	if (h == HASH_ENDIF)
		{
		    if (h2 == HASH_ENDIF)
			count++;
		    else if (h2 == HASH_IF)
		    {	if (count-- == 0)
			    goto found;
		    }
		}
		else
		{   if (h2 == HASH_IF)
			count++;
		    else
		    {	if (count == 0)
			    goto found;
			if (h2 == HASH_ENDIF)
			    count--;
		    }
		}
	    }
	}
	else
	{
	    ch = (cbo < len) ? lgetc(clp,cbo) : '\n';
	    if (eq(ch,chdec))
	    {   if (count-- == 0)
		{
		    /* We've found it	*/
		found:
		    curwp->w_dotp  = clp;
		    curwp->w_doto  = cbo;
		    curwp->w_flag |= WFMOVE;
		    return (TRUE);
		}
	    }
	    else if (eq(ch,chinc))
		count++;
	}
    }
    mlwrite("Not found");
    return (FALSE);
}
