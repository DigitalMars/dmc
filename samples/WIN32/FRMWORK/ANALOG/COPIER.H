// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for all globally scoped names in this module.
//

//-------------------------------------------------------------------------
// Dialog ID definitions.  Copier dialog definition.

#define DLG_COPIER      10

//-------------------------------------------------------------------------
// Command ID definitions.  These definitions are used to associate dialog 
// controls with commands.

#define IDD_NCOPYTXT     200
#define IDD_NCOPIES      201
#define IDD_UDCOPIES     202

#define IDD_RESET        220

#define IDD_STOP         230

#define IDD_ENLARGE      240
#define IDD_TBENLARGE    241
                           
#define IDD_DARK         250
#define IDD_TBDARK       251

#define IDD_STATUSBAR    260

#define IDD_PGBAR        270

LRESULT CALLBACK CopierProc(HWND, UINT, WPARAM, LPARAM);

