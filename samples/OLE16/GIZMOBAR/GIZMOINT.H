/*
 * GIZMOINT.H
 * GizmoBar Version 1.00, March 1993
 *
 * Internal definitions for the GizmoBar DLL
 *
 * Copyright (c)1992 Microsoft Corporation, All Rights Reserved
 *
 * Kraig Brockschmidt, Software Design Engineer
 * Microsoft Systems Developer Relations
 *
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  >INTERNET:kraigb@microsoft.com
 */


#ifndef _GIZMOINT_H_
#define _GIZMOINT_H_

#include "..\include\bttncur.h"
#include "..\include\win1632.h"
#include "gizmo.h"
#include "gizmobar.h"

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
	HWND        hWndAssociate;      //Associate window handle who gets messages.
	DWORD       dwStyle;            //Copy of GetWindowLong(hWnd, GWL_STYLE)
	UINT        uState;             //State flags

	HBRUSH      hBrFace;            //Static control background color
	COLORREF    crFace;             //Color of hBrFace
	HFONT       hFont;              //Font in use, defaults to system, WM_SETFONT
	BOOL        fEnabled;           //Are we enabled?

	PGIZMO      pGizmoTrack;        //Current pressed button.
	BOOL        fTracking;
	BOOL        fMouseOut;
	} GIZMOBAR, FAR *PGIZMOBAR;

#define CBGIZMOBAR sizeof(GIZMOBAR)


//Extra bytes for the window if the size of a local handle.
#define CBWINDOWEXTRA       sizeof(PGIZMOBAR)

#define GBWL_STRUCTURE      0


//Structure for passing paint information to a gizmo enumeration callback.
typedef struct
	{
	HDC     hDC;
	BOOL    fPaint;
	} PAINTGIZMO, FAR * PPAINTGIZMO;



//Private functions specific to the control.

//INIT.C
//HANDLE FAR PASCAL LibMain(HINSTANCE, WORD, WORD, LPSTR);
BOOL              FRegisterControl(HINSTANCE);
PGIZMOBAR         GizmoBarPAllocate(LPINT, HWND, HINSTANCE, HWND, DWORD, UINT);
PGIZMOBAR         GizmoBarPFree(PGIZMOBAR);


//PAINT.C
void              GizmoBarPaint(HWND, PGIZMOBAR);
BOOL FAR PASCAL   FEnumPaintGizmos(PGIZMO, UINT, DWORD);


//GIZMOBAR.C
LRESULT FAR PASCAL GizmoBarWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    FAR PASCAL FEnumChangeFont(PGIZMO, UINT, DWORD);
BOOL    FAR PASCAL FEnumEnable(PGIZMO, UINT, DWORD);
BOOL    FAR PASCAL FEnumHitTest(PGIZMO, UINT, DWORD);


//API.C  Also see GIZMOBAR.H for others
LRESULT    GBMessageHandler(HWND, UINT, WPARAM, LPARAM, PGIZMOBAR);
PGIZMO     PGizmoFromHwndID(HWND, UINT);


#endif //_GIZMOINT_H_
