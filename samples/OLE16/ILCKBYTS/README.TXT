Custom ILockBytes Sample

The custom ILockBytes sample demonstrates how to create an ILockBytes on an hGlobal.
This could easily be extended into support onto a region of a file in order to 
maintain backward compatability with a previous file format.  The sample test apps 
provided are designed to show the performance advantage of using custom marshalling 
along with the custom ILockBytes.


The Test Applications

The ILBSVR and ILBCNTR samples are designed to demonstrate that the custom ILockBytes 
is actually working, as well as to show the performance advantage of using custom 
marshalling.  To use the test applications, start ILBCNTR.  From the Bytes menu, 
select the number of bytes that you wish the *server* to write into a stream.  Then 
go to the test menu and select either "Use custom marshalling" or "No custom marshalling" 
to begin the test.  This will instruct the server application to write the specified
number of bytes into a stream.  When the server is done, a message box will appear 
with the length of time that the test took.  A subsequent message box will tell 
if the test passed/failed.

How does the test work?

ILBCNTR creates the ILockBytes, passing a flag to denote whether or not to do 
custom marshalling.  Following this, the "Test" routine is called.  In the "Test" 
routine, ILBCNTR calls CreateDocfileOnILockBytes, to actually create a root storage 
on top of the ILockBytes.  The container then creates a stream called "Number of Bytes" 
in this storage, and writes the number of bytes that the server needs to write in order 
to pass the test.  At this point ILBCNTR instantiates an instance of the server, asking 
for the IPersistStorage interface.  ILBCNTR then calls IPS::Save to start the test.

In ILBSVR the IPS::Save method opens up the "Number of Bytes" stream and reads the 
number of bytes that it needs to write in order to complete the test.  Now, ILBSVR 
creates a stream called "Test", and writes the appropriate number of bytes to the stream.

When the IPS::Save routine returns, ILBCNTR opens up the "Test" stream, and verifies 
that the correct number of bytes were actually written.
