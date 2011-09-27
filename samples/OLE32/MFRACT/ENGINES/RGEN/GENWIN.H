//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       genwin.h
//
//  Contents:   definition for the generator control window
//
//  Classes:    CGeneratorWindow
//
//  Functions:
//
//  History:    5-05-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __GENWIN_H__
#define __GENWIN_H__

#include <windows.h>
#include <cwindow.h>
#define MAX_POINTS 51

#ifdef __cplusplus

class CGeneratorWindow: public CHlprWindow
{
public:
    LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void InitializePoints(double * rgRadius, double * rgTheta, unsigned nSegments);
    void RetrievePoints(double * rgRadius, double * rgTheta, unsigned &nSegments);
    void Redraw(void);
private:
    double _rgX[MAX_POINTS];
    double _rgY[MAX_POINTS];
    unsigned _uTrackingPoint;
    unsigned _nPoints;
    unsigned _uWidth, _uHeight;
};

#endif // __cplusplus
#endif // __GENWIN_H__
