NDDECHAT - A NetDDE Chat Sample

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.


AUTHOR:
    Craig Link - Microsoft Developer Support

PURPOSE:
    To demonstrate the basics of Network DDE (NetDDE) programming with the
    DDE Management Library ( DDEML ) functions.


NetDDE APIs USED:
    NDdeIsValidAppTopicList
    NDdeShareDel
    NDdeShareAdd
    NDdeSetTrustedShare
    NDdeGetErrorString

DDEML APIs USED:
    DdeInitialize
    DdeUninitialize
    DdeQueryString
    DdeCreateStringHandle
    DdeFreeStringHandle
    DdeNameService
    DdePostAdvise
    DdeCreateDataHandle
    DdeGetData
    DdeConnect
    DdeDisconnect
    DdeClientTransaction


COMMENTS:
    This sample may be built as a 16 or 32 bit application.  The default
    is 32 bits.

    When NDDECHAT starts up, it create a NetDDE share, NDDECHAT$.  This
    share allows both read and write access, which will allow a client
    to be advised and poke data.  Once started, NDDECHAT waits for connect
    to be issued from a remote instance.  During this wait, the user
    may also initiate a conversation by selecting the Connect menu item 
    from the File menu and specifying a remote computer name.
    
    The NetDDE share is also given the STARTUP attribute which allows
    connecting clients to cause the server to startup if it is not
    already running ( for this behavior, the NDDECHAT.EXE file must be 
    located somewhere along the search path defined by the PATH
    environment variable )

    32 BIT SPECIFICS:
    - Make sure the NetDDE service is running.  The service can be started
      be executing "net start netdde" from the command line.

    - To allow WRITE access to the share, NDdeShareAdd must be called
      with a NULL Security Descriptor.  This is different than pass 
      NULL as a parameter to the API.  See the code in NDDEUTIL.C on 
      how to create a NULL Security Descriptor.

    - After adding the NetDDE share, it also must be trusted to allow
      remote access.  This is accomplished using the NDdeSetTrustedShare
      API.

    16 BIT SPECIFICS:
    - To build the 16 bit version, you are required to have the Windows
      For Workgroups SDK.  This is only available on the Microsoft
      Developers Network Level II CDs, beginning with the April '94
      edition.


MODULE MAP:
    NddeChat- Implements message handlers for the main window.  Displays
              the text of the current converation.
    Connect - Obtains the name of a server to connect to for a conversation.
    NddeUtil- Creates the NetDDE Share.
    Mem     - Contains the memory allocation routines.
    DdeCBack- Processes the DDEML communication messages.
    Dispatch- Message dispatching routines
    WinMain - Calls initialization functions and processes the message loop
    Init    - Performs application and instance specific initialization
    About   - Defines a standard about dialog box.
    Misc    - Defines the applications specific commands not related to
              a specific module.
