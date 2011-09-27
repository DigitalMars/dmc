
#define IDD_SUBCLSDLG               100
#define IDM_EDIT                    101
#define IDM_INSERT                  103
#define IDM_OVERRIDE                104
#define IDM_LIST                    106
#define IDM_INSITEM                 107
#define IDM_DELETE                  108


#define OVERRIDE                    0
#define INSERT                      1

// user defined message for this dialog box. 

#define WM_CHANGECARET   WM_USER + 500


// Extern Variables for Modal.c and Subcls.c

extern HWND ghDlgList ;
extern WORD gIDControl;

// function prototype...

LRESULT CALLBACK Modal(HWND, UINT, WPARAM, LPARAM);
