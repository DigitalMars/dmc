
// Function Prototypes
BOOL WINAPI DllMain (HINSTANCE, DWORD, LPVOID);

LRESULT CALLBACK GetMessageHookProc (int, WPARAM, LPARAM);
LRESULT CALLBACK CallWndProcHookProc (int, WPARAM, LPARAM);

void WINAPI AddMsgToList (HWND, UINT, WPARAM, LPARAM);

void WINAPI DecodeClipboardFormat (UINT, LPSTR);
void WINAPI DecodeControlNotification (int, WPARAM, LPARAM, 
                                       LPSTR, LPSTR);
void WINAPI DecodeHitTestCode (UINT, LPSTR);
void WINAPI DecodeKeyData (LPARAM, LPSTR);
void WINAPI DecodeMessageParams (HWND, UINT, WPARAM, LPARAM, 
                                 LPSTR, LPSTR, LPSTR, LPSTR);
void WINAPI DecodeVirtualKey (UINT, LPSTR);

BOOL WINAPI IsMessageFiltered (UINT, HWND);
BOOL WINAPI InstallHooks (HWND);
BOOL WINAPI RemoveHooks (void);
void WINAPI MsgStringToMsgNumber (BOOL);
void WINAPI HwndStringToHwndNumber (BOOL);
void WINAPI LparamStringToLparamNumber (BOOL);
void WINAPI WparamStringToWparamNumber (BOOL);
void WINAPI ChangeFilterOptions (DWORD, UINT, UINT, BOOL);
void WINAPI ClearMsgList (void);

// Option Bits for Message filtering

// Non-Client area messages, NOT including Mouse message
#define MSG_NON_CLIENT      0x0000001

// Mouse message, INCLUDING non-client area mouse messages
#define MSG_MOUSE           0x0000002

// WM_KEY* message
#define MSG_KEYBOARD        0x0000004

// Dynamic Data Exchange transaction messages
#define MSG_DDE             0x0000008

// Clipboard transaction messagea
#define MSG_CLIPBOARD       0x0000010

// WM_USER messages
#define MSG_WM_USER         0x0000020

// WM_TIMER messages
#define MSG_TIMER           0x0000040

// MDI-specific messages
#define MSG_MDI             0x0000080

// Idle time messages and menu related messages, NOT including
// WM_COMMAND or WM_SYSCOMMAND messages
#define MSG_MENU            0x0000100

// System messages including WM_SYSCOMMAND
#define MSG_SYSTEM          0x0000200

// Windows 3.1 specific messages
#define MSG_WIN31_MSGS      0x0000400

// Messages specificied with in a given range. This flag
// overrides any other filter flag
#define MSG_MSG_RANGE_ONLY  0x0000800

// Windows 4.0 specific messages
#define MSG_WIN40_MSGS      0x0001000

// Control Notification messages
#define MSG_LB_MSGS         0x0004000
#define MSG_CB_MSGS         0x0008000
#define MSG_EDT_MSGS        0x0010000
#define MSG_BTN_MSGS        0x0020000
#define MSG_STT_MSGS        0x0040000

// Any other message on the system
#define MSG_UNKNOWN         0x0080000

// WM_CTL* messages
#define MSG_CTL_MSGS        0x0100000

// Standars messages such as WM_COMMAND, WM_SIZE, etc.
// Any other message that will be be on the previous
// categories
#define MSG_STANDARD_MSGS   0x0200000

// Combined control notification flag
#define MSG_CONTROL_MSGS    (MSG_LB_MSGS | MSG_CB_MSGS | \
                             MSG_EDT_MSGS | MSG_BTN_MSGS | MSG_STT_MSGS)

// Filter on ALL messages on the system
#define MSG_ALL_MSGS        (MSG_NON_CLIENT | MSG_MOUSE | MSG_KEYBOARD | \
                             MSG_DDE | MSG_CLIPBOARD | MSG_WM_USER | \
                             MSG_TIMER | MSG_MDI | MSG_MENU | \
                             MSG_SYSTEM | MSG_CONTROL_MSGS | \
                             MSG_CTL_MSGS | MSG_STANDARD_MSGS | \
                             MSG_WIN40_MSGS | MSG_WIN31_MSGS)
                             /*MSG_UNKNOWN*/

// Default flags for monotoring on the system
#define MSG_DEFAULT         (MSG_KEYBOARD | MSG_STANDARD_MSGS | \
                             MSG_DDE | MSG_CLIPBOARD | MSG_WM_USER | \
                             MSG_TIMER | MSG_MDI | \
                             MSG_SYSTEM | MSG_CONTROL_MSGS)

// Max and min number of messages in list
#define MIN_MSG_BUFFER  10
#define MAX_MSG_BUFFER  100


// Obsolete Messages Not defined in Win32
#define WM_CTLCOLOR             0x0019
#define WM_COMMNOTIFY           0x0044
#define WM_SYSTEMERROR          0x0017

#define HOOK_SHARED_DATA_MEM_FILE       "HookSampleSharedDataFile"
#define HOOK_MSG_DATA_MEM_FILE          "HookSampleMsgDataFile"

#define HWND_STRING_SIZE        31
#define MESSAGE_STRING_SIZE     29
#define PARAM_STRING_SIZE       64
typedef struct tagHMMSG
{
    WPARAM  wParam;
    LPARAM  lParam;
    HWND    hWnd;
    UINT    message;
    char    szhWnd[HWND_STRING_SIZE+1];
    char    szMessage[MESSAGE_STRING_SIZE+1];
    char    szwParam[PARAM_STRING_SIZE+1];
    char    szlParam[PARAM_STRING_SIZE+1];
} HMMSG;

typedef struct tagHOOKSDATA
{
    HMMSG   Msgs[MAX_MSG_BUFFER];
} HOOKSDATA, *PHOOKSDATA;

typedef struct tagGLOBOPTS
{
    // Handle of the WH_GETMESSAGE Hook
    HHOOK   hWH_GETMESSAGEHook;

    // Handle of the WH_CALLWNDPROC Hook
    HHOOK   hWH_CALLWNDPROCHook;

    // Handle of the windows being monitored. If this handle is null, the
    // hook will be monitoring on ALL windows in the system
    HWND    hWndMonitored;

    // This handle identifies the window that will be notified
    // (with a WM_COPYDATA message) from the hook DLL. The windows processing
    // this message is responsible for sending the output to the Com port and
    // to the log file. Usually this wil be the Window Procedure 
    // of the main window
    HWND    hMsgWindow;

    // This is the handle to the list box where the strings will be displayed
    HWND    hListBox;

    // Set of flag bits used to filter incoming messages. Look at the message
    // filter options flags for a description of the available options
    DWORD   dwMsgFilterOptions;

    // Used in the internal control of the message list
    BOOL    fWrapped;
    int     nIndex;
    int     nListBoxSize;

    // If TRUE, the message filter will allow message intended for child 
    // windows of the monitored window
    BOOL    fMonitorChild;
    
    // Range of message to filter
    UINT    uLoRange;
    UINT    uHiRange;

    // If TRUE, the output will be sent to the window
    BOOL    fOutputToWindow;

    // If TRUE, the output will be sent to the COM port
    BOOL    fOutputToCom;

    // If TRUE, the output will written to a log file 
    BOOL    fOutputToLogFile;

    // If TRUE, the message will be displayed in text format
    // If FALSE, the field will be given in a HEX-numeric format
    BOOL    fHwndInText;
    BOOL    fMessageInText;
    BOOL    fWParamInText;
    BOOL    fLParamInText;

    // File Name buffer...
    char    szOutputFile[MAX_PATH];
} GLOBOPTS, *PGLOBOPTS;
