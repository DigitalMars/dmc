Sample: Demonstration of the Windows Sockets API

Summary: 

The WSOCK sample demonstrates the basics of sockets programming,
specifically for Windows Sockets. It demonstrates how to accept
incoming connections (via the Windows Sockets Asynchronous Extension
APIs, threads, and traditional BSD-style blocking calls) and how to
connect to remote hosts. Once connected, the user can send a text
string to the remote host. WSOCK also allows the user to view
information on a user-entered host name.

More Information:

For the program to operate correctly, the TCP/IP protocol must be
properly installed. Also, if two machines are used over a network,
both machines must have a "HOSTS" text file (for Windows NT machines,
this file is located in %SYSTEMROOT%\SYSTEM32\DRIVERS\ETC\HOSTS; if
TCP/IP is installed onto a Windows for Workgroups machine, the HOSTS
file is located in C:/WINDOWS/HOSTS). Within each HOSTS file, both the
remote and local addresses of both machines must be listed.

WSOCK can run on a single machine (execute two copies of WSOCK) or
over a network with two Win32 machines. The following example explains
how two separate machines over a network would test WSOCK:

1. Machine "Bob" executes a copy of WSOCK.

2. Machine "Fred" executes a copy of WSOCK.

3. Machine "Bob" chooses one of the Listen menu options (under
   WinSock) [Listen (Blocking), Listen With Threads, or Async Listen].

4. Machine "Fred" selects the Connect menu option (under WinSock).

5. Machine "Bob" enters "12" as a TCP port number.

6. Machine "Bob" waits for a connection.

7. Machine "Fred" enters "Bob" as the host name to connect to.

8. Machine "Fred" enters "12" as a TCP port number.

Both machines are now connected and can send strings back and forth by
using the WinSock Send Message menu option.

If "Bob" exits WSOCK while there is a connection, "Fred" will receive
a message box notification.

Windows Sockets calls used:

   accept
   closesocket
   connect
   gethostbyname
   getservbyname
   htons
   listen
   send
   recv
   WSAAsyncSelect
   WSACleanup
   WSAStartup
