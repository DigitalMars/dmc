Sample: Primitive Drag & Drop Unicode Input Method

Summary:

The UNIPUT sample provides a primitive, mouse based, input
method.  It will allow the user to grab any character covered
by a unicode font, and drag the character to a second window.
If the user drops the character on the second window, that
window is sent a WM_CHAR message with the appropriate key code.

More Information:

The status bar at the bottom of the window shows three fields
of information.  On the left is the title of the last window
to receive a WM_CHAR.  In the center, is the type of that window,
either Unicode or Ansi.  On the right is a list of the most
recently dropped characters... sort of a history buffer.

There is an online help file with more information.

Notice:

There is a font distributed on the Win32 SDK disk which covers
more than one thousand unicode characters.  In order to use this
application to its full potential, you will want to install that
font using the Control Panel, Fonts item.  The font is named
UCLUCIDA.TTF.
