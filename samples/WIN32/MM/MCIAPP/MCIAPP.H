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
//  mciapp.h
//
//  Description:
//      This file contains public definitions, etc for the MCI App specific
//      code.
//
//  History:
//       2/ 7/93    created.
//
//==========================================================================;


//
//
//
#ifndef _MCIERROR_
#define _MCIERROR_
typedef DWORD       MCIERROR;
#endif

#ifndef _MCIDEVICEID_
#define _MCIDEVICEID_
typedef UINT        MCIDEVICEID;
#endif


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  file manipulation functions
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

BOOL FNGLOBAL MciAppFileSaveModified
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
);

BOOL FNGLOBAL MciAppFileNew
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
);

BOOL FNGLOBAL MciAppFileOpen
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
);

BOOL FNGLOBAL MciAppFileSave
(
    HWND            hwnd,
    PCTSTR          pszFilePath
);


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  misc functions
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

BOOL FNGLOBAL MciAppChooseFont
(
    HWND            hwnd
);

BOOL FNGLOBAL MciAppUpdateOptions
(
    HWND            hwnd
);

BOOL FNGLOBAL MciAppResetStatus
(
    HWND            hwnd
);

BOOL FNGLOBAL MciAppHandleNotify
(
    HWND            hwnd,
    UINT            fuNotify,
    UINT            uId
);

MCIERROR FNGLOBAL MciAppSingleStep
(
    HWND            hwnd
);

MCIERROR FNGLOBAL MciAppExecute
(
    HWND            hwnd,
    BOOL            fYield
);

BOOL FNGLOBAL MciAppEnterLine
(
    HWND            hwnd,
    BOOL            fEditOnly
);


UINT FNGLOBAL MciAppGetNumDevices
(
    HWND            hwnd
);

BOOL FNGLOBAL MciAppCloseAllDevices
(
    HWND            hwnd
);

BOOL FNEXPORT MciAppDeviceDlgProc
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
);

BOOL FNGLOBAL MciAppDeviceList
(
    HWND            hwnd,
    BOOL            fActivate
);

BOOL FNGLOBAL MciAppDispatchMessage
(
    HWND            hwnd,
    PMSG            pmsg
);

BOOL FNGLOBAL MciAppSettingsSave
(
    HWND            hwnd
);

BOOL FNGLOBAL MciAppShutdown
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
);

BOOL FNGLOBAL MciAppInit
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle,
    LPTSTR          pszCmdLine,
    int             nCmdShow
);

BOOL FNGLOBAL MciAppExit
(
    void
);



//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;
//
//  global variables, etc.
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ;

extern BOOL         gfExecuting;
extern BOOL         gfAbortExec;


