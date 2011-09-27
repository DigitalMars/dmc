
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/***************************************************************************
 *                                                                         *
 *  PROGRAM     : Menu.c                                                   *
 *                                                                         *
 *  PURPOSE     : To give a demonstration of the use of popup menus, user  *
 *                defined menus and menu functions.                        *
 *                                                                         *
 *  FUNCTIONS   : WinMain()           - Calls the initialization function  *
 *                                      and enters the message loop.       *
 *                                                                         *
 *                MenuInit()          - Registers the app. window class.   *
 *                                                                         *
 *                About()             - Dialog function for the About..    *
 *                                      dialog.                            *
 *                                                                         *
 *                ShrinkBitmap()      - Shrinks a 64x64 bitmap to a size   *
 *                                      useable for a user-defined menu    *
 *                                      checkmark.                         *
 *                                                                         *
 *                HandleCreate()      - Creates a new menu and appends it  *
 *                                      to the main menu                   *
 *                                                                         *
 *                HandlePaint()       - Handles repainting the app's client*
 *                                      area                               *
 *                                                                         *
 *                HandleChangeColors()- Changes the state of the "colors"  *
 *                                      menu item.                         *
 *                                                                         *
 *                HandleDrawItem()    - Redraws the menu items in the      *
 *                                      "colors" menu                      *
 *                                                                         *
 *                HandlePopupMenu()   - handles display of the "floating"  *
 *                                      popup.                             *
 *                                                                         *
 *                MenuWndProc()       - Window function for the app.       *
 *                                                                         *
 *                                                                         *
 ***************************************************************************/
#include "windows.h"
#include "menu.h"


HANDLE   hInst;
HMENU    hBigMenu;
HBITMAP  hbmCheckOn;
HBITMAP  hbmCheckOff;

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : WinMain(HANDLE, HANDLE, LPSTR, int)                        *
 *                                                                          *
 *  PURPOSE    : Creates the main app. window, calls an initialization      *
 *               function and enters the message loop.                      *
 *                                                                          *
 ****************************************************************************/
int APIENTRY WinMain(
    HANDLE hInstance,
    HANDLE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
    )
{

    HWND  hWnd;
    MSG msg;                                 /* message                      */

    UNREFERENCED_PARAMETER( lpCmdLine );

    /* Register main window class if this is the first instance of the app. */
    if (!hPrevInstance)
        if (!MenuInit (hInstance))
            return 0;

    hInst = hInstance;

    /* Create the app. window */
    hWnd = CreateWindow ("menu",
                         "Menu Example",
                         WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         (HWND) NULL,
                         NULL,
                         hInstance,
                         (LPSTR) NULL);

    if (!hWnd)
        return 0;

    ShowWindow (hWnd, nCmdShow);
    UpdateWindow (hWnd);

    while (GetMessage (&msg, NULL, 0, 0)){
        /* Since we have no accelerators, no need to call
         * TranslateAccelerator here.
         */
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
    return(msg.wParam);
}


/****************************************************************************
 *                                                                          *
 *  FUNCTION   : MenuInit (hInstance)                                       *
 *                                                                          *
 *  PURPOSE    : Registers the main window class.                           *
 *                                                                          *
 *  RETURNS    : TRUE   -  if RegisterClass() went off ok                   *
 *               FALSE  -  otherwise.                                       *
 *                                                                          *
 ****************************************************************************/
BOOL NEAR PASCAL MenuInit (HANDLE hInstance)
{
    HANDLE    hMemory;
    PWNDCLASS pWndClass;
    BOOL      bSuccess;

    /* Initialize the menu window class */
    hMemory   = LocalAlloc(LPTR, sizeof(WNDCLASS));
    if(!hMemory){
        MessageBox(NULL, "<MenuInit> Not enough memory.", NULL, MB_OK | MB_ICONHAND);
        return(FALSE);
    }

    pWndClass = (PWNDCLASS) LocalLock(hMemory);

    pWndClass->style         = 0;
    pWndClass->lpfnWndProc   = (WNDPROC) MenuWndProc;
    pWndClass->hInstance     = hInstance;
    pWndClass->hIcon         = LoadIcon (hInstance, "menu");
    pWndClass->hCursor       = LoadCursor (NULL, IDC_ARROW);
    pWndClass->hbrBackground = GetStockObject (WHITE_BRUSH);
    pWndClass->lpszMenuName  = (LPSTR) "MenuMenu",
    pWndClass->lpszClassName = (LPSTR) "menu";

    bSuccess = RegisterClass (pWndClass);
    LocalUnlock (hMemory);
    LocalFree (hMemory);

    return bSuccess;
}


/****************************************************************************
 *                                                                          *
 *  FUNCTION   : About (hDlg, message, wParam, lParam)                      *
 *                                                                          *
 *  PURPOSE    : Dialog function for the About menu... dialog.              *
 *                                                                          *
 ****************************************************************************/
BOOL APIENTRY About(
        HWND hDlg,
        UINT message,
        UINT wParam,
        LONG lParam)

{
    switch (message){
        case WM_INITDIALOG:
            return(TRUE);

        case WM_COMMAND:
            // LOWORD added for portability
            if (LOWORD(wParam) == IDOK){
                EndDialog(hDlg,0);
                return(TRUE);
            }
            break;
    }
    return(FALSE);
        UNREFERENCED_PARAMETER(lParam);
}


/****************************************************************************
 *                                                                          *
 *  FUNCTION   : ShrinkBitmap(hwnd, hbm)                                    *
 *                                                                          *
 *  PURPOSE    : This function shrinks a 64x64 bitmap into a bitmap useable *
 *               for the user-defined checkmark for menu items. This can be *
 *               easily generalized to shrink bitmaps of any size.          *
 *                                                                          *
 *  RETURNS    : HBITMAP - A handle to the new bitmap.                      *
 *                                                                          *
 ****************************************************************************/
HBITMAP APIENTRY ShrinkBitmap (
        HWND hwnd,
        HBITMAP hbm)
{
    HDC     hdc;
    HDC     hmemorydcNew;
    HDC     hmemorydcOld;
    LONG    checkMarkSize;
    HBITMAP hCheckBitmap;
    HBITMAP hOldBitmapSave;
    HBITMAP hNewBitmapSave;

    hdc = GetDC (hwnd);

    /* Create DCs for the source (old) and target (new) bitmaps */
    hmemorydcNew = CreateCompatibleDC (hdc);
    hmemorydcOld = CreateCompatibleDC (hdc);

    /* Determine the dimensions of the default menu checkmark and
     * create a target bitmap of the same dimensions
     */
    checkMarkSize = GetMenuCheckMarkDimensions ();
    hCheckBitmap  = CreateCompatibleBitmap (hdc,
                                            LOWORD (checkMarkSize),
                                            HIWORD (checkMarkSize));

    /* Select the source bitmap and the target bitmap into their
     * respective DCs.
     */
    hOldBitmapSave = SelectObject (hmemorydcNew, hCheckBitmap);
    hNewBitmapSave = SelectObject (hmemorydcOld, hbm);

    /* Shrink the source bitmap into the target DC */
    StretchBlt (hmemorydcNew,
                0,
                0,
                LOWORD(checkMarkSize),
                HIWORD(checkMarkSize),
                hmemorydcOld,
                0,
                0,
                64,
                64,
                SRCCOPY);

    /* De-select the bitmaps and clean up .. */
    SelectObject (hmemorydcNew, hOldBitmapSave);
    SelectObject (hmemorydcOld, hNewBitmapSave);
    DeleteDC (hmemorydcNew);
    DeleteDC (hmemorydcOld);
    ReleaseDC (hwnd, hdc);

    /* .. and return a handle to the target bitmap */
    return hCheckBitmap;
}


/****************************************************************************
 *                                                                          *
 *  FUNCTION   : HandleCreate ( hwnd )                                      *
 *                                                                          *
 *  PURPOSE    : Creates a new (empty) menu and appends to it the "State"   *
 *               menu items. It sets up the user-defined checkmarks for the *
 *               menu. It then inserts this menu into the main menu bar.    *
 *                                                                          *
 ****************************************************************************/
VOID APIENTRY HandleCreate (HWND hwnd)
{
    HMENU   hMenu;
    HMENU   hWndMenu;

    /* Create a new menu into the menubar on the fly */
    hMenu = CreateMenu ();
    if (!hMenu)
        return;

    /* Append the state menu items to it */
    AppendMenu (hMenu, MF_STRING, IDM_STATE1, "South Dakota");
    AppendMenu (hMenu, MF_STRING, IDM_STATE2, "Washington");
    AppendMenu (hMenu, MF_STRING, IDM_STATE3, "California");
    if (!AppendMenu (hMenu, MF_STRING, IDM_STATE4, "Oregon")){
        /* It is unlikely the other appends will fail and this will succeed.
         * So just check this one. And if it fails, Destroy the menu for
         * good measure and return.
         */
        DestroyMenu(hMenu);
        return;
    }
    hbmCheckOn  = ShrinkBitmap (hwnd, LoadBitmap (hInst, "checkon"));
    hbmCheckOff = ShrinkBitmap (hwnd, LoadBitmap (hInst, "checkoff"));

    /* Set up the user-defined check marks */
    SetMenuItemBitmaps (hMenu, 0, MF_BYPOSITION, hbmCheckOff, hbmCheckOn);
    SetMenuItemBitmaps (hMenu, 1, MF_BYPOSITION, hbmCheckOff, hbmCheckOn);
    SetMenuItemBitmaps (hMenu, 2, MF_BYPOSITION, hbmCheckOff, hbmCheckOn);
    SetMenuItemBitmaps (hMenu, 3, MF_BYPOSITION, hbmCheckOff, hbmCheckOn);

    /* Now insert the menu into the main menu bar. */
    hWndMenu = GetMenu (hwnd);
    InsertMenu (hWndMenu, 2, MF_POPUP|MF_BYPOSITION, (DWORD)hMenu, "States");

    return;
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : HandlePaint ( hwnd )                                       *
 *                                                                          *
 *  PURPOSE    : Handles the repainting of the main app's client area.      *
 *                                                                          *
 ****************************************************************************/
VOID APIENTRY HandlePaint (HWND hwnd)
{
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rc;

    hdc = BeginPaint (hwnd, (LPPAINTSTRUCT)&ps);

    /* Center the text in the client area */
    GetClientRect (hwnd, (LPRECT)&rc);
    DrawText (hdc,
              "Down click in the window for a popup menu",
               41,
               (LPRECT)&rc,
               DT_CENTER | DT_WORDBREAK);
    EndPaint(hwnd, (LPPAINTSTRUCT)&ps);
}


/****************************************************************************
 *                                                                          *
 *  FUNCTION   : HandleChangeColors (hwnd)                                  *
 *                                                                          *
 *  PURPOSE    : Toggles the state of the Owner Draw item in the Colors     *
 *               menu. If it is on, the "Black", "Blue", "Red", and "Green" *
 *               individual menu text items are modified so that they will  *
 *               contain bands of color. Otherwise, the colors are replaced *
 *               by the text.                                               *
 *                                                                          *
 ****************************************************************************/
VOID APIENTRY HandleChangeColors(HWND hwnd)
{
    HMENU hMenu;
    BOOL  fOwnerDraw;

    /* Get a handle to the Colors menu. This is at position 1. */
    hMenu = GetSubMenu (GetMenu (hwnd), IDCOLORS_POS);

    /* Get the current state of the item */
    fOwnerDraw = GetMenuState ( hMenu,
                                IDM_COLOROWNERDR, MF_BYCOMMAND) & MF_CHECKED;

    /* Toggle the state of the item. */
    CheckMenuItem ( hMenu,
                    IDM_COLOROWNERDR,
                    MF_BYCOMMAND | (fOwnerDraw ? MF_UNCHECKED : MF_CHECKED));

    if (!fOwnerDraw){
        /* Change the items to owner-draw items. Pass the RGB value for the
         * color as the application-supplied data. This makes it easier for
         * us to draw the items.
         */
        ModifyMenu(hMenu,
                   IDM_BLACK,
                   MF_OWNERDRAW | MF_BYCOMMAND,
                   IDM_BLACK,
                   (LPSTR)RGB (0,0,0));

        ModifyMenu(hMenu,
                   IDM_BLUE,
                   MF_OWNERDRAW | MF_BYCOMMAND,
                   IDM_BLUE,
                   (LPSTR)RGB (0,0,255));

        ModifyMenu(hMenu,
                   IDM_RED,
                   MF_OWNERDRAW | MF_BYCOMMAND,
                   IDM_RED,
                   (LPSTR)RGB (255,0,0));

        ModifyMenu(hMenu,
                   IDM_GREEN,
                   MF_OWNERDRAW | MF_BYCOMMAND,
                   IDM_GREEN,
                   (LPSTR)RGB (0,255,0));
    }
    else {
        /* Change the items to normal text items. */
        ModifyMenu(hMenu, IDM_BLACK, MF_BYCOMMAND, IDM_BLACK, "Black");

        ModifyMenu(hMenu, IDM_BLUE, MF_BYCOMMAND, IDM_BLUE, "Blue");

        ModifyMenu(hMenu, IDM_RED, MF_BYCOMMAND, IDM_RED, "Red");

        ModifyMenu(hMenu, IDM_GREEN, MF_BYCOMMAND, IDM_GREEN, "Green");
    }
}


/****************************************************************************
 *                                                                          *
 *  FUNCTION   : HandleDrawItem ( hwnd, lpdis)                              *
 *                                                                          *
 *  PURPOSE    : Called in response to a WM_DRAWITEM message, i.e. when the *
 *               colors menu is being modified to an owner-draw menu, or    *
 *               one of the items is selected. It sizes the checkmark bitmap*
 *               to fit next to a color band and draws the color bands and  *
 *               the checkmark on the popup menu.                           *
 *                                                                          *
 ****************************************************************************/
VOID APIENTRY HandleDrawItem(
        HWND             hwnd,
        LPDRAWITEMSTRUCT lpdis)

{
    HDC     hdcBitmap;
    HBITMAP hbmSave;
    HBRUSH  hbr;
    RECT    rc;
    LONG    checkMarkSize;
    DWORD   textColorSave;
    DWORD   bkColorSave;

    /* Get the size of the checkmark so we can leave room for it since we
     * want to be able to check the selected color.
     */
    checkMarkSize = GetMenuCheckMarkDimensions ();

    if (lpdis->itemAction == ODA_SELECT ||
        lpdis->itemAction == ODA_DRAWENTIRE){

        CopyRect ((LPRECT)&rc, (LPRECT)&lpdis->rcItem);
        InflateRect ((LPRECT)&rc, (-2 - LOWORD(checkMarkSize)), -2);

        if (lpdis->itemState & ODS_SELECTED)
        {
            /* Item has been selected -- hilite with a gray frame */
            hbr = GetStockObject (GRAY_BRUSH);
            FrameRect (lpdis->hDC, (LPRECT)&rc, hbr);
        }
        else if (lpdis->itemAction == ODA_SELECT)
        {
            /* Item has been de-selected -- remove gray frame */
            hbr = CreateSolidBrush (GetSysColor (COLOR_MENU));
            FrameRect (lpdis->hDC, (LPRECT)&rc, hbr);
            DeleteObject (hbr);
        }
    }

    if (lpdis->itemAction == ODA_DRAWENTIRE){

        /* Paint the color item in the color requested. */
        hbr = CreateSolidBrush (lpdis->itemData);
        CopyRect ((LPRECT)&rc, (LPRECT)&lpdis->rcItem);
        InflateRect ((LPRECT)&rc, -10-LOWORD(checkMarkSize), -10);
        FillRect (lpdis->hDC, (LPRECT)&rc, hbr);
        DeleteObject (hbr);

        if (lpdis->itemState & ODS_CHECKED){
            /* Draw the check mark if the item is checked. */
            hdcBitmap = CreateCompatibleDC (lpdis->hDC);
            hbmSave = SelectObject (hdcBitmap, hbmCheckOn);

            textColorSave = SetTextColor (lpdis->hDC, 0x00000000L);
            bkColorSave   = SetBkColor (lpdis->hDC, 0x00FFFFFFL);

            /* Use Magic bitblt op so that monochrome bitmaps preserve
               background and foreground colors. */
            BitBlt (lpdis->hDC,
                    lpdis->rcItem.left,
                    lpdis->rcItem.top+
                           (MEASUREITEMHEIGHT - HIWORD (checkMarkSize)) / 2,
                    LOWORD (checkMarkSize),
                    HIWORD (checkMarkSize),
                    hdcBitmap,
                    0,
                    0,
                    ROP_PSDPxax);

            /* Restore colors and bitmap and clean up */
            SetTextColor (lpdis->hDC, textColorSave);
            SetBkColor (lpdis->hDC, bkColorSave);
            SelectObject (hdcBitmap, hbmSave);
            DeleteDC (hdcBitmap);

        }
    }
        UNREFERENCED_PARAMETER(hwnd);
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : HandlePopupMenu (hwnd, point)                              *
 *                                                                          *
 *  PURPOSE    : Handles the display of the "floating" popup that appears   *                                                           *
 *               on a mouse click in the app's client area.                 *
 *                                                                          *
 ****************************************************************************/
VOID APIENTRY HandlePopupMenu (
        HWND   hwnd,
        POINT point)

{
    HMENU hMenu;
    HMENU hMenuTrackPopup;

    /* Get the menu for the popup from the resource file. */
    hMenu = LoadMenu (hInst, "PopupMenu");
    if (!hMenu)
        return;

    /* Get the first menu in it which we will use for the call to
     * TrackPopup(). This could also have been created on the fly using
     * CreatePopupMenu and then we could have used InsertMenu() or
     * AppendMenu.
     */
    hMenuTrackPopup = GetSubMenu (hMenu, 0);

    /* Convert the mouse point to screen coordinates since that is what
     * TrackPopup expects.
     */
    ClientToScreen (hwnd, (LPPOINT)&point);

    /* Draw and track the "floating" popup */
    TrackPopupMenu (hMenuTrackPopup, 0, point.x, point.y, 0, hwnd, NULL);

    /* Destroy the menu since were are done with it. */
    DestroyMenu (hMenu);
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : MenuWndProc (hWnd, message, wParam, lParam)                *
 *                                                                          *
 *  PURPOSE    : Window function for the main app. window. Processes all the*
 *               menu selections and oter messages.                         *
 *                                                                          *
 ****************************************************************************/
LONG APIENTRY MenuWndProc (
        HWND hWnd,
        UINT message,
        UINT wParam,
        LONG lParam)

{
    FARPROC lpProc;
    HMENU hMenu;
    RECT rc;
    POINT pt;


    switch (message){
        case WM_SYSCOMMAND:
            /* Show the About ... dialog */
            if (wParam == ID_ABOUT){
                lpProc = MakeProcInstance ((FARPROC)About, hInst);
                DialogBox (hInst,
                           "AboutBox",
                           hWnd,
                           lpProc);

                FreeProcInstance (lpProc);
                break;
            }
            else
              return DefWindowProc (hWnd, message, wParam, lParam);

        case WM_COMMAND:
            // LOWORD added for portability
            switch (LOWORD(wParam)){
                 case IDM_EXIT:
                   DestroyWindow (hWnd);
                   break;

                 case IDM_ABOUT:
                   /* Bring up the About.. dialog box */
                   lpProc = MakeProcInstance ((FARPROC)About, hInst);
                   DialogBox (hInst,
                              "AboutBox",
                              hWnd,
                              lpProc);

                   FreeProcInstance (lpProc);
                   break;

                 case IDM_COLOROWNERDR:
                     /* Change colors in color menu depending on state of this
                        menu item. */
                     HandleChangeColors (hWnd);
                     break;

                 case IDM_STATE1:
                 case IDM_STATE2:
                 case IDM_STATE3:
                 case IDM_STATE4:
                      /* Get a handle to the states menu... */
                      hMenu = GetSubMenu (GetMenu (hWnd), IDSTATES_POS);

                      /* Uncheck all the items. */
                      CheckMenuItem (hMenu, IDM_STATE1,
                                     MF_BYCOMMAND | MF_UNCHECKED);
                      CheckMenuItem (hMenu, IDM_STATE2,
                                     MF_BYCOMMAND | MF_UNCHECKED);
                      CheckMenuItem (hMenu, IDM_STATE3,
                                     MF_BYCOMMAND | MF_UNCHECKED);
                      CheckMenuItem (hMenu, IDM_STATE4,
                                     MF_BYCOMMAND | MF_UNCHECKED);

                      /* ...and just check the selected one.*/
                      CheckMenuItem (hMenu, (WORD)wParam,
                                     MF_BYCOMMAND | MF_CHECKED);
                     break;

                 case IDM_BLACK:
                 case IDM_RED:
                 case IDM_BLUE:
                 case IDM_GREEN:
                      /* Get a handle to the Colors menu. */
                      hMenu = GetSubMenu (GetMenu (hWnd),IDCOLORS_POS);

                      /* Uncheck all the items. */
                      CheckMenuItem (hMenu, IDM_BLACK,
                                     MF_BYCOMMAND | MF_UNCHECKED);
                      CheckMenuItem (hMenu, IDM_RED,
                                     MF_BYCOMMAND | MF_UNCHECKED);
                      CheckMenuItem (hMenu, IDM_BLUE,
                                     MF_BYCOMMAND | MF_UNCHECKED);
                      CheckMenuItem (hMenu, IDM_GREEN,
                                     MF_BYCOMMAND | MF_UNCHECKED);

                      /* ...and just check the selected one.*/
                      CheckMenuItem (hMenu, (WORD)wParam,
                                     MF_BYCOMMAND | MF_CHECKED);
                      break;

                 case IDM_FONT:
                      /* Messages sent to us from TrackPopupMenu when
                       * items are selected from the "floating" popups
                       */
                      MessageBox (hWnd,
                                  "A font was selected",
                                  "Popup Menu Alert",
                                  MB_APPLMODAL|MB_OK);
                      break;

                 case IDM_SIZE:
                      MessageBox (hWnd,
                                  "A size was selected",
                                  "Popup Menu Alert",
                                  MB_APPLMODAL|MB_OK);
                      break;

                 case IDM_STYLE:
                      MessageBox (hWnd,
                                  "A style was selected",
                                  "Popup Menu Alert",
                                  MB_APPLMODAL|MB_OK);
                      break;
            }
            break;

        case WM_SIZE:
            if (lParam){
                /* If window is being sized to a non zero value...
                 * invalidate it's client area.
                 */
                InvalidateRect (hWnd, NULL, TRUE);
            }
            break;

        case WM_PAINT:
            HandlePaint (hWnd);
            break;

        case WM_MEASUREITEM:
            /* Use the same width for all items. We could examine the item id
               and use different widths/heights for each item. */
            ((LPMEASUREITEMSTRUCT)lParam)->itemWidth  = MEASUREITEMWIDTH;
            ((LPMEASUREITEMSTRUCT)lParam)->itemHeight = MEASUREITEMHEIGHT;
            return TRUE;

        case WM_DRAWITEM:
            /* Redraw the "colors" menu in normal/ownerdrawmode */
            HandleDrawItem (hWnd,(LPDRAWITEMSTRUCT)lParam);
            return TRUE;
            break;

        case WM_CREATE:
            /* Create the menu */
            HandleCreate (hWnd);
            break;

        case WM_DESTROY:
            /* Delete the on/off bitmaps so that they don't waste memory. */
            DeleteObject (hbmCheckOn);
            DeleteObject (hbmCheckOff);

            PostQuitMessage (0);
            break;

        case WM_LBUTTONDOWN:
            /* Draw the "floating" popup in the app's client area */
            GetClientRect (hWnd, (LPRECT)&rc);

            // Temporary porting macro
            LONG2POINT(lParam, pt);
            if (PtInRect ((LPRECT)&rc, pt))
                HandlePopupMenu (hWnd, pt);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return(0);
}
