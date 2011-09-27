//defines for demo.c and prototypes. 


LRESULT CALLBACK DemoDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT GetDrivesOnSystem(HWND, HWND);
LRESULT GetLogDriveInfo(HWND, LPSTR);
LRESULT GetDiskInfo(HWND, LPSTR);
LRESULT GetVolumeInfo(HWND, LPSTR);

#define IDM_FILESYSTEMINFO	105

#define IDM_HELPCONTENTS   300
#define IDM_HELPSEARCH     301
#define IDM_HELPHELP       302

#define DLG_VERFIRST        400
#define DLG_VERLAST         404

//defines for file system information...

#define IDM_LOGDRIVES               107
#define IDM_VOLNAME                 112
#define IDM_DRIVETYPE					111
#define IDM_SERIALNUM               113
#define IDM_COMPLENGTH              114
#define IDM_SYSFLAGS                116
#define IDM_FILESYSTEM              117
#define IDM_FREEDISKINFO            109
#define IDM_SECPCLSTR               118
#define IDM_SECTORS                 119
#define IDM_BYTESINSECTOR           120
#define IDM_SECTORSINCLUSTER        121
#define IDM_FREECLUSTERS            122
#define IDM_CLUSTERS                123
