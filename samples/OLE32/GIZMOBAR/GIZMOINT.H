/*
 * GIZMOINT.H
 * GizmoBar Version 1.01
 *
 * Internal definitions for the GizmoBar DLL
 *
 * Copyright (c)1993-1995 Microsoft Corporation, All Rights Reserved
 *
 * Kraig Brockschmidt, Software Design Engineer
 * Microsoft Systems Developer Relations
 *
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  >INTERNET:kraigb@microsoft.com
 */


#ifndef _GIZMOINT_H_
#define _GIZMOINT_H_

#include "gizmobar.h"
#include "win1632.h"
#include "gizmo.h"

#ifdef __cplusplus
extern "C"
    {
#endif


/*
 * The main gizmobar structure itself.  There's only one of these,
 * but it references the first GIZMO in the list.
 */

typedef struct tagGIZMOBAR
    {
    PGIZMO      pGizmos;            //List of gizmos we own.
    HWND        hWnd;               //Window handle of ourselves.
    HINSTANCE   hInst;
    HWND        hWndAssociate;      //Associate who gets messages.
    DWORD       dwStyle;            //Copy of GWL_STYLE
    UINT        uState;             //State flags
    UINT        uID;                //Control ID.

    HBRUSH      hBrFace;            //Static control background color
    COLORREF    crFace;             //Color of hBrFace
    HFONT       hFont;              //Font in use
    BOOL        fEnabled;           //Are we enabled?

    PGIZMO      pGizmoTrack;        //Current pressed button.
    BOOL        fTracking;
    BOOL        fMouseOut;
    } GIZMOBAR, * PGIZMOBAR;

#define CBGIZMOBAR sizeof(GIZMOBAR)


//Extra bytes for the window if the size of a local handle.
#define CBWINDOWEXTRA       sizeof(PGIZMOBAR)

#define GBWL_STRUCTURE      0


//Structure for passing paint info to a gizmo enumeration callback.
typedef struct
    {
    HDC     hDC;
    BOOL    fPaint;
    } PAINTGIZMO, * PPAINTGIZMO;



//Private functions specific to the control.

//INIT.C
BOOL              FRegisterControl(HINSTANCE);
PGIZMOBAR         GizmoBarPAllocate(int *, HWND, HINSTANCE, HWND
                      , DWORD, UINT, UINT);
PGIZMOBAR         GizmoBarPFree(PGIZMOBAR);


//PAINT.C
void              GizmoBarPaint(HWND, PGIZMOBAR);
BOOL WINAPI       FEnumPaintGizmos(PGIZMO, UINT, DWORD);


//GIZMOBAR.C
LRESULT WINAPI    GizmoBarWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    WINAPI    FEnumChangeFont(PGIZMO, UINT, DWORD);
BOOL    WINAPI    FEnumEnable(PGIZMO, UINT, DWORD);
BOOL    WINAPI    FEnumHitTest(PGIZMO, UINT, DWORD);


//API.C  Also see GIZMOBAR.H for others
LRESULT    GBMessageHandler(HWND, UINT, WPARAM, LPARAM, PGIZMOBAR);
PGIZMO     PGizmoFromHwndID(HWND, UINT);


#endif //_GIZMOINT_H_
