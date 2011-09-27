 Sample: StretchBlt Demonstration

Summary:

The STREBLT sample is an easy to use demonstration of the
StretchBlt API function. The program presents a dialog box
on the top of the window, and through this dialog box the
user can manipulate the parameters to StretchBlt. In the
main window, the source bitmap is displayed on the right
half of the window, and the destination bitmap is displayed
on the left half.

More Information:

The source and destination rectangles may be changed
directly in the dialog, or they may be changed by using the
direct manipulation objects in the two halves of the window.
Clicking and dragging the mouse in the upper-left corner
moves the rectangles; clicking and dragging the mouse in the
lower-right corner sizes the rectangles. The source direct
manipulation object is temporarily erased before calling
StretchBlt so that the top and left edges do not show in the
destination image.

The raster operation for the StretchBlt call may be changed
by altering the values in the right-most entry fields. The
contents are interpreted to be in hexadecimal. There is a
combo box directly beneath these entry fields that lists all
of the standard raster operations. If the user selects a
standard ROP from this combo box, its contents are copied
into the ROP entry fields and are then used in the
StretchBlt call.

Several of the raster operations make use of a pattern in
the destination HDC. For this reason, the program also
allows the user to select one of the standard pattern
brushes from a second combo box. This brush is selected into
the destination HDC just prior to making the StretchBlt
call.

The effect of the StretchBlt call is also affected by the
"StretchBlt mode" that has been set for the destination HDC.
A third combo box allows the user to select from any of the
standard modes. The difference is most easily observed when
stretching from a large source rectangle to a small
destination rectangle.

The "Draw" button may be chosen at any time to cause the
StretchBlt call to be made. This does not erase the
background, so that the effect of multiple ROPs on the HDC
can be observed. Manipulating the source rectangle also
causes a StretchBlt to occur without erasing the window.
However, manipulating the destination rectangle erases the
destination half of the window before the next StretchBlt is
called.
