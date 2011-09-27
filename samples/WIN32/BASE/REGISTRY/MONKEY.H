
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define IDE_NAME                    101
#define IDE_TITLEINDEX              102
#define IDE_ACL                     104
#define IDE_LASTWRT                 105
#define IDE_VALUE1                  106
#define IDE_VALUE2                  107
#define IDE_VALUE3                  108
#define IDE_CLASS                   109
#define IDB_NEXT                    111
#define IDB_BACK                    112
#define IDL_LISTBOX                 110
#define IDL_LISTBOX2                103
#define IDE_CVALUES                 113
#define IDB_PRINT                   114
#define IDR_FULL                    115
#define IDE_TEXTOUT                 116
#define IDR_TRIMMED                 117

#define MAX_VALUE_NAME              128
#define MAX_DATA_LEN                1024

#define LINE_LEN                    80

#define BRANCH                      1
#define CHILD                       2
#define OTHER                       3

#define BOOT_INI                    "c:\\boot.ini"
#define REGISTRY_TXT                "registry.txt"

// MAX_PATH is defined as 260 in windef.h

#define WM_GETFIRSTKEY              WM_APP

int  APIENTRY MonkeyDlgProc (HWND, WORD, LONG, LONG);

VOID EnumerateLevel (HWND hDlg, LPTSTR NameLBSelect,
                     LPTSTR RegPath, HKEY *hKeyRoot);

VOID QueryKey (HWND, HANDLE);
VOID DisplayKeyData (HWND, LPTSTR, HANDLE);
