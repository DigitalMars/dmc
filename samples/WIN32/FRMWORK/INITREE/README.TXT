INITREE - Based on Gen32

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.

PURPOSE:
    Show how to create and fill a TreeView control with a
    hierarchical list of items (in this example, the INI
    files in the Windows directory are used).
    Show how to use the ImageList in conjunction with the
    TreeView control.
    Show how to use the FindFirstFile and FindNextFile APIs
    to search a directory.

USES:
    Based on Gen32

COMMENTS:
    This is a 32-bit only sample.

MODULE MAP:
    Dispatch         - Message dispatching routines
    WinMain          - Calls initialization functions and processes 
                       the message loop
    Generic          - Implements the windows procedure for the main 
                       application window
    Init             - Performs application and instance specific 
                       initialization
    About            - Defines a standard about dialog box.
    Misc             - Defines the applications specific commands 
                       not related to a specific module.
    INI_FillTreeView - Entry point function to fill TreeView Control.
    FillIniKeys      - Build a list of sections for each INI file.
    FillKeyItems     - Build a list of entries for each section.
    CreateImageList  - Creates an Image List.
    CreateTreeView   - Create a TreeView control.
