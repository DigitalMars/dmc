SimpSvr - Simple OLE Server

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.


PURPOSE:
This sample is the simplest OLE 2.0 object that can be written and
still support the visual editing feature.  The object that this server
supports is a colored square with a black border.

COMMENTS:
    This is a 32-bit only sample.

    *IMPORTANT* Before a SimpSvr object can be embedded in a container
    application, it must be registered with the system.  This can be done
    by running "REGEDIT /S SIMPSVR.REG" from a command prompt or by opening
    SIMPSVR.REG from a shell window.

    Also, before registering SimpSvr, it is necessary to either copy
    SIMPSVR.EXE into a directory on your path or modify SIMPSVR.REG to add
    the full path to your version of SIMPSVR.EXE on all lines that refer
    to SIMPSVR.EXE.
