#define idsName             100
#define idsXFrom            9001
#define idsYFrom            9002
#define idsXTo              9003
#define idsYTo              9004
#define idsSaveAs           9005
#define idsIteration        9006

#define FIRST_PIXEL     429496796
#define MAXDOUBLELEN        22          // big enough?
#define MAXPATHLEN          256

#define ID_XFROM              2000
#define ID_XTO                2001
#define ID_YFROM              2002
#define ID_YTO                2003
#define ID_SAVEAS             2004
#define ID_ITERATION          2005

extern TCHAR szName[TITLEBARNAMELEN];
extern TCHAR szAppName[APPNAMEBUFFERLEN];
TCHAR szXFrom[MAXDOUBLELEN];
TCHAR szXTo[MAXDOUBLELEN];
TCHAR szYFrom[MAXDOUBLELEN];
TCHAR szYTo[MAXDOUBLELEN];
TCHAR szSaveFile[MAXPATHLEN];
extern TCHAR szIniFile[MAXFILELEN];
TCHAR szIteration[MAXDOUBLELEN];
extern TCHAR szScreenSaver[22];
extern TCHAR szHelpFile[MAXFILELEN];
extern TCHAR szNoHelpMemory[BUFFLEN];
extern UINT  MyHelpMessage;

CHAR  gtext[128];
HPEN   hpnRed;
HPEN   hpnBlack;
HPEN   hpnGreen;



INT    giPen = 0;

BOOL   gFloat = TRUE;
LONG   gStep = 1;
LONG   gIteration = 500;
BOOL   gbStretch = TRUE;
INT    giStretchMode = COLORONCOLOR;
INT    giDmOrient = DMORIENT_PORTRAIT;
INT    giNPrinters = 0;

typedef struct _PerWndInfo {
    HWND    hwnd;               // hJulia or hView, the drawing surface
    HANDLE  hThrd;              // handle to drawing thread or play thread
    BOOL    bDrawing;           // curently drawing?
    DWORD   dwThreadId;         // drawing thread or play thread ID
    DWORD   dwElapsed;          // elapsed time for the drawing operation
    double  xFrom;              // drawing range in floating points
    double  xTo;                //
    double  yFrom;              //         Floating Points
    double  yTo;                //
    double  c1;                 // C = c1 + i c2 where c1, c2 are reals
    double  c2;                 // the C value corresponding to the Julia set
    LONG    lxFrom;             // drawing range in fix points
    LONG    lxTo;               //
    LONG    lyFrom;             //         Fix Points
    LONG    lyTo;               //
    LONG    lc1;                // C = c1 + i c2 where c1, c2 are reals
    LONG    lc2;                // the C value corresponding to the Julia set
    HBITMAP hBmpSaved;          // saved bitmap for the drawing
    BOOL    bMandel;            // Drawing is a Mandelbrot or Julia set?
    int     iIteration;         // number of computations done on each pixel
    int     iStep;              // drawn on every one, two or three line(s)?
    BOOL    bStretch;           // stretching bitmaps
    int     iStretchMode;       // stretching mode to use
    BOOL    bSetDIBsToDevice;   // SetDIBsToDevice reather than BitBlt?
    HPALETTE    hPal;           // The palette used for the drawing
    HPALETTE    hHTPal;         // The halftone palette used for the drawing
    PVOID   *prghPen;
    INT     iPen;
    char    szSavef[MAXPATHLEN];
    BOOL    bClrCycle;
} INFO, *PINFO;

INFO  gInfo;


//
// Handy macros for the transformations used in fractal generations
//
#define Xform(Pt, SrcFrom, SrcTo, DestFrom, DestTo) \
         (((Pt-SrcFrom)/(SrcTo-SrcFrom)*(DestTo-DestFrom))+DestFrom)
#define Xform2(Pt, SrcFrom, SrcTo, DestFrom, DestTo) \
         ((int) ((Pt-SrcFrom)/(SrcTo-SrcFrom)*(DestTo-DestFrom)+DestFrom))
#define XformFix(Pt, SrcFrom, SrcTo, DestFrom, DestTo) \
         (lMul(lDiv(Pt-SrcFrom,SrcTo-SrcFrom), DestTo-DestFrom)+DestFrom)
