//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       fclass.h
//
//  Contents:   definition for the CFractalWindow class
//
//  Classes:    CFractalWindow
//
//  Functions:
//
//  History:    4-11-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __WFCLASS_H__
#define __WFCLASS_H__

#include "cwindow.h"
#include <commdlg.h>
#include <frhost.h>
#include <frcngn.h>
#include "engnlist.h"
#include <palsize.h>
#ifdef __cplusplus

//+---------------------------------------------------------------------------
//
//  Class:      CFractalWindow
//
//  Purpose:    implements MFract's main window
//
//  Interface:  CFractalWindow       -- constructor
//              ~CFractalWindow      -- destructor
//              InitInstance         -- initializes an instance of the class
//              WindowProc           -- main window procedure
//              DoMenu               -- menu command handler
//              OnCreate             -- called when the window is created
//              NewWindowSize        -- called when window size changes
//              SetUpColorTable      -- initializes the color table
//              SetDefaultColors     -- selects the default color set
//              Wash                 -- creats a wash over a range of colors
//              ToggleTitle          -- toggles full screen mode on/off
//              ChangeFractalEngines -- changes fractal engines
//              CycleStop            -- stops color cycling
//              Faster               -- alters speed of color cycling
//              LoadPalette          -- loads a color palette from a file
//              SavePalette          -- saves a color palette to a file
//              SaveBitmap           -- saves the graph as a bitmap
//              LoadFile             -- loads the graph from a docfile
//              SaveFile             -- saves the graph to a docfile
//              OnStart              -- starts the graph
//              OnStop               -- stops the graph
//              GetHwnd              -- gets the main window handle
//              DrawRect             -- draws the zoom rectangle
//              EraseRect            -- erases the zoom rectangle
//              ClipLine             -- clips a line to the graph dimensions
//
//              IUnknown methods:
//              QueryInterface       -- retreives interface pointers
//              AddRef               -- increments ref-count
//              Release              -- decrements ref-count
//
//              IFractalHost methods:
//              SetGraphSize         -- changes the size of the graph
//              GetGraphSize         -- returns the size of the graph
//              GetPoint             -- returns the color at a point
//              Rect                 -- draws a rectangle
//              Line                 -- draws a line
//              Point                -- plots a point
//              DoneDrawingGraph     -- signals when the graph is complete
//
//  History:    4-11-94   stevebl   Created
//              7-07-94   stevebl   changed definition of GetPoint
//
//----------------------------------------------------------------------------

class CFractalWindow: public CHlprWindow, public IFractalHost
{
public:
    CFractalWindow();
    ~CFractalWindow();
    BOOL InitInstance(HINSTANCE, int);
    LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT DoMenu(WPARAM wParam, LPARAM lParam);
    void OnCreate(void);
    void NewWindowSize(unsigned uWidth, unsigned uHeight);
    void SetUpColorTable(void);
    void SetDefaultColors(void);
    void Wash(int i, PALETTEENTRY rgColors[]);
    void ToggleTitle(void);
    void ChangeFractalEngines(void);
    void CycleStop(void);
    void Faster(int);
    void LoadPalette(void);
    void SavePalette(void);
    void SaveBitmap(void);
    void LoadFile(void);
    void SaveFile(void);
    void OnStart(void);
    void OnStop(void);
    HWND GetHwnd(void);
    BOOL DrawRect(HDC, const RECT *);
    BOOL EraseRect(HDC, const RECT *);
    BOOL ClipLine(int & x1, int & y1, int & x2, int & y2);

    // IUnknown members
    STDMETHOD(QueryInterface) (THIS_ REFIID, void **);
    STDMETHOD_(ULONG, AddRef) (THIS);
    STDMETHOD_(ULONG, Release) (THIS);

    // IFractalHost members
    STDMETHOD(SetGraphSize) (THIS_ unsigned uWidth, unsigned uHeight);
    STDMETHOD(GetGraphSize) (THIS_ unsigned * puWidth, unsigned * puHeight);
    STDMETHOD(GetPoint) (THIS_ unsigned * puValue, int x, int y);
    STDMETHOD(Rect)
        (THIS_
        int iLeft,
        int iTop,
        int iRight,
        int iBottom,
        unsigned uColor);
    STDMETHOD(Line)
        (THIS_
        int x1,
        int y1,
        int x2,
        int y2,
        unsigned uColor);
    STDMETHOD(Point)
        (THIS_
        int x,
        int y,
        unsigned uColor);
    STDMETHOD(DoneDrawingGraph)(THIS_ void);

private:
    HDC         _hdcMem;
    ULONG       _uRefreshRate;
    HANDLE      _hTitleEvent;
    HANDLE      _hGraphStartedEvent;
    HPALETTE    _hpal;
    HBITMAP     _hbmSavedBitmap;
    HBITMAP     _hbmMainWindow;
    unsigned    _uScreenWidth;
    unsigned    _uScreenHeight;
    unsigned    _uImageWidth;
    unsigned    _uImageHeight;
    unsigned    _uWindowWidth;
    unsigned    _uWindowHeight;
    unsigned    _uScrollPosX;
    unsigned    _uScrollPosY;
    int         _iCycleVelocity;
    int         _iCurrentDirection;
    double      _dLeft, _dRight, _dTop, _dBottom;
    PALETTEENTRY * _pEntry1, * _pEntry2;
    LOGPALETTE * _pLogPal;
    LOGPALETTE * _pSecondLogPal;
    ULONG       _uRefCount;
    CEngineList * _pEngineList;
    unsigned    _uFractalType;
    IFractalEngine * _pFractalEngine;
    RECT        _rectZoom;
    BOOL        _fZooming;
    BOOL        _fRButtonDown;
    OPENFILENAME _ofnPalette;
    OPENFILENAME _ofnBitmap;
    OPENFILENAME _ofnFile;
    TCHAR       _szPalFileName[MAX_PATH];
    TCHAR       _szPalFileTitle[MAX_PATH];
    TCHAR       _szBmpFileName[MAX_PATH];
    TCHAR       _szBmpFileTitle[MAX_PATH];
    TCHAR       _szFileName[MAX_PATH];
    TCHAR       _szFileTitle[MAX_PATH];
    TCHAR       _szHelpFile[MAX_PATH];
    RECT        _rectPainted;
    IStorage  * _pstg;
    char      * _rgPixels;
    BOOL        _fReRealizePalette;
    BOOL        _fCanAnimate;
    CRITICAL_SECTION _csGraph;
};

#endif // __cplusplus

#define REFRESH_TIMER 1
#define CYCLE_TIMER 2
#define CYCLE_RATE 50

#define QUICKWIDTH 160
#define QUICKHEIGHT 120

#endif // __WFCLASS_H__
