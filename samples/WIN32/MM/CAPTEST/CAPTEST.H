/****************************************************************************
 *
 *   captest.h: Include file for the CapTest Sample Program
 *
 *   Microsoft Video for Windows Capture Class Sample Program
 *
 ***************************************************************************/
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


#include "dialogs.h"

//
// General purpose constants...
//
#define MAXVIDDRIVERS            10
#define MS_FOR_15FPS             66
#define DEF_PALNUMFRAMES         10
#define DEF_PALNUMCOLORS         236L
#define ONEMEG                   (1024L * 1024L)

//
// Menu Ids...
//
#define IDM_FILE                  100
#define IDM_F_SETCAPTUREFILE      101
#define IDM_F_SAVEVIDEOAS         102
#define IDM_F_ALLOCATESPACE       103
#define IDM_F_EXIT                104

#define IDM_EDIT                  110
#define IDM_E_COPY                111
#define IDM_E_PASTEPALETTE        112

#define IDM_CAPTURE               120
#define IDM_C_CAPTUREVIDEO        121
#define IDM_C_CAPTUREFRAME        122
#define IDM_C_CAPTURESETTINGS     123

#define IDM_OPTIONS               130
#define IDM_O_PREVIEW             131
#define IDM_O_OVERLAY             132
#define IDM_O_AUDIOFORMAT         133
#define IDM_O_VIDEOFORMAT         134
#define IDM_O_VIDEOSOURCE         135
#define IDM_O_VIDEODISPLAY        136
#define IDM_O_PALETTE             137
#define IDM_O_CHOOSECOMPRESSOR    140

// !!! Leave some room after this define for the drivers
#define IDM_O_DRIVERS             150
#define IDM_O_DRIVERS1            151
#define IDM_O_DRIVERS2            152
#define IDM_O_DRIVERS3            153
#define IDM_O_DRIVERS4            154
#define IDM_O_DRIVERS5            155
#define IDM_O_DRIVERS6            156
#define IDM_O_DRIVERS7            157
#define IDM_O_DRIVERS8            158
#define IDM_O_DRIVERS9            159
// The above are just reserved dummy IDs.

#define IDM_HELP                  190
#define IDM_H_ABOUT               191

#define IDS_ERR_REGISTER_CLASS   1
#define IDS_ERR_CREATE_WINDOW    2


//
// Macro Definitions...
//
#define IsDriverIndex(w) ( ((w) >= IDM_O_DRIVERS)  &&  \
                           ((w) - IDM_O_DRIVERS < MAXVIDDRIVERS) )

#define Width(X)  ((X).right - (X).left)
#define Height(X) ((X).bottom - (X).top)


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ; 
// 
//  Win 16/32 portability stuff...
// 
// 
// 
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ; 

#ifndef WIN32
    #ifndef FNLOCAL
        #define FNLOCAL     NEAR PASCAL
        #define FNCLOCAL    NEAR _cdecl
        #define FNGLOBAL    FAR PASCAL
        #define FNCGLOBAL   FAR _cdecl
    #ifdef _WINDLL
        #define FNWCALLBACK FAR PASCAL __loadds
        #define FNEXPORT    FAR PASCAL __export
    #else
        #define FNWCALLBACK FAR PASCAL __loadds
        #define FNEXPORT    FAR PASCAL __export
    #endif
    #endif

    //
    //
    //
    //
    #ifndef FIELD_OFFSET
    #define FIELD_OFFSET(type, field)    ((LONG)&(((type *)0)->field))
    #endif

    //
    //  based code makes since only in win 16 (to try and keep stuff out of
    //  our fixed data segment...
    //
    #define BCODE           _based(_segname("_CODE"))

    #define HUGE            _huge

    //
    //  stuff for Unicode in Win 32--make it a noop in Win 16
    //
    #ifndef _TCHAR_DEFINED
        #define _TCHAR_DEFINED
        typedef char            TCHAR, *PTCHAR;
        typedef unsigned char   TBYTE, *PTUCHAR;

        typedef PSTR            PTSTR, PTCH;
        typedef LPSTR           LPTSTR, LPTCH;
        typedef LPCSTR          LPCTSTR;
    #endif

    #define TEXT(a)         a
    #define SIZEOF(x)       sizeof(x)
    #define SIZEOFACMSTR(x) sizeof(x)
#else
    #ifndef FNLOCAL
        #define FNLOCAL     _stdcall
        #define FNCLOCAL    _stdcall
        #define FNGLOBAL    _stdcall
        #define FNCGLOBAL   _stdcall
        #define FNWCALLBACK CALLBACK
        #define FNEXPORT    CALLBACK
    #endif

    #ifndef _CRTAPI1
    #define _CRTAPI1    __cdecl
    #endif
    #ifndef _CRTAPI2
    #define _CRTAPI2    __cdecl
    #endif
    #ifndef try
    #define try         __try
    #define leave       __leave
    #define except      __except
    #define finally     __finally
    #endif


    //
    //  there is no reason to have based stuff in win 32
    //
    #define BCODE

    #define HUGE
    #define HTASK                   HANDLE
    #define SELECTOROF(a)           (a)
    typedef LRESULT (CALLBACK* DRIVERPROC)(DWORD, HDRVR, UINT, LPARAM, LPARAM);

    //
    //  for compiling Unicode
    //
    #ifdef UNICODE
        #define SIZEOF(x)   (sizeof(x)/sizeof(WCHAR))
    #else
        #define SIZEOF(x)   sizeof(x)
    #endif
    #define SIZEOFACMSTR(x)	(sizeof(x)/sizeof(WCHAR))
#endif


//
// Global Variables...
//
extern TCHAR           gachAppName[] ;
extern TCHAR           gachIconName[] ;
extern TCHAR           gachMenuName[] ;
extern TCHAR           gachString[] ;
extern TCHAR           gachMCIDeviceName[] ;

extern HINSTANCE      ghInstApp ;
extern HWND           ghWndMain ;
extern HWND           ghWndCap ;
extern HANDLE         ghAccel ;
extern WORD           gwDeviceIndex ;
extern WORD           gwPalFrames ;
extern WORD           gwPalColors ;
extern WORD           gwCapFileSize ;

extern CAPSTATUS      gCapStatus ;
extern CAPDRIVERCAPS  gCapDriverCaps ;
extern CAPTUREPARMS   gCapParms ;

extern LPWAVEFORMATEX glpwfex ;

//
// Dialog Box Procedures...
//
int FAR PASCAL AboutProc(HWND, UINT, UINT, LONG) ;
int FAR PASCAL AudioFormatProc(HWND, UINT, UINT, LONG) ;
int FAR PASCAL CapSetUpProc(HWND, UINT, UINT, LONG) ;
BOOL CALLBACK MakePaletteProc(HWND, UINT, UINT, LONG) ;
int FAR PASCAL AllocCapFileProc(HWND, UINT, UINT, LONG) ;
