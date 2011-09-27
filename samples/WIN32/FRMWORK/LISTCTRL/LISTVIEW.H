LRESULT MsgNotify(HWND, UINT, WPARAM, LPARAM);
int CALLBACK ListViewCompareProc(LPARAM, LPARAM, LPARAM);
HWND WINAPI CreateListView(HWND hwndParent);
void SwitchView(HWND, DWORD);


// Constants

#define IDC_NUMCOLUMNS        7
#define IDD_LISTVIEW        100
//#define IDC_NUMITEMS          7
#define IDC_NUMITEMS          5

#define IDC_MAXCOLUMNHDG     31
#define IDC_MAXCOMPANYLEN    31
#define IDC_MAXSYMBOLLEN      6
#define IDC_MAXEXCHANGELEN   10

#define LVIS_GCNOCHECK  0x1000
#define LVIS_GCCHECK    0x2000

// Numeric constants for the stock icons

#define IDC_ONE    1
#define IDC_BORL   2
//#define IDC_DIS    3
//#define IDC_HNZ    4
#define IDC_INTC   3
#define IDC_EK     4
#define IDC_MSFT   5

// String table constants for listview column headings.
// These begin with 3 because of the global string table
// entries in GLOBALS.H

#define IDS_COMPANYCOLUMN     3
#define IDS_SYMBOLCOLUMN      4
#define IDS_EXCHANGECOLUMN    5
#define IDS_HIGHCOLUMN        6
#define IDS_LOWCOLUMN         7
#define IDS_CLOSECOLUMN       8
#define IDS_NEWSCOLUMN        9

#define IDB_CHECKSTATES       1

// Stock info struct

typedef struct tagSTOCKINFO
{
    char szCompany [IDC_MAXCOMPANYLEN];
    char szSymbol  [IDC_MAXSYMBOLLEN];
    char szExchange[IDC_MAXEXCHANGELEN];
    UINT iHigh;
    UINT iLow;
    UINT iClose;
    char szNews[4];
}STOCKINFO;
