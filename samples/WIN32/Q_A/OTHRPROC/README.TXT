 Sample: Sharing Named Memory Between Two Processes

Summary:

SHAREMEM and OTHRPROC are two samples which work together to
demonstrate sharing named memory between two separate
processes.  These samples must be run together.

More Information:

To use: first start an instance of SHAREMEM.  A window will
appear, devided into an upper and lower section.  The upper
section will have two edit fields: one displaying the mouse
pointer's X-coordinates, and the other the pointer's Y
coordinates, as the mouse moves in the lower section.

Start an instance of OTHRPROC.  Allow OTHRPROC to keep
focus, but move the pointer around over the SHAREMEM'S
window.  OTHRPROC has a similar window configuration;
however, you will notice that the X and Y coordinates of the
mouse as it moves over the SHAREMEM window are the values
that appear in the edit fields of OTHRPROC.  To emphasize
this, a cross hair will appear in OTHRPROC's lower section;
its movements relative to the mouse position in SHAREMEM's
window.

What's happening: When SHAREMEM is started, it creates a
allocates a piece of named shared memory the size of a DWORD
(the size needed to hold the mouse cursor's X and Y
coordinates) using CreateFileMapping.  As the mouse pointer
is moved across the window, the WM_MOUSEMOVE messages are
trapped, and the coordinates are written to the upper edit
fields and to the piece of named shared memory.

When OTHRPROC is started, it gets access to the named shared
memory by calling OpenFileMapping and MapViewOfFile.
OTHRPROC then uses a thread to poll and read the X and Y
coordinates written to the shared memory by SHAREMEM.  It
captures the coordinates and draws a bit map of a cross hair
in the specified location.
