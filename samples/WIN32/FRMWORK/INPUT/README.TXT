INPUT - Based directly on simple

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.

PURPOSE:
    Show the use of simple windows input via mouse, keyboard, and timer.


USES:
    Based on simple.
    Also uses some APIs first shown in output

COMMENTS:
    Defines a simple message dispatch method in WndProc and msgtable.c

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Input   - Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
