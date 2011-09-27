Sample: World Coordinate Transforms


Summary:

The WORLD sample demonstrates how an image can be translated and scaled
using SetWorldTransform(). The image is read from the metafile.

After the application is started, go to the Metafile menu and select
"Open" in order to specify the metafile which should be used. The image
is displayed and the horizontal and vertical thumbs are set to the
middle of the scrollbars. Use the scrollbars to translate the image. In
order to scale the image, choose the Scale menu option. Select the
desired horizontal and vertical scaling factors from the listboxes.


More Information:

The sample metafile, SAMPLE.EMF, was created using the code from the
Overviews manual, in Chapter 74: Metafiles. PlayEnhMetafile() is used
in the WM_PAINT case to display the image stored in the metafile.

SetWorldTransform() takes two parameters: a device context and a
structure which defines the transform to be applied. The transform
contains 6 fields, which have the following use in this program:

   eM11  :  Horizontal Scaling Factor
   eM12  :  Not Used
   eM21  :  Not Used
   eM22  :  Vertical Scaling Factor
   eDx   :  Horizontal Translation
   eDy   :  Vertical Translation

The WM_HSCROLL and WM_VSCROLL cases are handled pretty much as they
are in any of the other samples, except that ScrollWindow() is not used
to update the window contents. Instead, the appropriate modification is
made to the transform and InvalidateRect() is called to update the
window.

When the OK button is selected in the Scale Image dialog, the transform
is updated and InvalidRect() is called to update the window.

