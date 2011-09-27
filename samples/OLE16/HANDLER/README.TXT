========================
  DBGHANDLR & INSTALLR
========================

  DESCRIPTION
---------------

dbghandlr.dll & installr.exe are a sample of an OLE 2.0 InProc handler
which allows you to get in-between the OLE 2.0 default handler and the OLE 2.0
container. Dbghndlr traps all calls to interfaces from the container
and displays them with parameters in its window & a log file which is
the title of the window created for the object.

Use installr.exe as a simple means of installing dbghndlr.dll as an InProc
handler for any OLE 1.0 or 2.0 server which will allow for it.  Installr.exe
is limited to installing no more than five servers at once.

This sample uses Microsoft Foundation Classes.  If you need to make 
changes to the code it can be rebuilt using MFC 2.0 using the SMAKE command.

SNMAKE DEBUG=0 (release build)
SMAKE DEBUG=1 (debug build)
