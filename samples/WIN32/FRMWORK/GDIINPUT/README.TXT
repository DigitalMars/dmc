GDIINPUT - Based on BarSDI

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Shows a way to use the mouse for dynamic input for drawing on the
    screen.  Does "rubber-banding" of lines, rectangles, ellipses, and
    bezier curvers.

USES:
    Based on BarSDI

COMMENTS:
    This is a 32-bit only sample.

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    GDIInput- Implements the windows procedure for the main application window
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
