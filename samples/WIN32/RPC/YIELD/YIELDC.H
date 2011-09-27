#ifdef WIN16
#define APIENTRY     PASCAL
#define UNREFERENCED_PARAMETER
#endif

#define IDM_ABOUT    100    /* menu */
#define IDM_BIND     200    /* menu */
#define IDM_WAIT     300    /* menu */
#define IDM_YIELD    400    /* menu */
#define IDM_EXIT     500    /* menu */

#define IDD_PROT_SEQ 201    /* control for Protocol Sequence */
#define IDD_NET_ADDR 203    /* control for Network Address   */
#define IDD_ENDPOINT 202    /* control for Endpoint          */
#define IDD_WAITTIME 301    /* control for # seconds to wait */

#define IDD_CUSTOM   403    /* radio button selection */
#define IDD_STD_USER 402    /* radio button selection */
#define IDD_STD_RPC  401    /* radio button selection */

#define NETLEN        32    /* network address length       */
#define PATHLEN      260    /* Path                         */
#define MSGLEN       300    /* error message in display box */
#define MAXPROTSEQ    20    /* protocol sequence "ncacn_np" */

#define DEFAULT_ENDPOINT    "\\pipe\\yield"
#define DEFAULT_PROT_SEQ    "ncacn_np"
#define DEFAULT_WAIT        15         /* # seconds to wait */

#define WM_RPC_YIELD_MESSAGE WM_USER+1 

#define YIELD_START_MSG      "Yield period start..."
#define YIELD_END_MSG        "Yield period complete"
#define YIELD_PAINT_MSG      "Paint during yield..."

#define EXCEPT_MSG          "The remote procedure call raised an exception. \
Please make sure the server application is running and \
that the network address and endpoint are correct."


/* Function prototypes; same as Win SDK Generic sample */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
LONG APIENTRY MainWndProc(HWND, UINT, UINT, LONG);

/* Function prototypes specific to this RPC Yield sample */
BOOL APIENTRY About(HWND, UINT, UINT, LONG);        /* dialog function */
BOOL APIENTRY GetBindInfo(HWND, UINT, UINT, LONG);  /* dialog function */
BOOL APIENTRY GetYieldInfo(HWND, UINT, UINT, LONG); /* dialog function */
BOOL APIENTRY GetWaitInfo(HWND, UINT, UINT, LONG);  /* dialog function */
RPC_STATUS Bind(HWND); /* utility that calls RPC API functions */
