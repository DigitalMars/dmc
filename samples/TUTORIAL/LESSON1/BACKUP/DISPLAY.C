/** display.c
**
**   Part of TMLDOS, Symantec C++ 7.0 demonstration program
**   Copyright (C) 1994 Symantec Corporation
**
**   last mod:
**     rjw, 11/94 (release)
**
** Contents of this file:
**   Display routines for DOS TML reader
** Notes:
**   Routines use global pointer to output buffer.
**   The reader handles the following commands:
**     ESC - quits
**     Up, down - move cursor, scroll if at top or bottom
**     Left, right - move cursor
**     Home, end   - move cursor to start, end of line
**     Ctrl-home, Ctrl-end - scroll to top or bottom of document
**     F1 - prompt for file name, open file
**     Enter - hyperlink (if cursor positioned over link)
**/
#include <stdio.h>
#include <stdlib.h>
#include <disp.h>
#include <bios.h>

#include "TMLDOS.h"

/* keyboard scan codes
*/
#define  SC_F1         0x3b
#define  SC_F2         0x3c
#define  SC_F3         0x3d
#define  SC_F4         0x3e
#define  SC_F5         0x3f
#define  SC_F6         0x40
#define  SC_F7         0x41
#define  SC_F8         0x42
#define  SC_F9         0x43
#define  SC_F10        0x44
#define  SC_UP         0x48
#define  SC_DOWN       0x50
#define  SC_LEFT       0x4b
#define  SC_RIGHT      0x4d
#define  SC_PAGEUP     0x49
#define  SC_PAGEDN     0x51
#define  SC_HOME       0x47
#define  SC_END        0x4f
#define  SC_CHOME      0x77
#define  SC_CEND       0x75
#define  SC_DELETE     0x53


/* globals
*/

long lCurBufPos;    /* position in buffer of first line on screen */

/* status/help line default */
char *pszStatusLine = "%-*s F1:Open File  Enter:Hyperlink  Esc:Exit";
int  bStatusLineShown = 0;

/*** ClearScreen
****   Clears screen
***/
static
void
ClearScreen()
{
  disp_move(0,0);
  disp_eeop();
}

/*** ShowStatusLine
****   Shows status/help line at bottom of screen
***/
static
void
ShowStatusLine()
{
  char tbuf[200];
  int twid;
  int oldrow = disp_cursorrow;  /* save old cursor position */
  int oldcol = disp_cursorcol;

  /* width of title/filename field */
  /* 4 is width of "%-*s", -1 is to prevent writing in last column */
  twid = disp_numcols - strlen(pszStatusLine) + 4 - 1;

  /* get file title or name */
  if (pszTitle != NULL && pszTitle[0])
    strncpy (tbuf, pszTitle, twid);
  else
    strncpy (tbuf, szFilename, twid);
  tbuf[twid] = 0;


  disp_move (disp_numrows-1, 0);  /* move to bottom, write status line */
  disp_eeol();
  disp_setattr(DISP_REVERSEVIDEO);
  disp_printf(pszStatusLine, twid, tbuf);
  disp_setattr(DISP_NORMAL);

  disp_move (oldrow, oldcol);   /* replace cursor */
  disp_flush();
  bStatusLineShown = 1;
}

/*** ShowError
****   Writes error message at bottom of screen
***/
static
void
ShowError(char *s)
{
  int oldrow = disp_cursorrow;  /* save old cursor position */
  int oldcol = disp_cursorcol;

  disp_move (disp_numrows-1, 0);  /* move to bottom, write error line */
  disp_eeol();
  disp_setattr(DISP_REVERSEVIDEO);
  disp_puts(s);
  disp_setattr(DISP_NORMAL);
  disp_flush();

  disp_move (oldrow, oldcol);   /* replace cursor */
  disp_flush();
  bStatusLineShown = 0;
}

/*** DoEscape
****   Deal with escape sequence
****   Arguments:
****     bufpos is starting position in buffer
****   Returns
****     first buffer position beyond escape sequence
****   Interprets the first char as the escape sequence type:
****     1-6  start heading 1 to 6
****     a-l  start style a-l  ( a = normal )
****     A    start anchor
****     H    start hyperlink
****     X    end hyperlink
****   This routine sets video attributes.  Some of the mapping is arbitrary.
****   The italic-type codes are mapped to underline, but this only works on
****     some displays - on others it comes out, for example, dark blue.
****   See parse.c for routines that create escape codes.
***/
static
long
DoEscape(long bufpos)
{
  char c;

  c = fpOutBufPtr[bufpos++];
  switch (c) {
    case 'a':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'X':
      disp_setattr(DISP_NORMAL);
      break;
    case '1':
    case '2':
    case '3':
    case 'k':
    case 'l':
      disp_setattr(DISP_NORMAL|DISP_INTENSITY);
      break;
    case '4':
    case '5':
    case '6':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
/*      disp_setattr(DISP_UNDERLINE);*/
      disp_setattr(DISP_NORMAL|DISP_INTENSITY);
      break;
    case 'H':
      disp_setattr(DISP_REVERSEVIDEO);
      break;
  }

  /* read past rest of escape sequence */
  while ((c = fpOutBufPtr[bufpos++]) != ESCAPECODE) {};

  return bufpos;
}

/*** ShowLine
****   Shows line of text
****   Arguments:
****     bufpos is starting position in buffer
****     ypos is screen y position of line
****   Returns
****     first buffer position beyond line
****   This routine writes a line starting at the passed buffer position.
****     It calls DoEscape() when it encounters an escape code.
***/
static
long
ShowLine(long bufpos, int ypos)
{
  char c;

  disp_setattr(DISP_NORMAL);
  disp_move (ypos, 0);
  disp_eeol();

  if (fpOutBufPtr[bufpos] != 0) {
    while ((c = fpOutBufPtr[bufpos++]) != EOLCODE) {
      if (c == ESCAPECODE)
        bufpos = DoEscape(bufpos);
      else
        disp_putc(c);
    }
  }
  disp_flush();

  return bufpos;
}

/*** ShowScreen
****   Shows entire screen starting from current location in buffer
****   The ShowLine() routine is smart enough to stop
****     at the end of the buffer.
***/
static
void
ShowScreen()
{
  int linessofar = 0;
  long nextlinepos = lCurBufPos;
  int oldrow = disp_cursorrow;
  int oldcol = disp_cursorcol;

  /* display each line in sequence */
  while (linessofar < disp_numrows-1)
    nextlinepos = ShowLine(nextlinepos, linessofar++);

  ShowStatusLine();
  disp_move (oldrow, oldcol);
}

/*** FindRelativeLine
****   Locates start of a line relative to the current buffer position
****   Arguments:
****     dline is delta # lines
****   Returns:
****     buffer index of requested line
****   Note that if the buffer ends before the line is reached, the
****     index returned may not point to a line which is actually dline
****     lines away.
****   This routine is used mostly by the movement routines, but also to
****     find the buffer location of the start of the line over which
****     a hyperlink command was given.
***/
static
long
FindRelativeLine (int dline)
{
  long ibuf = lCurBufPos;
  int neols;
  char c;

  if (dline > 0) {
    neols = dline;
    /* step forward, counting EOLs */
    while ((c = fpOutBufPtr[ibuf]) != 0 && neols) {
      if (c == EOLCODE)
        neols--;
      ibuf++;
    }
  }
  else if (dline < 0) {
    neols = 1 - dline;
    if (ibuf > 0)  /* start with previous character */
      --ibuf;
    /* step backward, counting EOLs */
    while (ibuf > 0 && neols) {
      c = fpOutBufPtr[ibuf];
      if (c == EOLCODE)
        neols--;
      ibuf--;
    }
    /* if we found all the EOL's step back past last one */
    if (!neols)
      ibuf += 2;
  }

  return ibuf;
}

/*** Move functions
****   These functions take care of movement key input
****   They may alter lCurBufPos.
***/
static
void
MoveUp()
{
  if (disp_cursorrow > 0)
    disp_move(disp_cursorrow-1, disp_cursorcol);
  else {
    lCurBufPos = FindRelativeLine (-1);
    /* reshow from new position */
    ShowScreen();
  }
  disp_flush();
}
static
void
MoveDown()
{
  if (disp_cursorrow < disp_numrows-2)
    disp_move(disp_cursorrow+1, disp_cursorcol);
  else {
    lCurBufPos = FindRelativeLine (1);
    /* if at bottom, undo */
    if (fpOutBufPtr[lCurBufPos] == 0)
      lCurBufPos = FindRelativeLine (-1);
    /* reshow from new position */
    ShowScreen();
  }
  disp_flush();
}
static
void
MovePageUp()
{
  if (lCurBufPos > 0) {
    lCurBufPos = FindRelativeLine (1-disp_numrows);
    /* reshow from new position */
    ShowScreen();
  }
  else
    disp_move(0,0);
  disp_flush();
}
static
void
MovePageDn()
{
  lCurBufPos = FindRelativeLine (disp_numrows-1);
  /* if at bottom, undo */
  if (fpOutBufPtr[lCurBufPos] == 0)
    lCurBufPos = FindRelativeLine (-1);
  /* reshow from new position */
  ShowScreen();
  disp_flush();
}
static
void
MoveLeft()
{
  if (disp_cursorcol > 0)
    disp_move(disp_cursorrow, disp_cursorcol-1);
  disp_flush();
}
static
void
MoveRight()
{
  if (disp_cursorcol < disp_numcols-1)
    disp_move(disp_cursorrow, disp_cursorcol+1);
  disp_flush();
}
static
void
MoveHome()
{
  disp_move(disp_cursorrow, 0);
  disp_flush();
}
static
void
MoveEnd()
{
  disp_move(disp_cursorrow, disp_numcols-1);
  disp_flush();
}
static
void
MoveCtrlEnd()
{
  while (fpOutBufPtr[lCurBufPos] != 0)
    lCurBufPos++;
  /* points to end now */
  lCurBufPos = FindRelativeLine (-1);
  /* reshow from new position */
  ShowScreen();
  disp_move(0,0);
  disp_flush();
}
static
void
MoveCtrlHome()
{
  lCurBufPos = 0;
  ShowScreen();
  disp_move(0,0);  /* move cursor to upper left */
  disp_flush();
}
/*** DoOpenFile
****   Opens a new file
****   Arguments:
****     filename is file's name
****   Returns:
****     pointer to error message, NULL if successful
****   This routine opens a file and parses it.
***/
static
char *
DoOpenFile(char *filename)
{
  FILE *fin;
  int errorcode, errorline;
  char *errormsg = NULL;

  /* attempt to open file */
  if ((fin = fopen(filename, "r")) != NULL) {
    ChangeToFileDir(filename);
    /* free old buffer */
    if (fpOutBufPtr) {
      _ffree ((void *) fpOutBufPtr);
      fpOutBufPtr = NULL;
    }
    lCurBufPos = 0;
    /* parse */
    errorcode = Parse (fin, disp_numcols, &pszTitle, &fpOutBufPtr, &errormsg,
                       &errorline, 0);
    if (!errorcode)
      errormsg = NULL;
    /* close file, reset cursor position */
    fclose (fin);
    /* to avoid problems, if there is no data buffer, allocate one */
    if (fpOutBufPtr == NULL) {
      fpOutBufPtr = _fmalloc ((size_t) 4);
      if (fpOutBufPtr == NULL) {
        puts("Can't allocate any memory!\n");  /* if we can't allocate 4 bytes... */
        exit(20);
      }
      fpOutBufPtr[0] = 0;
    }
  }
  else
    errormsg = "Unable to open file";

  return errormsg;
}
/*** DoJumpAnchor
****   Reset file position to named anchor
****   Arguments:
****     anchorname is the anchor name
****   Returns:
****     pointer to error message, NULL if successful
****   This subroutine searches through the entire buffer looking for
****     the named anchor.  If found, the current position is changed
****     to the start of the line containing that anchor.
***/
static
char *
DoJumpAnchor(char *anchorname)
{
  char *errormsg = "Unable to locate anchor";
  char buf2[128], c;
  long llinestart, ibuf, jbuf;
  int ibuf2;

  for (ibuf = 0, llinestart = 0; fpOutBufPtr[ibuf] != 0; ibuf++) {
    c = fpOutBufPtr[ibuf];
    if (c == EOLCODE)            /* end of line */
      llinestart = ibuf + 1;
    else if (c == ESCAPECODE) {  /* escape code, look for anchor */
      c = fpOutBufPtr[++ibuf];
      if (c == 'A') {               /* start anchor */
        /* extract anchor */
        for (jbuf = ibuf + 1, ibuf2 = 0, buf2[0] = 0; ;++jbuf) {
          c = fpOutBufPtr[jbuf];
          if (c == ESCAPECODE)
            break;
          else {
            buf2[ibuf2++] = c;
            buf2[ibuf2] = 0;
          }
        }
        /* test against one we want */
        if (!strcmp(anchorname, buf2)) {
          errormsg = NULL;
          break;    /* found, break out of for loop */
        }
      }
      /* not anchor, or wrong anchor */
      while (fpOutBufPtr[++ibuf] != ESCAPECODE) {}  /* walk to end of escape code */
    }
  }

  /* if anchor found, set new buffer position */
  if (errormsg == NULL)
    lCurBufPos = llinestart;

  return errormsg;
}
/*** Hyperlink
****   Jump to hyperlink under cursor
****   This subroutine first finds the line and character under the
****     cursor, keeping track of whether that character is inside
****     a hyperlink.  If so, it extracts the filename and anchorname,
****     and calls other subroutines to open the file and jump to the anchor.
***/
static
void
Hyperlink()
{
  char *errormsg = NULL;
  int oldrow = disp_cursorrow;
  int oldcol = disp_cursorcol;
  char buf[128], buf2[128], c;
  long lhitline, lhitchar, lhyperchar;
  int nchars, ibuf;

  lhitline = FindRelativeLine (disp_cursorrow);  /* this is the start of the line */

  /* find the hyperlink escape previous to the character
  **  over which the cursor is positioned
  */
  for (lhitchar = lhitline, lhyperchar = -1, nchars = 0;; lhitchar++) {
    c = fpOutBufPtr[lhitchar];
    if (c == 0 || c == EOLCODE) /* look for end of line */
      break;
    if (c != ESCAPECODE) { /* ordinary character */
      nchars++;
      if (nchars-1 == disp_cursorcol) /* look for char under cursor */
        break;
    }
    else { /* escape code */
      c = fpOutBufPtr[++lhitchar];
      if (c == 'H')               /* start hyperlink */
        lhyperchar = lhitchar+1;
      else if (c == 'X')          /* end hyperlink */
        lhyperchar = -1;
      while (fpOutBufPtr[++lhitchar] != ESCAPECODE) {}  /* walk to end of escape code */
    }
  }

  /* if hyperlink found, perform the necessary work */
  if (lhyperchar >= 0) {
    /* extract the filename */
    for (ibuf = 0, buf[0] = 0; ;++lhyperchar) {
      c = fpOutBufPtr[lhyperchar];
      if (c == '#' || c == ESCAPECODE)
        break;
      else {
        buf[ibuf++] = c;
        buf[ibuf] = 0;
      }
    }
    /* extract the anchor name */
    for (ibuf = 0, buf2[0] = 0; ; ++lhyperchar) {
      c = fpOutBufPtr[lhyperchar];
      if (c == '#')
        continue;
      else if (c == ESCAPECODE)
        break;
      else {
        buf2[ibuf++] = c;
        buf2[ibuf] = 0;
      }
    }
    /* if there is a filename, open the file */
    if (buf[0]) {
      errormsg = DoOpenFile(buf);
      if (errormsg == NULL)
        oldrow = oldcol = 0;
    }
    /* if there is an anchorname, jump to that line */
    if (errormsg == NULL) {
      if (buf2[0]) {
        errormsg = DoJumpAnchor(buf2);
        if (errormsg == NULL)
          oldrow = oldcol = 0;
      }
    }
  }

  /* update display */
  ShowScreen();
  ShowStatusLine();
  if (errormsg != NULL)
    ShowError(errormsg);
  disp_move (oldrow, oldcol);
  disp_flush();
  /* DO NOT INSERT CODE FOR LESSON 1 HERE! */
  /* Did you forget to choose Function from the Goto menu, and select Display? */
}
/*** NewFile
****   Prompts for a new file name, goes to start of that file.
***/
static
void
NewFile()
{
  char *errormsg = NULL;
  int oldrow = disp_cursorrow;
  int oldcol = disp_cursorcol;
  char buf[120];

  /* get a file name */
  disp_move(disp_numrows-1,0);
  disp_setattr(DISP_NORMAL|DISP_INTENSITY);
  disp_puts("File:");
  disp_eeol();
  disp_flush();
  gets(buf);
  ClearScreen();

  if (buf[0]) {
    errormsg = DoOpenFile(buf);
    if (errormsg == NULL)
        oldrow = oldcol = 0;
  }

  /* update display */
  ShowScreen();
  ShowStatusLine();
  if (errormsg != NULL)
    ShowError(errormsg);
  disp_move (oldrow, oldcol);
  disp_flush();
}
/*** Display
****   Display parsed file interactively
****   Notes:
****     disp_open() must have been called already.
****     Uses global pointer to output buffer.
****   The user can open and hyperlink to other files, so
****     the global pointer to the output buffer may be changed.
***/
void
Display()
{
  int done = 0;
  int kbkey;

  /* to avoid problems, if there is no data buffer, allocate one */
  if (fpOutBufPtr == NULL) {
    fpOutBufPtr = _fmalloc ((size_t) 4);
    if (fpOutBufPtr == NULL) {
      puts ("Can't allocate any memory!\n");
      return;  /* if we can't allocate 4 bytes... */
    }
    fpOutBufPtr[0] = 0;
  }

  MoveCtrlHome();

  /* main loop - waits for keypress, restores status line if necessary,
  **   interprets keypress and calls appropriate subroutine
  */

  while (!done) {
    kbkey = bioskey(0);
    if (!bStatusLineShown)
      ShowStatusLine();
    if ((kbkey & 0xff) == 0x1b)        /* quit on Escape */
      done = 1;
    else if ((kbkey & 0xff) == 0x0d)   /* hyperlink on Enter */
      Hyperlink();
    else {
      switch ((kbkey >> 8) & 0xff) {
        case SC_F1:      /* new file */
          NewFile();
          break;
        case SC_PAGEUP:  /* movement keys */
          MovePageUp();
          break;
        case SC_PAGEDN:
          MovePageDn();
          break;
        case SC_UP:
          MoveUp();
          break;
        case SC_DOWN:
          MoveDown();
          break;
        case SC_LEFT:
          MoveLeft();
          break;
        case SC_RIGHT:
          MoveRight();
          break;
        case SC_HOME:
          MoveHome();
          break;
        case SC_END:
          MoveEnd();
          break;
        case SC_CHOME:
          MoveCtrlHome();
          break;
        case SC_CEND:
          MoveCtrlEnd();
          break;
      }
    }
  }
  /* move to bottom of screen */
  /* INSERT CODE FOR LESSON 1 HERE! */
}
