 Sample: Simple DLL Demonstration

Summary:

The SIMPLDLL sample provides a generic DLL template. Also
included are two small test applications, LINKTEST and
LOADTEST, which demonstrate load-time linking (to a DLL
import library) and dynamic loading, respectively.

More Information:

THE_DLL contains a skeleton DLL (dynamic-linked library)
entry point and five exported functions with varying
parameter lists. A resource file (containing a dialog box
template) is also used.

LINKTEST is a small application that links with the
THE_DLL's import library, and allows the user to make calls
into THE_DLL (via menu item selections).

LOADTEST is a small application that loads THE_DLL at run
time and calls the GetProcAddress function to retrieve the
addresses of THE_DLL's exported functions. Again, the user
is allowed to make calls into THE_DLL.

Additional reference words:

GetModuleFileName, LoadLibrary, GetProcAddress
