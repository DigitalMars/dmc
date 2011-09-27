#include <Windows.h>
#include "Syshook.H"

extern HINSTANCE hDllInstance;

#define     EDIT_CLASS          1
#define     BUTTON_CLASS        2
#define     LISTBOX_CLASS       3
#define     COMBOBOX_CLASS      4
#define     STATIC_CLASS        5
#define     SCROLLBAR_CLASS     6


//
//  FUNCTION:   DecodeClipboardFormat
//
//  PURPOSE:    Decode the clipboard format id and return a string
//              with the name of the clipboard
//
//  PARAMETERS:
//
//      uFormat     - Clipboard format Id
//      pszBuff     - Pointer to char buffer that will receive the name
//                    of the clipboard
//
//  RETURN VALUE:
//
//      none
//
//  COMMENTS:
//
//      The pointer to the string buffer assume that they point to a memory
//      block allocated by the calling function
//
void WINAPI DecodeClipboardFormat (UINT uFormat, LPSTR pszBuff)
{
    switch (uFormat)
    {
        case CF_TEXT:
            lstrcpy (pszBuff, "Clipboard Format: CF_TEXT");
            break;
        case CF_BITMAP:
            lstrcpy (pszBuff, "Clipboard Format: CF_BITMAP");
            break;
        case CF_METAFILEPICT:
            lstrcpy (pszBuff, "Clipboard Format: CF_METAFILEPICT");
            break;
        case CF_SYLK:
            lstrcpy (pszBuff, "Clipboard Format: CF_SYLK");
            break;
        case CF_DIF:
            lstrcpy (pszBuff, "Clipboard Format: CF_DIF");
            break;
        case CF_TIFF:
            lstrcpy (pszBuff, "Clipboard Format: CF_TIFF");
            break;
        case CF_OEMTEXT:
            lstrcpy (pszBuff, "Clipboard Format: CF_OEMTEXT");
            break;
        case CF_DIB:
            lstrcpy (pszBuff, "Clipboard Format: CF_DIB");
            break;
        case CF_PALETTE:
            lstrcpy (pszBuff, "Clipboard Format: CF_PALETTE");
            break;
        case CF_PENDATA:
            lstrcpy (pszBuff, "Clipboard Format: CF_PENDATA");
            break;
        case CF_RIFF:
            lstrcpy (pszBuff, "Clipboard Format: CF_RIFF");
            break;
        case CF_WAVE:
            lstrcpy (pszBuff, "Clipboard Format: CF_WAVE");
            break;
        case CF_OWNERDISPLAY:
            lstrcpy (pszBuff, "Clipboard Format: CF_OWNERDISPLAY");
            break;
        case CF_DSPTEXT:
            lstrcpy (pszBuff, "Clipboard Format: CF_DSPTEXT");
            break;
        case CF_DSPBITMAP:
            lstrcpy (pszBuff, "Clipboard Format: CF_DSPBITMAP");
            break;
        case CF_DSPMETAFILEPICT:
            lstrcpy (pszBuff, "Clipboard Format: CF_DSPMETAFILEPICT");
            break;
        default:
            if (uFormat >= CF_PRIVATEFIRST && uFormat <= CF_PRIVATELAST)
                lstrcpy (pszBuff, "Private Clipboard Format");
            else
                if (uFormat >= CF_GDIOBJFIRST && uFormat <= CF_GDIOBJLAST)
                    lstrcpy (pszBuff, "Clipboard: GDI Object");
                else
                    lstrcpy (pszBuff, "(Unknown Clipboard Format)");
    }
}

//
//  FUNCTION:   DecodeControlNotification
//
//  PURPOSE:    Decode the control notification sent to a control parent
//
//  PARAMETERS:
//
//      iClassType  - id of the control class
//      wParam      - wParam of message
//      lParam      - lParam of message
//      pszWParam   - Pointer to a string buffer
//      pszLParam   - Pointer to a string buffer
//
//  RETURN VALUE:
//
//      none
//
//  COMMENTS:
//
//      The pointer to the string buffer assume that they point to a memory
//      block allocated by the calling function
//
void WINAPI DecodeControlNotification (int iClassType, 
                                       WPARAM wParam,
                                       LPARAM lParam,
                                       LPSTR pszWParam,
                                       LPSTR pszLParam)
{
    lstrcpy (pszWParam, "Control Notification Message");
    wsprintf (pszLParam, 
              "Control ID: %d  Control HWND %#08x\t",
              LOWORD (wParam));
}

//
//  FUNCTION:   DecodeHitTestCode
//
//  PURPOSE:    Decode the hit test code and return a string holding it
//
//  PARAMETERS:
//
//      uCode   - Hit test code id
//      pszBuff - Pointer to a string buffer
//
//  RETURN VALUE:
//
//      none
//
//  COMMENTS:
//
//      The pointer to the string buffer assume that they point to a memory
//      block allocated by the calling function
//
void WINAPI DecodeHitTestCode (UINT uCode, LPSTR pszBuff)
{
    switch (uCode)
    {
        case HTERROR:
            lstrcpy (pszBuff, "Hit Test: HTERROR");
            break;
        case HTTRANSPARENT:
            lstrcpy (pszBuff, "Hit Test: HTTRANSPARENT"); 
            break;
        case HTNOWHERE:
            lstrcpy (pszBuff, "Hit Test: HTNOWHERE");
            break;
        case HTCLIENT:      
            lstrcpy (pszBuff, "Hit Test: HTCLIENT");
            break;
        case HTCAPTION:
            lstrcpy (pszBuff, "Hit Test: HTCAPTION");
            break;
        case HTSYSMENU:     
            lstrcpy (pszBuff, "Hit Test: HTSYSMENU");
            break;
        case HTSIZE:        
            lstrcpy (pszBuff, "Hit Test: HTSIZE");
            break;
        case HTMENU:        
            lstrcpy (pszBuff, "Hit Test: HTMENU");
            break;
        case HTHSCROLL:     
            lstrcpy (pszBuff, "Hit Test: HTHSCROLL");
            break;
        case HTVSCROLL:     
            lstrcpy (pszBuff, "Hit Test: HTVSCROLL");
            break;
        case HTREDUCE:      
            lstrcpy (pszBuff, "Hit Test: HTREDUCE");
            break;
        case HTZOOM:        
            lstrcpy (pszBuff, "Hit Test: HTZOOM");
            break;
        case HTLEFT:        
            lstrcpy (pszBuff, "Hit Test: HTLEFT");
            break;
        case HTRIGHT:       
            lstrcpy (pszBuff, "Hit Test: HTRIGHT");
            break;
        case HTTOP:         
            lstrcpy (pszBuff, "Hit Test: HTTOP");
            break;
        case HTTOPLEFT:     
            lstrcpy (pszBuff, "Hit Test: HTTOPLEFT");
            break;
        case HTTOPRIGHT:    
            lstrcpy (pszBuff, "Hit Test: HTTOPRIGHT");
            break;
        case HTBOTTOM:      
            lstrcpy (pszBuff, "Hit Test: HTBOTTOM");
            break;
        case HTBOTTOMLEFT:  
            lstrcpy (pszBuff, "Hit Test: HTBOTTOMLEFT");
            break;
        case HTBOTTOMRIGHT: 
            lstrcpy (pszBuff, "Hit Test: HTBOTTOMRIGHT");
            break;
        default:
            lstrcpy (pszBuff, "(Unknown Hit Code)");
    }
}

//
//  FUNCTION:   DecodeKeyData
//
//  PURPOSE:    Decode the key data for key-messages
//
//  PARAMETERS:
//
//      lParam  - Lparam holding key data
//      pszBuff - Pointer to a string buffer
//
//  RETURN VALUE:
//
//      none
//
//  COMMENTS:
//
//      The pointer to the string buffer assume that they point to a memory
//      block allocated by the calling function
//
void WINAPI DecodeKeyData (LPARAM lParam, LPSTR pszBuff)
{
    wsprintf (pszBuff, 
              "Scan Code: %d Repeat Count: %d", 
              LOBYTE(HIWORD(lParam)), 
              LOWORD(lParam));
}

//
//  FUNCTION:   DecodeMessageParams
//
//  PURPOSE:    Decode all windows messages and return a string for each
//              parameter
//
//  PARAMETERS:
//
//      hWnd        - Window for whom the message is intended
//      message     - Message id
//      wParam      - Wparam of message
//      lParam      - Lparam of message
//      pszHwnd     - Pointer for string buffer
//      pszMessage  - Pointer for string buffer
//      pszWParam   - Pointer for string buffer
//      pszLParam   - Pointer for string buffer
//
//  RETURN VALUE:
//
//      none
//
//  COMMENTS:
//
//      The pointer to the string buffer assume that they point to a memory
//      block allocated by the calling function
//
void WINAPI DecodeMessageParams (HWND hWnd, 
                                 UINT message,
                                 WPARAM wParam,
                                 LPARAM lParam,
                                 LPSTR pszHwnd,
                                 LPSTR pszMessage,
                                 LPSTR pszWParam,
                                 LPSTR pszLParam)
{
    GetClassName (hWnd, pszHwnd, HWND_STRING_SIZE);
    if (0 == LoadString (hDllInstance, message, pszMessage, MESSAGE_STRING_SIZE))
    {
        wsprintf (pszMessage, "Unknown: %#08x\t", message);
    }
#define NO_WPARAM lstrcpy (pszWParam, "Parameter Unused\t")
#define NO_LPARAM lstrcpy (pszLParam, "Parameter Unused\t")
    switch (message)
    {
        case WM_NULL : 
        case WM_DESTROY :
        case WM_PAINT :
        case WM_TIMECHANGE :
        case WM_QUERYNEWPALETTE :
        case WM_CHILDACTIVATE :
        case WM_CLOSE :
        case WM_GETTEXTLENGTH :
        case WM_QUERYOPEN :
        case WM_CANCELMODE :
        case WM_GETFONT :
        case WM_QUERYDRAGICON :
        case WM_GETDLGCODE :
        case WM_PAINTICON :
        case WM_GETHOTKEY :
        case WM_SYSCOLORCHANGE :
        case WM_QUEUESYNC :
        case WM_FONTCHANGE :
        case WM_MDIICONARRANGE :
        case WM_MDIREFRESHMENU :
        case WM_CUT :
        case WM_COPY :
        case WM_PASTE :
        case WM_CLEAR :
        case WM_UNDO :
        case WM_NCDESTROY :
        case WM_RENDERALLFORMATS :
        case WM_DESTROYCLIPBOARD :
        case WM_DRAWCLIPBOARD :
        case BM_GETCHECK :
        case BM_GETSTATE :
            NO_WPARAM;
            NO_LPARAM;
            break;

        case WM_NCCREATE :
        case WM_CREATE :
            NO_WPARAM;
            wsprintf (pszLParam, "CREATESTRUCT: %#08x\t", lParam);
            return;

        case WM_MOVE :
            NO_WPARAM;
            wsprintf (pszLParam, 
                      "X-Pos: %-5d  Y-Pos: %-5d\t",
                      LOWORD(lParam),
                      HIWORD(lParam));
            break;
            
        case WM_SIZE :
            switch (wParam)
            {
                case SIZE_MAXIMIZED :
                    lstrcpy (pszWParam, "SIZE_MAXIMIZED\t");
                    break;
                case SIZE_MINIMIZED :
                    lstrcpy (pszWParam, "SIZE_MINIMIZED\t");
                    break;
                case SIZE_RESTORED :
                    lstrcpy (pszWParam, "SIZE_RESTORED\t");
                    break;
                case SIZE_MAXHIDE :
                    lstrcpy (pszWParam, "SIZE_MAXHIDE\t");
                    break;
                case SIZE_MAXSHOW :
                    lstrcpy (pszWParam, "SIZE_MAXSHOW\t");
                    break;
                default :
                    lstrcpy (pszWParam, "(Unknown Flag)\t");
                    break;
            }
            wsprintf (pszLParam,
                      "Width: %d, Height: %d\t",
                      LOWORD(lParam), 
                      HIWORD(lParam));
            break;

        case WM_ACTIVATE :
            switch (LOWORD(wParam))
            {
                case WA_ACTIVE :
                    lstrcpy (pszWParam, "WA_ACTIVE");
                    break;
                case WA_CLICKACTIVE :
                    lstrcpy (pszWParam, "WA_CLICKACTIVE");
                    break;
                case WA_INACTIVE :
                    lstrcpy (pszWParam, "WA_INACTIVE");
                    break;
                default :
                    lstrcpy (pszWParam, "(Unknown Flag)");
                    break;
            }
            if (HIWORD(wParam))
                lstrcat (pszWParam, " - Minimized\t");
            else
                lstrcat (pszWParam, " - Not Minimized\t");
            wsprintf (pszLParam, "HWND: %#08x\t", lParam);
            break;

        case WM_NCACTIVATE :
            if (wParam)
                lstrcpy (pszWParam, "Redraw Title Bar/Icon: ACTIVATE\t");
            else
                lstrcpy (pszWParam, "Redraw Title Bar/Icon: DEACTIVATE\t");
            break;
            
        case WM_SETFOCUS :
        case WM_KILLFOCUS :
            wsprintf (pszWParam, "Focus HWND: %#08x\t", wParam);
            NO_LPARAM;
            break;
        
        case WM_ENABLE :
            if (wParam)
                lstrcpy (pszWParam, "Window Enabled\t");
            else
                lstrcat (pszWParam, "Window Disabled\t");
            NO_LPARAM;
            break;

        case WM_ERASEBKGND :
            wsprintf (pszWParam, "HDC: %#08x\t", wParam);
            NO_LPARAM;
            break;

        case WM_WINDOWPOSCHANGING :
        case WM_WINDOWPOSCHANGED :
            NO_WPARAM;
            wsprintf (pszLParam, "WINDOWPOS: %#08x\t", lParam);
            break;

        case WM_TIMER :
            wsprintf (pszWParam, "Timer ID: %d\t", wParam);
            wsprintf (pszLParam, "TIMERPROC: %#08x\t", lParam);
            break;

        case WM_SYSCOMMAND :
            switch (wParam)
            {
                case SC_CLOSE :
                    lstrcpy (pszWParam, "SC_CLOSE\t");
                    break;
                case SC_HOTKEY :
                    lstrcpy (pszWParam, "SC_HOTKEY\t");
                    break;
                case SC_HSCROLL :
                    lstrcpy (pszWParam, "SC_HSCROLL\t");
                    break;
                case SC_KEYMENU :
                    lstrcpy (pszWParam, "SC_KEYMENU\t");
                    break;
                case SC_MAXIMIZE :
                    lstrcpy (pszWParam, "SC_MAXIMIZE (SC_ZOOM)\t");
                    break;
                case SC_MINIMIZE :
                    lstrcpy (pszWParam, "SC_MINIMIZE (SC_ICON)\t");
                    break;
                case SC_MOUSEMENU :
                    lstrcpy (pszWParam, "SC_MOUSEMENU\t");
                    break;
                case SC_MOVE :
                    lstrcpy (pszWParam, "SC_MOVE\t");
                    break;
                case SC_NEXTWINDOW :
                    lstrcpy (pszWParam, "SC_NEXTWINDOW\t");
                    break;
                case SC_PREVWINDOW :
                    lstrcpy (pszWParam, "SC_PREVWINDOW\t");
                    break;
                case SC_RESTORE :
                    lstrcpy (pszWParam, "SC_RESTORE\t");
                    break;
                case SC_SCREENSAVE :
                    lstrcpy (pszWParam, "SC_SCREENSAVE\t");
                    break;
                case SC_SIZE :
                    lstrcpy (pszWParam, "SC_SIZE\t");
                    break;
                case SC_TASKLIST :
                    lstrcpy (pszWParam, "SC_TASKLIST\t");
                    break;
                case SC_VSCROLL :
                    lstrcpy (pszWParam, "SC_VSCROLL\t");
                    break;
                default :
                    lstrcpy (pszWParam, "(Unknown System Key)\t");
                    break;
            }
            wsprintf (pszLParam,
                      "X-Pos: %-5d  Y-Pos: %-5d\t",
                      LOWORD(lParam), 
                      HIWORD(lParam));
            break;

        case WM_SETREDRAW :
            if (wParam)
                lstrcpy (pszWParam, "Redraw: TRUE\t");
            else
                lstrcpy (pszWParam, "Redraw: FALSE\t");
            NO_LPARAM;
            break;

        case WM_QUIT :
            wsprintf (pszWParam, "Exit Code: %d\t", wParam);
            NO_LPARAM;
            break;

        case WM_SETTEXT :
            NO_WPARAM;
            wsprintf (pszLParam, "LPSTR: %#08x\t", lParam);
            break;

        case WM_SHOWWINDOW :
            if (wParam)
                lstrcpy (pszWParam, "Show Flag: TRUE\t");
            else
                lstrcpy (pszWParam, "Show Flag: FALSE\t");
            switch (lParam)
            {
                case SW_PARENTCLOSING :
                    lstrcpy (pszLParam, "SW_PARENTCLOSING");
                    break;
                case SW_PARENTOPENING :
                    lstrcpy (pszLParam, "SW_PARENTOPENING\t");
                    break;
                default :
                    lstrcpy (pszLParam, "(Unknown Flag)\t");
                    break;
            }
            break;

        case WM_GETMINMAXINFO :
            NO_WPARAM;
            wsprintf (pszLParam, "MINMAXINFO: %#08x\t", lParam);
            break;

        case WM_ACTIVATEAPP :
            if (wParam)
                lstrcpy (pszWParam, "Thread Activated\t");
            else
                lstrcpy (pszWParam, "Thread Deactivated\t");
            wsprintf (pszLParam, "Thread ID: %#08x\t", lParam);
            break;

        case WM_QUERYENDSESSION :
            if (wParam)
                lstrcpy (pszWParam, "Requested by TASK MANAGER\t");
            else
                lstrcpy (pszWParam, "Requested for END SESSION\t");
            NO_LPARAM;
            break;

        case WM_GETTEXT :
            wsprintf (pszWParam, "Number of Chars: %d\t", wParam);
            wsprintf (pszLParam, "LPSTR: %#08x\t", lParam);
            break;
        
        case WM_ICONERASEBKGND :
            wsprintf (pszWParam, "HDC: %#08x\t", wParam);
            NO_LPARAM;
            break;

        case WM_NEXTDLGCTL :
            wsprintf (pszWParam, "wCtlFlag: %d\t", wParam);
            if (LOWORD(lParam))
                lstrcpy (pszLParam, "fHandle: TRUE\t");
            else
                lstrcpy (pszLParam, "fHandle: FALSE\t");
            break;

        case WM_SPOOLERSTATUS :
            wsprintf (pszWParam, "PR_JOBSTATUS Flag: %d\t", wParam);
            wsprintf (pszLParam, "Jobs Pending: %d\t", LOWORD(lParam));
            break;

        case WM_DRAWITEM :
            wsprintf (pszWParam, "Control ID: %d\t", wParam);
            wsprintf (pszLParam, "DRAWITEMSTRUCT: %#08x\t", lParam);
            break;

        case WM_MEASUREITEM :
            wsprintf (pszWParam, "Control ID: %d\t", wParam);
            wsprintf (pszLParam, "MEASUREITEMSTRUCT: %#08x\t", lParam);
            break;

        case WM_DELETEITEM :
            wsprintf (pszWParam, "Control ID: %d\t", wParam);
            wsprintf (pszLParam, "DELETEITEMSTRUCT: %#08x\t", lParam);
            break;
    
        case WM_CHARTOITEM :
        case WM_VKEYTOITEM :
            {
            char TmpBuffer[PARAM_STRING_SIZE];
            DecodeVirtualKey ((UINT)LOWORD(wParam), TmpBuffer);
            wsprintf (pszWParam, 
                      "Virtual Key: %s  Caret Position: %d\t",
                      TmpBuffer,
                      HIWORD(wParam));
            wsprintf (pszLParam, "HWND To ListBox: %#08x\t", lParam);
            }
            break;

        case WM_SETFONT :
            wsprintf (pszWParam, "HFONT: %#08x\t", wParam);
            if (LOWORD (lParam))
                lstrcpy (pszWParam, "Redraw Flag: TRUE\t");
            else
                lstrcpy (pszWParam, "Redraw Flag: FALSE\t");
            break;
        
        case WM_COMPAREITEM :
            wsprintf (pszWParam, "Control ID: %d\t", wParam);
            wsprintf (pszLParam, "COMPAREITEMSTRUCT: %#08x\t", lParam);
            break;
        
        case WM_INITDIALOG :
            wsprintf (pszWParam, "HWND To Focus: %#08x\t", wParam);
            wsprintf (pszLParam, "LPARAM (DATA): %#08x\t", lParam);
            break;
            
        case WM_VSCROLLCLIPBOARD :
        case WM_HSCROLLCLIPBOARD :
        case WM_HSCROLL :
        case WM_VSCROLL :
            {
            char TmpBuffer[PARAM_STRING_SIZE];
            switch (LOWORD(wParam))
            {
                case SB_BOTTOM :
                    lstrcpy (TmpBuffer, "SB_BOTTOM\t");
                    break;
                case SB_ENDSCROLL :
                    lstrcpy (TmpBuffer, "SB_ENDSCROLL");
                    break;
                case SB_LINEUP :
                    lstrcpy (TmpBuffer, "SB_LINEUP\t");
                    break;
                case SB_LINEDOWN :
                    lstrcpy (TmpBuffer, "SB_LINEDOWN\t");
                    break;
                case SB_PAGEUP :
                    lstrcpy (TmpBuffer, "SB_PAGEUP\t");
                    break;
                case SB_PAGEDOWN :
                    lstrcpy (TmpBuffer, "SB_PAGEDOWN\t");
                    break;
                case SB_TOP :
                    lstrcpy (TmpBuffer, "SB_TOP\t");
                    break;
                case SB_THUMBPOSITION :
                    lstrcpy (TmpBuffer, "SB_THUMBPOSITION\t");
                    break;
                default :
                    lstrcpy (TmpBuffer, "(Unknown Flag)\t");
                    break;
            }
            wsprintf (pszWParam, 
                      "Scroll Code: %s Position: %d\t",
                      TmpBuffer, 
                      HIWORD(wParam));
            wsprintf (pszLParam, "hWndScrollBar: %08x\t", lParam);
            }
            break;
            
        case WM_DROPFILES :
            wsprintf (pszWParam, "HANDLE (hDrop): %08x\t", wParam);
            NO_LPARAM;
            break;

        case WM_PALETTECHANGED :
        case WM_PALETTEISCHANGING :
            wsprintf (pszWParam, "HWND: %08x\t", wParam);
            NO_LPARAM;
            break;

        case WM_SETHOTKEY :
            {
            char TmpBuffer[PARAM_STRING_SIZE];
            DecodeVirtualKey ((UINT)wParam, TmpBuffer);
            wsprintf (pszWParam, "Virtual Key: %s\t", TmpBuffer);
            NO_LPARAM;
            }
            break;

        case WM_HOTKEY :
            switch (wParam)
            {
                case IDHOT_SNAPDESKTOP :
                    lstrcpy (pszWParam, "IDHOT_SNAPDESKTOP\t");
                    break;
                case IDHOT_SNAPWINDOW :
                    lstrcpy (pszWParam, "IDHOT_SNAPWINDOW\t");
                    break;
                default :
                    lstrcpy (pszWParam, "(Unknown Flag)\t");
                    break;
            }
            NO_LPARAM;
            break;

        case WM_ENDSESSION :
            if (wParam)
                lstrcpy (pszWParam, "End Session: TRUE\t");
            else
                lstrcpy (pszWParam, "End Session: FALSE\t");
            NO_LPARAM;
            break;

        case WM_WININICHANGE :
        case WM_DEVMODECHANGE :
            NO_WPARAM;
            wsprintf (pszLParam, "LPSTR: %08x\t", lParam);
            break;
        
        case WM_POWER :
            switch (wParam)
            {
                case PWR_SUSPENDREQUEST :
                    lstrcpy (pszWParam, "PWR_SUSPENDREQUEST\t");
                    break;
                case PWR_SUSPENDRESUME :
                    lstrcpy (pszWParam, "PWR_SUSPENDRESUME\t");
                    break;
                case PWR_CRITICALRESUME :
                    lstrcpy (pszWParam, "PWR_CRITICALRESUME\t");
                    break;
                default :
                    lstrcpy (pszWParam, "(Unknown Flag)\t");
                    break;
            }
            NO_LPARAM;
            break;

        case WM_PARENTNOTIFY :
            switch (LOWORD(wParam))
            {
                case WM_CREATE :
                    wsprintf (pszWParam,
                              "%s - Child Window ID: %d\t",
                              "WM_CREATE\t",
                              HIWORD (wParam));
                    wsprintf (pszLParam, "Child HWND: %08x\t", lParam);
                    break;
                case WM_DESTROY :
                    wsprintf (pszWParam,
                              "%s - Child Window ID: %d\t",
                              "WM_DESTROY\t",
                              HIWORD (wParam));
                    wsprintf (pszLParam, "Child HWND: %08x\t", lParam);
                    break;
                case WM_LBUTTONDOWN :
                case WM_MBUTTONDOWN :
                case WM_RBUTTONDOWN :
                    LoadString (hDllInstance, 
                                LOWORD(wParam), 
                                pszWParam, 
                                PARAM_STRING_SIZE);
                    wsprintf (pszLParam, 
                              "X-Pos: %-5d  Y-Pos: %-5d\t",
                              LOWORD(lParam), 
                              HIWORD(lParam));
                    break;
                default :
                    lstrcpy (pszWParam, "(Unknown Parameter Value)\t");
                    lstrcpy (pszLParam, "(Unknown Parameter Value)\t");
                    break;
            }
            break;
/* MOUSE Messages Start Here*/

        case WM_SETCURSOR :
            {
            //char TmpBuffer[PARAM_STRING_SIZE];
            wsprintf (pszWParam, "HWND: %08x\t", wParam);
            /*
            DecodeHitTestCode (LOWORD(lParam), TmpBuffer);
            wsprintf (pszLParam, 
                      "%s - Mouse Message ID: %d\t",
                      TmpBuffer,
                      HIWORD(lParam));
            */
            lstrcpy (pszLParam, " \t");
            }
            break;

        case WM_MOUSEACTIVATE :
            {
            //char TmpBuffer[PARAM_STRING_SIZE];
            wsprintf (pszWParam, "HWND: %08x\t", wParam);
            /*
            DecodeHitTestCode (LOWORD(lParam), TmpBuffer);
            switch (HIWORD(lParam))
            {
                case MA_ACTIVATE : 
                    wsprintf (pszLParam, 
                              "%s - Mouse Message ID: %d\t",
                              TmpBuffer,
                              "MA_ACTIVATE\t");
                    break;
                case MA_NOACTIVATE : 
                    wsprintf (pszLParam, 
                              "%s - Mouse Message ID: %d\t",
                              TmpBuffer,
                              "MA_NOACTIVATE\t");
                    break;
                case MA_ACTIVATEANDEAT : 
                    wsprintf (pszLParam, 
                              "%s - Mouse Message ID: %d\t",
                              TmpBuffer,
                              "MA_ACTIVATEANDEAT\t");
                    break;
                case MA_NOACTIVATEANDEAT : 
                    wsprintf (pszLParam, 
                              "%s - Mouse Message ID: %d\t",
                              TmpBuffer,
                              "MA_NOACTIVATEANDEAT\t");
                    break;
                default :
                    lstrcpy (pszLParam, "(Unknown Flag)\t");
                    break;
            }
            */
            lstrcpy (pszLParam, " \t");
            }
            break;

        case WM_LBUTTONDOWN :
        case WM_LBUTTONUP :
        case WM_LBUTTONDBLCLK :
        case WM_RBUTTONDOWN :
        case WM_RBUTTONUP :
        case WM_RBUTTONDBLCLK :
        case WM_MBUTTONDOWN :
        case WM_MBUTTONUP :
        case WM_MBUTTONDBLCLK :
        case WM_MOUSEMOVE :
            switch (wParam)
            {
                case MK_CONTROL :
                    lstrcpy (pszWParam, "MK_CONTROL\t");
                    break;
                case MK_LBUTTON :
                    lstrcpy (pszWParam, "MK_LBUTTON\t");
                    break;
                case MK_MBUTTON :
                    lstrcpy (pszWParam, "MK_MBUTTON\t");
                    break;
                case MK_RBUTTON :
                    lstrcpy (pszWParam, "MK_RBUTTON\t");
                    break;
                default :
                    lstrcpy (pszWParam, "(Unknown Flag)\t");
                    break;
            }
            wsprintf (pszLParam,
                      "X-Pos: %-5d  Y-Pos: %-5d\t",
                      LOWORD(lParam), 
                      HIWORD(lParam));
            break;

        case WM_NCLBUTTONDOWN :
        case WM_NCLBUTTONUP :
        case WM_NCLBUTTONDBLCLK :
        case WM_NCRBUTTONDOWN :
        case WM_NCRBUTTONUP :
        case WM_NCRBUTTONDBLCLK :
        case WM_NCMBUTTONDOWN :
        case WM_NCMBUTTONUP :
        case WM_NCMBUTTONDBLCLK :
        case WM_NCMOUSEMOVE :
            /*
            DecodeHitTestCode (wParam, pszWParam);
            wsprintf (pszLParam, 
                      "X-Pos: %5d  Y-Pos: %5d\t",
                      ((LPPOINTS)lParam)->x,
                      ((LPPOINTS)lParam)->y);
            */
            lstrcpy (pszWParam, " \t");
            lstrcpy (pszLParam, " \t");
            break;
/* MOUSE Message End here */

        case WM_SYSCHAR :
        case WM_CHAR :
            DecodeVirtualKey ((UINT)wParam, pszWParam);
            lstrcat (pszWParam, "\t");
            DecodeKeyData (lParam, pszLParam);
            break;

        case WM_NCHITTEST :
            NO_WPARAM;
            wsprintf (pszLParam,
                      "X-Pos: %-5d  Y-Pos: %-5d\t",
                      LOWORD(lParam), 
                      HIWORD(lParam));
            break;

        case WM_MDICREATE :
            NO_WPARAM;
            wsprintf (pszLParam, "MDICREATESTRUCT: %08x\t", lParam);
            break;

        case WM_MDIDESTROY :
            wsprintf (pszWParam, "Closed HWND: %08x\t", wParam);
            NO_LPARAM;
            break;

        case WM_MDIACTIVATE :
            wsprintf (pszWParam, "Activate HWND: %08x\t", wParam);
            NO_LPARAM;
            break;

        case WM_MDIRESTORE :
            wsprintf (pszWParam, "Restore HWND: %08x\t", wParam);
            NO_LPARAM;
            break;

        case WM_MDINEXT :
            wsprintf (pszWParam, "Next HWND: %08x\t", wParam);
            if (lParam)
                lstrcpy (pszLParam, "fNext: NON-ZERO\t");
            else
                lstrcpy (pszLParam, "fNext: ZERO\t");
            break;

        case WM_MDIMAXIMIZE :
            wsprintf (pszWParam, "Maximize HWND: %08x\t", wParam);
            NO_LPARAM;
            break;

        case WM_MDICASCADE :
        case WM_MDITILE :
            switch (wParam)
            {
                case MDITILE_HORIZONTAL :
                    lstrcpy (pszWParam, "MDITILE_HORIZONTAL\t");
                    break;
                case MDITILE_VERTICAL :
                    lstrcpy (pszWParam, "MDITILE_VERTICAL\t");
                    break;

                default :
                    lstrcpy (pszWParam, "(Unknown Tiling Flag)\t");
            }
            NO_LPARAM;
            break;

        case WM_MDIGETACTIVE :
            NO_WPARAM;
            if (*((LPBOOL)lParam))
                lstrcpy (pszLParam, "Windows is being maximized\t");
            else
                lstrcpy (pszLParam, "Windows is NOT being maximized\t");
            break;

        case WM_MDISETMENU :
            wsprintf (pszWParam, "Frame HMENU: %08x\t", wParam);
            wsprintf (pszLParam, "Window HMENU: %08x\t", lParam);
            break;

        case WM_CTLCOLORMSGBOX :
        case WM_CTLCOLOREDIT :
        case WM_CTLCOLORLISTBOX :
        case WM_CTLCOLORBTN :
        case WM_CTLCOLORDLG :
        case WM_CTLCOLORSCROLLBAR :
        case WM_CTLCOLORSTATIC :
            wsprintf (pszWParam, "Control HDC: %08x\t", wParam);
            wsprintf (pszLParam, "Control HWND: %08x\t", lParam);
            break;

        case WM_COMPACTING :
            wsprintf (pszWParam, "Compacting Ratio: %d\t", wParam);
            NO_LPARAM;
            break;

        case WM_COPYDATA :
            wsprintf (pszWParam, "Sender HWND: %08x\t", wParam);
            wsprintf (pszLParam, "COPYDATASTRUCT: %08x\t", lParam);
            break;

        case WM_NCCALCSIZE :
            if (wParam)
                lstrcpy (pszWParam, "fCalcValidRects: TRUE\t");
            else
                lstrcpy (pszWParam, "fCalcValidRects: FALSE\t");
            wsprintf (pszLParam, "NCCALCSIZE_PARAMS: %08x\t", lParam);
            break;

        case WM_NCPAINT :
            wsprintf (pszWParam, "Region HRGN: %08x\t", wParam);
            NO_LPARAM;
            break;
        
        case WM_KEYDOWN :
        case WM_KEYUP :
        case WM_SYSKEYDOWN :
        case WM_SYSKEYUP :
            DecodeVirtualKey ((UINT)wParam, pszWParam);
            lstrcat (pszWParam, "\t");
            DecodeKeyData (lParam, pszLParam);
            break;

        case WM_DEADCHAR :
        case WM_SYSDEADCHAR :
            wsprintf (pszWParam, "Character Code (HEX): %#X\t", wParam);
            DecodeKeyData (lParam, pszLParam);
            break;

        case WM_INITMENU :
            wsprintf (pszWParam, "Menu HMENU: %08x\t", wParam);
            NO_LPARAM;
            break;

        case WM_INITMENUPOPUP :
            wsprintf (pszWParam, "Menu HMENU: %08x\t", wParam);
            wsprintf (pszLParam, "Item Position: %d  ", LOWORD (lParam));
            if (HIWORD(lParam))
                lstrcat (pszLParam, "System Menu\t");
            else
                lstrcat (pszLParam, "Application Menu\t");
            break;

        case WM_MENUSELECT :
            wsprintf (pszWParam, "Item ID: %d  - ", LOWORD (wParam));
            switch (HIWORD(wParam))
            {
                case MF_BITMAP :
                    lstrcat (pszWParam, "MF_BITMAP\t");
                    break;
                case MF_CHECKED :
                    lstrcat (pszWParam, "MF_CHECKED\t");
                    break;
                case MF_DISABLED :
                    lstrcat (pszWParam, "MF_DISABLED\t");
                    break;
                case MF_GRAYED :
                    lstrcat (pszWParam, "MF_GRAYED\t");
                    break;
                case MF_HILITE :
                    lstrcat (pszWParam, "MF_HILITE\t");
                    break;
                case MF_MOUSESELECT :
                    lstrcat (pszWParam, "MF_MOUSESELECT\t");
                    break;
                case MF_OWNERDRAW :
                    lstrcat (pszWParam, "MF_OWNERDRAW\t");
                    break;
                case MF_POPUP :
                    lstrcat (pszWParam, "MF_POPUP\t");
                    break;
                case MF_SYSMENU :
                    lstrcat (pszWParam, "MF_SYSMENU\t");
                    break;
                default :
                    lstrcpy (pszWParam, "(Unknown Menu Item Flag)\t");
            }
            wsprintf (pszLParam, "Menu HMENU: %08x", lParam);
            break;

        case WM_MENUCHAR :
            switch (HIWORD(wParam))
            {
                case MF_POPUP :
                    wsprintf (pszWParam,
                              "ASCII (decimal): %d - MF_POPUP\t",
                              LOWORD(wParam));
                    break;
                case MF_SYSMENU :
                    wsprintf (pszWParam,
                              "ASCII (decimal): %d - MF_SYSMENU\t",
                              LOWORD(wParam));
                    break;
                default :
                    lstrcpy (pszWParam, "(Unknown Menu Flag)\t");
            }
            wsprintf (pszLParam, "Active Menu HMENU: %08x\t", lParam);
            break;

        case WM_ENTERIDLE :
            switch (wParam)
            {
                case MSGF_DIALOGBOX :
                    lstrcpy (pszWParam, "Idle Source: MSGF_DIALOGBOX\t");
                    break;
                case MSGF_MENU :
                    lstrcpy (pszWParam, "Idle Source: MSGF_MENU\t");
                    break;
                default :
                    lstrcpy (pszWParam, "(Unknown Idle Source Flag)\t");
            }
            wsprintf (pszLParam, "Idle Window HWND: %08x\t", lParam);
            break;

        case WM_ENTERMENULOOP :
        case WM_EXITMENULOOP :
            if (wParam)
                lstrcpy (pszWParam, "Popup Menu?: TRUE\t");
            else
                lstrcpy (pszWParam, "Popup Menu?: FALSE\t");
            NO_LPARAM;
            break;
            
        case WM_RENDERFORMAT :
            DecodeClipboardFormat (wParam, pszWParam);
            NO_LPARAM;
            break;

        case WM_PAINTCLIPBOARD :
        case WM_SIZECLIPBOARD :
            wsprintf (pszWParam, "Clipboard Viewer HWND: %08x\t", wParam);
            wsprintf (pszLParam, "Handle (HGLOBAL): %08x\t", lParam);
            break;

        case WM_ASKCBFORMATNAME :
            wsprintf (pszWParam, "Buffer Size (decimal): %d\t", wParam);
            wsprintf (pszLParam, "Format Name LPSTR: %08x\t", lParam);
            break;

        case WM_CHANGECBCHAIN :
            wsprintf (pszWParam, "Removed Window HWND: %08x\t", wParam);
            wsprintf (pszLParam, "Next Window HWND: %08x\t", lParam);
            break;

        case WM_DDE_INITIATE :
        case WM_DDE_TERMINATE :
        case WM_DDE_ADVISE :
        case WM_DDE_UNADVISE :
        case WM_DDE_ACK :
        case WM_DDE_DATA :
        case WM_DDE_REQUEST :
        case WM_DDE_POKE :
        case WM_DDE_EXECUTE :
            wsprintf (pszWParam, "Application HWND: %08x\t", wParam);
            wsprintf (pszLParam, "LPARAM Value: %#08x\t", lParam);
            break;

        case WM_COMMAND :
            {    
            BOOL fIsWindowControl = FALSE;
            int  iClassType = 0;
            wsprintf (pszWParam, "%#08x\t", wParam);
            wsprintf (pszLParam, "%#08x\t", lParam);
            break;
            if (0 == lstrcmpi (pszHwnd, "EDIT\t"))
            {
                fIsWindowControl = TRUE;
                iClassType = EDIT_CLASS;
            }
            else
            {
                if (0 == lstrcmpi (pszHwnd, "BUTTON\t"))
                {
                    fIsWindowControl = TRUE;
                    iClassType = BUTTON_CLASS;
                }
                else
                {
                    if (0 == lstrcmpi (pszHwnd, "COMBOBOX\t"))
                    {
                        fIsWindowControl = TRUE;
                        iClassType = COMBOBOX_CLASS;
                    }
                    else
                    {
                        if (0 == lstrcmpi (pszHwnd, "LISTBOX\t"))
                        {
                            fIsWindowControl = TRUE;
                            iClassType = LISTBOX_CLASS;
                        }
                        else
                        {
                            if (0 == lstrcmpi (pszHwnd, "SCROLLBAR\t"))
                            {
                                fIsWindowControl = TRUE;
                                iClassType = SCROLLBAR_CLASS;
                            }
                            else
                            {
                                if (0 == lstrcmpi (pszHwnd, "STATIC\t"))
                                {
                                    fIsWindowControl = TRUE;
                                    iClassType = STATIC_CLASS;
                                }
                            }
                        }
                    }
                }
            }
            if (fIsWindowControl)
                DecodeControlNotification (iClassType, wParam, lParam, 
                                           pszWParam, pszLParam);
            }
            break;

        case EM_GETSEL :
        case EM_SETSEL :
        case EM_GETRECT :
        case EM_SETRECT :
        case EM_SETRECTNP :
        case EM_SCROLL :
        case EM_LINESCROLL :
        case EM_SCROLLCARET :
        case EM_GETMODIFY :
        case EM_SETMODIFY :
        case EM_GETLINECOUNT :
        case EM_LINEINDEX :
        case EM_SETHANDLE :
        case EM_GETHANDLE :
        case EM_GETTHUMB :
        case EM_LINELENGTH :
        case EM_REPLACESEL :
        case EM_GETLINE :
        case EM_LIMITTEXT :
        case EM_CANUNDO :
        case EM_UNDO :
        case EM_FMTLINES :
        case EM_LINEFROMCHAR :
        case EM_SETTABSTOPS :
        case EM_SETPASSWORDCHAR :
        case EM_EMPTYUNDOBUFFER :
        case EM_GETFIRSTVISIBLELINE :
        case EM_SETREADONLY :
        case EM_SETWORDBREAKPROC :
        case EM_GETWORDBREAKPROC :
        case EM_GETPASSWORDCHAR :
        case BM_SETCHECK :
        case BM_SETSTATE :
        case BM_SETSTYLE :
        
        case WM_USER :
        case WM_CTLCOLOR :
        case WM_COMMNOTIFY :
        case WM_SYSTEMERROR :
        default :
            wsprintf (pszWParam, "%#08x\t", wParam);
            wsprintf (pszLParam, "%#08x\t", lParam);
    }
}

//
//  FUNCTION:   DecodeVirtualKey
//
//  PURPOSE:    Decode the virtual key and return the VK_xxx string
//
//  PARAMETERS:
//
//      uVirtKey    - Virtual key code
//      pszBuff     - Pointer to buffer
//
//  RETURN VALUE:
//
//      none
//
//  COMMENTS:
//
//      The pointer to the string buffer assume that they point to a memory
//      block allocated by the calling function
//
void WINAPI DecodeVirtualKey (UINT uVirtKey, LPSTR pszBuff)
{
    if (uVirtKey >= 32 && uVirtKey < 128)
        wsprintf (pszBuff, "Keyboard Letter: %c", (char)uVirtKey);
    else
        lstrcpy (pszBuff, "(Unknown Key Code)");
    switch (uVirtKey)
    {
        case VK_LBUTTON:    
            lstrcpy (pszBuff, "VK_LBUTTON");
            break;
        case VK_RBUTTON:    
            lstrcpy (pszBuff, "VK_RBUTTON");
            break;
        case VK_CANCEL:     
            lstrcpy (pszBuff, "VK_CANCEL");
            break;
        case VK_MBUTTON:    
            lstrcpy (pszBuff, "VK_MBUTTON");
            break;
        case VK_BACK:       
            lstrcpy (pszBuff, "VK_BACK");
            break;
        case VK_TAB:        
            lstrcpy (pszBuff, "VK_TAB");
            break;
        case VK_CLEAR:      
            lstrcpy (pszBuff, "VK_CLEAR");
            break;
        case VK_RETURN:     
            lstrcpy (pszBuff, "VK_RETURN");
            break;
        case VK_SHIFT:      
            lstrcpy (pszBuff, "VK_SHIFT");
            break;
        case VK_CONTROL:    
            lstrcpy (pszBuff, "VK_CONTROL");
            break;
        case VK_MENU:       
            lstrcpy (pszBuff, "VK_MENU");
            break;
        case VK_PAUSE:      
            lstrcpy (pszBuff, "VK_PAUSE");
            break;
        case VK_CAPITAL:    
            lstrcpy (pszBuff, "VK_CAPITAL");
            break;
        case VK_ESCAPE:     
            lstrcpy (pszBuff, "VK_ESCAPE");
            break;
        case VK_SPACE:      
            lstrcpy (pszBuff, "VK_SPACE");
            break;
        case VK_PRIOR:      
            lstrcpy (pszBuff, "VK_PRIOR");
            break;
        case VK_NEXT:       
            lstrcpy (pszBuff, "VK_NEXT");
            break;
        case VK_END:        
            lstrcpy (pszBuff, "VK_END");
            break;
        case VK_HOME:       
            lstrcpy (pszBuff, "VK_HOME");
            break;
        case VK_LEFT:       
            lstrcpy (pszBuff, "VK_LEFT");
            break;
        case VK_UP:         
            lstrcpy (pszBuff, "VK_UP");
            break;
        case VK_RIGHT:      
            lstrcpy (pszBuff, "VK_RIGHT");
            break;
        case VK_DOWN:       
            lstrcpy (pszBuff, "VK_DOWN");
            break;
        case VK_SELECT:     
            lstrcpy (pszBuff, "VK_SELECT");
            break;
        case VK_PRINT:      
            lstrcpy (pszBuff, "VK_PRINT");
            break;
        case VK_EXECUTE:    
            lstrcpy (pszBuff, "VK_EXECUTE");
            break;
        case VK_SNAPSHOT:   
            lstrcpy (pszBuff, "VK_SNAPSHOT");
            break;
        case VK_INSERT:     
            lstrcpy (pszBuff, "VK_INSERT");
            break;
        case VK_DELETE:     
            lstrcpy (pszBuff, "VK_DELETE");
            break;
        case VK_HELP:       
            lstrcpy (pszBuff, "VK_HELP");
            break;
        case VK_NUMPAD0:    
            lstrcpy (pszBuff, "VK_NUMPAD0");
            break;
        case VK_NUMPAD1:    
            lstrcpy (pszBuff, "VK_NUMPAD1");
            break;
        case VK_NUMPAD2:    
            lstrcpy (pszBuff, "VK_NUMPAD2");
            break;
        case VK_NUMPAD3:    
            lstrcpy (pszBuff, "VK_NUMPAD3");
            break;
        case VK_NUMPAD4:    
            lstrcpy (pszBuff, "VK_NUMPAD4");
            break;
        case VK_NUMPAD5:    
            lstrcpy (pszBuff, "VK_NUMPAD5");
            break;
        case VK_NUMPAD6:    
            lstrcpy (pszBuff, "VK_NUMPAD6");
            break;
        case VK_NUMPAD7:    
            lstrcpy (pszBuff, "VK_NUMPAD7");
            break;
        case VK_NUMPAD8:    
            lstrcpy (pszBuff, "VK_NUMPAD8");
            break;
        case VK_NUMPAD9:    
            lstrcpy (pszBuff, "VK_NUMPAD9");
            break;
        case VK_MULTIPLY:   
            lstrcpy (pszBuff, "VK_MULTIPLY");
            break;
        case VK_ADD:        
            lstrcpy (pszBuff, "VK_ADD");
            break;
        case VK_SEPARATOR:  
            lstrcpy (pszBuff, "VK_SEPARATOR");
            break;
        case VK_SUBTRACT:   
            lstrcpy (pszBuff, "VK_SUBTRACT");
            break;
        case VK_DECIMAL:    
            lstrcpy (pszBuff, "VK_DECIMAL");
            break;
        case VK_DIVIDE:     
            lstrcpy (pszBuff, "VK_DIVIDE");
            break;
        case VK_F1:         
            lstrcpy (pszBuff, "VK_F1");
            break;
        case VK_F2:         
            lstrcpy (pszBuff, "VK_F2");
            break;
        case VK_F3:         
            lstrcpy (pszBuff, "VK_F3");
            break;
        case VK_F4:         
            lstrcpy (pszBuff, "VK_F4");
            break;
        case VK_F5:         
            lstrcpy (pszBuff, "VK_F5");
            break;
        case VK_F6:         
            lstrcpy (pszBuff, "VK_F6");
            break;
        case VK_F7:         
            lstrcpy (pszBuff, "VK_F7");
            break;
        case VK_F8:         
            lstrcpy (pszBuff, "VK_F8");
            break;
        case VK_F9:         
            lstrcpy (pszBuff, "VK_F9");
            break;
        case VK_F10:        
            lstrcpy (pszBuff, "VK_F10");
            break;
        case VK_F11:        
            lstrcpy (pszBuff, "VK_F11");
            break;
        case VK_F12:        
            lstrcpy (pszBuff, "VK_F12");
            break;
        case VK_F13:        
            lstrcpy (pszBuff, "VK_F13");
            break;
        case VK_F14:        
            lstrcpy (pszBuff, "VK_F14");
            break;
        case VK_F15:        
            lstrcpy (pszBuff, "VK_F15");
            break;
        case VK_F16:        
            lstrcpy (pszBuff, "VK_F16");
            break;
        case VK_NUMLOCK:    
            lstrcpy (pszBuff, "VK_NUMLOCK");
            break;
    }
}
