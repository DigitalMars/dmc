//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//--------------------------------------------------------------------------;
//
//  mixapp.h
//
//==========================================================================;

#ifndef _INC_MIXAPP
#define _INC_MIXAPP                 // #defined if file has been included

#ifndef RC_INVOKED
#pragma pack(1)                     // assume byte packing throughout
#endif

#ifndef EXTERN_C
#ifdef __cplusplus
    #define EXTERN_C extern "C"
#else
    #define EXTERN_C extern
#endif
#endif

#ifdef __cplusplus
extern "C"                          // assume C declarations for C++
{
#endif

//
//Some useful stuff to clarify the code a bit
//
#ifndef FNLOCAL
    #ifdef DEBUG
        #define FNLOCAL     _stdcall
    #else
        #define FNLOCAL     static _stdcall
    #endif
    #define FNGLOBAL    _stdcall
    #define FNCGLOBAL   _cdecl
#endif


//
//  for compiling Unicode
//
#ifdef UNICODE
    #define SIZEOF(x)   (sizeof(x)/sizeof(WCHAR))
#else
    #define SIZEOF(x)   sizeof(x)
#endif


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

#define APP_VERSION_NAME_RC         "mixapp32.exe\0"
#define APP_VERSION_BYLINE_RC       "\0"
#define APP_VERSION_COMPANYNAME_RC  "Microsoft Corporation\0"
#define APP_VERSION_COPYRIGHT_RC    "Copyright \251 1985-1995 Microsoft Corp.\0"

#if (defined(_X86_)) || (defined(i386))
#define APP_VERSION_PRODUCTNAME_RC  "Microsoft Windows Sample Application (i386)\0"
#endif
#if (defined(_MIPS_)) || (defined(MIPS))
#define APP_VERSION_PRODUCTNAME_RC  "Microsoft Windows Sample Application (MIPS)\0"
#endif
#if (defined(_ALPHA_)) || (defined(ALPHA))
#define APP_VERSION_PRODUCTNAME_RC  "Microsoft Windows Sample Application (Alpha)\0"
#endif
#ifndef APP_VERSION_PRODUCTNAME_RC
#define APP_VERSION_PRODUCTNAME_RC  "Microsoft Windows Sample Application\0"
#endif

#ifdef DEBUG
#define APP_VERSION_DESCRIPTION_RC  "Microsoft Audio Mixer Manager Sample Application (debug)\0"
#else
#define APP_VERSION_DESCRIPTION_RC  "Microsoft Audio Mixer Manager Sample Application\0"
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
#define APP_MAX_APP_NAME_CHARS          30
#define APP_MAX_APP_NAME_BYTES          (APP_MAX_APP_NAME_CHARS * sizeof(TCHAR))
#define APP_MAX_STRING_RC_CHARS         512
#define APP_MAX_STRING_RC_BYTES         (APP_MAX_STRING_RC_CHARS * sizeof(TCHAR))
#define APP_MAX_STRING_ERROR_CHARS      512
#define APP_MAX_STRING_ERROR_BYTES      (APP_MAX_STRING_ERROR_CHARS * sizeof(TCHAR))

#define APP_WINDOW_XOFFSET              CW_USEDEFAULT
#define APP_WINDOW_YOFFSET              CW_USEDEFAULT
#define APP_WINDOW_WIDTH                500 //CW_USEDEFAULT
#define APP_WINDOW_HEIGHT               300 //CW_USEDEFAULT


//
//  resource defines...
//
#define ICON_APP                        RCID(10)
#define ACCEL_APP                       RCID(15)


//
//  the application menu...
//
//
#define MENU_APP                        RCID(20)
#define APP_MENU_ITEM_FILE              0
#define IDM_FILE_MIXER_DEVICE           1000
#define IDM_FILE_FONT                   1001
#define IDM_FILE_DEBUG_LOG              1005
#define IDM_FILE_ABOUT                  1009
#define IDM_FILE_EXIT                   1010

#define APP_MENU_ITEM_VIEW              1
#define IDM_VIEW_LINE_INFO              1050
#define IDM_VIEW_LINE_CONTROLS          1051

#define IDM_UPDATE                      1100



//
//  the main window control id's...
//
#define IDD_APP_LIST_LINES                  100


//
//  misc dlg boxes...
//
#define DLG_ABOUT                           RCID(50)
#define IDD_ABOUT_VERSION_OS                100
#define IDD_ABOUT_VERSION_PLATFORM          101

#define IDD_ABOUT_VERSION_MMSYSTEM          150



#define DLG_MIXAPP_DEVICE                   RCID(51)
#define IDD_MADEVICE_COMBO_DEVICE           100
#define IDD_MADEVICE_EDIT_CAPABILITIES      101



#define DLG_LINEINFO                        RCID(55)
#define IDD_LINEINFO_EDIT_INFO              100


#define DLG_LINECONTROLS                    RCID(60)
#define IDD_LINECONTROLS_STATIC_POSITION    100
#define IDD_LINECONTROLS_LIST_CONTROLS      101
#define IDD_LINECONTROLS_BTN_INFO           102
#define IDD_LINECONTROLS_BTN_SETTINGS       103

#define DLG_CONTROLINFO                     RCID(61)
#define IDD_CONTROLINFO_EDIT_INFO           100



//
//  mainit.c
//
//
//
BOOL CALLBACK AboutDlgProc
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
);

BOOL FNGLOBAL MixAppChooseFont
(
    HWND            hwnd
);

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
    HINSTANCE               hinst,
    int                     nResult
);






//
//  macntrls.c
//
//
//
BOOL CALLBACK MixAppDlgProcLineControls
(
    HWND                    hwnd,
    UINT                    uMsg,
    WPARAM                  wParam,
    LPARAM                  lParam
);

LRESULT FNGLOBAL MixAppControlChange
(
    HWND                    hwnd,
    HMIXER                  hmx,
    DWORD                   dwControlID
);



//
//  mixapp.c
//
//
//
LRESULT CALLBACK AppWndProc
(
    HWND                    hwnd,
    UINT                    uMsg,
    WPARAM                  wParam,
    LPARAM                  lParam
);


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  string resources
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

#define IDS_APP_NAME                100



//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  Public function prototypes
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

void FNGLOBAL AppHourGlass
(
    BOOL                    fHourGlass
);
int FNCGLOBAL AppMsgBox
(
    HWND                    hwnd,
    UINT                    fuStyle,
    PTSTR                   pszFormat,
    ...
);
int FNCGLOBAL AppSetWindowText
(
    HWND                    hwnd,
    PTSTR                   pszFormat,
    ...
);
void FNCGLOBAL MixAppDebugLog
(
    PTSTR                   pszFormat,
    ...
);
//
//
//
//
int FNCGLOBAL MEditPrintF
(
    HWND            hedit,
    PTSTR           pszFormat,
    ...
);


HMIXER FNGLOBAL MixAppNewDevice
(
    HWND                    hwnd,
    HMIXER                  hmxCur,
    UINT                    uMxId
);

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  global variables, etc.
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

#define APP_OPTF_DEBUGLOG       0x0004


extern HINSTANCE    ghinst;
extern UINT         gfuAppOptions;

extern TCHAR        gszNull[];
extern TCHAR        gszCRLF[];

extern TCHAR        gszAppName[APP_MAX_APP_NAME_CHARS];


//
//
//
#include "tlb.h"

extern PTABBEDLISTBOX   gptlbLines;

extern HMIXER           ghmx;

extern BOOL             gfDisplayingControl;
extern DWORD            gdwControlID;
extern HWND             ghdlgControl;
extern MIXERLINE        gmxl;
extern MIXERCONTROL     gmxctrl;




BOOL CALLBACK MixAppDlgProcControlList
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
);


//
//  malines.c
//
//
//
BOOL CALLBACK MixAppDlgProcLineInfo
(
    HWND                    hwnd,
    UINT                    uMsg,
    WPARAM                  wParam,
    LPARAM                  lParam
);

BOOL FNGLOBAL MixAppRefreshLineList
(
    HWND                    hwnd,
    PTABBEDLISTBOX          ptlb
);

LRESULT FNGLOBAL MixAppLineChange
(
    HWND                    hwnd,
    HMIXER                  hmx,
    DWORD                   dwLineID
);


BOOL CALLBACK MixAppDlgProcControlMeter
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
);

BOOL CALLBACK MixAppDlgProcControlSwitch
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
);

BOOL CALLBACK MixAppDlgProcControlFader
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
);


BOOL CALLBACK MixAppDlgProcDevice
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
);

BOOL FNGLOBAL MixAppGetControlTypeName
(
    LPMIXERCONTROL          pmxctrl,
    LPTSTR                  szControlType
);


//
//
//
//
#define DLG_MIXAPP_CONTROL              RCID(42)
#define IDD_MACONTROL_GRP_MULTICHANNEL  100
#define IDD_MACONTROL_GRP_UNIFORM       101
#define IDD_MACONTROL_TXT_SHORT_NAME    102
#define IDD_MACONTROL_TXT_LONG_NAME     103
#define IDD_MACONTROL_TXT_VALUE         104
#define IDD_MACONTROL_TXT_BOUNDS        105
#define IDD_MACONTROL_TXT_METRICS       106
#define IDD_MACONTROL_TXT_LINEINFO      107

#define IDD_MACONTROL_MULTICHANNEL_BASE 200
#define IDD_MACONTROL_UNIFORM_BASE      300


//
//
//
typedef struct tMACONTROLINSTANCE
{
    HMIXER                      hmx;
    LPMIXERLINE                 pmxl;
    LPMIXERCONTROL              pmxctrl;

} MACONTROLINSTANCE, FAR *LPMACONTROLINSTANCE;


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

#ifndef RC_INVOKED
#pragma pack()                      // revert to default packing
#endif

#ifdef __cplusplus
}                                   // end of extern "C" {
#endif

#endif // _INC_APPPORT
