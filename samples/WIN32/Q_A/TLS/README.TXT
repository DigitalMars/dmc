Sample: Thread Local storage without cl help

Summary:

This sample demonstrates using TLS to store thread
specific data in a DLL.  As each thread attaches,
the memory is alloc'd and filled.  As each thread
detaches, it is retreived and as the process detaches,
the tls is freed.

This is a very basic sample, and the calls into the
DLL that cause TLS to be alloc'd and filled don't
really do anything, but this should be a good basis
to build TLS on.
