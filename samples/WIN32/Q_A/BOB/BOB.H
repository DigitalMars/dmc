typedef struct tagControlText {
    POINT   ptPosition;
    PTCHAR  szTitle;
    UINT    cbTitle;
    HBITMAP hDefaultButton;
    HBITMAP hSelectButton;
} CONTROLTEXT, FAR * LPCONTROLTEXT;


int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ButtonProc(HWND, UINT, WPARAM, LPARAM);
void HandleSelectedState(LPDRAWITEMSTRUCT lpdisCtrl, LPCONTROLTEXT lpCtlText);
void HandleFocusState(LPDRAWITEMSTRUCT lpdis, LPRECT lpFocusRect, LPRECT lpSelectRect );
void DrawBitmap(HDC hDC, RECT rect, HBITMAP hBitmap, DWORD rop);
BOOL CALLBACK About(HWND, UINT, WPARAM, LPARAM);

#define BOB  10
#define DEFAULTBUTTON  1
#define SELECTEDBUTTON 2

#define     SubclassWindow(hwnd, lpfn)	\
		((WNDPROC)SetWindowLong((hwnd), GWL_WNDPROC, (LPARAM)(WNDPROC)(lpfn)))
