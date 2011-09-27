//---------------------------------------------------------------------------
//	CMNDLG32.H
//---------------------------------------------------------------------------
#define STRICT

#ifdef	RC_INVOKED
#include <winuser.h>
#else	// RC_INVOKED
#include <windows.h>
#endif	// RC_INVOKED
#include "resource.h"

// window ids
#define ID_EDITCHILD		500

// string constants

#define IDS_DIALOGFAILURE     1
#define IDS_STRUCTSIZE        2
#define IDS_INITIALIZATION    3
#define IDS_NOTEMPLATE        4
#define IDS_NOHINSTANCE       5
#define IDS_LOADSTRFAILURE    6
#define IDS_FINDRESFAILURE    7
#define IDS_LOADRESFAILURE    8
#define IDS_LOCKRESFAILURE    9
#define IDS_MEMALLOCFAILURE  10
#define IDS_MEMLOCKFAILURE   11
#define IDS_NOHOOK           12
#define IDS_SETUPFAILURE     13
#define IDS_PARSEFAILURE     14
#define IDS_RETDEFFAILURE    15
#define IDS_LOADDRVFAILURE   16
#define IDS_GETDEVMODEFAIL   17
#define IDS_INITFAILURE      18
#define IDS_NODEVICES        19
#define IDS_NODEFAULTPRN     20
#define IDS_DNDMMISMATCH     21
#define IDS_CREATEICFAILURE  22
#define IDS_PRINTERNOTFOUND  23
#define IDS_NOFONTS          24
#define IDS_SUBCLASSFAILURE  25
#define IDS_INVALIDFILENAME  26
#define IDS_BUFFERTOOSMALL   27
#define IDS_FILTERSTRING     28
#define IDS_UNKNOWNERROR     29

// constants

#define MAX_PATH             260

// Function prototypes

// procs
long APIENTRY MainWndProc(HWND, UINT, UINT, LONG);
BOOL APIENTRY About(HWND, UINT, UINT, LONG);
BOOL APIENTRY EnterNew(HWND, UINT, UINT, LONG);
BOOL CALLBACK ComDlg32DlgProc(HWND, UINT, WPARAM, LPARAM);

//functions
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
BOOL OpenTheFile( HWND, HWND );
void ProcessCDError(DWORD, HWND);
BOOL NEAR PASCAL TestNotify(HWND, LPOFNOTIFY);
