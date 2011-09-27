#include <iosold\iomdefs.h>
#include <typeinfo.h>

#if Aiostfx
#define __IOStreams_Expand_Template_Functions
#include <iosold\iomanip.h>
#endif

#if Aiosman1
ios &_iomanip_setfill(ios &s, int c)
{
    s.fill(c);
    return s;
}
#endif

#if Aiosman2
ios &_iomanip_setw(ios &s, int w)
{
    s.width(w);
    return s;
}
#endif

#if Aiosman3
ios &_iomanip_setprecision(ios &s, int p)
{
    s.precision(p);
    return s;
}
#endif

#if Aiosman4
ios &_iomanip_resetiosflags(ios& s, long v)
{
    s.unsetf(v);
    return s;
}
#endif

#if Aiosman5
ios &_iomanip_setiosflags(ios& s, long v)
{
    s.flags(v|s.flags());
    return s;
}
#endif

#if Aiosman6
// cf. iostream - Rev 4 - X3J16/92-0059 - p. 27
ios &_iomanip_setbase(ios& s, int n)
{
	long b;
	
	switch(n) {
		case 10:
			b = ios::dec;
			break;
		case 8:
			b = ios::oct;
			break;
		case 16:
			b = ios::hex;
			break;
		default:
			b = 0L;
	}
	s.setf(b,ios::basefield);
	return s;
}
#endif
