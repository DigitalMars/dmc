
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

// CONSOLE.C
// ================================================================
// This module contains the standard console functions for this
// applicaiton. All of the 'graphical' functions are contained in
// GUI.C

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "ConGUI.h"

void usage (void);
static void DoArgs(int *pargc, char **pargv[]);
void ListArgs (int argc, char **argv);

extern int GetDialogArgs (char ***pargv);
extern int DoHelp (char *szHelpTopic);

BOOL bDoHelp = FALSE;
BOOL bDoDialog = FALSE;
BOOL bDoUsage = FALSE;

int main(int argc, char **argv)

{
    char szAppName[80];
    char **dargv;

    HANDLE hConsoleOutput;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    BOOL bLaunched;

    // Lets try a trick to determine if we were 'launched' as a seperate
    // screen, or just running from the command line.
    // We want to do this so that when we exit, we can prompt the user
    // before we shut down if we were 'launched'. Otherwise, any data on
    // the output window will be lost.
    // We will do this by simply getting the current cursor position. It
    // 'should' always be (0,0) on a launch, and something else if we were
    // executed as a command from a console window. The only time I can see
    // this as not working, is if the user executed a CLS and appended
    // our program with the '&' character, as in:
    //   C:\> CLS & ConGUI
    // This will also result in a (0,0) cursor pos, but in this case, the
    // user might also be wanting us to 'prompt' before closeing.
    // We also need to handle the case of:
    //   C:\> ConGUI > output.dat

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);
    bLaunched = ((csbi.dwCursorPosition.X==0) && (csbi.dwCursorPosition.Y==0));
    if ((csbi.dwSize.X<=0) || (csbi.dwSize.Y <= 0)) bLaunched = FALSE;

//    printf ("Window Size: (%i, %i)\n",csbi.dwSize.X, csbi.dwSize.Y);
//    printf ("Cursor Pos : (%i, %i)\n",csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);



    if (argc <= 1) { // Nothing on the command line, give brief usage info
        usage();
        if (bLaunched) {
            printf ("------------------------------------------\n");
            printf ("Program Finished - Hit <Enter> to continue");
            getchar();
        }
        return(0);
    }

    // Get the application name off of the command line.
    strcpy (szAppName, argv[0]); // this should always be there.
    argc--;
    argv++;

    // Get the rest of the arguments
    DoArgs (&argc, &argv);


    // And handle anything they told us to do
    if (bDoUsage) {         // Simple Usage Info
        usage();
    } else if (bDoHelp) {   // Robust Help
        DoHelp ("Contents");
    } else if (bDoDialog) { // Dialog
        dargv = (char **)GlobalAlloc (GPTR, 10*sizeof(char*));
        argc = GetDialogArgs(&dargv);
        if (argc > 0) {
            ListArgs (argc, dargv);
            DoArgs (&argc, &dargv);
        } else {
            printf ("\nUser Selected 'Cancel' in the dialog\n");
        }
    }

    if (bLaunched) {
        printf ("------------------------------------------\n");
        printf ("Program Finished - Hit <Enter> to continue");
        getchar();
    }

    return(0);
}


void usage(void)
{
    printf ("\nConGUI Usage information:\n");
    printf ("-------------------------\n\n");
    printf ("    -?       To print usage information\n");
    printf ("    -help    To bring up WinHelp for this application\n");
    printf ("    -dialog  To bring up a dialog to prompt for parameters\n");

}

void ListArgs (int argc, char **argv)
{
    printf ("\nCommand Line:\n    ConGUI ");
    while (argc--) {
        printf ("%s ", *argv);
        argv++;
    }
    printf ("\n");
}


typedef struct tagToken {
    int id;
    char str[25];
} Token;

Token params[] = {
    { T_USAGE,   "?"},
    { T_HELP,    "Help"},
    { T_DIALOG,  "Dialog"},
    { 0, 0}

};


// Check the option entered on the command line against the tokens
// we are expecting. The capital letters of the token represent the
// minimum amount of the token that needs to be provided. The lower
// case letters 'can' be provided, and if provided, must match.
BOOL TokensMatch (char *token, char *mask)
{
    int lToken, lMask, index;
    char *pchToken, *pchMask;

    lToken = strlen (token);
    lMask = strlen (mask);

    if (lToken > lMask) return FALSE;

    pchToken = &token[0];
    pchMask = &mask[0];

    index = 0;
    while (*pchToken) {
        if (toupper(*pchToken) != toupper(*pchMask)) return FALSE;

        pchToken++;
        pchMask++;
        index++;
    }

    if (index == lMask) return TRUE;

    if (islower(*pchMask)) return TRUE;

    return FALSE;

}

// Return the id of the specified token. Return -1 if no match.
int GetTokenID (char *token)
{
    int id = 0;
    int index = 0;

    while (params[index].id) {
        if (TokensMatch (token, params[index].str)) {
            return params[index].id;
        }
        ++index;
    }

    return -1;
}


// Parse the argument list, looking for valid keywords
static void DoArgs(int *pargc, char **pargv[])
{
    int argc, id;
    char **argv;
    char *p;

    argc = *pargc;
    argv = *pargv;

    p = argv[0];

    while (argc) {

        if (*p == '/' || (*p=='-' && *(p+1)!='\0')) {
            id = GetTokenID (&(p[1]));
            switch (id) {
                case -1 :
                    //printf ("Invalid Token: %s\n", &(p[1]));
                    break;

                case T_USAGE:
                    bDoUsage = TRUE;
                    break;

                case T_HELP:
                    bDoHelp = TRUE;
                    break;

                case T_DIALOG:
                    bDoDialog = TRUE;
                    break;

                default:
                    printf ("Found Token: %s\n", &(p[1]));
                    break;
            }
        } else {
            printf ("Unexpected keyword : %s\n", p);

        }

        argc--;
        argv++;
        p = argv[0];
    }

    *pargv = argv;
    *pargc = argc;
}
