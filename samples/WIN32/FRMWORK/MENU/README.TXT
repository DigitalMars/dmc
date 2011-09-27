MENU - Based on input

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Show several ways of using and manipulating menus, including
    handling menu commands from menu bars & pop menus, inserting
    menus on the fly, modifying menus, and implementing owner draw.

    Please Note: This sample is demonstrating the new Menu APIs but as
        direct translation of the 3.1 Menu APIs.  Therefore, this sample 
        does not use these new functions efficiently.  However, we are 
        working on a more efficient version.

USES:
    Based on input.
    Also uses some APIs first shown in output


COMMENTS:
    This is a 32-bit only application

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Menu    - Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
    Menu    - Basic menu support - command handling, item checking,
                bitmap check-marks, and on the fly menu insertion.
    OwnrDraw- Owner draw menu handling & on the fly menu modification
    Popup   - Implements a popup menu
