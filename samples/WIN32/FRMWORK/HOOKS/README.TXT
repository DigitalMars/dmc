HOOKS - Based on GENERIC, DLLSKEL, BARSDI, INITREE.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Shows how to implement hooks. Works like SPY++.

USES:

    Spy-like utility. 

COMMENTS:
    This is a 32-bit only sample.  
    Currently the code to Browse.AllWindows is partially implemented
    and is commented out for this release. Watch for it next rev.

MODULE MAP:
    Dispatch- Message-dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Init    - Performs application and instance-specific initialization
    About   - Defines a standard About dialog box
    Misc    - Defines the application-specific commands not related to
              a specific module.
    Treeview- Shows how a treeview is implemented. 
    Header  - Shows how a header is implemented. 
    toolbar - Implements the Toolbar for the sample
    statbar - Implements the Status bar
    hooks   - Houses the main routines of the module. 
    propsht - Implements property sheets. 
    split.c - Has code to maintain and split the main window. 
    Browse  - Implements the browse dialog.
    List    - Implements the listbox containing all messages.
    Dllmain - Main module for the DLLSKEL.DLL.
    Exports - Contains all exported functions of the DLL.
    Syshook - Installs system-wide hooks.
    Syshook2- Misc functions used by SysHook.C.
