Using Vertex Arrays for Improved Performance


The VASPHERE sample demonstrates the use of vertex-arrays, available
in OpenGL 1.1. A significant performance improvement can be gained
if objects in the scene are drawn using vertex-arrays instead of
other primitives. In this sample, a lit revolving sphere is drawn
using one of the following methods

 - Vertex-arrays
 - Triangles
 - Triangle-Strips

In addition, either immediate mode or display-list mode are used. Cycle
through the three primitives by pressing the space-bar, and toggle between
immediate-mode and display-list mode by pressing the 'd' key.
The window title shows the triangle/sec figure.

Note
----

VASPHERE will not run on an implementation of OpenGL version 1.0. The OpenGL
for Windows 95 redistributables included on the Win32 SDK are an
implementation of version 1.0.
