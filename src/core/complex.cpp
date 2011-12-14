
/*ident	"@(#)complex.cc	1.3   6/2/90" */

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

#include <iostream.h>
#include <oldcomplex.h>

#if !__INLINE_8087
extern "C"
{
double		__CLIB sqrt(double);
}
#endif

double abs( const complex& z)
{
  double ret;
  double x = fabs(z.re);   
  double y = fabs(z.im);

  if (x==0) 
    ret = y;
  else if(y==0) 
    ret = x;
  else
    {
	if(x>y) 
		ret = x * sqrt(1 + pow(y/x,2.0));
	else 
		ret = y * sqrt(1 + pow(x/y,2.0));
    }
  return ret;
}

complex sqrt(const complex& z) 
{
  complex ret;
  double a, b;
  if (z.re == 0 && z.im == 0 ) 
    ret = complex (0,0);
  else
    {
      a = sqrt ((fabs(z.re) + abs(z))*0.5);
      if (z.re >= 0 ) 
      	b = z.im / (a+a);
      else
		{
          b = z.im < 0 ? -a: a;
          a = z.im / (b+b);
		}
    }
  ret = complex(a,b);
  return ret;
}


complex pow ( const complex& lhs, const complex& rhs) 
{
  if (rhs == 0 ) 
    return complex (1,0);
  else if (rhs.im == 0 ) 
    return pow( lhs, rhs.re);
  else
    {
      complex logf, phase;
      logf.re = log(abs(lhs));   
      logf.im = atan2(lhs.im, lhs.re);
      phase.re = exp( logf.re * rhs.re - logf.im * rhs.im);
      phase.im = logf.re * rhs.im + logf.im * rhs.re;
      return complex ( phase.re * cos(phase.im), phase.re * sin(phase.im));
    }
}

complex pow ( const complex& lhs, int rhs) 
{
  complex z,z2;

  if (rhs == 0 ) 
    z = complex (1,0);
  else
    {
    z2 = (rhs < 0) ? (1 / lhs) : lhs;
	z = z2;
	for (int i = 1; i < abs(rhs); i++) 
		z = z*z2;
    }
  return z;
}

complex pow ( const complex& lhs, double rhs )
{                                                  
  double theta = arg(lhs)*rhs;
  double r = modulus(lhs);
  
  r = pow(r,rhs);
  return complex( r*cos(theta), r*sin(theta) );
}

complex pow( double lhs, const complex & rhs)
{
  return pow(complex(lhs,0),rhs);
}

