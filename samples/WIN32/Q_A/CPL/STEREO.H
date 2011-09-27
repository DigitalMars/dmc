#define CPCAPTION      1

#define AMP_ICON     100
#define AMP_NAME     101
#define AMP_DESC     102
#define AMP_DLG      103

#define TUNER_ICON   200
#define TUNER_NAME   201
#define TUNER_DESC   202
#define TUNER_DLG    203

#define TAPE_ICON    300
#define TAPE_NAME    301
#define TAPE_DESC    302
#define TAPE_DLG     303


// function prototypes
LONG CALLBACK CPlApplet(HWND, UINT, LONG, LONG);
BOOL APIENTRY AmpDlgProc(HWND, UINT, UINT, LONG);
BOOL APIENTRY TunerDlgProc(HWND, UINT, UINT, LONG);
BOOL APIENTRY TapeDlgProc(HWND, UINT, UINT, LONG);

