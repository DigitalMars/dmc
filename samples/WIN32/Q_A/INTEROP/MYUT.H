
#define MYPRINT 1
#define GETHWND 2

#define WF_WINNT 0x4000

DWORD FAR PASCAL LoadLibraryEx32W( LPCSTR, DWORD, DWORD );
BOOL FAR PASCAL FreeLibrary32W( DWORD );
DWORD FAR PASCAL GetProcAddress32W( DWORD, LPCSTR );
DWORD FAR PASCAL CallProc32W( DWORD, DWORD, DWORD, LPVOID, DWORD, DWORD );
