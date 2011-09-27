ServerEnumDialog

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.


AUTHOR:
    Craig Link - Microsoft Developer Support
    Special Thanks to Todd Lucas and Rohan Phillips

PURPOSE:
    To provide a dialog box which allows the user to browse the
    network for servers.

IMPORTANT:
    If you are planning to distribute this DLL with you code, please
    do one of the following...
    - Merge the code into one of your existing DLLs
    - Rename the DLL to a unique name
    - Move the code into the executable
    This will ensure that your application will be using the appropriate
    ServerEnumDialog DLL, and not a modified version of the sample which
    could be loaded on the user's system.

USAGE:
    - You should include SERVENUM.H in the calling source code file.
    - You can dynamically link to the DLL by calling LoadLibrary, and
      then GetProcAddress on the appropriate implentation of the API.
    - You can implicitly link to the DLL by linking with the lib file
      generated when building the DLL.

BUILDING:
    Run "nmake /f makefile" in the root directory of the sample.
    This will execute commands which builds both the DLL and DEMO code.
    If you build the DLL by hand, UNICODE must be defined because the
    DLL makes a call to a LanMan API.

COMMENTS:
    The ServerEnumDialog is a thread safe API.  There are ASCII and Unicode
    versions which are mapped to the appropriate implementation, when UNICODE
    is (un)defined.  ServerEnumDialog allows you to provide help for the
    dialog by including a function as a parameter, which is called when the
    user clicks the HELP button.

MODULE MAP:

    DLL directory
      ServEnum - Implements the API wrappers for the dialog box
      EnumDlg  - Implements the message handlers from the dialog box
      EnumThrd - Implements thread procedures for enumeration
      EnumUtil - Implements utility functions used for enumeration
      ThrdUtil - Implements functions to maintain handles for outstanding
                 threads
      MiscUtil - Implements miscellaneous utilities
      DllDisp  - Implements message handling routines
      Mem      - Implements memory allocation routines

    Demo directory
      Browse   - Implements browse demo window
      Dispatch - Message dispatching routines
      WinMain  - Calls initialization functions and processes the message loop
      Init     - Performs application and instance specific initialization
      About    - Defines a standard about dialog box.
      Misc     - Defines the applications specific commands not related to
                 a specific module.
