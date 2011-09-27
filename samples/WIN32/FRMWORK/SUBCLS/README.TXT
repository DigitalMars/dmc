SUBCLS - Based on Generic

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.

PURPOSE:
    Show a basic implementation of subclassing controls like edit and 
    a list box.

USES:
    Based on DIALOG

COMMENTS:
    The list box control and an edit control do not notify the user for
    certain special keys. For example, the list box will not notify the
    parent if the user hits the enter key or the INSERT or DELETE key. 
    To trap these keys and provide more functionality to the controls, 
    one needs to subclass the control and add the functionality. 

MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Subcls  - Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not realted to
                a specific module.
    Modal   - Implements the modal demo dialog box
    Subcls  - Has all the routines for subclassing the controls
    Dialog  - Defines the appliation specfic dialog and its commands. 
