                            
// menu commands

// Options menu
#define IDM_WIZARD      100
#define IDM_EXIT        101

// Help menu
#define IDM_ABOUT       200

// icons
#define EXE_ICON        300

// ids
#define ID_EDITCHILD	1000

// constants
#define NUM_PAGES	6
#define MAX_BUF		5000
#define MAX_LINE	512

// typedefs
typedef struct tagREVIEWINFO
{
    HINSTANCE hInst;        // current instance
	int iWorkHabits;
	int iTeamWork;
	int iReliability;
	int iGoals;
	int iAdaptation;
	char pszName[MAX_PATH];
	char pszTitle[MAX_PATH];
	char pszProject[MAX_PATH];
	char pszDepartment[MAX_PATH];
} REVIEWINFO;

// Function prototypes

// procs
long APIENTRY MainWndProc(HWND, UINT, UINT, LONG);
BOOL APIENTRY About(HWND, UINT, UINT, LONG);

// Pages for Wizard
BOOL APIENTRY WorkHabits(HWND, UINT, UINT, LONG);
BOOL APIENTRY TeamWork(HWND, UINT, UINT, LONG);
BOOL APIENTRY Reliability(HWND, UINT, UINT, LONG);
BOOL APIENTRY Goals(HWND, UINT, UINT, LONG);
BOOL APIENTRY Adaptation(HWND, UINT, UINT, LONG);

//functions
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
int CreateWizard(HWND, HINSTANCE);
void FillInPropertyPage( PROPSHEETPAGE* , int, LPSTR, DLGPROC);
void GenerateReview(HWND);

