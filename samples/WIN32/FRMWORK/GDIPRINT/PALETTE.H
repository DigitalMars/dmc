// palette defines
#define PALVERSION 0x300    // palette version

// function prototypes
LRESULT  ProcessPaletteChanged(HWND, WPARAM);
LRESULT  ProcessQueryNewPalette(HWND);
BOOL     IsPaletteDevice(void);     
int      PalEntriesOnDevice(HDC); 
int      ColorsInPalette(HPALETTE);
HPALETTE GetSystemPalette(void);
