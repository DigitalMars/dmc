#define UMSG_DECREMENTDLGCOUNT (WM_USER+1)



HANDLE hInst ;

BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
long FAR PASCAL MainWndProc(HWND, UINT, UINT, LONG);
void HandleTheCommand(HWND, UINT, LONG) ;
BOOL APIENTRY AboutProc(HWND, UINT, UINT, LONG) ;

TCHAR szTemp[100] ;

TCHAR szShortFilter[5] ;
TCHAR szLongFilter[5] ;

#define IDM_COLOR   100
#define IDM_FONT    101
#define IDM_FIND    102
#define IDM_TITLE   103
#define IDM_OPEN    104
#define IDM_SAVE    105
#define IDM_PRINT   106
#define IDM_REPLACE 107
#define IDM_EXIT    108
#define IDM_HEXMODE 200
#define IDM_DECIMALMODE 201
#define IDM_MULTITHREAD2 300
#define IDM_ABOUT   301


UINT nFindMsg ;
UINT nOpenShareVMsg ;
UINT nHelpMessage ;
