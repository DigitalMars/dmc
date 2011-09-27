Sample: Using Thread API Functions

Summary:

The THREADS sample application shows how to use
SetThreadPriority, SuspendThread, and ResumeThread.

More Information:

To use THREAD, start the application. Two threads will be
created; one will draw a red rectangle and the other a green
rectangle. Both of these rectangles move about the window;
their speed and behavior is based on the thread priority and
its resumed/suspended status. The priority and status are
set through menu selections.

Of special interest is the suspension count. The system
keeps track of the number of times a thread has been
suspended and resumed. Each time the thread is suspended,
the count is incremented; each time it is resumed, the count
is decremented. The suspension count can either be tracked
by applications manually, the same way this sample
application does, or the return value from ResumeThread and
SuspendThread can be used to obtain the previous suspension
count before the call was made. Only when the suspension
count is zero will the thread run.

A thread now has seven priorities exposed at the API level:

   THREAD_PRIORITY_IDLE
   THREAD_PRIORITY_LOWEST
   THREAD_PRIORITY_BELOW_NORMAL
   THREAD_PRIORITY_NORMAL
   THREAD_PRIORITY_ABOVE_NORMAL
   THREAD_PRIORITY_HIGHEST
   THREAD_PRIORITY_TIME_CRITICAL

Note that THREAD_PRIORITY_TIME_CRITICAL should be used very
cautiously by an application, and only if needed.  Setting a
thread to this level will interfere with the application's
window performance, and the performance of other applications
being run on the system.
