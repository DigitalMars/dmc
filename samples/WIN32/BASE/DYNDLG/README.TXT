Sample: Dynamic dialog creation

Summary:

Usually an application will define a dialog template in an
RC or DLG file, then create a dialog at run time from that.
However, it is possible to create the dialog template at run
time.  This sample shows how to do this for Win32.

More Information:

The dialog template in Win32 is conceptually similar to
the Windows 3.1 version, but is different enough to require
some porting work.  For example, any strings are now stored
in unicode.  The dialog template format is documented in the
associated help file.

==>> RUN-TIME DEPENDENCY!!!
==>> Build the SPINCUBE sample before executing dyndlg.

One of the dialogs uses a custom control which is expected to
be defined in the spincube.dll file.  The dyndlg sample tries to
load this DLL at run time in the following location:
    "..\spincube\spincube.dll"
I.e. in a directory of the same level, but named spincube. This
can obviously be trivially changed, and the sample recompiled.
Spincube is another sample distributed on the Win32 SDK.
