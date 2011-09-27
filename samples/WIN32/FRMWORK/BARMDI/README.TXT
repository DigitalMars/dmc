BARMDI - Based on BlandMDI

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Show a basic MDI application with a tool bar and status bar.

USES:
    Based on BlandMDI

COMMENTS:

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    BarMDI  - Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.
    MDIChild- Creates the MDI client window and the MDI Children.  Also it 
                contains the window procedure for the MDI children
    ToolBar - Creates the toolbar and processes ToolTips notifications.
    StatBar - Creates and manages the status bar.
