This directory contains the files for the sample distributed application
"nhello":

File    Description

README.TXT      Readme file for the NHELLO sample
NHELLO.IDL      Interface definition language file
NHELLOC.C       Client main program
NHELLOS.C       Server main program
NHELLOP.C       Remote procedures
NSSERV.C        NT Service program
SERVICE.C       Service Control Manager interface
MAKEFILE        Nmake file to build for Windows NT or Windows 95
MAKEFILE.DOS    Nmake file to build for MS-DOS

-----------------------------------------------------------------------
BUILDING CLIENT AND SERVER APPLICATIONS FOR MICROSOFT WINDOWS NT OR WINDOWS 95:
-----------------------------------------------------------------------

The following environment variables should be set for you already.
  set CPU=i386
  set INCLUDE=%SDKROOT%\h
  set LIB=%SDKROOT%\lib
  set PATH=%SDKROOT%\system32;%SDKROOT%\bin;

Where %SDKROOT% is the root directory for the 32-bit Windows SDK.

For mips, set CPU=mips
For alpha, set CPU=alpha

Build the sample distributed application:
  nmake cleanall
  nmake

This builds the executable programs nhelloc.exe
(client), nhellos.exe (console server), and nsserv
( NT service ).

-----------------------------------------------------------------------
BUILDING THE CLIENT APPLICATION FOR MS-DOS
-----------------------------------------------------------------------

After installing the Microsoft Visual C/C++ version 1.50 development
environment and the 16-bit RPC SDK on a Windows NT or Windows 95
computer, you can build the sample client application from Windows NT
or Windows 95.

  nmake -f makefile.dos cleanall
  nmake -f makefile.dos

This builds the client application nhelloc.exe.

You may also execute the Microsoft Visual C/C++ compiler under MS-DOS.
This requires a two step build process.

  Step One: Compile the .IDL files under Windows NT or Windows 95
     nmake -a -f makefile.dos nhello.h

  Step Two: Compile the C sources (stub and application) under MS-DOS.
     nmake -f makefile.dos


-----------------------------------------------------------------------
RUNNING THE CLIENT AND SERVER APPLICATIONS
-----------------------------------------------------------------------

-- NHELLOS --
On the server, enter

  net start rpclocator
  set start rpcss
  nhellos

(You can also start the RPC Locator and RPC Service by running
the Services program in Control Panel.)


-- NSSERV --
To install the service, first compile everything, and then:

    nsserv -install

Now all you have to do is start it, either using the
"net start" method, the control panel Services applet, or
the SC.EXE utilitie in the MSTOOLS\BIN directory:

    sc start nshelloservice

Once the service has been started, you can use the client
program to verify that it really is working, using the syntax:

    nhelloc

which should cause to service to stop since it calls Shutdown()


If, after playing with the sample you wish to remove the service,
simply say:

    nsserv -remove

Other: NSSERV can be run as a console app by entering

    nsserv -debug

For more information on SERVICES see the SERVICE sample included
with the Win32 SDK.


-- NHELLOC --
On the client, enter

  net start workstation
  nhelloc


Note:  The client and server applications can run on the same Microsoft
Windows NT computer when you use different screen groups.  Also, only NSSERV
or NHELLOS should be run at one time on the same Windows NT computer.


Several command line switches are available to change settings for this
program. For a listing of the switches available from the client
program, enter:

  nhelloc -?

For a listing of switches available from the server program and the
service program enter

  nhellos -?
