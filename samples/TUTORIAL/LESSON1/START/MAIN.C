/** main.c
**
**   Part of TMLDOS, Symantec C++ 7.0 demonstration program
**   Copyright (C) 1994 Symantec Corporation
**
**   last mod:
**     rjw, 11/94 (release)
**
** Contents of this file:
**   main entry for DOS TML reader
** Notes:
**   Usage: TMLDOS [/f] filename
**     If the /f flag is used, the program acts as a filter on the input file,
**       writing all output to stdout.  May also be "-f".
**     Otherwise, the interactive reader is invoked (with or without a starting
**       file)
**   Also responds to "TMLDOS ?" (or "/?" or "-?").
**   filename may be "-" to read from stdin if filter flag also used.
**   See parse.c for parser (called through Parse()),
**    and display.c for reader (called through Display()).
**/
#include <stdio.h>
#include <stdlib.h>
#include <disp.h>
#include <dir.h>

#include "TMLDOS.h"

/**************************** globals
*/
char *pszTitle;         /* pointer to document title */
char szFilename[128];   /* file name, path stripped  */
char FAR *fpOutBufPtr;  /* pointer to output buffer  */

/*** ChangeToFileDir
****   Change working directory to a file's directory, and set global filename
****   Arguments:
****     pathname is file path and name
****   Changing directory is done because the hyperlinks in TML files usually
****     contain path names relative to the TML file.  Thus it is easiest
****     to change to the file's directory so that other files are easily
****     opened.  We could also have saved the file's drive/directory and
****     prepended it to any file we must open, but this is slightly more work.
****   The file name is saved for the reader status line.
***/
void
ChangeToFileDir(char *pathname)
{
  char drive[MAXDRIVE];
  char dir[MAXDIR];
  char name[MAXFILE];
  char ext[MAXEXT];
  int  idrive, strlendir;

  /* split pathname into components */
  fnsplit (pathname, drive, dir, name, ext);

  /* save filename and extension */
  strcpy (szFilename, name);
  strcat (szFilename, ext);

  /* attempt to change drive to the destination drive */
  if (drive[0] >= 'a' && drive[0] <= 'z')
    _chdrive(drive[0]-'a'+1);
  else if (drive[0] >= 'A' && drive[0] <= 'Z')
    _chdrive(drive[0]-'A'+1);

  /* now change directory */
  strlendir = strlen(dir);
  if (strlendir && dir[strlendir-1] == '\\')
    dir[strlendir-1] = 0;
  chdir(dir);
}

/*** Usage
****   Display usage
***/
void
Usage()
{
  fprintf (stderr, "usage: TMLDOS [/f] filename\n");
}

/*** main
****   Program entry point
****   Arguments:
****     argc is number of command line arguments
****     argv[] is argument array
****   See top of file for description of usage.
****   This subroutine saves the current directory, parses the command line,
****     calls the parser, calls the reader if not acting as a filter, and
****     restores the original directory.
***/
int
main (int argc, char *argv[])
{
  FILE *fin;
  char *errormsg;       /* pointer to error message           */
  int  errorline;       /* line on which parse error occurred */
  int  errorcode;       /* parser error code                  */
  int  filter = 0;      /* flag for whether we should act as a filter */
  int  iargv = 1;
  int  retval = 20;     /* program return value */
  char currentdir[128]; /* buffer for restoring current directory */

  /* save current directory for later restoration */
  getcwd (currentdir, 126);
  strcat (currentdir, "\\"); /* so we can use it with ChangeToFileDir() */

  disp_open();  /* open display package */

  if (argc > iargv) {
    /* respond to request for help */
    if (!strcmp(argv[iargv], "/?") ||
        !strcmp(argv[iargv], "-?") || !strcmp(argv[iargv], "?")) {
      Usage();
      return 0; /* exit immediately */
    }
    /* set flag to use program as filter */
    if (!strcmp(argv[iargv], "/f") || !strcmp(argv[iargv], "-f")) {
      iargv++;
      filter = 1;
    }
  }
  if (argc > iargv) {
    /* open file */
    if (strcmp(argv[iargv], "-") == 0 && filter) /* get from stdin */
      fin = stdin;
    else
      fin = fopen(argv[iargv], "r");
    /* check for success */
    if (fin != NULL) {
      /* change to file's directory */
      if (fin != stdin)
        ChangeToFileDir(argv[iargv]);
      /* parse */
      errorcode = Parse (fin, disp_numcols, &pszTitle, &fpOutBufPtr, &errormsg,
                         &errorline, filter);
      /* close file, then either show error or run reader */
      if (fin != stdin)
        fclose (fin);
      if (errorcode) {
        fprintf (stderr, "TMLDOS: %s, file %s line %d\n",
                 errormsg, argv[iargv], errorline);
      }
      else {
        retval = 0;
        if (!filter)
          Display();
      }
    }
    else
      fprintf (stderr, "TMLDOS: could not open file %s\n", argv[iargv]);
  }
  /* no starting file */
  else {
    retval = 0;
    Display();
  }

   /* free allocated data buffer */
   if (fpOutBufPtr)
      _ffree ((void *) fpOutBufPtr);

  disp_close();        /* close display package */

  /* restore working directory */
  ChangeToFileDir (currentdir);

  return retval;
}
