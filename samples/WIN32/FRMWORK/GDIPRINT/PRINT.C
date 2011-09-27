// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: print.c
//
//  PURPOSE: Handle the application specific printing commands based on
//    parameters gathered from the common printer dialog box.
//
//  FUNCTIONS:  
//    CmdPrint      - Handles the File Print command.
//    CmdPageSetup  - Handles the File PageSetup command.                
//    CmdPrintSetup - Handles the File PrintSetup command. 
//    Print         - Proceses printing request.
//    PrintDIB      - Prints a DIB.
//    CalculatePrintRect - Calculates the output rectangle on printer.
//    AbortProc     - Processes messages for printer abort dialog box.
//    AbortDlg      - Processes messages for application's AbortDlg dialog box.
//    PageSetupProc - Displays dialog for page setup options.
//
//
//  COMMENTS:
//
//  SPECIAL INSTRUCTIONS: N/A
//


#include <windows.h>            // required for all Windows applications 
#include <windowsx.h> 
           
#include <string.h>
           
#include "globals.h"            // prototypes specific to this application
#include "dibutil.h"
#include "statbar.h"
#include "print.h"
#include "resource.h"

void Print(HWND, HDC, BOOL, BOOL, BOOL, BOOL, UINT, HGLOBAL);
WORD PrintDIB(HDC, WORD, WORD, WORD, LPSTR);            
BOOL CALLBACK AbortProc(HDC, int);
LRESULT CALLBACK AbortDlg(HWND, UINT, WPARAM, LPARAM);
void CalculatePrintRect(HDC, LPRECT, WORD, DWORD, DWORD);
LRESULT CALLBACK PageSetupProc(HWND, UINT, WPARAM, LPARAM);
              
#define MAKEBOOL(i)      ((BOOL) !!(i))  
#define PW_BESTFIT       1
#define PW_SCALE         2
#define PW_STRETCHTOPAGE 3

HANDLE hDevMode = NULL;
HANDLE hDevNames = NULL;                     
HWND hdlgAbort = NULL;   // Abort dialog hanlde
BOOL fAbort = FALSE;     // Abort Flag     
OPTIONSTRUCT PageSetupOptions;

// buffer for string resources
char szBuffer[50];              // watch out for recursive use of this buffer!
   
  
//  FUNCTION: CmdPrint(HWND, WORD, WORD, HWND)
//
//  PURPOSE:  Present the print dialog to the user and process the results.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - WM_COMMAND
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//   
//

LRESULT CmdPrint(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) 
{    
    PRINTDLG pd = {0};
    static DWORD Flags = PD_ALLPAGES | PD_SHOWHELP | PD_RETURNDC;    
    static WORD nCopies = 1;
    int cbWritten = 0; 
   
    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0)
        lstrcpy(szBuffer, "Unknown Command");
        
    UpdateStatusBar(szBuffer, 0, 0);
 
    pd.lStructSize = sizeof(pd);
    pd.hwndOwner   = hwnd;
    pd.hDevMode    = hDevMode;
    pd.hDevNames   = hDevNames;
    pd.Flags       = Flags;
    pd.nFromPage   = 1;
    pd.nToPage     = 1;
    pd.nMinPage    = 1;
    pd.nMaxPage    = 1;
    pd.nCopies     = nCopies;

    if (PrintDlg(&pd))
    {       
        Print(hwnd,
              pd.hDC,
              MAKEBOOL(pd.Flags & PD_PAGENUMS),
              MAKEBOOL(pd.Flags & PD_SELECTION),
              MAKEBOOL(pd.Flags & PD_COLLATE),
              MAKEBOOL(pd.Flags & PD_PRINTTOFILE),             
              pd.nCopies,
              pd.hDevNames);
              
        hDevMode   = pd.hDevMode;
        hDevNames  = pd.hDevNames;
        Flags      = pd.Flags;       
    }

    // once command is executed, set the statusbar text to original
    UpdateStatusBar(SZDESCRIPTION, 0, 0);  
    
    return 0;
}
   
//  FUNCTION: CmdPageSetup(HWND, WORD, WORD, HWND)
//
//  PURPOSE:  Present dialog with page setup options.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - WM_COMMAND
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    The options provided allow the output to be scaled in
//    different ways.
//

LRESULT CmdPageSetup(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) 
{      
    OPTIONSTRUCT Options;       // temp copy of options set in dialog
    int iReturn;
    int  cbWritten = 0;

    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0)
        lstrcpy(szBuffer, "Unknown Command");
        
    UpdateStatusBar(szBuffer, 0, 0);
   
    // get page setup options
    iReturn = DialogBoxParam(hInst, 
                             "PageSetupBox", 
                             hwnd, 
                             (DLGPROC)PageSetupProc,
                             (LPARAM)&Options);   
                             
    if (iReturn)
        // user did not cancel dialog so save option settings
        PageSetupOptions = Options;

    // once command is executed, set the statusbar text to original
    UpdateStatusBar(SZDESCRIPTION, 0, 0);
    return 0;
}

   
//  FUNCTION: CmdPrintSetup(HWND, WORD, WORD, HWND)
//
//  PURPOSE:  Present the print setup common dialog to the user.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - WM_COMMAND
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    Assumes there is a resource string describing this command with the
//    same ID as the command ID.  Loads the string and calls UpdateStatusBar
//    to put the string into main pane of the status bar.


LRESULT CmdPrintSetup(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) 
{         
    PRINTDLG pd = {0};
    int  cbWritten = 0;

    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0)
        lstrcpy(szBuffer, "Unknown Command");
        
    UpdateStatusBar(szBuffer, 0, 0);

    pd.lStructSize = sizeof(pd);
    pd.hwndOwner   = hwnd;
    pd.hDevMode    = hDevMode;
    pd.hDevNames   = hDevNames;
    pd.Flags = PD_SHOWHELP | PD_PRINTSETUP;

    if (PrintDlg(&pd))
    {
        hDevMode = pd.hDevMode;
        hDevNames = pd.hDevNames;
    }
    
    // once command is executed, set the statusbar text to original
    UpdateStatusBar(SZDESCRIPTION, 0, 0);
    
    return 0;
}


//
//  FUNCTION:
//    Print(HWND, HDC, BOOL, BOOL, BOOL, BOOL, UINT, HGLOBAL)
//
//  PURPOSE: To print a DIB based on information from the
//    common print dialog.
//
//
//  PARAMETERS:
//    hwnd - The window that the print message came from.
//    hdc - The display context to print to.
//    fPageNums - Print the page range specified by n[From|To]Page.
//    fSelection - Print the current selection.
//    fCollate - Print the pages in a collated order.
//    fFile - Print to a file.
//    uCopies - Number of copies to print
//    hDevNames - Handle to DEVNAMES structure.
//
//  RETURN VALUE:
//    None.
//
//  COMMENTS:
//
//

VOID Print(HWND hwnd,
           HDC  hdc,
           BOOL fPageNums,
           BOOL fSelection,
           BOOL fCollate,
           BOOL fFile,     
           UINT uCopies,
           HGLOBAL hDevNames)
{
    HCURSOR hcurSave;
    DOCINFO di;
    WORD wErrCode;   
    UINT u;             // printing loop counter

    hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
  
    // Define the abort function
    SetAbortProc(hdc, AbortProc);

    // Start the printing session
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = "GDIPRINT";
    di.lpszOutput = NULL;
    if (StartDoc(hdc, &di) < 0)
    {
        // StartDoc failed so cancel print job
                         
        // remove hourglass cursor                   
        SetCursor(hcurSave); 
                                 
        // tell the user        
        MessageBox(hwnd, 
                   "Unable to start print job", 
                   SZAPPNAME, 
                   MB_OK | MB_ICONHAND); 
                         
        DeleteDC(hdc);
        return;
    }
           
    // clear the abort flag           
    fAbort = FALSE; 

    // Create the AbortDlg dialog box (modeless)
    hdlgAbort = CreateDialog(hInst, "AbortDlg", hwnd, AbortDlg);
    if (!hdlgAbort)
    {   
        // failed to create abort dialog so cancel print job
                    
        // remove hourglass cursor                   
        SetCursor(hcurSave); 
        
        // tell the user  
        MessageBox(hwnd, 
                   "NULL Abort window handle", 
                   SZAPPNAME, 
                   MB_OK | MB_ICONHAND);   
                   
        DeleteDC(hdc);
        return;
    }

    // Now show Abort dialog
    ShowWindow(hdlgAbort, SW_NORMAL);

    // Disable the main window to avoid reentrancy problems
    EnableWindow(hwnd, FALSE); 
    
    // remove hourglass cursor
    SetCursor(hcurSave);     
    
    for (u = 0; u < uCopies; u++)
    {
        StartPage(hdc);
    
        wErrCode = PrintDIB(hdc, 
                            (WORD)PageSetupOptions.iOption, 
                            (WORD)PageSetupOptions.iXScale, 
                            (WORD)PageSetupOptions.iYScale, 
                            (LPSTR)(di.lpszDocName));
        EndPage(hdc);    
        
        if (wErrCode)
        {
            DIBError(wErrCode);
            break;
        }
    }
    
    EndDoc(hdc);
           
    EnableWindow(hwnd, TRUE);

    // Destroy the Abort dialog box
    DestroyWindow(hdlgAbort);   
    DeleteDC(hdc);
}

                                                
//
//  FUNCTION:
//    PrintDIB(HDC, WORD, WORD, WORD, LPSTR)
//
//  PURPOSE: To print a DIB based on information from the
//    common print dialog.
//
//
//  PARAMETERS:
//    hPrnDC - Printer device context
//    fPrintOpt - Print Options
//    wXScale - X Scaling factor
//    wYScale - Y Scaling factor
//    szJobName - Name of print job
// 
//
//  RETURN VALUE:
//    0 if successful, else an error code.
//
//  COMMENTS:
//
//

WORD PrintDIB(HDC hPrnDC,
              WORD fPrintOpt, 
              WORD wXScale, 
              WORD wYScale, 
              LPSTR szJobName)
{                                                   
    LPBITMAPINFOHEADER lpDIBHdr; // Pointer to DIB header
    RECT rcClip;                 // Rect structure used for banding  
    RECT rcPrint;                // Rect which specifies the area on the printer
                                 // (in printer coordinates) which we
                                 // want the DIB to go to
    RECT rcSrc, rcDest;          // rectangles for StretchDIBits;                                 
    double dblXScaling;          // X scaling factor
    double dblYScaling;          // Y scaling factor  
    int iReturn;
    WORD wErrorCode = 0;         // Error code to return
          
    lpDIBHdr = (LPBITMAPINFOHEADER)GlobalLock(hDIBInfo);
    if (!lpDIBHdr) 
        return ERR_LOCK;
  
    SetStretchBltMode(hPrnDC, COLORONCOLOR);
     
    // Determine rcPrint (printer area to print to) from the
    // fPrintOpt.  Fill in rcPrint.left and .top from wXScale and
    // wYScale just in case we use PW_SCALE (see the function
    // CalculatePrintRect).  
    rcPrint.left = wXScale;
    rcPrint.top = wYScale;
    CalculatePrintRect(hPrnDC, &rcPrint, fPrintOpt, lpDIBHdr->biWidth,
                         lpDIBHdr->biHeight);
    
    // print the entire DIB     
    rcClip = rcPrint;       

    dblXScaling = ((double)RECTWIDTH(&rcPrint)) / (double)lpDIBHdr->biWidth;
    dblYScaling = ((double)RECTHEIGHT(&rcPrint)) / (double)lpDIBHdr->biHeight;

    // Create destination rectangle in device coordinates
    IntersectRect(&rcDest, &rcPrint, &rcClip);
    if (IsRectEmpty(&rcDest))
    {
        // nothing to print
        wErrorCode = ERR_UNDEFINEDERROR;
        goto ErrExit;
    }
    
    // determine coordinates in DIB that will be printed    
    rcSrc.left = (int)((rcDest.left - rcPrint.left) / dblXScaling + 0.5);
    rcSrc.top  = (int)((rcDest.top - rcPrint.top) / dblYScaling + 0.5);
    rcSrc.right = (int)((rcSrc.left + RECTWIDTH(&rcDest)) / dblXScaling + 0.5);
    rcSrc.bottom = (int)((rcSrc.top + RECTHEIGHT(&rcDest)) / dblYScaling + 0.5);
                        
    iReturn = StretchDIBits(hPrnDC,                           // DestDC
                            rcDest.left,                      // DestX
                            rcDest.top,                       // DestY
                            RECTWIDTH(&rcDest),               // DestWidth
                            RECTHEIGHT(&rcDest),              // DestHeight
                            rcSrc.left,                       // SrcX
                            (int)(lpDIBHdr->biHeight) -       // SrcY
                            rcSrc.top - (RECTHEIGHT(&rcSrc)),
                            RECTWIDTH(&rcSrc),                // SrcWidth
                            RECTHEIGHT(&rcSrc),               // SrcHeight
                            lpvBits,                          // lpBits
                            (LPBITMAPINFO)lpDIBHdr,           // lpBitInfo
                            DIB_RGB_COLORS,                   // wUsage
                            SRCCOPY);                         // dwROP
    
    if (iReturn == 0)                       
        // StretchDIBits() failed
        wErrorCode = ERR_STRETCHDIBITS; 
             
ErrExit:                        
    GlobalUnlock(hDIBInfo);                          

    return (wErrorCode);
}

 
//
//  FUNCTION:
//    CalculatePrintRect(HDC, LPRECT, WORD, DWORD, DWORD)   
//
//  PURPOSE: Returns area on printer where image will be printed.
//
//  PARAMETERS:
//    hDC - Handle of printer device context.
//    lprcPrint - address of rectangle filled in by this function.
//    fPageSetup - page setup option flag.
//    cxDIB - width of DIB in pixels
//    cyDIB - height of DIB in pixels
//
//  RETURN VALUE:
//    Depends on the message number.
//
//  COMMENTS:
//    Given fPageSetup and a size of the DIB, return the area on the
//    printer where the image should go (in printer coordinates).  If
//    fPageSetup is PW_SCALE, then lprcPrint.left and .top should
//    contain WORDs which specify the scaling factor for the X and
//    Y directions, respecively.
//

void CalculatePrintRect(HDC hDC, 
                        LPRECT lprcPrint, 
                        WORD fPageSetup, 
                        DWORD cxDIB, 
                        DWORD cyDIB)    
{
    int cxPage, cyPage, cxInch, cyInch;

    if (!hDC)
        return;
   
    // Get some info from printer driver
    cxPage = GetDeviceCaps(hDC, HORZRES);    // Width of printr page - pixels
    cyPage = GetDeviceCaps(hDC, VERTRES);    // Height of printr page - pixels
    cxInch = GetDeviceCaps(hDC, LOGPIXELSX); // Printer pixels per inch - X
    cyInch = GetDeviceCaps(hDC, LOGPIXELSY); // Printer pixels per inch - Y
    
    switch (fPageSetup)
    {
        case PW_BESTFIT:
            // Best Fit case -- create a rectangle which preserves
            // the DIB's aspect ratio, and fills the page horizontally.
            // The formula in the "->bottom" field below calculates the Y
            // position of the printed bitmap, based on the size of the
            // bitmap, the width of the page, and the relative size of
            // a printed pixel (cyInch / cxInch).
            
            lprcPrint->top = 0;
            lprcPrint->left = 0;
            lprcPrint->bottom = 
                (int)(((double)cyDIB * cxPage * cyInch) / 
                ((double)cxDIB * cxInch));
            lprcPrint->right = cxPage;   
            
            break;

        case PW_SCALE:
        {               
            // Scaling option -- lprcPrint's top/left contain
            // multipliers to multiply the DIB's height/width by.
    
            int cxMult, cyMult;

            cxMult = lprcPrint->left;
            cyMult = lprcPrint->top;
            lprcPrint->top = 0;
            lprcPrint->left = 0;
            lprcPrint->bottom = (int)(cyDIB * cyMult);
            lprcPrint->right = (int)(cxDIB * cxMult); 
            
            break;
        }

        case PW_STRETCHTOPAGE:
        default:          
            // Stretch To Page case -- create a rectangle
            // which covers the entire printing page (note that this
            // is also the default).
            
            lprcPrint->top = 0;
            lprcPrint->left = 0;
            lprcPrint->bottom = cyPage;
            lprcPrint->right = cxPage;
                
            break;
    }
}
                     
                     
//
//  FUNCTION: AbortProc(HDC, int)
//
//  PURPOSE: Processes messages for the printer abort dialog box.
//
//  PARAMETERS:
//    hdc - The printer device context.
//    Code -  The error code (unused).
//
//  RETURN VALUE:
//    Returns FALSE if the user has aborted, TRUE otherwise.
//
//  COMMENTS:
//
//

BOOL CALLBACK AbortProc(HDC hdc, int Code)
{
    MSG msg;

    if (!hdlgAbort)              
        // the abort dialog isn't up yet
        return TRUE;

    // Process messages intended for the abort dialog box

    while (!fAbort && PeekMessage(&msg, NULL, 0, 0, TRUE))
        if (!IsDialogMessage(hdlgAbort, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    // fAbort is TRUE (return is FALSE) if the user has aborted

    return !fAbort;
}

//
//  FUNCTION: AbortDlg(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for application's AbortDlg dialog box.
//
//  PARAMETERS:
//    hdlg     - The dialog handle.
//    uMessage - The message number.
//    wparam   - Message specific data.
//    lparam   - Message specific data.
//
//  RETURN VALUE:
//    Depends on the message number.
//
//  COMMENTS:
//      This dialog box is created while the program is printing, and allows
//      the user to cancel the printing process.
//

LRESULT CALLBACK AbortDlg(HWND hdlg, UINT uMessage, 
    WPARAM wparam, LPARAM lparam)
{
    switch(uMessage)
    {
        // watch for Cancel button, RETURN key, ESCAPE key, or SPACE BAR

        case WM_COMMAND:
            return fAbort = TRUE;

        case WM_INITDIALOG:
            // center the dialog over the application window
            CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));
    
            // set the focus to the Cancel box of the dialog
            SetFocus(GetDlgItem(hdlg, IDCANCEL));
            
            // identify the file being printed
            SetDlgItemText(hdlg, IDD_FILENAME, (LPCTSTR)szCurrentFile);
            
            return TRUE;
    }

    return FALSE;
}


//
//  FUNCTION: PageSetupProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Provides page setup options.
//
//  PARAMETERS:
//    hdlg     - The dialog handle.
//    uMessage - The message number.
//    wparam   - Message specific data.
//    lparam   - Message specific data.
//
//  RETURN VALUE:
//    Depends on the message number.
//
//  COMMENTS:
//  
//

LRESULT CALLBACK PageSetupProc(HWND hdlg,
                               UINT uMessage, 
                               WPARAM wparam,
                               LPARAM lparam)
{
    static LPOPTIONSTRUCT lpOS;

    switch (uMessage)
    {
        case WM_INITDIALOG:
        {      
            // Center the dialog over the application window
            CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

            // Because DialogBoxParam() was used to invoke this dialog box,
            // lparam contains a pointer to the OPTIONSTRUCT.
            // Place user input in this structure before returning.
            lpOS = (LPOPTIONSTRUCT)lparam;

            // Check the default button -- "BEST FIT"
            CheckRadioButton(hdlg, IDD_BESTFIT, IDD_STRETCHTOPAGE, IDD_BESTFIT);

            // Gray out the stuff under "SCALE"
            EnableWindow(GetDlgItem(hdlg, IDD_XAXIS), FALSE);
            EnableWindow(GetDlgItem(hdlg, IDD_YAXIS), FALSE);
            EnableWindow(GetDlgItem(hdlg, IDD_XTEXT), FALSE);
            EnableWindow(GetDlgItem(hdlg, IDD_YTEXT), FALSE);
            break;
        }

        // Closing the Dialog should behave the same as Cancel

        case WM_CLOSE:        
            PostMessage(hdlg, WM_COMMAND, IDCANCEL, 0L);
            break;

        case WM_COMMAND:
            switch (wparam)
            {
                case IDD_BESTFIT:
                case IDD_STRETCHTOPAGE:
                case IDD_SCALE:

                    // Check the correct button
                    CheckRadioButton(hdlg, IDD_BESTFIT, IDD_SCALE, wparam);

                    // And enable or disable the options under "Scale",
                    // depending on whether or not the IDD_SCALE button
                    // is checked
                    EnableWindow(GetDlgItem(hdlg, IDD_XAXIS),
                            (BOOL)(wparam == IDD_SCALE));
                    EnableWindow(GetDlgItem(hdlg, IDD_YAXIS),
                            (BOOL)(wparam == IDD_SCALE));
                    EnableWindow(GetDlgItem(hdlg, IDD_XTEXT),
                            (BOOL)(wparam == IDD_SCALE));
                    EnableWindow(GetDlgItem(hdlg, IDD_YTEXT),
                            (BOOL)(wparam == IDD_SCALE));
                    break;

                case IDOK:
                {
                    char    szTmp[100];
                    BOOL fTranslated;

                    // Save the user's selection into the OPTIONSTRUCT
                    if (!lpOS)
                    {
                        EndDialog(hdlg, FALSE);
                        break;
                    }

                    if (IsDlgButtonChecked(hdlg, IDD_BESTFIT))
                        lpOS->iOption = IDD_BESTFIT;

                    if (IsDlgButtonChecked(hdlg, IDD_STRETCHTOPAGE))
                        lpOS->iOption = IDD_STRETCHTOPAGE;

                    if (IsDlgButtonChecked(hdlg, IDD_SCALE))
                        lpOS->iOption = IDD_SCALE;   

                    if (GetDlgItemText(hdlg, IDD_XAXIS, (LPSTR)szTmp, sizeof(szTmp)))
                        lpOS->iXScale = GetDlgItemInt(hdlg, IDD_XAXIS, &fTranslated, TRUE);

                    if (GetDlgItemText(hdlg, IDD_YAXIS, (LPSTR)szTmp, sizeof(szTmp)))
                        lpOS->iYScale = GetDlgItemInt(hdlg, IDD_YAXIS, &fTranslated, TRUE);

                    EndDialog(hdlg, TRUE);
                    break;
                }
         

                case IDCANCEL:
                    EndDialog(hdlg, FALSE);
                    break;

                break;
            } // End of WM_COMMAND
                

        default:
            return FALSE;
    }

    return TRUE;
}

