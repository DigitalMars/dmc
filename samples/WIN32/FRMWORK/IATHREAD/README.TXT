IAThread - Based on BlandMDI

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    Shows how to create a multithreaded MDI application, and allows the user
    to adjust relative thread priorities to experiment with the feel of a
    multithreaded application.

USES:
    As a starting point for writing multithreaded MDI applications, and as
    a learning tool for setting relative thread priorities within an
    application.

COMMENTS:
    This application has one main thread that handles all of the user
    interface.  All child window procedures run in the main user interface
    thread.  When a child window is created, a new thread is created that
    does work on the "document" that the child window represents, and draws
    in the child's client area.  Each child window uses its window extra
    bytes to store a pointer to a data structure that is shared between the
    child window procedure and the child's worker thread.  One important
    part of this data structure is a critcal section that is used to
    synchronize access to the rest of the data structure between the user
    interface thread running the child window procedure, and the child's
    worker thread.

    There is one general usability feature that is not obvious.  When a user
    selects the Time Critical or Highest thread priorities, all other
    threads in the application slow down immensely.  When a time critical
    thread is running, all other threads in the application stop executing.
    To prevent time critical and highest priority threads from rendering
    IAThread useless, these threads have a timeout mechanism that will
    reset their priorities after a while.  The function that implements
    the timeout check is ReducePriority() in MISC.C.  ReducePriority will also
    reduce the process's priority class for the same reason.

    When a time critical thread starts, it usually preempts the user-
    interface thread before the Set Priority/Class dialog box goes away.
    Thus, the dialog stays on the screen although it doesn't do any more
    processing.  Applications that want to dynamically adjust priorities
    and make sure that dialogs go away need to prevent threads with higher
    priorities than the thread handling the dialog from running.

MODULE MAP:
    Dispatch - Message dispatching routines
    WinMain  - Calls initialization functions and processes the message loop
    Iathread - Contains the window procedure for the main application window
    Init     - Performs application and instance specific initialization
    About    - Defines a standard about dialog box.
    Misc     - Defines the application specific commands not related to
                 a specific module.
    MDIChild - Creates the MDI client window and the MDI Children.  Also it
                 contains the window procedure for the MDI children
    Priority - Contains all functions related to the Priority dialog box.
