This directory contains the files for the sample distributed application
"yield":

File          Description

README.TXT    Readme file for the YIELD sample
YIELD.IDL     Interface definition language file
YIELD.ACF     Attribute configuration file
YIELDC.C      Client main program
YIELDC.RC     Client resource file
YIELDC.DLG    Client dialog box definitions
YIELDC.DEF    Client module definition file
YIELDS.C      Server main program
YIELDP.C      Remote procedures
MAKEFILE      Nmake file for Windows NT or Windows 95
MAKEFILE.win  Nmake file for Win 3.x

-------------------------------------------------------------------
BUILDING SERVER APPLICATION FOR MICROSOFT WINDOWS NT OR WINDOWS 95:
-------------------------------------------------------------------

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

These commands build the server executable program YIELDS.EXE.

--------------------------------------------
BUILDING THE CLIENT APPLICATION FOR WIN 3.X
--------------------------------------------

After installing the Microsoft Visual C/C++ version 1.50 development
environment and the 16-bit RPC SDK on a Windows NT or Windows 95
computer, you can build the sample client application from Windows NT
or Windows 95.

  nmake -f makefile.win cleanall
  nmake -f makefile.win

This builds the client application yieldc.exe.

You may also execute the Microsoft Visual C/C++ compiler under MS-DOS.
This requires a two step build process.

  Step One: Compile the .IDL files under Windows NT or Windows 95.
     nmake -a -f makefile.win yield.h

  Step Two: Compile the C sources (stub and application) under MS-DOS.
     nmake -f makefile.win

------------------------------------------
RUNNING THE CLIENT AND SERVER APPLICATIONS
------------------------------------------

On the server, enter

  yields

On the client, choose the Run command from the File menu in the
Microsoft Windows 3.x Program Manager and enter YIELDC.EXE.

Several command line switches are available to change settings for
the server application. For a listing of available switches, enter

  yields -?

Bug: If you cancel in the middle of a custom yield, you must wait
for the number of seconds you originally set before making another
remote procedure call.
