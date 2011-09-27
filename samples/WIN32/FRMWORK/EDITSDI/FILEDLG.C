// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: filedlg.c
//
//  PURPOSE: Shows basic use of "Open" and "Save As" common dialogs.
//
//
//  FUNCTIONS:
//
//
//  COMMENTS:
//
//
//
//  SPECIAL INSTRUCTIONS: N/A
//

#include <windows.h>            // required for all Windows applications
#ifdef WIN16
#include <commdlg.h>
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

static char szDirName[256] = {'\0'};    // directory string
void SetDirectory(LPOPENFILENAME);

//
//  FUNCTION: CmdFileOpen(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Call the open common dialog and show its results.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_OPEN (Unused)
//    wNotify  - (Unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//
//  COMMENTS:
//
//

LRESULT CmdFileOpen(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) {
    OPENFILENAME ofn = {0}; // common dialog box structure
    char szFile[256];       // filename string
    char szFileTitle[256];  // file-title string
    char szFilter[256];     // filter string
    char chReplace;         // strparator for szFilter
    int i, cbString;        // integer count variables

    if (!QuerySaveFile(hwnd)) {
        return(0);
    }

    // Retrieve the current directory name and store it in szDirName.

    if (szDirName[0] == '\0') {
#ifdef WIN16
        GetWindowsDirectory(szDirName, sizeof(szDirName));
#else
        GetCurrentDirectory(sizeof(szDirName), szDirName);
#endif
    }

    // Place the terminating null character in the szFile.

    szFile[0] = '\0';

    // Load the filter string from the resource file.

    cbString = LoadString(hInst, IDS_FILTERSTRING, szFilter, sizeof(szFilter));

    // Add a terminating null character to the filter string.

    chReplace = szFilter[cbString - 1];
    for (i = 0; szFilter[i] != '\0'; i++){
        if (szFilter[i] == chReplace)
            szFilter[i] = '\0';
    }

    // Set the members of the OPENFILENAME structure.

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = szDirName;
    ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box.

    if (GetOpenFileName(&ofn)) {
        Open(ofn.lpstrFile,MAKEBOOL(!(ofn.Flags && OFN_NOREADONLYRETURN)),hwnd);
        SetDirectory(&ofn);
    }
    return 0;
}


//
//  FUNCTION: CmdFileSaveAs(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Call the SaveAs common dialog and show the results.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_SAVEAS (Unused)
//    wNotify  - (Unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//
//  COMMENTS:
//
//

LRESULT CmdFileSaveAs(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) {
    OPENFILENAME ofn = {0}; // common dialog box structure
    char szFile[256];       // filename string
    char szFileTitle[256];  // file-title string
    char szFilter[256];     // filter string
    char chReplace;         // string separator for szFilter
    int i, cbString;        // integer count variables

    // Retrieve the current directory name and store it in szDirName.

    if (szDirName[0] == '\0') {
#ifdef WIN16
        GetWindowsDirectory(szDirName, sizeof(szDirName));
#else
        GetCurrentDirectory(sizeof(szDirName), szDirName);
#endif
    }

    // Place the terminating null character in szFile.

    szFile[0] = '\0';

    // Load the filter string from the .RC file.

    cbString = LoadString(hInst, IDS_FILTERSTRING, szFilter, sizeof(szFilter));

    // Add a terminating null character to the filter string.

    chReplace = szFilter[cbString - 1];

    for (i = 0; szFilter[i] != '\0'; i++) {
        if (szFilter[i] == chReplace)
            szFilter[i] = '\0';
    }

    // Set the members of the OPENFILENAME structure.

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);

    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = szDirName;
    ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

    // Display the Save As dialog box.

    if (GetSaveFileName(&ofn))
    {
        SaveAs(ofn.lpstrFile,hwnd);
        SetDirectory(&ofn);
        return 1L;
    }

    return 0;
}


void SetDirectory(LPOPENFILENAME lpofn) {
    lpofn->lpstrFile[lpofn->nFileOffset] = '\0';
    lstrcpy(szDirName,lpofn->lpstrFile);
}
