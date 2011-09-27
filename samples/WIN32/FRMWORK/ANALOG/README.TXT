ANALOG - Based on generic

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    This sample application demonstrates 4 of the Common controls, Status 
    Bar, Progress Bar, Tracker(slider) control and Up/Down(spin) control 
    in a copy machine panel theme.  

    To see the copier dialog select "Dialog" from the main menu then select
    "Copier".  The Up/Down control is used to select the number of copies to
    be made.  The Track bars are used for enlargement and shading.  The status
    bar, notifies the user of the selection made, the status of the copier 
    and the progress made(by using the new Progress Bar control) when copying
    is started.

USES:
    Based on Generic

COMMENTS:

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.
    Analog  - Implements the windows procedure for the main application window
    Copier  - Creates and displays Copier dialog box which in turn demonstrates 
                some of the new Windows 95 common controls.  The common controls
                it demonstrates are Up/Down control, Track Bar control, 
                Progress Bar and Status Bar controls.
