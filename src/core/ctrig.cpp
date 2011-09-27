/*ident	"@(#)ctrig.cc	1.3   6/2/90" */

/*
 **  Copyright (c) 1988-1990 Dyad Software Corp.
 **  All Rights Reserved
 **
 **  Authors: lwd, geb
 */


/*  References:  
 **         Abramowitz and Stegun 1972 
 **         Handbook of Mathematical Functions
 **         (National Bureau of Standards)
 **
 **         Eshbach 1975
 **         Handbook of Engineering Fundamentals
 **         (John Wiley and Sons)
 **
 **         ACM 1987
 **         Collected Algorithms from ACM
 **         (Association for Computing Machinery, Inc.)
 */

#include <oldcomplex.h>

#if 0
double asinh(const double& x)
{
  return double (log(x + sqrt(x*x +1)));
}

double acosh(const double& x)
{
  return double (log(x + sqrt(x*x -1)));
}

double atanh(const double& x)
{
  return double (log((1+x) / (1-x))/2.);
}

complex asin(const complex& z)    
{
  complex x = log ( complex (0,1) * z + sqrt(complex(1,0) - z*z));
  return complex ( x.im, -x.re );
}   

complex acos(const complex& z)   
{
  complex x = log ( z + complex (0,1) * sqrt(complex(1,0) - z*z));
  return complex ( x.im, -x.re );
}   
complex atan(const complex& z)   
{
  complex x = log((z + complex(0,1))/(-z + complex(0,1)));
  return complex ( -x.im/2, x.re/2 );
}

#endif









