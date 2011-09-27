This directory contains the files for the sample
distributed application "picklt":

File          Description

README.TXT    Readme file for the PICKLT sample
PICKLT.IDL    Interface definition language file
PICKLT.ACF    Attribute configuration file
PICKLTC.C     Client application for type pickling
MAKEFILE      Nmake file to build for Windows NT or Windows 95
MAKEFILE.DOS  Nmake file to build for MS-DOS

The sample demonstrates type pickling to a file.
A fixed buffer and an incremental handle styles are used.
The implicit primitive handle is used as the pickling handle.

-----------------------------------------------------------------------
BUILDING THE (CLIENT) FOR MICROSOFT WINDOWS NT OR WINDOWS 95:
-----------------------------------------------------------------------

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

This builds the executable programs pickltc.exe.
There is no server side in this sample as the notion of server does not
apply to pickling.

-----------------------------------------------------------------------
BUILDING THE CLIENT APPLICATION FOR MS-DOS
-----------------------------------------------------------------------

After installing the Microsoft Visual C/C++ version 1.50 development
environment and the 16-bit RPC SDK on a Windows NT or Windows 95
computer, you can build the sample client application from Windows NT
or Windows 95.

  nmake -f makefile.dos cleanall
  nmake -f makefile.dos

This builds the client application callc.exe.

You may also execute the Microsoft Visual C/C++ compiler under MS-DOS.
This requires a two step build process.

  Step One: Compile the .IDL files under Windows NT or Windows 95
     nmake -a -f makefile.dos picklt.h

  Step Two: Compile the C sources (stub and application) under MS-DOS.
     nmake -f makefile.dos


------------------------------------------
RUNNING THE APPLICATION
------------------------------------------

The default setting for this application is to encode data to pickle.dat file.
You enter

  pickltc

To decode data that has been encoded, enter

  pickltc -d

Several other command line switches are available to change
settings for this program. For a listing of the switches
available from the client program, enter

  pickltc -?


