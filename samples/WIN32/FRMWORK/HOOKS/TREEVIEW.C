// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: TreeView.c
//
//  PURPOSE: Handles general routines for the TreeView control
//
//  FUNCTIONS:
//    CreateTreeView    - Creates the TreeView control for the sample.
//
//  COMMENTS:
//


#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>           // prototypes and defs for common controls
#include "globals.h"            // prototypes specific to this application
#include "syshook.h"            // defines PGLOBOPTS
#include "treeview.h" 


typedef struct
{
//   char *pszName;
   int   nStringID;
   long  nOffset;
} WP_WORDS_STRUCT, far *LPWP_WORDS_STRUCT;


#define NUM_BITMAPS  4          // number of bitmaps to add to image list.


// Global Variables for the TreeView control
HWND  ghWndTreeView;
int   g_OpenFolder, g_CloseFolder, g_Window, g_Button;

extern int gToolbarHeight, gStatusbarHeight, gHeaderCtlHeight;
extern PGLOBOPTS   pS;


static WP_WORDS_STRUCT Styles[]   = {
   {IDS_WSMAXIMIZEBOXORTAB,WS_MAXIMIZEBOX},        
   {IDS_WSMINIMIZEBOXORGRP,WS_MINIMIZEBOX},        
   {IDS_WSTHICKFRAME,      WS_THICKFRAME},         
   {IDS_WSSYSMENU,         WS_SYSMENU},            
   {IDS_WSHSCROLL,         WS_HSCROLL},            
   {IDS_WSVSCROLL,         WS_VSCROLL},            
   {IDS_WSMAXIMIZE,        WS_MAXIMIZE},
   {IDS_WSCLIPCHILDREN,    WS_CLIPCHILDREN},       
   {IDS_WSCLIPSIBLINGS,    WS_CLIPSIBLINGS},       
   {IDS_WSDISABLED,        WS_DISABLED},           
   {IDS_WSVISIBLE,         WS_VISIBLE},            
   {IDS_WSMINIMIZE,        WS_MINIMIZE},           
   {IDS_WSCHILD,           WS_CHILD},              
   {IDS_WSPOPUP,           WS_POPUP}
   };


static WP_WORDS_STRUCT ExStyles[] = {
   {IDS_WSEXDLGMODALFRAME, WS_EX_DLGMODALFRAME},
   {IDS_WSEXNOPARENTNOTIFY,WS_EX_NOPARENTNOTIFY},
   {IDS_WSEXTOPMOST,       WS_EX_TOPMOST},
   {IDS_WSEXACCEPTFILES,   WS_EX_ACCEPTFILES},
   {IDS_WSEXTRANSPARENT,   WS_EX_TRANSPARENT},

   {IDS_WSEXMDICHILD ,      WS_EX_MDICHILD }
//,   {IDS_WSEXSMCAPTION,      WS_EX_SMCAPTION} These styles were in for M6
//,   {IDS_WSEXABSPOSITION,    WS_EX_ABSPOSITION}                           
   };

static WP_WORDS_STRUCT ClassStyles[] = {
   {IDS_CSVREDRAW,          CS_VREDRAW},
   {IDS_CSHREDRAW,          CS_HREDRAW},
   {IDS_CSKEYCVTWINDOW,     CS_KEYCVTWINDOW},
   {IDS_CSDBLCLKS,          CS_DBLCLKS},
   {IDS_CSOWNDC,            CS_OWNDC},
   {IDS_CSCLASSDC,          CS_CLASSDC},
   {IDS_CSPARENTDC,         CS_PARENTDC},
   {IDS_CSNOKEYCVT,         CS_NOKEYCVT},
   {IDS_CSNOCLOSE,          CS_NOCLOSE},
   {IDS_CSSAVEBITS,         CS_SAVEBITS},
   {IDS_CSBYTEALIGNCLIENT,  CS_BYTEALIGNCLIENT},
   {IDS_CSBYTEALIGNWINDOW,  CS_BYTEALIGNWINDOW},
   {IDS_CSGLOBALCLASS,      CS_GLOBALCLASS}
   };


static WP_WORDS_STRUCT EditStyles[] = {
   {IDS_ESCENTER,            ES_CENTER},
   {IDS_ESRIGHT,             ES_RIGHT},
   {IDS_ESMULTILINE,         ES_MULTILINE},
   {IDS_ESUPPERCASE,         ES_UPPERCASE},
   {IDS_ESLOWERCASE,         ES_LOWERCASE},
   {IDS_ESPASSWORD,          ES_PASSWORD},
   {IDS_ESAUTOVSCROLL,       ES_AUTOVSCROLL},
   {IDS_ESAUTOHSCROLL,       ES_AUTOHSCROLL},
   {IDS_ESNOHIDESEL,         ES_NOHIDESEL},
   {IDS_ESOEMCONVERT,        ES_OEMCONVERT},
   {IDS_ESREADONLY,          ES_READONLY},
   {IDS_ESWANTRETURN,        ES_WANTRETURN}
   };

static WP_WORDS_STRUCT ScrollbarStyles[] = {
   {IDS_SBSVERT,                    SBS_VERT                   },
   {IDS_SBSTOPALIGNORLEFT,          SBS_TOPALIGN               },
   {IDS_SBSBOTTOMALIGNORRIGHT,      SBS_BOTTOMALIGN            },
   {IDS_SBSSIZEBOXTOPLEFTALIGN,     SBS_SIZEBOXTOPLEFTALIGN    },
   {IDS_SBSSIZEBOXBOTTOMRIGHTALIGN, SBS_SIZEBOXBOTTOMRIGHTALIGN},
   {IDS_SBSSIZEBOX,                 SBS_SIZEBOX                } 
   };

static WP_WORDS_STRUCT ListboxStyles[] = {
   {IDS_LBSNOTIFY,           LBS_NOTIFY},
   {IDS_LBSSORT,             LBS_SORT},
   {IDS_LBSNOREDRAW,         LBS_NOREDRAW},
   {IDS_LBSMULTIPLESEL,      LBS_MULTIPLESEL},
   {IDS_LBSOWNERDRAWFIXED,   LBS_OWNERDRAWFIXED},
   {IDS_LBSOWNERDRAWVARIABLE,LBS_OWNERDRAWVARIABLE},
   {IDS_LBSHASSTRINGS,       LBS_HASSTRINGS},
   {IDS_LBSUSETABSTOPS,      LBS_USETABSTOPS},
   {IDS_LBSNOINTEGRALHEIGHT, LBS_NOINTEGRALHEIGHT},
   {IDS_LBSMULTICOLUMN,      LBS_MULTICOLUMN},
   {IDS_LBSWANTKEYBOARDINPUT,LBS_WANTKEYBOARDINPUT},
   {IDS_LBSEXTENDEDSEL,      LBS_EXTENDEDSEL},
   {IDS_LBSDISABLENOSCROLL,  LBS_DISABLENOSCROLL}
//   {IDS_LBSNODATA,           LBS_NODATA}                   
   };

static WP_WORDS_STRUCT ComboboxStyles[] = {
   {IDS_CBSOWNERDRAWFIXED,   CBS_OWNERDRAWFIXED},
   {IDS_CBSOWNERDRAWVARIABLE,CBS_OWNERDRAWVARIABLE},
   {IDS_CBSAUTOHSCROLL,      CBS_AUTOHSCROLL},
   {IDS_CBSOEMCONVERT,       CBS_OEMCONVERT},
   {IDS_CBSSORT,             CBS_SORT},
   {IDS_CBSHASSTRINGS,       CBS_HASSTRINGS},
   {IDS_CBSNOINTEGRALHEIGHT, CBS_NOINTEGRALHEIGHT},
   {IDS_CBSDISABLENOSCROLL,  CBS_DISABLENOSCROLL},
   {IDS_CBSUPPERCASE,        CBS_UPPERCASE},
   {IDS_CBSLOWERCASE,        CBS_LOWERCASE}
   };


static WP_WORDS_STRUCT ButtonStyles[] ={
   {IDS_BSPUSHBUTTON,        BS_PUSHBUTTON},
   {IDS_BSDEFPUSHBUTTON,     BS_DEFPUSHBUTTON},
   {IDS_BSCHECKBOX,          BS_CHECKBOX},
   {IDS_BSAUTOCHECKBOX,      BS_AUTOCHECKBOX},
   {IDS_BSRADIOBUTTON,       BS_RADIOBUTTON},
   {IDS_BS3STATE,            BS_3STATE},
   {IDS_BSAUTO3STATE,        BS_AUTO3STATE},
   {IDS_BSGROUPBOX,          BS_GROUPBOX},
   {IDS_BSUSERBUTTON,        BS_USERBUTTON},
   {IDS_BSAUTORADIOBUTTON,   BS_AUTORADIOBUTTON},
   {0, 0},
   {IDS_BSOWNERDRAW,         BS_OWNERDRAW},
   };

static WP_WORDS_STRUCT ButtonStyles2[] ={
//   {IDS_BSMONO,              BS_MONO}, // not declared in new build
   {IDS_BSRIGHTBUTTON,       BS_RIGHTBUTTON},
   {IDS_BSICON,              BS_ICON},
   {IDS_BSBITMAP,            BS_BITMAP},
   {IDS_BSPUSHLIKE,          BS_PUSHLIKE},
   {IDS_BSMULTILINE,         BS_MULTILINE},
   {IDS_BSNOTIFY,            BS_NOTIFY}
   };


BOOL CALLBACK EnumChildProc  (HWND hWnd, LPARAM lParam);
BOOL CALLBACK EnumThreadProc (HWND hWnd, LPARAM lParam);
HTREEITEM FillWindowInfo (HWND hWndTV, HTREEITEM htiParent, HWND hWnd);
void FillPszText (LPTV_ITEM lptvi, HWND hWndParent, HWND hWndGrandParent);

void AddStyleBitsToTree (HWND hWndTV, 
                         HTREEITEM hWhichParent,
                         HTREEITEM hWhichInsertAfter,
                         int    ilIndex,
                         int    ilSelectedIndex,
                         LONG   lWhichStyleType,
                         HWND   hWnd,
                         LONG   lStyles);



//
//  FUNCTION: MsgNotifyTreeView(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_NOTIFY messages for the TREEVIEW control.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage -
//    wparam   -
//    lparam   -
//
//  RETURN VALUE:
//
//  COMMENTS:
//
LRESULT MsgNotifyTreeView(HWND hwnd, 
                          UINT uMessage, 
                          WPARAM wparam, 
                          LPARAM lparam)
{

    LPNMHDR  lpnmhdr = (LPNMHDR)lparam;


    // Just before the Styles tree item gets EXPANDED,
    // we need to re-add the style bits.

    if (lpnmhdr->code == TVN_ITEMEXPANDING)
    {
        HTREEITEM htiCurrent;
        LPNM_TREEVIEW lpNMTreeView;
        TV_ITEM   tvi;


        lpNMTreeView = (LPNM_TREEVIEW)lparam;
        tvi = lpNMTreeView->itemNew;


        if ((lpNMTreeView->action == TVE_EXPAND) && 
            ((tvi.lParam == WI_STYLES) || 
             (tvi.lParam == WI_EXSTYLES) || 
             (tvi.lParam == CI_STYLES)))

        {


            // Delete all style bits under the Styles root.
            // Loop thru each child under the Styles root, and delete them
            // individually.

           HWND WhichhWnd;
           HTREEITEM hti;
           TV_ITEM _tvi;
           LONG lWhichStyles;


           hti = TreeView_GetParent (ghWndTreeView, tvi.hItem);
           hti = TreeView_GetParent (ghWndTreeView, hti);

           _tvi.hItem = hti;
           _tvi.mask  = TVIF_PARAM;
           TreeView_GetItem (ghWndTreeView, &_tvi);
           WhichhWnd = (HWND)_tvi.lParam;


           htiCurrent = TreeView_GetChild (ghWndTreeView, tvi.hItem);
           while (htiCurrent)
           {
               HTREEITEM htiNext;

               htiNext = TreeView_GetNextSibling (ghWndTreeView, htiCurrent);
               TreeView_DeleteItem (ghWndTreeView, htiCurrent);
               htiCurrent = htiNext;
           }

           // Since Style bits can change on the fly,
           // we want to keep re-adding the style bit items
           // everytime the Styles get displayed.


           if (tvi.lParam == WI_STYLES) 
               lWhichStyles = GetWindowLong (WhichhWnd, GWL_STYLE);

           else if (tvi.lParam == WI_EXSTYLES) 
               lWhichStyles = GetWindowLong (WhichhWnd, GWL_EXSTYLE);

           else if (tvi.lParam == CI_STYLES) 
               lWhichStyles = GetClassLong (WhichhWnd, GCL_STYLE);

           else
               lWhichStyles = 0;



           AddStyleBitsToTree (ghWndTreeView, 
                               tvi.hItem,  
                               TVI_LAST, 
                               g_Button,
                               g_Button,
                               tvi.lParam,
                               WhichhWnd,
                               lWhichStyles);

        }
    }


    // Processing this message on a TVN_ITEMEXPANDED notification
    // to change the iImage/iSelectedImage to the OpenFolder.
    if (lpnmhdr->code == TVN_ITEMEXPANDED)
    {

        LPNM_TREEVIEW lpNMTreeView;
        TV_ITEM       tvi2, tvi3, _tvi;


        lpNMTreeView = (LPNM_TREEVIEW)lparam;
        tvi2 = lpNMTreeView->itemNew;


        _tvi.hItem = tvi2.hItem;
        _tvi.mask  = TVIF_IMAGE;
        TreeView_GetItem (ghWndTreeView, &_tvi);



        // If tree item is EXPANDING (opening up) AND
        // current icon == CloseFolder,
        // change icon to OpenFolder.

        if ((lpNMTreeView->action == TVE_EXPAND) &&
            (_tvi.iImage == g_CloseFolder))
        {        

            tvi3.hItem = tvi2.hItem;
            tvi3.mask  = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            tvi3.iImage        = g_OpenFolder;
            tvi3.iSelectedImage= g_OpenFolder;


            TreeView_SetItem (ghWndTreeView, &tvi3);
        }

        // If tree item is COLLAPSING (closing up) AND
        // current icon == OpenFolder,
        // change icon to CloseFolder.

        else if ((lpNMTreeView->action == TVE_COLLAPSE) &&
            (_tvi.iImage == g_OpenFolder))

        {        

            tvi3.hItem = tvi2.hItem;
            tvi3.mask  = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            tvi3.iImage        = g_CloseFolder;
            tvi3.iSelectedImage= g_CloseFolder;

            TreeView_SetItem (ghWndTreeView, &tvi3);
        }

    }

    // Defining the text for treeItems to be LPSTR_TEXT_CALLBACK
    // causes the TVN_GETDISPINFO t get sent whenever the treeview
    // needs to display an item.
    else if (lpnmhdr->code == TVN_GETDISPINFO)
    {
       TV_DISPINFO FAR *lptvdi;
       TV_ITEM     tvi;

       lptvdi = (TV_DISPINFO FAR *)lparam;
       tvi    = lptvdi->item;


       if (tvi.mask & TVIF_TEXT)
       {
           HWND hWndParent, hWndGrandParent;
           HTREEITEM htiParent;
           TV_ITEM   tvi2;
    
           htiParent  = TreeView_GetParent (ghWndTreeView,tvi.hItem);
           tvi2.hItem = htiParent;
           tvi2.mask  = TVIF_PARAM;
           TreeView_GetItem (ghWndTreeView, &tvi2);
           hWndParent = (HWND)tvi2.lParam;

           htiParent  = TreeView_GetParent (ghWndTreeView,htiParent);
           tvi2.hItem = htiParent;
           tvi2.mask  = TVIF_PARAM;
           TreeView_GetItem (ghWndTreeView, &tvi2);
           hWndGrandParent = (HWND)tvi2.lParam;

           FillPszText (&tvi, hWndParent, hWndGrandParent);

           return 0;
       }
    }

    return DefWindowProc(hwnd, uMessage, wparam, lparam);

}

//
//  FUNCTION: FormatModuleName (HWND hWnd, LPSTR szDestBuf, DWORD dwSize)
//
//  PURPOSE: Formats module name of specified hWnd,
//           truncating the path from the module name returned
//           by the GetModuleFileName () API.
//
//  PARAMETERS:
//    hWnd     - window handle
//    szDestBuf- destination buffer
//    dwSize   - limit string to this size
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//

void FormatModuleName (HWND hWnd, LPSTR szDestBuf, WORD wSize)
{
    DWORD dwResult;
    int   i;
    char  szBuf[200];

    lstrcpy (szBuf, " ");
    dwResult = GetModuleFileName ((HMODULE)GetWindowLong (hWnd, GWL_HINSTANCE),
                                  szBuf, 
                                  sizeof (szBuf));
    if (dwResult)
    {
        for (i = (lstrlen (szBuf)+1); i >= 0; i--)
        {
            if (szBuf[i] == '\\')
               break;
        }

        lstrcpy (szBuf, szBuf+i+1);
    }

    if ((lstrlen (szBuf) + 1) > wSize)
    {
        // just copy 1st wSize characters to fit into szDestBuf

        for (i = 0; i < (wSize-1); i++)
            szDestBuf[i] = szBuf[i];

        szDestBuf [wSize-1] = '\0';
    }
    else
        lstrcpy (szDestBuf, szBuf);
}


//
//  FUNCTION: FormatClassName (HWND hWnd, LPSTR szDestBuf, WORD wSize)
//
//  PURPOSE: Formats class name of specified hWnd
//
//  PARAMETERS:
//    hWnd     - window handle
//    szDestBuf- Destination buffer
//    wSize   - limit string to this size
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//

void FormatClassName (HWND hWnd, LPSTR szDestBuf, WORD wSize)
{
    char gSzBuf [30];
    char szBuf  [30];
    DWORD dwResult;
    int   i;

    if (!hWnd)
       lstrcpy (gSzBuf, "UNDEFINED");
 
    else
    {
        dwResult = GetClassName (hWnd, gSzBuf, sizeof (gSzBuf));
        if (!dwResult)
            lstrcpy (gSzBuf, "\"\"");
    }

    wsprintf (szBuf, "(%s)", gSzBuf);


    if ((lstrlen (szBuf) + 1) > wSize)
    {
        // just copy 1st wSize characters to fit into szDestBuf

        for (i = 0; i < (wSize-1); i++)
            szDestBuf[i] = szBuf[i];

        szDestBuf [wSize-1] = '\0';
    }
    else
        lstrcpy (szDestBuf, szBuf);
}

//
//  FUNCTION: FormatRectText (RECT FAR *lpRect, LPSTR szDestBuf, WORD wSize)
//
//  PURPOSE: Formats rect text into: (x1,y1):(x2,y2)   cx= cy=
//
//  PARAMETERS:
//    lpRect   - long pointer to RECT struct
//    szDestBuf- Destination buffer
//    wSize   - limit string to this size
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//

void FormatRectText (LPRECT lpRect, LPSTR szDestBuf, WORD wSize)
{
    char szBuf  [100];
    int  i;


    wsprintf (szBuf, 
              "(%4d,%4d):(%4d,%4d)   cx= %d cy= %d", 
              lpRect->left, 
              lpRect->top, 
              lpRect->right, 
              lpRect->bottom,
              lpRect->right - lpRect->left,
              lpRect->bottom- lpRect->top);

    if ((lstrlen (szBuf) + 1) > wSize)
    {
        // just copy 1st wSize characters to fit into szDestBuf

        for (i = 0; i < (wSize-1); i++)
            szDestBuf[i] = szBuf[i];

        szDestBuf [wSize-1] = '\0';
    }
    else
        lstrcpy (szDestBuf, szBuf);
}


void ActualFillText (LPSTR lpszBuf, 
                     LPSTR lpszTitle, 
                     LONG  dwHex, 
                     LPSTR lpszText)
{

    char szBuf [50];
	char szExceptionBuf[] = "\0";

	// there's been some problems with this being a bad string pointer. 
	// hopefully this will take care of the problem.
	if( IsBadStringPtr( lpszText, 50 ))
	{
		lpszText = &(szExceptionBuf[0]);
	}			 

    // Concat approp. number of spaces to lpszTitle 
    // to align information nicely.
    lstrcpy (szBuf, lpszTitle);

    // Append a '-' if there's a dwHex or an lpszText following
    // the title.
    if ((dwHex != -1) || (lpszText))
        lstrcat (szBuf, " -");

    // A dwHex value of -1 indicates that no hex value needs to be filled.
    if (dwHex != -1)
        wsprintf (lpszBuf, "%s 0x%04x %s", szBuf, dwHex, lpszText);
    else
        wsprintf (lpszBuf, "%s %s",        szBuf, lpszText);
    
}


//
//  FUNCTION: FillPszText (LPTV_ITEM, HWND, HWND)
//
//  PURPOSE: One big CASE statement which, depending on the value of 
//           lptvi->lParam, fills in the appropriate pszText field
//           of the TV_ITEM structure.
//
//  PARAMETERS:
//    lptvi - long pointer to TVI struct, whose pszText field
//            we need to fill in this function
//    hWndParent      - window handle
//    hWndGrandParent - window handle
//
//  RETURN VALUE:
//    void
//
//  COMMENTS:
//    Function is called when parent receives a WM_NOTIFY
//    with code set to TVN_GETDISPINFO to define text
//    for a particular tree item.
//

void FillPszText (LPTV_ITEM lptvi, HWND hWndParent, HWND hWndGrandParent)
{
    if (lptvi->lParam == WI_ROOT) 
    {
        ActualFillText (lptvi->pszText, 
                        "Window Info",
                        -1,
                        NULL);
    }
    else if (lptvi->lParam == CI_ROOT) 
    {
        char szBuf[30];

        FormatClassName (hWndParent, szBuf, sizeof (szBuf));
        ActualFillText (lptvi->pszText, 
                        "Class Info",
                        -1,
                        szBuf);

    }

    else if (lptvi->lParam == WI_NEXT)
    {
        HWND hWnd = (HWND)GetWindow(hWndGrandParent, GW_HWNDNEXT);
        char szBuf [30];

        FormatClassName (hWnd, szBuf, sizeof (szBuf));
        ActualFillText (lptvi->pszText, 
                        "Next",
                        (LONG)hWnd,
                        szBuf);
    }

    else if (lptvi->lParam == WI_PREV)
    {
        HWND hWnd = (HWND)GetWindow(hWndGrandParent, GW_HWNDPREV);
        char szBuf [30];

        FormatClassName (hWnd, szBuf, sizeof (szBuf));
        ActualFillText (lptvi->pszText, 
                        "Prev",
                        (LONG)hWnd,
                        szBuf);
    }

    else if (lptvi->lParam == WI_PARENT)
    {
        HWND hWnd = (HWND)GetWindowLong (hWndGrandParent, GWL_HWNDPARENT);
        char szBuf [30];

        FormatClassName (hWnd, szBuf, sizeof (szBuf));
        ActualFillText (lptvi->pszText, 
                        "Parent",
                        (LONG)hWnd,
                        szBuf);
    }

    else if (lptvi->lParam == WI_OWNER )
    {
        HWND hWnd = (HWND)GetWindow (hWndGrandParent, GW_OWNER);
        char szBuf [30];

        FormatClassName (hWnd, szBuf, sizeof (szBuf));
        ActualFillText (lptvi->pszText, 
                        "Owner",
                        (LONG)hWnd,
                        szBuf);
    }

    else if (lptvi->lParam == WI_TASK)
    {
        ActualFillText (lptvi->pszText, 
                        "Task",
                        (LONG)GetWindowTask (hWndGrandParent),
                        NULL);
    }

    else if (lptvi->lParam == WI_INSTANCE)
    {
        ActualFillText (lptvi->pszText, 
                        "Instance",
                        GetWindowLong (hWndGrandParent, GWL_HINSTANCE),
                        NULL);
    }

    else if (lptvi->lParam == WI_WNDPROC)
    {
        ActualFillText (lptvi->pszText, 
                        "WndProc",
                        GetWindowLong (hWndGrandParent, GWL_WNDPROC),
                        NULL);
    }

    else if (lptvi->lParam == WI_STYLES)
    {
        ActualFillText (lptvi->pszText, 
                        "Styles",
                        GetWindowLong (hWndGrandParent, GWL_STYLE),
                        NULL);
    }

    else if (lptvi->lParam == WI_EXSTYLES)
    {
        ActualFillText (lptvi->pszText, 
                        "ExStyles",
                        GetWindowLong (hWndGrandParent, GWL_EXSTYLE),
                        NULL);
    }

    else if (lptvi->lParam == WI_WNDRECT)
    {
        RECT rect;
        char szBuf[50];

        GetWindowRect (hWndGrandParent, &rect);
        FormatRectText (&rect, szBuf, sizeof (szBuf));
        ActualFillText (lptvi->pszText, 
                        "WndRect",
                        -1,
                        szBuf);
    }

    else if (lptvi->lParam == WI_CLTRECT)
    {
        RECT rect;
        char szBuf [50];

        GetClientRect (hWndGrandParent, &rect);
        FormatRectText (&rect, szBuf, sizeof (szBuf));
        ActualFillText (lptvi->pszText, 
                        "CltRect",
                        -1,
                        szBuf);
    }

    else if (lptvi->lParam == WI_MODULE) 
    {
        char szBuf [30];

        FormatModuleName (hWndGrandParent, szBuf, sizeof (szBuf));
        ActualFillText (lptvi->pszText, 
                        "Module", 
                        -1, 
                        szBuf);
    }

    else if (lptvi->lParam == CI_STYLES)
    {
        ActualFillText (lptvi->pszText, 
                        "Styles", 
                        GetClassLong (hWndGrandParent, GCL_STYLE), 
                        NULL);
    }

    else if (lptvi->lParam == CI_HICON)
    {
        ActualFillText (lptvi->pszText, 
                        "hIcon", 
                        GetClassLong (hWndGrandParent, GCL_HICON), 
                        NULL);
    }

    else if (lptvi->lParam == CI_HCURSOR)
    {
        ActualFillText (lptvi->pszText, 
                        "hCursor", 
                        GetClassLong (hWndGrandParent, GCL_HCURSOR), 
                        NULL);
    }

    else if (lptvi->lParam == CI_HBACKGRND)
    {
        ActualFillText (lptvi->pszText, 
                        "hBrBackgrnd", 
                        GetClassLong (hWndGrandParent, GCL_HBRBACKGROUND), 
                        NULL);
    }

    else if (lptvi->lParam == CI_CLSEXTRA)
    {
        ActualFillText (lptvi->pszText, 
                        "ClsExtra", 
                        GetClassLong (hWndGrandParent, GCL_CBCLSEXTRA), 
                        NULL);
    }

    else if (lptvi->lParam == CI_WNDEXTRA)
    {
        ActualFillText (lptvi->pszText, 
                        "WndExtra", 
                        GetClassLong (hWndGrandParent, GCL_CBWNDEXTRA), 
                        NULL);
    }

    else if (lptvi->lParam == CI_MENUNAME)
    {
        ActualFillText (lptvi->pszText, 
                        "MenuName", 
                        -1,
                        (LPSTR)GetClassLong (hWndGrandParent, GCL_MENUNAME)); 
    }

    else if (lptvi->lParam == CI_HICONSM)
    {
        ActualFillText (lptvi->pszText, 
                        "hSmallIcon", 
                        GetClassLong (hWndGrandParent, GCL_HICONSM), 
                        NULL);
    }

    else 
    {
        ActualFillText (lptvi->pszText, 
                        "???", 
                        -1, 
                        NULL);
    }


    lptvi->cchTextMax = lstrlen (lptvi->pszText);
}

//
//  FUNCTION: InitImageList (HWND)
//
//  PURPOSE:  Creates image list, adds 3 bitmaps to it, and associates
//            the image list with the treeview control.
//
//
//  PARAMETERS:
//
//  hwnd -    TreeView Window handle
//
//  RETURN VALUE:
//
//  If successful, return TRUE,
//            else returns FALSE.
//
//  COMMENTS:
//
//

LRESULT InitImageList (HWND hWndParent)
{
    HIMAGELIST  hWndImageList;
    HBITMAP     hBmp;


    hWndImageList = ImageList_Create (GetSystemMetrics (SM_CXICON)/2, 
                                      GetSystemMetrics (SM_CYICON)/2,
                                      TRUE,
                                      NUM_BITMAPS, 
                                      8);
    if (!hWndImageList)
        return FALSE;


    hBmp = LoadBitmap (hInst, "Folders");
    g_CloseFolder = ImageList_AddMasked (hWndImageList, hBmp, RGB (255,0,0));

    g_OpenFolder = g_CloseFolder+1;
    g_Window     = g_CloseFolder+2;
    g_Button     = g_CloseFolder+3;

    // Fail if not all images were added.
    if (ImageList_GetImageCount (hWndImageList) < NUM_BITMAPS)
    {
        // ERROR: Unable to add all images to image list.
        return FALSE;
    }

    // Associate image list with treeView control.
    TreeView_SetImageList (hWndParent, hWndImageList, TVSIL_NORMAL);
    return TRUE;
}

//
//  FUNCTION: AddItemToTree (HWND, HTREEITEM, HTREEITEM, LPSTR,int,int,LPARAM)
//
//  PURPOSE:  Adds items to a TreeView control
//            (Fills in a TV_ITEM structure)
//
//  PARAMETERS:
//      hWndTV - handle to Treeview control
//      hWhichParent - for the hParent field of the TV_ITEM struct
//      hWhichInsertAfter - for the hInsertAfter field
//      lpszText - fills in the pszText field
//      ilIndex  - fills in the iImage field
//      ilSelectedIndex - fills in the iSelectedImage
//      lParam   - fills in the lParam field
//
//  RETURN VALUE:
//      HTREEITEM - handle to the item that just got inserted to the tree.
//
//  COMMENTS:
//
//

HTREEITEM AddItemToTree (HWND hWndTV, 
                         HTREEITEM hWhichParent,
                         HTREEITEM hWhichInsertAfter,
                         LPSTR  lpszText,
                         int    ilIndex,
                         int    ilSelectedIndex,
                         LPARAM lParam)
{
    TV_ITEM  tvi;
    TV_INSERTSTRUCT  tvins;
    HTREEITEM hTreeItem = NULL;


    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

    // Set the text of the item.
    tvi.pszText        = lpszText;
    tvi.iImage         = ilIndex;
    tvi.iSelectedImage = ilSelectedIndex;
    tvi.lParam         = lParam;

    tvins.item         = tvi;
    tvins.hParent      = hWhichParent;
    tvins.hInsertAfter = hWhichInsertAfter;

    
    hTreeItem = TreeView_InsertItem (hWndTV,
                                    (LPARAM)(LPTV_INSERTSTRUCT) &tvins);
    return hTreeItem;
}


//
//  FUNCTION: LoadStringAndAddToTree (int, HWND, HTREEITEM,HTREEITEM,int,int)
//
//  PURPOSE:  Loads the appropriate string from the StringTable
//            and calls AddItemToTree().
//            Called by each of the Decode??Styles() function.
//            
//
//  PARAMETERS:
//      nStringID - StringTable ID
//      hWndTV    - handle to Treeview Control
//      hWhichParent - for the hParent field of the TV_ITEM struct
//      hWhichInsertAfter - for the hInsertAfter field
//      lpszText - fills in the pszText field
//      ilIndex  - fills in the iImage field
//      ilSelectedIndex - fills in the iSelectedImage
//
//  RETURN VALUE:
//      NONE
//
//  COMMENTS:
//
//

void LoadStringAndAddToTree (int nStringID,
                             HWND hWndTV, 
                             HTREEITEM hWhichParent,
                             HTREEITEM hWhichInsertAfter,
                             int    ilIndex,
                             int    ilSelectedIndex)
{
    char szBuf [80];
    if (!LoadString (hInst, nStringID, szBuf, sizeof (szBuf)))
        lstrcpy (szBuf, "???");

    AddItemToTree (hWndTV,
                   hWhichParent,
                   hWhichInsertAfter,
                   szBuf,
                   ilIndex,
                   ilSelectedIndex,
                   0L);
}




//
//  FUNCTION: DecodeWindowStyles (HWND,HTREEITEM,HTREEITEM,int,int,LONG)
//
//  PURPOSE:  
//      Decodes the window style and adds the appropriate string to tree.
//
//  PARAMETERS:
//      hWndTV    - handle to Treeview Control
//      hWhichParent - for the hParent field of the TV_ITEM struct
//      hWhichInsertAfter - for the hInsertAfter field
//      lpszText - fills in the pszText field
//      ilIndex  - fills in the iImage field
//      ilSelectedIndex - fills in the iSelectedImage
//
//
//  RETURN VALUE:
//      NONE
//
//
//  COMMENTS:
//
//

void DecodeWindowStyles (HWND hWndTV, 
                   HTREEITEM hWhichParent,
                   HTREEITEM hWhichInsertAfter,
                   int    ilIndex,
                   int    ilSelectedIndex,
                   LONG   lStyles)
{

    int i, nStringID = 0;

    //
    //  Some styles are special, and can't be handled by our table (i.e.
    //  some styles use the same bits, but depend on other criteria to
    //  figure out what they really mean.
    // 
    if ((lStyles & WS_CAPTION) == WS_CAPTION)
        nStringID = IDS_WSCAPTION;

    else 
    {
        if (lStyles & WS_BORDER)
            nStringID = IDS_WSBORDER;

        if (lStyles & WS_DLGFRAME)
            nStringID = IDS_WSDLGFRAME;
    }

    if (nStringID)
        LoadStringAndAddToTree (nStringID,
                                hWndTV,
                                hWhichParent,
                                hWhichInsertAfter,
                                ilIndex,
                                ilSelectedIndex);

    //
    //  Now, do the styles that are mutually exclusive, using our table.
    // 


    for (i = 0;   i < sizeof(Styles) / sizeof (Styles[0]);  i++)

        if (lStyles & Styles[i].nOffset)
            LoadStringAndAddToTree (Styles[i].nStringID, 
                                    hWndTV,
                                    hWhichParent,
                                    hWhichInsertAfter,
                                    ilIndex,
                                    ilSelectedIndex);

}

//
//  FUNCTION: DecodeWindowExStyles (HWND,HTREEITEM,HTREEITEM,int,int,LONG)
//
//  PURPOSE:  
//      Decodes the window EXTENDED styles and adds the appropriate string to 
//      the tree.
//
//  PARAMETERS:
//      hWndTV    - handle to Treeview Control
//      hWhichParent - for the hParent field of the TV_ITEM struct
//      hWhichInsertAfter - for the hInsertAfter field
//      lpszText - fills in the pszText field
//      ilIndex  - fills in the iImage field
//      ilSelectedIndex - fills in the iSelectedImage
//
//
//  RETURN VALUE:
//      NONE
//
//
//  COMMENTS:
//
//

void DecodeWindowExStyles (HWND hWndTV, 
                           HTREEITEM hWhichParent,
                           HTREEITEM hWhichInsertAfter,
                           int    ilIndex,
                           int    ilSelectedIndex,
                           LONG   lStyles)
{
    int i, nStringID=0;

    /*
     *  Some styles are special, and can't be handled by our table (i.e.
     *  some styles use the same bits, but depend on other criteria to
     *  figure out what they really mean.
     */
//   WS_EX_EDGEMASK is undeclared for Build 197
//   Commented out code here.
//    if ((lStyles & WS_EX_EDGEMASK) == WS_EX_EDGEMASK)
//        nStringID = IDS_WSEXEDGEMASK;
//
//    else 
//    {
        if (lStyles & WS_EX_WINDOWEDGE)
            nStringID = IDS_WSEXWINDOWEDGE;

        if (lStyles & WS_EX_CLIENTEDGE)
            nStringID = IDS_WSEXCLIENTEDGE;
//    }

    if (nStringID)
        LoadStringAndAddToTree (nStringID,
                                hWndTV,
                                hWhichParent,
                                hWhichInsertAfter,
                                ilIndex,
                                ilSelectedIndex);

    nStringID = 0;
    if (lStyles & WS_EX_RIGHT)
        nStringID = IDS_WSEXRIGHT;
    else
        nStringID = IDS_WSEXLEFT;

    if (nStringID)
        LoadStringAndAddToTree (nStringID,
                                hWndTV,
                                hWhichParent,
                                hWhichInsertAfter,
                                ilIndex,
                                ilSelectedIndex);

    nStringID = 0;
    if (lStyles & WS_EX_RTLREADING)
        nStringID = IDS_WSEXRTLREADING;
    else
        nStringID = IDS_WSEXLTRREADING;

    if (nStringID)
        LoadStringAndAddToTree (nStringID,
                                hWndTV,
                                hWhichParent,
                                hWhichInsertAfter,
                                ilIndex,
                                ilSelectedIndex);

    nStringID = 0;
    if (lStyles & WS_EX_LEFTSCROLLBAR)
        nStringID = IDS_WSEXLEFTSCROLLBAR;
    else
        nStringID = IDS_WSEXRIGHTSCROLLBAR;

    if (nStringID)
        LoadStringAndAddToTree (nStringID,
                                hWndTV,
                                hWhichParent,
                                hWhichInsertAfter,
                                ilIndex,
                                ilSelectedIndex);


    /*
     *  Now, do the styles that are mutually exclusive, using our table.
     */

    for (i = 0;   i < sizeof(ExStyles) / sizeof (ExStyles[0]);  i++)

        if (lStyles & ExStyles[i].nOffset)
            LoadStringAndAddToTree (ExStyles[i].nStringID, 
                                    hWndTV,
                                    hWhichParent,
                                    hWhichInsertAfter,
                                    ilIndex,
                                    ilSelectedIndex);
}


//
//  FUNCTION: DecodeCtrlStyles (HWND,HTREEITEM,HTREEITEM,int,int,LONG)
//
//  PURPOSE:  
//      Decodes the style for each predefined Windows control, 
//      and adds the appropriate string to tree.
//
//  PARAMETERS:
//      hWndTV    - handle to Treeview Control
//      hWhichParent - for the hParent field of the TV_ITEM struct
//      hWhichInsertAfter - for the hInsertAfter field
//      ilIndex  - fills in the iImage field
//      ilSelectedIndex - fills in the iSelectedImage
//      hWnd     - window handle used to obtain ClassName
//                 to figure out what type of control we're dealing with.
//      lStyles  - window style to be decoded
//
//  RETURN VALUE:
//      NONE
//
//
//  COMMENTS:
//
//

void DecodeCtrlStyles (HWND hWndTV, 
                          HTREEITEM hWhichParent,
                          HTREEITEM hWhichInsertAfter,
                          int    ilIndex,
                          int    ilSelectedIndex,
                          HWND   hWnd,
                          LONG   lStyles)
{
    int i;
    char szBuf[30];

    GetClassName (hWnd, szBuf, sizeof (szBuf));



    if (!lstrcmpi (szBuf, "EDIT"))
    {
        // Decode Edit Control Styles  (ES_*)

        for (i = 0;   i < sizeof(EditStyles) / sizeof (EditStyles[0]);  i++)
    
            if (lStyles & EditStyles[i].nOffset)
                LoadStringAndAddToTree (EditStyles[i].nStringID, 
                                        hWndTV,
                                        hWhichParent,
                                        hWhichInsertAfter,
                                        ilIndex,
                                        ilSelectedIndex);
    }

    if (!lstrcmpi (szBuf, "SCROLLBAR"))
    {
        // Decode Scrollbar Styles  (SBS_*)
    
        for (i = 0;   
             i < sizeof(ScrollbarStyles) / sizeof (ScrollbarStyles[0]);  
             i++)
    
            if (lStyles & ScrollbarStyles[i].nOffset)
                LoadStringAndAddToTree (ScrollbarStyles[i].nStringID, 
                                        hWndTV,
                                        hWhichParent,
                                        hWhichInsertAfter,
                                        ilIndex,
                                        ilSelectedIndex);
    }

    if (!lstrcmpi (szBuf, "LISTBOX"))
    {
        // Decode Listbox Styles  (LBS_*)
    
        for (i = 0;   
             i < sizeof(ListboxStyles) / sizeof (ListboxStyles[0]);  
             i++)
    
            if (lStyles & ListboxStyles[i].nOffset)
                LoadStringAndAddToTree (ListboxStyles[i].nStringID, 
                                        hWndTV,
                                        hWhichParent,
                                        hWhichInsertAfter,
                                        ilIndex,
                                        ilSelectedIndex);
    }

    if (!lstrcmpi (szBuf, "COMBOBOX"))
    {
        int nStringID=0;

        // Decode ComboBox Styles  (CBS_*)
    
        /*
         *  Some styles are special, and can't be handled by our table (i.e.
         *  some styles use the same bits, but depend on other criteria to
         *  figure out what they really mean.
         */
        if ((lStyles & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
            nStringID = IDS_CBSDROPDOWNLIST;
    
        else 
        {
            if (lStyles & CBS_SIMPLE)
                nStringID = IDS_CBSSIMPLE;
    
            if (lStyles & CBS_DROPDOWN)
                nStringID = IDS_CBSDROPDOWN;
        }

        if (nStringID)
            LoadStringAndAddToTree (nStringID,
                                    hWndTV,
                                    hWhichParent,
                                    hWhichInsertAfter,
                                    ilIndex,
                                    ilSelectedIndex);

        /*
         *  Now, do the styles that are mutually exclusive, using our table.
         */

        for (i = 0;   
             i < sizeof(ComboboxStyles) / sizeof (ComboboxStyles[0]);  
             i++)
    
            if (lStyles & ComboboxStyles[i].nOffset)
                LoadStringAndAddToTree (ComboboxStyles[i].nStringID, 
                                        hWndTV,
                                        hWhichParent,
                                        hWhichInsertAfter,
                                        ilIndex,
                                        ilSelectedIndex);
    }



    if (!lstrcmpi (szBuf, "BUTTON"))
    {
        int nStringID=0;


        /*
         *  Some styles are special, and can't be handled by our table (i.e.
         *  some styles use the same bits, but depend on other criteria to
         *  figure out what they really mean.
         */

        if ((lStyles & 0x0F) < 
            (sizeof (ButtonStyles) / sizeof (ButtonStyles[0])))
        {
            nStringID = ButtonStyles [lStyles & 0x0F].nStringID;
            LoadStringAndAddToTree (nStringID,
                                    hWndTV,
                                    hWhichParent,
                                    hWhichInsertAfter,
                                    ilIndex,
                                    ilSelectedIndex);
        }


        nStringID = 0;
        if ((lStyles & BS_CENTER) == BS_CENTER)
            nStringID = IDS_BSCENTER;
    
        else 
        {
            if (lStyles & BS_LEFT)
                nStringID = IDS_BSLEFT;
    
            if (lStyles & BS_RIGHT)
                nStringID = IDS_BSRIGHT;
        }

        if (nStringID)
            LoadStringAndAddToTree (nStringID,
                                    hWndTV,
                                    hWhichParent,
                                    hWhichInsertAfter,
                                    ilIndex,
                                    ilSelectedIndex);

        nStringID = 0;
        if ((lStyles & BS_VCENTER) == BS_VCENTER)
            nStringID = IDS_BSVCENTER;
    
        else 
        {
            if (lStyles & BS_TOP)
                nStringID = IDS_BSTOP;
    
            if (lStyles & BS_BOTTOM)
                nStringID = IDS_BSBOTTOM;
        }

        if (nStringID)
            LoadStringAndAddToTree (nStringID,
                                    hWndTV,
                                    hWhichParent,
                                    hWhichInsertAfter,
                                    ilIndex,
                                    ilSelectedIndex);

        for (i = 0;   
             i < sizeof(ButtonStyles2) / sizeof (ButtonStyles2[0]);  
             i++)
    
            if (lStyles & ButtonStyles2[i].nOffset)
                LoadStringAndAddToTree (ButtonStyles2[i].nStringID, 
                                        hWndTV,
                                        hWhichParent,
                                        hWhichInsertAfter,
                                        ilIndex,
                                        ilSelectedIndex);
    }

    if (!lstrcmpi (szBuf, "STATIC"))
    {
    }


}


//
//  FUNCTION: AddStyleBitsToTree (HWND,HTREEITEM,HTREEITEM,int,int,LONG,
//                                HWND,LONG)
//
//  PURPOSE:  Adds appropriate window style bits to the tree.
//
//  PARAMETERS:
//      hWndTV    - handle to Treeview Control
//      hWhichParent - for the hParent field of the TV_ITEM struct
//      hWhichInsertAfter - for the hInsertAfter field
//      ilIndex  - fills in the iImage field
//      ilSelectedIndex - fills in the iSelectedImage
//      lWhichStyleType - whether WI_STYLE, WI_EXSTYLE, or CI_STYLE
//      hWnd     - window handle used to obtain ClassName
//                 to figure out what type of control we're dealing with.
//      lStyles  - window style to be decoded
//
//
//  RETURN VALUE:
//      NONE
//
//  COMMENTS:
//
//

void AddStyleBitsToTree (HWND hWndTV, 
                         HTREEITEM hWhichParent,
                         HTREEITEM hWhichInsertAfter,
                         int    ilIndex,
                         int    ilSelectedIndex,
                         LONG   lWhichStyleType,
                         HWND   hWnd,
                         LONG   lStyles)
{
    if (lWhichStyleType == WI_STYLES)
    {

        // Decode WINDOW STYLES  (WS_*) 

        DecodeWindowStyles (hWndTV,
                            hWhichParent,
                            hWhichInsertAfter,
                            ilIndex,
                            ilSelectedIndex,
                            lStyles);

        DecodeCtrlStyles (hWndTV,
                          hWhichParent,
                          hWhichInsertAfter,
                          ilIndex,
                          ilSelectedIndex,
                          hWnd,
                          lStyles);

    }

    else if (lWhichStyleType == WI_EXSTYLES)
    {
        // Decode EXTENDED WINDOW STYLES  (WS_EX_*) 

        DecodeWindowExStyles (hWndTV,
                            hWhichParent,
                            hWhichInsertAfter,
                            ilIndex,
                            ilSelectedIndex,
                            lStyles);
    }

    else if (lWhichStyleType == CI_STYLES)
    {

        // Decode CLASS STYLES  (CS_*) 

        int i;

        for (i = 0;   i < sizeof(ClassStyles) / sizeof (ClassStyles[0]);  i++)

            if (lStyles & ClassStyles[i].nOffset)
                LoadStringAndAddToTree (ClassStyles[i].nStringID, 
                                        hWndTV,
                                        hWhichParent,
                                        hWhichInsertAfter,
                                        ilIndex,
                                        ilSelectedIndex);

    }



}

//
//  FUNCTION: InitTreeViewItems (HWND)
//
//  PURPOSE:  
//      Initializes items of the treeview control.          
//
//  PARAMETERS:
//
//  hwnd - TreeView Window handle
//
//  RETURN VALUE:
//
//  If successful, return TRUE,
//            else returns FALSE.
//
//  COMMENTS:
//
//

LRESULT InitTreeViewItems (HWND hWndTV, HWND hWndToBrowse)
{

   HTREEITEM htiParent;
   
   // FOR TESTING ONLY----------------------------
   // hWndToBrowse = FindWindow ("NOTEPAD", NULL);
   // pS->hWndMonitored = hWndToBrowse;
   // TESTING ONLY--------------------------------


   TreeView_DeleteAllItems (hWndTV);
   if (hWndToBrowse)
   {
       htiParent = FillWindowInfo (hWndTV, NULL, hWndToBrowse);
//       EnumChildWindows (hWndToBrowse, EnumChildProc, (LPARAM)htiParent);
//       EnumThreadWindows (GetWindowThreadProcessId (hWndToBrowse, NULL), 
//                          EnumThreadProc, 
//                          (LPARAM)htiParent);
   }

   return TRUE;
}

//
//  FUNCTION: EnumChildProc (hWnd, lParam)
//
//  PURPOSE:  
//
//  PARAMETERS:
//
//  hwnd   - TreeView Window handle
//  lparam - 32-bit app-defined value
//         - in our case, the HTREEITEM of the parent window
//
//  RETURN VALUE:
//
//  To continue enumeration, the callback function returns TRUE;
//  To stop enumeration, the callback returns FALSE.
//
//  COMMENTS:
//
//

BOOL CALLBACK EnumChildProc (HWND hWnd, LPARAM lParam)
{
    HTREEITEM hti;


    // EnumThreadWindows will enum even the root window,
    // and we don't need that.
    if (hWnd != pS->hWndMonitored)
    {
        hti = FillWindowInfo (ghWndTreeView, (HTREEITEM)lParam, hWnd);
        //EnumChildWindows (hWnd, EnumChildProc, (LPARAM)hti);
    }

    return TRUE;
}

//
//  FUNCTION: EnumThreadProc (hWnd, lParam)
//
//  PURPOSE:  
//
//  PARAMETERS:
//
//  hwnd   - TreeView Window handle
//  lparam - 32-bit app-defined value
//         - in our case, the HTREEITEM of the parent window
//
//  RETURN VALUE:
//
//  To continue enumeration, the callback function returns TRUE;
//  To stop enumeration, the callback returns FALSE.
//
//  COMMENTS:
//
//

BOOL CALLBACK EnumThreadProc (HWND hWnd, LPARAM lParam)
{
    HTREEITEM hti;


    // EnumThreadWindows will enum even the root window,
    // and we don't need that.
    if (hWnd != pS->hWndMonitored)
    {
        hti = FillWindowInfo (ghWndTreeView, (HTREEITEM)lParam, hWnd);
//        EnumChildWindows (hWnd, EnumChildProc, (LPARAM)hti);
    }

    return TRUE;
}


//  FUNCTION: FillWindowInfo (HWND, HTREEITEM, HWND)
//
//  PURPOSE:  
//      Fills the tree with necessary window information
//
//  PARAMETERS:
//
//      hwndTV - TreeView Window handle
//      htiParent - Adds root item under this parent.
//      hWnd   - Window we're dumping information about.
//
//  RETURN VALUE:
//      htiHWND - handle to tree item for the root
//
//  COMMENTS:
//
//

HTREEITEM FillWindowInfo (HWND hWndTV, HTREEITEM htiParent, HWND hWnd)
{
    HTREEITEM htiHWND, htiWINFO, htiCINFO, htiStyles, htiExStyles;
    int   g_Icon, g_WhichIcon;
    char  szBuf[80], szBuf2 [80];
    HIMAGELIST  hWndImageList;
    HICON hIcon;

    hWndImageList = TreeView_GetImageList (hWndTV, 0);

    hIcon = (HICON)GetClassLong (hWnd, GCL_HICON);
    if (hIcon)
        g_Icon = ImageList_AddIcon (hWndImageList, 
                                    (HICON)hIcon);


    // Fill in the ROOT's text here.
    // All the rest, we initialize pszText to LPSTR_TEXTCALLBACK.
    if (!GetWindowText (hWnd, szBuf, sizeof (szBuf)))
        lstrcpy (szBuf, "No Title");
    wsprintf (szBuf2, "0x%04x - %s", hWnd, szBuf);

    // If the selected window has an icon, use that icon.
    // Otherwise, use the generic window icon.
    if (hIcon)
        g_WhichIcon = g_Icon;
    else
        g_WhichIcon = g_Window;
    htiHWND = AddItemToTree (hWndTV, 
                             htiParent, 
                             TVI_LAST, 
                             szBuf2,
                             g_WhichIcon, 
                             g_WhichIcon, 
                             (LPARAM)hWnd);

    htiWINFO= AddItemToTree (hWndTV, 
                             htiHWND,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_CloseFolder,
                             g_CloseFolder,
                             (LPARAM)WI_ROOT);
              AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Window,
                             g_Window,
                             (LPARAM)WI_NEXT);
              AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Window,
                             g_Window,
                             (LPARAM)WI_PREV);
              AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Window,
                             g_Window,
                             (LPARAM)WI_PARENT);
              AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Window,
                             g_Window,
                             (LPARAM)WI_OWNER);
              AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)WI_TASK);
              AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)WI_MODULE);
              AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)WI_INSTANCE);

              AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)WI_WNDPROC);
              AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)WI_WNDRECT);
              AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)WI_CLTRECT);

    htiStyles=AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_CloseFolder,
                             g_CloseFolder,
                             (LPARAM)WI_STYLES);

              // Add a dummy window style under Styles.
              // Otherwise, the Styles item does not come up
              // with a button to expand/collapse.

              AddItemToTree (hWndTV, 
                             htiStyles,  
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)WI_STYLES);

    htiExStyles=AddItemToTree (hWndTV, 
                             htiWINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_CloseFolder,
                             g_CloseFolder,
                             (LPARAM)WI_EXSTYLES);


              // Add a dummy window style under ExStyles.
              // Otherwise, the Styles item does not come up
              // with a button to expand/collapse.

              AddItemToTree (hWndTV, 
                             htiExStyles,  
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)WI_EXSTYLES);

    htiCINFO= AddItemToTree (hWndTV,
                             htiHWND,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_CloseFolder,
                             g_CloseFolder,
                             (LPARAM)CI_ROOT);

    htiStyles=AddItemToTree (hWndTV, 
                             htiCINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_CloseFolder,
                             g_CloseFolder,
                             (LPARAM)CI_STYLES);

              // Add a dummy window style under Class Styles.
              // Otherwise, the Styles item does not come up
              // with a button to expand/collapse.

              AddItemToTree (hWndTV, 
                             htiStyles,  
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)CI_STYLES);



              AddItemToTree (hWndTV, 
                             htiCINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)CI_HICON);
              AddItemToTree (hWndTV, 
                             htiCINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)CI_HICONSM);
              AddItemToTree (hWndTV, 
                             htiCINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)CI_HCURSOR);
              AddItemToTree (hWndTV, 
                             htiCINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)CI_HBACKGRND);
              AddItemToTree (hWndTV, 
                             htiCINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)CI_CLSEXTRA);

              AddItemToTree (hWndTV, 
                             htiCINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)CI_WNDEXTRA);

              AddItemToTree (hWndTV, 
                             htiCINFO,   
                             TVI_LAST, 
                             LPSTR_TEXTCALLBACK,
                             g_Button,
                             g_Button,
                             (LPARAM)CI_MENUNAME);
    return htiHWND;
}


//
//  FUNCTION: CreateTreeView (HWND)
//
//  PURPOSE:  Calls CreateWindow (,WC_TREEVIEW,..)
//
//
//  PARAMETERS:
//
//  hwnd - Window handle : Used for the hWndParent parameter of the control.
//
//  RETURN VALUE:
//
//  If toolbar control was created successfully Return TRUE,
//  else returns FALSE.
//
//  COMMENTS:
//
//

LRESULT CreateTreeView (HWND hWndParent)

{
    RECT  rcClient;

    // Get dimensions of parent window's client area.
    GetClientRect (hWndParent, &rcClient);


    // Create TreeView control.
    ghWndTreeView = CreateWindowEx (WS_EX_CLIENTEDGE,
                                    WC_TREEVIEW,
                                    "Tree View",
                                    //WS_VISIBLE |
                                    WS_CHILD |
                                    WS_BORDER |
                                    TVS_HASLINES |
                                    TVS_LINESATROOT |
                                    TVS_HASBUTTONS,
                                    0,
                                    gToolbarHeight,
                                    rcClient.right,
                                    rcClient.bottom - 
                                    gToolbarHeight -
                                    gStatusbarHeight,
                                    hWndParent,
                                    (HMENU)IDM_TREEVIEW,
                                    hInst,
                                    NULL);

    if (!ghWndTreeView)
        // ERROR: Unable to create TreeView
        return FALSE;

    // Init the image list and add items to the control
    if (!InitImageList (ghWndTreeView))
        // ERROR: Unable to init image list.
        return FALSE;


    if (!InitTreeViewItems (ghWndTreeView, pS->hWndMonitored))
        // ERROR: Unable to init treeview items
        return FALSE;

    
}
