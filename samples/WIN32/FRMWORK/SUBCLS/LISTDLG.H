
// define for Insert Dialog

#define IDD_INSDIALOG               1111
#define IDD_INSCLEAR                1112
#define IDD_INSEDIT                 1113

// defines for Delete Dialog

#define IDD_DELDIALOG               2222
#define IDD_DELCLEAR                2223
#define IDD_DELEDIT                 2224

// Function Prototypes specific to lstdlg...

void DispInsDialog(HWND, WORD) ;
void DispDelDialog(HWND, WORD) ;
LRESULT CALLBACK Insert(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Delete(HWND, UINT, WPARAM, LPARAM);

LRESULT MsgListInsertCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgListDeleteCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdListInsertDone(HWND, WORD, WORD, HWND);
LRESULT CmdListInsertClear(HWND, WORD, WORD, HWND);
LRESULT CmdListDeleteDone(HWND, WORD, WORD, HWND);
LRESULT CmdListInsertOk(HWND, WORD, WORD, HWND);
LRESULT CmdListDeleteOk(HWND, WORD, WORD, HWND);





