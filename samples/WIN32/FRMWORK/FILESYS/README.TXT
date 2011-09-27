FILESYS - Based on Dialog

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Show a basic implementation of some of the new FILE SYSTEM API's

USES:
    Based on DIALOG

COMMENTS:


MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
              a specific module.
    Filesys - App specific module. 
    Demo    - Displays the dialog and fills it with all the necessary
            - information. 
