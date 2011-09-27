//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       polar.h
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

#ifndef __POLAR_H__
#define __POLAR_H__

#ifdef __cplusplus

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

void CartesianFromPolar(double & x, double & y, double r, double theta);

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

void PolarFromCartesian(double & r, double & theta, double x, double y);

#endif // __cplusplus

#endif // __POLAR_H__
