// This message is defined in Exports.C too and in SysHook.C.
// User-Defined Message to pass the hWndBrowse selected 
// from the DLL back to main application.
// with wParam containing the hWnd.
#define WM_PASSHWNDBROWSE  WM_USER+501


// Function prototypes specific to hooks.c


LRESULT MsgSetCursor(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgCopyData(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPassHwndBrowse(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgLButtonDown(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgLButtonUp(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgMouseMove(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgCtlColorListBox(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdWindowProperties(HWND, WORD, WORD, HWND);
LRESULT CmdClearWnd (HWND, WORD, WORD, HWND);
LRESULT CmdCapOn (HWND, WORD, WORD, HWND);
LRESULT CmdCapOff(HWND, WORD, WORD, HWND);
LRESULT CmdFont(HWND, WORD, WORD, HWND);


//
// ID's for the browse dialog. Used in Browse.dlg
//
#define IDM_WNDINFO                 110
#define IDM_CLASSINFO               111
#define IDM_MODULEINFO              112
#define IDM_OWNER                   113
#define IDM_PARENT                  114
#define IDM_STYLE                   115
#define IDM_EXTENDED                117


//-------------------------------------------------------------------------
// Functions for handling main window commands--ie. functions for
// processing WM_COMMAND messages based on the wParam value.
// The message-dispatching mechanism expects all command-handling
// functions to have the following prototype:
//
//     LRESULT FunctionName(HWND, WORD, WORD, HWND);

LRESULT CmdBrowseWnd(HWND, WORD, WORD, HWND);
LRESULT CmdAllWnds(HWND, WORD, WORD, HWND);


//  Auxiliary Functions
BOOL WINAPI InitAppHandles (void);
void WINAPI CloseAppHandles (void);
void WINAPI InitSharedData (DWORD, HWND, HWND);
BOOL WINAPI OpenOutputHandles (void);
void WINAPI CloseOutputHandles (void);
void WINAPI HandleHookMessage (PCWPSTRUCT);
void  ChooseNewFont(HWND);
                               

