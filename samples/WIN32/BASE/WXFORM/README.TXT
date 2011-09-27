Sample: World Coordinate Transformation

Summary:

The SDK sample named WXFORM provides a demonstration of the
new world-coordinate transformation. This sample displays a
rectangle in world coordinates and a matrix containing the
transform values. Users can directly manipulate the
rectangle and see the effect on the transformation, or they
can set the transformation and see the effect on the
rectangle.

More Information:

The program begins by setting the viewport origin to the
center of the client area. It then draws a rectangle in
world coordinate space from the point (0, 0) to the point
(100, 100). The user can directly manipulate this rectangle
by using the left and right mouse buttons. Specific actions
are described more fully in the "Direct Manipulation Help"
dialog box.

There is a second dialog box titled "World Transform." This
shows the values of the eM11, eM12, eM21, eM22, eDx, and eDy
fields in the XFORM structure retrieved by calling the
GetWorldTransform function. By choosing the buttons on this
dialog box, the user can cause a SetWorldTransform to occur
in the program.

There are three coordinate systems of interest in this
sample. The first one is the world coordinate system, which
is new to Win32. These points are ultimately mapped to the
second coordinate system, device coordinates, before being
painted in the window. This program must also use a third
coordinate system, screen coordinates, for certain
interactions with the mouse pointer.

There is a third dialog box titled "Mouse Position" that
shows the location of the cursor in all three of these
coordinate systems. The device coordinates are relative to
the upper-left corner of the client area. They are not
relative to the viewport origin.

Additional reference words:  ModifyWorldTransform
