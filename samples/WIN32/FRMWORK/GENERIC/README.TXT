GENERIC - Based on menu

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Show a generic windows application with a menu and an about box,
    but no real functionality.

USES:
    Based on Menu

COMMENTS:

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Generic - Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.
