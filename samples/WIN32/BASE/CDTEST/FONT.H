void DoFontDialog(HWND) ;
BOOL APIENTRY FontProc(HWND, UINT, UINT, LONG) ;


#define ID_FONTDIALOG               607
#define WM_CF_LF                    (WM_USER+2)

#define ID_STRUCTSIZEF              116
#define ID_HWNDOWNERF               117
#define ID_HDCF                     118
#define ID_LOGFONTF                 119
#define ID_POINTSIZEF               120
#define ID_FLAGSF                   121
#define ID_RGBCOLORSF               122
#define ID_CUSTDATAF                123
#define ID_HOOKF                    124
#define ID_TEMPLATEF                125
#define ID_HINSTANCEF               126
#define ID_STYLEF                   127
#define ID_FONTTYPEF                128
#define ID_SIZEMINF                 129
#define ID_SIZEMAXF                 130
#define ID_ERRORF                   135
#define ID_RETURNF                  136
#define ID_HDCSCREEN                137
#define ID_HDCPRINTER               138
#define ID_HDCNULL                  131
#define ID_EDITLOGFONT              132
#define ID_RESETFONT                139
#define ID_HDCINVALID               609
#define ID_NULLSTRUCTFONT           608
#define ID_PRELOADEDFONT            610
#define ID_FONTLIST                 611
#define F_ANSIONLY                  614
#define F_TTONLY                    615
#define F_SCREENFONTS               616
#define F_PRINTERFONTS              617
#define F_FIXEDPITCHONLY            618
#define F_NOOEMFONTS                619
#define F_NOSIMULATIONS             620
#define F_NOVECTORFONTS             621
#define F_SCALABLEONLY              622
#define F_WYSIWYG                   623
#define ID_MULTITHREADFONT          624
#define ID_SEND_WM_CF_LF_MSG        625
