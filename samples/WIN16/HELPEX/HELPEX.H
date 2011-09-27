/* file menu items */

#define     IDM_NEW      100
#define     IDM_OPEN     101
#define     IDM_SAVE     102
#define     IDM_SAVEAS   103
#define     IDM_PRINT    104
#define     IDM_EXIT     105

/* edit menu items */

#define     IDM_UNDO     200
#define     IDM_CUT      201
#define     IDM_COPY     202
#define     IDM_PASTE    203
#define     IDM_CLEAR    204

/* define help items */

#define     IDM_ABOUT          300
#define     IDM_HELP_INDEX     301
#define     IDM_HELP_KEYBOARD  302
#define     IDM_HELP_HELP      303

#define EXE_NAME_MAX_SIZE  128

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK About(HWND, UINT, WPARAM, LPARAM);
