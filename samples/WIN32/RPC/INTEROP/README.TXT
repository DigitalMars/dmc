/*************************************************************/
/**                                                         **/
/**                    Microsoft RPC Examples               **/
/**               OSF DCE Interop Sample Application        **/
/**                Copyright(c) Microsoft Corp. 1993-1995   **/
/**                                                         **/
/*************************************************************/

Overview:
---------

This is a small demo RPC application.  It is designed to be portable
between OSF DCE RPC and Microsoft RPC platforms.

The most important part of the demo is the header file dceport.h.  This
header file maps OSF DCE RPC APIs and data structures to the
Microsoft RPC equivalents.

The program just sends simple messages (strings) from the client
to the server.

Building on Windows NT or Windows 95
------------------------------------

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

Building on MS-DOS Systems
--------------------------

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
     nmake -a -f makefile.dos msg.h

  Step Two: Compile the C sources (stub and application)
     nmake -f makefile.dos

Building on DCE Systems
________________________

You need to copy the following files to the DCE machine:
client.c
server.c
manager.c
msg.idl
msg.acf
makefile.dce

  make -f makefile.dce cleanall all

Note: You will probably need to change the CFLAGS and LIBS
variables in makefile.dce to match your platform.

Using the program:
------------------

The basic example:

Run
  server
on the server machine.

Run:
  client -n <server name> -s "Hi, I'm a client"
on the client machine to send the message.

Run:
  client -n <server name> -s "Okay, stop this example" -x
on the client to send the message and cause the server to stop.

You can use fixed endpoints by adding the -e switch:

server -e 3452
client -e 3452 -n <server name> -s "Hi, I'm a client"
client -e 3452 -n <server name> -s "Okay, stop this example" -x

You can run the demo over a different protocol by adding a -t switch
to both the client and server:

server -t ncacn_np
client -t ncacn_np -n <server name> -s "Hi, I'm a client"
client -t ncacn_np -n <server name> -s "Okay, stop this example" -x


Options:
--------

The -h switch displays a usage message.

The -s <message> switch is used to change with message sent from the client
    to the server.  Without it the message "Hello World" is sent.

The -n <server_name> switch is used for specifying a server machine.
    Without it the server is assumed to run on the same machine.

The -e <endpoint> switch is used to specify a fixed endpoint to be used.
    Without it a dynamic endpoint will be used and registered with the
    endpoint mapper.

The -t <protseq> switch is used to specify which protocol to use.  Without
    it the protocol sequence "ncacn_ip_tcp" will be used.


RUNNING ON WINDOWS 95:
----------------------

If you wish to run the server on Windows 95, you will need to use
the LRPC protocol.

To start the server, enter

 server -t ncalrpc

To start the client (from the same machine), enter

 client -t ncalrpc

