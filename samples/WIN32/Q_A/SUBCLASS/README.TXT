 Sample: SUBCLASS Program Demonstration

Summary:

The SUBCLASS sample demonstrates how a program can subclass
standard controls in order to extend their normal
functionality. This sample replaces the window procedure for
buttons, edit fields, and list boxes.

More Information:

The standard subclassing technique is to replace the window
procedure in the window structure by using:

   SetWindowLong (hwnd, GWL_WNDPROC, (LONG) SubclassWndProc);

In the SUBCLASS sample, the old window procedure is also
saved in a structure pointed at by the user data. Thus, any
functionality can be added to various classes of windows
without having to know what the class originally was.

In this sample, the subclass procedure adds the ability to
move and size the control windows when the application is
not in "test mode." When the application is in test mode,
the subclass procedure calls the original window procedure
and the controls behave as normal. Thus, this sample
provides the bare bones for a "dialog editor" type of
program.

Additional reference words: CallWindowProc
