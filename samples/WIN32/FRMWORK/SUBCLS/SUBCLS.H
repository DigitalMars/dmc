// Define the number of controls that are subclassed. 

#define IDM_NUMSUBCLS  2

// Callback functions.  These are called by Windows.

// **TODO**  Add new callback function prototypes here.  Win16 compiles
//           require the __export keyword to generate proper prolog
//           and epilog code for exported functions.

#ifdef WIN16

LRESULT CALLBACK __export SubClsEdit(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK __export SubClsList(HWND, UINT, WPARAM, LPARAM);

#else

LRESULT CALLBACK SubClsEdit(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SubClsList(HWND, UINT, WPARAM, LPARAM);


#endif

// Function prototypes specific to Subcls Module. 

LRESULT EditMsgChar(HWND, UINT, WPARAM, LPARAM);
LRESULT EditMsgKeyDown(HWND, UINT, WPARAM, LPARAM);
LRESULT ListMsgKeydown(HWND, UINT, WPARAM, LPARAM);
BOOL    SubClsControl(HWND, WORD) ;
void    ChangeCaret(HWND, BOOL) ;

typedef struct _DCWP
{
HWND       hWndChild ;
WNDPROC 	  origWndProc;
} DCWP;                   // Structur for calling original Window Procedure

