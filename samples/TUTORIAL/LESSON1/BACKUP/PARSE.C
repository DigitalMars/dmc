/** parse.c
**
**   Part of TMLDOS, Symantec C++ 7.0 demonstration program
**   Copyright (C) 1994 Symantec Corporation
**
**   last mod:
**     rjw, 11/94 (release)
**
** Contents of this file:
**   parser for DOS TML reader
** Notes:
**   Parse() is called by main program to read and interpret TML files and
**    format them into a global buffer.
**
**   The command codes and interpretations thereof correspond largely to
**    standard HTML (as described in "A Beginner's Guide to HTML", available
**    at your nearest WWW browser).  Images are not supported in the DOS version.
**
**   The format of the output buffer is as follows:  Mostly just text.  Lines are
**    separated by EOLCODE.  Buffer ends with a 0.  Escape codes are enclosed by
**    two ESCAPECODE characters.  They are guaranteed not to straddle an EOLCODE.
**    The first character after the first ESCAPECODE gives the command; for anchors
**    and hyperlinks, this is followed by the argument.  Escape codes which might be
**    found are:
**       '1'-'6' to indicate start of header style,
**       'a'-'l' to indicate start of character style,
**       'A' for an anchor,
**       'H' for a hyperlink,
**       'X' to indicate the end of the hyperlink.
**    Anchors are not terminated with 'X'; they are just prepended
**    to the first word in the anchor, which is sufficient.
**
**   Formatting, such as indentation, numbering list items, word wrapping, etc.,
**    is done here in the parser, so the output routine need not worry about it.
**
**   Lines are each self-contained in terms of the escape sequences necessary to
**    display it with the proper styles.  Thus, if bold begins on one line, the
**    corresponding escape sequence is prepended to each following line until the
**    bold style is ended.  Similarly, if a hyperlink wraps around the end of a line,
**    the following line is started with another hyperlink escape.  On the other hand,
**    escapes which wrap around are not necessarily terminated at the end of the line.
**/
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "TMLDOS.h"

/**************************** #defines
*/

/* word/event types
*/
#define TYPE_WORD    0x01  /* contents is word, needs spaces, wrapping */
#define TYPE_LITERAL 0x02  /* contents is literal - doesn't need */
                           /*  spaces or wrapping */
#define TYPE_HRULE   0x03  /* horizontal rule */
#define TYPE_IMAGE   0x04  /* image - contents is file name */

#define TYPE_TITLE   0x05  /* document title */

/* character styles */

#define STYLE_NONE    0x00  /* no char styles - just plain */
#define STYLE_DFN     0x01  /* word being defined          */
#define STYLE_EM      0x02  /* emphasis                    */
#define STYLE_CITE    0x03  /* citation                    */
#define STYLE_VAR     0x04  /* metasyntactic variable      */
#define STYLE_I       0x05  /* italic                      */
#define STYLE_CODE    0x06  /* computer code sample        */
#define STYLE_KBD     0x07  /* keyboard entry              */
#define STYLE_SAMP    0x08  /* computer output             */
#define STYLE_TT      0x09  /* typewriter                  */
#define STYLE_STRONG  0x0a  /* strong emphasis             */
#define STYLE_B       0x0b  /* bold                        */

/* context bits */

#define CONTEXT_NONE     0x00  /* no special context (ordinary paragraph) */
#define CONTEXT_NEWLINE  0x01  /* word must start new line */
#define CONTEXT_NEWPAR   0x02  /* word must start new paragraph */
#define CONTEXT_ANCHOR   0x04  /* anchor */
#define CONTEXT_LINK     0x08  /* hyperlink */
#define CONTEXT_FIRST    0x10  /* first word in this context */
#define CONTEXT_SENT     0x20  /* start of sentence */
#define CONTEXT_FIRSTLNK 0x40  /* first word in link or anchor */
#define CONTEXT_FLAGS    0xff

#define CONTEXT_HEADING  0x100  /* heading (wHLevel specifies level) */
#define CONTEXT_ULIST    0x200  /* unordered list item */
#define CONTEXT_OLIST    0x300  /* ordered list item */
                                          /*  (wOLItem specifies item #) */
#define CONTEXT_QUOTE    0x400  /* block quote */
#define CONTEXT_PREFORM  0x500  /* preformatted text */
#define CONTEXT_BLOCKS   0xf00


/* Error codes returned by Parse(),
** correspond to error messages returned by ErrorMessage()
*/
#define PARSE_ERROR_TOOBIG   1
#define PARSE_ERROR_LONGLINE 2
#define PARSE_ERROR_MAXWORDS 3
#define PARSE_ERROR_BADCODE  4
#define PARSE_ERROR_BADARG   5
#define PARSE_ERROR_MEMORY   6

/* Buffer sizes
*/
#define PARSE_MAXWORDSIZE 256    /* maximum length of a word, in characters */
#define PARSE_MAXLINESIZE 512    /* maximum length of a line, in characters */
#define PARSE_MAXBUFSIZE  65500  /* don't go quite up to 65536 */
#define PARSE_INITBUFSIZE 10500  /* initial buffer size */
#define PARSE_BUFGROWSIZE 5000   /* size by which to increase */


/**************************** structures
*/

/* State structure keeps track of the last context, style, etc.
    and is used to store data in the word
*/
typedef struct State {
  BYTE type;     /* type - WORD, LITERAL, HRULE, IMAGE             */
  BYTE style;    /* character style bits (for WORD or LITERAL)     */
  WORD contextflags;/* context bits - flags                        */
  WORD contextblock;/* context bits - block type                   */
  WORD hlevel;   /* header level (1 to 6, only in HEADING context) */
  WORD olitem;   /* item number (1 or more, only in OLIST context) */
  WORD ulnest;   /* nesting (1 or more, only in ULIST context)     */
  WORD ilink;    /* link index (if context is LINK or ANCHOR)      */
  WORD iimage;   /* image index (if IMAGE)                         */
  char arg1[PARSE_MAXWORDSIZE];  /* command argument               */
  char arg2[PARSE_MAXWORDSIZE];  /* second argument                */
} STATE;

/**************************** global data
*/
/*
** Array of parser error messages
*/
static char * parser_errormsg[] = {
  "No error",
  "File too big",
  "Line too long",
  "Too many words in file",
  "Bad <> pair - possibly unterminated <",
  "Bad or missing argument to <A> or <IMG>",
  "Unable to allocate enough memory"
};
/* these are shared by parser functions
*/
static FILE  *hFileIn;                    /* input file handle       */
static int   nWidth;                      /* screen width            */
static char  szTitle[PARSE_MAXWORDSIZE];  /* document title          */
static char  sCurWord[PARSE_MAXWORDSIZE]; /* current word            */
static int   nIChar;                      /* index into current word */
static char  szCurLine[PARSE_MAXLINESIZE];/* current line            */
static int   nILine;                      /* index into current line */
static int   nNLine;                      /* count of displayable chars in line */

static STATE stateCurState;               /* state                   */
static char  szCurLinkAnchor[2*PARSE_MAXWORDSIZE];/* current link or anchor name */

static char FAR *fpOutBuf;                /* output buffer           */
static DWORD uOutBufSize;                 /* output buffer size      */
static DWORD uNOutBuf;                    /* number of chars so far in buffer */

static jmp_buf jmpEnvironment;            /* environment for longjmp on error */

/* these are used to start lines in certain contexts */

static char *szFmtQuote = "      ";       /* block quote */
static char *szFmtUList = "    o ";       /* bullet list */
static char *szFmtOList = "%4d. ";        /* numbered list */

/* if set to 1, no escape codes are output, and all output goes to puts() */
static int bFilter;

/***
**** OutputLine
****  Adds output line to output buffer
****  This routine builds the output buffer by adding lines.  If the buffer
****    becomes full, it attempts to reallocate the buffer to a larger size.
****  After copying the line to the buffer, the line is cleared.
***/
void
OutputLine ()
{
  int i;
  DWORD newsize, newnoutbuf;
  char FAR *newptr;

  if (bFilter)
    puts(szCurLine);
  else {
    /* no output buffer yet, need to make one */
    if (fpOutBuf == NULL) {
      fpOutBuf = _fmalloc ((size_t) PARSE_INITBUFSIZE);
      if (fpOutBuf == NULL)
        longjmp (jmpEnvironment, PARSE_ERROR_MEMORY);
      uNOutBuf = 0;
      uOutBufSize = PARSE_INITBUFSIZE;
    }
    /* calculate new buffer size */
    newnoutbuf = uNOutBuf + nILine + 1; /* extra 1 for newline */

    /* if output has outgrown buffer, realloc */
    if (newnoutbuf + 1 > uOutBufSize) {
      if (uOutBufSize == PARSE_MAXBUFSIZE)      /* cannot grow past a certain size */
        longjmp (jmpEnvironment, PARSE_ERROR_TOOBIG);
      newsize = uOutBufSize + PARSE_BUFGROWSIZE;  /* calculate new size */
      if (newsize > PARSE_MAXBUFSIZE)             /* limit to maximum */
        newsize = PARSE_MAXBUFSIZE;
      newptr = _frealloc (fpOutBuf, (size_t) newsize);  /* reallocate */
      if (newptr == NULL)
        longjmp (jmpEnvironment, PARSE_ERROR_MEMORY);
      fpOutBuf = newptr;
      uOutBufSize = newsize;
      if (newnoutbuf + 1 > uOutBufSize)                 /* check again */
        longjmp (jmpEnvironment, PARSE_ERROR_TOOBIG);
    }

    for (i = 0; i < nILine; i++)           /* put line into buffer */
      fpOutBuf[uNOutBuf++] = szCurLine[i];
    fpOutBuf[uNOutBuf++] = EOLCODE;           /* add newline to buffer */
    fpOutBuf[uNOutBuf] = 0;
  }
  /* clear out line */
  szCurLine[0] = 0;
  nILine = nNLine = 0;
  return;
}

/***
**** OutputEscape
****  Add an escape sequence to the output line.
****  Arguments:
****    s is escape sequence
***/
void
OutputEscape(char *s)
{
  int ns;
  char escbuf[PARSE_MAXWORDSIZE];

  if (bFilter)
    return;

  sprintf (escbuf, "%c%s%c", ESCAPECODE, s, ESCAPECODE);

  ns = strlen(escbuf);
  if (nILine + ns + 1 < PARSE_MAXLINESIZE) {
    strcpy (&szCurLine[nILine], escbuf);
    nILine += ns;
  }
  else
    longjmp (jmpEnvironment, PARSE_ERROR_LONGLINE);
}

/***
**** OutputLiteral
****  Add real characters to the output line.
****  Arguments:
****    s is string to add to output line
***/
void
OutputLiteral(char *s, int ns)
{
  int errorcode;

  if (ns < 0)
    ns = strlen(s);
  if (nILine + ns + 1 < PARSE_MAXLINESIZE) {
    /* do not check against screen width here */
    strcpy (&szCurLine[nILine], s);
    nILine += ns;
    nNLine += ns;
  }
  else
    longjmp (jmpEnvironment, PARSE_ERROR_LONGLINE);
}

/***
**** LogicalOutputStartLine
****  Outputs whatever is necessary to get the current line started
****  Returns:
****    0 if okay, or error code
****  This routine writes out any leading indentation space, numbers for a numbered list,
****    a dash for a bullet item, and any escape codes for link or style in progress.
***/
void
LogicalOutputStartLine ()
{
  char buf[PARSE_MAXWORDSIZE];
  if (stateCurState.style != STYLE_NONE) {
    buf[0] = 'a' + (stateCurState.style - STYLE_NONE);
    buf[1] = 0;
    OutputEscape(buf);
  }
  if (stateCurState.contextflags & CONTEXT_LINK &&
      !(stateCurState.contextflags & CONTEXT_FIRSTLNK)) {
    OutputEscape(szCurLinkAnchor);
  }
}

/***
**** OutputRule
****  Writes out line of a single character
****  Arguments:
****    n is number of chars to write
****    c is the character
****  This routine ends the current line and writes a line of a single character,
****    it is used for horizontal rules and for underlining headers.
***/
void
OutputRule (int n, char c, char *escape)
{
  char buf[PARSE_MAXWORDSIZE];
  int errorcode = 0;
  int i;

  OutputLine();
  for (i = 0; i < n; i++)
    buf[i] = c;
  buf[n] = 0;

  if (escape != NULL && strlen(escape))
    OutputEscape(escape);


  OutputLiteral(buf, n);
}
/***
**** OutputEndHeader
****  Finishes header, possibly underlines
****  This routine is called from ReadCode() when an end-of-header
****    command is read.
***/
void
OutputEndHeader ()
{
  int ullen;

  ullen = nNLine;
  if (stateCurState.hlevel == 1) {
    OutputRule (ullen, '=', "1");
  }
  else if (stateCurState.hlevel == 2) {
    OutputRule (ullen, '-', "2");
  }
  /* nothing for others right now */
}
/***
**** OutputWord
****  Adds word to output line
****  Returns:
****    0 if okay, or error code
****  This routine builds an output line by adding the current word.
****    If the line is too full for the word, or word starts a new line,
****    calls OutputLine() to dump the current line to the output buffer.
****  After copying the word to the line, the line is cleared.
****  Special treatment for word if it is image, hrule, or last word of header.
***/
void
OutputWord ()
{
  int tmp;
  char prebuf[PARSE_MAXWORDSIZE], buf[PARSE_MAXWORDSIZE], spacebuf[10], buf2[40];
  int i, nspcbuf, nbuf;

  /* horizontal rule... end line, write out */
  if (stateCurState.type == TYPE_HRULE) {
    OutputRule(nWidth, '-', NULL);
  }
  /* image - end line, write out name */
  else if (stateCurState.type == TYPE_IMAGE) {
    OutputLine();
    OutputLine();
    /* insert escape to start a link or anchor */
    if ((stateCurState.contextflags & CONTEXT_LINK ||
         stateCurState.contextflags & CONTEXT_ANCHOR) &&
        stateCurState.contextflags & CONTEXT_FIRSTLNK) {
      OutputEscape(szCurLinkAnchor);
    }
    OutputLiteral ("(IMAGE \"", -1);
    OutputLiteral (stateCurState.arg1, -1);
    OutputLiteral ("\")", -1);
    OutputLine();
  }
  /* words and literals */
  else {
    /* forced start of new line - blank line if new paragraph */
    if (stateCurState.contextflags & CONTEXT_NEWLINE ||
        stateCurState.contextflags & CONTEXT_NEWPAR) {
      if (stateCurState.contextflags & CONTEXT_NEWPAR)
        OutputLine();
      OutputLine();
    }
    spacebuf[0] = buf[0] = 0;
    /* should word be preceded by spaces? */
    if (stateCurState.type == TYPE_WORD && nNLine != 0) {
      if (stateCurState.contextflags & CONTEXT_SENT) {
        strcpy (spacebuf, "  ");
      }
      else {
        strcpy (spacebuf, " ");
      }
    }
    /* add word and measure */
    strcpy (buf, sCurWord);
    nbuf = strlen (buf);
    /* does it fit on line? */
    if (nNLine + strlen(spacebuf) + nbuf > nWidth && nNLine != 0) {
      /* no, end line first */
      OutputLine();
      /* kill spaces */
      spacebuf[0] = 0;
    }
    if (nILine == 0) {
      /* deal with differences between context blocks */
      prebuf[0] = 0;
      if (stateCurState.contextblock == CONTEXT_QUOTE) {  /* block quotes indented */
        strcat (prebuf, szFmtQuote);
      }
      else if (stateCurState.contextblock == CONTEXT_ULIST) { /* bullet items */
        for (i = stateCurState.ulnest; i > 1; i--)
          strcat (prebuf, szFmtQuote);
        if (stateCurState.contextflags & CONTEXT_FIRST)
          strcat (prebuf, szFmtUList);
        else
          strcat (prebuf, szFmtQuote);
      }
      else if (stateCurState.contextblock == CONTEXT_OLIST) {  /* numbered items */
        if (stateCurState.contextflags & CONTEXT_FIRST) {
          sprintf (buf2, szFmtOList, stateCurState.olitem);
          strcat (prebuf, buf2);
        }
        else
          strcat (prebuf, szFmtQuote);
      }
      /* need to do something about headings too */
      else if (stateCurState.contextblock == CONTEXT_HEADING) {
        if (stateCurState.contextflags & CONTEXT_FIRST) {
          sprintf (buf2, "%d", stateCurState.hlevel);
          OutputEscape(buf2);
        }
      }
      OutputLiteral(prebuf, -1);
      LogicalOutputStartLine();
    }
    OutputLiteral (spacebuf, -1);
    /* insert escape to start a link or anchor */
    if ((stateCurState.contextflags & CONTEXT_LINK ||
         stateCurState.contextflags & CONTEXT_ANCHOR) &&
        stateCurState.contextflags & CONTEXT_FIRSTLNK) {
      OutputEscape(szCurLinkAnchor);
    }
    /* send out the word */
    OutputLiteral(buf, nbuf);
  }

  return;
}

/*** ResetState
****  Reset the global state
***/
static
void
ResetState()
{
  stateCurState.type = TYPE_WORD;
  stateCurState.contextflags = CONTEXT_NONE;
  stateCurState.contextblock = CONTEXT_NONE;
  stateCurState.style = STYLE_NONE;
  stateCurState.hlevel = stateCurState.olitem = 0;
  stateCurState.ulnest = stateCurState.ilink = 0;
  stateCurState.iimage = 0;
}

/***
**** EndWord
****  Adds word to output
****  This routine finishes a word by calling OutputWord() to send it
****  out and then setting things up for the next word.
***/
static
void
EndWord ()
{
  int error;

  /* if called by consecutive white space chars, return */
  if (nIChar == 0 &&
      stateCurState.type != TYPE_HRULE &&
      stateCurState.type != TYPE_IMAGE)
    return;

  sCurWord[nIChar] = 0; /* end word */

  if (stateCurState.type == TYPE_TITLE) /* title is special */
    strcpy (szTitle, sCurWord);
  else    /* output the word... */
    OutputWord();
  sCurWord[0] = 0;
  nIChar = 0;

  /* reset things for next word */
  if (stateCurState.type == TYPE_HRULE ||
      stateCurState.type == TYPE_IMAGE) {
    stateCurState.type = TYPE_WORD;
    stateCurState.contextflags &= ~(CONTEXT_FIRST |
                            CONTEXT_FIRSTLNK |
                            CONTEXT_SENT  |
                            CONTEXT_ANCHOR);
    /* keep newline/newpar */
  }
  else {
    stateCurState.contextflags &= ~(CONTEXT_NEWLINE |
                            CONTEXT_NEWPAR |
                            CONTEXT_FIRST |
                            CONTEXT_FIRSTLNK |
                            CONTEXT_SENT  |
                            CONTEXT_ANCHOR);
    /* check for end of sentence... this could be done in more complex */
    /*  manner if necessary.  It has to be attached to next word. */
    if (stateCurState.type == TYPE_WORD) {
      if (sCurWord[nIChar-1] == '.' ||
          sCurWord[nIChar-1] == '?' ||
          sCurWord[nIChar-1] == '!')
        stateCurState.contextflags |= CONTEXT_SENT;
    }
  }
}

/***
**** ReadArg
****  Extract and argument from the passed buffer
****  Arguments:
****    arg points to the argument name
****    buf points to the buffer
****    type is 1 if we are looking for a single argument,
****      or 2 if a double argument (separated by # sign)
****  Returns:
****    0 if arg found, PARSE_ERROR_BADARG if not
****  Puts arguments it finds into stateCurState
****  Example:
****    ReadArg ("ADDRESS", "name=\"myname\" address=\"MyAdrs\"", 1)
****   puts "MyAdrs" into stateCurState.arg1.
****   Double arg (type = 2) used for hrefs ("file#label").
***/
int
ReadArg (char *arg, char *buf, int type)
{
  int error = PARSE_ERROR_BADARG;
  char *buf2;

  while (*buf) {
    for (; *buf && isspace(*buf); buf++){};  /* skip white space */
    /* is this the right argument? */
    if (strncmpi(arg, buf, strlen(arg)) == 0) {
      buf = strchr(buf, '\"');     /* find first quote */
      if (buf == NULL) break;
      buf2 = ++buf;              /* first char past quote */
      buf2 = strchr(buf2, '\"'); /* find second quote */
      if (buf2 == NULL) break;
      *buf2 = 0;
      if (type == 1) { /* single argument */
        strcpy (stateCurState.arg1, buf);
        stateCurState.arg2[0] = 0;
        error = 0;
      }
      else {  /* double argument, second preceded by # */
        buf2 = strchr (buf, '#');
        if (buf2 == NULL) {  /* second part of arg not there */
          strcpy (stateCurState.arg1, buf);
          stateCurState.arg2[0] = 0;
        }
        else {               /* both parts of arg exist */
          *buf2 = 0;
          strcpy (stateCurState.arg1, buf);
          strcpy (stateCurState.arg2, ++buf2);
        }
        error = 0;
      }
    }
    /* wrong argument, skip past it */
    else {
      buf = strchr(buf, '\"'); /* find first quote */
      if (buf == NULL) break;
      buf = strchr(++buf, '\"'); /* find second quote */
      if (buf == NULL) break;
      buf++;                     /* past second quote */
    }
  }
  return error; /* no longjmp as caller may recover */
}

/***
**** ReadEscape
****  Read a character escape and return its value
****  Returns:
****    character represented by escape sequence
****    '*' if unrecognized
****  The leading '&' is already read.
***/
char
ReadEscape ()
{
  char buf[PARSE_MAXWORDSIZE];   /* escape code buffer */
  int  ibuf = 0;                 /* index into escape code buffer */
  char retval = '*';
  char c;

  /* part one: get code out of file */
  while ((c = getc(hFileIn)) != EOF && c != 26) {  /* end on ctrl-Z also */
    if (ibuf >= PARSE_MAXWORDSIZE) {  /* check for word too long here */
      ibuf = 0;
      break; /* could just not break, and keep eating file... */
    }
    if (c == ';')           /* escape sequence end */
      break;
    else                    /* add to buffer */
      buf[ibuf++] = c;
  }
  buf[ibuf] = 0;


  /* part two: interpret the escape code */
  /* we only handle four... */
  if (stricmp(buf,"quot") == 0)
    retval = '\"';
  else if (stricmp(buf,"amp") == 0)
    retval = '&';
  else if (stricmp(buf,"lt") == 0)
    retval = '<';
  else if (stricmp(buf,"gt") == 0)
    retval = '>';

  return retval;
}


/***
**** TML code ID's, used to associate each code with a number
***/
enum MLCodeID {
  ML_NONE,
  ML_TITLE, ML_xTITLE,
  ML_H1, ML_xH1, ML_H2, ML_xH2, ML_H3, ML_xH3,
  ML_H4, ML_xH4, ML_H5, ML_xH5, ML_H6, ML_xH6,
  ML_P, ML_BR, ML_HR, ML_IMG,
  ML_UL, ML_xUL, ML_OL, ML_xOL, ML_LI,
  ML_PRE, ML_xPRE,
  ML_BLOCKQUOTE, ML_xBLOCKQUOTE,
  ML_DL, ML_DT, ML_DD, ML_xDL,
  ML_A, ML_xA,
  ML_DFN, ML_xDFN, ML_EM, ML_xEM, ML_CITE, ML_xCITE, ML_VAR, ML_xVAR,
  ML_I, ML_xI, ML_CODE, ML_xCODE, ML_KBD, ML_xKBD, ML_SAMP, ML_xSAMP,
  ML_TT, ML_xTT, ML_STRONG, ML_xSTRONG, ML_B, ML_xB
};

/***
**** association between code IDs and TML code strings
***/
static struct {
  char *mlcode;
  enum MLCodeID id;
} codelist[] = {
  { "TITLE",      ML_TITLE },
  { "/TITLE",     ML_xTITLE },
  { "H1",         ML_H1 },
  { "/H1",        ML_xH1 },
  { "H2",         ML_H2 },
  { "/H2",        ML_xH2 },
  { "H3",         ML_H3 },
  { "/H3",        ML_xH3 },
  { "H4",         ML_H4 },
  { "/H4",        ML_xH4 },
  { "H5",         ML_H5 },
  { "/H5",        ML_xH5 },
  { "H6",         ML_H6 },
  { "/H6",        ML_xH6 },
  { "P",          ML_P },
  { "BR",         ML_BR },
  { "HR",         ML_HR },
  { "IMG",        ML_IMG },
  { "UL",         ML_UL },
  { "/UL",        ML_xUL },
  { "OL",         ML_OL },
  { "/OL",        ML_xOL },
  { "LI",         ML_LI },
  { "PRE",        ML_PRE },
  { "/PRE",       ML_xPRE },
  { "BLOCKQUOTE",   ML_BLOCKQUOTE },
  { "/BLOCKQUOTE",  ML_xBLOCKQUOTE },
  { "DL",         ML_DL },
  { "DT",         ML_DT },
  { "DD",         ML_DD },
  { "/DL",        ML_xDL },
  { "A",          ML_A },
  { "/A",         ML_xA },
  { "DFN",        ML_DFN },
  { "/DFN",       ML_xDFN },
  { "EM",         ML_EM },
  { "/EM",        ML_xEM },
  { "CITE",       ML_CITE },
  { "/CITE",      ML_xCITE },
  { "VAR",        ML_VAR },
  { "/VAR",       ML_xVAR },
  { "I",          ML_I },
  { "/I",         ML_xI },
  { "CODE",       ML_CODE },
  { "/CODE",      ML_xCODE },
  { "KBD",        ML_KBD },
  { "/KBD",       ML_xKBD },
  { "SAMP",       ML_SAMP },
  { "/SAMP",      ML_xSAMP },
  { "TT",         ML_TT },
  { "/TT",        ML_xTT },
  { "STRONG",     ML_STRONG },
  { "/STRONG",    ML_xSTRONG },
  { "B",          ML_B },
  { "/B",         ML_xB },
  { NULL, ML_NONE }            /* note terminator */
};


/***
**** ReadCode
****  Reads and interprets a markup code
****  Note that the leading '<' has been read already.
***/
static
void
ReadCode ()
{
  char buf[PARSE_MAXWORDSIZE];   /* code buffer */
  int  ibuf = 0;                 /* index into code buffer */
  char c;                        /* input char */
  char *parg, *parg2;            /* argument pointers */
  enum MLCodeID code = ML_NONE;  /* enum code type */
  int i, error = 0;

  /* part one: get code out of file */
  while ((c = getc(hFileIn)) != EOF && c != 26) {  /* end on ctrl-Z also */
    if (ibuf >= PARSE_MAXWORDSIZE)  /* check for word too long here */
      longjmp (jmpEnvironment, PARSE_ERROR_BADCODE);
    if (c == '>')           /* markup command end */
      break;
    else                    /* just another character */
      buf[ibuf++] = c;
  }
  buf[ibuf] = 0;            /* terminate string */

  /* part two: divide into code and arg */
  for (parg = buf; *parg && !isspace(*parg); parg++){}; /* find null or space */
  parg2 = parg; /* save this point */
  for (; *parg && isspace(*parg); parg++){};  /* continue through white space */
  *parg2 = 0;   /* ends first part, which is code */
  /* at this point parg either points to null or to start of argument */


  /* part three: identify the code by trying to match them */
  /*  one-by-one */
  for (i = 0; codelist[i].mlcode != NULL; i++) {
    if (stricmp(buf, codelist[i].mlcode) == 0) {
      code = codelist[i].id;
      break;
    }
  }

  /* part four: deal with the code... giant switch
  **  This is where we interpret the code to mean something,
  **  by setting up the stateCurState with flags and other
  **  information
  */
  switch (code) {
    case ML_NONE: /* code not found, so do nothing... */
      break;
    case ML_TITLE:    /* Title requires special extraction */
      stateCurState.type = TYPE_TITLE;
      break;
    case ML_xTITLE:   /* end of title */
      ResetState();
      break;
    case ML_H1:       /* headers */
    case ML_H2:
    case ML_H3:
    case ML_H4:
    case ML_H5:
    case ML_H6:
      ResetState();
      stateCurState.contextflags |= CONTEXT_NEWPAR;
      stateCurState.contextflags |= CONTEXT_FIRST;
      stateCurState.contextblock = CONTEXT_HEADING;
      switch (code) {
        case ML_H1: stateCurState.hlevel = 1; break;
        case ML_H2: stateCurState.hlevel = 2; break;
        case ML_H3: stateCurState.hlevel = 3; break;
        case ML_H4: stateCurState.hlevel = 4; break;
        case ML_H5: stateCurState.hlevel = 5; break;
        case ML_H6: stateCurState.hlevel = 6; break;
      }
      break;
    case ML_xH1:      /* end of headers */
    case ML_xH2:
    case ML_xH3:
    case ML_xH4:
    case ML_xH5:
    case ML_xH6:
      OutputEndHeader();
      ResetState();
      stateCurState.contextflags |= CONTEXT_NEWLINE;
      break;
    case ML_P:        /* new paragraph */
      stateCurState.contextflags |= CONTEXT_NEWPAR;
      break;
    case ML_BR:       /* line break */
      stateCurState.contextflags |= CONTEXT_NEWLINE;
      break;
    case ML_HR:       /* horizontal rule */
      stateCurState.type = TYPE_HRULE;
      stateCurState.contextflags |= CONTEXT_NEWLINE;
      break;
    case ML_IMG:      /* image */
      stateCurState.type = TYPE_IMAGE;
      stateCurState.contextflags |= CONTEXT_NEWLINE;
      error = ReadArg ("SRC", parg, 1);
      break;
    case ML_OL:       /* ordered list */
      ResetState();
      stateCurState.contextflags |= CONTEXT_NEWPAR;
      stateCurState.contextflags |= CONTEXT_FIRST;
      stateCurState.contextblock = CONTEXT_OLIST;
      stateCurState.olitem = 0;
      break;
    case ML_LI:       /* item in ordered or unordered list */
      stateCurState.contextflags |= CONTEXT_NEWLINE;
      stateCurState.contextflags |= CONTEXT_FIRST;
      stateCurState.olitem++;
      break;
    case ML_PRE:      /* preformatted */
      ResetState();
      stateCurState.contextflags |= CONTEXT_NEWPAR;
      stateCurState.contextflags |= CONTEXT_FIRST;
      stateCurState.contextblock = CONTEXT_PREFORM;
      stateCurState.type = TYPE_LITERAL;
      stateCurState.style = STYLE_TT;
      break;
    case ML_BLOCKQUOTE: /* block quote */
      ResetState();
      stateCurState.contextflags |= CONTEXT_NEWPAR;
      stateCurState.contextflags |= CONTEXT_FIRST;
      stateCurState.contextblock = CONTEXT_QUOTE;
      break;
    case ML_xOL: /* same exit for any of these contexts */
    case ML_xPRE:
    case ML_xBLOCKQUOTE:
      ResetState();
      stateCurState.contextflags |= CONTEXT_NEWPAR;
      break;

    case ML_UL:     /* unordered lists may be nested */
      if (stateCurState.contextblock == CONTEXT_ULIST)
        stateCurState.ulnest++;
      else {
        ResetState();
        stateCurState.contextblock = CONTEXT_ULIST;
        stateCurState.contextflags |= CONTEXT_NEWPAR;
        stateCurState.ulnest = 1;
      }
      stateCurState.contextflags |= CONTEXT_FIRST;
      break;
    case ML_xUL:    /* end uf unordered list */
      if (stateCurState.ulnest > 1) {
        stateCurState.contextflags |= CONTEXT_NEWLINE;
        stateCurState.ulnest--;
      }
      else {
        ResetState();
        stateCurState.contextflags |= CONTEXT_NEWPAR;
      }
      break;

    case ML_DL:     /* definition list */
    case ML_xDL:
      ResetState();
      stateCurState.contextflags |= CONTEXT_NEWPAR;
      break;
    case ML_DT:     /* term to be defined */
      stateCurState.contextblock = CONTEXT_NONE;
      stateCurState.contextflags |= CONTEXT_NEWLINE;
      stateCurState.contextflags |= CONTEXT_FIRST;
      break;
    case ML_DD:     /* definition of term */
      stateCurState.contextflags |= CONTEXT_NEWLINE;
      stateCurState.contextflags |= CONTEXT_FIRST;
      stateCurState.contextblock = CONTEXT_QUOTE;
      break;

    case ML_A:      /* anchor/link */
      if ((error = ReadArg ("NAME", parg, 1)) == 0) {
        stateCurState.contextflags |= (CONTEXT_ANCHOR | CONTEXT_FIRSTLNK);
        sprintf (szCurLinkAnchor, "A%s", stateCurState.arg1);
      }
      else if ((error = ReadArg ("HREF", parg, 2)) == 0) {
        stateCurState.contextflags |= (CONTEXT_LINK | CONTEXT_FIRSTLNK);
        sprintf (szCurLinkAnchor, "H%s#%s", stateCurState.arg1, stateCurState.arg2);
      }
      break;
    case ML_xA:    /* end anchor/link */
      if (stateCurState.contextflags & CONTEXT_LINK)
        OutputEscape("X");
      stateCurState.contextflags &= ~(CONTEXT_ANCHOR|CONTEXT_LINK);
      break;

    /* character styles */
    case ML_DFN:
      stateCurState.style = STYLE_DFN;
      OutputEscape("b");
      break;
    case ML_EM:
      stateCurState.style = STYLE_EM;
      OutputEscape("c");
      break;
    case ML_CITE:
      stateCurState.style = STYLE_CITE;
      OutputEscape("d");
      break;
    case ML_VAR:
      stateCurState.style = STYLE_VAR;
      OutputEscape("e");
      break;
    case ML_I:
      stateCurState.style = STYLE_I;
      OutputEscape("f");
      break;
    case ML_CODE:
      stateCurState.style = STYLE_CODE;
      OutputEscape("g");
      break;
    case ML_KBD:
      stateCurState.style = STYLE_KBD;
      OutputEscape("h");
      break;
    case ML_SAMP:
      stateCurState.style = STYLE_SAMP;
      OutputEscape("i");
      break;
    case ML_TT:
      stateCurState.style = STYLE_TT;
      OutputEscape("j");
      break;
    case ML_STRONG:
      stateCurState.style = STYLE_STRONG;
      OutputEscape("k");
      break;
    case ML_B:
      stateCurState.style = STYLE_B;
      OutputEscape("l");
      break;
    case ML_xDFN:
    case ML_xEM:
    case ML_xCITE:
    case ML_xVAR:
    case ML_xI:
    case ML_xCODE:
    case ML_xKBD:
    case ML_xSAMP:
    case ML_xTT:
    case ML_xSTRONG:
    case ML_xB:
      stateCurState.style = STYLE_NONE;
      OutputEscape("a");
      break;
  }
  if (error)
    longjmp (jmpEnvironment, error);
}


/***
**** ErrorMessage
****  Returns error message text
****  Arguments:
****    i  index of error message
****  Returns:
****    pointer to character string for error
***/
static
char *
ErrorMessage(int i)
{
  return parser_errormsg [i];

}
/***
**** Parse
****  Parses input file
****  Arguments:
****    fin is a pointer to an open FILE
****    width is the display width
****    title is a pointer to a pointer where the pointer to
****      the title string is stored
****    outbufptr is a pointer to a pointer where the pointer
****      to the parsed data is stored
****    errormsg is a pointer to a pointer where the pointer to
****      the error message is stored
****    errorline is a pointer to an integer where the line at
****      which an error occurred is stored
****    filter is 1 if Parse() should act as a file filter (that is,
****      instead of storing data in buffer, write it to stdout)
****  Returns:
****    error code, or 0 if no error
****  This is called from main program to parse a file.
****    The file must be already open.  If there is no error on
****    input, the parsed data is in an allocated array, and
****    a pointer to the array is placed in outbufptr.  Caller must
****    open and close file, and deallocate data array.
***/
int
Parse (FILE *fin, int width, char **title, char FAR **outbufptr,
       char **errormsg, int *errorline, int filter)
{
  int error = 0;    /* error code        */
  int c;            /* current character */

  /* initialize globals */
  hFileIn = fin;
  nWidth = width;
  bFilter = filter;
  ResetState();
  szTitle[0] = 0;
  sCurWord[0] = 0;
  nIChar = 0;          /* no characters in first word */
  szCurLine[0] = 0;
  nILine = 0;          /* no characters in first line */
  nNLine = 0;
  fpOutBuf = NULL;     /* no output buffer yet - will allocate when needed */

  /* initialize arguments */
  *outbufptr = NULL;
  *errorline = 0;

  /* any error will longjmp to here */
  if ((error = setjmp(jmpEnvironment)) != 0) {
    if (fpOutBuf != NULL)
      _ffree (fpOutBuf);
    *errormsg = ErrorMessage(error);
    return error;
  }


  /* continue loop while there are characters */
  while ((c = getc(hFileIn)) != EOF && c != 26) {  /* end on ctrl-Z also */
    if (c == ESCAPECODE) /* skip escape code character */
      continue;
    if (nIChar >= PARSE_MAXWORDSIZE) {  /* check for word too long here */
      longjmp (jmpEnvironment, PARSE_ERROR_LONGLINE);
      /* break; */
    }
    if (c == '<') {                          /* markup command */
      EndWord ();  /* end previous word */
      ReadCode (); /* reads code and changes state */
      /* on return need to check for hrule or image... */
      /*  or for certain start/end commands...  */
      /*  these codes generate their own output */
      if (stateCurState.type == TYPE_HRULE ||
          stateCurState.type == TYPE_IMAGE) {
        EndWord ();
      }
    }
    /* characters between commands */
    else {
      if (c == '&')          /* text escape */
        c = ReadEscape ();   /* replace with real character */
      /* linefeed, carriage return, formfeed */
      if (c == 0x0d || c == 0x0a || c == '\f') {
        if (c == 0x0a) /* keep track of line number in case of error */
          ++*errorline;
        /* preformatted text: use to break line */
        if (stateCurState.contextblock == CONTEXT_PREFORM) {
          EndWord ();
          stateCurState.contextflags |= CONTEXT_NEWLINE;
          continue;
        }
        /* if not preformatted, just change to space */
        else
          c = ' ';
      }
      if (c == '\t') /* change tabs to spaces too */
        c = ' ';

      /* space */
      if (c == ' ') {
        /* preformatted and title keep spaces */
        if (stateCurState.contextblock == CONTEXT_PREFORM ||
            stateCurState.type == TYPE_TITLE) {
          sCurWord[nIChar++] = c;
        }
        /* otherwise, space breaks word */
        else
          EndWord ();
      }
      /* not a space; add character to word */
      else
        sCurWord[nIChar++] = c;
    }
  }

  /* end last word */
  EndWord();
  /* output last line */
  OutputLine();

  /* return information for caller */
  *outbufptr = fpOutBuf;
  *errormsg = "";
  pszTitle = szTitle;
  return 0;
}

