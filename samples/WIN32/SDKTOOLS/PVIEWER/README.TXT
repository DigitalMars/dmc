Sample: Process Viewer  [NT only]

Summary:

The Process Viewer sample shows how to read the various bits
of trivia and statistics from the Registry relating to the
various threads and process hanging about on your machine.
It also allows you to muck about with the priority of the
processes.  There is about a third of the code present to
do the same on a thread-by-thread basis, so if you feel
adventurous and/or wish to learn something, feel free to
flesh those sections out.

Addtional Information:

Two list boxes present themselves on startup, these being
the list of processes, and when a particular process is
selected, the other list box will contain the threads that
are associated with the selected process.  Clicking on the
"Memory Details" button will bring up another dialog that
conatins more detail on how the memory associated with that
process is being used.  Another list box allows you to see
how the memory usage breaks down at the DLL level.

Obscure Information about the source code:

The way the program works may be non-obvious at first, but
it will mostly clear up when you realize that when the
programmer named a function "PaintAddressSpace," it wasn't
named that because it fills the monitored process/thread's
memory with certain values, it fetches all of the various
bits of statistics pertaining to memory associated with the
specified process/thread.
