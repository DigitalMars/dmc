This directory contains the files for the sample
distributed application "rpcssm":

File          Description

README.TXT    Readme file for the rpcssm sample
rpcssm.IDL    Interface definition language file
rpcssm.ACF    Attribute configuration file
rpcssmC.C     Client main program
rpcssmS.C     Server main program
rpcssmP.C     Remote procedures
MAKEFILE      Nmake file to build for Windows NT or Windows 95

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

For mips,  set CPU=mips
For alpha, set CPU=alpha

Build the sample distributed application:
  nmake cleanall
  nmake

This builds the executable programs
rpcssmc.exe (client) and rpcssms.exe (server).

-----------------------------------------------------------------------
BUILDING THE CLIENT APPLICATION FOR MS-DOS
-----------------------------------------------------------------------

This sample does not currently support a DOS build.

------------------------------------------
RUNNING THE CLIENT AND SERVER APPLICATIONS
------------------------------------------

On the server, enter

  rpcssms

On the client, enter

  net start workstation
  rpcssmc

Note:  The client and server applications can run on
the same Microsoft Windows NT computer when you use
different screen groups.

Several command line switches are available to change
settings for this program. For a listing of the switches
available from the client program, enter

  rpcssmc -?

For a listing of switches available from the server
program, enter

  rpcssms -?

---------------------
RUNNING ON WINDOWS 95
---------------------

If you wish to run the server on Windows 95, you will need to use
the LRPC protocol.

To start the server, enter

 rpcssms -p ncalrpc

To start the client (from the same machine), enter

 rpcssmc -p ncalrpc

