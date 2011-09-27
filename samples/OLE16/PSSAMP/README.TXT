This directory contains a sample proxy and stub for the interface
IOLE2DevConfSample.  The files in this directory are:

makefile    - makes pssamp.dll
odcsamp.h   - contains the defintion of the interface (what is given to others)
pssamp.h    - definitions for proxy and stub objects
pssamp.cpp  - C++ code for the proxy and stub objects
pssamp.def  - def file for pssamp.dll
pssamp.reg  - registration file

To compile, just run smake.

To install, copy pssamp.dll to a directory on your path (or windows\system)
and run regedit pssamp.reg.

To create a proxy/stub for your own interface, replace odcsamp.h with
an equivalent, change all references to the interface (IID, methods, etc.)
and change the CLSID.  Add a version resource in case you need to update
the dll later.

See the OLE2 Programmer's Reference on assigning IID and CLSID values.  You must
not use the values in this sample for anything other than the sample
interface and sample proxy/stub.
