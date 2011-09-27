
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*******************************************************************************
 *                                                                             *
 *  MODULE      : DLGOPEN.C                                                    *
 *                                                                             *
 *  DESCRIPTION : Routines to display a standard File/Open and File/Save       *
 *                dialog boxes.                                                *
 *                                                                             *
 *  FUNCTIONS   : DlgOpenFile() - Displays a dialog box for opening or saving a*
 *                                file.                                        *
 *                                                                             *
 *                DlgfnOpen()   - Dialog function for the above dialog.        *
 *                                                                             *
 *                AddExt()      - Adds an extension to a filename if not       *
 *                                already present.                             *
 *                                                                             *
 *                FSearchSpec() - Checks if given string contains a wildcard   *
 *                                character.                                   *
 *                                                                             *
 *                FillListBox() - Fills listbox with files that match specs.   *
 *                                                                             *
 *                DlgCheckOkEnable() - Enables <OK> button iff there's text in *
 *                                     the edit control.                       *
 *                                                                             *
 *                NOTE : These routines require that the app. be running       *
 *                       SS = DS since they use near pointers into the stack.  *
 *                                                                             *
 *******************************************************************************/
#include <windows.h>
#include <string.h>
#include "showdib.h"

static PSTR         szExt;
static PSTR         szFileName;
static PSTR         szTitle;
static DWORD        flags;
static WORD         fOpt;

/* Forward declarations of helper functions */

static VOID  NEAR DlgCheckOkEnable(HWND hwnd, INT idEdit, UINT message);
static CHAR *NEAR FillListBox (HWND,CHAR*, UINT);
static BOOL  NEAR FSearchSpec (CHAR*);
static VOID  NEAR AddExt (CHAR*,CHAR*);

#define DLGOPEN_UNUSED   0

/* Mask to eliminate bogus style and bitcount combinations ...
 * RLE formats, if chosen should be matched with the bitcounts:
 *   RLE4 scheme should be used only for 4 bitcount DIBs.
 *   RLE8   "      "     "   "   "    "  8   "       "
 *
 * BITCOUNTMASK is indexed by DLGOPEN_RLE4 >> 4, DLGOPEN_RLE8 >> 4
 * and DLGOPEN_RLE8 >> 4
 */

static WORD BITCOUNTMASK[] = { DLGOPEN_UNUSED,
                               DLGOPEN_1BPP | DLGOPEN_8BPP | DLGOPEN_24BPP,
                               DLGOPEN_1BPP | DLGOPEN_4BPP | DLGOPEN_24BPP,
                               DLGOPEN_UNUSED,
                               0 };


/*******************************************************************************
 *                                                                             *
 *  FUNCTION   :DlgOpen(LPSTR szFile)                                          *
 *                                                                             *
 *  PURPOSE    :Display dialog box for opening files. Allow user to interact   *
 *              with dialogbox, change directories as necessary, and try to    *
 *              open file if user selects one. Automatically append            *
 *              extension to filename if necessary.                            *
 *              This routine correctly parses filenames containing KANJI       *
 *              characters.                                                    *
 *                                                                             *
 *  RETURNS    :  - Handle to the opened file if legal filename.               *
 *                - 0 if user presses <cancel>                                 *
 *                - 1 if filename entered is illegal                           *
 *                                                                             *
 *******************************************************************************/
HFILE APIENTRY DlgOpenFile (
    HWND          hwndParent,
    CHAR          *szTitleIn,
    DWORD         flagsIn,
    CHAR          *szExtIn,
    CHAR          *szFileNameIn,
    WORD          *pfOpt)
{
    INT      fh;
    FARPROC  lpProc;
    CHAR     achFile[128];
    CHAR     achExt[128];
    HANDLE   hInstance;
    WORD     w;

    if (pfOpt == NULL)
        pfOpt = &w;

    flags    = flagsIn;
    fOpt     = *pfOpt;

    lstrcpy (szFileName = achFile, szFileNameIn);
    lstrcpy (szExt = achExt, szExtIn);
    szTitle = szTitleIn;

    hInstance = (HANDLE)GetWindowLong (hwndParent, GWL_HINSTANCE);

    /* Show the dialog box */
    lpProc = MakeProcInstance ((FARPROC)DlgfnOpen, hInstance);
    fh = DialogBox (hInstance, "DlgOpenBox", hwndParent, (DLGPROC)lpProc);
    FreeProcInstance (lpProc);

    if (fh != 0){
        lstrcpy (szFileNameIn, szFileName);
        *pfOpt = fOpt;
    }
    return fh;
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   :DlgfnOpen (hwnd, msg, wParam, lParam)                       *
 *                                                                          *
 *  PURPOSE    :Dialog function for File/Open dialog.                       *
 *                                                                          *
 ****************************************************************************/
LONG APIENTRY DlgfnOpen (
    HWND hwnd,
    UINT msg,
    UINT wParam,
    LONG lParam)
{
    INT      result = -1;    /* Assume illegal filename initially */
    int     w;
    CHAR     c;
    WORD     f;
    OFSTRUCT of;
    RECT     rc, rcCtl;
    HWND     hwndT;
    BOOL     fEnable;

    switch (msg) {
        case WM_INITDIALOG:
            if (szTitle && *szTitle)
                SetWindowText (hwnd, szTitle);

            /* Set text on <OK> button according to mode (File/Open or File/Save) */
            if (flags & OF_SAVE)
                SetDlgItemText (hwnd, IDOK, "&Save");
            if (flags & OF_OPEN)
                SetDlgItemText (hwnd, IDOK, "&Open");

            if ((flags & OF_NOOPTIONS) &&
                (hwndT = GetDlgItem(hwnd,DLGOPEN_FOLDOUT)))
                EnableWindow (hwndT,FALSE);

            if (hwndT = GetDlgItem (hwnd, DLGOPEN_SMALL)) {
                GetWindowRect (hwnd,&rc);
                GetWindowRect (GetDlgItem(hwnd,DLGOPEN_SMALL),&rcCtl);

                SetWindowPos (hwnd,
                              NULL,
                              0,
                              0,
                              rcCtl.left - rc.left,
                              rc.bottom - rc.top,
                              SWP_NOZORDER | SWP_NOMOVE);
            }
            /* fill list box with filenames that match specifications, and
             * fill static field with path name.
             */
            FillListBox(hwnd,szExt, WM_INITDIALOG);

            /* If in Save mode, set the edit control with default (current)
             * file name,and select the corresponding entry in the listbox.
             */
            if ((flags & OF_SAVE) && *szFileName) {
                        SetDlgItemText (hwnd, DLGOPEN_EDIT, szFileName);
                        SendDlgItemMessage (hwnd,
                                    DLGOPEN_FILE_LISTBOX,
                                    LB_SELECTSTRING,
                                    0,
                                    (LONG)(LPSTR)szFileName);
            }
            else {
                /*  Set the edit field with the default extensions... */
                if (flags & OF_NOSHOWSPEC)
                    SetDlgItemText (hwnd, DLGOPEN_EDIT, "");
                else
                    SetDlgItemText (hwnd, DLGOPEN_EDIT, szExt);
            }
            /*  ...and select all text in the edit field */
                /* JAP added HWND cast*/
            SendMessage((HWND)GetDlgItem(hwnd, DLGOPEN_EDIT), EM_SETSEL, GET_EM_SETSEL_MPS(0, 0x7fff));

            /*  check all options that are set */
            for ( f = DLGOPEN_1BPP; f; f<<=1)
                CheckDlgButton(hwnd, (INT)FID(f), (WORD) (fOpt & f));

            break;

        case WM_COMMAND:
            w = LOWORD(wParam);
            switch (w) {
                case IDOK:
                    if (IsWindowEnabled (GetDlgItem(hwnd, IDOK))) {
                        /* Get contents of edit field and add search spec. if it
                         * does not contain one.
                         */
                        GetDlgItemText (hwnd, DLGOPEN_EDIT, (LPSTR)szFileName, 128);

                                                w = lstrlen(szFileName)-1;
                        c = szFileName[w];
                        switch (c) {
                            case '\\':
                            case '/':
                                szFileName[w] = 0;
                                break;
                        }
                        if (SetCurrentDirectory ((LPSTR)szFileName))
                            lstrcpy (szFileName,szExt);

                        /*  Try to open path.  If successful, fill listbox with
                         *  contents of new directory.  Otherwise, open datafile.
                         */
                        if (FSearchSpec(szFileName)) {
                            lstrcpy (szExt, FillListBox (hwnd, szFileName, WM_COMMAND));
                            if (flags & OF_NOSHOWSPEC) {
                                SetDlgItemText (hwnd, DLGOPEN_EDIT, "");
                            } else {
                                SetDlgItemText (hwnd, DLGOPEN_EDIT, szExt);
                            }
                            break;
                        }

                        /*  Make filename upper case and if it's a legal DOS
                         *  name, try to open the file.
                         */
                        AnsiUpper(szFileName);
                        AddExt(szFileName,szExt);
                        result = (INT)OpenFile(szFileName, &of, (WORD)flags);

                        if (result != -1) {
                            lstrcpy(szFileName,of.szPathName);
                        }
                        else if (flags & OF_MUSTEXIST) {
                            MessageBeep(0);
                            return 0L;
                        }

                        /*  Get the state of all checked options */
                        for (f = DLGOPEN_1BPP; f; f <<= 1){
                            if (IsDlgButtonChecked (hwnd, FID (f)))
                                fOpt |= f;
                            else
                                fOpt &= ~f;
                        }

                        EndDialog (hwnd, result);
                    }
                    break;

                case DLGOPEN_OPTION + DLGOPEN_RLE4:
                case DLGOPEN_OPTION + DLGOPEN_RLE8:
                case DLGOPEN_OPTION + DLGOPEN_RGB:
                    /* Mask out incompatible bitcount options and gray the
                     * appropriate radiobuttons.
                     */
                    for (f = DLGOPEN_1BPP; f <= DLGOPEN_24BPP; f <<= 1){
                        fEnable = !(f & BITCOUNTMASK [IDF(w) >> 4 ]);
                        EnableWindow (GetDlgItem (hwnd, FID(f)), fEnable);

                        /* If the radiobutton is being grayed, uncheck it and
                         * and check an "allowed" option so the bitcount group
                         * is still accessible via the keyboard
                         */
                        if (!fEnable && IsDlgButtonChecked (hwnd, FID (f))){
                            CheckDlgButton(hwnd, FID(f), FALSE);
                            CheckDlgButton(hwnd, FID(IDF(w) >> 3), TRUE);
                        }
                    }
                    break;

                case IDCANCEL:
                    /* User pressed cancel.  Just take down dialog box. */
                    EndDialog (hwnd, 0);
                    break;

                /*  User single clicked or doubled clicked in listbox -
                 *  Single click means fill edit box with selection.
                 *  Double click means go ahead and open the selection.
                 */
                case DLGOPEN_FILE_LISTBOX:
                case DLGOPEN_DIR_LISTBOX:
                    switch (GET_WM_COMMAND_CMD(wParam, lParam)) {
                        /* Single click case */
                        case LBN_SELCHANGE:
                            /* Get selection, which may be either a prefix to a
                             * new search path or a filename. DlgDirSelectEx parses
                             * selection, and appends a backslash if selection
                             * is a prefix
                             */
                            DlgDirSelectEx(hwnd, szFileName, 128, LOWORD(wParam));
                            w = lstrlen(szFileName)-1;
                            c = szFileName[w];
                            switch (c) {
                                case ':':
                                    lstrcat (szFileName,".");
                                    break;
                                case '\\':
                                case '/':
                                    szFileName[w] = 0;
                                    break;
                            }
                            SetDlgItemText(hwnd, DLGOPEN_EDIT, szFileName);
                            break;
                        /* Double click case - first click has already been
                         * processed as single click
                         */
                        case LBN_DBLCLK:
                            PostMessage (hwnd,WM_COMMAND,IDOK,0L);
                            break;
                    }
                    break;

                case DLGOPEN_EDIT:
                    DlgCheckOkEnable(hwnd, DLGOPEN_EDIT, HIWORD(lParam));
                    break;

                case DLGOPEN_FOLDOUT:
                    GetWindowRect(hwnd,&rc);
                    GetWindowRect(GetDlgItem(hwnd,DLGOPEN_BIG),&rcCtl);

                    if ((rcCtl.left <= rc.right) && (rcCtl.top <= rc.bottom))
                         GetWindowRect (GetDlgItem (hwnd, DLGOPEN_SMALL), &rcCtl);

                    SetWindowPos (hwnd,
                                  NULL,
                                  0,
                                  0,
                                  rcCtl.left - rc.left,
                                  rc.bottom - rc.top,
                                  SWP_NOZORDER | SWP_NOMOVE);
                    break;
            }
        default:
            return FALSE;
    }
    return TRUE;
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : static void NEAR DlgCheckOkEnable(hwnd, idEdit, message)   *
 *                                                                          *
 *  PURPOSE    : Enables the <OK> button in a dialog box iff the edit item  *
 *               contains text.                                             *
 *                                                                          *
 ****************************************************************************/
static VOID NEAR DlgCheckOkEnable(
    HWND        hwnd,
    INT idEdit,
    UINT message)
{
    if (message == EN_CHANGE) {
        EnableWindow ( GetDlgItem (hwnd, IDOK),
                       (BOOL)SendMessage (GetDlgItem (hwnd, idEdit),
                                          WM_GETTEXTLENGTH,
                                          0, 0L));
    }
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : AddExt (pch, ext)                                          *
 *                                                                          *
 *  PURPOSE    : Add an extension to a filename if none is already specified*
 *                                                                          *
 ****************************************************************************/
static VOID NEAR AddExt (
    CHAR *pch,    /* File name    */
    CHAR *ext)    /* Extension to add */
{
    CHAR acExt[20];
    CHAR *pext = acExt;

    while (*ext && *ext != '.')
        ext++;
    while (*ext && *ext != ';')
        *pext++ = *ext++;
    *pext = 0;
    pext = acExt;

    while (*pch == '.') {
        pch++;
        if ((*pch == '.') && pch[1] == '\\')
            pch += 2;                       /* ..\ */
        if (*pch == '\\')
            pch++;                         /* .\ */
    }
    while (*pch != '\0')
        if (*pch++ == '.')
            return;

    // *pch++ = '.';
    do
        *pch++ = *pext;
    while (*pext++ != '\0');
}
/****************************************************************************
 *                                                                          *
 *  FUNCTION   : FSearchSpec (sz)                                           *
 *                                                                          *
 *  PURPOSE    : Checks to see if NULL-terminated strings contains a "*" or *
 *               a "?".                                                     *
 *                                                                          *
 *  RETURNS    : TRUE  - if the above characters are found in the string    *
 *               FALSE - otherwise.                                         *
 *                                                                          *
 ****************************************************************************/
static BOOL NEAR FSearchSpec(CHAR       *sz)
{
    for (; *sz ;sz++) {
        if (*sz == '*' || *sz == '?')
            return TRUE;
    }
    return FALSE;
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : static char * NEAR FillListBox (hDlg,pFile, cmd)                   *
 *                                                                          *
 *  PURPOSE    : Fill list box with filenames that match specifications, and*
 *               fills the static field with the path name.                 *
 *                                                                          *
 *  RETURNS    : A pointer to the pathname.                                                           *
 *                                                                          *
 ****************************************************************************/
static CHAR * NEAR FillListBox (
    HWND  hDlg,
    CHAR  *pFile,  /* [path]{list of file wild cards, separated by ';'} */
    UINT cmd)    /* if initdialog, or WM_COMMAND*/
{
    CHAR  ach[128];
    CHAR  *pch;
    CHAR  *pDir;   /* Directory name or path */
    CHAR  pCurDir[256];

    pch  = pFile;
    pDir = ach;
    if(cmd == WM_INITDIALOG){
    while (*pch && *pch != ';')
        pch++;
    while ((pch > pFile) && (*pch != '/') && (*pch != '\\'))
        pch--;
    if (pch > pFile) {
       *pch = 0;
       lstrcpy (pDir, pFile);
       pFile = pch + 1;
    }
    else {
       lstrcpy (pDir,".");
    }
    }
    else{
        /* since SetCurrentDirectory was called already, I'll use GetCurrentDirectory*/
        /* to get pDir*/
        GetCurrentDirectory(256, pCurDir);
        strcpy(pDir, pCurDir);
    }
    DlgDirList (hDlg, pDir, (INT)DLGOPEN_DIR_LISTBOX, (INT)DLGOPEN_PATH,(WORD)ATTRDIRLIST);
    SendDlgItemMessage (hDlg, DLGOPEN_FILE_LISTBOX, LB_RESETCONTENT, 0, 0L);
    SendDlgItemMessage (hDlg, DLGOPEN_FILE_LISTBOX, WM_SETREDRAW, FALSE, 0L);
    pDir = pFile;            /* save pFile to return */
    while (*pFile) {
        pch = ach;
        while (*pFile==' ')
            pFile++;
        while (*pFile && *pFile != ';')
            *pch++ = *pFile++;
        *pch = 0;
        if (*pFile)
            pFile++;
        SendDlgItemMessage (hDlg,
                            DLGOPEN_FILE_LISTBOX,
                            LB_DIR,ATTRFILELIST,
                            (LONG)(LPSTR)ach);
    }
    SendDlgItemMessage (hDlg, DLGOPEN_FILE_LISTBOX, WM_SETREDRAW, TRUE, 0L);
    InvalidateRect (GetDlgItem (hDlg, DLGOPEN_FILE_LISTBOX), NULL, TRUE);

    return pDir;
}
