EDITSDI - Based on dialog

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Show a generic windows aplication with basic editor functionality.
    This sample is similar to "Notepad" in that it uses an edit control
    as the input control for storing the text.

USES:
    Based on Dialog

COMMENTS:

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Editsdi - Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.
    Filedlg - Shows basic use of "Open" and "Save As" common dialogs.
    Finddlg - To show the use of the find and replace common dialogs.
    Optdlg  - To show the use of the fonts and colors common dialogs.
    Printdlg- To show the use of the "Print" and "Print Setup" common dialog
              boxes.
    File    - File manipulation functions.
    Print   - Application specific printing commands
    Search  - Find and replace dialog support
