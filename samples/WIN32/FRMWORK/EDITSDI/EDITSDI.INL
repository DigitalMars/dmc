// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: editsdi.inl
//
//  PURPOSE: Inline functions
//
//  FUNCTIONS:
//    GetFName      - Get the current file name.
//    GetEditWindow - Return a handle to the current edit control.
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
//    The full path name of the current file.
//
//  COMMENTS:
//
//

extern char szFName[];
extern HWND hwndEdit;

__inline char *GetFName(VOID)
{
    return szFName;
}

//
//  FUNCTION: GetEditWindow(VOID)
//
//  PURPOSE: Return a handle to the current edit control.
//
//  PARAMETERS:
//    NONE
//
//  RETURN VALUE:
//    A handle to the current edit control.
//
//  COMMENTS:
//    This functionality is implemented as an inline function rather than
//    exporting hwndEdit.  Making it an inline function obviates the
//    efficiency arguments for using an exported variable.
//

__inline HWND GetEditWindow(VOID)
{
    return hwndEdit;
}



