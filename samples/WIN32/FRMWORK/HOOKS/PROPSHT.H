// Constants for PropSheets module

#define PROP_OPTION_PAGES   4
#define INITIALIZE          1
#define SHOW                2

#define TYPE_DLG        1
#define INPUT_DLG       2
#define OUTPUT_DLG      3
#define RANGE_DLG       4


// Function Prototypes for calls you can make into the PropSheets module
BOOL OptionsPropertySheets(UINT, HWND );	//Use to initialize and display the propert sheet



//#define PDIALOGFUNC(p) BOOL CALLBACK (*p)(HWND,UINT,WPARAM,LPARAM)
// Defines for all the controls in the property sheets.

//Dialog IDs
#define IDD_OUTPUT                     5101
#define IDD_INPUT                      5102
#define IDD_CATEGORY                   5103
#define IDD_RANGE                      5104

//Output IDs
#define IDC_WINDOW                     5201
#define IDC_COM1                       5202
#define IDC_DISK                       5203
#define IDC_FILENAME                   5209

//Input IDs
#define IDC_SINGLE                     5211
#define IDC_PLUSCHILD                  5212
#define IDC_ALLWIN                     5215

//Category IDs
#define IDC_MOUSE                      5216
#define IDC_KEYBOARD                   5217
#define IDC_MENU                       5218
#define IDC_NONCLIENT                  5219
#define IDC_CLIPBOARD                  5220
#define IDC_SYSTEM                     5221
#define IDC_TIMER                      5222
#define IDC_DDE                        5223
#define IDC_MDI                        5224
#define IDC_WIN31                      5225
#define IDC_USER                       5226
#define IDC_OTHER                      5227
#define IDC_STANDARD                   5228
#define IDC_CONTROL                    5229
#define IDC_UNKNOWN                    5230
#define IDC_STATICCTL                  5231
#define IDC_BUTTONCTL                  5232
#define IDC_EDITCTL                    5233
#define IDC_COMBOBOX                   5234
#define IDC_LISTBOX                    5235
#define IDC_WIN32                      5236
#define IDC_WIN40                      5237
#define IDC_RANGE                      5238


#define IDC_INCSTART                   5239
#define IDC_INCEND                     5240
#define IDC_STATIC                     5250
#define IDC_STATIC2                    5251

//Global Flags maintained by the property sheet's pages

#define MAX_MESSAGEID   65535           //Largest message number
#define MIN_MESSAGEID   0               //Smallest message number


//Option Bits for Message filtering
/*
#define MSG_NON_CLIENT      0x0000001
#define MSG_MOUSE           0x0000002
#define MSG_KEYBOARD        0x0000004
#define MSG_DDE             0x0000008
#define MSG_CLIPBOARD       0x0000010
#define MSG_WM_USER         0x0000020
#define MSG_TIMER           0x0000040
#define MSG_MDI             0x0000080
#define MSG_MENU            0x0000100
#define MSG_SYSTEM          0x0000200
#define MSG_WIN31_MSGS      0x0000400
#define MSG_MSG_RANGE_ONLY  0x0000800
#define MSG_WIN40_MSGS      0x0001000
#define MSG_WIN32_MSGS      0x0002000
#define MSG_LB_MSGS         0x0004000
#define MSG_CB_MSGS         0x0008000
#define MSG_EDT_MSGS        0x0010000
#define MSG_BTN_MSGS        0x0020000
#define MSG_STT_MSGS        0x0040000
#define MSG_UNKNOWN         0x0080000
#define MSG_CTL_MSGS        0x0100000
#define MSG_STANDARD_MSGS   0x0200000
#define MSG_OTHER           0x0400000
*/
#define IS_SINGLEWINDOW         0x01
#define IS_SINGLEWINANDCHILD    0x02
#define IS_ALLWINDOWS           0x04

#define OD_WINDOW               0x01
#define OD_COM1                 0x02
#define OD_FILE                 0x04

/*
#define MSG_CONTROL_MSGS    (MSG_LB_MSGS | MSG_CB_MSGS | \
			     MSG_EDT_MSGS | MSG_BTN_MSGS | MSG_STT_MSGS)

#define MSG_ALL_MSGS        (MSG_NON_CLIENT | MSG_MOUSE | MSG_KEYBOARD | \
			     MSG_DDE | MSG_CLIPBOARD | MSG_WM_USER | \
			     MSG_TIMER | MSG_MDI | MSG_MENU | \
			     MSG_SYSTEM | MSG_CONTROL_MSGS | MSG_UNKNOWN | \
			     MSG_CTL_MSGS | MSG_STANDARD_MSGS)

#define MSG_DEFAULT         (MSG_KEYBOARD | MSG_STANDARD_MSGS | \
			     MSG_DDE | MSG_CLIPBOARD | MSG_WM_USER | \
			     MSG_TIMER | MSG_MDI | \
			     MSG_SYSTEM | MSG_UNKNOWN)

*/
