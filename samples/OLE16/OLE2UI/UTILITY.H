/*
 * UTILITY.H
 *
 * Miscellaneous prototypes and definitions for OLE UI dialogs.
 *
 * Copyright (c)1992 Microsoft Corporation, All Right Reserved
 */


#ifndef _UTILITY_H_
#define _UTILITY_H_

//Function prototypes
//UTILITY.C
HCURSOR  WINAPI HourGlassOn(void);
void     WINAPI HourGlassOff(HCURSOR);

BOOL     WINAPI Browse(HWND, LPSTR, LPSTR, UINT, UINT, DWORD);
int      WINAPI ReplaceCharWithNull(LPSTR, int);
int      WINAPI ErrorWithFile(HWND, HINSTANCE, UINT, LPSTR, UINT);
HFILE WINAPI DoesFileExist(LPSTR lpszFile, OFSTRUCT FAR* lpOpenBuf);


HICON FAR PASCAL    HIconAndSourceFromClass(REFCLSID, LPSTR, UINT FAR *);
BOOL FAR PASCAL FIconFileFromClass(REFCLSID, LPSTR, UINT, UINT FAR *);
LPSTR FAR PASCAL PointerToNthField(LPSTR, int, char);
BOOL FAR PASCAL GetAssociatedExecutable(LPSTR, LPSTR);
HICON    WINAPI HIconFromClass(LPSTR);
BOOL     WINAPI FServerFromClass(LPSTR, LPSTR, UINT);
UINT     WINAPI UClassFromDescription(LPSTR, LPSTR, UINT);
UINT     WINAPI UDescriptionFromClass(LPSTR, LPSTR, UINT);
BOOL     WINAPI FVerbGet(LPSTR, UINT, LPSTR);
LPSTR    WINAPI ChopText(HWND hwndStatic, int nWidth, LPSTR lpch);
void     WINAPI OpenFileError(HWND hDlg, UINT nErrCode, LPSTR lpszFile);


#endif //_UTILITY_H_
