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
//  app.c
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

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <commdlg.h>
#include <shellapi.h>
#include <stdarg.h>
#include <memory.h>
#include "appport.h"
#include "app.h"
#include "mciapp.h"

#include "debug.h"


//
//  globals, no less
//
HINSTANCE       ghinst;
UINT            gfuAppOptions       = APP_OPTF_YIELDEXEC;

TCHAR           gszAppSection[]     = TEXT("MCI App");
TCHAR           gszNull[]           = TEXT("");

TCHAR           gszAppName[APP_MAX_APP_NAME_CHARS];
TCHAR           gszFileUntitled[APP_MAX_FILE_TITLE_CHARS];
TCHAR           gszAppFileTitle[APP_MAX_FILE_TITLE_CHARS];
TCHAR           gszAppFilePath[APP_MAX_FILE_PATH_CHARS];


//==========================================================================;
//
//  Application helper functions
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  int AppMsgBox
//
//  Description:
//      This function displays a message for the application in a standard
//      message box.
//
//      Note that this function takes any valid argument list that can
//      be passed to wsprintf. Because of this, the application must
//      remember to cast near string pointers to FAR when built for Win 16.
//      You will get a nice GP fault if you do not cast them correctly.
//
//  Arguments:
//      HWND hwnd: Handle to parent window for message box holding the
//      message.
//
//      UINT fuStyle: Style flags for MessageBox().
//
//      PCTSTR pszFormat: Format string used for wvsprintf().
//
//  Return (int):
//      The return value is the result of MessageBox() function.
//
//  History:
//       2/13/93    created.
//
//--------------------------------------------------------------------------;

int FNCGLOBAL AppMsgBox
(
    HWND            hwnd,
    UINT            fuStyle,
    PCTSTR          pszFormat,
    ...
)
{
    va_list     va;
    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];
    int         n;

    //
    //  format and display the message..
    //
    va_start(va, pszFormat);
    wvsprintf(ach, pszFormat, (LPSTR)va);
    va_end(va);

    n = MessageBox(hwnd, ach, gszAppName, fuStyle);

    return (n);
} // AppMsgBox()


//--------------------------------------------------------------------------;
//
//  int AppMsgBoxId
//
//  Description:
//      This function displays a message for the application. The message
//      text is retrieved from the string resource table using LoadString.
//
//      Note that this function takes any valid argument list that can
//      be passed to wsprintf. Because of this, the application must
//      remember to cast near string pointers to FAR when built for Win 16.
//      You will get a nice GP fault if you do not cast them correctly.
//
//  Arguments:
//      HWND hwnd: Handle to parent window for message box holding the
//      message.
//
//      UINT fuStyle: Style flags for MessageBox().
//
//      UINT uIdsFormat: String resource id to be loaded with LoadString()
//      and used a the format string for wvsprintf().
//
//  Return (int):
//      The return value is the result of MessageBox() if the string
//      resource specified by uIdsFormat is valid. The return value is zero
//      if the string resource failed to load.
//
//  History:
//       2/13/93    created.
//
//--------------------------------------------------------------------------;

int FNCGLOBAL AppMsgBoxId
(
    HWND            hwnd,
    UINT            fuStyle,
    UINT            uIdsFormat,
    ...
)
{
    va_list     va;
    TCHAR       szFormat[APP_MAX_STRING_RC_CHARS];
    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];
    int         n;

    n = LoadString(ghinst, uIdsFormat, szFormat, SIZEOF(szFormat));
    if (0 != n)
    {
        //
        //  format and display the message..
        //
        va_start(va, uIdsFormat);
        wvsprintf(ach, szFormat, (LPSTR)va);
        va_end(va);

        n = MessageBox(hwnd, ach, gszAppName, fuStyle);
    }

    return (n);
} // AppMsgBoxId()


//--------------------------------------------------------------------------;
//
//  void AppHourGlass
//
//  Description:
//      This function changes the cursor to that of the hour glass or
//      back to the previous cursor.
//
//      This function can be called recursively.
//
//  Arguments:
//      BOOL fHourGlass: TRUE if we need the hour glass.  FALSE if we need
//      the arrow back.
//
//  Return (void):
//      On return, the cursor will be what was requested.
//
//  History:
//      11/ 8/92    created.
//
//--------------------------------------------------------------------------;

void FNGLOBAL AppHourGlass
(
    BOOL            fHourGlass
)
{
    static HCURSOR  hcur;
    static UINT     uWaiting = 0;

    if (fHourGlass)
    {
        if (!uWaiting)
        {
            hcur = SetCursor(LoadCursor(NULL, IDC_WAIT));
            ShowCursor(TRUE);
        }

        uWaiting++;
    }
    else
    {
        --uWaiting;

        if (!uWaiting)
        {
            ShowCursor(FALSE);
            SetCursor(hcur);
        }
    }
} // AppHourGlass()


//--------------------------------------------------------------------------;
//
//  BOOL AppYield
//
//  Description:
//      This function yields by dispatching all messages stacked up in the
//      application queue.
//
//  Arguments:
//      HWND hwnd: Handle to main window of application if not yielding
//      for a dialog. Handle to dialog box if yielding for a dialog box.
//
//      BOOL fIsDialog: TRUE if being called to yield for a dialog box.
//
//  Return (BOOL):
//      The return value is always TRUE.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL AppYield
(
    HWND            hwnd,
    BOOL            fIsDialog
)
{
    MSG     msg;

    if (fIsDialog)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if ((NULL == hwnd) || !IsDialogMessage(hwnd, &msg))
            {
                //
                //  see comment below..
                //
                MciAppDispatchMessage(GetParent(hwnd), &msg);
            }
        }
    }
    else
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            //
            //  normally, we would only do the following here:
            //
            //      TranslateMessage(&msg);
            //      DispatchMessage(&msg);
            //
            //  but this app is special, so dispatch messages in a
            //  special way...
            //
            MciAppDispatchMessage(hwnd, &msg);
        }
    }

    return (TRUE);
} // AppYield()


//--------------------------------------------------------------------------;
//
//  int AppSetWindowText
//
//  Description:
//      This function formats a string and sets the specified window text
//      to the result.
//
//  Arguments:
//      HWND hwnd: Handle to window to receive the new text.
//
//      PCTSTR pszFormat: Pointer to any valid format for wsprintf.
//
//  Return (int):
//      The return value is the number of bytes that the resulting window
//      text was.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

int FNCGLOBAL AppSetWindowText
(
    HWND            hwnd,
    PCTSTR          pszFormat,
    ...
)
{
    va_list     va;
    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];
    int         n;

    //
    //  format and display the string in the window...
    //
    va_start(va, pszFormat);
    n = wvsprintf(ach, pszFormat, (LPSTR)va);
    va_end(va);

    SetWindowText(hwnd, ach);

    return (n);
} // AppSetWindowText()


//--------------------------------------------------------------------------;
//
//  int AppSetWindowTextId
//
//  Description:
//      This function formats a string and sets the specified window text
//      to the result. The format string is extracted from the string
//      table using LoadString() on the uIdsFormat argument.
//
//  Arguments:
//      HWND hwnd: Handle to window to receive the new text.
//
//      UINT uIdsFormat: String resource id to be loaded with LoadString()
//      and used a the format string for wvsprintf().
//
//  Return (int):
//      The return value is the number of bytes that the resulting window
//      text was. This value is zero if the LoadString() function fails
//      for the uIdsFormat argument.
//
//  History:
//       2/ 7/93    created.
//
//--------------------------------------------------------------------------;

int FNCGLOBAL AppSetWindowTextId
(
    HWND            hwnd,
    UINT            uIdsFormat,
    ...
)
{
    va_list     va;
    TCHAR       szFormat[APP_MAX_STRING_RC_CHARS];
    TCHAR       ach[APP_MAX_STRING_ERROR_CHARS];
    int         n;

    n = LoadString(ghinst, uIdsFormat, szFormat, SIZEOF(szFormat));
    if (0 != n)
    {
        //
        //  format and display the string in the window...
        //
        va_start(va, uIdsFormat);
        n = wvsprintf(ach, szFormat, (LPSTR)va);
        va_end(va);

        SetWindowText(hwnd, ach);
    }

    return (n);
} // AppSetWindowTextId()


//--------------------------------------------------------------------------;
//
//  BOOL AppGetFileTitle
//
//  Description:
//      This function extracts the file title from a file path and returns
//      it in the caller's specified buffer.
//
//  Arguments:
//      PCTSTR pszFilePath: Pointer to null terminated file path.
//
//      PTSTR pszFileTitle: Pointer to buffer to receive the file title.
//
//  Return (BOOL):
//      Always returns TRUE. But should return FALSE if this function
//      checked for bogus values, etc.
//
//  History:
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL AppGetFileTitle
(
    PCTSTR          pszFilePath,
    PTSTR           pszFileTitle
)
{
    #define IS_SLASH(c)     ('/' == (c) || '\\' == (c))

    PTSTR       pch;

    //
    //  scan to the end of the file path string..
    //
    for (pch = pszFilePath; '\0' != *pch; pch++)
        ;

    //
    //  now scan back toward the beginning of the string until a slash (\),
    //  colon, or start of the string is encountered.
    //
    while ((pch >= pszFilePath) && !IS_SLASH(*pch) && (':' != *pch))
    {
        pch--;
    }

    //
    //  finally, copy the 'title' into the destination buffer.. skip ahead
    //  one char since the above loop steps back one too many chars...
    //
    lstrcpy(pszFileTitle, ++pch);

    return (TRUE);
} // AppGetFileTitle()


//--------------------------------------------------------------------------;
//
//  BOOL AppGetFileName
//
//  Description:
//      This function is a wrapper for the Get[Open/Save]FileName commdlg
//      chooser dialogs. Based on the fuFlags argument, this function will
//      display the appropriate chooser dialog and return the result.
//
//  Arguments:
//      HWND hwnd: Handle to parent window for chooser dialog.
//
//      PTSTR pszFilePath: Pointer to buffer to receive the file path.
//
//      PTSTR pszFileTitle: Pointer to buffer to receive the file title.
//      This argument may be NULL, in which case no title will be returned.
//
//      UINT fuFlags:
//
//  Return (BOOL):
//      The return value is TRUE if a file was chosen. It is FALSE if the
//      user canceled the operation.
//
//  History:
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL AppGetFileName
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle,
    UINT            fuFlags
)
{
    #define APP_OFN_FLAGS_SAVE  (OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT)
    #define APP_OFN_FLAGS_OPEN  (OFN_HIDEREADONLY | OFN_FILEMUSTEXIST)

    TCHAR           szExtDefault[APP_MAX_EXT_DEFAULT_CHARS];
    TCHAR           szExtFilter[APP_MAX_EXT_FILTER_CHARS];
    OPENFILENAME    ofn;
    BOOL            f;
    PTCHAR          pch;


    //
    //  get the extension filter and default extension for this application
    //
    LoadString(ghinst, IDS_OFN_EXT_DEF, szExtDefault, SIZEOF(szExtDefault));
    LoadString(ghinst, IDS_OFN_EXT_FILTER, szExtFilter, SIZEOF(szExtFilter));


    //
    //  NOTE! building the filter string for the OPENFILENAME structure
    //  is a bit more difficult when dealing with Unicode and C8's new
    //  optimizer. it joyfully removes literal '\0' characters from
    //  strings that are concatted together. if you try making each
    //  string separate (array of pointers to strings), the compiler
    //  will dword align them... etc, etc.
    //
    //  if you can think of a better way to build the silly filter string
    //  for common dialogs and still work in Win 16 and Win 32 [Unicode]
    //  i'd sure like to hear about it...
    //
    for (pch = &szExtFilter[0]; '\0' != *pch; pch++)
    {
        if ('!' == *pch)
            *pch = '\0';
    }

    //
    //  initialize the OPENFILENAME members
    //
    memset(&ofn, 0, sizeof(OPENFILENAME));

    pszFilePath[0]          = '\0';
    if (pszFileTitle)
        pszFileTitle[0]     = '\0';

    ofn.lStructSize         = sizeof(OPENFILENAME);
    ofn.hwndOwner           = hwnd;
    ofn.lpstrFilter         = szExtFilter;
    ofn.lpstrCustomFilter   = NULL;
    ofn.nMaxCustFilter      = 0L;
    ofn.nFilterIndex        = 1L;
    ofn.lpstrFile           = pszFilePath;
    ofn.nMaxFile            = APP_MAX_FILE_PATH_CHARS;
    ofn.lpstrFileTitle      = pszFileTitle;
    ofn.nMaxFileTitle       = pszFileTitle ? APP_MAX_FILE_TITLE_CHARS : 0;
    ofn.lpstrInitialDir     = NULL;
    ofn.nFileOffset         = 0;
    ofn.nFileExtension      = 0;
    ofn.lpstrDefExt         = szExtDefault;

    //
    //  if the fuFlags.APP_GFNF_SAVE bit is set, then call GetSaveFileName()
    //  otherwise call GetOpenFileName(). why commdlg was designed with
    //  two separate functions for save and open only clark knows.
    //
    if (fuFlags & APP_GFNF_SAVE)
    {
        ofn.Flags = APP_OFN_FLAGS_SAVE;
        f = GetSaveFileName(&ofn);
    }
    else
    {
        ofn.Flags = APP_OFN_FLAGS_OPEN;
        f = GetOpenFileName(&ofn);
    }

    return (f);
} // AppGetFileName()


//--------------------------------------------------------------------------;
//
//  BOOL AppTitle
//
//  Description:
//      This function formats and sets the title text of the application's
//      window.
//
//  Arguments:
//      HWND hwnd: Handle to application window to set title text for.
//
//      PCTSTR pszFileTitle: Pointer to file title to display.
//
//  Return (BOOL):
//      The return value is always TRUE.
//
//  History:
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL AppTitle
(
    HWND            hwnd,
    PCTSTR          pszFileTitle
)
{
    static  TCHAR   szFormatTitle[]     = TEXT("%s - %s");

    TCHAR       ach[APP_MAX_FILE_PATH_CHARS];

    //
    //  format the title text as 'AppName - FileTitle'
    //
    wsprintf(ach, szFormatTitle, (LPSTR)gszAppName, (LPSTR)pszFileTitle);
    SetWindowText(hwnd, ach);

    return (TRUE);
} // AppTitle()


//--------------------------------------------------------------------------;
//
//  BOOL AppFileNew
//
//  Description:
//      This function is called to handle the IDM_FILE_NEW message. It is
//      responsible for clearing the working area for a new unnamed file.
//
//  Arguments:
//      HWND hwnd: Handle to application window.
//
//      PTSTR pszFilePath: Pointer to current null terminated file path.
//      This buffer will be reinitialized if the function succeeds.
//
//      PTSTR pszFileTitle: Pointer to current null terminated file title.
//      This buffer will be reinitialized if the function succeeds.
//
//  Return (BOOL):
//      The return value is TRUE if the working area was cleared and is
//      ready for new stuff. The return value is FALSE if the user canceled
//      the operation.
//
//  History:
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL AppFileNew
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
)
{
    BOOL    f;

    //
    //  if there is currently a file path, then we have to do some real
    //  work...
    //
    if ('\0' != pszFilePath[0])
    {
        f = MciAppFileNew(hwnd, pszFilePath, pszFileTitle);
        if (!f)
            return (FALSE);
    }

    //
    //  blow away the old file path and title; set the window title and
    //  return success
    //
    lstrcpy(pszFilePath,  gszFileUntitled);
    lstrcpy(pszFileTitle, gszFileUntitled);

    AppTitle(hwnd, pszFileTitle);

    MciAppResetStatus(hwnd);
    AppSetWindowTextId(GetDlgItem(hwnd, IDD_APP_TEXT_STATUS),
                       IDS_MCI_SCRIPT_CREATED, (LPSTR)pszFilePath);

    SetFocus(GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT));

    return (TRUE);
} // AppFileNew()


//--------------------------------------------------------------------------;
//
//  BOOL AppFileOpen
//
//  Description:
//      This function handles the IDM_FILE_OPEN message. It is responsible
//      for getting a new file name from the user and opening that file
//      if possible.
//
//  Arguments:
//      HWND hwnd: Handle to application window.
//
//      PTSTR pszFilePath: Pointer to current null terminated file path.
//      This buffer will contain the new file path if one is selected.
//
//      PTSTR pszFileTitle: Pointer to current null terminated file title.
//      This buffer will contain the new file title if one is selected.
//
//  Return (BOOL):
//      The return value is TRUE if a new file was selected and opened.
//      It is FALSE if the user canceled the operation.
//
//  History:
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL AppFileOpen
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle
)
{
    TCHAR       szFilePath[APP_MAX_FILE_PATH_CHARS];
    TCHAR       szFileTitle[APP_MAX_FILE_TITLE_CHARS];
    BOOL        f;

    //
    //  first test for a modified script that has not been saved. if the
    //  return value is FALSE we should cancel the File.Open operation.
    //
    f = MciAppFileSaveModified(hwnd, pszFilePath, pszFileTitle);
    if (!f)
        return (FALSE);


    //
    //  get the file name of the new script into temporary buffers (so
    //  if we fail to open it we can back out cleanly).
    //
    f = AppGetFileName(hwnd, szFilePath, szFileTitle, APP_GFNF_OPEN);
    if (!f)
        return (FALSE);


    //
    //  read the new script...
    //
    f = MciAppFileOpen(hwnd, szFilePath, NULL);
    if (f)
    {
        //
        //  copy the new file path and title into the global buffers and
        //  set the window title text...
        //
        lstrcpy(gszAppFilePath,  szFilePath);
        lstrcpy(gszAppFileTitle, szFileTitle);

        AppTitle(hwnd, szFileTitle);
        MciAppResetStatus(hwnd);

        AppSetWindowTextId(GetDlgItem(hwnd, IDD_APP_TEXT_STATUS),
                           IDS_MCI_SCRIPT_OPENED, (LPSTR)szFilePath);

        SetFocus(GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT));
    }

    return (f);
} // AppFileOpen()


//--------------------------------------------------------------------------;
//
//  BOOL AppFileSave
//
//  Description:
//      This function handles the IDM_FILE_SAVE[AS] messages. It is
//      responsible for saving the current file. If a file name needs
//      to be specified then the save file dialog is displayed.
//
//  Arguments:
//      HWND hwnd: Handle to application window.
//
//      PTSTR pszFilePath: Pointer to current null terminated file path.
//      This buffer will contain the new file path if one is selected.
//
//      PTSTR pszFileTitle: Pointer to current null terminated file title.
//      This buffer will contain the new file title if one is selected.
//
//      BOOL fSaveAs: TRUE if the save file chooser should be displayed
//      before saving the file. FALSE if should operate like File.Save.
//
//  Return (BOOL):
//      The return value is TRUE if the file was saved. It is FALSE if the
//      user canceled the operation or the file does not need saved.
//
//  History:
//       2/ 6/93    created.
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL AppFileSave
(
    HWND            hwnd,
    PTSTR           pszFilePath,
    PTSTR           pszFileTitle,
    BOOL            fSaveAs
)
{
    TCHAR       szFilePath[APP_MAX_FILE_PATH_CHARS];
    TCHAR       szFileTitle[APP_MAX_FILE_TITLE_CHARS];
    BOOL        f;

    //
    //  check if we should bring up the save file chooser dialog...
    //
    if (fSaveAs || !lstrcmp(pszFileTitle, gszFileUntitled))
    {
        //
        //  get the file name for saving the script to into temporary
        //  buffers (so if we fail to save it we can back out cleanly).
        //
        f = AppGetFileName(hwnd, szFilePath, szFileTitle, APP_GFNF_SAVE);
        if (!f)
            return (FALSE);
    }
    else
    {
        //
        //  copy the file path into our temporary buffer so we don't have
        //  to special case this on MciAppFileSave..
        //
        lstrcpy(szFilePath,  pszFilePath);
        lstrcpy(szFileTitle, pszFileTitle);
    }

    //
    //  save the script...
    //
    f = MciAppFileSave(hwnd, szFilePath);
    if (f)
    {
        //
        //  copy the (possibly) new file path and title into the global
        //  buffers and set the window title text...
        //
        lstrcpy(gszAppFilePath,  szFilePath);
        lstrcpy(gszAppFileTitle, szFileTitle);

        AppTitle(hwnd, szFileTitle);

        //
        //  changes have been saved, so clear the modified bit...
        //
        Edit_SetModify(GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT), FALSE);

        AppSetWindowTextId(GetDlgItem(hwnd, IDD_APP_TEXT_STATUS),
                           IDS_MCI_SCRIPT_SAVED, (LPSTR)szFilePath);

        SetFocus(GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT));
    }

    return (f);
} // AppFileSave()


//==========================================================================;
//
//  Main application window handling code...
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  LRESULT AppInitMenuPopup
//
//  Description:
//      This function handles the WM_INITMENUPOPUP message. This message
//      is sent to the window owning the menu that is going to become
//      active. This gives an application the ability to modify the menu
//      before it is displayed (disable/add items, etc).
//
//  Arguments:
//      HWND hwnd: Handle to window that generated the WM_INITMENUPOPUP
//      message.
//
//      HMENU hmenu: Handle to the menu that is to become active.
//
//      int nItem: Specifies the zero-based relative position of the menu
//      item that invoked the popup menu.
//
//      BOOL fSysMenu: Specifies whether the popup menu is a System menu
//      (TRUE) or it is not a System menu (FALSE).
//
//  Return (LRESULT):
//      Returns zero if the message is processed.
//
//  History:
//       1/ 2/93    created.
//
//--------------------------------------------------------------------------;

LRESULT FNLOCAL AppInitMenuPopup
(
    HWND            hwnd,
    HMENU           hmenu,
    int             nItem,
    BOOL            fSysMenu
)
{
    BOOL    f;
    int     nSelStart;
    int     nSelEnd;
    HWND    hwndScript;

    DPF(0, "AppInitMenuPopup(hwnd=%Xh, hmenu=%Xh, nItem=%d, fSysMenu=%d)",
            hwnd, hmenu, nItem, fSysMenu);

    //
    //  if the system menu is what got hit, succeed immediately... this
    //  application has no stuff in the system menu.
    //
    if (fSysMenu)
        return (0L);

    //
    //  initialize the menu that is being 'popped up'
    //
    switch (nItem)
    {
        case APP_MENU_ITEM_FILE:
            //
            //  if the script has been modified, then enable the File.Save
            //  menu
            //
            hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);
            f = Edit_GetModify(hwndScript);
            EnableMenuItem(hmenu, IDM_FILE_SAVE,
                           (UINT)(f ? MF_ENABLED : MF_GRAYED));
            break;

        case APP_MENU_ITEM_EDIT:
            //
            //  check to see if something is selected in the script edit
            //  window and enable/disable Edit menu options appropriately
            //
            hwndScript = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);
            Edit_GetSelEx(hwndScript, &nSelStart, &nSelEnd);

            f = (nSelStart != nSelEnd);
            EnableMenuItem(hmenu, WM_CUT,   (UINT)(f ? MF_ENABLED : MF_GRAYED));
            EnableMenuItem(hmenu, WM_COPY,  (UINT)(f ? MF_ENABLED : MF_GRAYED));
            EnableMenuItem(hmenu, WM_CLEAR, (UINT)(f ? MF_ENABLED : MF_GRAYED));

            f = Edit_CanUndo(hwndScript);
            EnableMenuItem(hmenu, WM_UNDO,  (UINT)(f ? MF_ENABLED : MF_GRAYED));

            f = IsClipboardFormatAvailable(CF_TEXT);
            EnableMenuItem(hmenu, WM_PASTE, (UINT)(f ? MF_ENABLED : MF_GRAYED));
            break;

        case APP_MENU_ITEM_DEVICE:
            //
            //
            //
            f = (0 != (gfuAppOptions & APP_OPTF_DEVICELIST));
            CheckMenuItem(hmenu, IDM_DEVICE_OPENLIST,
                          (UINT)(f ? MF_CHECKED : MF_UNCHECKED));

            f = (MciAppGetNumDevices(hwnd) != 0);
            EnableMenuItem(hmenu, IDM_DEVICE_CLOSEALL,
                           (UINT)(f ? MF_ENABLED : MF_GRAYED));
            break;

        case APP_MENU_ITEM_OPTIONS:
            //
            //  make sure the options that need a checkmark are checked..
            //
            f = (0 != (gfuAppOptions & APP_OPTF_EDITONLY));
            CheckMenuItem(hmenu, IDM_OPTIONS_EDITONLY,
                          (UINT)(f ? MF_CHECKED : MF_UNCHECKED));

            f = (0 != (gfuAppOptions & APP_OPTF_YIELDEXEC));
            CheckMenuItem(hmenu, IDM_OPTIONS_YIELDEXEC,
                          (UINT)(f ? MF_CHECKED : MF_UNCHECKED));

            f = (0 != (gfuAppOptions & APP_OPTF_DEBUGLOG));
            CheckMenuItem(hmenu, IDM_OPTIONS_DEBUGLOG,
                          (UINT)(f ? MF_CHECKED : MF_UNCHECKED));
            break;
    }

    //
    //  we processed the message--return 0...
    //
    return (0L);
} // AppInitMenuPopup()


//--------------------------------------------------------------------------;
//
//  LRESULT AppCommand
//
//  Description:
//      This function handles the WM_COMMAND message.
//
//  Arguments:
//      HWND hwnd: Handle to window receiving the WM_COMMAND message.
//
//      int nId: Control or menu item identifier.
//
//      HWND hwndCtl: Handle of control if the message is from a control.
//      This argument is NULL if the message was not generated by a control.
//
//      UINT uCode: Notification code. This argument is 1 if the message
//      was generated by an accelerator. If the message is from a menu,
//      this argument is 0.
//
//  Return (LRESULT):
//      Returns zero if the message is processed.
//
//  History:
//      11/ 8/92    created.
//
//--------------------------------------------------------------------------;

LRESULT FNLOCAL AppCommand
(
    HWND            hwnd,
    int             nId,
    HWND            hwndCtl,
    UINT            uCode
)
{
    BOOL        f;

    if (gfExecuting)
    {
        if ((IDD_ACCL_ABORT == nId) || (IDD_APP_BTN_STOP == nId))
            gfAbortExec = TRUE;

        return (0L);
    }

    switch (nId)
    {
        case IDM_FILE_NEW:
            AppFileNew(hwnd, gszAppFilePath, gszAppFileTitle);
            break;

        case IDM_FILE_OPEN:
            AppFileOpen(hwnd, gszAppFilePath, gszAppFileTitle);
            break;

        case IDM_FILE_SAVE:
            AppFileSave(hwnd, gszAppFilePath, gszAppFileTitle, FALSE);
            break;

        case IDM_FILE_SAVEAS:
            AppFileSave(hwnd, gszAppFilePath, gszAppFileTitle, TRUE);
            break;

        case IDM_FILE_ABOUT:
            DialogBox(ghinst, DLG_ABOUT, hwnd, (DLGPROC)AboutDlgProc);
            break;

        case IDM_FILE_EXIT:
            FORWARD_WM_CLOSE(hwnd, SendMessage);
            break;


        case WM_UNDO:
        case WM_CUT:
        case WM_COPY:
        case WM_PASTE:
        case WM_CLEAR:
            //
            //  pass edit messages received to the script edit window
            //
            SendMessage(GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT), nId, 0, 0L);
            break;

        case IDM_EDIT_SELECTALL:
            Edit_SetSel(GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT), 0, -1);
            break;


        case IDM_DEVICE_CLOSEALL:
            MciAppCloseAllDevices(hwnd);
            break;


        case IDM_OPTIONS_EDITONLY:
            gfuAppOptions ^= APP_OPTF_EDITONLY;
            MciAppUpdateOptions(hwnd);
            break;

        case IDM_OPTIONS_YIELDEXEC:
            gfuAppOptions ^= APP_OPTF_YIELDEXEC;
            MciAppUpdateOptions(hwnd);
            break;

        case IDM_OPTIONS_DEBUGLOG:
            gfuAppOptions ^= APP_OPTF_DEBUGLOG;
            MciAppUpdateOptions(hwnd);
            break;

        case IDM_DEVICE_OPENLIST:
            //
            //  bring up the device list dialog--if this event was triggered
            //  by an accelerator (uCode == 1) then do not give the focus
            //  to the dialog. if this event was triggered by the user
            //  selecting the menu option with the mouse, then give the
            //  focus to the device list dialog...
            //
            gfuAppOptions ^= APP_OPTF_DEVICELIST;
            MciAppDeviceList(hwnd, (1 != uCode));
            break;

        case IDM_OPTIONS_FONT:
            MciAppChooseFont(hwnd);
            break;


        case IDD_APP_BTN_STEP:
            //
            //  execute the current line in the script window
            //
            MciAppSingleStep(hwnd);
            break;

        case IDD_APP_BTN_RUN:
            //
            //  execute every line in the script window starting from the
            //  _first_ line..
            //
            Edit_SetSel(GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT), 0, 0);

            // -- Fall Through --

        case IDD_APP_BTN_GO:
            //
            //  execute every line in the script window starting from the
            //  _current_ line..
            //
            f = (0 != (gfuAppOptions & APP_OPTF_YIELDEXEC));
            MciAppExecute(hwnd, f);
            break;


        case IDOK:
            //
            //  when we receive an IDOK for the script window (user pressed
            //  return key), 'enter' the new line...
            //
            //  uCode will be 0 if Alt+Enter was pressed, so reverse the
            //  logic of the Edit Only option in this case
            //
            if (GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT) == hwndCtl)
            {
                f = (0 != (gfuAppOptions & APP_OPTF_EDITONLY));
                MciAppEnterLine(hwnd, (1 == uCode) ? f : !f);
            }
            break;
    }

    return (0L);
} // AppCommand()


//--------------------------------------------------------------------------;
//
//  LRESULT AppDropFiles
//
//  Description:
//      This function handles the WM_DROPFILES message. This message is
//      sent when files are 'dropped' on the window from file manager
//      (or other drag/drop servers made by ISV's that figured out the
//      undocumented internal workings of the SHELL).
//
//      A window must be registered to receive these messages either by
//      called DragAcceptFiles() or using CreateWindowEx() with the
//      WS_EX_ACCEPTFILES style bit.
//
//  Arguments:
//      HWND hwnd: Handle to window receiving the message.
//
//      HDROP hdrop: Handle to drop structure.
//
//  Return (LRESULT):
//      Returns 0 if the message is processed.
//
//  History:
//       2/ 8/93    created.
//
//--------------------------------------------------------------------------;

LRESULT FNLOCAL AppDropFiles
(
    HWND            hwnd,
    HDROP           hdrop
)
{
    TCHAR       szFileTitle[APP_MAX_FILE_PATH_CHARS];
    TCHAR       szFilePath[APP_MAX_FILE_PATH_CHARS];
    UINT        uNumFiles;
    UINT        u;
    BOOL        f;
    int         n;

    //
    //  if executing a script and this message is received, we bail...
    //  too much code to fix this...
    //
    if (gfExecuting)
    {
        MessageBeep((UINT)-1);
        goto App_Drop_Files_Exit;
    }


    //
    //  first test for a modified script that has not been saved. if the
    //  return value is FALSE we should cancel the drop operation.
    //
    f = MciAppFileSaveModified(hwnd, gszAppFilePath, gszAppFileTitle);
    if (!f)
        goto App_Drop_Files_Exit;

    //
    //  get number of files dropped on our window
    //
    uNumFiles = DragQueryFile(hdrop, (UINT)-1, NULL, 0);

    DPF(0, "AppDropFiles(hwnd=%Xh, hdrop=%Xh)--uNumFiles=%u",
             hwnd, hdrop, uNumFiles);

    //
    //  step through each file and stop on the one the user wants or
    //  the last file (whichever comes first).
    //
    for (u = 0; u < uNumFiles; u++)
    {
        //
        //  get the next file name and try to open it--if not a valid
        //  file, then skip to the next one (if there is one).
        //
        DragQueryFile(hdrop, u, szFilePath, SIZEOF(szFilePath));


        //
        //  attempt to open the file
        //
        f = MciAppFileOpen(hwnd, szFilePath, szFileTitle);
        if (!f)
            continue;

        //
        //  update display info
        //
        lstrcpy(gszAppFilePath, szFilePath);
        lstrcpy(gszAppFileTitle, szFileTitle);

        AppTitle(hwnd, szFileTitle);
        MciAppResetStatus(hwnd);

        AppSetWindowTextId(GetDlgItem(hwnd, IDD_APP_TEXT_STATUS),
                           IDS_MCI_SCRIPT_OPENED, (LPSTR)szFilePath);


        //
        //  if this is NOT the last file in the list of files that are
        //  being dropped on us, then bring up a box asking if we should
        //  continue or stop where we are..
        //
        if ((uNumFiles - 1) != u)
        {
            n = AppMsgBoxId(hwnd, MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL,
                            IDS_DROP_CONTINUE);
            if (IDNO == n)
                break;
        }
    }

    //
    //  tell the shell to release the memory it allocated for beaming
    //  the file name(s) over to us... return 0 to show we processed
    //  the message.
    //
App_Drop_Files_Exit:

    DragFinish(hdrop);
    return (0L);
} // AppDropFiles()


//--------------------------------------------------------------------------;
//
//  LRESULT AppSize
//
//  Description:
//      This function handles the WM_SIZE message for the application's
//      window. This message is sent to the application window after the
//      size has changed (but before it is painted).
//
//  Arguments:
//      HWND hwnd: Handle to window that generated the WM_SIZE message.
//
//      UINT fuSizeType: Specifies the type of resizing requested. This
//      argument is one of the following: SIZE_MAXIMIZED, SIZE_MINIMIZED,
//      SIZE_RESTORED, SIZE_MAXHIDE, or SIZE_MAXSHOW.
//
//      int nWidth: Width of the new client area for the window.
//
//      int nHeight: Height of the new client area for the window.
//
//  Return (LRESULT):
//      Returns zero if the application processes the message.
//
//  History:
//       2/ 5/93    created.
//
//--------------------------------------------------------------------------;

LRESULT FNLOCAL AppSize
(
    HWND            hwnd,
    UINT            fuSizeType,
    int             nWidth,
    int             nHeight
)
{
    HWND    hwndButton;
    HWND    hwndOutput;
    HWND    hwndScript;
    HWND    hwndStatus;
    HWND    hwndSeparator;
    HWND    hwndNotify;
    HWND    hwndOptions;
    RECT    rc;
    int     nHeightButton;
    int     nWidthButton;
    int     nHeightScript;
    int     nHeightStatus;
    int     n, m;
    int     nWidthNotify;

    DPF(0, "AppSize(hwnd=%Xh, fuSizeType=%u, nWidth=%d, nHeight=%d)",
            hwnd, fuSizeType, nWidth, nHeight);

    //
    //  unless this application is the one being resized then don't waste
    //  time computing stuff that doesn't matter. this applies to being
    //  minimized also because this application does not have a custom
    //  minimized state.
    //
    if ((SIZE_RESTORED != fuSizeType) && (SIZE_MAXIMIZED != fuSizeType))
        return (0L);

    //
    //  for this application, we have three different sections that need
    //  to be fit into the newly sized client area--it looks roughly like
    //  this:
    //
    //  +-------------------------------------------+
    //  | - |        MCI App - ZYZSMAG.MCI      |^|v| <- title bar
    //  +-------------------------------------------+
    //  | File Edit Options                         | <- menu bar
    //  +-------------------------------------------+
    //  |{Step}{ Go }{ Run } Count:[x ] Output: zyz | <- button bar
    //  +-------------------------------------------+
    //  | MCI script window                         | <- script window
    //  |                                           |
    //  |                                           |
    //  |                                           |
    //  |                                           |
    //  |                                           |
    //  |                                           |
    //  +-------------------------------------------+
    //  | Silly status bar      | Notify: zyz | EYL | <- options status
    //  +-------------------------------------------+
    //       ^                  ^   ^
    //       +- status bar      |   +- notify window
    //                          |
    //                          +----- separator
    //
    //  the 'button bar' does not have to move or change size (except the
    //  width of the output window)
    //
    //  the 'script window' should fill all space between the bottom of
    //  the button bar and the top of the status bar.
    //
    //  the 'status bar' should sit on the bottom of the window and
    //  remain a constant height.
    //
    //  the 'notify window' should sit on the bottom of the window to the
    //  right of the status bar and remain a constant height.
    //
    //  the 'options status' window should sit in the bottom right corner
    //  and remain a constant width and height.
    //
    //  the 'separators' should be a visible separator between the status
    //  bar, notify window, and options status.
    //

    //
    //  first grab handles to all the windows we need to muck with...
    //
    hwndButton  = GetDlgItem(hwnd, IDD_APP_BTN_GO);
    hwndOutput  = GetDlgItem(hwnd, IDD_APP_TEXT_OUTPUT);
    hwndScript  = GetDlgItem(hwnd, IDD_APP_EDIT_SCRIPT);
    hwndStatus  = GetDlgItem(hwnd, IDD_APP_TEXT_STATUS);
    hwndNotify  = GetDlgItem(hwnd, IDD_APP_TEXT_NOTIFY);
    hwndOptions = GetDlgItem(hwnd, IDD_APP_TEXT_OPTIONS);

    //
    //  get the height of the button bar and status bar
    //
    GetClientRect(hwndButton, &rc);
    nHeightButton = (int)(rc.bottom - rc.top);
    nWidthButton  = (int)(rc.right - rc.left);


    //
    //  change width of output window..
    //
    GetClientRect(hwndStatus, &rc);
    nHeightStatus = (int)(rc.bottom - rc.top);

    n = nWidthButton * 6;
    m = (nWidth > (n + 2)) ? (nWidth - n - 2) : 0;
    rc.top = (nHeightButton - nHeightStatus) / 2;
    MoveWindow(hwndOutput, n, (int)rc.top, m, nHeightStatus, TRUE);


    //
    //  calculate the new height for the script window and move it to just
    //  below the bottom of the button bar and above the status bar (which
    //  is the height of the button bar)... note that we put the left and
    //  right edges of the script window outside of the client area because
    //  it looks better that way.
    //
    nHeightScript = nHeight - (nHeightButton * 2);
    if (nHeightScript < 0)
        nHeightScript = 0;

    MoveWindow(hwndScript, -1, nHeightButton, nWidth + 2, nHeightScript, TRUE);


    //
    //  put the status bar sitting on top of the bottom of the app window
    //  note that we center it in a space that is as high as the button
    //  bar so it looks better... it is assumed (correctly) that the button
    //  bar is taller than the status bar control.
    //
    //  ~                                           ~
    //  |                                           |
    //  +-------------------------------------------+
    //  | Silly status bar      | Notify: zyz | EYL |
    //  +-------------------------------------------+
    //  ^                       ^             ^     ^
    //  +------ min = 20 -------+-min = ~130 -+- 40-+
    //
    //  there is pad of 2 on the left and right of each text window (giving
    //  a separator a effective width of 5).
    //
    //
    nWidthNotify = (nWidth < (20 + 5 + 130 + 5 + 40)) ?
                        (nWidth - (20 + 5 + 5 + 40)) : 130;

    hwndSeparator = GetDlgItem(hwnd, IDD_APP_BOX_SEPARATOR1);
    MoveWindow(hwndSeparator, nWidth - nWidthNotify - 3 - 5 - 40,
                              nHeight - nHeightButton,
                              1, nHeightButton, TRUE);


    n = nHeightStatus + ((nHeightButton - nHeightStatus) / 2);
    MoveWindow(hwndStatus, 2, nHeight - n,
                           nWidth - nWidthNotify - 8 - 5 - 40,
                           nHeightStatus, TRUE);

    MoveWindow(hwndNotify, nWidth - nWidthNotify - 5 - 40, nHeight - n,
                           nWidthNotify - 2, nHeightStatus, TRUE);

    hwndSeparator = GetDlgItem(hwnd, IDD_APP_BOX_SEPARATOR2);
    MoveWindow(hwndSeparator, nWidth - 3 - 40,
                              nHeight - nHeightButton,
                              1, nHeightButton, TRUE);

    MoveWindow(hwndOptions, nWidth - 40, nHeight - n,
                           40 - 2, nHeightStatus, TRUE);

    //
    //  we processed the message..
    //
    return (0L);
} // AppSize()


//--------------------------------------------------------------------------;
//
//  LRESULT AppWndProc
//
//  Description:
//      This is the main application window procedure.
//
//  Arguments:
//      HWND hwnd: Handle to window.
//
//      UINT uMsg: Message being sent to the window.
//
//      WPARAM wParam: Specific argument to message.
//
//      LPARAM lParam: Specific argument to message.
//
//  Return (LRESULT):
//      The return value depends on the message that is being processed.
//
//  History:
//      11/ 8/92    created.
//
//--------------------------------------------------------------------------;

LRESULT FNEXPORT AppWndProc
(
    HWND            hwnd,
    UINT            uMsg,
    WPARAM          wParam,
    LPARAM          lParam
)
{
    LRESULT     lr;

    switch (uMsg)
    {
        case WM_CREATE:
            lr = HANDLE_WM_CREATE(hwnd, wParam, lParam, AppCreate);
            return (lr);

        case WM_WININICHANGE:
            HANDLE_WM_WININICHANGE(hwnd, wParam, lParam, AppWinIniChange);
            return (0L);

        case WM_INITMENUPOPUP:
            HANDLE_WM_INITMENUPOPUP(hwnd, wParam, lParam, AppInitMenuPopup);
            return (0L);

        case WM_COMMAND:
            lr = HANDLE_WM_COMMAND(hwnd, wParam, lParam, AppCommand);
            return (lr);

        case WM_DROPFILES:
            //
            //  some windowsx.h files have a screwed up message cracker for
            //  WM_DROPFILES. because this is a sample app, i don't want
            //  people having trouble with bogus windowsx.h files, so crack
            //  the message manually... you should use the message cracker
            //  if you know your windowsx.h file is good.
            //
            //  lr = HANDLE_WM_DROPFILES(hwnd, wParam, lParam, AppDropFiles);
            //
            lr = AppDropFiles(hwnd, (HDROP)wParam);
            return (lr);

        case WM_SIZE:
            //
            //  handle what we want for sizing, and then always call the
            //  default handler...
            //
            HANDLE_WM_SIZE(hwnd, wParam, lParam, AppSize);
            break;

        case WM_QUERYENDSESSION:
            lr = HANDLE_WM_QUERYENDSESSION(hwnd, wParam, lParam, AppQueryEndSession);
            return (lr);

        case WM_ENDSESSION:
            HANDLE_WM_ENDSESSION(hwnd, wParam, lParam, AppEndSession);
            return (0L);

        case WM_CLOSE:
            HANDLE_WM_CLOSE(hwnd, wParam, lParam, AppClose);
            return (0L);

        case WM_DESTROY:
            PostQuitMessage(0);
            return (0L);


        case MM_MCINOTIFY:
            MciAppHandleNotify(hwnd, wParam, LOWORD(lParam));
            break;
    }

    return (DefWindowProc(hwnd, uMsg, wParam, lParam));
} // AppWndProc()


//==========================================================================;
//
//  Main entry and message dispatching code
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  int WinMain
//
//  Description:
//      This function is called by the system as the initial entry point
//      for a Windows application.
//
//  Arguments:
//      HINSTANCE hinst: Identifies the current instance of the
//      application.
//
//      HINSTANCE hinstPrev: Identifies the previous instance of the
//      application (NULL if first instance). For Win 32, this argument
//      is _always_ NULL.
//
//      LPSTR pszCmdLine: Points to null-terminated unparsed command line.
//      This string is strictly ANSI regardless of whether the application
//      is built for Unicode. To get the Unicode equivalent call the
//      GetCommandLine() function (Win 32 only).
//
//      int nCmdShow: How the main window for the application is to be
//      shown by default.
//
//  Return (int):
//      Returns result from WM_QUIT message (in wParam of MSG structure) if
//      the application is able to enter its message loop. Returns 0 if
//      the application is not able to enter its message loop.
//
//  History:
//      11/ 8/92    created.
//
//--------------------------------------------------------------------------;

int PASCAL WinMain
(
    HINSTANCE       hinst,
    HINSTANCE       hinstPrev,
    LPSTR           pszCmdLine,
    int             nCmdShow
)
{
    int     nResult;
    HWND    hwnd;
    MSG     msg;

    //
    //  our documentation states that WinMain is supposed to return 0 if
    //  we do not enter our message loop--so assume the worst...
    //
    nResult = 0;

    //
    //  make our instance handle global for convenience..
    //
    ghinst = hinst;

    //
    //  init some stuff, create window, etc.. note the explicit cast of
    //  pszCmdLine--this is to mute a warning (and an ugly ifdef) when
    //  compiling for Unicode. see AppInit() for more details.
    //
    hwnd = AppInit(hinst, hinstPrev, (LPTSTR)pszCmdLine, nCmdShow);
    if (hwnd)
    {
        //
        //  dispatch messages
        //
        while (GetMessage(&msg, NULL, 0, 0))
        {
            //
            //  do all the special stuff required for this application
            //  when dispatching messages..
            //
            MciAppDispatchMessage(hwnd, &msg);
        }

        //
        //  return result of WM_QUIT message.
        //
        nResult = (int)msg.wParam;
    }

    //
    //  shut things down, clean up, etc.
    //
    nResult = AppExit(hinst, nResult);

    return (nResult);
} // WinMain()

