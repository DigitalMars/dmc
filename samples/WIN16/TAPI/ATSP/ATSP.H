//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// PURPOSE.
//
// Copyright 1993-1995 Microsoft Corporation, all rights reserved.
#include <windows.h>
#include "TSPI.H"
#include "atsprc.h"

#define ATSP_VERSION 0x00010003
#define HELP_FILE               "atsp.hlp"


// messages to the wndproc of our companion app

#define WM_EXEMSG                               (WM_USER + 97)
#define msgMakeCall                     1
#define msgDial                         2
#define msgDrop                         3


#define SMALLBUFFER 40

typedef struct lineInfo
{
        // We have only a single call on a single address on a single line
        // This structure is constructed in TSPI_Initialize

        BOOL                                    bLineOpen;
        DWORD                                   lineID;                                         // the tapi ID of our line
        int                                     hcd;                                                    // the associated COM device
        DWORD                                   dwLineMediaModes;
        DWORD                                   dwLineStates;
        DWORD                                   dwAddressStates;
        DWORD                                   dwppID;                                         // Our permanent provider ID
        HTAPILINE                       htLine;                                         // TAPI opaque line handle
        LINEEVENT                       lpfnEventProc;                          // TAPI event callback function
        ASYNC_COMPLETION        lpfnCompletion;                 // TAPI Completion callback

        DWORD                                   callState;                                      // The state of this call
        HTAPICALL                       htCall;                                         // TAPI opaque call handle
        DWORD                                   dwRequestID;                            // for async requests
        LINEDIALPARAMS          dpDialParams;
        DWORD                                   dwMediaMode;
        DWORD                                   dwAppSpecific;
        BOOL                                    bInUse;
        BOOL                                    partial;
        char                                    DestAddress[TAPIMAXDESTADDRESSSIZE];

        int                                     ignore;                                         // 0
        int                                     pulse;                                          // 0
        char                                    port[SMALLBUFFER];              // "COM1"
        char                                    speed[SMALLBUFFER];             // "19200"
        char                                    initstr[SMALLBUFFER];   // "ATX0=1"
        char                                    linename[SMALLBUFFER];  // "My Phone"
        char                                    lineaddr[SMALLBUFFER];  // "555-1212"

} ATSPLineData;


// Debug Message Macro

#ifdef DEBUG
#include <stdarg.h>
#include <stdio.h>
void CDECL SPTrace(LPCSTR pszFormat, ...);
#define DebugMsg(_x_)  SPTrace _x_
#else
#define DebugMsg(_x_)
#endif
