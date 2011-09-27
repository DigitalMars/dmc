/******************************Module*Header*******************************\
* Module Name: glscrnsv.c
*
* Companion file to scrnsave.c.  Hooks out any changes in functionality
* defined as GL_SCRNSAVE in scrnsave.c, and does general intialization.
*
* Copyright (c) 1996 Microsoft Corporation
*
\**************************************************************************/

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include "scrnsave.h"

#include "glscrnsv.h"
#include "ssintrnl.hxx"
#include "sswindow.hxx"
#include "glscrnsv.hxx"
#include "sswproc.hxx"

static UINT (*KeyDownFunc)(int)        = NULL;

// Global ptr to screen saver instance
SCRNSAVE *gpss = NULL;

// Global strings.
#define GEN_STRING_SIZE 64
TCHAR szScreenSaverTitle[GEN_STRING_SIZE];
extern TCHAR szClassName[];  // from sswindow.cxx
LPCTSTR pszWindowClass = TEXT("WindowsScreenSaverClass");  // main class name
TCHAR szClassName[] = TEXT("ScreenSaverClass"); // child class name

// forward declarations of internal fns
static BOOL RegisterMainClass( WNDPROC wndProc, HBRUSH hbrBg, HCURSOR hCursor );
static BOOL RegisterChildClass();
static BOOL AttemptResolutionSwitch( int width, int height, ISIZE *pNewSize );

// externs
extern void InitRealScreenSave(); // scrnsave.cxx
extern LRESULT WINAPI
    RealScreenSaverProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
extern VOID UnloadPwdDLL(VOID); 
extern BOOL GLScreenSaverConfigureDialog( HWND hDlg, UINT msg, WPARAM wParam,
                              LPARAM lParam ); // sswproc.cxx

#ifdef SS_INITIAL_CLEAR
static void InitialClear( PSSW *pssw );
#endif

#ifdef SS_WIN95
#define SS_WIN95_MULTIWINDOW_WORKAROUND 1
#define SS_WIN95_DESKTOP_REDRAW_FIX 1
static BOOL bPowerSaveEnabled();
#endif

/**************************************************************************\
* GLDoScreenSave
*
* Runs the screen saver in the specified mode
*
* GL version of DoScreenSave in scrnsave.c
*
* Does basic init, creates initial set of windows, and starts the message
* loop, which runs until terminated by some event.
*
\**************************************************************************/

static int
GLDoScreenSave( int winType, LPARAM lParam )
{
    MSG msg;

    // Create screen saver instance - this calls ss_Init()
    SCRNSAVE ss( winType, lParam );

    // Setup all the windows and start the message loop

    if( ss.SetupInitialWindows() )
    {
        // Send message to main window to start the drawing timer
#ifdef SS_DELAYED_START_KLUGE
        // Kluge to work around 'window-not-ready' problem in child
        // preview mode - trigger off of WM_PAINT instead
        if( ! SS_DELAY_START(winType) )
            SendMessage( ss.psswMain->hwnd, SS_WM_START, 0, 0 );
#else
        SendMessage( ss.psswMain->hwnd, SS_WM_START, 0, 0 );
#endif

        while( GetMessage( &msg, NULL, 0, 0 ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    // We're done - screen saver exiting.

    // free password-handling DLL if loaded
    UnloadPwdDLL();

    return msg.wParam;
}

/**************************************************************************\
* SCRNSAVE constructors
*
\**************************************************************************/

SCRNSAVE::SCRNSAVE( int typeArg, LPARAM lParam )
{
    type = typeArg;
    initParam = lParam;
    Init();
}

SCRNSAVE::SCRNSAVE( int typeArg )
{
    type = typeArg;
    initParam = 0;
    Init();
}

void
SCRNSAVE::Init()
{
    psswMain    = NULL;
    psswGL      = NULL;
    bResSwitch  = FALSE;
    pssc        = NULL;
    pssPal      = NULL;
    flags       = 0;
#ifdef SS_DEBUG
    bDoTiming   = type == SS_TYPE_NORMAL ? TRUE : FALSE;
#endif

    // Global ptr to the screen saver instance
    gpss = this;

    // Platform detections

    ss_QueryOSVersion();

    // Initialize randomizer

    ss_RandInit();

    // Create multi-purpose black bg brush

    hbrBg = (HBRUSH) GetStockObject( BLACK_BRUSH );

    // Call client ss's init function, to get ptr to its configuration
    // request

    if( type == SS_TYPE_CONFIG ) {
        // This case handled differently
        return;
    }

    pssc = ss_Init();
    SS_ASSERT( pssc, "SCRNSAVE constructor failure\n" );

    // Set GL config structure from pssc
    
    GLc.pfFlags = 0;
    GLc.hrc = 0;
    GLc.pStretch = NULL;
    switch( pssc->depthType ) {
        case SS_DEPTH16 :
            GLc.pfFlags |= SS_DEPTH16_BIT;
            break;
        case SS_DEPTH32 :
            GLc.pfFlags |= SS_DEPTH32_BIT;
            break;
    }
    if( pssc->bDoubleBuf )
        GLc.pfFlags |= SS_DOUBLEBUF_BIT;
    if( pssc->bStretch )
        GLc.pStretch = &pssc->stretchInfo;

}

/**************************************************************************\
* SetupInitialWindows
*
* Create / Configure all required windows.
*
\**************************************************************************/

BOOL 
SCRNSAVE::SetupInitialWindows()
{
    // Create the windows

    if( ! CreateInitialWindows() ) {
        SS_WARNING( "SCRNSAVE:: Couldn't create windows\n" );
        return FALSE;
    }

    // Initial window clear

#ifdef SS_INITIAL_MAIN_WINDOW_CLEAR
    if( type == SS_TYPE_PREVIEW ) {
    // Make sure the screen is cleared to black before we start drawing
    // anything, as sometimes the background WM_PAINT doesn't get to us right
    // away.  This is only a problem in preview mode
       psswMain->GdiClear();
    }
#endif

    // Configure and Init the windows, if applicable

#ifdef SS_DELAYED_START_KLUGE
    // delay start for some configurations
    if( ! SS_DELAY_START(type) ) {
        SendMessage( psswMain->hwnd, SS_WM_INITGL, 0, 0 );
    }
#else
    SendMessage( psswMain->hwnd, SS_WM_INITGL, 0, 0 );
#endif // SS_DELAYED_START_KLUGE

    return TRUE;
}



/**************************************************************************\
* CreateInitialWindows
*
* Create the intitial set of windows.
*
\**************************************************************************/

BOOL 
SCRNSAVE::CreateInitialWindows()
{
    PSSW    pssw;

    if( !pssc )
        return FALSE;

    // Handle any request for resolution change

#define SS_RESOLUTION_SWITCH 1
#ifdef SS_RESOLUTION_SWITCH
    if( pssc->bStretch && (type == SS_TYPE_FULLSCREEN) 
        && ss_fOnWin95() ) {

        STRETCH_INFO *pStretch = &pssc->stretchInfo;
        ISIZE newSize;

        // Try and change screen resolution to match stretch size
        bResSwitch = AttemptResolutionSwitch( pStretch->baseWidth, 
                                               pStretch->baseHeight, &newSize ); 
        // Is stretching still necessary if resolution changed ?
        if( bResSwitch ) {
            if( (newSize.width == pStretch->baseWidth) &&
                (newSize.height == pStretch->baseHeight) ) 
                // exact match, no stretching now necessary
                pssc->bStretch = FALSE;
        }
    }
#endif

    // Currently the bitmaps used in stretch mode don't support palette
    // messages, so disable any stretching when in PREVIEW mode (where we
    // need to support palette interaction).
    // actually this is only a consideration in 8-bit mode...

    if( (type == SS_TYPE_PREVIEW) && pssc->bStretch )
        pssc->bStretch = FALSE;

    // Create the main ss window

    if( ! CreateMainWindow() )
        return FALSE;

#ifdef SS_INITIAL_CLEAR
    // If main window is transparent, can do an initial clear here before
    // any other windows are created or palettes modified
    // This is bogus on NT, as system switches to secure desktop when screen
    // saver kicks in automatically.
    InitialClear( pssw );
#endif

    // For now, simple window environment is described by pssc, so things
    // like bFloater and bStretch are mutually exclusive.

    SS_GL_CONFIG *pGLc = &gpss->GLc;

    if( pssc->bFloater ) {

        if( !(pssw = CreateChildWindow( &pssc->floaterInfo )) )
            return FALSE;

        pssw->pGLc = pGLc;
        psswGL = pssw; // support old-style
    } else {
        psswMain->pGLc = pGLc;
        psswGL = psswMain; // support old-style
    }

    return TRUE;
}

/**************************************************************************\
* NormalWindowScreenSaverProc
*
* Highest level window proc, used only in normal window (/w) mode.
*
\**************************************************************************/

LRESULT WINAPI
NormalWindowScreenSaverProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   switch( uMsg )
   {
      case WM_SETTEXT:
         // RealScreenSaverProc won't allow this - bypass it
         return ScreenSaverProc( hWnd, uMsg, wParam, lParam );

      case WM_CHAR:
            if( KeyDownFunc ) {
                int key = wParam;
                (*KeyDownFunc)(key);
            }
            break;
                    
      case WM_KEYDOWN:
            if( wParam == VK_ESCAPE ) {
                PostMessage( hWnd, WM_CLOSE, 0, 0l );
                break;
            } else if( KeyDownFunc ) {
                (*KeyDownFunc)(wParam);
                return 0;
            }
            return 0; // ??

      default:
         break;
   }

   return RealScreenSaverProc( hWnd, uMsg, wParam, lParam );
}

/**************************************************************************\
* DoScreenSave
*
* Hooked out version of DoScreenSave in standard scrnsave.c
*
\**************************************************************************/

int
DoScreenSave( HWND hwndParent )
{
    return GLDoScreenSave( hwndParent ? SS_TYPE_PREVIEW : SS_TYPE_FULLSCREEN, 
                           (LPARAM) hwndParent );
}

/**************************************************************************\
* DoWindowedScreenSave
*
* Called when screen saver invoked with /w (window mode) parameter
*
\**************************************************************************/

int
DoWindowedScreenSave( LPCTSTR szArgs )
{
    return GLDoScreenSave( SS_TYPE_NORMAL, (LPARAM) szArgs );
}

/**************************************************************************\
* DoConfigBox
*
* Hooked out version of DoConfigBox in standard scrnsave.c
*
\**************************************************************************/

int
DoConfigBox( HWND hwndParent )
{
    // let the consumer register any special controls for the dialog
    if( !RegisterDialogClasses( hMainInstance ) )
        return FALSE;

    // Create screen saver instance
    SCRNSAVE ss( SS_TYPE_CONFIG );

    int retVal = DialogBox( hMainInstance, 
                           MAKEINTRESOURCE( DLG_SCRNSAVECONFIGURE ),
                           hwndParent, (DLGPROC)GLScreenSaverConfigureDialog );

    return retVal;
}

/**************************************************************************\
* CreateMainWindow
*
* Creates main screen saver window based on the window type
*
\**************************************************************************/

BOOL
SCRNSAVE::CreateMainWindow()
{
    WNDPROC  wndProc;
    int      nCx, nCy;
    ISIZE    size;
    UINT     uStyle = 0;
    UINT     uExStyle = 0;
    IPOINT2D pos;
    LPCTSTR  pszWindowTitle;
    HCURSOR  hCursor = NULL;
    HBRUSH   hbrBgMain;
    PSSW     pssw;
    HWND    hwndParent = NULL;
   
    wndProc = RealScreenSaverProc;

    switch( type ) {
        case SS_TYPE_FULLSCREEN:
          {
            HWND hOther;

            nCx = GetSystemMetrics( SM_CXSCREEN );
            nCy = GetSystemMetrics( SM_CYSCREEN );
#ifdef SS_DEBUG
//#define SS_FULLSCREEN_DEBUG 1
#ifdef SS_FULLSCREEN_DEBUG
            // Reduce window size so we can see debugger
            nCx /= 2;
            nCy /= 2;
#endif
#endif
            uStyle = WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
            uExStyle = WS_EX_TOPMOST;

            pszWindowTitle = TEXT("Screen Saver"); // MUST differ from preview

            // if there is another NORMAL screen save instance, switch to it
            hOther = FindWindow( szClassName, pszWindowTitle );

            if( hOther && IsWindow( hOther ) )
            {
               SetForegroundWindow( hOther );
               return FALSE;
            }

            InitRealScreenSave();
          }
          break;

        case SS_TYPE_PREVIEW:
          {
            RECT rcParent;

            hwndParent = (HWND) initParam;

            GetClientRect( hwndParent, &rcParent );
    
            fChildPreview = TRUE;
            nCx = rcParent.right;
            nCy = rcParent.bottom;
            uStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;

            pszWindowTitle = TEXT("Preview");   // MUST differ from full screen
          }
          break;

        case SS_TYPE_NORMAL:

          {
            // We set fChildPreview even when we're running in a normal window,
            // as this flag is used in scrnsave.c to differentiate from full
            // screen.
            fChildPreview = TRUE;

            // init size to half screen by default
            nCx = GetSystemMetrics( SM_CXSCREEN ) >> 1;
            nCy = GetSystemMetrics( SM_CYSCREEN ) >> 1;

            if( initParam ) {
                // get size of window from args
                LPCTSTR szArgs = (LPCTSTR) initParam;
            }
                
            LoadString(hMainInstance, IDS_DESCRIPTION, szScreenSaverTitle, 
               sizeof(szScreenSaverTitle) / sizeof(TCHAR));
            pszWindowTitle = szScreenSaverTitle; // MUST differ from preview

            uStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | 
                   WS_CLIPSIBLINGS;

            hCursor = LoadCursor( NULL, IDC_ARROW );

            // Wrap RealScreenSaverProc
            wndProc = NormalWindowScreenSaverProc;
          }
          break;

        default:
          break;
    }

    size.width = nCx;
    size.height = nCy;
#ifdef SS_INITIAL_CLEAR
    hbrBgMain = NULL;
#else
    hbrBgMain = hbrBg;
#endif

    pos.x = pos.y = 0;

    // Create SSW window wrapper

    pssw = new SSW( NULL,       // parent
                    &size, 
                    &pos, 
                    FALSE,      // bMotion
                    NULL       // ChildSizeFunc
                  );
    if( !pssw )
        return FALSE;

    // Initialize the window class and create the window

    if( !RegisterMainClass
        (
            wndProc, 
            hbrBgMain, 
            hCursor
        ) ||

        !pssw->CreateSSWindow
        (   
            hMainInstance, 
            uStyle, 
            uExStyle, 
            szScreenSaverTitle,
            wndProc,
            pszWindowClass,
            hwndParent     // mf: ! hwndParentOverride
        ) )
    {
        delete pssw;
        return FALSE;
    }

    if( type != SS_TYPE_PREVIEW )
#ifndef SS_DEBUG
        SetForegroundWindow( pssw->hwnd );
#else
    {
        if( !SetForegroundWindow( pssw->hwnd ) )
            SS_DBGPRINT( "Main_Proc: SetForegroundWindow failed\n" );
    }
#endif

    // Always configure the main window for gdi
    pssw->ConfigureForGdi();

    psswMain = pssw;
    return TRUE;
}

/**************************************************************************\
* CreateChildWindow
*
* Creates a child window of the parent window
*
* This is a kind of wrapper-constructor
\**************************************************************************/

PSSW
SCRNSAVE::CreateChildWindow( FLOATER_INFO *pFloater )
{
    pFloater->bSubWindow = FALSE; // default is no logical subwin's

#ifdef SS_WIN95_MULTIWINDOW_WORKAROUND
    if( ss_fOnWin95() && ss_fFullScreenMode() && bPowerSaveEnabled() ) {
        // Make the floater a logical sub-window of the main window, not a
        // separate window.  This works around a USER bug where the power 
        // save feature was not getting activated for multi-windowed screen
        // savers
        SS_DBGINFO( "SCRNSAVE::CreateChildWindow : Using logical sub-windows\n" );
        pFloater->bSubWindow = TRUE;
    }
#endif // SS_WIN95_MULTIWINDOW_WORKAROUND

    // Register child window class
    // This only has to be done once, since so far, all child window
    // classes are the same
    if( !pFloater->bSubWindow && !RegisterChildClass() )
        return NULL;

    return CreateChildWindow( psswMain, pFloater );
}

PSSW
SCRNSAVE::CreateChildWindow( PSSW psswParent, FLOATER_INFO *pFloater )
{
    UINT     uStyle = 0;
    UINT     uExStyle = 0;
    PSSW     pssw;

    uStyle = WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    // Size and position are NULL here, as the SSW will call the size
    // function callback in pFloater to get these and other values

    // Create the SSW window wrapper

    pssw = new SSW( psswParent,      // parent
                    NULL,            // size
                    NULL,            // position
                    pFloater->bMotion,
                    pFloater->ChildSizeFunc
                  );
    if( !pssw )
        return NULL;

    if( pFloater->bSubWindow )
        // Don't need to create win32 window
        return pssw;

    // Create a window

    if( !pssw->CreateSSWindow
        (
            hMainInstance, 
            uStyle,
            0,                      // uExStyle 
            szScreenSaverTitle ,
            SS_ScreenSaverProc,
            szClassName,
            NULL                    // hwndParentOverride
        ) )
    {
        delete pssw;
        return NULL;
    }

    return pssw;
}


/**************************************************************************\
* RegisterMainClass
*
* Registers class of the main SS window
\**************************************************************************/

static BOOL 
RegisterMainClass( WNDPROC wndProc, HBRUSH hbrBg, HCURSOR hCursor )
{
    WNDCLASS cls;

    cls.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_OWNDC;
    cls.lpfnWndProc = wndProc;
    cls.cbClsExtra = 0;
    cls.cbWndExtra = 0;
    cls.hInstance = hMainInstance;
    cls.hIcon = LoadIcon( hMainInstance, MAKEINTATOM( ID_APP ) );
    cls.hCursor = hCursor;
    cls.hbrBackground = hbrBg;
    cls.lpszMenuName = (LPTSTR)NULL;
    cls.lpszClassName = (LPTSTR)pszWindowClass;
    return RegisterClass(&cls);
}

/**************************************************************************\
* RegisterChildClass
*
* Registers class of a standard child window
\**************************************************************************/

static BOOL 
RegisterChildClass()
{
    static BOOL bRegistered = FALSE;

    if( bRegistered )
        return TRUE;

    WNDCLASS cls;

    cls.style = CS_VREDRAW | CS_HREDRAW;
    cls.lpfnWndProc = SS_ScreenSaverProc;
    cls.cbClsExtra = 0;
    cls.cbWndExtra = 0;
    cls.hInstance = hMainInstance;
    cls.hIcon = NULL;
    cls.hCursor = NULL;
    cls.hbrBackground = NULL;
    cls.lpszMenuName = (LPTSTR)NULL;
    cls.lpszClassName = (LPTSTR)szClassName;

    if( !RegisterClass(&cls) )
        return FALSE;

    // success
    bRegistered = TRUE;
    return TRUE;
}

/******************************Public*Routine******************************\
* AttemptResolutionSwitch
*
* Try doing resolution switching to match or get close to the desired size.
*
\**************************************************************************/

static BOOL
AttemptResolutionSwitch( int width, int height, ISIZE *pNewSize ) 
{
    BOOL bChanged = FALSE;

    // Try doing resolution switching to match or get close to the
    // desired width and height

    // Try switching to requested size
    if( 0 ) {
        // for now force failure of user request and try standard 640x480
        bChanged = TRUE;
    } else {
        // Can't switch to requested size, try for best match
        width = 640; 
        height = 480;
        // If screen already this size or less, leave be
        if( (GetSystemMetrics( SM_CXSCREEN ) <= width) &&
            (GetSystemMetrics( SM_CYSCREEN ) <= height) )
            return FALSE;

        // use this when trying for best match
        // ss_QueryDisplaySettings();

        if( ss_ChangeDisplaySettings( width, height, 0 ) )
            bChanged = TRUE;
    }

    if( bChanged ) {
        pNewSize->width = width;
        pNewSize->height = height;
    }
    return bChanged;
}

#ifdef SS_INITIAL_CLEAR
static void
InitialClear( PSSW *pssw )
{
    ss_GdiRectWipeClear( pssw->hwnd, pssw->size.width, pssw->size.height );
}
#endif // SS_INITIAL_CLEAR

/**************************************************************************\
* CloseWindows
*
* Close down any open windows.
*
* This sends a WM_CLOSE message to the top-level window if it is still open.  If
* the window has any children, they are also closed.  For each window, the
* SSW destructor is called.
\**************************************************************************/

void
SCRNSAVE::CloseWindows()
{
    if( psswMain ) {
        if( psswMain->bOwnWindow )
            DestroyWindow( psswMain->hwnd );
        else
            delete psswMain;
    }
}

/**************************************************************************\
* SCRNSAVE destructor
*
\**************************************************************************/

SCRNSAVE::~SCRNSAVE()
{
    // Close any open windows (there might be some open if errors occurred)
    CloseWindows();

#ifdef SS_WIN95_DESKTOP_REDRAW_FIX
    // On win95, sometimes the desktop is drawn incorrectly after we have
    // taken over the palette
    if( flags & SS_PALETTE_TAKEOVER ) {
        SS_DBGINFO( "SCRNSAVE destructor: Redrawing desktop\n" );
        ss_RedrawDesktop();
    }
#endif

    if( bResSwitch ) {
        // Restore previous display settings
        ChangeDisplaySettings(NULL, CDS_FULLSCREEN);
    }

    gpss = NULL;
}


#ifdef SS_WIN95_MULTIWINDOW_WORKAROUND

// Macro to convert string to integer
#ifdef UNICODE
#define sztoi( pszStr ) _wtoi( (wchar_t *) (pszStr) )
#else
#define sztoi( pszStr ) atoi( (char *) (pszStr) )
#endif

/**************************************************************************\
* GetRegValueBool
*
* Get registry value and convert to BOOL
\**************************************************************************/

#define SS_REG_BUF_SIZE 30

static BOOL
bGetRegValueBool( HKEY hKey, LPTSTR szValue, BOOL *pbEnabled )
{
    TCHAR szBuf[SS_REG_BUF_SIZE];
    DWORD dataSize = SS_REG_BUF_SIZE;
    DWORD dwType;

    // Get the value

    if( ! ( RegQueryValueEx( 
              hKey, 
              szValue,
              (LPDWORD) NULL,
              (LPDWORD) &dwType,
              (LPBYTE) szBuf,
              (LPDWORD) &dataSize ) == ERROR_SUCCESS
          ) 
      )
        return FALSE;

    // Convert value to BOOL (the expected type is REG_SZ)

    switch( dwType ) {
        case REG_SZ :
            // Convert string to BOOL
            *pbEnabled = (BOOL) sztoi( szBuf );
            break;
    
        case REG_DWORD :
            // Handle numerical value
            *pbEnabled = (BOOL) *( (LPDWORD) szBuf );
            break;

        default:
            SS_WARNING( "GetRegValueBool : Unexpected type\n" );
            return FALSE;
    }

    return TRUE;
}

/**************************************************************************\
* bPowerSaveEnabled
* 
* Detect if power saving is enabled by checking registry values.
*
\**************************************************************************/

static BOOL
bPowerSaveEnabled()
{
    BOOL bLowPowerActive;
    BOOL bPowerOffActive;
    HKEY hKey;

    // Open desktop key

    if( ! ( RegOpenKeyEx( 
              HKEY_CURRENT_USER, 
              (LPCTSTR) TEXT( "Control Panel\\desktop"),
              0, 
              KEY_QUERY_VALUE, 
              &hKey  ) == ERROR_SUCCESS 
          )
      ) 
    {
        return FALSE;  // Can't open desktop key
    }

    // Check values - power saving is enabled if either ScreenSaveLowPowerActive
    // or ScreenSavePowerOffActive is enabled.

    if( (
          bGetRegValueBool( 
            hKey,
            (LPTSTR) TEXT("ScreenSaveLowPowerActive"), 
            &bLowPowerActive ) 
          &&
          bLowPowerActive
        ) 
        ||
        (
          bGetRegValueBool(
            hKey,
            (LPTSTR) TEXT("ScreenSavePowerOffActive"), 
            &bPowerOffActive ) 
          &&
          bPowerOffActive
        ) 
      ) 
    {
        return TRUE;
    }

    return FALSE;
}
#endif // SS_WIN95_MULTIWINDOW_WORKAROUND
