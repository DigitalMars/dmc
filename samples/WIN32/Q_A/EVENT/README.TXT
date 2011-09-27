 Sample: Asynchronous I/O Demonstration

Summary:

The EVENT sample demonstrates perfoming asynchronous I/O in
Win32.  In Win32 you are able to do this in two ways.  One
is similar to OS/2 where a thread was spawned which did the
IO and returned. With Win32, when you create a file it is
able to signal to the system that you wish to perform I/O
asynchronously.  Then when, say, ReadFile is going to take a
significant amount of time to complete it will generate
ERROR_IO_PENDING. This signals to you to go about doing
other tasks till you NEED the data.  At which time you will
use the function GetOverlappedResults which will finish the
I/O.

More Information:

Note this has all been done without the need of an
additional thread to perform the work. This sample only
touches on the capabilities of what one can do with the new
overlapped I/O functions. For example an application that
uses pipes to communicate over the network to other clients
can create these file handles with the overlapped flag.
Then instead of blocking and waiting for a connection the
server app can go about doing useful tasks waiting for the
pipe to enter the "signaled" state. Another even more exotic
feature of files created this way is you are able to perform
more than one operation on this handle at one time such as
reading and writing to the same file.

All this power does not come with out some responsibilities
on the programmer's side. First the system does not keep
track of the system file pointers as you are no doubt used
to. And naturally just because the call returns imediately
you are NOT able to use the data until the system responds
by setting an event to a signaled state.

In the first case this simply means you need to keep track
of the value "lpNumberOfBytesTransfered" returned by
GetOverlappedResult and update the OVERLAPPED structure with
this information. This structure OVERLAPPED will then be
passed into the Read/Writefile function which will use this
as the offset to starting point for the I/O operation. The
first call to Read/Writefile will normally then have the
offset fields in the OVERLAPPED structure set to zero. The
second case should be used as a criteria of whether to use
this type of I/O obviously if you need the data before you
can do anything else you may as well use normal sychronous
I/O and let the system handle the details for you. This also
demonstrates an important reason for using an EVENT to wait
on rather than the file handle. While both are allowed in a
multithread application one could not gaurantee that the
thread which set the handle to the signalled state would be
the one returning from the GetOverlappedResult since each
thread was using the same handle to wait on.

Inorder to keep this sample focused, the user interface is
simple.  To run this sample at the command prompt type:

     ASYNC_IO <In_file> <Out_file>

where In_file and Out_file are place holders. As this is
implemented you are not able to write over an existing file.
While this app is running you will see the vital statistics
such as:

  When IO is pending
  How many bytes are transfered
  End of file



