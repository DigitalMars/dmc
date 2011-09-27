This directory contains the files for the sample
distributed application "whello":

File          Description

README.TXT    Readme file for the WHELLO sample
WHELLO.IDL    Interface definition language file
WHELLO.ACF    Attribute configuration file
WHELLOC.C     Client main program
WHELLO.RC     Client resource file
WHELLO.DLG    Client dialog box definitions
WHELLO.DEF    Client module definition file
WHELLOS.C     Server main program
WHELLOP.C     Remote procedures
MAKEFILE      Nmake file for Windows NT or Windows 95
MAKEFILE.WIN  Nmake file for Win 3.x

-------------------------------------------
BUILDING CLIENT AND SERVER APPLICATIONS FOR
MICROSOFT WINDOWS NT OR WINDOWS 95:
-------------------------------------------

The following environment variables should be set for you already.
  set CPU=i386
  set INCLUDE=%SDKROOT%\h
  set LIB=%SDKROOT%\lib
  set PATH=%SDKROOT%\system32;%SDKROOT%\bin;

Where %SDKROOT% is the root directory for the 32-bit Windows SDK.

For mips, set CPU=mips
For alpha, set CPU=alpha

Build the sample server application:

  nmake cleanall
  nmake

These commands build the executable program whellos.exe
and whelloc.exe for Microsoft Windows NT or Windows 95.

--------------------------------------------
BUILDING THE CLIENT APPLICATION FOR WIN 3.X
--------------------------------------------

After installing the Microsoft Visual C/C++ version 1.50 development
environment and the 16-bit RPC SDK on a Windows NT or Windows 95
computer, you can build the sample client application from Windows NT
or Windows 95.

  nmake -f makefile.win cleanall
  nmake -f makefile.win

This builds the client application whelloc.exe.

You may also execute the Microsoft Visual C/C++ compiler under MS-DOS.
This requires a two step build process.

  Step One: Compile the .IDL files under Windows NT or Windows 95
     nmake -a -f makefile.win whello.h

  Step Two: Compile the C sources (stub and application) under MS-DOS.
     nmake -f makefile.win

------------------------------------------
RUNNING THE CLIENT AND SERVER APPLICATIONS
------------------------------------------

On the server, enter

  whellos

On the client, enter

  net start workstation
  whelloc

Note:  The client and server applications can run on
the same Microsoft Windows NT or Windows 95 computer when you use
different screen groups.  If you run the client on the
Microsoft MS-DOS and Windows computer, choose the Run
command from the File menu in the Microsoft Windows 3.x
Program Manager and enter whelloc.exe.

Several command line switches are available to change
settings for the server application. For a listing of
available switches, enter

  whellos -?

---------------------
RUNNING ON WINDOWS 95
---------------------

If you wish to run the server on Windows 95, you will need to use
the LRPC protocol.

To start the server, enter

 whellos -p ncalrpc

To start the client (from the same machine), enter

 whelloc

Change the protocol to "ncalrpc" by selecting "Protocol sequence name"
from the "Remote Call" menu and entering

 ncalrpc

