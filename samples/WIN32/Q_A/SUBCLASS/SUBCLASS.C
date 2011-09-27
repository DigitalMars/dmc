
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/**************************************************************************\
*  subclass.c -- Demonstrate subclassing of standard controls.
*
* This sample allows the user to create an arbitrary number of child
*  controls on the main window.  These controls are subclassed, and the
*  subclass procedure provides the user a way to move and size the controls.
*  A menu item switches in and out of "Test Mode."  When this is on, the
*  subclass procedure passes all messages through to the old procedure, and
*  the controls act just like normal.
*
* There is a single subclass window procedure for a variety of different
*  control classes.  This is accomplished by storing the old window procedure
*  for the control in a structure pointed at by the 4 "user extra bytes."
*  I.e. the GWL_USERDATA contains a pointer to the following structure:
*
*       typedef struct tagExtraBytes{
*           WNDPROC    pfnOldProc;
*           RECT       rect;
*           int        Action;
*       } ExtraBytes, *PExtraBytes;
*
*  and the old window procedure is stored in the pfnOldProc field.
*  The rect field is used for drawing the temporary rectangle while the
*  user is dragging, and the Action field holds the type of action that
*  is allowed (move, size, ...).  Notice that the rect rectangle is in
*  the coordinate space of the parent window.
*
* Warning:  buttons, edit fields, and list boxes are easy.  It is difficult
*  to extend this to static controls because they do not normally receive
*  input and thus do not get mousedown messages.  It is difficult to extend
*  this to comboboxes because of the compound structure and the fact that
*  the comboboxes children controls (edit field & list box) get the mouse
*  messages first.
*
\**************************************************************************/

#include <windows.h>
#include "subclass.h"



/**************************************************************************\
*
*  function:  WinMain()
*
*  input parameters:  c.f. generic sample
*
\**************************************************************************/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
  MSG   msg;

  UNREFERENCED_PARAMETER( lpCmdLine );


  /* Check for previous instance.  If none, then register class. */
  if (!hPrevInstance) {
      WNDCLASS  wc;

      wc.style = 0;
      wc.lpfnWndProc = (WNDPROC)MainWndProc;
      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;
      wc.hInstance = hInstance;
      wc.hIcon = LoadIcon(hInstance, "subclassIcon");
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = GetStockObject(GRAY_BRUSH);
      wc.lpszMenuName =  "scMenu";
      wc.lpszClassName = "scClass";

      if (!RegisterClass(&wc)) return (FALSE);
  }  /* class registered o.k. */


  /* Create the main window.  Return false if CreateWindow() fails */
  hInst = hInstance;

  hwndMain = CreateWindow(
      "scClass",
      "Subclass Window Sample",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,
      NULL);

  if (!hwndMain) return (FALSE);


  ShowWindow(hwndMain, nCmdShow);

  /* Loop getting messages and dispatching them. */
  while (GetMessage(&msg,NULL, 0,0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  /* Return the value from PostQuitMessage */
  return (msg.wParam);
}


/**************************************************************************\
*
*  function:  MainWndProc()
*
*  input parameters:  normal window procedure parameters.
*  global variables:
*   hInst     - global instance handle.  used in create window.
*   fTestMode - global boolean, set by menu, if true ignore subclass actions.
*
* The main window actually does very little.  Just respond to menu commands.
\**************************************************************************/

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   HWND hwndChild;

   switch (message) {

    /**********************************************************************\
    *  WM_DESTROY
    *
    * Post quit message.
    \**********************************************************************/
    case WM_DESTROY:
        PostQuitMessage(0);
    break;


    /**********************************************************************\
    *  WM_COMMAND
    *
    * Switch on the different menu items.  Create a different class of
    *  window for most of them.  Switch on and off the fTestMode global
    *  variable on IDM_TEST.
    \**********************************************************************/

    case WM_COMMAND: {
      switch (LOWORD (wParam)) {
        case IDM_BUTTON:
          hwndChild = CreateWindow(
            "BUTTON", "button",
            WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS,
            50,10, 60, 32,
            hwnd, NULL, hInst, NULL);

          SubclassWindow (hwndChild, SubclassWndProc);
        break;

        case IDM_EDIT:
          hwndChild = CreateWindow(
            "EDIT", "edit",
            WS_VISIBLE | WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS,
            50,50, 60, 28,
            hwnd, NULL, hInst, NULL);

          SubclassWindow (hwndChild, SubclassWndProc);
        break;

        case IDM_LIST:
          hwndChild = CreateWindow(
            "LISTBOX", NULL,
            WS_VISIBLE | WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS,
            50,90, 60, 50,
            hwnd, NULL, hInst, NULL);

          SubclassWindow (hwndChild, SubclassWndProc);
        break;

        case IDM_TEST:
          fTestMode = ToggleMenu (GetMenu (hwnd), IDM_TEST);
        break;

      }       /* end switch */
    } break;  /* end WM_COMMAND  */

  }
  return (DefWindowProc(hwnd, message, wParam, lParam));
}


/**************************************************************************\
*
*  function:  SubclassWndProc
*
*  input parameters:  normal window procedure parameters.
*  global variables:
*   hwndMain  - parent window of the control.
*   fTestMode - global boolean, set by menu, if true ignore subclass actions.
\**************************************************************************/

LRESULT CALLBACK SubclassWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PExtraBytes  peb;


  /* get the pointer to the extra bytes structure out of the user data. */
  peb = (PExtraBytes)GetWindowLong (hwnd, GWL_USERDATA);

  /* if running in test mode, just return the old procedure. */
  if (fTestMode)
    return (CallWindowProc ((peb->pfnOldProc), hwnd, message, wParam, lParam));


  switch (message) {


    /**********************************************************************\
    *  WM_DESTROY
    *
    * Free up the ExtraBytes structure which was allocated at subclass time.
    \**********************************************************************/
    case WM_DESTROY:
      LocalUnlock (LocalHandle ((LPSTR)peb));
    break;


    /**********************************************************************\
    *  WM_SETCURSOR
    *
    * Set the mouse pointer conditional on the Action which would be taken
    *  if the user presses mouse down.  This is set in WM_MOUSEMOVE.
    \**********************************************************************/
    case WM_SETCURSOR:
      switch (peb->Action) {
        case ACTIONNONE  :SetCursor (LoadCursor (NULL, IDC_ARROW)); break;
        case ACTIONMOVE  :SetCursor (LoadCursor (NULL, IDC_SIZEALL)); break;
        case ACTIONSIZEX :SetCursor (LoadCursor (NULL, IDC_SIZEWE)); break;
        case ACTIONSIZEY :SetCursor (LoadCursor (NULL, IDC_SIZENS)); break;
        case ACTIONSIZEXY:SetCursor (LoadCursor (NULL, IDC_SIZENWSE)); break;
      }
    return (0);


    /**********************************************************************\
    *  WM_LBUTTONDOWN
    *
    * The user is beginning a drag operation.  Fill in an initial window pos
    *  in the rect field, figure out which corner the cursor should be set
    *  to, translate that point to screen coordinates, and finally capture
    *  the mouse.
    \**********************************************************************/

    case WM_LBUTTONDOWN: {
      POINT  mouse;

      QueryWindowPos (hwnd, &peb->rect);

      switch (peb->Action) {
        case ACTIONMOVE  :
          mouse.x = peb->rect.left;
          mouse.y = peb->rect.top ;
        break;

        case ACTIONSIZEX :
          mouse.x = peb->rect.right;
          mouse.y = peb->rect.top ;
        break;

        case ACTIONSIZEY :
          mouse.x = peb->rect.left;
          mouse.y = peb->rect.bottom;
        break;

        case ACTIONSIZEXY:
          mouse.x = peb->rect.right;
          mouse.y = peb->rect.bottom;
        break;
      }

      /* SetCursorPos() works based on screen position, and mouse is
       *  currently relative to hwndMain.
       */
      ClientToScreen (hwndMain, &mouse);
      SetCursorPos (mouse.x, mouse.y);
      SetCapture (hwnd);
    } return 0;


    /**********************************************************************\
    *  WM_LBUTTONUP
    *
    * Complement of the WM_LBUTTONDOWN message.  Release the capture,
    *  and reset the actual window's position.
    \**********************************************************************/

    case WM_LBUTTONUP: {
      LONG lTemp;

      if (GetCapture() == hwnd) {
        ReleaseCapture();

        if (peb->rect.left > peb->rect.right)
        {
            lTemp = peb->rect.left;
            peb->rect.left = peb->rect.right;
            peb->rect.right = lTemp;
        }

        if (peb->rect.top > peb->rect.bottom)
        {
            lTemp = peb->rect.top;
            peb->rect.top = peb->rect.bottom;
            peb->rect.bottom = lTemp;
        }

        SetWindowPos (hwnd, NULL, peb->rect.left, peb->rect.top,
                      (peb->rect.right - peb->rect.left),
                      (peb->rect.bottom - peb->rect.top),
                      SWP_NOZORDER);
        InvalidateRect (hwnd, NULL, TRUE);
      }
    return 0;
   }

    /**********************************************************************\
    *  WM_MOUSEMOVE
    *
    * There are two cases of interest here.  If the mouse is captured, then
    *  change the rect field in the extrabyte structure to reflect the new
    *  size/position that the user is selection.
    *
    *  If the mouse is not captured, then set the Action field dependent on
    *  the quadrant so that the WM_SETCURSOR will display the correct cursor.
    *
    \**********************************************************************/

    case WM_MOUSEMOVE: {
      POINT mouse;
      /* the LO/HI-WORD from lParam will be a signed short relative to
       *  the child control window.
       */
      mouse.x = (int)(short)LOWORD(lParam);
      mouse.y = (int)(short)HIWORD(lParam);



      /* if the mouse is captured, then we are doing direct manipulation */
      if (GetCapture() == hwnd) {

        /* translate mouse pos to be client relative, not control relative */
        ClientToScreen (hwnd, &mouse);
        ScreenToClient (hwndMain, &mouse);

        /* erase the old rectangle */
        PaintRect (hwndMain, &peb->rect);

        switch (peb->Action) {
          case ACTIONMOVE  : {
            int width, height;

            width = peb->rect.right - peb->rect.left;
            height = peb->rect.bottom - peb->rect.top;
            peb->rect.left = mouse.x;
            peb->rect.top  = mouse.y;
            peb->rect.right  = peb->rect.left + width;
            peb->rect.bottom = peb->rect.top +  height;
          }break;

          case ACTIONSIZEX :
            peb->rect.right  = mouse.x;
          break;

          case ACTIONSIZEY :
            peb->rect.bottom = mouse.y;
          break;

          case ACTIONSIZEXY:
            peb->rect.right  = mouse.x;
            peb->rect.bottom = mouse.y;
          break;
        }

        /* Redraw the new rectangle */
        PaintRect (hwndMain, &peb->rect);
      }


      /* if the mouse is not captured, then set the action for the sake of
       *  the WM_SETCURSOR message.  The action is dependent upon which
       *  quadrant of the window the mouse cursor is over.
       */
      else {
        int width2, height2;
        RECT rect;

        GetClientRect (hwnd, &rect);
        width2 =  (rect.right -  rect.left)/2;
        height2 =  (rect.bottom -  rect.top)/2;

        /* upper left hand corner */
        if ((mouse.x <= (width2)) && (mouse.y <= (height2))) {
            peb->Action = ACTIONMOVE;

        /* lower left hand corner */
        } else if ((mouse.x <= (width2)) && (mouse.y > (height2))) {
            peb->Action = ACTIONSIZEY;

        /* upper right hand corner */
        } else if ((mouse.x > (width2)) && (mouse.y <= (height2))) {
            peb->Action = ACTIONSIZEX;

        /* lower right hand corner */
        } else if ((mouse.x > (width2)) && (mouse.y > (height2))) {
            peb->Action = ACTIONSIZEXY;
        }
      }
    } return 0;



    /**********************************************************************\
    * for messages that are not handled explicitly here, pass them
    *  back to the old window procedure.
    \**********************************************************************/
    default:
      return (CallWindowProc ((peb->pfnOldProc), hwnd, message, wParam, lParam));
    break;
    } /* end switch */

  return 0;
}



/**************************************************************************\
*  function:  SubclassWindow
*
*  input parameters:
*   hwnd            - window handle to be subclassed,
*   SubclassWndProc - the new window procedure.
*
*  Set in a new window procedure for this window.  Store the old window
*   procedure in the first field of the extrabytes structure.  This routine
*   is specific to this program in the use of this particular extrabyte
*   structure.  Note that the pointer in the user bytes needs to be freed
*   later (in WM_DESTROY).
\**************************************************************************/

VOID SubclassWindow (HWND hwnd, WNDPROC SubclassWndProc)
{
  PExtraBytes  peb;

  peb = (PExtraBytes)LocalAlloc (LPTR, sizeof (ExtraBytes));
  peb->pfnOldProc = (WNDPROC) GetWindowLong (hwnd, GWL_WNDPROC);

  SetWindowLong (hwnd, GWL_USERDATA, (LONG) peb);
  SetWindowLong (hwnd, GWL_WNDPROC,  (LONG) SubclassWndProc);
}


/**************************************************************************\
*  function:  QueryWindowPos
*
*  input parameters:
*   hwnd - window handle,  prect - pointer to rectangle to hold the answer.
*
*  global variables:
*   hwndMain  - parent window of the control.
*
*  Return the bounding rectangle for the hwnd control relative to hwndMain.
\**************************************************************************/
VOID QueryWindowPos (HWND hwnd, LPRECT prect)
{
  GetWindowRect (hwnd, prect);
  ScreenToClient (hwndMain, (LPPOINT)&(prect->left));
  ScreenToClient (hwndMain, (LPPOINT)&(prect->right));
}


/**************************************************************************\
*  function:  PaintRect
*
*  input parameters:
*   hwnd - window handle,  prect - pointer to rectangle to be painted.
*
*  Get an HDC and draw the rectangle in R2_NOT mode which if done twice will
*   leave the screen in its initial state.
\**************************************************************************/

VOID PaintRect (HWND hwnd, LPRECT prect)
{
    HDC hdc;

    hdc = GetDC (hwnd);
    SetROP2(hdc, R2_NOT);
    SelectObject (hdc, GetStockObject (NULL_BRUSH));
    Rectangle (hdc, prect->left, prect->top,
                    prect->right, prect->bottom);
    ReleaseDC (hwnd, hdc);
}


/**************************************************************************\
*  function:  ToggleMenu
*
*  input parameters:
*   hmenu - menu handle,  iditem - id of the item to check.
*
*  returns:  the new checked state.
*
*  Change a menu item to be checked if it is not, or unchecked if it is.
\**************************************************************************/

DWORD ToggleMenu (HMENU hmenu, UINT iditem)
{
  DWORD  state;

  /* query the checked state.
   *  unfortunately, this turns checked off.
   */
  state = CheckMenuItem (hmenu, iditem, MF_BYCOMMAND);

  /* switch the truth value of oldstate. */
  state = (state) ? MF_UNCHECKED : MF_CHECKED;

  /* reset the menu item. */
  CheckMenuItem (hmenu, iditem, MF_BYCOMMAND | state);

  return state;
}
