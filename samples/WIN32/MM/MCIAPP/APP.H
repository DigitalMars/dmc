//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
//  A PARTICULAR PURPOSE.
//
//  Copyright (C) 1993 - 1995 Microsoft Corporation. All Rights Reserved.
//
//--------------------------------------------------------------------------;
//
//  app.h
//
//  Description:
//      This is a sample application that demonstrates how to use the
//      Media Control Interface (MCI) in Windows. This application is
//      also useful as an MCI device tester.
//
//  History:
//      11/ 8/92    created.
//
//==========================================================================;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  Application Version Information:
//
//
//
//
//  NOTE! all string resources that will be used in app.rcv for the
//  version resource information *MUST* have an explicit \0 terminator!
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

#define APP_VERSION_MAJOR           4
#define APP_VERSION_MINOR           0
#define APP_VERSION_BUILD           0
#ifdef UNICODE
#define APP_VERSION_STRING_RC       "Version 4.00 (Unicode Enabled)\0"
#else
#define APP_VERSION_STRING_RC       "Version 4.00\0"
#endif

#define APP_VERSION_NAME_RC         "mciapp.exe\0"
#define APP_VERSION_BYLINE_RC       "\0"
#define APP_VERSION_COMPANYNAME_RC  "Microsoft Corporation\0"
#define APP_VERSION_COPYRIGHT_RC    "Copyright \251 1985-1995 Microsoft Corp.\0"

#ifdef WIN32
#if (defined(_X86_)) || (defined(i386))
#define APP_VERSION_PRODUCTNAME_RC  "Microsoft Win32 (i386)\0"
#endif
#if (defined(_MIPS_)) || (defined(MIPS))
#define APP_VERSION_PRODUCTNAME_RC  "Microsoft Windows NT (MIPS)\0"
#endif
#if (defined(_ALPHA_)) || (defined(ALPHA))
#define APP_VERSION_PRODUCTNAME_RC  "Microsoft Windows NT (Alpha)\0"
#endif
#ifndef APP_VERSION_PRODUCTNAME_RC
#define APP_VERSION_PRODUCTNAME_RC  "Microsoft Windows NT\0"
#endif
#else
#define APP_VERSION_PRODUCTNAME_RC  "Microsoft Windows\0"
#endif

#ifdef DEBUG
#define APP_VERSION_DESCRIPTION_RC  "MCI Script Application (debug)\0"
#else
#define APP_VERSION_DESCRIPTION_RC  "MCI Script Application\0"
#endif


//
//  Unicode versions (if UNICODE is defined)... the resource compiler
//  cannot deal with the TEXT() macro.
//
#define APP_VERSION_STRING          TEXT(APP_VERSION_STRING_RC)
#define APP_VERSION_NAME            TEXT(APP_VERSION_NAME_RC)
#define APP_VERSION_BYLINE          TEXT(APP_VERSION_BYLINE_RC)
#define APP_VERSION_COMPANYNAME     TEXT(APP_VERSION_COMPANYNAME_RC)
#define APP_VERSION_COPYRIGHT       TEXT(APP_VERSION_COPYRIGHT_RC)
#define APP_VERSION_PRODUCTNAME     TEXT(APP_VERSION_PRODUCTNAME_RC)
#define APP_VERSION_DESCRIPTION     TEXT(APP_VERSION_DESCRIPTION_RC)




//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  misc defines for misc sizes and things...
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

//
//  bilingual. this allows the same identifier to be used in resource files
//  and code without having to decorate the id in your code.
//
#ifdef RC_INVOKED
    #define RCID(id)    id
#else
    #define RCID(id)    MAKEINTRESOURCE(id)
#endif


//
//  misc. defines
//
#define APP_MAX_APP_NAME_CHARS      30
#define APP_MAX_APP_NAME_BYTES      (APP_MAX_APP_NAME_CHARS * sizeof(TCHAR))
#define APP_MAX_STRING_RC_CHARS     512
#define APP_MAX_STRING_RC_BYTES     (APP_MAX_STRING_RC_CHARS * sizeof(TCHAR))
#define APP_MAX_STRING_ERROR_CHARS  512
#define APP_MAX_STRING_ERROR_BYTES  (APP_MAX_STRING_ERROR_CHARS * sizeof(TCHAR))
#define APP_MAX_FILE_PATH_CHARS     144
#define APP_MAX_FILE_PATH_BYTES     (APP_MAX_FILE_PATH_CHARS * sizeof(TCHAR))
#define APP_MAX_FILE_TITLE_CHARS    16
#define APP_MAX_FILE_TITLE_BYTES    (APP_MAX_FILE_TITLE_CHARS * sizeof(TCHAR))

#define APP_MAX_STRING_INT_CHARS    18
#define APP_MAX_STRING_INT_BYTES    (APP_MAX_STRING_INT_CHARS * sizeof(TCHAR))

#define APP_MAX_EXT_DEFAULT_CHARS   4
#define APP_MAX_EXT_DEFAULT_BYTES   (APP_MAX_EXT_DEFAULT_CHARS * sizeof(TCHAR))
#define APP_MAX_EXT_FILTER_CHARS    256
#define APP_MAX_EXT_FILTER_BYTES    (APP_MAX_EXT_FILTER_CHARS * sizeof(TCHAR))

#define APP_WINDOW_XOFFSET          CW_USEDEFAULT
#define APP_WINDOW_YOFFSET          CW_USEDEFAULT
#define APP_WINDOW_WIDTH            500 //CW_USEDEFAULT
#define APP_WINDOW_HEIGHT           300 //CW_USEDEFAULT


//
//  resource defines...
//
#define ICON_APP                    RCID(10)
#define ACCEL_APP                   RCID(15)


//
//  the application menu...
//
//  NOTE! for our Edit menu, we use the following defines from windows.h--
//  so don't reuse these defines for menu items!
//
//      #define WM_CUT      0x0300
//      #define WM_COPY     0x0301
//      #define WM_PASTE    0x0302
//      #define WM_CLEAR    0x0303
//      #define WM_UNDO     0x0304
//
#define MENU_APP                    RCID(20)
#define APP_MENU_ITEM_FILE          0
#define IDM_FILE_NEW                1100
#define IDM_FILE_OPEN               1101
#define IDM_FILE_SAVE               1102
#define IDM_FILE_SAVEAS             1103
#define IDM_FILE_ABOUT              1109
#define IDM_FILE_EXIT               1110

#define APP_MENU_ITEM_EDIT          1
#define IDM_EDIT_SELECTALL          1200

#define APP_MENU_ITEM_DEVICE        2
#define IDM_DEVICE_NONE             1300
#define IDM_DEVICE_OPENLIST         1398
#define IDM_DEVICE_CLOSEALL         1399

#define APP_MENU_ITEM_OPTIONS       3
#define IDM_OPTIONS_EDITONLY        1400
#define IDM_OPTIONS_DEBUGLOG        1401
#define IDM_OPTIONS_YIELDEXEC       1402
#define IDM_OPTIONS_FONT            1410


#define IDD_ACCL_ABORT              666



//
//  the main window control id's...
//
#define IDD_APP_BTN_STEP            100
#define IDD_APP_BTN_STOP            IDD_APP_BTN_STEP
#define IDD_APP_BTN_GO              101
#define IDD_APP_BTN_RUN             102
#define IDD_APP_EDIT_RUNCOUNT       103
#define IDD_APP_TEXT_OUTPUT         110
#define IDD_APP_EDIT_SCRIPT         150
#define IDD_APP_TEXT_STATUS         175
#define IDD_APP_BOX_SEPARATOR1      176
#define IDD_APP_TEXT_NOTIFY         177
#define IDD_APP_BOX_SEPARATOR2      178
#define IDD_APP_TEXT_OPTIONS        179


//
//  misc dlg boxes...
//
#define DLG_ABOUT                   RCID(50)
#define IDD_ABOUT_VERSION_OS        100
#define IDD_ABOUT_VERSION_PLATFORM  101

#define DLG_MCIERR                  RCID(51)


//
//  current open mci devices list window...
//
#define DLG_MCIDEVS                 RCID(52)
#define IDD_MCIDEVS_BTN_INFO        100
#define IDD_MCIDEVS_LIST_OPEN       101



//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  string resources
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

#define IDS_APP_NAME                100
#define IDS_FILE_UNTITLED           101

#define IDS_OFN_EXT_DEF             125
#define IDS_OFN_EXT_FILTER          126

#define IDS_DROP_CONTINUE           150

#define IDS_EXEC_ABORT              175

#define IDS_MCI_SCRIPT_CREATED      200
#define IDS_MCI_SCRIPT_OPENED       201
#define IDS_MCI_SCRIPT_SAVED        202
#define IDS_MCI_SCRIPT_CHANGED      203

#define IDS_MCI_NOTIFY_SUCCESSFUL   225
#define IDS_MCI_NOTIFY_SUPERSEDED   226
#define IDS_MCI_NOTIFY_ABORTED      227
#define IDS_MCI_NOTIFY_FAILURE      228
#define IDS_MCI_NOTIFY_UNKNOWN      229

#define IDS_WARN_OPEN_DEVICES       400

#define IDS_ERROR_OPEN_FAILED       500





//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  APPINIT.C -- Public helper functions
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

LRESULT FNGLOBAL AppCreate
(
    HWND            hwnd,
    LPCREATESTRUCT  pcs
);

LRESULT FNGLOBAL AppQueryEndSession
(
    HWND            hwnd
);

LRESULT FNGLOBAL AppEndSession
(
    HWND            hwnd,
    BOOL            fEndSession
);

LRESULT FNGLOBAL AppClose
(
    HWND            hwnd
);

HWND FNGLOBAL AppInit
(
    HINSTANCE       hinst,
    HINSTANCE       hinstPrev,
    LPTSTR          pszCmdLine,
    int             nCmdShow
);

int FNGLOBAL AppExit
(
    HINSTANCE       hinst,
    int             nResult
);


LRESULT FNGLOBAL AppGetWindowsVersion
(
    PTSTR           pszEnvironment,
    PTSTR           pszPlatform
);

LRESULT FNGLOBAL AppWinIniChange
(
    HWND            hwnd,
    LPCTSTR         pszSection
);

HFONT FNGLOBAL AppChooseFont
(
    HWND            hwnd,
    HFONT           hFont,
    PLOGFONT        plf
);


#define APP_CFF_NORMAL          0x0000
#define APP_CFF_ITALIC          0x0001
#define APP_CFF_UNDERLINE       0x0002
#define APP_CFF_STRIKEOUT       0x0004
#define APP_CFF_BOLD            0x0008

BOOL FNEXPORT AboutDlgProc
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
);


BOOL FNGLOBAL AppProfileWriteBytes
(
    PCTSTR          pszKey,
    LPBYTE          pbStruct,
    UINT            cbStruct
);

BOOL FNGLOBAL AppProfileReadBytes
(
    PCTSTR          pszKey,
    LPBYTE          pbStruct,
    UINT            cbStruct
);


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  APP.C -- Public helper functions
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

int FNCGLOBAL AppMsgBox
(
    HWND            hwnd,
    UINT            fuStyle,
    PCTSTR          pszFormat,
    ...
);

int FNCGLOBAL AppMsgBoxId
(
    HWND            hwnd,
    UINT            fuStyle,
    UINT            uIdsFormat,
    ...
);

void FNGLOBAL AppHourGlass
(
    BOOL            fHourGlass
);

BOOL FNGLOBAL AppYield
(
    HWND            hwnd,
    BOOL            fIsDialog
);

int FNGLOBAL AppDialogBox
(
    HWND            hwnd,
    LPCTSTR         pszDlg,
    DLGPROC         pfnDlg,
    LPARAM          lParam
);

BOOL FNGLOBAL AppTitle
(
    HWND            hwnd,
    PCTSTR          pszFileTitle
);

int FNCGLOBAL AppSetWindowText
(
    HWND            hwnd,
    PCTSTR          pszFormat,
    ...
);

int FNCGLOBAL AppSetWindowTextId
(
    HWND            hwnd,
    UINT            uIdsFormat,
    ...
);

BOOL FNGLOBAL AppGetFileTitle
(
    PCTSTR          pszFilePath,
    PTSTR           pszFileTitle
);

BOOL FNGLOBAL AppGetFileName
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle,
    UINT            fuFlags
);

BOOL FNGLOBAL AppFileNew
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
);


//
//  fuFlags for AppGetFileName()...
//
#define APP_GFNF_OPEN       0x0000
#define APP_GFNF_SAVE       0x0001


BOOL FNGLOBAL AppFileSave
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle,
    BOOL            fSaveAs
);


LRESULT FNEXPORT AppWndProc
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
);


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  global variables, etc.
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

//
//  flags for gfuAppOptions
//
#define APP_OPTF_EDITONLY       0x0001
#define APP_OPTF_YIELDEXEC      0x0002
#define APP_OPTF_DEBUGLOG       0x0004
#ifdef UNICODE
#define APP_OPTF_UNICODE        0x0008
#endif

#define APP_OPTF_DEVICELIST     0x8000


extern HINSTANCE    ghinst;
extern UINT         gfuAppOptions;

extern TCHAR        gszAppSection[];
extern TCHAR        gszNull[];

extern TCHAR        gszAppName[APP_MAX_APP_NAME_CHARS];
extern TCHAR        gszFileUntitled[APP_MAX_FILE_TITLE_CHARS];
extern TCHAR        gszAppFileTitle[APP_MAX_FILE_TITLE_CHARS];
extern TCHAR        gszAppFilePath[APP_MAX_FILE_PATH_CHARS];


