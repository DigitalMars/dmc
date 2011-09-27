//*************************************************************************
//
//   General constants
//
//*************************************************************************

//
// Types of DCs.  Used to determine which color profiles to enumerate
//
#define SCREEN_DC 	0	
#define PRINTER_DC 	1

//*************************************************************************
//
//   Functions available to other modules
//
//*************************************************************************


    //
    // Dialog procedures.  These must be exported in WIN16 DEF file.
    //
LRESULT CALLBACK dlgprcAddLCS( HWND hDlg, UINT message, WPARAM uParam, LPARAM lParam );
BOOL FAR PASCAL  dlgprcDefineColorSpace(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam);
BOOL FAR PASCAL  dlgprcAbout(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SetDeviceProfile_DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    //
    // Functions for handling LCS control structure.
    //
BOOL LoadCSArrayIntoDlg(HWND hDlg, int iControlId);
void UpdateLCS(HWND hDlg, LPLOGCOLORSPACE lplcs);
LRESULT FindCurrentCSInArray(HWND hDlg, int iControlId);

    //
    // Functions for reading/writing LCS control structure to/from INI
    //
void ReadCSArrayFromINI(void);
void ReadLCSFromINI(LPLOGCOLORSPACE lplcs, char szName[]);
void WriteLCSArrayToINI(void);
void WriteLCSToINI(LPSTR lpszSection, LOGCOLORSPACE lcs);
DWORD GetPrivateProfileDword(LPSTR lpszSection, LPSTR lpszEntry, DWORD dwDefault, LPSTR lpszFilename);

extern char    gszColorSpace[MAX_CHAR];
