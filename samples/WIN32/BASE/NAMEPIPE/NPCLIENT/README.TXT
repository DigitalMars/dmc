Sample: Named Pipe Client/Server Demonstration

Summary:

NPCLIENT and NPSERVER demonstrate the use of named pipes.
The basic design consist of a server application serving
multiple client applications.  The user can use the client
applications as an interface to all of the other client
applications via the server.  The effect is a simple
communication program that can be used over the network
between multiple clients.

More Information:

The actual implementation works by having the NPSERVER
application launch a new thread, which creates and services
a new instance of the server side of the named pipe every
time a client connects to it.  You need only start one
instance of the NPSERVER application.  It will service up to
100 instances of the NPCLIENT application.  (Note that the
100 instance limit is hard coded into the sample.  It does
not reflect the number of named pipe instances you can
create, which is virtually infinite.)

TO USE:

Start an instance of NPSERVER.  A window will appear.

Start an instance of NPCLIENT.  Two dialog boxes will
appear, one on top of the other.  The top level dialog box
will prompt you for a share name and a client or user name.
If the instance of NPCLIENT is local to (on the same machine
as) the NPSERVER instance, enter a '.' for the share name.
Otherwise, enter the machine name of the server that the
NPSERVER instance was started on, i.e. 'FoobarServer'.  For
the client or user name, enter any name you wish to be
identified with.  Hit enter or click the OK button.

The upper dialog box will go away, and you'll see the Client
dialog box of NPCLIENT.  It consists of two edit fields and
a 'Send' button.  You will be able to read messages from
other clients (and yourself) in the larger/upper edit field.
(Note, if the message seems garbled, make sure the cursor of
the edit field is located in the lower left hand corner of
the field.)  The smaller edit field is used to type
messages.  To send a message: type something in the
lower/smaller edit field, and hit enter or click the Send
button.  The message will appear in the larger edit field of
all the clients connected to the NPSERVER instance;
prepended by the user name you selected.  Note that the user
name you selected will be entered into the caption bar of
the NPCLIENT instance.  This allows you to more easily keep
track of multiple instances of NPCLIENT on the same machine.

At the same time the top level dialog box was dismissed from
the NPCLIENT instance, the NPSERVER window was updated with
the picture of a red spool of thread accompanied by the user
name you selected.  This red spool indicates an active
client thread connected to NPSERVER.  The spool may be
connected to other spools with a thin blue line (similar to
the way the File Manager connects files or directories).
Any time a client disconnects from NPSERVER; the spool
representing it will be grayed out.

DESIGN:

Basically, the NPSERVER application launches multiple
instances of a server thread.  When the application is
started, the first thread is created.  It creates an
instance of the server side of the named pipe, and waits for
a client to connect.  Once a client connects, another thread
is started and it too blocks waiting for a client.
Meanwhile, the first thread updates a global array of client
information with this specific client's information.  The
thread then enters a loop reading from this client.  Any
time this specific client sends a message, this server
thread will call a function (TellAll) which will write the
message to all the clients that have been listed in the
global array.

On the client side, NPCLIENT tries to connect to the named
pipe with a CreateFile call.  Once it has connected, it
creates a thread which loops and reads any message from the
server side.  Once a message is read, it is printed in the
larger edit field.  Any time the user hits the Send button,
the main thread grabs any text in the lower edit field, and
writes it to the server.

The steps between NPSERVER and an instance of NPCLIENT looks
like this:

   NPSERVER                          NPCLIENT
   --------                          --------

   CreateNamedPipe()
   ConnectPipe()   // Blocks
                                      CreateFile()  //Connects to pipe.
                                      spawn separate thread to read pipe
   return from block
   updates array of clients
   spawn another server thread
   Loop
     ReadFile() // Blocks on overlap
                                      WriteFile() // User hits Send.

     return from block
     WriteFile() // Broadcast to clients
   End loop      // When client breaks pipe.


                                      ReadPipe Thread:
                                        Loop
                                          ReadFile()
                                            block till server broadcasts

                                            return from block.
                                            put string in edit field.

                                         End loop // when server breaks.

The overlapped structure should be used anytime a pipe is
expected to block for any length of time on a read or write.
This allows the thread to return immediately from a read or
write to service any other part of your application.  The
overlapped structure should also be used on a named pipe
anytime you expect to do simultaneous reads and writes.
