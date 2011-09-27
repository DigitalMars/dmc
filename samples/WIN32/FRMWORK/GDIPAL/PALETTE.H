// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for the pen style dialog.
//

// palette defines
#define PALVERSION 0x300    // palette version

// function prototypes
LRESULT  ProcessPaletteChanged (HWND, WPARAM);
LRESULT  ProcessQueryNewPalette(HWND);
BOOL     IsPaletteDevice(void);
int      ColorsInPalette(HPALETTE);
HPALETTE GetSystemPalette(void);
