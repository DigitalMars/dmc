//_ cstrm.cpp
//  (c) Copyright 1991-1993 by Digital Mars
//  All Rights Reserved
//  Written by SST 6/5/91

#if 1

#include <iostream.h>
#include <oldcomplex.h>

ostream& operator<<(ostream& s, const complex& z)
{
    if (s.opfx()) {
        #if 0 /* iosold support */
          long of = s.flags();
          s.unsetf(ios::spacing);
        #endif
        int w = s.width();
        int t = (w-2)/2;
        s.width(0);
        s << '(';
        s.width(t);
        s << z.re << ',';
        s.width(w-2-t);
        s << z.im << ')';
        s.width(w);
        #if 0
          s.flags(of);
        #endif
    }
    s.osfx();
    return s;
}

istream &operator>>(istream &s, complex &v)
{
    if (s.ipfx(0)) {
        long oldf = s.flags(ios::right|ios::skipws);
        double a, b = 0;
        char c = 0;
        s >> c;
        if (c == '(') {
             s >> a >> c;
            if (c == ',')
                s >> b >> c;
            if (c != ')')
                s.clear(ios::failbit|ios::badbit);
        } else {
            s.putback(c);
            s >> a;
        }
        if (s.good())
            v = complex(a,b);
        s.flags(oldf);
    }
    return s;
}

#else // old oldstream version

/*
**  (c) Copyright 1988-1990 by Dyad Software Corp.
**  All Rights Reserved
**
**  Authors: lwd, geb
*/

#include <oldstr\stream.h>
#include <oldcomplex.h>

ostream& operator << ( ostream& s, const complex& z)
{
     return s << "(" << z.re << "," << z.im << ")" ;
}

istream& operator >> ( istream& s, complex& z)
{
     return s >> z.re >> z.im;
}

#endif
