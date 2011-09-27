/*********************************************************
 *
 *              which.c
 *
 *              Celso C. Barriga
 *              1992-1993
 *
 *  This program is similar to the UNIX 'which' command.
 *  It searches for a file or files from the current dir,
 *  then in the PATH environment variable. Ideal for
 *  searching which version of the program will be run
 *  when you type its filename if multiple versions of
 *  the programs exist in different directories.
 *
 *  If a file is given with an extension, the file is
 *  searched in the current directory first. If nothing
 *  is found, it then searches through the PATH.
 *
 *  If no extension is found, the following extensions
 *  tried in the following order: .com, .exe, .bat
 *  Again, it searches from the current directory first
 *  before the PATH.    
 *
 *
 ********************************************************/

#include "filespec.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>

void doit(int, char *[]);

void main (int argc, char *argv[])
{
	printf("WHICH 1.01 (c)1992-1993 by Celso C. Barriga\n");

	// check for arguments
	if (argc < 2) {
		printf("Usage: which [files]\n");
		exit(1);
	}

	if ( ((argv[1][0] == '/') || (argv[1][0] == '-')) &&
		  ((argv[1][1] == '?') || (argv[1][1] == 'H') || (argv[1][1] == 'h')) )
	{
		printf("Usage: which [filespec]\n");
		exit(1);
	}

	doit(argc, argv);

   exit(0);
}                              

void doit(int nKeys, char *SearchKey[])
{
	int i;
	char *pathlist;
	char *res, *temp;

	if ( (pathlist = getenv("PATH")) == NULL )
	{
		printf("No PATH in the environment.\n");
		exit(1);
	}

	for (i=1; i<nKeys; i++)                 // for each file to be search
	{
		// does it have an ext?
		res = filespecdotext(SearchKey[i]);
		if ( strcmp(res, "") != 0 )
		{
			res =   file_searchpath(NULL, SearchKey[i]);    // try in current dir
			if ( res == NULL )
				res = file_searchpath(pathlist, SearchKey[i]);          // try the pathlist
			goto done;
		}
		
		// try .com in current dir
		temp = filespecforceext (SearchKey[i], "com");
		res = file_searchpath(NULL, temp);
				
		if ( res == NULL )                      // try .exe in current dir
		{
			temp = filespecforceext (SearchKey[i], "exe");
			res = file_searchpath(NULL, temp);
		}

		if ( res == NULL )                      // try .bat in current dir
		{
			temp = filespecforceext (SearchKey[i], "bat");
			res = file_searchpath(NULL, temp);
		}

		if ( res == NULL )                      // try .com in path
		{
			temp = filespecforceext (SearchKey[i], "com");
			res = file_searchpath(pathlist, temp);
		}
		
		if ( res == NULL )                      // try .exe in path
		{
			temp = filespecforceext (SearchKey[i], "exe");
			res = file_searchpath(pathlist, temp);
		}

		if ( res == NULL )                      // try .bat in path
		{
			temp = filespecforceext (SearchKey[i], "bat");
			res = file_searchpath(pathlist, temp);
		}

done:
		if ( res )
			printf ("%s\n", res);
		else
			printf ("%s not found.\n", SearchKey[i]);
	}
}
