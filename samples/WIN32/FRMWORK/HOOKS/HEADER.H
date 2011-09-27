#define IDM_HEADERCTL       501
#define TOTAL_TABS          3

BOOL CreateHeaderCtl(HWND);
BOOL InitializeHeaderCtl(HWND);
BOOL InsertHeaderItem(HWND, int, int, LPSTR);
void TrackHeader(int, int, int);
LRESULT MsgSize(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgHeaderNotify(HWND, UINT, WPARAM, LPARAM);


