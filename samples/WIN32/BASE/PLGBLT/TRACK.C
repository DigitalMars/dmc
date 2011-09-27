
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/**************************************************************************\
*  track.c -- support for direct manipulation of parallelogram object.
\**************************************************************************/

#include <windows.h>
#include <math.h>
#include "track.h"

#define EPSILON  (float) 0.0001
#define RECTSIZE  60



/**************************************************************************\
*
*  function:  doTrackObject()
*
*  input parameters:
*   pto -  pointer to a track object.
*   msg -  message selecting what action to take.  Values may include WM_*'s
*           (see case statements below for more information.)
*   hwnd - Window handle for the window the track object exists within.
*   lParam - Usually fourth param to window proc. varies based on msg.
*
*  global variables:  none.
*
*  coordinate spaces:  There are three coordinate spaces of interest here,
*   and this routine is frequently switching between them...
*
*           WORLD                   DEVICE                  SCREEN
*
*      object coordinates       input mouse pos       used w/ SetCursorPos()
*         (pto->rect)          (lParam for WM_*)
*
*             ----->  LPtoDP() ---->    ----> ClientToScreen() -->
*             <-----  DPtoLP() <----    <---- ScreenToClient() <--
*
*   in addition, the HDC has an offset origin.  Device coordinates for the
*   mouse (lParam) never take this into account, but it is necessary to
*   translate them in order to get direct manipulation right.
*
\**************************************************************************/
PTrackObject doTrackObject(PTrackObject pto, int msg, HWND hwnd, LONG lParam)
{
  if ((pto == NULL) && (msg != TROB_NEW))  return NULL;

  switch (msg) {


    /**********************************************************************\
    *  TROB_NEW
    *
    * Allocate new PTrackObject structure.  Fill in default values
    *  for the fields of the structure.  Set up the HDC correctly.
    * return - pointer to the new object.
    \**********************************************************************/
    case  TROB_NEW: {
        PTrackObject  pto;

        /* with LPTR returned value is a pointer. */
        pto = (PTrackObject) LocalAlloc (LPTR, sizeof (TrackObject));

        /* initialize the HDC and other fields. */
        pto->hdc = GetDC (hwnd);
        SetGraphicsMode (pto->hdc, GM_ADVANCED);
        SetROP2(pto->hdc, R2_NOT);
        SelectObject (pto->hdc, GetStockObject (NULL_BRUSH));
        pto->Mode = TMNONE;
        pto->allowedModes = TMMOVE | TMSIZEXY;

        GetWorldTransform (pto->hdc, &(pto->xfmChange));

        /* initialize the size. */
        pto->rect.top = pto->rect.left = 0;
        pto->rect.bottom = pto->rect.right = RECTSIZE;

        return (pto);
    }



    /**********************************************************************\
    *  TROB_DELETE
    *
    * Complement of TROB_NEW.  Free up the memory allocated for the object.
    \**********************************************************************/
    case  TROB_DELETE:
        doTrackObject (pto, TROB_PAINT, hwnd, lParam);
        ReleaseDC (hwnd, pto->hdc);
        LocalFree (LocalHandle ((LPSTR)pto));
    return NULL;



    /**********************************************************************\
    *  TROB_PAINT
    *
    * Paint the object into its hdc.  Called half the time to erase
    *  the object, and half the time to redraw it.
    \**********************************************************************/
    case TROB_PAINT: {
        MoveToEx (pto->hdc, pto->rect.right, pto->rect.top, NULL);
        LineTo (pto->hdc,  pto->rect.left, pto->rect.top);
        LineTo (pto->hdc,  pto->rect.left, pto->rect.bottom);

        if (pto->allowedModes & TMSIZEXY) {
          LineTo (pto->hdc,  pto->rect.right, pto->rect.bottom);
          LineTo (pto->hdc,  pto->rect.right, pto->rect.top);
        }

        if (pto->allowedModes & TMROTATE) {
          MoveToEx (pto->hdc, pto->rect.left, pto->rect.bottom/ 4, NULL);
          AngleArc (pto->hdc, pto->rect.left, pto->rect.top,
                 (DWORD) pto->rect.bottom/ 4, (float) 270.0, (float) 90.0);
        }

    } return NULL;



    /**********************************************************************\
    *  TROB_HITTEST
    *
    * Check the point sent in in the lParam to see if it lays within
    *  the bounds of the objects defining rectangle.
    * return - pointer to the object iff the point is in rectangle,
    *  otherwise return NULL.
    \**********************************************************************/
    case TROB_HITTEST:{
        POINT  mouWorld;
        mouWorld.x = LOWORD(lParam);
        mouWorld.y = HIWORD(lParam);

        DPtoLP (pto->hdc, &mouWorld, 1);

        if (PtInRect (&pto->rect, mouWorld))  return pto;
        else  return NULL;
    }



    /**********************************************************************\
    *  WM_LBUTTONDOWN &  WM_RBUTTONDOWN
    *
    * Capture the mouse, set the tracking mode depending on the mouse
    *  location in world coordinates, reset the mouse position.
    *
    \**********************************************************************/
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN: {
      POINT  newmouScreen;
      POINT  mouWorld;

      mouWorld.x = LOWORD(lParam);
      mouWorld.y = HIWORD(lParam);
      DPtoLP (pto->hdc, &mouWorld, 1);

      /* upper left hand corner. right button is no-op. */
      if ((mouWorld.x <= (pto->rect.right  / 2)) &&
          (mouWorld.y <= (pto->rect.bottom / 2))) {
          if (msg == WM_RBUTTONDOWN) return NULL;
          pto->Mode = TMMOVE;
          newmouScreen.x = pto->rect.left;
          newmouScreen.y = pto->rect.top;

      /* lower left hand corner */
      } else if ((mouWorld.x <= (pto->rect.right  / 2)) &&
          (mouWorld.y > (pto->rect.bottom / 2))) {

          pto->Mode = (msg == WM_RBUTTONDOWN) ? TMSHEARY : TMSIZEY;
          newmouScreen.x = pto->rect.left;
          newmouScreen.y = pto->rect.bottom;

      /* upper right hand corner */
      } else if ((mouWorld.x > (pto->rect.right  / 2)) &&
          (mouWorld.y <= (pto->rect.bottom / 2))) {

          pto->Mode = (msg == WM_RBUTTONDOWN) ? TMSHEARX : TMSIZEX;
          newmouScreen.x = pto->rect.right;
          newmouScreen.y = pto->rect.top;

      /* lower right hand corner */
      } else if ((mouWorld.x > (pto->rect.right  / 2)) &&
          (mouWorld.y > (pto->rect.bottom / 2))) {

          pto->Mode = (msg == WM_RBUTTONDOWN) ? TMROTATE : TMSIZEXY;
          newmouScreen.x = pto->rect.right;
          newmouScreen.y = pto->rect.bottom;
      }

      if (! (pto->Mode & pto->allowedModes)) {
        pto->Mode = TMNONE;
        return NULL;
      }

      SetCapture(hwnd);
      LPtoDP (pto->hdc, &newmouScreen, 1);
      ClientToScreen (hwnd, &newmouScreen);
      SetCursorPos (newmouScreen.x,newmouScreen.y);

      GetWorldTransform (pto->hdc, &pto->xfmDown);
    } return NULL;



    /**********************************************************************\
    *  WM_MOUSEMOVE
    *
    * this is where almost all of the interesting calculation is done.
    *  First clip the mouse location to be in rectClip, then
    *  call MouseMove() to handle the different tracking modes.
    \**********************************************************************/
    case WM_MOUSEMOVE: {
      if ((short) LOWORD(lParam) < (short)pto->rectClip.left)
        lParam = MAKELONG ((WORD)pto->rectClip.left, HIWORD(lParam));

      if (LOWORD(lParam) > (WORD)pto->rectClip.right)
        lParam = MAKELONG ((WORD)pto->rectClip.right, HIWORD(lParam));

      if ((short) HIWORD(lParam) < (short)pto->rectClip.top)
        lParam = MAKELONG (LOWORD(lParam), (WORD)pto->rectClip.top);

      if (HIWORD(lParam) > (WORD)pto->rectClip.bottom)
        lParam = MAKELONG (LOWORD(lParam),(WORD)pto->rectClip.bottom);

      MouseMove (pto, msg, hwnd, lParam);

    } return NULL;



    /**********************************************************************\
    *  WM_RBUTTONUP & WM_LBUTTONUP
    *
    * simply release the mouse capture, and set the mode to TMNONE.
    \**********************************************************************/
    case WM_RBUTTONUP:
    case WM_LBUTTONUP: {
      if (pto->Mode) {
         ReleaseCapture();
         pto->Mode = TMNONE;
      }
    } return NULL;

  }  /* end switch(msg) */
}






/**************************************************************************\
*  function:  MouseMove()
*
*  input parameters:
*   pto -  pointer to a track object.
*   msg -  not used.
*   hwnd - Window handle for the window the track object exists within.
*   lParam - Usually fourth param to window proc. varies based on msg.
*
*  The tracking behavior which the user observers when moving the mouse
*   is based on the current tracking mode of the object.  This is usually
*   determined on the mouse down event (c.f. TM*).  First erase the old
*   object, then figure out the change to the transform matrix, finally
*   change the world transform matrix and redraw the object.
*
*  Tranform:
*    (    eM11        eM12        0   )
*    (    eM21        eM22        0   )
*    (    eDx         eDy         1   )
*
*   xDevice = (xWorld * eM11) + (yWorld * eM21) + eDx
*   yDevice = (xWorld * eM12) + (yWorld * eM22) + eDy
*
*   In this routine the Device (mouse location) and World (rectangle corner)
*   points are known.  Therefore, the two equations above are solved for
*   the desired matrix entry value (e.g. eM11, 1M12, ... eDy).  The tracking
*   mode determines which one of these entries may be changed.  E.g. scaling
*   in X modifies eM11 while shearing in X modifies eM12.  So rather than
*   using the world transform to map from world to device points, we are
*   back-computing the proper contents of the world transform.
*
\**************************************************************************/
VOID MouseMove(PTrackObject pto, int msg, HWND hwnd, LONG lParam)
{
POINT  mouWorld, mouDevice, orgDevice;

    UNREFERENCED_PARAMETER(msg);

    doTrackObject(pto, TROB_PAINT, hwnd, lParam);
    mouDevice.x = mouWorld.x = LOWORD(lParam);
    mouDevice.y = mouWorld.y = HIWORD(lParam);

    SetWorldTransform(pto->hdc, &pto->xfmDown);
    DPtoLP (pto->hdc, &mouWorld, 1);

    /* offset the mouse device point for the viewport's origin. */
    GetViewportOrgEx (pto->hdc, &orgDevice);
    mouDevice.x -= orgDevice.x;
    mouDevice.y -= orgDevice.y;

    GetWorldTransform(pto->hdc, &pto->xfmChange);

    switch (pto->Mode) {
      /*******************************************************\
      *    (     1         xShear       0   )
      *    (     0           1          0   )
      *    (     0           0          1   )
      *
      * xWorld = rect.left == 0;
      \*******************************************************/
      case TMSHEARX: {
        pto->xfmChange.eM12 = (float) mouDevice.y;
        pto->xfmChange.eM12 -=pto->xfmChange.eDy;
        pto->xfmChange.eM12 /=(float) pto->rect.right ;
        SetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;


      /*******************************************************\
      *    (     1           0          0   )
      *    (   yShear        1          0   )
      *    (     0           0          1   )
      *
      * yWorld = rect.top == 0;
      \*******************************************************/
      case TMSHEARY: {
        pto->xfmChange.eM21 = (float) mouDevice.x;
        pto->xfmChange.eM21 -=pto->xfmChange.eDx;
        pto->xfmChange.eM21 /=(float) pto->rect.bottom ;
        SetWorldTransform (pto->hdc, &pto->xfmChange);

      } break;


      /*******************************************************\
      *    (   cos(a)      -sin(a)      0   )
      *    (   sin(a)       cos(a)      0   )
      *    (     0           0          1   )
      *
      * a == rotation angle.  Since mouse in in lower right,
      *  we need to shift this back 45 degrees (assuming that
      *  straight down is 0 degrees).  Thus we actually compute
      *  cos(a) = cos(b - 45) = cos(b)sin(45) + cos(45)sin(45)
      *  where b is angle from the origin to the mouse (x,y)
      *  cos(45) = sin(45) ~= 0.707107
      *  cos(b) = y/r    sin(b) = x/r
      *
      \*******************************************************/
      case TMROTATE: {
        float r;

        /* translate back to the origin. */
        pto->xfmChange.eDx = pto->xfmChange.eDy = (float)0.0;
        SetWorldTransform (pto->hdc, &pto->xfmChange);

        /* rotate about the origin. */
        r = (float) sqrt( (double)(mouWorld.x * mouWorld.x) +
                          (double)(mouWorld.y * mouWorld.y));

        pto->xfmChange.eM11 = (float) mouWorld.y / r;
        pto->xfmChange.eM11 += (float) mouWorld.x / r;
        pto->xfmChange.eM11 *= (float) 0.707107;
        pto->xfmChange.eM22 = pto->xfmChange.eM11;

        pto->xfmChange.eM12 = (float) mouWorld.y / r;
        pto->xfmChange.eM12 -= (float) mouWorld.x / r;
        pto->xfmChange.eM12 *= (float) 0.707107;
        pto->xfmChange.eM21 = -pto->xfmChange.eM12;

        pto->xfmChange.eDx = pto->xfmChange.eDy = (float)0.0;

        ModifyWorldTransform (pto->hdc, &pto->xfmChange, MWT_RIGHTMULTIPLY);

        /* translate back to the original offset. */
        pto->xfmChange.eM11 =
        pto->xfmChange.eM22 = (float) 1.0;
        pto->xfmChange.eM12 =
        pto->xfmChange.eM21 = (float) 0.0;

        pto->xfmChange.eDx = pto->xfmDown.eDx;
        pto->xfmChange.eDy = pto->xfmDown.eDy;
        ModifyWorldTransform (pto->hdc, &pto->xfmChange, MWT_RIGHTMULTIPLY);
        GetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;


      /*******************************************************\
      *    (  Size X         0          0   )
      *    (     0        Size Y        0   )
      *    (     0           0          1   )
      *
      \*******************************************************/
      case TMSIZEXY: {
        pto->xfmChange.eM11 = (float) mouDevice.x;
        pto->xfmChange.eM11 -=pto->xfmChange.eDx;
        pto->xfmChange.eM11 -=((float) pto->rect.bottom*pto->xfmChange.eM21);
        pto->xfmChange.eM11 /=(float) pto->rect.right ;
        if (fabs(pto->xfmChange.eM11) < EPSILON)  // HACK.  system bug ?
           pto->xfmChange.eM11 = EPSILON;

        pto->xfmChange.eM22 = (float) mouDevice.y;
        pto->xfmChange.eM22 -=pto->xfmChange.eDy;
        pto->xfmChange.eM22 -=((float) pto->rect.right*pto->xfmChange.eM12);
        pto->xfmChange.eM22 /=(float) pto->rect.bottom ;
        if (fabs(pto->xfmChange.eM22) < EPSILON)  // HACK.  system bug ?
           pto->xfmChange.eM22 = EPSILON;
        SetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;


      /*******************************************************\
      *    (  Size X         0          0   )
      *    (     0           1          0   )
      *    (     0           0          1   )
      *
      * yWorld = rect.top == 0;
      \*******************************************************/
      case TMSIZEX: {
        pto->xfmChange.eM11 = (float) mouDevice.x;
        pto->xfmChange.eM11 -=pto->xfmChange.eDx;
        pto->xfmChange.eM11 /=(float) pto->rect.right ;
        if (fabs(pto->xfmChange.eM11) < EPSILON)  // HACK.  system bug ?
           pto->xfmChange.eM11 = EPSILON;

        SetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;


      /*******************************************************\
      *    (     1           0          0   )
      *    (     0        Size Y        0   )
      *    (     0           0          1   )
      *
      * xWorld = rect.left == 0;
      \*******************************************************/
      case TMSIZEY: {
        pto->xfmChange.eM22 = (float) mouDevice.y;
        pto->xfmChange.eM22 -=pto->xfmChange.eDy;
        pto->xfmChange.eM22 /=(float) pto->rect.bottom ;
        if (fabs(pto->xfmChange.eM22) < EPSILON)  // HACK.  system bug ?
           pto->xfmChange.eM22 = EPSILON;
        SetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;


      /*******************************************************\
      *    (     1           0          0   )
      *    (     0           1          0   )
      *    (   Move x      Move y       1   )
      *
      * xWorld = rect.left == 0;
      * yWorld = rect.top == 0;
      \*******************************************************/
      case TMMOVE: {
        pto->xfmChange.eDx = (float) mouDevice.x ;
        pto->xfmChange.eDy = (float) mouDevice.y ;
        SetWorldTransform (pto->hdc, &pto->xfmChange);
      } break;
    } /* end switch */

    doTrackObject(pto, TROB_PAINT, hwnd, lParam);

    return;
 }

