 Sample: Using Semaphores to Control Threads

Summary:

The SEMAPHOR sample application shows how to control four
threads with a semaphore. It demonstrates this by having
four threads competing for the right to draw their color to
a rectangle in the center of the window. Access to the
center area is controlled by the semaphore.

More Information:

When you start the application you will see five rectangles:
a dynamic rectangle in the center (always changing color),
and four static rectangles surrounding it. Each of the four
rectangles has its own color: red, blue, green, and gray.
The one in the center alternates between these colors.

The four static rectangles represent four threads. These
four threads compete for the rectangle in the middle, and
their access is controlled by the semaphore. When a thread
gains control of the semaphore, it gets to draw its color in
the center rectangle. (Note: The threads do not actually
draw any of the four static rectangles. To make the code
simpler, this is handled in the WM_PAINT message in the
MainWndProc function. The rectangles are used only as visual
representations of the threads. The threads do, however,
draw the rectangle in the center with their specific color.)

The semaphore has a use count. When it is set to zero, any
thread can access the semaphore and execute the code within
its "semaphore gate" by using WaitForSingleObject. When the
thread gains control of the semaphore using this call, the
use count is incremented by 1. When the thread is done
executing its code, it can call ReleaseSemaphore. This will
decrement the count by whatever value you indicate (this
code uses 1), signaling to any other thread that it may gain
control of the semaphore.

Note: Any thread that has access to the semaphore may
decrement the semaphore's use count with ReleaseSemaphore;
the thread does not have to have control of the semaphore at
the time.

In this code, the WM_CREATE message in MainWndProc creates a
semaphore. The four threads are then created, each waiting
on the semaphore.

Each of the threads loop, blocking on a WaitForSingleObject
call. Once any thread has set the use count to 0, the thread
gets to draw the center rectangle with its color and then
sleeps for half a second before freeing the semaphore again.
The thread then runs through the loop again.

Additional reference words:

CreateSemaphore, WaitForSingleObject, ReleaseSemaphore
