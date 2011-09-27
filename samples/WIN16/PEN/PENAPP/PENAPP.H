/**********************************************************************

      Header file for PENAPP.C


**********************************************************************/

#ifndef  _INC_PENAPP
#define  _INC_PENAPP

/******** Prototypes *********/

LRESULT CALLBACK MainWndProc( HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK InputWndProc( HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK InfoWndProc( HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK RawWndProc( HWND, UINT, WPARAM, LPARAM);

BOOL InitInstance( HANDLE hInstance, int cmdShow );
BOOL InitApp( HANDLE hInstance );
VOID CopyRawData( LPRCRESULT lprcresult );
VOID DisplayGuesses( HDC hdc );
VOID DrawArrow( HWND hwnd, HDC hdc );
VOID DrawMirrorImage( HDC hdc );
VOID DrawRawData( HDC hdc );
VOID MakeMirrorImage( HPENDATA hpendata );
int  NSetExtents( HDC hdc, LPPENDATAHEADER lppndt, LPRECT lprectWnd );
VOID ResetWindow( int mi );

/******** Macros *********/

//
// GetLPWidth(): Convert given ink width into logical coordinates.  For
// these purposes, logical coordinates are actually tablet coordinates.
//
#define GetLPWidth( nInkWidth, pnLPWidth )      \
{                                               \
      POINT ptT;                                \
      ptT.x = nInkWidth;                        \
      ptT.y = nInkWidth;                        \
      DPtoTP( &ptT, 1 );                        \
      *pnLPWidth = ptT.x;                       \
}

//
// The following macros return window dimensions.
//
#define  XRawWnd( dxParent )  (0)
#define  YRawWnd( dyParent )  (DyInfoWnd( dyParent ))
#define  DxRawWnd( dxParent ) ((dxParent)/3)
#define  DyRawWnd( dyParent ) ((dyParent)-DyInfoWnd( dyParent ))

#define  XInfoWnd( dxParent ) (0)
#define  YInfoWnd( dyParent ) (0)
#define  DxInfoWnd( dxParent )   ((dxParent)/3)
#define  DyInfoWnd( dyParent )   ((dyParent)/2)

#define  XInputWnd( dxParent )   (DxRawWnd( dxParent ))
#define  YInputWnd( dyParent )   (0)
#define  DxInputWnd( dxParent )  (dxParent-XInputWnd( dxParent ))
#define  DyInputWnd( dyParent )  (dyParent-YInputWnd( dyParent ))


/******** Constants *********/

#define rgbBlue      RGB( 0, 0, 255 )        // Color of up-strokes

#define szPenAppWnd  "Sample Pen App"        // Window titles
#define szInputWnd   "Input"
#define szInfoWnd "Info"
#define szRawWnd  "Raw Data"

#define szPenAppClass      "PASampleClass"      // Window class names
#define szPenAppInputClass "PAInputClass"
#define szPenAppInfoClass  "PAInfoClass"
#define szPenAppRawClass   "PARawClass"

#define szSampleRec     "SREC.DLL"           // Recognizer DLL name

#define MAX_CHAR  20                   // Maximum number of chars
#define MAX_GUESS 5                    // Maximum number of guesses

#define dwLength  80                   // Arrow and border dimensions
#define dxArrow      10
#define dyArrow      5
#define cBorder      5

#endif   // !_INC_PENAPP



/**********************************************************************

Resource constants for PENAPP.C

**********************************************************************/


/******** Menu Definitions *********/

#define menuPenApp   1        // Menu index
#define miExit    100
#define miSystem  200
#define miSample  201
#define miMirror  202

/******** Icons *********************/

#define iconPenApp 1
