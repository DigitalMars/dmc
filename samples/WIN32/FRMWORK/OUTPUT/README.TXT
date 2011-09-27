OUTPUT - SIMPLE

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.

PURPOSE:
    Shows the use of two fundamental text drawing apis and several
    of the line and shape drawing apis.

USES:
    Based directly on Simple

COMMENTS:


MODULE MAP:
    WinMain - Calls initialization functions and processes the message loop
              **OUTPUT** Added call to FreeOutput to destroy gdi objects
                    created during instance initialization.
    Output  - Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
              **OUTPUT** Added call InitOutput to create gdi objects
