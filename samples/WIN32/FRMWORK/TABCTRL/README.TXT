TABCTRL - Based on dialog

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Show a  windows aplication with a menu and a demo dialog box with
    a tab control in it. 
   

USES:
    Based on Dialog

COMMENTS:


MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    TabCtrl - Implements the window procedure for the main application window
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.
    Demo    - Demos the tab control by bringing up the demo dialog box.
    Page    - Implements the dialog procedure for the "Page" dialogs
                that correspond to the tabs in the tab control.
