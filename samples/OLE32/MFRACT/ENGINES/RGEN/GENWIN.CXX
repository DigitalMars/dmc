//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       genwin.cxx
//
//  Contents:   implementation for the generator window control
//
//  Classes:    CGeneratorWindow
//
//  Functions:
//
//  History:    5-05-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include "genwin.h"
#include <math.h>
#include "polar.h"

#define POINTRADIUS 2

//+---------------------------------------------------------------------------
//
//  Member:     CGeneratorWindow::WindowProc
//
//  Synopsis:   Handles mouse messages and painting.
//
//  Arguments:  [uMsg]   - message
//              [wParam] - first message parameter
//              [lParam] - second message parameter
//
//  History:    5-05-94   stevebl   Created
//
//----------------------------------------------------------------------------

LRESULT CGeneratorWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        {
            SetCapture(_hwnd);

            // translate mouse coordinates to real coordinates
            double x, y;
            x = ((double) (LOWORD(lParam) * 2.0) / _uWidth) - 1.0;
            y = ((double) (HIWORD(lParam) * -2.0) / _uHeight) + 1.0;

            // Find if we need to create a new point or if we are going to track
            // an old point.

            // Distances used here are really distances squared
            // to keep us from having to do a sqrt function.

            unsigned uClosest = 1;
            double dClosestDistance = 10.0;
            // an insanely huge number given our graph dimensions of 2 x 2

            // start with point one and go to point _nPoints - 2
            // since the end points are not moveable

            unsigned uIndex = 1;
            double dTestDistance =
                ((double)(4 * POINTRADIUS) / _uWidth) *
                ((double)(4 * POINTRADIUS) / _uHeight);
            while (uIndex < _nPoints - 1)
            {
                double dDistance =
                    (x - _rgX[uIndex]) * (x - _rgX[uIndex]) +
                    (y - _rgY[uIndex]) * (y - _rgY[uIndex]);
                if (dTestDistance >= dDistance)
                {
                    // we clicked on a point (or at least close enough to it)
                    _uTrackingPoint = uIndex;
                    return(FALSE);
                }
                if (dDistance < dClosestDistance)
                {
                    dClosestDistance = dDistance;
                    uClosest = uIndex;
                }
                uIndex++;
            }
            // If we haven't returned yet, then we didn't click on a point.
            // Therefore we must be inserting a new point.

            // Determine if we can add any more points to the list.
            if (MAX_POINTS == _nPoints)
            {
                // Nope.  Better just track the closest point then.
                _uTrackingPoint = uClosest;
                return(FALSE);
            }

            // Next determine if we should insert behind or in front.
            if (_nPoints > 2)
            {
                // We always insert in front if there are only two points.
                // Otherwise, a simple distance check will help us decide which
                // segment to insert in.
                double d1 =
                    (x - _rgX[uClosest - 1]) * (x - _rgX[uClosest - 1]) +
                    (y - _rgY[uClosest - 1]) * (y - _rgY[uClosest - 1]);
                double d2 =
                    (x - _rgX[uClosest + 1]) * (x - _rgX[uClosest + 1]) +
                    (y - _rgY[uClosest + 1]) * (y - _rgY[uClosest + 1]);
                if (d2 < d1)
                {
                    // insert after
                    ++uClosest;
                }
            }

            // Then insert the point into the list of points.

            for (uIndex = _nPoints; uIndex > uClosest; uIndex--)
            {
                _rgX[uIndex] = _rgX[uIndex - 1];
                _rgY[uIndex] = _rgY[uIndex - 1];
            }
            _rgX[uClosest] = x;
            _rgY[uClosest] = y;
            _uTrackingPoint = uClosest;
            _nPoints++;

            // And cause the window to be repainted.
            InvalidateRect(_hwnd, NULL, TRUE);
        }
        break;
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON)
        {
            // translate mouse coordinates to real coordinates
            double x, y;
            x = ((double) (LOWORD(lParam) * 2.0) / _uWidth) - 1.0;
            y = ((double) (HIWORD(lParam) * -2.0) / _uHeight) + 1.0;

            // Move whatever point we are tracking
            _rgX[_uTrackingPoint] = x;
            _rgY[_uTrackingPoint] = y;

            // And cause the window to be repainted.
            InvalidateRect(_hwnd, NULL, TRUE);
        }
        break;
    case WM_LBUTTONUP:
        // If the point we have been tracking has moved off the screen, remove it from the list.
        if (_rgX[_uTrackingPoint] < -1.0
            || _rgX[_uTrackingPoint] > 1.0
            || _rgY[_uTrackingPoint] < -1.0
            || _rgY[_uTrackingPoint] > 1.0
            )
        {
            _nPoints--;
            for (unsigned uIndex = _uTrackingPoint; uIndex < _nPoints; uIndex++)
            {
                _rgX[uIndex] = _rgX[uIndex + 1];
                _rgY[uIndex] = _rgY[uIndex + 1];
            }
        }

        // And cause the window to be repainted.
        InvalidateRect(_hwnd, NULL, TRUE);
        ReleaseCapture();
        break;
    case WM_SIZE:
        _uWidth = LOWORD(lParam);
        _uHeight = HIWORD(lParam);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT rc;
            HDC hdc = BeginPaint(_hwnd, &ps);
            if (GetClipBox(ps.hdc, &rc) != NULLREGION)
            {
                // paint the background grid
                HPEN hpen = (HPEN) CreatePen(PS_DOT, 0, WHITENESS);
                hpen = (HPEN) SelectObject(ps.hdc, hpen);

#define NUMDIVISIONS 12

                int i1, i2;
                for (i1 = NUMDIVISIONS-1; i1--;)
                {
                    i2 = (_uHeight * (i1 + 1)) / NUMDIVISIONS;
                    MoveToEx(ps.hdc, 0, i2, NULL);
                    LineTo(ps.hdc, _uWidth, i2);
                    i2 = (_uWidth * (i1 + 1)) / NUMDIVISIONS;
                    MoveToEx(ps.hdc, i2, 0, NULL);
                    LineTo(ps.hdc, i2, _uHeight);
                }

                // The vertical lines

                hpen = (HPEN) SelectObject(ps.hdc, hpen);
                DeleteObject(hpen);

                // paint the generator
                hpen = (HPEN) GetStockObject(WHITE_PEN);
                HBRUSH hbrush = (HBRUSH) GetStockObject(WHITE_BRUSH);
                hpen = (HPEN) SelectObject(ps.hdc, hpen);
                hbrush = (HBRUSH) SelectObject(ps.hdc, hbrush);

                POINT rgPt[MAX_POINTS];
                for (unsigned uIndex = 0; uIndex < _nPoints; uIndex++)
                {
                    rgPt[uIndex].x = (long)(((_rgX[uIndex] + 1.0) * _uWidth) / 2);
                    rgPt[uIndex].y = (long)(((_rgY[uIndex] - 1.0) * _uHeight) / -2);
                    Ellipse(
                        ps.hdc,
                        rgPt[uIndex].x - POINTRADIUS,
                        rgPt[uIndex].y - POINTRADIUS,
                        rgPt[uIndex].x + POINTRADIUS,
                        rgPt[uIndex].y + POINTRADIUS);
                }
                Polyline(ps.hdc, rgPt, _nPoints);

                hbrush = (HBRUSH) SelectObject(ps.hdc, hbrush);
                hpen = (HPEN) SelectObject(ps.hdc, hpen);
                DeleteObject(hbrush);
                DeleteObject(hpen);
            }
            EndPaint(_hwnd, &ps);
        }
        break;
    default:
        return(DefWindowProc(_hwnd, uMsg, wParam, lParam));
    }
    return(FALSE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CGeneratorWindow::InitializePoints
//
//  Synopsis:   sets up internal list of points
//
//  Arguments:  [rgRadius]  - array of radius values
//              [rgTheta]   - array of angles
//              [nSegments] - number of segments in the lists
//
//  Modifies:   _rgX, _rgY, and _nPoints
//
//  History:    5-05-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CGeneratorWindow::InitializePoints(
    double * rgRadius,
    double * rgTheta,
    unsigned nSegments)
{
    _rgX[0] = -0.5;
    _rgY[0] = 0;
    for (unsigned index = 0; index < nSegments-1; index ++)
    {
        CartesianFromPolar(
            _rgX[index + 1],
            _rgY[index + 1],
            rgRadius[index],
            rgTheta[index]);
        _rgX[index + 1] += _rgX[index];
        _rgY[index + 1] += _rgY[index];
    }

    // guarantee the position of the last point
    _rgX[nSegments] = 0.5;
    _rgY[nSegments] = 0;
    _nPoints = nSegments + 1;
}

//+---------------------------------------------------------------------------
//
//  Member:     CGeneratorWindow::RetrievePoints
//
//  Synopsis:   returns a new list of segments built from the point list
//
//  Arguments:  [rgRadius]  - [out] array of radius values
//              [rgTheta]   - [out] array of angles
//              [nSegments] - [out] number of segments
//
//  History:    5-05-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CGeneratorWindow::RetrievePoints(
    double * rgRadius,
    double * rgTheta,
    unsigned &nSegments)
{
    nSegments = _nPoints - 1;
    for (unsigned index = 0; index < nSegments - 1; index++)
    {
        PolarFromCartesian(
            rgRadius[index],
            rgTheta[index],
            _rgX[index + 1] - _rgX[index],
            _rgY[index + 1] - _rgY[index]);
    }
    // guarantee the position of the last point
    PolarFromCartesian(
        rgRadius[nSegments - 1],
        rgTheta[nSegments - 1],
        0.5 - _rgX[nSegments - 1],
        0.0 - _rgY[nSegments - 1]);
}

//+---------------------------------------------------------------------------
//
//  Member:     CGeneratorWindow::Redraw
//
//  Synopsis:   causes the generator control to repaint itself
//
//  History:    5-05-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CGeneratorWindow::Redraw(void)
{
    InvalidateRect(_hwnd, NULL, TRUE);
}

