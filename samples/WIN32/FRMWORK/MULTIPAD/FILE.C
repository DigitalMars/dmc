// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: file.c
//
//  PURPOSE: File manipulation functions.
//
//  FUNCTIONS:
//    CmdFileSave   - Save the current document.
//    Open          - Open a file based on a file name and readonly attribute.
//    QuerySaveFile - Save the current document if necessary & the user 
//                      agrees.
//    SetNewBuffer  - Create a new edit buffer.
//    SaveFile      - Save the text from the editor control to the current 
//                      file.
//    SaveAs        - Save the edit text to the specified file.
//
//  COMMENTS:
//


#include <windows.h>            // required for all Windows applications
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

void SetNewBuffer(HANDLE);
BOOL SaveFile(HWND);

char szFName[256];
static char szUntitled[] = "EditSDI - (untitled)";// default window title


//
//  FUNCTION: CmdFileSave(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Save the current document.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_FILESAVE (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdFileSave(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    // If there is no filename, use the saveas command to get one.
    //   Otherwise, save the file using the current filename.

    if (!GetProp(GetActiveMDIChild(), "FilenameSet"))
    {
        CmdFileSaveAs(hwnd, wCommand, 0, hwndCtrl);
    }
    else
    {
        // If the document has not been modified, don't bother saving it.
        if (SendMessage(GetEditWindow(NULL), EM_GETMODIFY, FALSE, 0L))
            SaveFile(hwnd);
    }

    return 0;
}


//
//  FUNCTION: Open(LPSTR, BOOL, HWND)
//
//  PURPOSE: Open a file based on a file name and readonly attribute.
//
//  PARAMETERS:
//    lsz - The full path name of the file to be opened.
//    fReadOnly - Read only attribute of the file.
//    hwnd - The window into which the file is being opened.
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//
//

VOID Open(LPSTR lsz, BOOL fReadOnly, HWND hwnd)
{
    HFILE   hfile;      // Hande to the file being opened.
    HCURSOR hcursSave;  // The cursor replace by the hourglass during save.
    LPVOID lpvEdit;     // The edit buffer
    long   cchSize;     // The size of the edit buffer
    UINT   cchRead;     // The number of bytes read from the file
    HANDLE hszT;        // Handle to the edit buffer

    // Open the file and get its handle
    hfile = _lopen(lsz, OF_READ | OF_SHARE_DENY_WRITE);
    if (hfile == HFILE_ERROR)
        return;

    // Allocate edit buffer to the size of the file + 1
    cchSize = _llseek(hfile, 0L, 2);
    _llseek(hfile,0L,0);
    hszT = LocalAlloc(LMEM_MOVEABLE | LMEM_ZEROINIT, (UINT)cchSize + 1);

    if (!hszT)
    {
        MessageBox(hwnd, 
                   "Not enough memory.", 
                   SZAPPNAME, 
                   MB_OK | MB_ICONHAND);
        return;
    }
    hcursSave = SetCursor(hcursHourGlass);
    lpvEdit = LocalLock(hszT);

    cchRead = _lread(hfile, lpvEdit, (UINT)cchSize);
    _lclose(hfile);

    // # bytes read must equal file size
    if (cchRead == HFILE_ERROR ||  cchRead != (UINT)cchSize)
    {
        char rgchT[256];

        wsprintf(rgchT, "Error reading %s.", lsz);
        SetCursor(hcursSave);      // Remove the hourglass
        MessageBox(hwnd, rgchT, SZAPPNAME, MB_OK | MB_ICONEXCLAMATION);
    }
    LocalUnlock(hszT);

    // Set up a new buffer and window title
    {
        SetNewBuffer(hszT);
    }

    SendMessage(GetEditWindow(NULL), EM_SETREADONLY, fReadOnly, 0L);
    SetCursor(hcursSave);
    lstrcpy(GetFName(), lsz);
    SetProp(GetActiveMDIChild(), "FilenameSet", (HANDLE)1);
}

//
//  FUNCTION: QuerySaveFile(HWND)
//
//  PURPOSE: Save the current document if necessary & the user agrees.
//
//  PARAMETERS:
//    hwnd - The document window.
//
//  RETURN VALUE:
//    TRUE  - The file has been saved, or any changes can be thrown out.
//    FALSE - The use has changed his/her mind, don't continue the
//      operation that would throw away this document.
//
//  COMMENTS:
//
//

BOOL QuerySaveFile(HWND hwnd)
{
    int  idResponse;
    char rgch[256];

    if (SendMessage(GetEditWindow(NULL), EM_GETMODIFY, 0, 0L))
    {
        wsprintf(rgch, "Save current changes? %s", GetFName());
        idResponse = MessageBox(hwnd, 
                                rgch, 
                                SZAPPNAME,  
                                MB_YESNOCANCEL | MB_ICONEXCLAMATION);

        if (idResponse == IDYES)            // Save changes
        {
            // Make sure there is a filename to save to
            if (!GetProp(GetActiveMDIChild(), "FilenameSet"))
                return (BOOL)CmdFileSaveAs(hwnd, IDM_FILESAVEAS, 0, NULL);
            else
                SaveFile(hwnd);
        }
        else if (idResponse == IDCANCEL)    // Cancel entire operation
            return FALSE;
    }

    return TRUE;                            // Proceed
}


//
//  FUNCTION: SetNewBuffer(HWND, HANDLE, LPSTR)
//
//  PURPOSE: Create a new edit buffer.
//
//  PARAMETERS:
//    hwnd    - Handle to the main window.
//    hsz     - Local handle to the text (may be NULL).
//    szTitle - Title to display for the user.
//
//  RETURN VALUE:
//    None.
//
//  COMMENTS:
//

void SetNewBuffer(HANDLE hsz)
{
    if (!hsz)                    // Allocates a buffer if none exists
        hsz = LocalAlloc(LMEM_MOVEABLE | LMEM_ZEROINIT, 1);

    // Updates the buffer and displays new buffer
    SetEditText(GetEditWindow(NULL), hsz);

    SendMessage(GetEditWindow(NULL), EM_SETMODIFY, FALSE, 0L);
}


//
//  FUNCTION: SaveFile(HWND)
//
//  PURPOSE: Save the text from the editor control to the current file.
//
//  PARAMETERS:
//    hwnd - The main application window.
//
//  RETURN VALUE:
//    TRUE - Sucessfully saved the file.
//    FALSE - Unable to save the file.
//
//  COMMENTS:
//

BOOL SaveFile(HWND hwnd)
{
    BOOL  fSuccess;
    int   IOStatus;  // result of a file write
    char  rgch[256];
    HFILE hfile;
    OFSTRUCT ofs;
    HCURSOR hcursSave;
    LPVOID lpvEdit;

    hfile = OpenFile(GetFName(),  &ofs, OF_PROMPT | OF_CANCEL | OF_CREATE);
    if (hfile == HFILE_ERROR)
    {
        // If the file can't be saved

        wsprintf(rgch, "Cannot write to %s.", GetFName());
        MessageBox(hwnd, rgch, SZAPPNAME, MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
    }

    lpvEdit = LockEditText(GetEditWindow(NULL));

    // Set the cursor to an hourglass during the file transfer

    hcursSave = SetCursor(hcursHourGlass);
    IOStatus = _lwrite(hfile, lpvEdit, lstrlen(lpvEdit));
    _lclose(hfile);
    SetCursor(hcursSave);

    if (IOStatus != (int)lstrlen(lpvEdit))
    {
        wsprintf(rgch, "Error writing to %s.", GetFName());
        MessageBox(hwnd, rgch, SZAPPNAME, MB_OK | MB_ICONHAND);
        fSuccess = FALSE;
    }
    else
    {
        fSuccess = TRUE;    // Indicates the file was saved
        SendMessage(GetEditWindow(NULL), EM_SETMODIFY, FALSE, 0L);
    }

    UnlockEditText(GetEditWindow(NULL));
    return fSuccess;
}


//
//  FUNCTION: SaveAs(LPSTR, LPSTR, HWND)
//
//  PURPOSE: Save the edit text to the specified file.
//
//  PARAMETERS:
//    lsz             - The fully qualified path of the file to save.
//    lszCurrentName  - The current name of the file; used to restore 
//                      the caption if saving fails
//    hwnd            - The main application window.
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//
//

VOID SaveAs(LPSTR lsz, LPSTR lszCurrentName, HWND hwnd)
{
    if (!SaveFile(hwnd))
        SetWindowText(hwnd, lszCurrentName);
    else
        SetProp(GetActiveMDIChild(), "FilenameSet", (HANDLE)1);
}
