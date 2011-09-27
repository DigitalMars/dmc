// Dialog box procedures must be exported in 16-bit applications for Windows.
#ifdef WIN16
LRESULT CALLBACK __export Modal(HWND, UINT, WPARAM, LPARAM);
#else
LRESULT CALLBACK Modal(HWND, UINT, WPARAM, LPARAM);
#endif

#define IDD_MODALDIALOG             100
#define IDD_STUDENTS                101
#define IDD_NAME                    104
#define IDD_TITLE                   106
#define IDD_PHONE                   108
#define IDD_INCLUDEBOYS             109
