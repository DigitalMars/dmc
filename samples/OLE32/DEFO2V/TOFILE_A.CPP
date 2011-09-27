// ansiwrap.cpp
//
// Creates ANSI versions of all modules.  Any exported function get's "A"
// appended.
//
//

#ifdef UNICODE
#undef UNICODE
#endif
#ifdef _UNICODE
#undef _UNICODE
#endif

#include "precompa.h"
#include "tofile.cpp"

