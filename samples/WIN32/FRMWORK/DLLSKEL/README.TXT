DLLSKEL - Based on GENERIC

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
    To demonstrate how to construct a simple Win32 DLL and implicitly load
    it from a Win32 application.  The application calls two of the DLL's
    exported functions.

USES:
    Use this sample as a starting point for building Win32 DLLs.

COMMENTS:
    No module definition file is used in this sample because Win32
    applications do not export functions and all settings in a module
    definition file can be set with linker command-line switches.

    The 32-bit C/C++ compiler supports two new keywords for exporting
    functions from Win32 DLLs and importing them into applications:

      __declspec(dllexport)     // Used to declare exported functions
      __declspec(dllimport)     // Used to declare imported functions

    Both of these keywords go before the function declaration, as in:

       __declspec(dllexport) int FunctionName (int param1, float param2);
       __declspec(dllimport) int FunctionName (int param1, float param2);

    A DLL exports a function by putting the __declspec(dllexport)
    keyword in front of the function declaration and then using the
    -implib:DLLName.LIB linker command line argument.

    An application can then import the function by declaring it with the
    __declspec(dllimport) keyword and linking to the DLL's import library.


    In general, Win32 DLLs should be built to handle multithreaded
    applications robustly because DLLs cannot control which types of
    applications call them.  To do so, link with the multithreaded
    runtime libraries, use the /MT switch, and then make sure that
    global variables and other shared resources (file handles, dynamically-
    allocated memory, etc) are protected by critical sections or mutexes.
    A good place to initialize critical sections and mutexes is in the
    DLL_PROCESS_ATTACH message of DllMain().

MODULE MAP:
  The following files implement the application (APPSKEL.EXE):

    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    AppSkel - Implements the windows procedure for the main application window
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
                a specific module.

  The following files implement the DLL (DLLSKEL.DLL):

    DLLMain - The DLL's entry point function
    Exports - Contains the DLL's exported functions


    MAKEFILE     - The application's makefile.  Also executes DLL's makefile.
    DLLSKEL.MAK  - The DLL's makefile

