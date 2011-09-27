// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: writepad.inl
//
//  PURPOSE: Inline Functions
//
//  FUNCTIONS:
//    GetFName      - Get the current file name.
//
//  COMMENTS:
//

//
//  FUNCTION: GetFName(VOID)
//
//  PURPOSE: Get the current file name.
//
//  PARAMETERS:
//    NONE
//
//  RETURN VALUE:
//    The full path name of the current file.  The current filename is
//    that of the active MDI child window, and the name is in the caption
//
//  COMMENTS:
//
//

extern char szFName[256];

__inline char *GetFName(VOID)
{
    GetWindowText(GetActiveMDIChild(), szFName, sizeof(szFName));
    return szFName;
}




