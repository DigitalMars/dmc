Unfortunately the two methods this applications relies
on to tell what platform it is running in did not make
it into the current build of NT at the time this sample
needed to be frozen.

Therefore please use this sample as a guide as how things
will be. In particular *all* Win32 applications will need
to use GetVersion() to tell whether it's on the Win32s or
Win32. GetVersion() will set the high bit of the high word
if it's running on Win32s.

Win16 applications will be able to tell whether there are
running on Win32/NT or Win16/MS-DOS by exmaining the flags
return by GetWinFlags() a unique manifest constant will
be returned if it's running in the WOW layer.
