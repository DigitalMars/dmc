Sample: MaskBlt Function Demonstration

Summary:

The MASKBLT sample is an easy-to-use demonstration of the
MaskBlt Win32 API function. The program presents a dialog
box on the top of the window; through this dialog box the
user can manipulate the parameters to MaskBlt. In the main
window, the source bitmap is displayed in the center third
of the window, the monochrome bitmap mask is displayed in
the right third of the window, and the destination bitmap is
displayed on the left.

More Information:

The destination rectangle may be changed directly in the
dialog box, or it may be changed by using the direct
manipulation object in the left third of the window.
Clicking and dragging the mouse in the upper-left corner
moves the rectangle; clicking and dragging the mouse in the
lower-right corner sizes the rectangle. The function
requires only a starting point (not a rectangle) for the
source and mask bitmaps. There is one additional direct
manipulation object for the source and one for the mask.
These objects may be moved by clicking and dragging with the
mouse.

The raster operation for the MaskBlt call may be changed by
altering the values in the right most entry fields. The
contents are interpreted to be in hexadecimal. There is a
combo box directly beneath these entry fields that lists all
of the standard raster operations. If the user selects a
standard ROP from this combo box, its contents are copied
into the ROP entry fields and are then used in the MaskBlt
call.

This sample provides clipboard support in the following manner.
Hitting <ctrl>+<insert> will copy the destination image into the
clipboard.  Hitting <shift>+<insert> will copy a bitmap from the
clipboard into the source region.  Hitting <alt>+<insert> will
do both; the destination image will be copied into the clipboard
and then down to the source region.
