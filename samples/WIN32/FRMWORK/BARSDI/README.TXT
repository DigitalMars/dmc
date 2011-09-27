BARSDI - Based on generic

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Show a generic windows aplication with a toolbar and status bar.

USES:
    Based on Generic.

COMMENTS:     This sample demonstrates the use of  statusbar and toolbar
              controls in their simplest forms. These controls are attached
              to the main window (typically) of an application to give users
              a visual indication of menu items and other relevant pieces 
              of information.

              This is a 32-bit only sample.


MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    BarSDI  - Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.
    ToolBar - Creates the toolbar and processes ToolTips notifications.
    StatBar - Creates and manages the status bar.
