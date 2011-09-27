/*
 * GIZMOBAR.H
 * GizmoBar Version 1.01
 *
 * Public definitions for application that use the GizmoBar such as
 * messages, prototypes for API functions, notification codes, and
 * control styles.
 *
 * Copyright (c)1993-1995 Microsoft Corporation, All Rights Reserved
 *
 * Kraig Brockschmidt, Software Design Engineer
 * Microsoft Systems Developer Relations
 *
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  >INTERNET:kraigb@microsoft.com
 */


#ifndef _GIZMOBAR_H_
#define _GIZMOBAR_H_

#include <bttncur.h>

#ifdef __cplusplus
extern "C"
    {
#endif


//Classname
#define CLASS_GIZMOBAR  TEXT("gizmobar")


//Message API Functions
HWND    WINAPI GBHwndAssociateSet(HWND, HWND);
HWND    WINAPI GBHwndAssociateGet(HWND);

BOOL    WINAPI GBGizmoAddW(HWND, UINT, UINT, UINT, UINT, UINT, LPWSTR , HBITMAP, UINT, UINT);
BOOL    WINAPI GBGizmoAddA(HWND, UINT, UINT, UINT, UINT, UINT, LPSTR , HBITMAP, UINT, UINT);

int     WINAPI GBGizmoTextGetW(HWND, UINT, LPWSTR, UINT);
int     WINAPI GBGizmoTextGetA(HWND, UINT, LPSTR, UINT);

void    WINAPI GBGizmoTextSetW(HWND, UINT, LPWSTR);
void    WINAPI GBGizmoTextSetA(HWND, UINT, LPSTR);

#ifdef UNICODE
#define GBGizmoAdd      GBGizmoAddW
#define GBGizmoTextGet  GBGizmoTextGetW
#define GBGizmoTextSet  GBGizmoTextSetW
#else
#define GBGizmoAdd      GBGizmoAddA
#define GBGizmoTextGet  GBGizmoTextGetA
#define GBGizmoTextSet  GBGizmoTextSetA
#endif

BOOL    WINAPI GBGizmoRemove(HWND, UINT);

LRESULT WINAPI GBGizmoSendMessage(HWND, UINT, UINT, WPARAM, LPARAM);

BOOL    WINAPI GBGizmoShow(HWND, UINT, BOOL);
BOOL    WINAPI GBGizmoEnable(HWND, UINT, BOOL);
BOOL    WINAPI GBGizmoCheck(HWND, UINT, BOOL);
UINT    WINAPI GBGizmoFocusSet(HWND, UINT);
BOOL    WINAPI GBGizmoExist(HWND, UINT);

int     WINAPI GBGizmoTypeGet(HWND, UINT);

DWORD   WINAPI GBGizmoDataSet(HWND, UINT, DWORD);
DWORD   WINAPI GBGizmoDataGet(HWND, UINT);
BOOL    WINAPI GBGizmoNotifySet(HWND, UINT, BOOL);
BOOL    WINAPI GBGizmoNotifyGet(HWND, UINT);

UINT    WINAPI GBGizmoIntGet(HWND, UINT, BOOL FAR *, BOOL);
void    WINAPI GBGizmoIntSet(HWND, UINT, UINT, BOOL);



//Notification codes sent via WM_COMMAND from GBHwndAssociateSet
#define GBN_ASSOCIATEGAIN               1
#define GBN_ASSOCIATELOSS               2
#define GBN_GIZMOADDED                  3
#define GBN_GIZMOREMOVED                4

//Message equivalents for functions.
#define GBM_HWNDASSOCIATESET            (WM_USER+0)
#define GBM_HWNDASSOCIATEGET            (WM_USER+1)
#define GBM_GIZMOADDW                   (WM_USER+2)
#define GBM_GIZMOADDA                   (WM_USER+3)
#define GBM_GIZMOREMOVE                 (WM_USER+4)
#define GBM_GIZMOSENDMESSAGE            (WM_USER+5)
#define GBM_GIZMOSHOW                   (WM_USER+6)
#define GBM_GIZMOENABLE                 (WM_USER+7)
#define GBM_GIZMOCHECK                  (WM_USER+8)
#define GBM_GIZMOFOCUSSET               (WM_USER+9)
#define GBM_GIZMOEXIST                  (WM_USER+10)
#define GBM_GIZMOTYPEGET                (WM_USER+11)
#define GBM_GIZMODATASET                (WM_USER+12)
#define GBM_GIZMODATAGET                (WM_USER+13)
#define GBM_GIZMONOTIFYSET              (WM_USER+14)
#define GBM_GIZMONOTIFYGET              (WM_USER+15)
#define GBM_GIZMOTEXTGETA               (WM_USER+16)
#define GBM_GIZMOTEXTGETW               (WM_USER+17)
#define GBM_GIZMOTEXTSETA               (WM_USER+18)
#define GBM_GIZMOTEXTSETW               (WM_USER+19)
#define GBM_GIZMOINTGET                 (WM_USER+20)
#define GBM_GIZMOINTSET                 (WM_USER+21)

#ifdef UNICODE
#define GBM_GIZMOTEXTGET GBM_GIZMOTEXTGETW
#define GBM_GIZMOTEXTSET GBM_GIZMOTEXTSETW
#define GBM_GIZMOADD GBM_GIZMOADDW
#else
#define GBM_GIZMOTEXTGET GBM_GIZMOTEXTGETA
#define GBM_GIZMOTEXTSET GBM_GIZMOTEXTSETA
#define GBM_GIZMOADD GBM_GIZMOADDA
#endif



/*
 * Structure passed in lParam of GBM_GIZMOADD that mirrors the
 * parameters to GBGizmoAdd.
 */

typedef struct
    {
    HWND        hWndParent;         //Parent window
    UINT        iType;              //Type of gizmo
    UINT        iGizmo;             //Position to create gizmo
    UINT        uID;                //Identifier of gizmo
    UINT        dx;                 //Dimensions of gizmo
    UINT        dy;
    LPSTR       pszText;            //Gizmo text
    HBITMAP     hBmp;               //Source of gizmo button image.
    UINT        iImage;             //Index of image from hBmp
    UINT        uState;             //Initial state of the gizmo.
    } CREATEGIZMOA, FAR *LPCREATEGIZMOA;
typedef struct
    {
    HWND        hWndParent;         //Parent window
    UINT        iType;              //Type of gizmo
    UINT        iGizmo;             //Position to create gizmo
    UINT        uID;                //Identifier of gizmo
    UINT        dx;                 //Dimensions of gizmo
    UINT        dy;
    LPWSTR      pszText;            //Gizmo text
    HBITMAP     hBmp;               //Source of gizmo button image.
    UINT        iImage;             //Index of image from hBmp
    UINT        uState;             //Initial state of the gizmo.
    } CREATEGIZMOW, FAR *LPCREATEGIZMOW;

#ifdef UNICODE
typedef CREATEGIZMOW    CREATEGIZMO;
typedef LPCREATEGIZMOW  LPCREATEGIZMO;
#define CBCREATEGIZMO sizeof(CREATEGIZMOW)
#else
typedef CREATEGIZMOA    CREATEGIZMO;
typedef LPCREATEGIZMOA  LPCREATEGIZMO;
#define CBCREATEGIZMO sizeof(CREATEGIZMOA)
#endif


//For GBM_GIZMOSENDMESSAGE
typedef struct
    {
    UINT        iMsg;
    WPARAM      wParam;
    LPARAM      lParam;
    } GBMSG, FAR * LPGBMSG;

#define CBGBMSG sizeof(GBMSG);

//For GBM_GIZMOGETTEXT
typedef struct
    {
    LPSTR       psz;
    UINT        cch;
    } GBGETTEXTA, FAR * LPGBGETTEXTA;
typedef struct
    {
    LPWSTR      psz;
    UINT        cch;
    } GBGETTEXTW, FAR * LPGBGETTEXTW;

#ifdef UNICODE
typedef GBGETTEXTW  GBGETTEXT;
typedef LPGBGETTEXTW  LPGBGETTEXT;
#define CBGBGETTEXT sizeof(GBGETTEXTW);
#else
typedef GBGETTEXTA  GBGETTEXT;
typedef LPGBGETTEXTA  LPGBGETTEXT;
#define CBGBGETTEXT sizeof(GBGETTEXTA);
#endif


//For GBM_GIZMOGETINT
typedef struct
    {
    BOOL        fSigned;
    BOOL        fSuccess;
    } GBGETINT, FAR * LPGBGETINT;

#define CBGBGETINT sizeof(GBGETINT);


//For GBM_GIZMOSETINT
typedef struct
    {
    UINT        uValue;
    BOOL        fSigned;
    } GBSETINT, FAR * LPGBSETINT;

#define CBGBSETINT sizeof(GBSETINT);



//Gizmo control types.  DO NOT CHANGE THESE!
#define GIZMOTYPE_EDIT                  0x0001
#define GIZMOTYPE_LISTBOX               0x0002
#define GIZMOTYPE_COMBOBOX              0x0004
#define GIZMOTYPE_BUTTONNORMAL          0x0008
#define GIZMOTYPE_TEXT                  0x0010
#define GIZMOTYPE_SEPARATOR             0x0020
#define GIZMOTYPE_BUTTONATTRIBUTEIN     0x0040
#define GIZMOTYPE_BUTTONATTRIBUTEEX     0x0080
#define GIZMOTYPE_BUTTONCOMMAND         0x0100


//Generic state flags for non-buttons based on BTTNCUR.H's groups.
#define GIZMO_NORMAL                    (BUTTONGROUP_ACTIVE)
#define GIZMO_DISABLED                  (BUTTONGROUP_DISABLED)



#ifdef __cplusplus
    }   //Match with extern "C" above.
#endif



#endif //_GIZMOBAR_H_
