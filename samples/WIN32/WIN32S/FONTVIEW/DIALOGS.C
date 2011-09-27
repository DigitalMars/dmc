
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define NOMINMAX
#include <windows.h>
#include "FontView.h"

#include <stdlib.h>

#if !defined (APIENTRY)
#define APIENTRY FAR PASCAL
#endif

BOOL CenterWindow (HWND, HWND);


typedef struct FONTENUM {
    short       ft;
    TEXTMETRIC  tm;
    LOGFONT     lf;
} FONTSTRUCT;
typedef FONTSTRUCT      *PFONTSTRUCT;
typedef FONTSTRUCT NEAR *NPFONTSTRUCT;
typedef FONTSTRUCT FAR  *LPFONTSTRUCT;

typedef struct FONTLIST {
    int     count;
    HANDLE  hList;
} FONTLIST;
typedef FONTLIST        *PFONTLIST;
typedef FONTLIST NEAR   *NPFONTLIST;
typedef FONTLIST FAR    *LPFONTLIST;


void SetDlgItemValue (HWND hDlg, int nIDDlgItem, int wValue, BOOL bSigned, int nBase)
{
    char szValue[20];

    switch (nBase) {
        case 8:
            wsprintf (szValue, "0o%o", wValue);
            SetDlgItemText (hDlg, nIDDlgItem, szValue);
            break;
        case 16:
            wsprintf (szValue, "0x%x", wValue);
            SetDlgItemText (hDlg, nIDDlgItem, szValue);
            break;
        default:
            SetDlgItemInt (hDlg, nIDDlgItem, wValue, bSigned);
            break;
    }
}

int FAR PASCAL DlgEnumFontSizes (lpLogFont, lpTextMetric, nFontType, lpData)
    LPLOGFONT lpLogFont;
    LPTEXTMETRIC lpTextMetric;
    short nFontType;
    LPHANDLE lpData;
{
    HANDLE hFonts;
    LPFONTSTRUCT pFS;
    LPFONTLIST pFL;

    /*
        This function will lock down the incoming handle, properly alloc, and realloc the
        handle within it to hold the data of the fonts enumerated, then unlock the handle.
    */
    hFonts = *lpData;
    pFL = (LPFONTLIST)GlobalLock (hFonts);
    if (!pFL) {
        return FALSE;
    } else if (pFL->count == 0) {
        pFL->hList = GlobalAlloc (GHND, sizeof(FONTSTRUCT));
    } else {
        pFL->hList = GlobalReAlloc (pFL->hList, sizeof(FONTSTRUCT)*(1+pFL->count), GMEM_MOVEABLE);
    }

    if (pFL->hList) {
        pFS = (LPFONTSTRUCT)GlobalLock (pFL->hList);
        if (pFS) {
            pFS[pFL->count].ft = nFontType;
            pFS[pFL->count].tm = *lpTextMetric;
            pFS[pFL->count].lf = *lpLogFont;
            GlobalUnlock (pFL->hList);
            pFL->count++;
        }
        GlobalUnlock (hFonts);

    } else {
        GlobalUnlock (hFonts);
        return FALSE;
    }

    return TRUE;
}

int FAR PASCAL DlgEnumFontNames (lpLogFont, lpTextMetric, nFontType, lpData)
    LPLOGFONT lpLogFont;
    LPTEXTMETRIC lpTextMetric;
    short nFontType;
    LPHANDLE lpData;
{
    HDC  hdc;
    HWND hwnd;
    HANDLE hInst;
    FARPROC lpFontEnumProc;

/*
   This function is just a pass through. For each face encountered, it will in turn enumerate all
   sizes available. the lpData, which is a FAR * to a FONTSTRUCT structure will simply be passed
   on to the second enumeration procedure which will fill it in.
 */
    hwnd = GetFocus();
#if defined (WIN32)
    hInst = (HANDLE)GetWindowLong (hwnd, GWL_HINSTANCE);
#elif defined (WIN16)
    hInst = (HANDLE)GetWindowWord (hwnd, GWW_HINSTANCE);
#endif

    lpFontEnumProc = MakeProcInstance((FARPROC)DlgEnumFontSizes, hInst);
    if (lpFontEnumProc) {
        hdc  = GetDC(hwnd);
        EnumFonts (hdc, lpLogFont->lfFaceName, (FONTENUMPROC)lpFontEnumProc, (LPARAM)lpData);
        ReleaseDC(hwnd, hdc);
        FreeProcInstance (lpFontEnumProc);
    } else {
        MessageBox (GetFocus(), "Couldn't create a proc instance", "FontView", MB_OK);
        return FALSE;
    }
    return TRUE;

    lpTextMetric;  // unreferenced formal parameter
    nFontType;     // unreferenced formal parameter
}



BOOL APIENTRY SimpleDlgProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
    int wmId;
    static HBITMAP hbmFontView;
    static BITMAP bmFontView;
    RECT rect;
    HDC hdc, hdcSrc;
    HBITMAP hbmOld;
    PAINTSTRUCT ps;
    HANDLE hInst;

#if defined (WIN32)
    hInst = (HANDLE)GetWindowLong (hwnd, GWL_HINSTANCE);
#elif defined (WIN16)
    hInst = (HANDLE)GetWindowWord (hwnd, GWW_HINSTANCE);
#endif

    switch (msg) {
        case WM_INITDIALOG:
            CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER));
            hbmFontView = LoadBitmap (hInst, "FONTVIEW");
            GetObject (hbmFontView,sizeof(BITMAP), &bmFontView);
            if (!hbmFontView) MessageBeep(0);
            return (TRUE);

        case WM_DESTROY:
            DeleteObject (hbmFontView);
            break;

        case WM_PAINT:
            hdc = BeginPaint (hwnd, &ps);
            GetWindowRect (hwnd, &rect);
            ScreenToClient (hwnd, (LPPOINT)&rect.left);
            ScreenToClient (hwnd, (LPPOINT)&rect.right);
            hdc = GetDC (hwnd);
            hdcSrc = CreateCompatibleDC (hdc);
            hbmOld = SelectObject (hdcSrc, hbmFontView);
            if (!BitBlt (hdc, 0, 0, bmFontView.bmWidth, bmFontView.bmHeight, hdcSrc, 0, 0, SRCCOPY)) {
                MessageBeep(0);
            }
            SelectObject (hdcSrc, hbmOld);
            DeleteDC (hdcSrc);
            EndPaint (hwnd, &ps);
            break;

        case WM_COMMAND:
#if defined (WIN32)
            wmId = LOWORD(wParam);
#elif defined (WIN16)
            wmId = wParam;
#endif
            switch (wmId) {

                case IDOK:
                    EndDialog(hwnd, TRUE);
                    return (TRUE);

                case IDCANCEL:
                    EndDialog(hwnd, TRUE);
                    return (TRUE);
            }
            break;
    }
    return (FALSE);

    lParam; // unreferenced formal parameter
}


/*
    This dialog will present edit controls for all the parameters of a CreateFont call.
    The user can put any value in any of the fields, no validation is done. These parameters
    are then used to create a font with, and that will be the font that will be displayed.
 */
BOOL APIENTRY CreateDlgProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
    int wmId, i;
    BOOL bDone;
static LOGFONT lfDlg;
static LPLOGFONT lplf;

    switch (msg) {
        case WM_INITDIALOG:
            CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER));
            lplf = (LOGFONT *)lParam;
            lfDlg = *lplf;
            SetDlgItemInt (hwnd, CFD_HEIGHT, (int)lfDlg.lfHeight, TRUE);
            SetDlgItemInt (hwnd, CFD_WIDTH, (int)lfDlg.lfWidth, TRUE);
            SetDlgItemInt (hwnd, CFD_ESCAPEMENT, (int)lfDlg.lfEscapement, TRUE);
            SetDlgItemInt (hwnd, CFD_ORIENTATION, (int)lfDlg.lfOrientation, TRUE);
            SetDlgItemInt (hwnd, CFD_WEIGHT, (int)lfDlg.lfWeight, FALSE);
            SetDlgItemInt (hwnd, CFD_ITALIC, (int)lfDlg.lfItalic, FALSE);
            SetDlgItemInt (hwnd, CFD_UNDERLINE, (int)lfDlg.lfUnderline, FALSE);
            SetDlgItemInt (hwnd, CFD_STRIKEOUT, (int)lfDlg.lfStrikeOut, FALSE);
            SetDlgItemInt (hwnd, CFD_CHARSET, (int)lfDlg.lfCharSet, FALSE);
            SetDlgItemInt (hwnd, CFD_OUTPUTPRECISION, (int)lfDlg.lfOutPrecision, FALSE);
            SetDlgItemInt (hwnd, CFD_CLIPPRECISION, (int)lfDlg.lfClipPrecision, FALSE);
            SetDlgItemInt (hwnd, CFD_QUALITY, (int)lfDlg.lfQuality, FALSE);
            SetDlgItemInt (hwnd, CFD_PITCHANDFAMILY, (int)lfDlg.lfPitchAndFamily, FALSE);
            SetDlgItemText(hwnd, CFD_FACENAME, lfDlg.lfFaceName);

            return (TRUE);

        case WM_COMMAND:
#if defined (WIN32)
            wmId = LOWORD(wParam);
#elif defined (WIN16)
            wmId = wParam;
#endif
            switch (wmId) {
                case CFD_DEFAULT:
                    // Set all elements to ZERO. This will give us a 'default' font
                    for (i=CFD_BASE; i<=CFD_PITCHANDFAMILY; i++) {
                        SetDlgItemInt (hwnd, i, 0, FALSE);
                    }
                    SetDlgItemText (hwnd, CFD_FACENAME, "");
                    break;

                case IDOK:
                    // Get the data from the edit control, we will then use this for a 'CreatFont' call
                    lfDlg.lfHeight = GetDlgItemInt (hwnd, CFD_HEIGHT, &bDone, TRUE);
                    lfDlg.lfWidth = GetDlgItemInt (hwnd, CFD_WIDTH, &bDone, TRUE);
                    lfDlg.lfEscapement = GetDlgItemInt (hwnd, CFD_ESCAPEMENT, &bDone, TRUE);
                    lfDlg.lfOrientation = GetDlgItemInt (hwnd, CFD_ORIENTATION, &bDone, TRUE);
                    lfDlg.lfWeight = GetDlgItemInt (hwnd, CFD_WEIGHT, &bDone, FALSE);
                    lfDlg.lfItalic = (BYTE)GetDlgItemInt (hwnd, CFD_ITALIC, &bDone, FALSE);
                    lfDlg.lfUnderline = (BYTE)GetDlgItemInt (hwnd, CFD_UNDERLINE, &bDone, FALSE);
                    lfDlg.lfStrikeOut = (BYTE)GetDlgItemInt (hwnd, CFD_STRIKEOUT, &bDone, FALSE);
                    lfDlg.lfCharSet = (BYTE)GetDlgItemInt (hwnd, CFD_CHARSET, &bDone, FALSE);
                    lfDlg.lfOutPrecision = (BYTE)GetDlgItemInt (hwnd, CFD_OUTPUTPRECISION, &bDone, FALSE);
                    lfDlg.lfClipPrecision = (BYTE)GetDlgItemInt (hwnd, CFD_CLIPPRECISION, &bDone, FALSE);
                    lfDlg.lfQuality = (BYTE)GetDlgItemInt (hwnd, CFD_QUALITY, &bDone, FALSE);
                    lfDlg.lfPitchAndFamily = (BYTE)GetDlgItemInt (hwnd, CFD_PITCHANDFAMILY, &bDone, FALSE);
                    GetDlgItemText(hwnd, CFD_FACENAME, lfDlg.lfFaceName,20);
                    // and copy the data into our external structure
                    *lplf = lfDlg;
                    EndDialog(hwnd, TRUE);
                    return (TRUE);

                case IDCANCEL:
                    // Exit without changing anything
                    EndDialog(hwnd, TRUE);
                    return (TRUE);

            }
            break;
    }
    return (FALSE);

    lParam; //unreferenced formal paramter
}

/*
 This dialog will display the TEXTMETRIC data that is retrieved from a 'GetTextMetric' call.
 If the 'Select' button is clicked, then the current CreateFont data will be replaced with
 as much data from TEXTMETRIC as possible.
*/
BOOL APIENTRY MetricsDlgProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
static LOGFONT lfDlg;
static LPLOGFONT lplf;
static TEXTMETRIC tm;
static char szFacename[LF_FACESIZE];
    int wmId;
    BOOL bDone;
    TEXTMETRIC tmTmp;
    HFONT hfont, hfontPrev;
    HDC   hdc;
    char szBuffer[LF_FACESIZE+15];


    switch (msg) {
        case WM_INITDIALOG:
            CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER));
            lplf = (LOGFONT *)lParam;
            lfDlg = *lplf;

            hfont = CreateFontIndirect (&lfDlg);
            hdc = GetDC (hwnd);
            hfontPrev = SelectObject (hdc, hfont);
            GetTextMetrics (hdc, &tm);
            GetTextFace    (hdc, sizeof(szFacename), szFacename);
            SelectObject (hdc, hfontPrev);
            DeleteObject (hfont);
            ReleaseDC (hwnd, hdc);

            wsprintf (szBuffer, "TextMetrics: %s", (LPSTR)szFacename);
            SetWindowText (hwnd, szBuffer);

            SetDlgItemInt (hwnd, TMD_HEIGHT, (int)tm.tmHeight, TRUE);
            SetDlgItemInt (hwnd, TMD_ASCENT, (int)tm.tmAscent, TRUE);
            SetDlgItemInt (hwnd, TMD_DESCENT, (int)tm.tmDescent, TRUE);
            SetDlgItemInt (hwnd, TMD_INTERNAL, (int)tm.tmInternalLeading, TRUE);
            SetDlgItemInt (hwnd, TMD_EXTERNAL, (int)tm.tmExternalLeading, TRUE);
            SetDlgItemInt (hwnd, TMD_AVEWIDTH, (int)tm.tmAveCharWidth, TRUE);
            SetDlgItemInt (hwnd, TMD_MAXWIDTH, (int)tm.tmMaxCharWidth, TRUE);
            SetDlgItemInt (hwnd, TMD_WEIGHT, (int)tm.tmWeight, TRUE);
            SetDlgItemInt (hwnd, TMD_ITALIC, (int)tm.tmItalic, FALSE);
            SetDlgItemInt (hwnd, TMD_UNDERLINE, (int)tm.tmUnderlined, FALSE);
            SetDlgItemInt (hwnd, TMD_STRUCKOUT, (int)tm.tmStruckOut, FALSE);
            SetDlgItemInt (hwnd, TMD_FIRSTCHAR, (int)tm.tmFirstChar, FALSE);
            SetDlgItemInt (hwnd, TMD_LASTCHAR, (int)tm.tmLastChar, FALSE);
            SetDlgItemInt (hwnd, TMD_DEFAULTCHAR, (int)tm.tmDefaultChar, FALSE);
            SetDlgItemInt (hwnd, TMD_BREAKCHAR, (int)tm.tmBreakChar, FALSE);
            SetDlgItemInt (hwnd, TMD_PITCHANDFAMILY, (int)tm.tmPitchAndFamily, FALSE);
            SetDlgItemInt (hwnd, TMD_CHARSET, (int)tm.tmCharSet, FALSE);
            SetDlgItemInt (hwnd, TMD_OVERHANG, (int)tm.tmOverhang, TRUE);
            SetDlgItemInt (hwnd, TMD_DIGITIZEDASPECTX, (int)tm.tmDigitizedAspectX, TRUE);
            SetDlgItemInt (hwnd, TMD_DIGITIZEDASPECTY, (int)tm.tmDigitizedAspectY, TRUE);

            return (TRUE);

        case WM_COMMAND:
#if defined (WIN32)
            wmId = LOWORD(wParam);
#elif defined (WIN16)
            wmId = wParam;
#endif
            switch (wmId) {
                case IDOK:
                    // Lets pull in as much data from the TEXTMETRIC structure as possible...
                    lfDlg.lfHeight         = tm.tmHeight;
                    lfDlg.lfWidth          = tm.tmAveCharWidth;
                    //lfDlg.lfEscapement - No Use
                    //lfDlg.lfOrientation - No Use
                    lfDlg.lfWeight         = tm.tmWeight;
                    lfDlg.lfItalic         = tm.tmItalic;
                    lfDlg.lfUnderline      = tm.tmUnderlined;
                    lfDlg.lfStrikeOut      = tm.tmStruckOut;
                    lfDlg.lfCharSet        = tm.tmCharSet;
                    //lfDlg.lfOutPrecision - No Use
                    //lfDlg.lfClipPrecision - No Use
                    //lfDlg.lfQuality - No Use
                    lfDlg.lfPitchAndFamily = tm.tmPitchAndFamily;
                    lstrcpy(lfDlg.lfFaceName, szFacename);

                    // Lets create a font with this new data
                    hfont = CreateFontIndirect (&lfDlg);
                    hdc = GetDC (hwnd);
                    hfontPrev = SelectObject (hdc, hfont);
                    GetTextMetrics (hdc, &tmTmp);
                    // Get the face name
                    GetTextFace    (hdc, sizeof(szBuffer), szBuffer);
                    SelectObject (hdc, hfontPrev);
                    DeleteObject (hfont);
                    ReleaseDC (hwnd, hdc);

                    // And verify that we did indeed get the same font.
                    bDone = TRUE;
                    bDone = bDone && (tm.tmHeight==tmTmp.tmHeight);
                    bDone = bDone && (tm.tmAscent==tmTmp.tmAscent);
                    bDone = bDone && (tm.tmDescent==tmTmp.tmDescent);
                    bDone = bDone && (tm.tmInternalLeading==tmTmp.tmInternalLeading);
                    bDone = bDone && (tm.tmExternalLeading==tmTmp.tmExternalLeading);
                    bDone = bDone && (tm.tmAveCharWidth==tmTmp.tmAveCharWidth);
                    bDone = bDone && (tm.tmMaxCharWidth==tmTmp.tmMaxCharWidth);
                    bDone = bDone && (tm.tmWeight==tmTmp.tmWeight);
                    bDone = bDone && (tm.tmItalic==tmTmp.tmItalic);
                    bDone = bDone && (tm.tmUnderlined==tmTmp.tmUnderlined);
                    bDone = bDone && (tm.tmStruckOut==tmTmp.tmStruckOut);
                    bDone = bDone && (tm.tmFirstChar==tmTmp.tmFirstChar);
                    bDone = bDone && (tm.tmLastChar==tmTmp.tmLastChar);
                    bDone = bDone && (tm.tmDefaultChar==tmTmp.tmDefaultChar);
                    bDone = bDone && (tm.tmBreakChar==tmTmp.tmBreakChar);
                    bDone = bDone && (tm.tmPitchAndFamily==tmTmp.tmPitchAndFamily);
                    bDone = bDone && (tm.tmCharSet==tmTmp.tmCharSet);
                    // Did it work?
                    if (bDone) {
                        *lplf = lfDlg;
                        EndDialog(hwnd, TRUE);
                        return (TRUE);
                    } else {
                        // We need to take a close look at the font verification
                        // code. Currently, it sometimes will report that the
                        // font didn't get properly selected, even if it did.
                        *lplf = lfDlg;
                        EndDialog(hwnd, TRUE);
                        return (TRUE);

                        // This is what we want to do once we beef up the font
                        // verification code:
                        lfDlg = *lplf;
                        MessageBox (GetFocus(),
                            "Unable to re-create font from TextMetrics",
                            "FontView", MB_OK);
                    }
                    break;

                case IDCANCEL:
                    EndDialog(hwnd, TRUE);
                    return (TRUE);

            }
            break;
    }
    return (FALSE);

    /* Just For Reference */
    lParam;
}


BOOL FillEnumFields (HWND hwnd, int iType, LPTEXTMETRIC ptm, LPLOGFONT plf, int nBase)
{
            SetDlgItemValue (hwnd, TMD_HEIGHT, (int)ptm->tmHeight, TRUE, nBase);
            SetDlgItemValue (hwnd, TMD_ASCENT, (int)ptm->tmAscent, TRUE, nBase);
            SetDlgItemValue (hwnd, TMD_DESCENT, (int)ptm->tmDescent, TRUE, nBase);
            SetDlgItemValue (hwnd, TMD_INTERNAL, (int)ptm->tmInternalLeading, TRUE, nBase);
            SetDlgItemValue (hwnd, TMD_EXTERNAL, (int)ptm->tmExternalLeading, TRUE, nBase);
            SetDlgItemValue (hwnd, TMD_AVEWIDTH, (int)ptm->tmAveCharWidth, TRUE, nBase);
            SetDlgItemValue (hwnd, TMD_MAXWIDTH, (int)ptm->tmMaxCharWidth, TRUE, nBase);
            SetDlgItemValue (hwnd, TMD_WEIGHT, (int)ptm->tmWeight, TRUE, nBase);
            SetDlgItemValue (hwnd, TMD_ITALIC, (int)ptm->tmItalic, FALSE, nBase);
            SetDlgItemValue (hwnd, TMD_UNDERLINE, (int)ptm->tmUnderlined, FALSE, nBase);
            SetDlgItemValue (hwnd, TMD_STRUCKOUT, (int)ptm->tmStruckOut, FALSE, nBase);
            SetDlgItemValue (hwnd, TMD_FIRSTCHAR, (int)ptm->tmFirstChar, FALSE, nBase);
            SetDlgItemValue (hwnd, TMD_LASTCHAR, (int)ptm->tmLastChar, FALSE, nBase);
            SetDlgItemValue (hwnd, TMD_DEFAULTCHAR, (int)ptm->tmDefaultChar, FALSE, nBase);
            SetDlgItemValue (hwnd, TMD_BREAKCHAR, (int)ptm->tmBreakChar, FALSE, nBase);
            SetDlgItemValue (hwnd, TMD_PITCHANDFAMILY, (int)ptm->tmPitchAndFamily, FALSE, nBase);
            SetDlgItemValue (hwnd, TMD_CHARSET, (int)ptm->tmCharSet, FALSE, nBase);
            SetDlgItemValue (hwnd, TMD_OVERHANG, (int)ptm->tmOverhang, TRUE, nBase);
            SetDlgItemValue (hwnd, TMD_DIGITIZEDASPECTX, (int)ptm->tmDigitizedAspectX, TRUE, nBase);
            SetDlgItemValue (hwnd, TMD_DIGITIZEDASPECTY, (int)ptm->tmDigitizedAspectY, TRUE, nBase);

            SetDlgItemValue (hwnd, CFD_HEIGHT, (int)plf->lfHeight, TRUE, nBase);
            SetDlgItemValue (hwnd, CFD_WIDTH, (int)plf->lfWidth, TRUE, nBase);
            SetDlgItemValue (hwnd, CFD_ESCAPEMENT, (int)plf->lfEscapement, TRUE, nBase);
            SetDlgItemValue (hwnd, CFD_ORIENTATION, (int)plf->lfOrientation, TRUE, nBase);
            SetDlgItemValue (hwnd, CFD_WEIGHT, (int)plf->lfWeight, FALSE, nBase);
            SetDlgItemValue (hwnd, CFD_ITALIC, (int)plf->lfItalic, FALSE, nBase);
            SetDlgItemValue (hwnd, CFD_UNDERLINE, (int)plf->lfUnderline, FALSE, nBase);
            SetDlgItemValue (hwnd, CFD_STRIKEOUT, (int)plf->lfStrikeOut, FALSE, nBase);
            SetDlgItemValue (hwnd, CFD_CHARSET, (int)plf->lfCharSet, FALSE, nBase);
            SetDlgItemValue (hwnd, CFD_OUTPUTPRECISION, (int)plf->lfOutPrecision, FALSE, nBase);
            SetDlgItemValue (hwnd, CFD_CLIPPRECISION, (int)plf->lfClipPrecision, FALSE, nBase);
            SetDlgItemValue (hwnd, CFD_QUALITY, (int)plf->lfQuality, FALSE, nBase);
            SetDlgItemValue (hwnd, CFD_PITCHANDFAMILY, (int)plf->lfPitchAndFamily, FALSE, nBase);
            SetDlgItemText(hwnd, CFD_FACENAME, plf->lfFaceName);

            SetDlgItemValue (hwnd, ED_TYPE, (int)iType, FALSE, nBase);

            return TRUE;
}


BOOL DrawSample (HWND hwnd, LPLOGFONT plf)
{
    HFONT   hfont, hfontPrev;
    HDC     hdc;
    RECT    r;

    hfont = CreateFontIndirect (plf);
    hdc = GetDC (hwnd);
    hfontPrev = SelectObject (hdc, hfont);
    GetWindowRect (hwnd, &r);
    ScreenToClient (hwnd, (LPPOINT)&r.left);
    ScreenToClient (hwnd, (LPPOINT)&r.right);

    Rectangle (hdc, r.left, r.top, r.right, r.bottom);

    InflateRect (&r, -1, -1);
    SetTextAlign (hdc, TA_BOTTOM | TA_CENTER);
    ExtTextOut (hdc, r.left + ((r.right-r.left)/2), r.bottom, ETO_CLIPPED, &r,"AaBbCcDdEe 012345", 17, NULL);

    SelectObject (hdc, hfontPrev);
    DeleteObject (hfont);
    ReleaseDC (hwnd, hdc);
    return TRUE;
}


/*
    Display a dialog that the user can use to enumerate through all of the fonts in the system.
    Show him not only all of the metrics for the font, but a sample of the font as well.
    If the user picks the 'Select' button, then the Metrics of this font will be used for
    the CreateFont call.
*/
BOOL APIENTRY EnumDlgProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
    static HANDLE /*TO FONTLIST*/ hFonts;
    static int iLoc=0;
    static int count=0;
    static BOOL bHex = FALSE;
    static LOGFONT *lplf;
    static LOGFONT lfDlg;
    int wmId, i, j;
    HANDLE hInst;
    HWND   hwndItem;
    HDC hdc;
    FARPROC lpEnumFonts;
    LPFONTLIST    pFL;
    LPFONTSTRUCT  pFS;
    char szTmp[80];
    HFONT hfont, hfontPrev;
    BOOL bDone;
    TEXTMETRIC tmTmp;


    switch (msg) {
        case WM_INITDIALOG:
            lplf = (LOGFONT *)lParam;
            lfDlg = *lplf;
            iLoc = 0;
            count = 0;
            bHex = FALSE;
            /* First, lets enumerate ALL fonts, and store them in our list */
            hFonts = GlobalAlloc (GHND, sizeof(FONTLIST));
            if (hFonts) {

#if defined (WIN32)
    hInst = (HANDLE)GetWindowLong (hwnd, GWL_HINSTANCE);
#elif defined (WIN16)
    hInst = (HANDLE)GetWindowWord (hwnd, GWW_HINSTANCE);
#endif

                lpEnumFonts = MakeProcInstance((FARPROC)DlgEnumFontNames, hInst);
                if (lpEnumFonts) {
                    hdc  = GetDC(hwnd);
                    // The enumeration function will lock down the handle
                    EnumFonts (hdc, NULL, (FONTENUMPROC)lpEnumFonts, (LPARAM)&hFonts);
                    // The handle will come back to us properly unlocked
                    ReleaseDC(hwnd, hdc);
                    FreeProcInstance (lpEnumFonts);

                 }
            }
            pFL = (LPFONTLIST)GlobalLock (hFonts);
            if (pFL) {
                pFS = (LPFONTSTRUCT)GlobalLock (pFL->hList);
                if (pFS) {
                    iLoc = 0;
                    j = 100;
                    for (i=0; i<pFL->count; i++) {
                        if (lstrcmp(lfDlg.lfFaceName, pFS[i].lf.lfFaceName) == 0) {
                            if (abs(lfDlg.lfHeight-pFS[i].lf.lfHeight) < j) {
                                j = abs(lfDlg.lfHeight-pFS[i].lf.lfHeight);
                                iLoc = i;
                            }
                        }
                    }
                    FillEnumFields (hwnd, pFS[iLoc].ft, &pFS[iLoc].tm, &pFS[iLoc].lf, (bHex?16:10));
                    hwndItem = GetDlgItem (hwnd, ED_SAMPLE);
                    DrawSample (hwndItem, &pFS[iLoc].lf);
                    GlobalUnlock (pFL->hList);

                }
                count = pFL->count;
                GlobalUnlock (hFonts);

            }
            /* now fill in the dialog values */
            wsprintf (szTmp, "EnumFonts %d of %d", iLoc+1, count);
            SetWindowText (hwnd, szTmp);
            CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER));

            return TRUE;

        case WM_PAINT:
            pFL = (LPFONTLIST)GlobalLock (hFonts);
            if (pFL) {
                pFS = (LPFONTSTRUCT)GlobalLock (pFL->hList);
                if (pFS) {
                    FillEnumFields (hwnd, pFS[iLoc].ft, &pFS[iLoc].tm, &pFS[iLoc].lf, (bHex?16:10));
                    hwndItem = GetDlgItem (hwnd, ED_SAMPLE);
                    DrawSample (hwndItem, &pFS[iLoc].lf);
                    GlobalUnlock (pFL->hList);
                }
                GlobalUnlock (hFonts);
            }
            return 0;

        case WM_COMMAND:
#if defined (WIN32)
            wmId = LOWORD(wParam);
#elif defined (WIN16)
            wmId = wParam;
#endif
            switch (wmId) {

                case ED_HEX:
                    // Display the data in either Hex mode or Dec mode.
                    bHex = !bHex;
                    CheckDlgButton (hwnd, wmId, bHex);
                    pFL = (LPFONTLIST)GlobalLock (hFonts);
                    if (pFL) {
                        pFS = (LPFONTSTRUCT)GlobalLock (pFL->hList);
                        if (pFS) {
                            FillEnumFields (hwnd, pFS[iLoc].ft, &pFS[iLoc].tm, &pFS[iLoc].lf, (bHex?16:10));
                            if (GlobalUnlock (pFL->hList)) {
                                MessageBox (GetFocus(), "In HEX", "Unlock pFL->hList", MB_OK);
                            }
                        }
                        if (GlobalUnlock (hFonts)) {
                            MessageBox (GetFocus(), "In HEX", "Unlock hFonts", MB_OK);
                        }

                    }
                    break;

                case ED_PREV:
                    pFL = (LPFONTLIST)GlobalLock (hFonts);
                    if (pFL) {
                        if (iLoc > 0) {
                            iLoc--;
                            pFS = (LPFONTSTRUCT)GlobalLock (pFL->hList);
                            if (pFS) {
                                FillEnumFields (hwnd, pFS[iLoc].ft, &pFS[iLoc].tm, &pFS[iLoc].lf, (bHex?16:10));
                                hwndItem = GetDlgItem (hwnd, ED_SAMPLE);
                                DrawSample (hwndItem, &pFS[iLoc].lf);
                                GlobalUnlock (pFL->hList);
                            }
                        }
                        GlobalUnlock (hFonts);
                    }
                    wsprintf (szTmp, "EnumFonts %d of %d", iLoc+1, count);
                    SetWindowText (hwnd, szTmp);
                    break;

                case ED_NEXT:
                    pFL = (LPFONTLIST)GlobalLock (hFonts);
                    if (pFL) {
                        if ((pFL->count-1) > iLoc) {
                            iLoc++;
                            pFS = (LPFONTSTRUCT)GlobalLock (pFL->hList);
                            if (pFS) {
                                FillEnumFields (hwnd, pFS[iLoc].ft, &pFS[iLoc].tm, &pFS[iLoc].lf, (bHex?16:10));
                                hwndItem = GetDlgItem (hwnd, ED_SAMPLE);
                                DrawSample (hwndItem, &pFS[iLoc].lf);
                                GlobalUnlock (pFL->hList);
                            }

                        }
                        GlobalUnlock (hFonts);
                    }
                    wsprintf (szTmp, "EnumFonts %d of %d", iLoc+1, count);
                    SetWindowText (hwnd, szTmp);
                    break;

                case IDOK:
                    // Copy the LOGFONT structure from the enumeration list, into our private LF
                    pFL = (LPFONTLIST)GlobalLock (hFonts);
                    bDone = FALSE;
                    if (pFL) {
                        if ((pFL->count-1) >= iLoc) {
                            pFS = (LPFONTSTRUCT)GlobalLock (pFL->hList);
                            if (pFS) {
                                lfDlg = pFS[iLoc].lf;

                    // Lets create a font with this new data
                    hfont = CreateFontIndirect (&lfDlg);
                    hdc = GetDC (hwnd);
                    hfontPrev = SelectObject (hdc, hfont);
                    GetTextMetrics (hdc, &tmTmp);
                    // Get the face name
                    GetTextFace    (hdc, sizeof(szTmp), szTmp);
                    SelectObject (hdc, hfontPrev);
                    DeleteObject (hfont);
                    ReleaseDC (hwnd, hdc);

                    // And verify that we did indeed get the same font.
                    bDone = TRUE;
                    bDone = bDone && (pFS[iLoc].tm.tmHeight==tmTmp.tmHeight);
                    bDone = bDone && (pFS[iLoc].tm.tmAscent==tmTmp.tmAscent);
                    bDone = bDone && (pFS[iLoc].tm.tmDescent==tmTmp.tmDescent);
                    bDone = bDone && (pFS[iLoc].tm.tmInternalLeading==tmTmp.tmInternalLeading);
                    bDone = bDone && (pFS[iLoc].tm.tmExternalLeading==tmTmp.tmExternalLeading);
                    bDone = bDone && (pFS[iLoc].tm.tmAveCharWidth==tmTmp.tmAveCharWidth);
                    bDone = bDone && (pFS[iLoc].tm.tmMaxCharWidth==tmTmp.tmMaxCharWidth);
                    bDone = bDone && (pFS[iLoc].tm.tmWeight==tmTmp.tmWeight);
                    bDone = bDone && (pFS[iLoc].tm.tmItalic==tmTmp.tmItalic);
                    bDone = bDone && (pFS[iLoc].tm.tmUnderlined==tmTmp.tmUnderlined);
                    bDone = bDone && (pFS[iLoc].tm.tmStruckOut==tmTmp.tmStruckOut);
                    bDone = bDone && (pFS[iLoc].tm.tmFirstChar==tmTmp.tmFirstChar);
                    bDone = bDone && (pFS[iLoc].tm.tmLastChar==tmTmp.tmLastChar);
                    bDone = bDone && (pFS[iLoc].tm.tmDefaultChar==tmTmp.tmDefaultChar);
                    bDone = bDone && (pFS[iLoc].tm.tmBreakChar==tmTmp.tmBreakChar);
                    bDone = bDone && (pFS[iLoc].tm.tmPitchAndFamily==tmTmp.tmPitchAndFamily);
                    bDone = bDone && (pFS[iLoc].tm.tmCharSet==tmTmp.tmCharSet);
                    // Did it work?
                    if (bDone) {
                        *lplf = lfDlg;
                        //EndDialog(hwnd, TRUE);
                        //return (TRUE);
                    } else {
                        // Again, font verification code is'nt quite up to
                        // snuff yet, so just Select the font anyway:
                        *lplf = lfDlg;

                        // ...and this is what we want to do once we fix the
                        // font verification code:
                        //lfDlg = *lplf;
                        //MessageBox (GetFocus(),
                        //    "Unable to re-create font from TextMetrics",
                        //    "FontView", MB_OK);
                    }


                                GlobalUnlock (pFL->hList);
                            }
                        }
                        GlobalUnlock (hFonts);
                    }


                    // Now set our master lf to this value. This way, we 'could' verify that the
                    // LOGFONT structure will in fact select this particular font, we just aren't yet
                    // *lplf = lfDlg;

                    pFL = (LPFONTLIST)GlobalLock (hFonts);
                    if (pFL) {
                        if (GlobalFree (pFL->hList)) {
                            MessageBox (GetFocus(), "Failed To Free", "pFL->hList", MB_OK);
                        } else {
                            GlobalUnlock (hFonts);
                            if (GlobalFree (hFonts)) {
                                MessageBox (GetFocus(), "Failed To Free", "hFonts", MB_OK);
                            }
                        }
                    }
                    EndDialog(hwnd, TRUE);
                    return (bDone);

                case IDCANCEL:
                    pFL = (LPFONTLIST)GlobalLock (hFonts);
                    if (pFL) {
                        if (GlobalFree (pFL->hList)) {
                            MessageBox (GetFocus(), "Failed To Free", "pFL->hList", MB_OK);
                        } else {
                            GlobalUnlock (hFonts);
                            if (GlobalFree (hFonts)) {
                                MessageBox (GetFocus(), "Failed To Free", "hFonts", MB_OK);
                            }
                        }
                    }
                    EndDialog(hwnd, TRUE);
                    return (TRUE);
            }
            break;
    }
    return (FALSE);

    lParam; // unreferenced formal parameter
}
