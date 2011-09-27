// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   enumdlg.c
//
//  PURPOSE:   Displays the ServerEnum dialog box
//
//  FUNCTIONS:
//    EnumDlg - Processes messages for "EnumDlg" dialog box.
//    MsgEnumDlgInit - Initializes the dialog box
//    MsgEnumDlgInitEnum - Adds result of initial enumeration to listbox
//    MsgEnumDlgEnumExpand - Adds result of entry expansion to listbox
//    MsgEnumDlgMeasureItem - Computes height of list box item
//    MsgEnumDlgDrawItem - Draws items in owner draw listbox
//    MsgEnumDlgVKeyToItem - Process key presses in listbox
//    MsgEnumDlgCharToItem - Searches for items in listbox by character
//    MsgEnumDlgSetCursor - Sets the cursor to wait is enumerationg
//    MsgEnumDlgSysColor - Updates colors used in dialog box
//    MsgEnumDlgCommand - Processes WM_COMMAND messages
//    MsgEnumDlgDestroy - Unititializes dialog box
//    CmdEnumDlgOK - Process OK button click
//    CmdEnumDlgCancel - Process Cancel button click
//    CmdEnumDlgHelp - Calls user defined procedure
//    CmdEnumDlgBrowse - Process listbox notification messages
//
//  COMMENTS:
//    Display dialog box which allows the user to select a computer.
//    The list box is filled when the threads doing the enumeration
//    finish
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "dlldisp.h"
#include "enumdlg.h"
#include "enumutil.h"
#include "enumthrd.h"
#include "thrdutil.h"
#include "miscutil.h"
#include "mem.h"
#include "macros.h"

// MSVC++ 1.1 for NT does not define this in WINERROR.H
#ifndef ERROR_CANCELLED
#define ERROR_CANCELLED 1223L
#endif

extern DWORD ndxTls;

LRESULT MsgEnumDlgInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgEnumDlgInitEnum(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgEnumDlgEnumExpand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgEnumDlgMeasureItem(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgEnumDlgDrawItem(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgEnumDlgVKeyToItem(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgEnumDlgCharToItem(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam);
LRESULT MsgEnumDlgSetCursor(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgEnumDlgSysColor(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgEnumDlgCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgEnumDlgDestroy(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdEnumDlgOK(HWND, WORD, WORD, HWND);
LRESULT CmdEnumDlgCancel(HWND, WORD, WORD, HWND);
LRESULT CmdEnumDlgHelp(HWND, WORD, WORD, HWND);
LRESULT CmdEnumDlgBrowse(HWND, WORD, WORD, HWND);


// EnumDlg dialog message table definition.
MSD rgmsdEnumDlg[] =
{
    {WM_INITDIALOG,     MsgEnumDlgInit},
    {WM_INIT_ENUM,      MsgEnumDlgInitEnum},
    {WM_ENUM_EXPAND,    MsgEnumDlgEnumExpand},
    {WM_MEASUREITEM,    MsgEnumDlgMeasureItem},
    {WM_DRAWITEM,       MsgEnumDlgDrawItem},
    {WM_VKEYTOITEM,     MsgEnumDlgVKeyToItem},
    {WM_CHARTOITEM,     MsgEnumDlgCharToItem},
    {WM_SETCURSOR,      MsgEnumDlgSetCursor},
    {WM_SYSCOLORCHANGE, MsgEnumDlgSysColor},
    {WM_COMMAND,        MsgEnumDlgCommand},
    {WM_DESTROY,        MsgEnumDlgDestroy}
};

MSDI msdiEnumDlg =
{
    sizeof(rgmsdEnumDlg) / sizeof(MSD),
    rgmsdEnumDlg,
    edwpNone
};

// EnumDlg dialog command table definition.
CMD rgcmdEnumDlg[] =
{
    {IDOK,       CmdEnumDlgOK},
    {IDCANCEL,   CmdEnumDlgCancel},
    {IDB_HELP,   CmdEnumDlgHelp},
    {IDL_BROWSE, CmdEnumDlgBrowse}
};

CMDI cmdiEnumDlg =
{
    sizeof(rgcmdEnumDlg) / sizeof(CMD),
    rgcmdEnumDlg,
    edwpNone
};


//
//  FUNCTION: EnumDlg(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "EnumDlg" dialog box.
//
//  PARAMETERS:
//    hdlg - window handle of the dialog box
//    wMessage - type of message
//    wparam - message-specific information
//    lparam - message-specific information
//
//  RETURN VALUE:
//    TRUE - message handled
//    FALSE - message not handled
//
//  COMMENTS:
//    Dispatches messages for dialog box
//

LRESULT CALLBACK EnumDlg(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiEnumDlg, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgEnumDlgInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the enumdlg box.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_INITDIALOG.
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns TRUE - message handled.
//
//  COMMENTS:
//    Initializes syncronization object, and creates a thread for initial
//    enumeration.  If any initialization fails, the dialog box ends
//
LRESULT MsgEnumDlgInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HANDLE hThread;
    DWORD tID;
    DWORD dwRet;
    RECT rect;
    HWND hwndBrowse;
    LPTLSOBJECT lpTls = TlsGetValue(ndxTls);

    // create and initialize syncronization object
    if ( ERROR_SUCCESS == ( dwRet = MallocMem( &(lpTls->lpSync), sizeof( ENUMSYNC ) ) ) )
    {
        lpTls->lpSync->hwnd = hdlg;
        lpTls->lpSync->cThread = 1;
        lpTls->lpSync->cNetBuf = 0;
        lpTls->lpSync->lplpnetBuf = NULL;
        lpTls->lpSync->lpRoot = NULL;

        InitializeCriticalSection( &(lpTls->lpSync->cs) );

        // create root node and start enumeration thread
        if( ERROR_SUCCESS == ( dwRet = MallocMem( &(lpTls->lpSync->lpRoot), sizeof( ENUMNODE ) ) ) )
        {
            lpTls->lpSync->lpRoot->lpNet = NULL;
            lpTls->lpSync->lpRoot->cChild = 0;
            lpTls->lpSync->lpRoot->lpChild = NULL;

            lpTls->lpSync->cThread++;
            if ( ( hThread = CreateThread(NULL, 100,
                                         (LPTHREAD_START_ROUTINE)InitialEnumThread,
                                         (LPVOID) lpTls->lpSync,
                                         0, &tID) ) == NULL )
            {
                lpTls->lpSync->cThread--;
                dwRet = ERROR_FAILURE;
            }
            else
            {
                MarkOutstanding( &hThread ); // add thread to outstanding list

                // perform UI initialization
                LoadBitmapArray( lpTls->hbmpArray );
                lpTls->hbrushHL = CreateSolidBrush( GetSysColor(COLOR_HIGHLIGHT) );

                hwndBrowse = GetDlgItem( hdlg, IDL_BROWSE );

                GetClientRect( hwndBrowse, &rect );

                lpTls->lpfnListProc = (WNDPROC) SetWindowLong( hwndBrowse, GWL_WNDPROC, (LONG) ListSubProc );
                SendMessage( hwndBrowse, LB_ADDSTRING, 0L, (LPARAM) WORKING );
                SendMessage( hwndBrowse, WM_SETFONT, (WPARAM) lpTls->hfont, 0 );

                SendDlgItemMessage( hdlg, IDE_SERVER, EM_LIMITTEXT, MAX_COMPUTERNAME_LENGTH, 0 );
                SendDlgItemMessage( hdlg, IDE_SERVER, WM_SETFONT, (WPARAM) lpTls->hfont, 0 );

                EnableWindow( GetDlgItem( hdlg, IDT_SELECT), FALSE );

                if ( lpTls->lpfnHelpProc == NULL )
                    EnableWindow( GetDlgItem( hdlg, IDB_HELP), FALSE );

                // Center the dialog over the application window
                CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

            }
        }
    }

    if ( dwRet != ERROR_SUCCESS )
    {
        // deallocation occurs in the WM_DESTROY message handler
        EndDialog(hdlg, ERROR_NOT_ENOUGH_MEMORY);
    }

    return TRUE;
}


//
//  FUNCTION: MsgEnumDlgInitEnum(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Adds result of initial enumeration to listbox
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_INIT_ENUM.
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns TRUE - message handled.
//
//  COMMENTS:
//
LRESULT MsgEnumDlgInitEnum(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    UINT ndx;
    TCHAR szServer[MAX_COMPUTERNAME_LENGTH+3] = TEXT("\\\\");
    TCHAR szTemp[MAX_COMPUTERNAME_LENGTH+3];
    HWND hwndBrowse = GetDlgItem( hdlg, IDL_BROWSE );
    LPTLSOBJECT lpTls = TlsGetValue(ndxTls);

    SetCursor( LoadCursor( NULL, IDC_WAIT ) );
    SendMessage( hwndBrowse, WM_SETREDRAW, FALSE, 0 );
    SendMessage( hwndBrowse, LB_RESETCONTENT, 0L, 0L );

    // recursively add tree to listbox, looking for the listbox index
    // of the local machine along the way
    ndx = MAX_COMPUTERNAME_LENGTH+1;
    GetComputerName( szServer+2, &ndx );
    ndx = EnumListRecurse( hwndBrowse, lpTls->lpSync->lpRoot, szServer+2);

    // check to see if the user has typed in a server name
    // before reseting the edit control
    GetDlgItemText( hdlg, IDE_SERVER, szTemp, MAX_COMPUTERNAME_LENGTH+3);
    if ( *szTemp == TEXT('\0') )  // the user has not typed anything yet
    {
        SetDlgItemText( hdlg, IDE_SERVER, szServer );
    }

    SendMessage( hwndBrowse, LB_SETTOPINDEX, ndx, 0 );
    SendMessage( hwndBrowse, LB_SETCURSEL, ndx, 0 );

    EnableWindow( GetDlgItem( hdlg, IDT_SELECT), TRUE );
    SendMessage( hwndBrowse, WM_SETREDRAW, TRUE, 0 );
    SetCursor( LoadCursor( NULL, IDC_ARROW ) );


    return TRUE;
}


//
//  FUNCTION: MsgEnumDlgEnumExpand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Adds results of an single enumeration to listbox.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_ENUM_EXPAND
//    wparam - Index of node that was expanded.
//    lparam - Pointer to ENUMNODE that was expanded.
//
//  RETURN VALUE:
//    Always returns TRUE - message handled.
//
//  COMMENTS:
//
LRESULT MsgEnumDlgEnumExpand(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LPENUMNODE lpNode = (LPENUMNODE) lparam;
    UINT cChild;
    HWND hwndBrowse = GetDlgItem( hdlg, IDL_BROWSE );

    SetCursor( LoadCursor( NULL, IDC_WAIT ) );
    SendMessage( hwndBrowse, WM_SETREDRAW, FALSE, 0 );

    cChild = lpNode->cChild;
    while( cChild-- )
    {
        // all of these bitmaps default to not expanded
        switch( lpNode->lpChild[cChild].lpNet->dwDisplayType )
        {
            case RESOURCEDISPLAYTYPE_DOMAIN:
                lpNode->lpChild[cChild].ndxBmp = DOM_NDX;
                break;

            case RESOURCEDISPLAYTYPE_SERVER:
                lpNode->lpChild[cChild].ndxBmp = SERV_NDX;
                break;
        }

        SendMessage( hwndBrowse, LB_INSERTSTRING, wparam+1, (LPARAM) &(lpNode->lpChild[cChild]) );

    }

    EnableWindow( GetDlgItem( hdlg, IDT_SELECT), TRUE );
    SendMessage( hwndBrowse, WM_SETREDRAW, TRUE, 0 );
    SetCursor( LoadCursor( NULL, IDC_ARROW ) );
    return TRUE;
}


//
//  FUNCTION: MsgEnumDlgMeasureItem(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Computes the height of an item in the listbox.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_MEASUREITEM.
//    wparam - Message specific data (unused).
//    lparam - LPMEASUREITEMSTRUCT.
//
//  RETURN VALUE:
//    TRUE - message handled.
//
//
//  COMMENTS:
//
LRESULT MsgEnumDlgMeasureItem(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT) lparam;
    LPTLSOBJECT lpTls = TlsGetValue(ndxTls);

    lpmis->itemHeight = MAX( lpTls->tm.tmHeight, BITMAP_HEIGHT ) ;

    return TRUE;
}


//
//  FUNCTION: MsgEnumDlgDrawItem(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Draws items in the owner draw listbox.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_DRAWITEM.
//    wparam - id of control.
//    lparam - LPDRAWITEMSTRUCT.
//
//  RETURN VALUE:
//    TRUE - message handled.
//    FALSE - message not handled.
//
//  COMMENTS:
//
LRESULT MsgEnumDlgDrawItem(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT) lparam;
    LPENUMNODE lpNode = (LPENUMNODE) lpdis->itemData;
    HBITMAP hbmpOld = NULL;
    HDC hdcMem = NULL;
    HBRUSH hbrush;
    COLORREF crefOldText;
    COLORREF crefOldBk;
    UINT nTop;
    UINT nIndent = 0;
    TCHAR szWorking[] = TEXT("Working...");
    TCHAR szServer[MAX_COMPUTERNAME_LENGTH+3];
    UINT nHighlight = 0;
    LPTLSOBJECT lpTls;

    // If this isn't the correct dialog item, skip this message
    if ( (UINT) wparam != IDL_BROWSE )
        return FALSE;

    // If there are no list box items, skip this message.
    if (lpdis->itemID == -1)
        return FALSE;

    switch (lpdis->itemAction)
    {
        case ODA_SELECT:
        case ODA_DRAWENTIRE:

            // if we're still doing the initial enumeration
            if ( (LONG) lpNode == WORKING ) 
            {
                TextOut(lpdis->hDC, 2, 2, szWorking, _tcslen(szWorking) );
            }
            else
            {
                lpTls = TlsGetValue(ndxTls);

                // compute indent for list item
                switch( lpNode->lpNet->dwDisplayType )
                {
                    case RESOURCEDISPLAYTYPE_GENERIC:
                        nIndent = 0;
                        break;

                    case RESOURCEDISPLAYTYPE_DOMAIN:
                        nIndent = BITMAP_WIDTH;
                        break;

                    case RESOURCEDISPLAYTYPE_SERVER:
                        nIndent = 2 * BITMAP_WIDTH;
                        break;
                }

                // Create MemDC for bitmap
                hdcMem = CreateCompatibleDC(lpdis->hDC);

                // Is the item selected?
                if (lpdis->itemState & ODS_SELECTED)
                {
                    hbmpOld = SelectObject(hdcMem, lpTls->hbmpArray[lpNode->ndxBmp+HIGHLIGHT_NDX]);
                    hbrush = lpTls->hbrushHL;
                    crefOldText = SetTextColor( lpdis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT) );
                    crefOldBk = SetBkColor( lpdis->hDC, GetSysColor(COLOR_HIGHLIGHT) );
                }
                else
                {
                    hbmpOld = SelectObject(hdcMem, lpTls->hbmpArray[lpNode->ndxBmp]);
                    hbrush = GetStockObject( WHITE_BRUSH );
                    crefOldText = SetTextColor( lpdis->hDC, GetSysColor(COLOR_WINDOWTEXT) );
                    crefOldBk = SetBkColor( lpdis->hDC, GetSysColor(COLOR_WINDOW) );
                }

                // Draw background
                FillRect( lpdis->hDC,
                          &(lpdis->rcItem),
                          hbrush );

                // Display bitmap
                nTop = (lpdis->rcItem.bottom + lpdis->rcItem.top - BITMAP_HEIGHT) / 2;
                BitBlt(lpdis->hDC,
                       lpdis->rcItem.left+nIndent,
                       nTop,
                       lpdis->rcItem.right - lpdis->rcItem.left,
                       lpdis->rcItem.bottom - lpdis->rcItem.top,
                       hdcMem, 0, 0, SRCCOPY);

                // Display the text associated with the item. */
                nTop = (lpdis->rcItem.bottom + lpdis->rcItem.top - lpTls->tm.tmHeight) / 2;
                TextOut(lpdis->hDC,
                    BITMAP_WIDTH + nIndent + 4,
                    nTop,
                    lpNode->lpNet->lpRemoteName,
                    _tcslen(lpNode->lpNet->lpRemoteName) );

                // Display comment if there is one
                if ( lpNode->lpNet->lpComment )
                {
                    TextOut(lpdis->hDC,
                        lpdis->rcItem.right/2,
                        nTop,
                        lpNode->lpNet->lpComment,
                        _tcslen(lpNode->lpNet->lpComment) );
                }

                SetBkColor( lpdis->hDC, crefOldBk );
                SetTextColor( lpdis->hDC, crefOldText );
                SelectObject(hdcMem, hbmpOld);

                DeleteDC(hdcMem);
            }
            break;

        case ODA_FOCUS: // taks appropriate measures if item has focus
            if ( (LONG) lpNode != WORKING )
            {
                DrawFocusRect( lpdis->hDC, &(lpdis->rcItem) );

                if (lpdis->itemState & ODS_FOCUS)
                {
                    if ( lpNode->lpNet->dwDisplayType == RESOURCEDISPLAYTYPE_SERVER )
                    {
                        _tcscpy( szServer, TEXT("\\\\") );
                        _tcscat( szServer, lpNode->lpNet->lpRemoteName );
                    }
                    else
                    {
                        *szServer = TEXT('\0');
                    }
                    SetDlgItemText( hdlg, IDE_SERVER, szServer );
                }
            }
            break;
    }

    return TRUE;
}



//
//  FUNCTION: MsgEnumDlgVKeyToItem(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handles ccrolling in listbox
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_VKEYTOITEM.
//    wparam - LOWORD(wparam) = v-key code.
//    lparam - Handle of listbox.
//
//  RETURN VALUE:
//    -2 - message handled.
//    -1  - message not handled
//
//  COMMENTS:
//
LRESULT MsgEnumDlgVKeyToItem(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int nCur;
    int nCount;
    int nItemsPerPage;
    LPTLSOBJECT lpTls;
    HWND hwndBrowse = GetDlgItem( hdlg, IDL_BROWSE );
    RECT rect;

    if ( (HWND)lparam == hwndBrowse )
    {

        lpTls = TlsGetValue(ndxTls);
        nCur = SendMessage( hwndBrowse, LB_GETCURSEL, 0, 0 );
        nCount = SendMessage( hwndBrowse, LB_GETCOUNT, 0, 0 );
        GetClientRect( hwndBrowse, &rect );
        nItemsPerPage = rect.bottom / ( MAX( lpTls->tm.tmHeight, BITMAP_HEIGHT ) );

        if ( nCount != 0 )
        {
            switch( LOWORD(wparam) )
            {
                case VK_HOME:
                    SendMessage( hwndBrowse, LB_SETCURSEL, 0, 0 );
                    break;

                case VK_END:
                    SendMessage( hwndBrowse, LB_SETCURSEL, nCount-1, 0 );
                    break;

                case VK_UP:
                    SendMessage( hwndBrowse, LB_SETCURSEL, MAX( nCur-1, 0 ), 0 );
                    break;

                case VK_DOWN:
                    SendMessage( hwndBrowse, LB_SETCURSEL, MIN( nCur+1, nCount-1 ), 0 );
                    break;

                case VK_PRIOR:
                    SendMessage( hwndBrowse, WM_SETREDRAW, FALSE, 0 );
                    SendMessage( hwndBrowse, LB_SETCURSEL, MAX( nCur-nItemsPerPage, 0 ), 0 );
                    SendMessage( hwndBrowse, WM_SETREDRAW, TRUE, 0 );
                    break;

                case VK_NEXT:
                    SendMessage( hwndBrowse, WM_SETREDRAW, FALSE, 0 );
                    SendMessage( hwndBrowse, LB_SETCURSEL, MIN( nCur+nItemsPerPage, nCount-1 ), 0 );
                    SendMessage( hwndBrowse, WM_SETREDRAW, TRUE, 0 );
                    break;

                default:
                    return -1;
            }
            return -2;
        }
    }
    return -1;
}


//
//  FUNCTION: MsgEnumDlgCharToItem(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Search for items in listbox based on character
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_CHARTOITEM.
//    wparam - LOWORD(wparam) = character
//    lparam - Handle of listbox.
//
//  RETURN VALUE:
//    -2 - message handled.
//    -1  - message not handled
//
//  COMMENTS:
//    Because we have to use unicode, we need to convert the character
//    keypress to unicode before search.  If no match is found, we
//    leave the current selection highlighted
//
LRESULT MsgEnumDlgCharToItem(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int nCur;
    int nCount;
    int ndx;
    LPENUMNODE lpNode  = NULL;
    TCHAR w_key[2];
    CHAR key = (CHAR) LOWORD(wparam);
    HWND hwndBrowse = GetDlgItem( hdlg, IDL_BROWSE );

    if ( (HWND)lparam == hwndBrowse )  // handle message for this listbox
    {
        if ( (UINT)key == 0xD )  // enter key is the same as a double click
        {
            SendMessage( hdlg, WM_COMMAND, MAKELONG(IDL_BROWSE, LBN_DBLCLK), (LPARAM) hwndBrowse );
            return -2;
        }

        nCur = SendMessage( hwndBrowse, LB_GETCURSEL, 0, 0 );
        nCount = SendMessage( hwndBrowse, LB_GETCOUNT, 0, 0 );

        if ( nCount > 0 )
        {
            if ( MultiByteToWideChar( CP_ACP, 0, &key, 1, w_key, 2 ) )
            {
                for( ndx = nCur+1; ndx != nCur; ndx++ )
                {
                     if ( ndx >= nCount )
                     {
                         ndx = -1;
                         continue;
                     }

                     if ( LB_ERR != SendMessage( hwndBrowse, LB_GETTEXT, ndx, (LPARAM) &lpNode) )
                     {
                         if ( _totupper(*w_key) == _totupper(*lpNode->lpNet->lpRemoteName) )
                         {
                             SendMessage( hwndBrowse, LB_SETCURSEL, ndx, 0 );
                             return -2;
                         }
                     }
                }
            }
        }
    }
    return -1;
}



//
//  FUNCTION: MsgEnumDlgSetCursor(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Sets the cursor to Wait when over listbox and
//           and enumeration is occuring
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_SETCURSOR.
//    wparam - handle of window of which the cursor is over.
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    TRUE - message handled.
//    FALSE - message not handled.
//
//  COMMENTS:
//
LRESULT MsgEnumDlgSetCursor(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HWND hwndBrowse = GetDlgItem( hdlg, IDL_BROWSE );
    LPTLSOBJECT lpTls;

    if ( (HWND)wparam == hwndBrowse )
    {
        lpTls = TlsGetValue(ndxTls);

        if ( lpTls->lpSync->cThread > 1 )
            SetCursor( LoadCursor( NULL, IDC_WAIT ) );
        else
            SetCursor( LoadCursor( NULL, IDC_ARROW ) );
        return TRUE;
    }

    return FALSE;
}



//
//  FUNCTION: MsgEnumDlgSysColor(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Updates the colors used by the dialog box
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_SYSCOLORCHANGE.
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns TRUE - message handled.
//
//  COMMENTS:
//
LRESULT MsgEnumDlgSysColor(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    UINT ndx = HIGHLIGHT_NDX*2;
    LPTLSOBJECT lpTls = TlsGetValue(ndxTls);

    while( ndx-- )
        DeleteObject( lpTls->hbmpArray[ndx] );

    LoadBitmapArray( lpTls->hbmpArray );

    DeleteObject( lpTls->hbrushHL );
    lpTls->hbrushHL = CreateSolidBrush( GetSysColor(COLOR_HIGHLIGHT) );

    return TRUE;
}


//
//  FUNCTION: MsgEnumDlgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the enumdlg box.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_COMMAND.
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    TRUE for message handled
//    FALSE for message not handled
//
//  COMMENTS:
//    Uses this DispCommand function defined in wndproc.c combined
//    with the cmdiEnumDlg structure defined in this file to handle
//    the command messages for the enumdlg dialog box.
//
LRESULT MsgEnumDlgCommand(HWND   hwnd,
                        UINT   uMessage,
                        WPARAM wparam,
                        LPARAM lparam)
{
    return DispCommand(&cmdiEnumDlg, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgEnumDlgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Frees memory and resource used by dialogbox
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. WM_DESTROY.
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns TRUE - message handled.
//
//  COMMENTS:
//
LRESULT MsgEnumDlgDestroy(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    UINT ndx = HIGHLIGHT_NDX*2;
    LPTLSOBJECT lpTls = TlsGetValue(ndxTls);
    HWND hwndBrowse = GetDlgItem( hdlg, IDL_BROWSE );

    if ( lpTls->hbmpArray[0] )
    {
        while( ndx-- )
            DeleteObject( lpTls->hbmpArray[ndx] );
    }

    if ( lpTls->hbrushHL )
        DeleteObject( lpTls->hbrushHL );

    if ( lpTls->lpfnListProc )
        SetWindowLong( hwndBrowse, GWL_WNDPROC, (LONG) lpTls->lpfnListProc );

    if ( lpTls->lpSync )
        FreeEnumSync( &(lpTls->lpSync) );

    return TRUE;
}


//
//  FUNCTION: CmdEnumDlgOK(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Processes the OK Button press.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled IDOK.
//    wNotify  - The notification code to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//    If there is not not text in the edit control
//    this behaves the same as a cancel.  If there
//    is text, we make sure that it is prepended
//    with a '\\'.  Next, we make sure there is
//    enough room to store it in the user's buffer.
//
LRESULT CmdEnumDlgOK(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    TCHAR szServer[MAX_COMPUTERNAME_LENGTH+5] = TEXT("\\\\");
    LPTSTR psz = szServer+2;
    DWORD cchServer = 0;
    LPTLSOBJECT lpTls = TlsGetValue(ndxTls);

    SendDlgItemMessage( hdlg, IDE_SERVER, WM_GETTEXT, (WPARAM)(MAX_COMPUTERNAME_LENGTH+3), (LPARAM)psz );

    if ( *psz == TEXT('\0') ) // no server name, so same as cancelled
    {
        EndDialog(hdlg, ERROR_CANCELLED);
    }
    else
    {
        if ( *psz != TEXT('\\') ) // if the name does not begin with a '\', we make 
        {                         // it begin with '\\' by ajusting the pointer
            psz = szServer;
            psz[MAX_COMPUTERNAME_LENGTH+2] = TEXT('\0');
        }
        cchServer = _tcslen( psz )+1;

        if ( ( cchServer > *(lpTls->lpcchServer) ) ||
             ( lpTls->lpszServer == NULL ) )
        {
            *(lpTls->lpcchServer) = cchServer;
            EndDialog(hdlg, ERROR_MORE_DATA);
        }
        else
        {
            _tcscpy( lpTls->lpszServer, psz );
            *(lpTls->lpcchServer) = cchServer;
            EndDialog(hdlg, ERROR_SUCCESS);
        }
    }
    return TRUE;
}


//
//  FUNCTION: CmdEnumDlgCancel(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Process the cancel button press
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled IDCANCEL.
//    wNotify  - The notification code to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//
LRESULT CmdEnumDlgCancel(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    EndDialog(hdlg, ERROR_CANCELLED);          // Exit the dialog
    return TRUE;
}


//
//  FUNCTION: CmdEnumDlgHelp(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Process the help button press
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled IDB_HELP.
//    wNotify  - The notification code to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls the users defined help procedure
//
LRESULT CmdEnumDlgHelp(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    LPTLSOBJECT lpTls = TlsGetValue(ndxTls);

    (*(lpTls->lpfnHelpProc))();
    return TRUE;
}


//
//  FUNCTION: CmdEnumDlgBrowse(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Process notification messages for listbox
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled IDL_BROWSE.
//    wNotify  - The notification code to be handled - LBN_DBLCLK.
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    TRUE if message is processed.
//
//  COMMENTS:
//    Double click on an item causes either expansion or contraction
//
LRESULT CmdEnumDlgBrowse(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    LPENUMNODE lpNode;
    UINT ndx;
    HWND hwndBrowse;
    LPTLSOBJECT lpTls = TlsGetValue(ndxTls);

    if ( lpTls->lpSync->cThread == 1 )
    {

        hwndBrowse = GetDlgItem( hdlg, IDL_BROWSE );
        ndx = SendMessage( hwndBrowse, LB_GETCURSEL, 0, 0 );
        SendMessage( hwndBrowse, LB_GETTEXT, (WPARAM) ndx, (LPARAM) &lpNode );

        switch( wNotify )
        {
            case LBN_DBLCLK:  // expanding and collapsing nodes
                switch( lpNode->ndxBmp )
                {
                    case PROV_NDX:
                        lpNode->ndxBmp = PROVEX_NDX;
                        EnableWindow( GetDlgItem( hdlg, IDT_SELECT), FALSE );
                        EnumListExpand( hwndBrowse, lpTls->lpSync, lpNode, ndx );
                        break;

                    case DOM_NDX:
                        lpNode->ndxBmp = DOMEX_NDX;
                        EnableWindow( GetDlgItem( hdlg, IDT_SELECT), FALSE );
                        EnumListExpand( hwndBrowse, lpTls->lpSync, lpNode, ndx );
                        break;

                    case PROVEX_NDX:
                        lpNode->ndxBmp = PROV_NDX;
                        EnumListCollapse( hwndBrowse, lpNode, ndx );
                        break;

                    case DOMEX_NDX:
                        lpNode->ndxBmp = DOM_NDX;
                        EnumListCollapse( hwndBrowse, lpNode, ndx );
                        break;

                    case SERV_NDX:
                        SendMessage( hdlg, WM_COMMAND, (WPARAM) IDOK, 0 );
                        break;
                }
                return TRUE;
        }
    }
    return FALSE;
}



LRESULT APIENTRY ListSubProc(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LPTLSOBJECT lpTls = TlsGetValue(ndxTls);

    switch (uMessage)
    {
        case WM_SETCURSOR:
            SendMessage( GetParent(hwnd), WM_SETCURSOR, (WPARAM) hwnd, MAKELONG( HTCLIENT, 0 ) );
            break;

        case WM_GETDLGCODE:
            return DLGC_WANTALLKEYS | CallWindowProc(lpTls->lpfnListProc, hwnd, uMessage, wparam, lparam);

        default:
            return CallWindowProc(lpTls->lpfnListProc, hwnd, uMessage, wparam, lparam);
    }
    return (0);
}
