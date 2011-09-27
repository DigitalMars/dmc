// **********************************************************************  
//                                                                         
// Constants to determine the size of data structures, string lengths,     
// etc.                                                                    
//                                                                         
// **********************************************************************  

#define CS_ENABLE           1
#define CS_DISABLE          2
#define CS_DELETE_TRANSFORM 3

#define BMP_1			1	// Constants used to track current BMP
#define BMP_4			2	// information.  MAX_BMP must be one
#define BMP_8			3	// greater than number of BMP's used
#define BMP_16		        4	// to allow functions to use these
#define BMP_555 	        5	// constants as indices to array.
#define BMP_565 	        6
#define BMP_24		        7
#define BMP_32		        8
#define BMP_888 		9
#define BMP_8_DOWN		10
#define BMP_1_ICM	        11
#define BMP_4_ICM	        12
#define BMP_8_ICM	        13
#define BMP_16_ICM		14
#define BMP_555_ICM		15
#define BMP_565_ICM		16
#define BMP_24_ICM		17
#define BMP_32_ICM		18
#define BMP_888_ICM		19
#define BMP_CMYK_ICM		20
#define BMP_FACES		21
#define BMP_SUNSET		22
#define BMP_CHURCH		23
#define BMP_REDBUSH		24
#define BMP_RED_GRAD		25
#define BMP_GREEN_GRAD		26
#define BMP_BLUE_GRAD		27
#define BMP_CYAN_GRAD		28
#define BMP_MAGENTA_GRAD	29
#define BMP_YELLOW_GRAD 	30
#define BMP_BW_GRAD		31
#define BMP_RECY_GRAD		32
#define BMP_GRMA_GRAD		33
#define BMP_BLYE_GRAD		34
#define BMP_24_BIG		35
#define MAX_BMP 	        36

#define MAXPALETTE          256     // max. # supported palette entries     
#define MAXREAD             32768   // Number of bytes to be read during    

//
// A few misc. string lengths
//
#define MAX_CHAR            256
#define MAX_COLORSPACES     16
#define MAX_FRIENDLY_NAME   32  
#define MAX_COLORSPACE_NAME 256

#define FIRST_ROW           50
#define FIRST_COL           165
#define ROW_HEIGHT          35
#define COL_WIDTH           140
#define COL_SPACING         100

// **********************************************************************  
//                                                                         
// Constants to determine the size of data structures, string lengths,     
// etc.                                                                    
//                                                                         
// **********************************************************************  
#define DIB8      1
#define DIB16     2
#define DIB24     3
#define DIB24BIG  4

// **********************************************************************  
//                                                                         
// Macros to display/remove hourglass cursor for lengthy operations        
//                                                                         
// **********************************************************************  
#define StartWait() ghcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT))
#define EndWait()   SetCursor(ghcurSave)

// **********************************************************************  
//                                                                         
// Macros to ensure Win16/Win32 portability                                
//                                                                         
// **********************************************************************  

#ifndef WIN32

#define MoveToEx(hDC, X, Y, lpPoint)  MoveTo(hDC, X, Y)
#define HUGE  huge

#else

#define HUGE 
#endif

// **********************************************************************  
//                                                                         
// Plain Old Macros
//                                                                         
// **********************************************************************  

#define ABS(x)		(((x) >= 0) ? (x) : (-(x)))

#ifdef DEBUG
#define INT3(lpsz)                                            \
OutputDebugString((LPSTR)">>>");                              \
OutputDebugString((LPSTR)lpsz);                               \
OutputDebugString((LPSTR)"\r\n>>>Here comes an INT3!!\r\n");  \
_asm                                                          \
{                                                             \
  int     3                                                   \
}
#else
#define INT3(lpsz)                                            \
OutputDebugString(lpsz);                                      \
OutputDebugString("\r\n");
#endif

// **********************************************************************  
//                                                                         
// Typedefs for ICMTEST                                                    
//                                                                         
// **********************************************************************  
typedef struct tagBMPINFO 
{
  char      szBMP[128];     // Name of BMP
  HANDLE    hDib;           // Handle to memory DIB
  HBITMAP   hBmp;           // Handle to memory BITMAP
} BMPINFO, *lpBMPINFO;


// **********************************************************************  
//                                                                         
// Typedefs for ICM structures                                             
//                                                                         
// **********************************************************************  
typedef struct tagCOLORSPACECONTROLENTRY
{
  LOGCOLORSPACE   lcs;
  HCOLORSPACE     hlcs;
  char            szName[MAX_FRIENDLY_NAME];
} COLORSPACECONTROLENTRY;

// ***********************************************************************
//
//   Functions needed across files
//
// ***********************************************************************
void        DIBPaint (HDC,int,int);
BOOL        InitDIBs(void);
void        FreeDIBs (void);
int         ErrMsg (PSTR sz,...);
WORD        PaletteSize (VOID FAR * pv);
HDC PASCAL  GetPrinterDC (void);
BOOL	      PrintICMImages(HWND hWnd, int Page);
void        DebugOut (PSTR sz,...);
