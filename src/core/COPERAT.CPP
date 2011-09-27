/*ident	"@(#)coperat.cc	1.3   6/2/90" */

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


complex operator / ( const complex& lhs, const complex& rhs )
{
  double ratio, den;
  if (fabs(rhs.re) >= fabs(rhs.im))
    {
      ratio = rhs.im / rhs.re;
      den = rhs.re + ratio * rhs.im;
      return  complex ( (lhs.re + lhs.im * ratio) / den, 
		       (lhs.im - lhs.re * ratio )/ den );
    }
  else
    {
      ratio = rhs.re / rhs.im;
      den = rhs.im + ratio * rhs.re;
      return  complex ( (lhs.re * ratio + lhs.im ) / den, 
		       (lhs.im * ratio - lhs.re )/ den );
    }
}



complex operator / ( double lhs, const complex& rhs )
{
  double ratio, den;
  if (fabs(rhs.re) >= fabs(rhs.im))
    {
      ratio = rhs.im / rhs.re;
      den = rhs.re + ratio * rhs.im;
      return  complex ( lhs / den, (- lhs * ratio )/ den );
    }
  else
    {
      ratio = rhs.re / rhs.im;
      den = rhs.im + ratio * rhs.re;
      return  complex ( (lhs * ratio ) / den, 
		        - lhs / den );
    }
}

