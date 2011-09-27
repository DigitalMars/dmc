Sample: PolyDraw Function Demonstration

Summary:

The POLYDRAW SDK sample provides an easy-to-use
demonstration of how the PolyDraw Win32 API function works.
The user can place points in the window with the left mouse
button, and move these points by dragging with the same
mouse button. The PolyDraw curve is drawn dynamically to
follow the position of the new points.

More Information:

To use this program, click the left mouse button at
miscellaneous places in the client area. A Polyline call
shows exactly where the points were put. By default, the
type entered into the type array is PT_LINETO. This can be
changed to a PT_MOVETO type by holding down the SHIFT key.
It can be changed to a PT_BEZIERTO type by holding down the
CTRL key. The resulting purple curve shows the results.
There will be no curve when the bezier points do not come in
groups of 4, 7, ... , (3n+1).
