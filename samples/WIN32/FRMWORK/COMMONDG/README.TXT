COMMDLG - Based on dialog

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Show basic uses of each of the common dialogs.

USES:
    Based on dialog

COMMENTS:

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Commondg- Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.
    commdlg - Implement stub and paint functions for the commdlg sample.
    filedlg - Shows basic use of "Open" and "Save As" common dialogs.
    finddlg - To show the use of the find and replace common dialogs.
    optdlg  - To show the use of the fonts and colors common dialogs.
    printdlg- To show the use of the "Print" and "Print Setup" common dialog
              boxes.
