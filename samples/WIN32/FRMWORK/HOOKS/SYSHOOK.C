#include <windows.h>
#include "GLOBALS.H"
#include "SysHook.H"

HINSTANCE       hDllInstance        = NULL;
HANDLE          hSharedDataMemFile  = NULL;
PGLOBOPTS       pS;
static CWPSTRUCT gCwp;
static COPYDATASTRUCT  gCds = {0, sizeof (CWPSTRUCT), &gCwp};


//
//  FUNCTION:   AddMsgToList
//
//  PURPOSE:    Copies the data into a CWPSTRUCT and sends it to the message
//              handler function for addition to the list of messages.
//
//  PARAMETERS:
//
//      hWnd    - handle of the window for which the message is intended
//      message - message number
//      wParam  - wParam of message
//      lParam  - lParam of message
//
//  RETURN VALUE:
//
//      none
//
//  COMMENTS:
//
//      none
//
void WINAPI AddMsgToList (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!IsWindow(pS->hMsgWindow) ||        // Make sure windows are still available
        !IsWindow(pS->hListBox))            // before sending messages to them
    {
        //if (!FindMsgWindow())  // Find message window
            return;
    }
    // Copy message information into structure
    gCwp.hwnd    = hWnd;
    gCwp.message = message;
    gCwp.wParam  = wParam;
    gCwp.lParam  = lParam;
    // Send message to target window who will handle the message
    SendMessage (pS->hMsgWindow, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&gCds);
}

//
//  FUNCTION:   CallWndProcHookProc
//
//  PURPOSE:    Callback function for the WH_CALLWNDPROC
//
//  PARAMETERS:
//
//      nCode   - Hook code
//      wParam  - Current-process flag
//      lParam  - Address of CWPSTRUCT message structure
//
//  RETURN VALUE:
//
//      Return value from CallNextHookEx
//
//  COMMENTS:
//
//      none
//
LRESULT CALLBACK CallWndProcHookProc (int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && HC_ACTION == nCode)
    {
        // If the message is filterd add it to the list
        if (IsMessageFiltered (((PCWPSTRUCT)lParam)->message, 
                               ((PCWPSTRUCT)lParam)->hwnd))
        {
            AddMsgToList (((PCWPSTRUCT)lParam)->hwnd,
                          ((PCWPSTRUCT)lParam)->message,
                          ((PCWPSTRUCT)lParam)->wParam,
                          ((PCWPSTRUCT)lParam)->lParam);
            if ((WM_DESTROY == ((PCWPSTRUCT)lParam)->message) && pS->hWndMonitored)
            {
                PostMessage (pS->hMsgWindow, WM_COMMAND, IDM_CAPOFF, 0);
            }
        }
    }
    return CallNextHookEx (pS->hWH_CALLWNDPROCHook, nCode, wParam, lParam);
}

//
//  FUNCTION:   ChangeFilterOptions
//
//  PURPOSE:    Change filtering options for deciding which messages should be
//              monitored
//
//  PARAMETERS:
//
//      options        - ORed number with the flags of the messages monitored
//      uLoRange       - Lowest number of message filtered
//      uHiRange       - Highest number of message filtered
//      fMonitoredBool - BOOLean flag: If true, messages will be filtered for
//                       child windows of window monitored
//  RETURN VALUE:
//
//      none
//
//  COMMENTS:
//
//      This functions is called by the property sheet when an option need to
//      be changed on the fly. The parameters modified on this function will
//      be reflected inmediately on the message list
//
void WINAPI ChangeFilterOptions (DWORD options, UINT uLoRange, UINT uHiRange, BOOL fMonitorChild)
{
    pS->dwMsgFilterOptions = options;
}

//
//  FUNCTION:   ClearMsgList
//
//  PURPOSE:    This function deletes the contents of the list box with the
//              messages. It also resets the message array index and related
//              flags.
//
//  PARAMETERS:
//
//      none
//
//  RETURN VALUE:
//
//      none
//
//  COMMENTS:
//
//      none
//
void WINAPI ClearMsgList (void)
{
    pS->nIndex = 0;
    pS->fWrapped = FALSE;
    SendMessage (pS->hListBox, LB_RESETCONTENT, 0, 0);
}

//
//  FUNCTION:   DllMain
//
//  PURPOSE:    Entry point for DLL. This functions is called when the DLL
//              is mapped and unmapped from a process address space
//
//  PARAMETERS:
//
//      hDll        - Instance of the dll being mapped or unmapped. This number 
//                    is process-specific and is not valid accross processes
//      fdwReason   - Reason for which the functions is called.
//      lpvReserved - Reserved for future use. Do not use.
//
//  RETURN VALUE:
//
//      TRUE    If DLL should go ahead and be mapped (or unmapped)
//      FALSE   If otherwise
//
//  COMMENTS:
//
//      If you want to use C runtime libraries, keep this function named
//      "DllMain" and you won't have to do anything special to initialize
//      the runtime libraries.
//
//      When fdwReason == DLL_PROCESS_ATTACH, the return value is used to
//      determine if the DLL should remain loaded, or should be immediately
//      unloaded depending upon whether the DLL could be initialized properly.
//      For all other values of fdwReason, the return value is ignored.
//
BOOL WINAPI DllMain (HINSTANCE hDll, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH :
            hDllInstance = hDll;
            hSharedDataMemFile = CreateFileMapping ((HANDLE)0xFFFFFFFF, 
                                                    NULL, 
                                                    PAGE_READWRITE,
                                                    0, 
                                                    sizeof (GLOBOPTS), 
                                                    HOOK_SHARED_DATA_MEM_FILE);
            if (!hSharedDataMemFile)
                goto OnError;

            pS = (PGLOBOPTS)MapViewOfFile (hSharedDataMemFile, 
                                           FILE_MAP_WRITE, 
                                           0, 
                                           0, 
                                           0);
            if (!pS)
                goto OnError;
            
            return TRUE;
OnError:
            UnmapViewOfFile ((LPVOID)pS);
            CloseHandle (hSharedDataMemFile);
            return FALSE;

        case DLL_PROCESS_DETACH :
            UnmapViewOfFile ((LPVOID)pS);
            CloseHandle (hSharedDataMemFile);
            break;
    }
    return TRUE;
}

//
//  FUNCTION:   GetMessageHookProc
//
//  PURPOSE:    Callback function for the WH_GETMESSAGE
//
//  PARAMETERS:
//
//      nCode   - Hook code
//      wParam  - Removal flag
//      lParam  - Address of MSG message structure
//
//  RETURN VALUE:
//
//      Return value from CallNextHookEx
//
//  COMMENTS:
//
//      none
//
LRESULT CALLBACK GetMessageHookProc (int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && HC_ACTION == nCode)
    {   
        if (IsMessageFiltered (((PMSG)lParam)->message, ((PMSG)lParam)->hwnd))
        {
            AddMsgToList (((PMSG)lParam)->hwnd,
                          ((PMSG)lParam)->message,
                          ((PMSG)lParam)->wParam,
                          ((PMSG)lParam)->lParam);
        }
    }
    return CallNextHookEx (pS->hWH_GETMESSAGEHook, nCode, wParam, lParam);
}


//
//  FUNCTION:   InstallHooks
//
//  PURPOSE:    Installs two hooks: WH_CALLWNDPROC and WH_GETMESSAGE. This 
//              hooks will filter all the message comming into the system
//
//  PARAMETERS:
//
//      hWndToMon   - The handle of the window monitored. If this parameter
//                    is null, the hooks are installed to monitor all the
//                    threads in the system
//
//  RETURN VALUE:
//
//      TRUE    - If hooks were installed successfully
//      FALSE   - If the hooks SetWindowsHookEx() function failed
//
//  COMMENTS:
//
//      The flag fMonitorChild will determine if we will monitor message 
//      for all the children windows of the window selected
//
BOOL WINAPI InstallHooks (HWND hWndToMon)
{       
    DWORD dwThreadToMonitor;

    // Get the thread ID of the windows monitored
    // if we're browinsg ALL windows (hWndToMon == 0x0000FFFF),
    // we set dwThreadToMonitor = NULL.
    if (hWndToMon != (HWND)0x0000FFFF)
//      dwThreadToMonitor = GetWindowThreadProcessId (hWndToMon, NULL);
        dwThreadToMonitor = 0;
    else
        dwThreadToMonitor = 0;

    // If the hooks are already installed, return OK.
    if (pS->hWH_CALLWNDPROCHook || pS->hWH_GETMESSAGEHook)
        return TRUE;

    // The instance to the DLL cannot be null. SetWindowsHookEx() will
    // fail, usually in a nasty (system crash) way.
    if (NULL == hDllInstance)
        return FALSE;

    // Install the Hooks
    pS->hWH_CALLWNDPROCHook = SetWindowsHookEx (WH_CALLWNDPROC,
                                                (HOOKPROC)CallWndProcHookProc,
                                                hDllInstance,
                                                dwThreadToMonitor);
    pS->hWH_GETMESSAGEHook = SetWindowsHookEx (WH_GETMESSAGE, 
                                               (HOOKPROC)GetMessageHookProc,
                                               hDllInstance,
                                               dwThreadToMonitor);
    // Verify the hooks were installed. If they failed, try removing the other
    // hook if it was successfull, then return
    if (NULL == pS->hWH_CALLWNDPROCHook)
        if (pS->hWH_GETMESSAGEHook)
        {
            UnhookWindowsHookEx (pS->hWH_GETMESSAGEHook);
            pS->hWH_GETMESSAGEHook = NULL;
            return FALSE;
        }
    if (NULL == pS->hWH_GETMESSAGEHook)
        if (pS->hWH_CALLWNDPROCHook)
        {
            UnhookWindowsHookEx (pS->hWH_CALLWNDPROCHook);
            pS->hWH_CALLWNDPROCHook = NULL;
            return FALSE;
        }

    // Set the shared structure's member to the hWnd of the Windows monitored.
    // This value may be NULL
    pS->hWndMonitored = hWndToMon;

    // Reset index of message array, and reset fWrapped flag
    pS->nIndex = 0;
    pS->nListBoxSize = 0;
    pS->fWrapped = FALSE;
    return TRUE;
}

//
//  FUNCTION:   IsMessageFiltered
//
//  PURPOSE:    Determine if a message should be added to the list of messages.
//              Determine if the message is intended for the Hook monitor 
//              application and prevent recursion from happening.
//              Use the dwMsgFilterOptions bits to determines which messages
//              should be allowed to go through.
//
//  PARAMETERS:
//
//      hWnd    - Handle of the window for which the message is destined
//      message - Message Id
//
//  RETURN VALUE:
//
//      TRUE    - If message should be added
//      FALSE   - If message does not meet filter criteria
//
//  COMMENTS:
//
//      none
//
BOOL WINAPI IsMessageFiltered (UINT message, HWND hWnd)
{
    if (pS->hMsgWindow == hWnd || IsChild (pS->hMsgWindow, hWnd))
        return FALSE;       // Cannot monitor on itself or any of our
                            // child windows (controls)

    // If we are monitoring on a specific window
    // AND not browsing on all windows
    if ((pS->hWndMonitored) && (pS->hWndMonitored != (HWND)0x0000FFFF))
    {
        // If the message destination window a child of the window
        // we are monitoring?
        if (IsChild (pS->hWndMonitored, hWnd))
        {
            // If the message is for a child window of the window we are
            // monitoring, are we supposed to monitor them?
            if (!pS->fMonitorChild)
                return FALSE;   // No
        }
        else
        {
            // If the message destination window the window
            // we are monitoring?
            if (pS->hWndMonitored != hWnd)
                return FALSE;   // No
        }
    }
    
    // If we are monitoring on ALL message, just return and 
    // don't bother any more
    if (MSG_ALL_MSGS == pS->dwMsgFilterOptions)
        return TRUE;

    // Are we monitoring on a specific range of message?
    if (pS->dwMsgFilterOptions & MSG_MSG_RANGE_ONLY)
    {
        // Are the message on the specifc range?
        if (message >= pS->uLoRange && message <= pS->uHiRange)
            return TRUE;
    }

    // Verify the message is selected in the filter options
    switch (message)
    {
        case WM_NULL :
        case WM_CREATE :
        case WM_DESTROY :
        case WM_MOVE :
        case WM_SIZE :
        case WM_ACTIVATE :
        case WM_SETFOCUS :
        case WM_KILLFOCUS :
        case WM_ENABLE :
        case WM_SETREDRAW :
        case WM_SETTEXT :
        case WM_GETTEXT :
        case WM_GETTEXTLENGTH :
        case WM_PAINT :
        case WM_CLOSE :
        case WM_QUERYENDSESSION :
        case WM_QUIT :
        case WM_QUERYOPEN :
        case WM_ERASEBKGND :
        case WM_SHOWWINDOW :
        case WM_ACTIVATEAPP :
        case WM_CANCELMODE :
        case WM_GETMINMAXINFO :
        case WM_ICONERASEBKGND :
        case WM_NEXTDLGCTL :
        case WM_SPOOLERSTATUS :
        case WM_DRAWITEM :
        case WM_MEASUREITEM :
        case WM_DELETEITEM :
        case WM_VKEYTOITEM :
        case WM_CHARTOITEM :
        case WM_SETFONT :
        case WM_GETFONT :
        case WM_QUERYDRAGICON :
        case WM_COMPAREITEM :
        case WM_WINDOWPOSCHANGING :
        case WM_WINDOWPOSCHANGED :
        case WM_CHILDACTIVATE :
        case WM_GETDLGCODE :
        case WM_INITDIALOG :
        case WM_COMMAND :
        case WM_HSCROLL :
        case WM_VSCROLL :
        case WM_DROPFILES :
        case WM_QUERYNEWPALETTE :
        case WM_PALETTEISCHANGING :
        case WM_PALETTECHANGED :
        case WM_PAINTICON :
        case WM_PENWINFIRST :
        case WM_PENWINLAST :
        case WM_SETHOTKEY :
        case WM_GETHOTKEY :
        case WM_HOTKEY :
            return ((pS->dwMsgFilterOptions & MSG_STANDARD_MSGS)
                     ? TRUE : FALSE);

        case WM_SYSCOLORCHANGE :
        case WM_ENDSESSION :
        case WM_WININICHANGE :
        case WM_DEVMODECHANGE :
        case WM_QUEUESYNC :
        case WM_FONTCHANGE :
        case WM_TIMECHANGE :
        case WM_COMPACTING :
        case WM_POWER :
        case WM_SYSCOMMAND :
        case WM_PARENTNOTIFY :        
            return ((pS->dwMsgFilterOptions & MSG_SYSTEM) 
                     ? TRUE : FALSE);

        case WM_CTLCOLOR :
        case WM_COMMNOTIFY :
        case WM_SYSTEMERROR :
            return ((pS->dwMsgFilterOptions & MSG_WIN31_MSGS)
                     ? TRUE : FALSE);

        case WM_SETCURSOR :
        case WM_MOUSEACTIVATE :
        case WM_MOUSEMOVE :
        case WM_LBUTTONDOWN :
        case WM_LBUTTONUP :
        case WM_LBUTTONDBLCLK :
        case WM_RBUTTONDOWN :
        case WM_RBUTTONUP :
        case WM_RBUTTONDBLCLK :
        case WM_MBUTTONDOWN :
        case WM_MBUTTONUP :
        case WM_MBUTTONDBLCLK :
        case WM_NCHITTEST :
        case WM_NCMOUSEMOVE :
        case WM_NCLBUTTONDOWN :
        case WM_NCLBUTTONUP :
        case WM_NCLBUTTONDBLCLK :
        case WM_NCRBUTTONDOWN :
        case WM_NCRBUTTONUP :
        case WM_NCRBUTTONDBLCLK :
        case WM_NCMBUTTONDOWN :
        case WM_NCMBUTTONUP :
        case WM_NCMBUTTONDBLCLK :
            return ((pS->dwMsgFilterOptions & MSG_MOUSE)
                     ? TRUE : FALSE);

        case WM_NCCREATE :
        case WM_NCDESTROY :
        case WM_NCCALCSIZE :
        case WM_NCPAINT :
        case WM_NCACTIVATE :
            return ((pS->dwMsgFilterOptions & MSG_NON_CLIENT)
                     ? TRUE : FALSE);

        case WM_KEYDOWN :
        case WM_KEYUP :
        case WM_CHAR :
        case WM_DEADCHAR :
            return ((pS->dwMsgFilterOptions & MSG_KEYBOARD)
                     ? TRUE : FALSE);
        
        case WM_SYSKEYDOWN :
        case WM_SYSKEYUP :
        case WM_SYSCHAR :
        case WM_SYSDEADCHAR :
            return (((pS->dwMsgFilterOptions & MSG_KEYBOARD) 
                   &&(pS->dwMsgFilterOptions & MSG_SYSTEM)) 
                      ? TRUE : FALSE);
    
        case WM_TIMER :
            return ((pS->dwMsgFilterOptions & MSG_TIMER)
                     ? TRUE : FALSE);

        case WM_INITMENU :
        case WM_INITMENUPOPUP :
        case WM_MENUSELECT :
        case WM_MENUCHAR :
        case WM_ENTERIDLE :
        case WM_ENTERMENULOOP :
        case WM_EXITMENULOOP :
            return ((pS->dwMsgFilterOptions & MSG_MENU) 
                     ? TRUE : FALSE);

        case WM_MDICREATE :
        case WM_MDIDESTROY :
        case WM_MDIACTIVATE :
        case WM_MDIRESTORE :
        case WM_MDINEXT :
        case WM_MDIMAXIMIZE :
        case WM_MDITILE :
        case WM_MDICASCADE :
        case WM_MDIICONARRANGE :
        case WM_MDIGETACTIVE :
        case WM_MDISETMENU :
        case WM_MDIREFRESHMENU :
            return ((pS->dwMsgFilterOptions & MSG_MDI)
                     ? TRUE : FALSE);

        case WM_CTLCOLORMSGBOX :
        case WM_CTLCOLOREDIT :
        case WM_CTLCOLORLISTBOX :
        case WM_CTLCOLORBTN :
        case WM_CTLCOLORDLG :
        case WM_CTLCOLORSCROLLBAR :
        case WM_CTLCOLORSTATIC :
            return ((pS->dwMsgFilterOptions & MSG_CTL_MSGS)
                     ? TRUE : FALSE);

        case WM_USER :
            return ((pS->dwMsgFilterOptions & MSG_WM_USER)
                     ? TRUE : FALSE);

        case WM_CUT :
        case WM_COPY :
        case WM_PASTE :
        case WM_CLEAR :
        case WM_UNDO :
        case WM_RENDERFORMAT :
        case WM_RENDERALLFORMATS :
        case WM_DESTROYCLIPBOARD :
        case WM_DRAWCLIPBOARD :
        case WM_PAINTCLIPBOARD :
        case WM_VSCROLLCLIPBOARD :
        case WM_SIZECLIPBOARD :
        case WM_ASKCBFORMATNAME :
        case WM_CHANGECBCHAIN :
        case WM_HSCROLLCLIPBOARD :
            return ((pS->dwMsgFilterOptions & MSG_CLIPBOARD)
                     ? TRUE : FALSE);

        case WM_DDE_INITIATE :
        case WM_DDE_TERMINATE :
        case WM_DDE_ADVISE :
        case WM_DDE_UNADVISE :
        case WM_DDE_ACK :
        case WM_DDE_DATA :
        case WM_DDE_REQUEST :
        case WM_DDE_POKE :
        case WM_DDE_EXECUTE :
            return ((pS->dwMsgFilterOptions & MSG_DDE)
                     ? TRUE : FALSE);

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
            return ((pS->dwMsgFilterOptions & MSG_EDT_MSGS)
                     ? TRUE : FALSE);

        case BM_GETCHECK :
        case BM_SETCHECK :
        case BM_GETSTATE :
        case BM_SETSTATE :
        case BM_SETSTYLE :
            return ((pS->dwMsgFilterOptions & MSG_BTN_MSGS)
                     ? TRUE : FALSE);
    }
    return ((pS->dwMsgFilterOptions & MSG_UNKNOWN) ? TRUE : FALSE);
}


//
//  FUNCTION:   MsgStringToMsgNumber
//
//  PURPOSE:    Toggles the MESSAGE
//
//  PARAMETERS:
//
//      fSet    - If TRUE, toggles from STRING text to NUMBER text
//              - If FALSE, toggles from NUMBER text to STRING text
//
//  RETURN VALUE:
//
//      none
//
//  COMMENTS:
//
//      ( This functions all 4 do the same for the different field )
//      ( eventually they will be moved to the HOOKS.C module      )
//      ( Right now they are just place holders                    )
//      ( I will add the code for this soon                        )
//
void WINAPI MsgStringToMsgNumber (BOOL fSet)
{
    return;
}

void WINAPI HwndStringToHwndNumber (BOOL fSet)
{
    return;
}

void WINAPI LparamStringToLparamNumber (BOOL fSet)
{
    return;
}

void WINAPI WparamStringToWparamNumber (BOOL fSet)
{
    return;
}


//
//  FUNCTION:   RemoveHooks
//
//  PURPOSE:    Deinstalls the hooks by calling UnhookWindowsHookEx
//
//  PARAMETERS:
//
//      none
//
//  RETURN VALUE:
//
//      TRUE    - If UnhookWindowsHookEx succeded
//      FALSE   - If UnhookWindowsHookEx failed
//
//  COMMENTS:
//
//      none
//
BOOL WINAPI RemoveHooks ()
{
    if (pS->hWH_CALLWNDPROCHook)
    {
        if (!UnhookWindowsHookEx (pS->hWH_CALLWNDPROCHook))
            return FALSE;
        pS->hWH_CALLWNDPROCHook = NULL;
    }
    if (pS->hWH_GETMESSAGEHook)
    {
        if (!UnhookWindowsHookEx (pS->hWH_GETMESSAGEHook))
            return FALSE;
        pS->hWH_GETMESSAGEHook  = NULL;
    }
    return TRUE;
}
