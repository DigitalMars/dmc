MFract is a 32 bit Windows application that uses OLE2's Component
Object Model to provide a mechanism for implementing modular fractal
engines.  (The "M" stands for "Modular".)

Each of MFract's fractal engines are implemented as Inproc Servers
which provide a custom interface called IFractalEngine.  This allows
new fractal engines to be created for MFract without having to
modify or re-compile the application.  Indeed, creating a new fractal
engine doesn't require that the programmer know anything about MFract
beyond the custom interfaces that it uses.

MFract can be compiled as either a Unicode or ANSI application.
Type "nmake unicode" to build a unicode version.  Type "nmake" or
"nmake ansi" to build an ANSI version.  Type "nmake clean" to remove
any target files from the build.

To run MFract (after it has been compiled of course)
-----------------------------------------------------
Step 1: cd bin

Step 2: Run regedit MFract.reg to register the sample app.

Step 3: Run MFract.
