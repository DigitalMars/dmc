GDIPAL - Based on GDIInput

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Demonstrates the use of the Windows palette manager on palette
    devices. Allows the user to choose pen and brush colors from
    the system palette.

USES:
    Based on GDIInput

COMMENTS:
    This is a 32-bit only sample.

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    GDIPal  - Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.
    ToolBar - Creates the toolbar and processes ToolTips notifications.
    StatBar - Creates and manages the status bar.
    Client  - Implements the window procedure for a child window
                that covers the entire client area of the main window except
                for the tool bar and status bar.  Performs all drawing.
    PenDlg  - Dialog box for choosing the pen style used for drawing.
    BrushDlg- Dialog box for choosing the brush style used for
                filling objects.
    ColorDlg- Dialog box for choosing a color from the system palette
                when running GDIOut on a palette-based device.
    PalCtrl - Implements a custom control for displaying and selecting
                colors from the current system palette.
    Palette - Implements palette manager-related routines such as
                handlers for palette messages.
    InfoDlg - Dialog box for displaying some basic color and palette
                information regarding the display device.
