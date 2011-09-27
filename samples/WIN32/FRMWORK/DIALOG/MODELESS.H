// Dialog box procedures must be exported in 16-bit applications for Windows.
#ifdef WIN16
LRESULT CALLBACK __export Modeless(HWND, UINT, WPARAM, LPARAM);
#else
LRESULT CALLBACK Modeless(HWND, UINT, WPARAM, LPARAM);
#endif

extern DLGPROC lpProcModeless;
extern HWND    hwndModeless;

#define IDD_MODELESSDIALOG          101
#define IDD_RED                     104
#define IDD_GREEN                   105
#define IDD_BLUE                    106
