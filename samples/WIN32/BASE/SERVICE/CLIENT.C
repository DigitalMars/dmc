// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   client.c
//
//  PURPOSE:  This program is a command line oriented
//            demonstration of the Simple service sample.
//
//  FUNCTIONS:
//    main(int argc, char **argv);
//
//  COMMENTS:
//
//
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

VOID main(int argc, char *argv[])
{
    char    inbuf[80];
    char    outbuf[80];
    DWORD   bytesRead;
    BOOL    ret;
    LPSTR   lpszPipeName = "\\\\.\\pipe\\simple";
    LPSTR   lpszString = "World";
    int     ndx;

    // allow user to define pipe name
    for ( ndx = 1; ndx < argc-1; ndx++ )
    {
        if ( (*argv[ndx] == '-') || (*argv[ndx] == '/') )
        {
            if ( stricmp( "pipe", argv[ndx]+1 ) == 0 )
            {
                lpszPipeName = argv[++ndx];
            }
            else if ( stricmp( "string", argv[ndx]+1 ) == 0 )
            {
                lpszString = argv[++ndx];
            }
            else
            {
                printf("usage: client [-pipe <pipename>] [-string <string>]\n");
                exit(1);
            }
        }
        else
        {
            printf("usage: client [-pipe <pipename>] [-string <string>]\n");
            exit(1);
        }

    }

    strcpy( outbuf, lpszString );

    ret = CallNamedPipeA(lpszPipeName,
                         outbuf, sizeof(outbuf),
                         inbuf, sizeof(inbuf),
                         &bytesRead, NMPWAIT_WAIT_FOREVER);

    if (!ret) {
        printf("client: CallNamedPipe failed, GetLastError = %d\n", GetLastError());
        exit(1);
    }

    printf("client: received: %s\n", inbuf);
}
