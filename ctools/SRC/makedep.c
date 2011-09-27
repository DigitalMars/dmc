/*_ makedep.c   Sun Dec  4 1988   Modified by: Walter Bright */
/* Written by Walter Bright				*/
/* Makefile dependency list generator			*/

#include	<stdio.h>
#include	<ctype.h>
#include	<string.h>
#include	<dos.h>

#if __SC__ || MSC
#include	<stdlib.h>
#else
extern char *getenv();
#endif

#include	"toolkit.h"
#include	"mem.h"
#include	"list.h"
#include	"filespec.h"
#include	"file.h"

#ifdef _WINDOWS
char winc_programname[] = "MAKEDEPW.EXE";
char winc_programtitle[] = "MAKEDEP for Windows";
#endif

#if M_UNIX || M_XENIX
#define OBJ_FILE "o"
#define ASM_FILE ".asm"
#define NOTDIRCHAR '\\'
#define DIRCHAR '/'
#define DIRSTR "/"
#else
#define OBJ_FILE "OBJ"
#define ASM_FILE ".ASM"
#define NOTDIRCHAR '/'
#define DIRCHAR '\\'
#define DIRSTR "\\"
#endif

extern int __cdecl __wildcard;
int *__wild = &__wildcard;

int debug = 0;		/* debugging flag			*/
int systemincludes = 1;	/* do system includes			*/
list_t pathlist = NULL;		/* list of include paths	*/
list_t excludelist = NULL;      /* list of directories to exclude */
int objpaths = 1;       /* do add paths to object files         */
int listmissingfiles=1;
int linelen = 0;		/* current length of output line */

char *buffer = NULL;		/* line buffer				*/
int bufmax = 0;			/* # of characters buffer can hold	*/
int changes;			/* # of changes wrought			*/

/*****************************
 * One of these for each file.
 */

typedef struct
{	char	*filename;	/* name of file				*/
	list_t	include;	/* list of file_t's included by this file */
	char	searched;	/* 1 if already searched		*/
	char	listed;		/* 1 if already listed		*/
	char    missing;	/* 1 if file doesn't exist      */
} file_t;

void slashtobackslash(char *filename);
int isnotexcluded(char *filename);

#define list_file(fl)	((file_t *) list_ptr(fl))

list_t filelist = NULL;		/* list of files			*/
list_t targetlist = NULL;	/* list of targets			*/

file_t *addfile(char *filename);
file_t *dofiledefault(char *filename);
file_t *dofilepath(char *filename);
int searchfile(file_t *pfile);

/*****************************
 */

main(argc,argv)
int argc;
char *argv[];
{   char *p;
    int i;
    file_t *ptarget;

    if (argc == 1)
    {	usage();
	exit(EXIT_FAILURE);
    }
    mem_init();
    list_init();
    for (i = 1; i < argc; i++)
    {	p = argv[i];
	if (*p == '-')
	    switch (p[1])
	    {
		case 'd':
		case 'D':
		    debug = 1;
		    break;
		case 's':
		case 'S':
		    systemincludes = 0;
		    break;
		case 'i':
		case 'I':
		    list_append(&pathlist,&p[2]);
		    break;
		case 'm':
		case 'M':
		    listmissingfiles = 0;
		    break;    
		case 't':
		case 'T':
		    objpaths = 0;
		    break;
		case 'x':
		case 'X':
		    list_append(&excludelist,&p[2]);    
		    break;
		default:
		    usage();
		    exit(EXIT_FAILURE);
	    }
	else
	{
	    ptarget = addfile(p);
	    list_append(&targetlist,ptarget);
	}
    } /* for each argument */

    list_append(&pathlist,getenv("INCLUDE"));  /* add environment search path */
    search();				/* search files for includes	*/
    output();				/* output dependency lists	*/

#ifdef TERMCODE
    list_free(&pathlist,FPNULL);
    list_free(&excludelist,FPNULL);
    list_term();
    mem_free(buffer);
    mem_term();
#endif
    return EXIT_SUCCESS;
}

usage()
{
    fputs("\
Generate makefile dependency lists for C and ASM source files\n\
V6.0, Written by Walter Bright\n\
Use:\n\
	makedep [-d] {-ipath} {-xexclude} [-t] [-s] filename...\n\
	-d	Print debugging output\n\
	-i	Specify search path for includes\n\
	-x	Specify directory to exclude from search\n\
	-s	Ignore system includes\n\
	-t	Do not include path names for targets\n\
",stdout);
}

/**********************
 * Print error and exit.
 */

faterr(char *s)
{
    fputs("Error: ",stdout);
    fputs(s,stdout);
    exit(EXIT_FAILURE);
}

/*********************************
 * Add filename to list of files, if it's not already there.
 */

file_t *addfile(char *filename)
{   file_t *pfile;
    list_t fl;

    for (fl = filelist; fl; fl = list_next(fl))
    {
	if (strcmpl(filename,list_file(fl)->filename) == 0)
	    return list_file(fl);
    }

    pfile = mem_calloc(sizeof(*pfile));
    pfile->filename = mem_strdup(filename);
    list_append(&filelist,pfile);
    return pfile;
}

/*****************************
 * Search for file down path, starting with current directory.
 */

file_t *dofiledefault(char *filename)
{
    if (file_exists(filename))
	return addfile(filename);
    else
	return dofilepath(filename);
}

/*****************************
 * Search for file down path.
 */

file_t *dofilepath(char *filename)
{   char *name;
    list_t pl;
    file_t *pfile;

    for (pl = pathlist; pl; pl = list_next(pl))
    {
	name = file_searchpath((char *) list_ptr(pl),filename);
	if (name)			/* if we found it		*/
	{   pfile = addfile(name);
	    mem_free(name);
	    return pfile;
	}
    }
    return addfile(filename);
}

/******************************
 * Search files for include files.
 */

search()
{   list_t fl;

    do
    {
	changes = 0;
	for (fl = filelist; fl; fl = list_next(fl))
	{
	    if (!list_file(fl)->searched)
	    {	
	        if (!searchfile(list_file(fl)))
	           list_file(fl)->missing = 1;
		list_file(fl)->searched = 1;
		changes++;
	    }
	}
    } while (changes);
}

/******************************
 * Process a file.
 */

int searchfile(file_t *pfile)
{   FILE *fin;
    int asmfile;		/* 1 if .ASM source file	*/

    asmfile = !strcmpl(filespecdotext(pfile->filename),ASM_FILE);
    if (debug)
    {	fputs("Processing file '",stderr);
	fputs(pfile->filename,stderr);
	fputs("'\n",stderr);
    }
    /* Open input file	*/
    fin = fopen(pfile->filename,"r");
    if (!fin)
    {	if (debug)
	{   fputs("Can't open file '",stderr);
	    fputs(pfile->filename,stderr);
	    fputs("'\n",stderr);
	}
	return 0;
    }

    /* Do the file	*/
    while (readline(fin))
	if (asmfile)
	    asmline(pfile,buffer);
	else
	    cline(pfile,buffer);

    fclose(fin);
    return 1;
}

/*****************************
 * Process line of .ASM file.
 * Look for lines of the form
 *	include filename
 */

asmline(file_t *pfile,char *line)
{   char *p;

    while (isspace(*line))
	line++;
    if (*line == 'i' || *line == 'I')
    {	p = line;
	do
	    p++;
	while (isalpha(*p));
	if (isspace(*p))
	{   *p = 0;
	    if (!strcmpl(line,"include"))
	    {	line = p;
		do
		    line++;
		while (isspace(*line));
		if (*line)
		{   p = line;
		    do
			p++;
		    while (*p && !isspace(*p));
		    *p = 0;
		    slashtobackslash(line);
		    if (isnotexcluded(line))
		        list_append(&pfile->include,dofiledefault(line));
		}
	    }
	}
    }
}

/*****************************
 * Process line of C file.
 * Look for lines of the form
 *	#include <filename>
 *	#include "filename"
 */

cline(file_t *pfile,char *line)
{   char *p;
    char tc;

    /*printf("Doing line|%s",line);*/
    while (isspace(*line))
	line++;
    if (*line == '#')
    {	p = line;
	do
	    p++;
	while (isalpha(*p));
	if (isspace(*p))
	{   *p = 0;
	    if (!strcmp(line,"#include"))
	    {	line = p;
		do
		    line++;
		while (isspace(*line));
		tc = *line++;
		if (tc == '<')
		{   if (!systemincludes)
			return;
		    tc = '>';
		}
		if (tc == '>' || tc == '"')
		{   file_t *pf;

		    p = line;
		    do
			p++;
		    while (*p && *p != tc);
		    *p = 0;
		    slashtobackslash(line);
		    if (tc == '>')
			pf = dofilepath(line);
		    else
			pf = dofiledefault(line);
		    if (isnotexcluded(line))
		        list_append(&pfile->include,pf);
		}
	    }
	}
    }
}

/*******************************
 * Convert all slashes to backslashes
 */

void slashtobackslash(char *filename)
{
   register char *fc = filename;
   
   while ((fc = strchr (fc, NOTDIRCHAR)) != NULL)
   {
      *fc = DIRCHAR;
   }
}

/*******************************
 * Check to see if filename is in an excluded directory
 */

int isnotexcluded(char *filename)
{
   list_t xl;
   char *xp;
   char path[258];
   int pl;
   int fp;
   
   for (xl = excludelist; xl; xl = list_next(xl))
   {
      xp = (char *)list_ptr(xl);
      
      if (*xp == DIRCHAR)
         strncpy (path,xp+1,sizeof(path)-2);
      else
         strncpy (path,xp,sizeof(path)-2);   
      path[sizeof(path)-2] = '\0';
         
      if (path[strlen(path)-1] != DIRCHAR)
         strcat (path, DIRSTR);
         
      pl = strlen(path);         
         
      for (fp = 0; fp<strlen(filename); fp=strcspn(filename+fp,DIRSTR)+fp+1)
      {
         if (memicmp (path,filename+fp,pl) == 0) return 0;
      }
   }
   return 1;
}

/*****************************
 * Read a line from the source file into a buffer.
 * End of line is a 0, \n or EOF.
 * Return 1 if not end of file yet.
 */

int readline(FILE *fin)
{   int i,c;

    c = fgetc(fin);
    if (c == EOF)
	return 0;		/* end of input file			*/
    i = 0;
    do
    {
	if (i + 1 >= bufmax)	/* the +1 leaves room for trailing 0	*/
	{   buffer = mem_realloc(buffer,bufmax + 50);
	    bufmax += 50;
	}
	buffer[i++] = c;
	buffer[i] = 0;
	if (c == 0 || c == '\n')
	    break;
	c = fgetc(fin);
    } while (c != EOF);
    return 1;
}

/*******************************
 * Output dependency lists.
 */

output()
{   list_t tl;
    list_t fl;

    for (tl = targetlist; tl; tl = list_next(tl))
    {	char *targetname;

	targetname = filespecforceext(list_file(tl)->filename, OBJ_FILE);
#if !(M_UNIX || M_XENIX)
	strupr (targetname);
#endif
	if (objpaths == 0 && strlen(targetname) > 0)
	{  char *c;
	   
	   for (c=targetname+strlen(targetname)-1;
	        c>=targetname;
	        c--)
	      if (*c == ':' || *c == '\\' || *c == '/') break;
	   c++;
	   fputs(c,stdout);        
	   linelen = strlen(c);
	}
	else
	   {
	   fputs(targetname,stdout);
	   linelen = strlen(targetname);
    }
	fputs(" :",stdout);
	linelen += 2;
	mem_free(targetname);

	for (fl = filelist; fl; fl = list_next(fl))
	    list_file(fl)->listed = 0;
	outputfile(list_file(tl));
	putchar('\n');
	linelen = 0;
    }
}

outputfile(pfile)
file_t *pfile;
{   list_t fl;

    if (!pfile->listed)
    {	
    int flen = strlen(pfile->filename);
    
#if !(M_UNIX || M_XENIX)
    strupr(pfile->filename);
#endif
	if (linelen + flen > 77)
		{
		fputs(" \\\n",stdout);
		linelen = 0;
		}
	linelen += flen + 2;
	putchar(' ');
	fputs(pfile->filename,stdout);
	pfile->listed = 1;
	for (fl = pfile->include; fl; fl = list_next(fl))
	    if (listmissingfiles || !list_file(fl)->missing)
	        outputfile(list_file(fl));
    }
}

