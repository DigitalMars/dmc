/*----------------------------------------------------------------------------*\
|   ICWalk.c - Walks all ICM compressors                                       |
\*----------------------------------------------------------------------------*/
/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/

#define STRICT
#include <windows.h>
#include <windowsx.h>

#define NOAVIFILE
#include <vfw.h>

#include "icwalk.h"

//----------------------------------------------------------------------------

static HINSTANCE ghInst;
static BOOL      gfDefDlgEx;         //the recursion flag for message crackers
static DWORD     fccWalk;
static int       aiTabs[] = {TABSTOP1, TABSTOP2, TABSTOP3};
static ICINFO    icinfo;
static int       gnStuff;
static char      gszBuffer[BUFSIZE];

//----------------------------------------------------------------------------

static  int      CountStuff(void);
static  void     WalkStuff(HWND);
static  LRESULT  Dlg_DefProc(HWND, UINT, WPARAM, LPARAM);
static  BOOL     ICWalk_OnInitDialog(HWND, HWND, LPARAM);
static  void     ICWalk_OnCommand(HWND, int, HWND, UINT);
static  void     ICWalk_OnActivate(HWND, UINT, HWND, BOOL);
static  void     ICWalk_OnSize(HWND, UINT, int, int);
static  LRESULT  ICWalkNewDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK ICWalkDlgProc(HWND, UINT, WPARAM, LPARAM);
int     PASCAL   WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
BOOL CALLBACK    AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//----------------------------------------------------------------------------
static int CountStuff(
)
{
        int i;

        for (i=0; ICInfo(fccWalk, i, &icinfo); i++)
                continue;

        return i;
}

//----------------------------------------------------------------------------
static void WalkStuff(
HWND lb)
{
        char szLBBuffer[4*BUFSIZE];
        char *pch;
        int i;
        HCURSOR hcur;

        hcur = SetCursor(LoadCursor(NULL, IDC_WAIT));

        SetWindowFont(lb, GetStockObject(ANSI_FIXED_FONT), FALSE);
        ListBox_ResetContent(lb);
        ListBox_SetTabStops(lb, (sizeof(aiTabs) / sizeof(int)), aiTabs);
        //
        // call ICInfo to enumerate compressors.
        //
        // we use 0 as the fccType to walk all compressors
        // of any type.
        //
        for (i = 0; ICInfo(fccWalk, i, &icinfo); i++) {
                HIC hic;
                pch = szLBBuffer;

                if (icinfo.fccHandler > 256) {
                        LoadString(ghInst, IDS_FORMAT_1, gszBuffer, BUFSIZE);
                        pch += wsprintf(pch, gszBuffer, (LPSTR)&icinfo.fccType, (LPSTR)&icinfo.fccHandler);
                } else {
                        LoadString(ghInst, IDS_FORMAT_2, gszBuffer, BUFSIZE);
                        pch += wsprintf(pch, gszBuffer, (LPSTR)&icinfo.fccType, icinfo.fccHandler);
                }

                hic = ICOpen(icinfo.fccType, icinfo.fccHandler, ICMODE_QUERY);
                //
                // find out more info (like the compressor name).
                //
                if (hic) {
                        ICGetInfo(hic, &icinfo, sizeof(icinfo));
                        ICClose(hic);
                } else {
                        LoadString(ghInst, IDS_CANTLOAD, gszBuffer, BUFSIZE);
                        MultiByteToWideChar(CP_ACP, 0, gszBuffer, -1, icinfo.szDescription, sizeof(icinfo.szDescription));
                }

                LoadString(ghInst, IDS_FORMAT_3, gszBuffer, BUFSIZE);

                pch += wsprintf(pch,
                        gszBuffer,
                        (LPCSTR)icinfo.szName,
                        (LPCSTR)icinfo.szDescription,
                        (LPCSTR)icinfo.szDriver);

                ListBox_AddString(lb, szLBBuffer);
        }
        gnStuff = i;
}


//***************************************************************************
//
//      FUNCTION: Dlg_DefProc(HWND, UINT, WPARAM, LPARAM)
//
//      PURPOSE:  Handles default messages for all dialog boxes
//
//
static LRESULT Dlg_DefProc(
HWND hDlg,
UINT message,
WPARAM wParam,
LPARAM lParam )
{
        return DefDlgProcEx( hDlg, message, wParam, lParam, &gfDefDlgEx );
}
//
//***************************************************************************
//
//      FUNCTION: ICWalk_OnInitDialog(HWND, HWND, LPARAM)
//
//      PURPOSE:  Handles initialization for dialog box
//
//
static BOOL ICWalk_OnInitDialog(
HWND hDlg,
HWND hwndFocus,
LPARAM lParam)
{
        HMENU hSysMenu;
        TCHAR szAboutItem[64];
        //
        // if ICInfo does not support walking all types, then just
        // walk video types
        //
        if (!ICInfo(0, 0, &icinfo))
                fccWalk = ICTYPE_VIDEO;

        //
        // Add an "about..." selection to system menu
        //
        hSysMenu = GetSystemMenu(hDlg,FALSE);
        if (hSysMenu)
        {
                LoadString(ghInst,IDS_ABOUT,szAboutItem,sizeof(szAboutItem));
                AppendMenu(hSysMenu,MF_STRING,IDM_ABOUT,szAboutItem);
        }

        return (FALSE);
}
//
//***************************************************************************
//
//      FUNCTION: ICWalk_OnCommand(HWND, HWND, UINT)
//
//      PURPOSE:  Handles the child controls for dialog box
//
//      DIALOGBOX ID'S
//
//              IDOK            - ok button
//              IDCANCEL        - cancel button
//
static void ICWalk_OnCommand(
HWND hDlg,
int control_source,
HWND control_handle,
UINT control_action )
{
        switch (control_source)
        {
                case IDOK:
                case IDCANCEL:
                {
                        EndDialog(hDlg, 0);
                }
                break;
        }
        return;
}
//
//***************************************************************************
//
//      FUNCTION: ICWalk_OnActivate(HWND, UINT, HWND, BOOL)
//
//      PURPOSE:  Handles the activation of the dialog box
//
static void ICWalk_OnActivate(
HWND hwnd,
UINT state,
HWND hwndActDeact,
BOOL fMinimized )
{
        if ( state && gnStuff != CountStuff() )
                WalkStuff( GetDlgItem(hwnd, ID_LISTBOX) );

        return;
}
//
//***************************************************************************
//
//      FUNCTION: ICWalk_OnSize(HWND, UINT, int, int)
//
//      PURPOSE:  Handles the sizing of the dialog box
//
static void ICWalk_OnSize(
HWND hwnd,
UINT state,
int cx,
int cy)
{
        RECT rc;

        GetClientRect(hwnd, &rc);
        MoveWindow( GetDlgItem(hwnd, ID_LISTBOX),
                INDENT,
                INDENT,
                rc.right  - 2 * INDENT,
                rc.bottom - 2 * INDENT,
                TRUE);

        return;
}

//
//***************************************************************************
//
//      FUNCTION: ICWalkNewDlgProc(HWND, UINT, WPARAM, LPARAM)
//
//      PURPOSE:  Processes messages for dialog box using message crackers
//
//      MESSAGES:
//
//              WM_INITDIALOG   - initialize dialog box
//              WM_COMMAND      - process user input
//              WM_ACTIVATE     - updates dialog box
//              WM_SIZE         - sizes dialog box
//
//***************************************************************************
//
static LRESULT ICWalkNewDlgProc(
HWND hDlg,
UINT message,
WPARAM wParam,
LPARAM lParam)
{
        switch (message)
        {
        HANDLE_MSG( hDlg, WM_INITDIALOG,        ICWalk_OnInitDialog     );
        HANDLE_MSG( hDlg, WM_COMMAND,           ICWalk_OnCommand        );
        HANDLE_MSG( hDlg, WM_ACTIVATE,          ICWalk_OnActivate       );
        HANDLE_MSG( hDlg, WM_SIZE,              ICWalk_OnSize           );
        case WM_SYSCOMMAND:
                if (wParam == IDM_ABOUT) {
                    DialogBox(ghInst, MAKEINTRESOURCE(IDD_ABOUT), hDlg, AboutDlgProc);
                    return 0L;
                }
        default:
        return Dlg_DefProc( hDlg, message, wParam, lParam );
    }
}
//
//
//***************************************************************************
//
//      FUNCTION: ICWalkDlgProc(HWND, UINT, WPARAM, LPARAM)
//
//      PURPOSE:  Processes messages for info dialog box
//
//
BOOL CALLBACK ICWalkDlgProc(
HWND hDlg,
UINT message,
WPARAM wParam,
LPARAM lParam)
{
        CheckDefDlgRecursion( &gfDefDlgEx );
        return SetDlgMsgResult( hDlg, message, ICWalkNewDlgProc( hDlg, message, wParam, lParam ) );
}
//
//
//***************************************************************************
//
//      FUNCTION: WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//
//      PURPOSE:  Calls the DB, that's all
//
//
int PASCAL WinMain(
HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPSTR szCmdLine,
int sw)
{
        ghInst = hInstance;
        LoadString( hInstance, IDS_DIALOGNAME, gszBuffer, BUFSIZE );

        DialogBox( hInstance, gszBuffer, NULL, ICWalkDlgProc );

        return 0;
}


/* AboutDlgProc()
 *
 * Dialog Procedure for the "about" dialog box.
 *
 */

BOOL CALLBACK AboutDlgProc(
        HWND    hwnd,
        UINT    msg,
        WPARAM  wParam,
        LPARAM  lParam)
{
        switch (msg) {
        case WM_COMMAND:
                EndDialog(hwnd, TRUE);
                return TRUE;
        case WM_INITDIALOG:
                return TRUE;
        }
        return FALSE;
}
