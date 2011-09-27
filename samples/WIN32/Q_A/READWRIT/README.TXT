 Sample: Read/Write Synchronization Demonstration

Summary:

This article refers to the file DATABASE.C, a part of the
READWRIT sample, which is one variation of the classical
synchronization problem Reader/Writers. The Reader/Writer
problem first stated and solved by Courtois revolves around
a shared resource which is typically a database. The issue
here is to allow both readers and writers access to the
database without corrupting it.

More Information:

Typically one allows multiple readers to access the database
as long as a writer is not accessing it. However, when a
Writer is accessing the database no other Readers or Writers
are allowed access. There are several variations of this
problem around. The simplest will favor readers to the
exclusion of writers or visa versa.

The implementation used by the READWRIT sample allows as
many readers in the database at one time until a writer
wished entrance in. Then no other reader can enter the
database until this writer is finished. All readers
currently in the database, however, can finish. This way no
starvation of either the readers or writers which is
inherent in the simpler methods will happen.

In order to keep this sample focused,the user interface is
very simple; to run this sample at the command prompt type:

   READWRIT

What you will see are the values the readers put on the
screen. These are the running total of the threads that
wrote to the database.



