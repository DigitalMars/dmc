Threads - Based on generic.  A simple threading sample.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    To demonstrate thread creation and destruction.

USES:
    Based on Generic.

COMMENTS:
    This is a very simple thread demonstration program.  It divides
    the main window into 4 child windows and allows the user to 
    control the creation and deletion of 4 threads.  The threads
    are: 1) A simple count from 1 to the maximum int 2) A calculation
    of the Greatest Common Divisor for two numbers 3) A calculation
    of all the prime numbers between 1 and MAX_INT and 4) Drawing
    random rectangles.

    The key structure to the Threads sample is ThreadInfo. An array
    of these structures contain the window handles of the respective
    threads, the height and width of the client area of the child 
    windows, and the state flag of the thread.  The state flag 
    indicates whether the thread is excuting or not.


MODULE MAP:
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Threads - Implements the windows procedure for the main application 
              window.  It also has the dispatch loops for messages and
              commands.  For the Threads sample, this source module 
              contains message handlers and cmd handlers.
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.
    Threads - Threads contains the dispatch loop for the child windows,
              the WM_THREADSTATE message handler, the WM_SIZE handler
              for the child windows, and the thread procedures.
        
              The threads themselves are not designed to do anything 
              terribly interesting.  If you were designing an application
              which made use of threads, you probably wouldn't want 
              to have threads that are as impolite as these.  The reason
              the threads in this sample are impolite is that they loop
              while TRUE and never worry about setting priorities or 
              synchronizing their execution.
