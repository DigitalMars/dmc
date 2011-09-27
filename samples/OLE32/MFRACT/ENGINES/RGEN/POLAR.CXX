//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       polar.cxx
//
//  Contents:   routines for converting between polar and cartesian coordinate
//              systems
//
//  Classes:
//
//  Functions:  PolarFromCartesian
//              CartesianFromPolar
//
//  History:    5-04-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include "polar.h"
#include <math.h>

//+---------------------------------------------------------------------------
//
//  Function:   CartesianFromPolar
//
//  Synopsis:   converts from polar coordinates to cartesian coordinates
//
//  Arguments:  [x]     - [out] x position
//              [y]     - [out] y position
//              [r]     - [in] radius
//              [theta] - [in] angle in radians
//
//  Returns:    nothing
//
//  History:    5-04-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CartesianFromPolar(double & x, double & y, double r, double theta)
{
    x = r * cos(theta);
    y = r * sin(theta);
}

//+---------------------------------------------------------------------------
//
//  Function:   PolarFromCartesian
//
//  Synopsis:   converts from cartesian to polar coordinates
//
//  Arguments:  [r]     - [out] radius
//              [theta] - [out] angle in radians
//              [x]     - [in] x position
//              [y]     - [in] y position
//
//  Returns:    nothing
//
//  History:    5-04-94   stevebl   Created
//
//----------------------------------------------------------------------------

void PolarFromCartesian(double & r, double & theta, double x, double y)
{
    r = sqrt(x * x + y * y);
    if (r == 0)
    {
        theta = 0;
        return;
    }
    if (y > 0)
    {
        theta = acos(x / r);
    }
    else
    {
        // acos(0) == pi / 2
        // I'm using the trig functions instead of a constant
        // to ensure that the conversion functions use the
        // same values when going in both directions.
        theta = 4.0 * acos(0) - acos(x / r);
    }
}


