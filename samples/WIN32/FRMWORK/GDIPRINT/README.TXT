GDIPrint - Based on GDIDIB

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Demonstrates printing of DIBs.

USES:
    Bitmap creation, editing, storage, retrieval and printing.

COMMENTS:
    This is a 32-bit only sample.

    ********** Metafile functionality to be included in future release **********

    Note that the dialog for selecting pen and brush colors will allow
    the selection of colors not contained in the color table of the
    DIBSection bitmap when the bitmap is in 1bpp or 4bpp format and the
    display device supports more colors than are in the color table.
    Objects painted with the pen or brush will initially appear in the
    selected color, but when the image is refreshed from the bitmap in the
    memory DC, the objects will assume colors in the bitmap's color table
    that are mapped from the original colors using the nearest color
    algorithm. These are the colors that are actually stored in the bitmap.

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    GDIPrint- Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.
    ToolBar - Creates the toolbar and processes ToolTips notifications.
    StatBar - Creates and manages the status bar.
    Client  - Implements the window procedure for a child window
                that displays either an initialized bitmap or one loaded
                from disk file, and performs all drawing on the bitmap.
    PenDlg  - Dialog box for choosing the pen style used for drawing.
    BrushDlg- Dialog box for choosing the brush style used for
                filling objects.
    ColorDlg- Dialog box for choosing a color from the system palette
                when running GDIPrint on a palette-based device.
    PalCtrl - Implements a custom control for displaying and selecting
                colors from the current system palette.
    Palette - Implements palette manager-related routines such as
                handlers for palette messages.
    InfoDlg - Dialog box for displaying some basic color and palette
                information regarding the display device.
    DIBUtil - Contains various routines for manipulating DIBs.
    FileDlg - Implements dialogs for File Save, Save As and Close.
    FileIO  - Implements code to read and write DIB disk files.
    FileNew - Implements code to create a new DIB.
    Print   - Implements code to print a DIB.

